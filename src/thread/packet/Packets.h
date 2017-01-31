/*
 * Copyright (c) 2016 Dariusz Stojaczyk. All Rights Reserved.
 * The following source code is released under an MIT-style license,
 * that can be found in the LICENSE file.
 */

#ifndef VEE_THREAD_PACKET_PACKETS_H
#define VEE_THREAD_PACKET_PACKETS_H

#include "thread/Core.h"

/**
 * S Packets are sent back from Core (Logic thread) to Application (Gui thread).
 * They are accessible as an argument in provided handler functions
 */
class SPacket {
public:
    /**
     * Handler function to-be-called by Application (Gui thread)
     */
    using Handler = std::function<void(SPacket &)>;
    
    /**
     * The constructor.
     * @param responseHandler handler to bind with this packet
     */
    SPacket(const Handler &responseHandler) {
        m_responseHandler = responseHandler;
    }

    /**
     * Will call any handler bound to this packet.
     * Called by Application (Gui thread)
     */
    void handle() {
        if (m_responseHandler) {
            m_responseHandler(*this);
        }
    }

protected:
    Handler m_responseHandler;
};

/**
 * Q Packets are sent from Application (Gui thread) to Core (Logic thread).
 */
class QPacket {
public:
    /**
     * Bind response handler to this packet
     * @param responseHandler handler to be bound with this packet
     */
    void responseHandler(const SPacket::Handler &responseHandler) {
        m_responseHandler = responseHandler;
    }

    /**
     * Will be called in the Core (Logic thread).
     * All heavy operations should be executed here.
     * @param core core object, enables you to send packets back to the Application
     */
    virtual void handle(Core &core) = 0;

protected:
    SPacket::Handler m_responseHandler;
};

#endif //VEE_THREAD_PACKET_PACKETS_H
