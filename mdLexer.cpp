#include "mdLexer.h"

MdLexer::MdLexer(QObject *parent) : QsciLexerMarkdown(parent) {

}

// Returns the foreground colour of the text for a style.
QColor MdLexer::defaultColor(int style) const
{

    switch (style)
    {
    case Special:
        return QColor(0xcc, 0x00, 0xff);

    case StrongEmphasisAsterisks:
    case StrongEmphasisUnderscores:
        return aqua;

    case EmphasisAsterisks:
    case EmphasisUnderscores:
        return aqua;

        // violet
    case Header1:
        return green_106;

    case Header2:
        return green_100;

    case Header3:
        return yellow_172;

    case Header4:
        return yellow_136;

    case Header5:
        return QColor(0x77, 0x33, 0x00);

    case Header6:
        return QColor(0x55, 0x22, 0x00);

    case Prechar:
        return QColor(0x00, 0x00, 0x00);

    case UnorderedListItem:
    case OrderedListItem:
        return red;

    case BlockQuote:
        return QColor(0x00, 0x66, 0x00);

    case StrikeOut:
        return QColor(0xdd, 0xdd, 0xdd);

    case HorizontalRule:
        return QColor(0x1f, 0x1c, 0x1b);

    case Link:
        return green_106;

    case CodeBackticks:
    case CodeDoubleBackticks:
        return QColor(0x7f, 0x00, 0x7f);

    case CodeBlock:
        return QColor(0x00, 0x45, 0x8a);
    }

    return QsciLexer::defaultColor(style);
}


// Returns the font of the text for a style.
QFont MdLexer::defaultFont(int style) const
{
    QFont f;

    switch (style)
    {
    case StrongEmphasisAsterisks:
    case StrongEmphasisUnderscores:
        f = QsciLexer::defaultFont(style);
        f.setBold(true);
        break;

    case EmphasisAsterisks:
    case EmphasisUnderscores:
        f = QsciLexer::defaultFont(style);
        f.setItalic(true);
        break;

    case Header1:
    case Header2:
    case Header3:
    case Header4:
    case Header5:
    case Header6:
#if defined(Q_OS_WIN)
        f = QFont("Courier New", 10);
#elif defined(Q_OS_MAC)
        f = QFont("Courier", 12);
#else
        f = QFont("Source Code Pro", 9);
#endif
        f.setBold(true);
        break;

    case HorizontalRule:
    case CodeBackticks:
    case CodeDoubleBackticks:
    case CodeBlock:
#if defined(Q_OS_WIN)
        f = QFont("Courier New", 10);
#elif defined(Q_OS_MAC)
        f = QFont("Courier", 12);
#else
        f = QFont("Source Code Pro", 9);
#endif
        break;

    case Link:
        f = QsciLexer::defaultFont(style);
        f.setUnderline(true);
        break;

    default:
        f = QsciLexer::defaultFont(style);
    }

    return f;
}


// Returns the background colour of the text for a style.
QColor MdLexer::defaultPaper(int style) const
{
    switch (style)
    {
    case Prechar:
        return QColor(0xee, 0xee, 0xaa);

    case UnorderedListItem:
    case OrderedListItem:
        return bg0;

    case BlockQuote:
        return QColor(0xcb, 0xdc, 0xcb);

    case StrikeOut:
        return red;

    case HorizontalRule:
        return QColor(0xe7, 0xd1, 0xc9);

    case CodeBackticks:
    case CodeDoubleBackticks:
    case CodeBlock:
    case StrongEmphasisAsterisks:
    case StrongEmphasisUnderscores:
    case EmphasisAsterisks:
    case EmphasisUnderscores:
        return bg0_s;
    }

    return bg;
}

