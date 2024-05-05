#include "mainwindow.h"
#include <QCommandLineParser>
#include <QDebug>
#include <QStyleFactory>
#include <QtGlobal>
#include <QTranslator>
#include "settings/settings_def.h"
#include "syncapp.h"
#include <QApplication>
#include <QTextStream>
#include <QFile>
#include <QDateTime>
#include <QMutex>

const qint64 LOG_FILE_LIMIT = 3000000;
const QString LOG_PATH = "logs/";
const QString LOG_FILENAME = "dockit.log";
QMutex mutex;

void redirectDebugMessages(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    mutex.lock();

    QString txt;
    QString datetime = QDateTime::currentDateTime().toString("yyyy.MM.dd hh:mm:ss");

    switch (type) {
    case QtDebugMsg:
        txt = QString("[Debug] ");
        break;
    case QtWarningMsg:
        txt = QString("[Warning] ");
        break;
    case QtInfoMsg:
        txt = QString("[Info] ");
        break;
    case QtCriticalMsg:
        txt = QString("[Critical] ");
        break;
    case QtFatalMsg:
        txt = QString("[Fatal] ");
        break;
    }

    QString filePath = LOG_PATH + LOG_FILENAME;
    QFile outFile(filePath);

    if (outFile.size() > LOG_FILE_LIMIT) {
        QFile::remove(filePath + ".1");
        QFile::rename(filePath, filePath + ".1");
        QFile::resize(filePath, 0);
    }

    outFile.open(QIODevice::WriteOnly | QIODevice::Append);
    QTextStream ts(&outFile);
    ts << datetime << txt << msg << endl;
    outFile.close();

    mutex.unlock();
}

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QCoreApplication::setAttribute(Qt::AA_ShareOpenGLContexts);
    // qInstallMessageHandler(redirectDebugMessages);
    SyncApp a(argc, argv);
    a.setStyleSheet(QString::fromUtf8("QScrollBar:vertical {"
                                      "    background:white;"
                                      "    width: 8px;    "
                                      "    margin: 0px 0px 0px 0px;"
                                      "}"
                                      "QScrollBar::handle:vertical {"
                                      "    background: #C2C2C2;"
                                      "    min-height: 0px;"
                                      "    border: 1px transparent #2A2929;\n"
                                      "    border-radius: 3px;"
                                      "}"
                                      "QScrollBar::add-line:vertical {"
                                      "    background: #C2C2C2;"
                                      "    border: 1px transparent #2A2929;\n"
                                      "    border-radius: 3px;"
                                      "    height: 0px;"
                                      "    subcontrol-position: bottom;"
                                      "    subcontrol-origin: margin;"
                                      "}"
                                      "QScrollBar::sub-line:vertical {"
                                      "    background: #C2C2C2;"
                                      "    height: 0 px;"
                                      "    subcontrol-position: top;"
                                      "    subcontrol-origin: margin;"
                                      "}"
                                      "QStatusBar, QWidget[objectName^=\"centralWidget\"] {"
                                      "    background-color: #F7F6F8;"
                                      "}"
    ));
    QTranslator translator;
    QString locale = QLocale::system().name();
    if (locale == "zh_CN") {
        translator.load(":/translations/syncfolder_zh_CN.qm");
    }
    a.installTranslator(&translator);

#if defined (Q_OS_MAC) || defined (Q_OS_MACOS)
    if (QStyleFactory::keys().contains("macintosh")) {
        a.setStyle(QStyleFactory::create("macintosh"));
    }
#elif defined (Q_OS_LINUX)
    if (QStyleFactory::keys().contains("kvantum")) {
        a.setStyle(QStyleFactory::create("kvantum"));
        qDebug() <<"kvantum";
    }
#endif

    QCoreApplication::setApplicationVersion(SYNC_FOLDER_VER);
    QCoreApplication::setOrganizationName("faywong personal");
    QCoreApplication::setOrganizationDomain("litedockit.faywong.cc");
    QCoreApplication::setApplicationName("LiteDocKit");
    QCommandLineParser parser;
    parser.setApplicationDescription("Your lite document helper");


    QCommandLineOption dirArg("dir", "Directory to open.", "dir");
    parser.addOption(dirArg);

    QCommandLineOption orgCaptureArg("orgcapture", "Org-capture url.", "org_capture");
    parser.addOption(orgCaptureArg);
    parser.addHelpOption();
    parser.addVersionOption();
    parser.process(a);

    QString rootPath;
    QString orgCaptureValue;

    if (parser.isSet(dirArg)) {
        rootPath = parser.value(dirArg);
    }
    if (parser.isSet(orgCaptureArg)) {
        orgCaptureValue = parser.value(orgCaptureArg);
    }
    MainWindow w;

    if (rootPath.isEmpty()) {
        rootPath = SyncFolderSettings::getString(KEY_LAST_FOLDER);
        qDebug() << "No rootPath value provided.";
    } else {
        qDebug() << "rootPath: " << rootPath;
        w.setCurrentRootDirPath(rootPath);
    }

    if (!orgCaptureValue.isEmpty()) {
        qDebug() << "orgCaptureValue: " << orgCaptureValue;
        w.handleOrgCaptured(orgCaptureValue);
    } else {
        qDebug() << "No orgCaptureValue value provided.";
    }

    QString lastEditedFile = SyncFolderSettings::getString(KEY_LAST_FILE);
    if (!lastEditedFile.isEmpty()) {
        w.openFile_l(lastEditedFile, 1, true);
    }

    // 获取所有的命令行参数
    QStringList arguments = a.arguments();

    // 打印所有的命令行参数
    qDebug() << "Command Line Arguments:";
    for (const QString &arg : arguments) {
        qDebug() << arg;
    }

    w.show();

    return a.exec();
}
