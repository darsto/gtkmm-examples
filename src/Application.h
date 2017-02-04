/*
 * Copyright (c) 2016 Dariusz Stojaczyk. All Rights Reserved.
 * The following source code is released under an MIT-style license,
 * that can be found in the LICENSE file.
 */

#ifndef VEE_APPLICATION_H
#define VEE_APPLICATION_H

#include <gtkmm/application.h>
#include <gtkmm/builder.h>
#include <glibmm/dispatcher.h>
#include "ConcurrentQueue.h"
#include "windows/RAIIWindow.h"

class Core;
class SPacket;
class QPacket;

/**
 * Main Gui manager.
 * It binds with the Core class.
 */
class Application {
public:
    /**
     * The constructor.
     * Given program arguments will be processed inside it.
     * @param argc number of program arguments (forwarded from main())
     * @param argv program arguments (forwarded from main())
     */
    Application(int argc, char **argv);
    
    /**
     * Binds given core with this object.
     * A must-be-called method.
     * It's not a part of the constructor, 
     * since Core has to be initialized with the Application as an argument.
     * @param core 
     */
    void bindCore(Core *core);
    
    /**
     * Send given packet to the Core (Logic thread).
     * @param packet packet to be sent
     */
    void sendMessage(std::unique_ptr<QPacket> packet);
    
    /**
     * Send given packet to the Core (Logic thread).
     * Given handler might be called (depending on a q packet internals)
     * in the Application (Gui thread) after QPacket has been handled.
     * @param packet packet to send
     * @param handler function to be called after packet has been handled
     */
    void sendMessage(std::unique_ptr<QPacket> packet, std::function<void(SPacket &)> handler);
    
    /**
     * Push given packet to the packets queue.
     * This is called by the Core (Logic thread)
     * @param packet s packet to be received
     */
    void receiveMessage(std::unique_ptr<SPacket> packet);
    
    /**
     * Process all packets in the queue.
     * After calling this, the queue will be emptied.
     */
    void handleMessages();
    
    /**
     * Show error window with copy-able given message
     * @param errMessage message to be shown in the window
     */
    void showErrorWindow(const std::string &errMessage);
    
    /**
     * Initialize all windows, show them, and enter GTK event loop.
     * The method will return once main window is hidden (or closed)
     * @return regular exit status
     */
    int run();

    /**
     * Create a RAIIWindow of given type.
     * @tparam T type of window to create. It must inherit from RAIIWindow
     * @tparam Args arguments to forward to T constructor
     * @param args arguments to forward to T constructor
     * @return reference to just created window
     */
    template<class T, typename ...Args>
    T &createWindow(Args &&...args) {
        static_assert(std::is_base_of<RAIIWindow, T>::value, "Window to create must be of type RAIIWindow");

        std::unique_ptr<T> window = std::make_unique<T>(std::forward<Args>(args)...);
        registerWindow(*window);

        auto &ret = *window;
        m_windows.push_back(std::move(window));
        return ret;
    }
    
    /**
     * (!) If you wish to stop the Application, please use Core::stop().
     * This should be only called by the Core class.
     * ---
     * Starts the application closing sequence.
     * Stops accepting messages from the Core (Logic thread).
     * Also, the Core (Logic thread) might be stuck, as it's trying to push packets into a full packets queue.
     * In this case, a single packet from the queue is popped.
     */
    void stop();
    
    /**
     * The destructor.
     */
    ~Application();

private:
    void registerWindow(RAIIWindow &windowPtr);
    
private:
    std::vector<std::string> m_customArgs;
    Glib::RefPtr<Gtk::Application> m_app;
    Glib::Dispatcher m_dispatcher;
    ConcurrentQueue<std::unique_ptr<SPacket>, 1000> m_packetsQueue;
    std::function<void(std::unique_ptr<QPacket>)> m_sendMessageFunc;
    bool m_acceptMessages = true;
    std::vector<std::unique_ptr<RAIIWindow>> m_windows;
};

#endif //VEE_APPLICATION_H
