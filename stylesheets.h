//---------------------------------------------------------------------------------
// Stylesheets for Toolbar/toolbutton (w) 2025 Jan Buchholz
// last change: 20251014
//---------------------------------------------------------------------------------

#pragma once

#include <QString>

QString styleToolButton = R"(
        QToolBar > QToolButton {
            border-radius: 3px;
            padding: 2px 2px;
        }
        QToolBar > QToolButton:hover {
            background-color: gray;
            border-color: auto;
        }
        QToolBar > QToolButton:pressed {
            background-color: darkGray;
            border-color: auto;
        }
        QToolBar > QToolButton:checked {
            background-color: darkGray;
            border-color: auto;
        }
)";

QString styleToolBar = R"(
        QToolBar {
            border: 0px;
            padding: 1px 1px;
        }
        QToolBar > QToolButton {
            border-radius: 3px;
            padding: 4px 4px;
            height: 16px;
            width: 16px;
            max-width: 16px;
            max-height: 16px;
        }
        QToolBar > QToolButton:hover {
            background-color: lightGray;
            border-color: auto;
        }
        QToolBar > QToolButton:pressed {
            background-color: darkGray;
            border-color: auto;
        }
        QToolBar > QToolButton:checked {
            background-color: darkGray;
            border-color: auto;
        }
)";
