/*
 * PcpMarker.cc
 *
 *  Created on: Nov 7, 2019
 *      Author: vamsi
 */


#include "PcpMarker.h"
#include "inet/common/packet/Message.h"
#include "inet/common/packet/Packet.h"
#include "inet/common/packet/Message_m.h"
#include "inet/common/IProtocolRegistrationListener.h"
#include "inet/common/ModuleAccess.h"
#include "inet/common/ProtocolGroup.h"
#include "inet/common/ProtocolTag_m.h"

namespace detnetmod {

Define_Module(PcpMarker);

void PcpMarker::initialize() {
    NumClasses = gateSize("in");
    int i=0;
    while (i<NumClasses){
        std::string parameter = "MarkPcp";
        parameter.append(std::to_string(i));
        /*Default PCP is 1 if it is not specified in configuration file*/
        PCPValues.push_back(par(parameter.c_str()));
        i++;
    }
    llcSocket.setOutputGate(gate("out"));

    llcSocket.open(-1, ssap);
//    registerService(Protocol::ipv4, nullptr, gate("out"));
//     registerProtocol(Protocol::ipv4, gate("out"), nullptr);
}
void PcpMarker::handleMessage(cMessage *msg)
{

    if (dynamic_cast<inet::Request*>(msg)) {
        send(msg,gate("out"));
        return;
    }
    Packet *packet = check_and_cast<Packet *>(msg);
    const int Class = packet->getArrivalGate()->getIndex();
//    EV_INFO << "Packet Arrived on gate = ", Class;
    auto vlanTag = packet->addTagIfAbsent<VLANTagReq>();
    vlanTag->setPcp(PCPValues[Class]);
    send(msg,gate("out"));
}
}
