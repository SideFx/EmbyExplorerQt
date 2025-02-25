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
        assets/toolbar_quit.svg
        assets/emby.png
        assets/toolbar_details.svg
        assets/toolbar_export.svg
        assets/toolbar_fetch.svg
        assets/toolbar_login.svg
        assets/toolbar_preferences.svg
        assets/toolbar_quit.svg
        splashdialog.h splashdialog.cpp splashdialog.ui
        app.ico
        assets/embylogo.png
        ${app_icon_resource_windows}
        assets/toolbar_about.svg
        lib/libSimpleXlsx.a
        include/PathManager.hpp include/UTF8Encoder.hpp include/XMLWriter.hpp
        include/scratch/clsScratchTable.h
        include/Xlsx/Chart.h include/Xlsx/Chartsheet.h include/Xlsx/Drawing.h include/Xlsx/SimpleXlsxDef.h include/Xlsx/Workbook.h include/Xlsx/Worksheet.h include/Xlsx/XlsxHeaders.h
        include/XLSXColors/clsRGBColorRecord.h include/XLSXColors/XLSXColorLib.h
        include/Zip/zip.h
        export.h export.cpp
        messagedialog.h messagedialog.cpp
        aboutdialog.h aboutdialog.cpp aboutdialog.ui
        .gitignore
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
target_link_libraries(EmbyExplorerQt PRIVATE Qt6::Widgets Qt6::Network ${CMAKE_SOURCE_DIR}/lib/libSimpleXlsx.a)
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
