//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
//

#include "QueueDetnet.h"

#include <iostream>
#include <fstream>
#include <string>


namespace detnetmod{

Define_Module(QueueDetnet);

QueueDetnet::~QueueDetnet() {
    cancelEvent(&requestPacketMsg);
    while (!queue.isEmpty()) {
        delete queue.pop();
    }
    queue.clear();
}

void QueueDetnet::initialize() {
    rcvdPkSignal = registerSignal("rcvdPk");
    enqueuePkSignal = registerSignal("enqueuePk");
    dequeuePkSignal = registerSignal("dequeuePk");
    dropPkByQueueSignal = registerSignal("dropPkByQueue");
    queueingTimeSignal = registerSignal("queueingTime");
    queueLengthSignal = registerSignal("queueLength");

    queue.setName(par("queueName"));
    availableBufferCapacity = par("bufferCapacity");
    expressQueue = par("expressQueue");
    WATCH(numPacketsReceived);
    WATCH(numPacketsDropped);
    WATCH(numPacketsEnqueued);
    WATCH(availableBufferCapacity);
//    WATCH(numPacketsDequeued);

    // module references
    tsAlgorithm = getModuleFromPar<TSAlgorithm>(
            par("transmissionSelectionAlgorithmModule"), this);

    // statistics
    emit(queueLengthSignal, queue.getLength());
}

void QueueDetnet::enqueue(cPacket* packet) {
    if (availableBufferCapacity >= packet->getBitLength()) {
        emit(enqueuePkSignal, packet->getTreeId());
        numPacketsEnqueued++;
        queue.insert(packet);
        availableBufferCapacity -= packet->getBitLength();
        handlePacketEnqueuedEvent(packet);
        bytelength+=packet->getByteLength();
    } else {
        emit(dropPkByQueueSignal, packet->getTreeId());
        numPacketsDropped++;
        handlePacketEnqueuedEvent(packet);
        delete packet;
    }
    emit(queueLengthSignal, queue.getLength());
}

cPacket* QueueDetnet::dequeue() {
    if (queue.isEmpty()) {
        return nullptr;
    }

    cPacket* packet = static_cast<cPacket*>(queue.pop());
    availableBufferCapacity += packet->getBitLength();
//    numPacketsDequeued++;
    emit(queueLengthSignal, queue.getLength());
    bytelength-=packet->getByteLength();
    return packet;
}

}
// namespace nesting
