//
// Created by faywong on 2020/4/4.
//

#include "MarkdownPreviewWebPage.h"

#include <QDesktopServices>

bool MarkdownPreviewWebPage::acceptNavigationRequest(const QUrl &url, QWebEnginePage::NavigationType type,
                                                     bool isMainFrame) {
    if (type == NavigationTypeLinkClicked) {
        QDesktopServices::openUrl(url);
        return false;
    } else {
        // qDebug()<<"url: " << url;
        return QWebEnginePage::acceptNavigationRequest(url, type, isMainFrame);
    }
}

MarkdownPreviewWebPage::MarkdownPreviewWebPage(QWebEngineProfile *profile, QObject *parent) : QWebEnginePage(profile,
                                                                                                             parent) {}
