cmake_minimum_required(VERSION 3.16)

project(EmbyExplorerQt VERSION 1.3.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(QT_NO_PRIVATE_MODULE_WARNING ON)

#added GuiPrivate Qt6.10
find_package(QT NAMES Qt6 REQUIRED COMPONENTS Gui GuiPrivate Widgets Network)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Gui GuiPrivate Widgets Network)

set(QM_FILES ${CMAKE_SOURCE_DIR}/translations/qt_en.qm)

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
)
add_subdirectory("${CMAKE_SOURCE_DIR}/QXlsx")

if(APPLE)
set(MACOSX_BUNDLE_ICON_FILE app.icns)
set(APP_ICON "${CMAKE_SOURCE_DIR}/app.icns")
set_source_files_properties(${APP_ICON} PROPERTIES MACOSX_PACKAGE_LOCATION "Resources")
endif()

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    set(app_icon_resource_windows "${CMAKE_SOURCE_DIR}/appicon.rc")
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
        assets/tb_about.png assets/tb_details.png assets/tb_export.png assets/tb_fetch.png assets/tb_login.png assets/tb_preferences.png assets/tb_quit.png
        jbpreferences.cpp jbpreferences.h
        app.icns
        stylesheets.h
    )
endif()

add_library(SimpleXlsx STATIC)
target_link_libraries(EmbyExplorerQt PRIVATE Qt6::GuiPrivate Qt6::Widgets Qt6::Network QXlsx::QXlsx)

set_target_properties(EmbyExplorerQt PROPERTIES
    ${BUNDLE_ID_OPTION}
#    UPDATE_TRANSLATIONS .
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

if(QT_VERSION_MAJOR GREATER_EQUAL 6)
    qt_finalize_executable(EmbyExplorerQt)
endif()

# --- Windows ---
if(WIN32)
    set(QM_DEST ${CMAKE_BINARY_DIR}/translations)
    add_custom_command(
        TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory ${QM_DEST}
        COMMAND ${CMAKE_COMMAND} -E copy ${QM_FILES} ${QM_DEST}
    )
endif()

# --- macOS ---
if(APPLE)
    # App-Bundle in Resources/i18n/
    set(QM_DEST ${CMAKE_BINARY_DIR}/${PROJECT_NAME}.app/Contents/Resources/i18n)
    add_custom_command(
        TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory ${QM_DEST}
        COMMAND ${CMAKE_COMMAND} -E copy ${QM_FILES} ${QM_DEST}
    )
endif()

# --- Linux/Unix ---
if(UNIX AND NOT APPLE)
    # Local
    set(QM_DEST ${CMAKE_BINARY_DIR}/translations)
    add_custom_command(
        TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory ${QM_DEST}
        COMMAND ${CMAKE_COMMAND} -E copy ${QM_FILES} ${QM_DEST}
    )
    # System
    install(FILES ${QM_FILES} DESTINATION share/${PROJECT_NAME}/translations)
endif()
