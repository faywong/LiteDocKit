/* PEG Markdown Highlight
 *
 * highlighter.cpp
 *
 * refined by sid
 */

#include <QtGui>
#include <QtConcurrent>
#include <QApplication>
#include <QDesktopWidget>
#include <algorithm>
#include <settings/settings_def.h>
#include "highlighter.h"
#include "scidown_md.h"

HGMarkdownHighlighter::HGMarkdownHighlighter(QsciScintilla *parent,
                                             DMEditorDelegate *mainWindow,
                                             int aWaitInterval) : QObject(parent),
    mainWin(mainWindow),
    document(parent),
    highlightingStyles(nullptr),
    waitInterval(aWaitInterval),
    postfixRe("(\"|\\n|\\r|#|-|=|\")*")
{
    threadPool.setMaxThreadCount(1);
    timer = new QTimer(this);
    timer->setSingleShot(true);
    timer->setInterval(aWaitInterval);
    connect(timer, SIGNAL(timeout()), this, SLOT(timerTimeout()));
    connect(document, SIGNAL(textChanged()),
            this, SLOT(handleContentsChange()));

    this->parse();
}

void HGMarkdownHighlighter::setStyles(QVector<HighlightingStyle> &styles)
{
    this->highlightingStyles = &styles;
}

#define STY(type, format) styles->append(HighlightingStyle{type, format})
void HGMarkdownHighlighter::setDefaultStyles()
{
    QVector<HighlightingStyle> *styles = new QVector<HighlightingStyle>();

    QTextCharFormat headers;
    headers.setForeground(QBrush(QColor("#68489A")));
    // 遵循 css 默认 size
    headers.setFontWeight(QFont::Normal);

    headers.setToolTip(tr("H1"));
    STY(pmh_H1, headers);

    headers.setToolTip(tr("H2"));
    STY(pmh_H2, headers);

    headers.setToolTip(tr("H3"));
    STY(pmh_H3, headers);

    headers.setToolTip(tr("H4"));
    STY(pmh_H4, headers);

    headers.setToolTip(tr("H5"));
    STY(pmh_H5, headers);

    headers.setToolTip(tr("H6"));
    STY(pmh_H6, headers);

    QTextCharFormat hrule;
    hrule.setForeground(QBrush(QColor("#5E6E5E")));
    STY(pmh_HRULE, hrule);
    hrule.setToolTip(tr("horizontal line"));

    QTextCharFormat list;
    list.setForeground(QBrush(QColor("#E6193C")));
    STY(pmh_LIST_BULLET, list);
    STY(pmh_LIST_ENUMERATOR, list);
    list.setToolTip(tr("list"));

    QTextCharFormat link;
    link.setForeground(QBrush(QColor("#54C691")));
    link.setUnderlineStyle(QTextCharFormat::SingleUnderline);
    link.setUnderlineColor(QColor("#54C691"));
    STY(pmh_LINK, link);
    STY(pmh_AUTO_LINK_URL, link);
    STY(pmh_AUTO_LINK_EMAIL, link);
    link.setToolTip(tr("link"));

    QTextCharFormat image;
    image.setForeground(QBrush(QColor("#54C691")));
    STY(pmh_IMAGE, image);
    image.setToolTip(tr("image"));

    QTextCharFormat ref;
    ref.setForeground(QBrush(QColor("#87711D")));
    STY(pmh_REFERENCE, ref);
    ref.setToolTip(tr("reference"));

    QTextCharFormat code;
    code.setBackground(QBrush(QColor("#66AAAAAA")));
    // `abc` ```javascript ... 这种
    STY(pmh_CODE, code);
    STY(pmh_VERBATIM, code);
    code.setToolTip(tr("code/verbratim"));

    /* Emphasis, aka italics, with *asterisks* or _underscores_. */
    QTextCharFormat emph;
    emph.setForeground(QBrush(QColor("#AD2BEE")));
    emph.setFontItalic(true);
    emph.setFontWeight(QFont::Bold);
    emph.setToolTip(tr("Emphasis"));
    STY(pmh_EMPH, emph);

    /* Strong emphasis, aka bold, with **asterisks** or __underscores__. */
    QTextCharFormat strong;
    strong.setForeground(QBrush(QColor("#3F4777")));
    strong.setFontWeight(QFont::Bold);
    STY(pmh_STRONG, strong);
    strong.setToolTip(tr("strong"));

    QTextCharFormat comment;
    comment.setForeground(QBrush(QColor("#809980")));
    STY(pmh_COMMENT, comment);
    comment.setToolTip(tr("comment"));

    QTextCharFormat blockquote;
    blockquote.setForeground(QBrush(QColor("#87711D")));
    blockquote.setBackground(QBrush(QColor("#F0F0F0")));
    STY(pmh_BLOCKQUOTE, blockquote);
    blockquote.setToolTip(tr("blockquote"));

    QTextCharFormat note;
    note.setForeground(QBrush(QColor("#AD2BEE")));
    note.setBackground(QBrush(QColor("#F0F0F0")));
    note.setToolTip(tr("note"));

    STY(pmh_NOTE, note);

    QTextCharFormat strike;
    strike.setForeground(QBrush(QColor("#D55B7B")));
    strike.setFontStrikeOut(true);
    strike.setFontWeight(QFont::Bold);
    strike.setToolTip(tr("strike"));
    STY(pmh_STRIKE, strike);

    QTextCharFormat htmlBlock;
    htmlBlock.setForeground(QBrush(QColor("#E6193C")));
    htmlBlock.setBackground(QBrush(QColor("#CFE8CF")));
    htmlBlock.setToolTip(tr("html block"));

    STY(pmh_HTMLBLOCK, htmlBlock);
    this->setStyles(*styles);
}

