cmake_minimum_required(VERSION 3.16)

project(EmbyExplorerQt VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Network LinguistTools)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Network LinguistTools)

set(TS_FILES EmbyExplorerQt_en_US.ts)

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
        ${TS_FILES}
)

qt_add_translations(EmbyExplorerQt TS_FILES EmbyExplorerQt_en_US.ts)
qt_standard_project_setup(
    I18N_SOURCE_LANGUAGE de         # optional - this is the default
    I18N_TRANSLATED_LANGUAGES en
)

add_subdirectory(QXlsx)

set(MACOSX_BUNDLE_ICON_FILE app.icns)
set(APP_ICON "${CMAKE_CURRENT_SOURCE_DIR}/app.icns")
set_source_files_properties(${APP_ICON} PROPERTIES MACOSX_PACKAGE_LOCATION "Resources")

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    set(app_icon_resource_windows "${CMAKE_CURRENT_SOURCE_DIR}/appicon.rc")
    qt_add_executable(EmbyExplorerQt
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        resource.qrc
        globals.h
        preferencesdialog.h preferencesdialog.cpp preferencesdialog.ui
        dispatcher.h dispatcher.cpp
        restapi.h restapi.cpp
        detailsdialog.h detailsdialog.cpp detailsdialog.ui
        assets/emby.png
        splashdialog.h splashdialog.cpp splashdialog.ui
        app.ico
        assets/embylogo.png
        ${app_icon_resource_windows}
        export.h export.cpp
        messagedialog.h messagedialog.cpp
        aboutdialog.h aboutdialog.cpp aboutdialog.ui
        .gitignore
        QXlsx/cmake/modules/CPackConfig.cmake QXlsx/cmake/modules/qxlsx-config-version.cmake.in QXlsx/cmake/modules/qxlsx-config.cmake.in QXlsx/header/xlsxabstractooxmlfile.h QXlsx/header/xlsxabstractooxmlfile_p.h QXlsx/header/xlsxabstractsheet.h QXlsx/header/xlsxabstractsheet_p.h QXlsx/header/xlsxcell.h QXlsx/header/xlsxcell_p.h QXlsx/header/xlsxcellformula.h QXlsx/header/xlsxcellformula_p.h QXlsx/header/xlsxcelllocation.h QXlsx/header/xlsxcellrange.h QXlsx/header/xlsxcellreference.h QXlsx/header/xlsxchart.h QXlsx/header/xlsxchart_p.h QXlsx/header/xlsxchartsheet.h QXlsx/header/xlsxchartsheet_p.h QXlsx/header/xlsxcolor_p.h QXlsx/header/xlsxconditionalformatting.h QXlsx/header/xlsxconditionalformatting_p.h QXlsx/header/xlsxcontenttypes_p.h QXlsx/header/xlsxdatavalidation.h QXlsx/header/xlsxdatavalidation_p.h QXlsx/header/xlsxdatetype.h QXlsx/header/xlsxdocpropsapp_p.h QXlsx/header/xlsxdocpropscore_p.h QXlsx/header/xlsxdocument.h QXlsx/header/xlsxdocument_p.h QXlsx/header/xlsxdrawing_p.h QXlsx/header/xlsxdrawinganchor_p.h QXlsx/header/xlsxformat.h QXlsx/header/xlsxformat_p.h QXlsx/header/xlsxglobal.h QXlsx/header/xlsxmediafile_p.h QXlsx/header/xlsxnumformatparser_p.h QXlsx/header/xlsxrelationships_p.h QXlsx/header/xlsxrichstring.h QXlsx/header/xlsxrichstring_p.h QXlsx/header/xlsxsharedstrings_p.h QXlsx/header/xlsxsimpleooxmlfile_p.h QXlsx/header/xlsxstyles_p.h QXlsx/header/xlsxtheme_p.h QXlsx/header/xlsxutility_p.h QXlsx/header/xlsxworkbook.h QXlsx/header/xlsxworkbook_p.h QXlsx/header/xlsxworksheet.h QXlsx/header/xlsxworksheet_p.h QXlsx/header/xlsxzipreader_p.h QXlsx/header/xlsxzipwriter_p.h QXlsx/source/xlsxabstractooxmlfile.cpp QXlsx/source/xlsxabstractsheet.cpp QXlsx/source/xlsxcell.cpp QXlsx/source/xlsxcellformula.cpp QXlsx/source/xlsxcelllocation.cpp QXlsx/source/xlsxcellrange.cpp QXlsx/source/xlsxcellreference.cpp QXlsx/source/xlsxchart.cpp QXlsx/source/xlsxchartsheet.cpp QXlsx/source/xlsxcolor.cpp QXlsx/source/xlsxconditionalformatting.cpp QXlsx/source/xlsxcontenttypes.cpp QXlsx/source/xlsxdatavalidation.cpp QXlsx/source/xlsxdatetype.cpp QXlsx/source/xlsxdocpropsapp.cpp QXlsx/source/xlsxdocpropscore.cpp QXlsx/source/xlsxdocument.cpp QXlsx/source/xlsxdrawing.cpp QXlsx/source/xlsxdrawinganchor.cpp QXlsx/source/xlsxformat.cpp QXlsx/source/xlsxmediafile.cpp QXlsx/source/xlsxnumformatparser.cpp QXlsx/source/xlsxrelationships.cpp QXlsx/source/xlsxrichstring.cpp QXlsx/source/xlsxsharedstrings.cpp QXlsx/source/xlsxsimpleooxmlfile.cpp QXlsx/source/xlsxstyles.cpp QXlsx/source/xlsxtheme.cpp QXlsx/source/xlsxutility.cpp QXlsx/source/xlsxworkbook.cpp QXlsx/source/xlsxworksheet.cpp QXlsx/source/xlsxzipreader.cpp QXlsx/source/xlsxzipwriter.cpp QXlsx/CMakeLists.txt QXlsx/QXlsx.pri QXlsx/QXlsx.pro
        assets/tb_about.png assets/tb_details.png assets/tb_export.png assets/tb_fetch.png assets/tb_login.png assets/tb_preferences.png assets/tb_quit.png
        app.icns
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET EmbyExplorerQt APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation

#qt_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
else()
    if(ANDROID)
        add_library(EmbyExplorerQt SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(EmbyExplorerQt
            ${PROJECT_SOURCES}
        )
    endif()

    qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
endif()

add_library(SimpleXlsx STATIC)
target_link_libraries(EmbyExplorerQt PRIVATE Qt6::Widgets Qt6::Network QXlsx::QXlsx)
add_dependencies(EmbyExplorerQt update_translations)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.EmbyExplorerQt)
endif()
set_target_properties(EmbyExplorerQt PROPERTIES
    ${BUNDLE_ID_OPTION}
    UPDATE_TRANSLATIONS .
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS EmbyExplorerQt
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(EmbyExplorerQt)
endif()
