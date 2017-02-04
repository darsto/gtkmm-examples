/*
 * Copyright (c) 2017 Dariusz Stojaczyk. All Rights Reserved.
 * The following source code is released under an MIT-style license,
 * that can be found in the LICENSE file.
 */

#include <gtkmm/builder.h>
#include "MainWindow.h"
#include "windows/second/SecondWindow.h"

MainWindow::MainWindow(Application &app)
    : RAIIWindow(app) {

    auto builder = Gtk::Builder::create_from_file("../data/main.ui");
    builder->get_widget("mainWindow", m_window);
    builder->get_widget("mw_switch", m_switch);
    
    m_switch->signal_clicked().connect([this] () {
        auto &second = m_app.createWindow<SecondWindow>(m_app);
        
        second->show_all();
        m_window->close();
    });
}
