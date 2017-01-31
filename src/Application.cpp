/*
 * Copyright (c) 2016 Dariusz Stojaczyk. All Rights Reserved.
 * The following source code is released under an MIT-style license,
 * that can be found in the LICENSE file.
 */

#include <giomm.h>
#include <gtkmm/window.h>
#include <gtkmm/builder.h>
#include <gtkmm/textview.h>
#include <gtkmm/box.h>
#include "Application.h"
#include "thread/Core.h"
#include "thread/packet/Packets.h"

Application::Application(int argc, char **argv) {
    try {
        for (int i = argc - 1; i >= 0; --i) {
            // will accept any strings starting with '-', except for ones starting with '--g-'
            if (argv[i][0] == '-' && (strlen(argv[i]) < 4 || argv[i][1] != '-' || argv[i][2] != 'g' || argv[i][3] != '-')) {
                int loops = argv[i][1] == '-' ? 1 : 2;

                // arguments are in the following formats: "-$variable $option" and "--$flag"
                for (int l = 0; l < loops; ++l) {
                    m_customArgs.push_back(std::string(argv[i]));
                    --argc;
                    for (int j = i; j < argc; ++j) {
                        argv[j] = argv[j + 1];
                    }
                }
            }
        }

        m_app = Gtk::Application::create(argc, argv, "io.github.darsto.vee");
        m_builder = Gtk::Builder::create_from_file("../data/main.ui");

        m_dispatcher.connect([this]() {
            handleMessages();
        });

    } catch (const Glib::Exception &ex) {
        showErrorWindow(ex.what());
    } catch (const std::exception &ex) {
        showErrorWindow(ex.what());
    }
}

void Application::bindCore(Core *core) {
    m_sendMessageFunc = std::bind(&Core::receiveMessage, core, std::placeholders::_1);
}

void Application::sendMessage(std::unique_ptr<QPacket> packet) {
    if (m_sendMessageFunc) {
        m_sendMessageFunc(std::move(packet));
    } else {
        //todo warning msg log
    }
}

void Application::sendMessage(std::unique_ptr<QPacket> packet, std::function<void(SPacket &)> handler) {
    packet->responseHandler(handler);
    sendMessage(std::move(packet));
}

void Application::receiveMessage(std::unique_ptr<SPacket> packet) {
    if (m_acceptMessages) {
        m_packetsQueue.push(std::move(packet));
        m_dispatcher.emit();
    }
}

void Application::handleMessages() {
    while (!m_packetsQueue.empty()) {
        std::unique_ptr<SPacket> packet = m_packetsQueue.pop();
        if (packet) {
            packet->handle();
        }
    }
}

int Application::run() {
    int ret;

    Gtk::Window *window = nullptr;
    try {
        if (!m_app) throw std::runtime_error("Window has not been initialized properly. (Invalid arguments ?)");

        m_builder->get_widget("mainWindow", window);

        ret = m_app->run(*window);
    } catch (const Glib::Exception &ex) {
        ret = 1;
        showErrorWindow(ex.what());
    } catch (const std::exception &ex) {
        ret = 2;
        showErrorWindow(ex.what());
    }

    delete window;
    return ret;
}

void Application::stop() {
    m_acceptMessages = false;
    if (!m_packetsQueue.empty()) m_packetsQueue.pop();
}

void Application::showErrorWindow(const std::string &errMessage) {
    auto errorWindow = std::make_unique<Gtk::Window>();
    auto contentBox = Gtk::manage(new Gtk::Box(Gtk::ORIENTATION_VERTICAL));
    auto headerLabel = Gtk::manage(new Gtk::Label("Error occured. Please mail a bug report to darek.stojaczyk@gmail.com with the following information:"));
    auto textBuffer = Gtk::TextBuffer::create();
    auto textView = Gtk::manage(new Gtk::TextView(textBuffer));

    errorWindow->add(*contentBox);
    contentBox->pack_start(*headerLabel, false, false);
    contentBox->pack_start(*textView);

    errorWindow->set_title("Error - VEE");
    
    contentBox->set_margin_bottom(5);
    contentBox->set_margin_top(5);
    contentBox->set_margin_end(5);
    contentBox->set_margin_start(5);
    
    headerLabel->set_line_wrap(true);
    headerLabel->set_margin_bottom(2);
    
    textBuffer->set_text(errMessage);
    textView->set_editable(false);
    textView->set_wrap_mode(Gtk::WRAP_WORD);
    textView->set_left_margin(2);
    textView->set_top_margin(2);
    textView->set_right_margin(2);
    textView->set_bottom_margin(2);

    errorWindow->show_all();
    m_app->run(*errorWindow);
}

Application::~Application() = default;
