/*
 * Copyright (c) 2017 Dariusz Stojaczyk. All Rights Reserved.
 * The following source code is released under an MIT-style license,
 * that can be found in the LICENSE file.
 */

#ifndef VEE_WINDOWS_MAIN_MAINWINDOW_H
#define VEE_WINDOWS_MAIN_MAINWINDOW_H

#include <gtkmm/button.h>
#include "Application.h"
#include "windows/RAIIWindow.h"

class MainWindow : public RAIIWindow {
public:
    MainWindow(Application &app);
    
private:
    Gtk::Button *m_switch;
};

#endif //VEE_WINDOWS_MAIN_MAINWINDOW_H
