/*
 * Copyright (c) 2016 Dariusz Stojaczyk. All Rights Reserved.
 * The following source code is released under an MIT-style license,
 * that can be found in the LICENSE file.
 */

#ifndef VEE_THREAD_PACKET_PDELAY_H
#define VEE_THREAD_PACKET_PDELAY_H

#include <chrono>
#include <thread>
#include <string>
#include "Packets.h"

/**
 * General purpose packet.
 * Makes the Core (Logic thread) wait for the given amount of time, then sends back empty SPacket.
 * Can be used to delay executing some given code on the Application side (Gui thread)
 */
class QDelay : public QPacket {
public:
    /**
     * The constructor.
     * @param miliseconds time to wait in miliseconds 
     */
    QDelay(unsigned int miliseconds = 0)
        : m_miliseconds(miliseconds) {}

    void handle(Core &core) override {
        std::this_thread::sleep_for(std::chrono::milliseconds(m_miliseconds));
        core.sendMessage(std::make_unique<SPacket>(m_responseHandler));
    }

private:
    unsigned int m_miliseconds;

};

#endif //VEE_THREAD_PACKET_PDELAY_H
