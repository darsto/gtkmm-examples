/*
 * Copyright (c) 2017 Dariusz Stojaczyk. All Rights Reserved.
 * The following source code is released under an MIT-style license,
 * that can be found in the LICENSE file.
 */

#ifndef VEE_WINDOWS_RAIIWINDOW_H
#define VEE_WINDOWS_RAIIWINDOW_H

#include <gtkmm/window.h>

class Application;

class RAIIWindow {
public:
    RAIIWindow(Application &app);
    Gtk::Window &operator*() const;
    Gtk::Window *operator->() const;
    Gtk::Window *get() const;

protected:
    Application &m_app;
    Gtk::Window *m_window = nullptr;
};

#endif //VEE_WINDOWS_RAIIWINDOW_H
