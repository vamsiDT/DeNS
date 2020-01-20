/*
 * SetKind.cc
 *
 *  Created on: Nov 5, 2019
 *      Author: vamsi
 */

#include "SetKind.h"
#include "inet/common/packet/Packet.h"

//using namespace omnet;
using namespace inet;

namespace detnetmod {

Define_Module(SetKind);

void SetKind::initialize() {
}

void SetKind::handleMessage(cMessage *msg) {
    if (msg->isPacket()){
        std::ostringstream str;
        str << getId() << "-" << numSent;
        Packet *packet = check_and_cast<Packet *>(msg);
        packet->setName(str.str().c_str());
        numSent++;
//        packet->setKind(getId());
    }
    send(msg, "LowerLayerOut");
}

}
