/*
 * Copyright (c) 2016 Dariusz Stojaczyk. All Rights Reserved.
 * The following source code is released under an MIT-style license,
 * that can be found in the LICENSE file.
 */

#include "Core.h"
#include "Application.h"
#include "packet/Packets.h"

Core::Core(Application &app)
    : m_app(app),
      m_thread(
          [this]() {
              m_running = true;
              while (m_running) {
                  handleMessages();
              }
          }
      ) { }

bool Core::running() const {
    return m_running;
}

void Core::receiveMessage(std::unique_ptr<QPacket> packet) {
    m_packetsQueue.push(std::move(packet));
    m_queuePopMutex.unlock();
}

void Core::sendMessage(std::unique_ptr<SPacket> packet) {
    m_app.receiveMessage(std::move(packet));
}

void Core::handleMessages() {
    std::unique_lock<std::mutex> lock1(m_queuePopMutex, std::defer_lock);
    if (m_packetsQueue.empty() && m_running) {
        m_queuePopMutex.lock();
    } else {
        std::unique_ptr<QPacket> packet = m_packetsQueue.pop();
        if (packet && m_running) {
            packet->handle(*this);
        }
    }
}

void Core::stop() {
    m_running = false;
    if (!m_packetsQueue.empty()) m_packetsQueue.pop();
    m_queuePopMutex.unlock();
    m_app.stop();
    m_thread.join();
}

Core::~Core() = default;
