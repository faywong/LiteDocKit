#-------------------------------------------------
#
# Project created by QtCreator 2019-08-02T14:20:49
#
#-------------------------------------------------

QT       += core gui concurrent network widgets webenginewidgets

TARGET = SyncFolder
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++17 libpng_static

SOURCES += \
    FileIconProvider.cpp \
    FileLocatorWindow.cpp \
    FullTextSearchWindow.cpp \
    configurationpage.cpp \
    fileformat.cpp \
    main.cpp \
    mainwindow.cpp \
    settingdialog.cpp \
    syncapp.cpp \
    uiwidgets/DisplayQueuedFilesAction.cpp \
    uiwidgets/ProgressCircle.cpp \
    3rdparty/scidown/bin/scidown.c \
    3rdparty/scidown/bin/smartypants.c

INCLUDEPATH += . \
            uiwidgets \

HEADERS += \
        EditorDelegate.h \
        FileIconProvider.h \
        FileLocatorWindow.h \
        FullTextSearchWindow.h \
        configurationpage.h \
        fileformat.h \
        mainwindow.h \
        mainwindowbuilder.h \
        qmarkdowntextedit/highlighter.h \
        qmarkdowntextedit/pmh_definitions.h \
        qmarkdowntextedit/pmh_parser.h \
        qmarkdowntextedit/pmh_styleparser.h \
        settingdialog.h \
        settings/settings_def.h \
        syncapp.h \
        uiwidgets/CircleProgressBar.h \
        uiwidgets/DisplayQueuedFilesAction.h \
        uiwidgets/ProgressCircle.h \
        3rdparty/scidown/bin/scidown.h \
        3rdparty/scidown/bin/common.h

FORMS +=

include(qmarkdowntextedit/qmarkdowntextedit.pri)

RESOURCES += \
    resources.qrc

# TODO: icons
#ICON = syncfolder.icns

#RC_ICONS = syncfolder.ico

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    bin/unison

QMAKE_INFO_PLIST = config/Info.plist
TRANSLATIONS = translations/syncfolder_zh_CN.ts

include(3rdparty/QSimpleUpdater/QSimpleUpdater.pro)

#DEFINES += "pmh_DEBUG_OUTPUT=1"