void HGMarkdownHighlighter::clearFormatting()
{
    // QTextBlock block = document->firstBlock();
    // while (block.isValid()) {
    //     block.layout()->clearAdditionalFormats();
    //     block = block.next();
    // }
}

class tok {
public:
    tok(pmh_element_type t, unsigned long p, int l) : type(t), pos(p), len(l) {}
    pmh_element_type type;    /**< \brief Type of element */
    unsigned long pos;
    int len;
};

bool operator<(const tok & lhs, const tok & rhs)
{
    return lhs.pos < rhs.pos;
}

void HGMarkdownHighlighter::highlight(pmh_element **parsedElement)
{
//     if (parsedElement == nullptr) {
//         qDebug() << "parsedElement is NULL";
//         return;
//     }

//     if (highlightingStyles == nullptr) {
//         this->setDefaultStyles();
//     }

//     this->clearFormatting();

//     QString textContent = document->toRawText();
//     std::vector<tok> tocs;
//     QStringList referredImages;

//     for (int i = 0; i < highlightingStyles->size(); i++)
//     {
//         HighlightingStyle style = highlightingStyles->at(i);
//         pmh_element *elem_cursor = parsedElement[style.type];

//         bool isTocElement = (style.type >= pmh_H1 && style.type <= pmh_H6);

//         while (elem_cursor != NULL)
//         {
//             if (elem_cursor->end <= elem_cursor->pos) {
//                 elem_cursor = elem_cursor->next;
//                 continue;
//             }

//             if (style.type == pmh_IMAGE) {
//                 referredImages << elem_cursor->address;
//             }

//             if (isTocElement) {
//                 tok t(elem_cursor->type, elem_cursor->pos, (int)(elem_cursor->end - elem_cursor->pos));
//                 tocs.push_back(t);
//             }

//             // "The QTextLayout object can only be modified from the
//             // documentChanged implementation of a QAbstractTextDocumentLayout
//             // subclass. Any changes applied from the outside cause undefined
//             // behavior." -- we are breaking this rule here. There might be
//             // a better (more correct) way to do this.

//             int startBlockNum = document->findBlock(elem_cursor->pos).blockNumber();
//             int endBlockNum = document->findBlock(elem_cursor->end).blockNumber();
//             for (int j = startBlockNum; j <= endBlockNum; j++)
//             {
//                 QTextBlock block = document->findBlockByNumber(j);
//                 QTextLayout *layout = block.layout();
//                 QList<QTextLayout::FormatRange> list = layout->additionalFormats();
//                 int blockpos = block.position();
//                 QTextLayout::FormatRange r;
//                 r.format = style.format;

//                 if (j == startBlockNum) {
//                     r.start = elem_cursor->pos - blockpos;
//                     r.length = (startBlockNum == endBlockNum)
//                                 ? elem_cursor->end - elem_cursor->pos
//                                 : block.length() - r.start;
//                 } else if (j == endBlockNum) {
//                     r.start = 0;
//                     r.length = elem_cursor->end - blockpos;
//                 } else {
//                     r.start = 0;
//                     r.length = block.length();
//                 }

//                 list.append(r);
//                 layout->setAdditionalFormats(list);
//             }

//             elem_cursor = elem_cursor->next;
//         }
//     }

//     std::stable_sort(tocs.begin(), tocs.end());
//     QVector<QStandardItem*> tocItems;
//     QVector<QStandardItem*> flatTocItems;

//     for (std::vector<tok>::size_type i = 0; i < tocs.size(); i++) {
//         tok t = tocs[i];
//         int j = i;
//         QString keyword = textContent.mid(t.pos, t.len);
// //        qDebug()<<"keyword: " <<keyword;
//         keyword = keyword.replace(postfixRe, "");
//         QStandardItem *item = new QStandardItem(keyword);
//         const int posRole = Qt::UserRole + 1;
//         const int typeRole = Qt::UserRole + 2;
//         item->setData((qulonglong)t.pos, posRole);
//         item->setData((qint32)t.type, typeRole);

//         bool parentFound = false;
//         while (--j >= 0) {
//             if (tocs[j].type < t.type) { // 有更高级别的标题
//                 flatTocItems[j]->appendRow(item);
//                 parentFound = true;
//                 break;
//             }
//         }
//         if (!parentFound) {
//             tocItems.push_back(item);
//         }
//         flatTocItems.push_back(item);
//     }

//     mainWin->updateToc(tocItems);
//     document->markContentsDirty(0, document->characterCount());
//     pmh_free_elements(parsedElement);
}

