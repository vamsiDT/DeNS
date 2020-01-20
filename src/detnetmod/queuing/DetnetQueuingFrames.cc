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

#include "DetnetQueuingFrames.h"
#define COMPILETIME_LOGLEVEL omnetpp::LOGLEVEL_TRACE

namespace detnetmod {

Define_Module(DetnetQueuingFrames);

void DetnetQueuingFrames::initialize() {
    //Initialize the number of queues, which is equal to the number of "out"
    // vector gate. The default values is 8, if user has not specified it.
    numberOfQueues = gateSize("out");

    //Precondition: numberOfQueues must have a valid number, i.e <= number of
    // all possible pcp values.

    if (numberOfQueues > kNumberOfPCPValues || numberOfQueues < 1) {
        throw new cRuntimeError(
                "Invalid assignment of numberOfQueues. Number of queues should not "
                        "be bigger than the number of all possible pcp values!");
    }
}

void DetnetQueuingFrames::handleMessage(cMessage *msg) {
    if (dynamic_cast<inet::Request*>(msg)) {
        delete msg;
        return;
    }

    inet::Packet *packet = check_and_cast<inet::Packet *>(msg);

int pcpValue=1;
    /////////////////// This could work. Need to reposition the queuing module.
    packet->trimFront();
        const auto& ethernetMacHeader = packet->removeAtFront<inet::EthernetMacHeader>();

        auto vlanHeaderS = ethernetMacHeader->getSTag();
        auto vlanHeaderC = ethernetMacHeader->getCTag();
        if(vlanHeaderS){
        pcpValue = vlanHeaderS->getPcp();
//        packet->insertAtFront(ethernetMacHeader);
        packet->insertAtFront(ethernetMacHeader);
        auto oldFcs = packet->removeAtBack<inet::EthernetFcs>();
        inet::EtherEncap::addFcs(packet, oldFcs->getFcsMode());
//    packet->trim();
        }
        else if (vlanHeaderC){
            pcpValue = vlanHeaderC->getPcp();
//            packet->insertAtFront(ethernetMacHeader);
            packet->insertAtFront(ethernetMacHeader);
            auto oldFcs = packet->removeAtBack<inet::EthernetFcs>();
            inet::EtherEncap::addFcs(packet, oldFcs->getFcsMode());
//        packet->trim();
        }
        else{
            packet->insertAtFront(ethernetMacHeader);
                    auto oldFcs = packet->removeAtBack<inet::EthernetFcs>();
                    inet::EtherEncap::addFcs(packet, oldFcs->getFcsMode());
            pcpValue=1;

        }
        EV_DETAIL << "PCP VALUE = " << pcpValue;
    // switch ingoing VLAN Tag to outgoing Tag
//    auto vlanTagIn = packet->removeTagIfPresent<VLANTagInd>();
//    auto vlanTagHighin = packet->findTag<VLANTagReq>();
//    int pcpValue;
//    // FIXME:DetnetModule
//    if(vlanTagIn){
//        pcpValue = vlanTagIn->getPcp();
//        auto vlanTagOut = packet->addTag<VLANTagReq>();
//        vlanTagOut->setPcp(pcpValue);
//        vlanTagOut->setDe(vlanTagIn->getDe());
//        vlanTagOut->setVID(vlanTagIn->getVID());
//        delete vlanTagIn;
//    }
//    else if (vlanTagHighin){
//        pcpValue = vlanTagHighin->getPcp();
////        auto vlanTagOut = packet->addTag<VLANTagReq>();
////        vlanTagOut->setPcp(pcpValue);
////        vlanTagOut->setDe(vlanTagIn->getDe());
////        vlanTagOut->setVID(vlanTagIn->getVID());
////        delete vlanTagHighin;
//    }
//    else{
//        pcpValue=6;
//        auto vlanTagOut = packet->addTag<VLANTagReq>();
//        vlanTagOut->setPcp(pcpValue);
//        vlanTagOut->setDe(false);
//        vlanTagOut->setVID(1);
//    }
//    // switch ingoing MAC Tag to outgoing MAC Tag
////    auto macTagIn = packet->removeTag<inet::MacAddressInd>();
////    auto macTagOut = packet->addTag<inet::MacAddressReq>();
////    macTagOut->setDestAddress(macTagIn->getDestAddress());
////    macTagOut->setSrcAddress(macTagIn->getSrcAddress());
//
//    // switch ingoing sap tag to outgoing sap tag
////    auto sapTagIn = packet->removeTag<inet::Ieee802SapInd>();
////    auto sapTagOut = packet->addTag<inet::Ieee802SapReq>();
////    sapTagOut->setDsap(sapTagIn->getDsap());
////    sapTagOut->setSsap(sapTagIn->getSsap());
////    delete macTagIn;
//
////    delete sapTagIn;
//
//    // remove encapsulation
//    packet->trim();

    // Check whether the PCP value is correct.
    if (pcpValue > kNumberOfPCPValues) {
        throw new cRuntimeError(
                "Invalid assignment of PCP value. The value of PCP should not be "
                        "bigger than the number of supported queues.");
    }

    // Get the corresponding queue from the 2-dimensional matrix
    // standardTrafficClassMapping.
    int queueIndex =
            this->standardTrafficClassMapping[numberOfQueues - 1][pcpValue];

    // Get the corresponding gate and transmit the frame to it.
    EV_TRACE << getFullPath() << ": Sending packet '" << packet
                    << "' with pcp value '" << pcpValue << "' to queue "
                    << queueIndex << endl;

    cGate* outputGate = gate("out", queueIndex);
    send(msg, outputGate);
}

} // namespace nesting
