/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.13.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#define leftPanel_INDEX 0
#define mdEditor_INDEX 1
#define mdPreviewer_INDEX 2
#include <QWebEngineProfile>
#include <QWebChannel>

int gSplitWeights[] = {
        1,
        3,
        3
};

#include "FileIconProvider.h"
#include "MarkdownPreviewWebPage.h"
#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QFileSystemModel>
#include <QHBoxLayout>
#include <QTreeView>
#include <QFileIconProvider>
#include <QDesktopWidget>
#include <qmarkdowntextedit.h>
#include <QSplitter>
#include <QScreen>
#include <QMetaObject>
#include <QLabel>
#include <QScrollArea>
#include <QStandardItemModel>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QScrollBar>
#include <QtWebEngineWidgets/QWebEngineView>
#include <QWebEngineSettings>
#include "settings/settings_def.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{

public:
    QMarkdownTextEdit *markdownEditor;
    QWebEngineView *markdownPreviewView;
    QMenuBar *menuBar;
//    QStatusBar *statusBar;
    QHBoxLayout *bodyLayout;
    QSplitter *splitter;
    QTreeView *fileTree;
    QTreeView *tocTree;
    QStandardItemModel *tocModel;
    QFileSystemModel *fileTreeModel;
    // image viewer part
    QLabel *imageLabel;
    void setupUi(QMainWindow *mainWindow)
    {
        if (mainWindow->objectName().isEmpty())
            mainWindow->setObjectName(QString::fromUtf8("MainWindow"));

        int lastWidth = SyncFolderSettings::getInt(KEY_LAST_WIN_WIDTH);
        int lastHeight = SyncFolderSettings::getInt(KEY_LAST_WIN_HEIGHT);

        QScreen *screen = QGuiApplication::primaryScreen();
        QRect screenGeometry = screen->geometry();
        int windowWidth = screenGeometry.width();
        int windowHeight = screenGeometry.height();

        if (lastWidth > 0 && lastHeight > 0) {
            windowWidth = lastWidth;
            windowHeight = lastHeight;
        } else {
            float factor = windowWidth >= 1920 ? 0.618f : 0.8f;
            if (windowWidth <= 800) {
                factor = 1.0f;
            }
            windowWidth *= factor;
            windowHeight *= factor;
            SyncFolderSettings::setInt(KEY_LAST_WIN_WIDTH, windowWidth);
            SyncFolderSettings::setInt(KEY_LAST_WIN_HEIGHT, windowHeight);
        }

        // TODO: handle window size changed
        mainWindow->resize(windowWidth, windowHeight);

        bodyLayout = new QHBoxLayout;
//        +----------------------------------------------------------+
//        |                 splitter                                 |
//        |    +---------------------+    +------------------------+ |
//        |    |leftPanelSplitter    |    |     markdownEditor     | |
//        |    | +-----------------+ |    |                        | |
//        |    | | fileTree        | |    |                        | |
//        |    | |                 | |    |                        | |
//        |    | +-----------------+ |    |                        | |
//        |    | +-----------------+ |    |                        | |
//        |    | | tocTree         | |    |                        | |
//        |    | |                 | |    |                        | |
//        |    | +-----------------+ |    |                        | |
//        |    |                     |    |                        | |
//        |    +---------------------+    +------------------------+ |
//        |                                                          |
//        +----------------------------------------------------------+
        // 横向 flex 布局，最顶层的 splitter
        splitter = new QSplitter;
        splitter->setHandleWidth(0);
        fileTree = new QTreeView;
        tocTree = new QTreeView;
        tocModel = new QStandardItemModel(mainWindow);

        // 竖向 flex 布局
        auto leftPanelSplitter = new QSplitter(Qt::Vertical);
        leftPanelSplitter->setHandleWidth(0);
        leftPanelSplitter->addWidget(fileTree);
        leftPanelSplitter->setStretchFactor(0, 3);
        leftPanelSplitter->addWidget(tocTree);
        leftPanelSplitter->setStretchFactor(1, 2);

        QStringList headers;
        headers << QMainWindow::tr("table of contents");

        tocModel->setHorizontalHeaderLabels(headers);
        // register the model
        tocTree->setModel(tocModel);
        tocTree->expandAll();
        QObject::connect(tocTree->selectionModel(), SIGNAL(selectionChanged(const QItemSelection&,const QItemSelection&)), mainWindow, SLOT(handleTocClicked(const QItemSelection&,const QItemSelection&)));

        bodyLayout->addWidget(splitter);
        splitter->addWidget(leftPanelSplitter);
        splitter->setStretchFactor(leftPanel_INDEX, 1);

        fileTreeModel = new QFileSystemModel(mainWindow);
        fileTreeModel->setIconProvider(new DMFileIconProvider);
        fileTreeModel->setReadOnly(false);
        QObject::connect(fileTreeModel, SIGNAL(fileRenamed(const QString &, const QString &, const QString &)),
                         mainWindow,
                         SLOT(handleFileRenamed(const QString &, const QString &, const QString &)));

        // Set filter
        fileTreeModel->setFilter(QDir::NoDotAndDotDot |
                                QDir::AllDirs | QDir::Files);

        // drag & drop
        fileTree->setDragEnabled(true);
//        fileTree->viewport()->setAcceptDrops(true);
        fileTree->setDropIndicatorShown(true);
        fileTree->setDragDropMode(QAbstractItemView::InternalMove);
        fileTree->setSelectionMode(QAbstractItemView::MultiSelection);
        fileTree->setSortingEnabled(true);
        fileTree->sortByColumn(0, Qt::AscendingOrder);
        fileTree->setSelectionBehavior(QAbstractItemView::SelectItems);
        fileTree->setSelectionMode(QAbstractItemView::SingleSelection);
        fileTree->setModel(fileTreeModel);
        QObject::connect(fileTree->selectionModel(), SIGNAL(selectionChanged(const QItemSelection&,const QItemSelection&)), mainWindow, SLOT(fileSelectionChanged(const QItemSelection&,const QItemSelection&)));
        fileTree->setContextMenuPolicy(Qt::CustomContextMenu);

        QObject::connect(fileTree, SIGNAL(customContextMenuRequested(const QPoint &)),
                mainWindow, SLOT(contextMenu(const QPoint &)));

        fileTree->setHeaderHidden(true);
        for (int i = 1; i < fileTreeModel->columnCount(); ++i) {
            fileTree->hideColumn(i);
        }

        QFont font = QFont();
        int primaryFontSize = SyncFolderSettings::getInt(KEY_LAST_PRIMARY_FONT_SIZE, 12);
        font.setPointSize(primaryFontSize);
        fileTree->setFont(font);
        fileTree->setStyleSheet("QWidget {background-color:#ECECEC; color:#000000; selection-background-color:#DCDCDC; selection-color:#564C4F; }");
        tocTree->setFont(font);
        tocTree->setStyleSheet("QWidget {background-color:#ECECEC; color:#000000; selection-background-color:#DCDCDC; selection-color:#564C4F; }");

        // create markdown editor
        markdownEditor = new QMarkdownTextEdit(mainWindow);
        // TODO: add content change & save action -> window status
//        QObject::connect(markdownEditor->document(), &QTextDocument::contentsChanged,
//                [](int,int,int) {
//            // setDocStatus
//        });

        splitter->addWidget(markdownEditor);
        splitter->setStretchFactor(mdEditor_INDEX, 3);

        markdownPreviewView = new QWebEngineView(mainWindow);

        QWebEnginePage *webPage = new MarkdownPreviewWebPage(QWebEngineProfile::defaultProfile(), markdownPreviewView);
        webPage->settings()->setAttribute(QWebEngineSettings::LocalContentCanAccessFileUrls, true);
        webPage->settings()->setAttribute(QWebEngineSettings::LocalContentCanAccessRemoteUrls, true);
        webPage->settings()->setAttribute(QWebEngineSettings::XSSAuditingEnabled, false);
        webPage->settings()->setAttribute(QWebEngineSettings::HyperlinkAuditingEnabled, false);

        markdownPreviewView->setPage(webPage);

        QWebChannel *channel = new QWebChannel(webPage);
        channel->registerObject(QString("mdEditor"), mainWindow);
        webPage->setWebChannel(channel);

        splitter->addWidget(markdownPreviewView);
        splitter->setStretchFactor(mdPreviewer_INDEX, 3);
        splitter->setChildrenCollapsible(false);

        // we handle drag & drop globally in MainWindow, so disable it here
        markdownEditor->viewport()->setAcceptDrops(false);

        // sync scroll: editor -> previewer
        bool hasTriggered = false;
        QObject::connect(markdownEditor->verticalScrollBar(), &QScrollBar::valueChanged,
                         [&](int newValue ) {
//                             e new / e max = (p new) / p max
                             int eMax = markdownEditor->verticalScrollBar()->maximum();
                             if (eMax > 0.01 && !hasTriggered) {
                                 hasTriggered = true;
//                                 int newV = newValue * markdownPreviewView->verticalScrollBar()->maximum() / eMax;
//                                 markdownPreviewView->verticalScrollBar()->setValue(newV);
                                 hasTriggered = false;
                             }});

//        QObject::connect(markdownPreviewView->verticalScrollBar(), &QScrollBar::valueChanged,
//                         [&](int newValue/* p new*/ ) {
////                             e new / e max = (p new) / p max
//                             int eMax = markdownEditor->verticalScrollBar()->maximum();
//                             if (eMax > 0.01 && !hasTriggered) {
//                                 hasTriggered = true;
//                                 int newV = newValue * eMax  / markdownPreviewView->verticalScrollBar()->maximum();
//                                 markdownEditor->verticalScrollBar()->setValue(newV);
//                                 hasTriggered = false;
//                             }});

        imageLabel = new QLabel;
        imageLabel->setBackgroundRole(QPalette::Base);
        imageLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
        imageLabel->setScaledContents(true);

        // create center widget
        QWidget *centerWidget = new QWidget();
        centerWidget->setObjectName(QString::fromUtf8("centralWidget"));
        centerWidget->setLayout(bodyLayout);

        mainWindow->setCentralWidget(centerWidget);

        menuBar = new QMenuBar(mainWindow);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, windowWidth, 22));
        mainWindow->setMenuBar(menuBar);

//        statusBar = new QStatusBar(mainWindow);
//        statusBar->setObjectName(QString::fromUtf8("statusBar"));
//        mainWindow->setStatusBar(statusBar);

        mainWindow->centralWidget()->layout()->setContentsMargins(0, 0, 0, 0);

        retranslateUi(mainWindow);

        QMetaObject::connectSlotsByName(mainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "SyncFolder", nullptr));
    } // retranslateUi
};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