void HGMarkdownHighlighter::parse()
{
    threadPool.clear();
    parseTaskFuture.cancel();
    toMdTaskFuture.cancel();

    QString content = document->text();
    if (content.length() > 1) {
        parseTaskFuture = QtConcurrent::run(&threadPool, [=]() {
            // QByteArray ba = content.toUtf8();
            // char *contentCString = (char *)ba.data();
            // pmh_element **result;
            // pmh_markdown_to_elements(contentCString, pmh_EXT_NOTES | pmh_EXT_STRIKE, &result);
            // emit parseFinished(result);
        });

        toMdTaskFuture = QtConcurrent::run(&threadPool, [=]() {
            QByteArray ba = content.toUtf8();
            char *contentCString = (char *)ba.data();
            uint8_t* output_data = nullptr;
            size_t output_size = 0;
            md2html(reinterpret_cast<const uint8_t *>(contentCString), ba.size(), &output_data, &output_size, QApplication::desktop()->screenGeometry().height());
            QString html = QString::fromUtf8(reinterpret_cast<const char *>(output_data), output_size);
            emit md2htmlFinished(html);
            free(reinterpret_cast<void *>(output_data));
        });
    }
}

void HGMarkdownHighlighter::handleContentsChange()
{
//    qDebug() << "contents changed. chars removed/added:" << charsRemoved << charsAdded;
    timer->stop();
    timer->start();
}

void HGMarkdownHighlighter::timerTimeout()
{
    qDebug() << "timerTimeout:";
    this->parse();
}
