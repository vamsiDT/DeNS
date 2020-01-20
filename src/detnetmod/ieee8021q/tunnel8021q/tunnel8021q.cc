/*
 * tunnel8021q.cc
 *
 *  Created on: Oct 21, 2019
 *      Author: vamsi
 */


#include "inet/applications/common/SocketTag_m.h"
#include "inet/common/ModuleAccess.h"
#include "inet/common/packet/Packet.h"
#include "inet/linklayer/common/InterfaceTag_m.h"
#include "inet/linklayer/vlan/VlanTag_m.h"
#include "inet/linklayer/vlan/VlanTunnel.h"
#include "inet/networklayer/contract/IInterfaceTable.h"

#include "tunnel8021q.h"
using namespace inet;
//using namespace nesting;

namespace detnetmod {

Define_Module(tunnel8021q);

void tunnel8021q::initialize(int stage)
{

    inet::VlanTunnel::initialize(stage);
    if (stage == INITSTAGE_LOCAL) {
            vid = par("vid");
            de = par("de");
            pcp = par("pcp");
        }

}

void tunnel8021q::handleMessage(cMessage *message)
{
    if (socket.belongsToSocket(message))
        socket.processMessage(message);
    else {
        auto packet = check_and_cast<Packet *>(message);
        auto vlanTag = packet->addTagIfAbsent<VLANTagReq>();
//        auto vlanTag = packet->findTag<VLANTagReq>();
        vlanTag->setVID(vid);
        vlanTag->setDe(de);
        vlanTag->setPcp(pcp);
        EV_DETAIL << "Pcp Set to - " << pcp;

        socket.send(packet);
    }
}

void tunnel8021q::socketDataArrived(EthernetSocket *socket, Packet *packet)
{
    packet->removeTag<SocketInd>();
    packet->getTag<InterfaceInd>()->setInterfaceId(interfaceEntry->getInterfaceId());
    send(packet, "upperLayerOut");
}

void tunnel8021q::socketErrorArrived(EthernetSocket *socket, Indication *indication)
{
    throw cRuntimeError("Invalid operation");
}





}
