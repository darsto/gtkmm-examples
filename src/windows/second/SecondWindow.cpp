/*
 * Copyright (c) 2017 Dariusz Stojaczyk. All Rights Reserved.
 * The following source code is released under an MIT-style license,
 * that can be found in the LICENSE file.
 */

#include <gtkmm/builder.h>
#include "SecondWindow.h"
#include "windows/main/MainWindow.h"

SecondWindow::SecondWindow(Application &app)
    : RAIIWindow(app) {

    auto builder = Gtk::Builder::create_from_file("../data/second.ui");
    builder->get_widget("secondWindow", m_window);
    builder->get_widget("sw_switch", m_switch);

    m_switch->signal_clicked().connect([this] () {
        auto &main = m_app.createWindow<MainWindow>(m_app);

        main->show_all();
        m_window->close();
    });
}
