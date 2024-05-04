#ifndef MDLEXER_H
#define MDLEXER_H

#include <Qsci/qscilexermarkdown.h>

class MdLexer : public QsciLexerMarkdown
{
    Q_OBJECT
public:
    MdLexer(QObject *parent = 0);
    //! Returns the foreground colour of the text for style number \a style.
    //!
    //! \sa defaultPaper()
    QColor defaultColor(int style) const;

    //! Returns the font for style number \a style.
    QFont defaultFont(int style) const;

    //! Returns the background colour of the text for style number \a style.
    //!
    //! \sa defaultColor()
    QColor defaultPaper(int style) const;

private:
    // gruvbox light mode:
    QColor bg = QColor("#fbf1c7");
    QColor red = QColor("#cc241d");
    QColor green_106 = QColor("#98971a");
    QColor green_100 = QColor("#79740e");
    QColor yellow_172 = QColor("#d79921");
    QColor yellow_136 = QColor("#b57614");
    QColor blue = QColor("#458588");
    QColor purple = QColor("#b16286");
    QColor aqua = QColor("#689d6a");
    QColor gray = QColor("#7c6f64");
    QColor orange = QColor("#d65d0e");
    QColor fg0 = QColor("#282828");
    QColor fg1 = QColor("#3c3836");
    QColor fg2 = QColor("#504945");
    QColor fg3 = QColor("#665c54");

    QColor bg0 = bg;
    QColor bg1 = QColor("#ebdbb2");
    QColor bg2 = QColor("#d5c4a1s");
    QColor bg0_s = QColor("#f2e5bc");

};

#endif // MDLEXER_H
