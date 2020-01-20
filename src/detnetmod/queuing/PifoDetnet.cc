/*
 * PifoDetnet.cc
 *
 *  Created on: Jan 3, 2020
 *      Author: vamsi
 */

#include "inet/common/INETDefs.h"
#include "PifoDetnet.h"

namespace detnetmod {

Define_Module(PifoDetnet);

PifoDetnet::~PifoDetnet() {
    cancelEvent(&requestPacketMsg);
    while (!queue.isEmpty()) {
        delete queue.pop();
    }
    queue.clear();
}

void PifoDetnet::initialize()
{

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

    bytelength = 0;
}

void PifoDetnet::enqueue(cPacket* packet)
{
    Packet *netpacket = check_and_cast<Packet *>(packet);
    int pcpValue=1;
    netpacket->trimFront();
    const auto& ethernetMacHeader = netpacket->removeAtFront<inet::EthernetMacHeader>();
    auto vlanHeaderS = ethernetMacHeader->getSTag();
    auto vlanHeaderC = ethernetMacHeader->getCTag();
    if(vlanHeaderS){
        pcpValue = vlanHeaderS->getPcp();
        netpacket->insertAtFront(ethernetMacHeader);
        auto oldFcs = netpacket->removeAtBack<inet::EthernetFcs>();
        inet::EtherEncap::addFcs(netpacket, oldFcs->getFcsMode());
    }
    else if (vlanHeaderC){
        pcpValue = vlanHeaderC->getPcp();
        netpacket->insertAtFront(ethernetMacHeader);
        auto oldFcs = netpacket->removeAtBack<inet::EthernetFcs>();
        inet::EtherEncap::addFcs(netpacket, oldFcs->getFcsMode());
    }
    else{
        netpacket->insertAtFront(ethernetMacHeader);
        auto oldFcs = netpacket->removeAtBack<inet::EthernetFcs>();
        inet::EtherEncap::addFcs(netpacket, oldFcs->getFcsMode());
        pcpValue=1;
    }

    if (availableBufferCapacity >= packet->getBitLength()) {
        emit(enqueuePkSignal, packet->getTreeId());
        numPacketsEnqueued++;

        if(pcpValue=7){
            if(!queue.isEmpty()){
//                if(enqueuedDetnet)
//                    queue.insertAfter(enqueuedDetnet, packet);
//                else
                queue.insertBefore(queue.front(), packet);
            }
            else
                queue.insert(packet);

//            enqueuedDetnet=packet;
        }
        else
            queue.insert(packet);
        availableBufferCapacity -= packet->getBitLength();
        handlePacketEnqueuedEvent(packet);
        bytelength+=packet->getByteLength();
        }
    else {
        if(pcpValue==7){
            uint64_t pktlen = packet->getBitLength();
            uint64_t rembits=0;
            while(rembits<=pktlen){
                cPacket* tail= check_and_cast<cPacket *> (queue.back());
//                if(tail==enqueuedDetnet){
//                    enqueuedDetnet=nullptr;
//                    EV_INFO<< "#################This is queue dequeue printing pcp = "<< "7";
//                }
                rembits+=tail->getBitLength();
                queue.remove(tail);
                delete tail;
            }
            availableBufferCapacity+=rembits;
            if(!queue.isEmpty()){
//                if(enqueuedDetnet)
//                    queue.insertAfter(enqueuedDetnet, packet);
//                else
                queue.insertBefore(queue.front(), packet);
            }
            else
                queue.insert(packet);
//            enqueuedDetnet=packet;
            availableBufferCapacity -= packet->getBitLength();
            handlePacketEnqueuedEvent(packet);
        }
        else{
            emit(dropPkByQueueSignal, packet->getTreeId());
            numPacketsDropped++;
            handlePacketEnqueuedEvent(packet);
            delete packet;
        }
    }
    emit(queueLengthSignal, queue.getLength());
}

cPacket* PifoDetnet::dequeue() {
    if (queue.isEmpty()) {
        return nullptr;
    }

    cPacket* packet = static_cast<cPacket*>(queue.pop());
//
//    if(packet==enqueuedDetnet){
//        enqueuedDetnet=nullptr;
//        EV_INFO<< "#################This is queue dequeue printing pcp = "<< "7";
//    }


    availableBufferCapacity += packet->getBitLength();
//    numPacketsDequeued++;
    emit(queueLengthSignal, queue.getLength());
    bytelength-=packet->getByteLength();
    return packet;
}


} // namespace detnetmod
