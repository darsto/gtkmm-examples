/*
 * Copyright (c) 2016 Dariusz Stojaczyk. All Rights Reserved.
 * The following source code is released under an MIT-style license,
 * that can be found in the LICENSE file.
 */

#ifndef VEE_THREAD_CORE_H
#define VEE_THREAD_CORE_H

#include <thread>
#include "ConcurrentQueue.h"

class Application;
class QPacket;
class SPacket;

/**
 * Class encapsulating a special thread for background actions.
 * It binds with Application class.
 */
class Core {
public:
    /**
     * The constructor.
     * Bind given application with this object.
     * @param app 
     */
    Core(Application &app);
    
    /**
     * Whether the internal thread is already and yet running.
     * @return whether the internal thread is already and yet running
     */
    bool running() const;
    
    /**
     * Add given packet to the queue.
     * This is called by the Application (Gui thread).
     * @param packet packet to be received
     */
    void receiveMessage(std::unique_ptr<QPacket> packet);
    
    /**
     * Send a packet to the Application (Gui thread).
     * @param packet packet to be sent
     */
    void sendMessage(std::unique_ptr<SPacket> packet);
    
    /**
     * Process the top packet in the queue.
     * This method uses mutex lock and might not exit as long as the queue is empty.
     */
    void handleMessages();
    
    /**
     * Stops the internal thread as well as the bound Application.
     * Once this method exits, both the Core (Logic thread)
     * and the Application (Gui thread) will be stopped.
     */
    void stop();
    
    /**
     * The destructor.
     */
    ~Core();
    
private:
    Application &m_app;
    std::thread m_thread;
    
    std::atomic<bool> m_running;
    ConcurrentQueue<std::unique_ptr<QPacket>, 100> m_packetsQueue;
    std::mutex m_queuePopMutex;
};

#endif //VEE_THREAD_CORE_H
