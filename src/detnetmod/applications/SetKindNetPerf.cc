/*
 * SetKind.cc
 *
 *  Created on: Nov 5, 2019
 *      Author: vamsi
 */

#include "SetKindNetPerf.h"
#include "inet/common/packet/Packet.h"

//using namespace omnet;
using namespace inet;

namespace detnetmod {

Define_Module(SetKindNetPerf);

void SetKindNetPerf::initialize() {
}

void SetKindNetPerf::handleMessage(cMessage *msg) {
    if (msg->isPacket()){
        std::ostringstream str;
        str << getId() << "-" << numSent;
        Packet *packet = check_and_cast<Packet *>(msg);
        packet->setName(str.str().c_str());
        numSent++;
//        packet->setKind(getId());
        EV_DETAIL << "This is a packet";
    }
    EV_DETAIL << "Arrival gate is " << msg->getArrivalGate()->getName();

    if(std::string(msg->getArrivalGate()->getName()) == std::string("UpperLayerInTcp")){
        EV_DETAIL << "Sent out TCP";
        send(msg, "LowerLayerOutTcp");

    }
    if(msg->getArrivalGate()->getName() == "UpperLayerInSctp")
            send(msg, "LowerLayerOutSctp");
    if(msg->getArrivalGate()->getName() == "UpperLayerInUdp")
                send(msg, "LowerLayerOutUdp");
}

}
