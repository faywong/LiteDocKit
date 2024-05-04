cmake_minimum_required(VERSION 3.1.0)

project(syncfolder)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

if(CMAKE_VERSION VERSION_LESS "3.7.0")
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif()

if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    set(Qt5_DIR "/Users/faywong/tools/Qt5.14.1/5.14.1/clang_64/lib/cmake/Qt5")
    message( STATUS "CMAKE_MODULE_PATH: " ${CMAKE_MODULE_PATH} )
    message( STATUS "Qt5_DIR: " ${Qt5_DIR} )
elseif (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    set(Qt5_DIR "E:\\Qt\\5.15.2\\msvc2019\\lib\\cmake\\Qt5")
    message( STATUS "CMAKE_MODULE_PATH: " ${CMAKE_MODULE_PATH} )
    message( STATUS "Qt5_DIR: " ${Qt5_DIR} )
endif()

find_package(Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt5 COMPONENTS Core REQUIRED)
find_package(Qt5 COMPONENTS Gui REQUIRED)
find_package(Qt5 COMPONENTS Concurrent REQUIRED)
find_package(Qt5 COMPONENTS Network REQUIRED)
find_package(Qt5 COMPONENTS WebEngineCore REQUIRED)
find_package(Qt5 COMPONENTS WebEngineWidgets REQUIRED)
find_package(Qt5 COMPONENTS WebChannel REQUIRED)

FILE(GLOB scidownSrcs 3rdparty/scidown/*.c 3rdparty/scidown/src/*.c 3rdparty/charter/src/*.c
        3rdparty/charter/src/csv_parser/*.c 3rdparty/charter/src/tinyexpr/*.c)

add_executable(SyncFolder
    FileIconProvider.cpp
    FileLocatorWindow.cpp
    FullTextSearchWindow.cpp
    fileformat.cpp
    main.cpp
    mainwindow.cpp
    settingdialog.cpp
    syncapp.cpp
    configurationpage.cpp
    uiwidgets/ProgressCircle.cpp
    uiwidgets/DisplayQueuedFilesAction.cpp
    resources.qrc
    translations/syncfolder_zh_CN.ts
    qmarkdowntextedit/highlighter.cpp
    qmarkdowntextedit/pmh_parser.c
    qmarkdowntextedit/pmh_styleparser.c
    qmarkdowntextedit/media.qrc
    3rdparty/QSimpleUpdater/src/Updater.cpp
    3rdparty/QSimpleUpdater/src/Updater.h
    3rdparty/QSimpleUpdater/src/Downloader.cpp
    3rdparty/QSimpleUpdater/src/Downloader.h
    3rdparty/QSimpleUpdater/src/QSimpleUpdater.cpp
    3rdparty/QSimpleUpdater/include/QSimpleUpdater.h
    3rdparty/QSimpleUpdater/src/Downloader.ui
    3rdparty/QSimpleUpdater/etc/resources/qsimpleupdater.qrc
    3rdparty/scidown/
    ISettingPage.h
    ${scidownSrcs}
    3rdparty/scidown/bin/scidown.c
    3rdparty/scidown/bin/scidown_md.h uiwidgets/MarkdownPreviewWebPage.cpp uiwidgets/MarkdownPreviewWebPage.h
    mdLexer.h mdLexer.cpp)

target_include_directories(SyncFolder PUBLIC
        .
        uiwidgets
        qmarkdowntextedit
        3rdparty/scidown/src
        3rdparty/scidown/bin
        3rdparty/charter/src
        3rdparty/
        3rdparty/QSimpleUpdater
        3rdparty/QSimpleUpdater/include
        3rdparty/QSimpleUpdater/src
        prebuilt/scintilla/include)

set_target_properties(SyncFolder PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED YES
        CXX_EXTENSIONS NO
        )

target_link_libraries(SyncFolder Qt5::Widgets Qt5::Concurrent Qt5::Network Qt5::WebEngineCore Qt5::WebEngineWidgets Qt5::WebChannel
            ${CMAKE_SOURCE_DIR}/prebuilt/libqscintilla2_qt5.a)
