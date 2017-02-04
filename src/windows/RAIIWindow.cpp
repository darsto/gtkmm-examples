/*
 * Copyright (c) 2017 Dariusz Stojaczyk. All Rights Reserved.
 * The following source code is released under an MIT-style license,
 * that can be found in the LICENSE file.
 */

#include "RAIIWindow.h"
#include "Application.h"

RAIIWindow::RAIIWindow(Application &app)
    : m_app(app) {}

Gtk::Window &RAIIWindow::operator*() const {
    return *m_window;
}

Gtk::Window *RAIIWindow::operator->() const {
    return m_window;
}

Gtk::Window *RAIIWindow::get() const {
    return m_window;
}
