/*
 * Copyright (c) 2016 Dariusz Stojaczyk. All Rights Reserved.
 * The following source code is released under an MIT-style license,
 * that can be found in the LICENSE file.
 */

#ifndef VEE_THREAD_PACKET_PLAZY_H
#define VEE_THREAD_PACKET_PLAZY_H

#include <string>
#include "Packets.h"

/**
 * General purpose packet.
 * Executes different actions based on the given commmand string.
 */
class QLazy : public QPacket {
public:
    /**
     * The constructor
     * @param command information to identify this packet
     */
    QLazy(const std::string &command)
        : m_command(command) {}

    void handle(Core &core) override {
        //...
    }

private:
    std::string m_command;
};

#endif //VEE_THREAD_PACKET_PLAZY_H
