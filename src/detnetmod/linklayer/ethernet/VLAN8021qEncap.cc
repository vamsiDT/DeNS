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

#include "VLAN8021qEncap.h"

#include "inet/applications/common/SocketTag_m.h"
#include "inet/common/INETUtils.h"
#include "inet/common/IProtocolRegistrationListener.h"
#include "inet/common/ModuleAccess.h"
#include "inet/common/ProtocolTag_m.h"
#include "inet/common/checksum/EthernetCRC.h"
#include "inet/linklayer/common/FcsMode_m.h"
#include "inet/linklayer/common/Ieee802Ctrl.h"
#include "inet/linklayer/common/Ieee802SapTag_m.h"
#include "inet/linklayer/common/InterfaceTag_m.h"
#include "inet/linklayer/common/MacAddressTag_m.h"
#include "inet/linklayer/ethernet/EtherEncap.h"
#include "inet/linklayer/ethernet/EtherFrame_m.h"
#include "inet/linklayer/ethernet/EthernetCommand_m.h"
#include "inet/linklayer/ethernet/EtherPhyFrame_m.h"
#include "inet/linklayer/ieee8022/Ieee8022LlcHeader_m.h"

#include "inet/networklayer/contract/IInterfaceTable.h"
#include <algorithm>

#define COMPILETIME_LOGLEVEL omnetpp::LOGLEVEL_TRACE

using namespace inet;

namespace detnetmod {

Define_Module(VLAN8021qEncap);

void VLAN8021qEncap::processPacket(Packet *packet, std::vector<int>& vlanIdFilter, std::map<int, int>& vlanIdMap, cGate *gate)
{
    packet->trimFront();
    const auto& ethernetMacHeader = packet->removeAtFront<EthernetMacHeader>();

    /* Vlan Tag */
    Ieee8021qHeader *vlanTag = findVlanTag(ethernetMacHeader);

    /* Vlan Id*/
    auto oldVlanId = vlanTag != nullptr ? vlanTag->getVid() : -1;
    auto vlanReq = packet->removeTagIfPresent<VLANTagReq>();
    auto newVlanId = vlanReq != nullptr ? vlanReq->getVID() : oldVlanId;

    /* PCP field in Vlan Tag*/
    auto oldPcp = vlanTag != nullptr ? vlanTag->getPcp() : 1;
    auto newPcp = vlanReq != nullptr ? vlanReq->getPcp() : oldPcp;

    /*De field in Vlan Tag*/
    auto oldDe = vlanTag != nullptr ? vlanTag->getDe() : false;
    auto newDe = vlanReq != nullptr ? vlanReq->getDe() : oldDe;

    bool acceptPacket = vlanIdFilter.empty() || std::find(vlanIdFilter.begin(), vlanIdFilter.end(), newVlanId) != vlanIdFilter.end();

    if (acceptPacket) {
        auto it = vlanIdMap.find(newVlanId);
        if (it != vlanIdMap.end())
            newVlanId = it->second;
        if ((newVlanId != oldVlanId) || (newPcp != oldPcp)) {
            EV_WARN << "Changing VLAN ID: new = " << newVlanId << ", old = " << oldVlanId << ".\n";
            if ((oldVlanId == -1 && newVlanId != -1)|| (oldPcp == 1 && newPcp != 1) ){
                Ieee8021qHeader *vTag = addVlanTag(ethernetMacHeader);
                vTag->setVid(newVlanId);
                vTag->setPcp(newPcp);
                vTag->setDe(newDe);
            }
            else if ((oldVlanId != -1 && newVlanId == -1) || (oldPcp !=1 && newPcp == 1))
                removeVlanTag(ethernetMacHeader);
            else{
                vlanTag->setVid(newVlanId);
                vlanTag->setPcp(newPcp);
                vlanTag->setDe(newDe);
            }
            packet->insertAtFront(ethernetMacHeader);
            auto oldFcs = packet->removeAtBack<EthernetFcs>();
            EtherEncap::addFcs(packet, oldFcs->getFcsMode());
        }
        else
            packet->insertAtFront(ethernetMacHeader);

        auto newVlanTag = packet->addTagIfAbsent<VLANTagInd>();
        newVlanTag->setVID(newVlanId);
        newVlanTag->setPcp(newPcp);
        newVlanTag->setDe(newDe);
        send(packet, gate);
    }
    else {
        EV_WARN << "Received VLAN ID = " << oldVlanId << " is not accepted, dropping packet.\n";
        PacketDropDetails details;
        details.setReason(OTHER_PACKET_DROP);
        emit(packetDroppedSignal, packet, &details);
        delete packet;
    }
}

void VLAN8021qEncap::handleMessage(cMessage *message)
{
    if (message->getArrivalGate()->isName("upperLayerIn"))
        processPacket(check_and_cast<Packet *>(message), outboundVlanIdFilter, outboundVlanIdMap, gate("lowerLayerOut"));
    else if (message->getArrivalGate()->isName("lowerLayerIn"))
        processPacket(check_and_cast<Packet *>(message), inboundVlanIdFilter, inboundVlanIdMap, gate("upperLayerOut"));
    else
        throw cRuntimeError("Unknown message");
}

} // namespace nesting
