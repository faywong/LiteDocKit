#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QItemSelection>
#include <QMainWindow>
#include <QProcess>

#include <iostream>
#include <QStandardItem>
#include "FullTextSearchWindow.h"
#include "FileLocatorWindow.h"
#include "CircleProgressBar.h"
#include "highlighter.h"
#include <QSimpleUpdater.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow, public DMEditorDelegate
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr, QString* dirPath = nullptr);
    ~MainWindow() override;

    void setCurrentRootDirPath(const QString &value);
    void setDocStatus(const QString &fileName, const QString &status);

    // implement of DMEditorDelegate start
    void openFile_l(const QString &, size_t lineNo, bool needSelect = false) override;
    void updateToc(const QVector<QStandardItem*> &nodes) override;
    // implement of DMEditorDelegate end

    QString getSyncConfigDir();

    void saveFileFromText(const QString &text);
    void newFileWithTitleContent(const QString &title, const QString &content);
public slots:
    void syncFiles();
    void launchSettings();
    void newFile();
    void openFile();
    void openDirectory();
    void saveFile();
    void fileSelectionChanged(const QItemSelection&,const QItemSelection&);
    void launchSearchWindow();
    void launchFindFileWindow();
    void handleSyncFinished(int exitCode, QProcess::ExitStatus exitStatus);
    void revealInTreeView();
    void about();
    void help();
    void contextMenu(const QPoint &pos);
    void processSyncStdOutput();
    void processSyncStdError();
    void handleFileRenamed(const QString &path,
                           const QString &oldName,
                           const QString &newName);
    void handleOrgCaptured(const QString &);
    void handleTocClicked(const QItemSelection&,const QItemSelection&);
    void onCheckingUpdateFinished (const QString& url);
    void displayAppcast (const QString& url, const QByteArray& reply);
    void onUpdateDownloadFinished(const QString& url, const QString& path);
    void updateMarkdownPreview(const QString &html);

protected:
    void setupMenus();
    QFileInfo selectedFile();
    void revealInTreeView_l(const QString &path);

    /**
       * this event is called when the drop operation is initiated at the widget
       */
    void dropEvent(QDropEvent *event) override;
    void dragEnterEvent(QDragEnterEvent* event) override;

private:
    Ui::MainWindow *ui;
    HGMarkdownHighlighter *_highlighter;
    QString currentFilePath;
    QString currentRootDirPath;
    QSimpleUpdater* m_updater;

    // search related
    std::vector<std::string> fileNamesDictionary;
    FullTextSearchWindow *searchWindow;
    FindFileWindow *findFileWindow;

    // sync related
    QProcess *unisonProcess;
    QString syncLog;
    CircleProgressBar *syncProgressBar;
    // 文件操作队列
    QSet<QString> fileOperationQueue;

    void showInFolder(const QString &path);
};

#endif // MAINWINDOW_H
