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

#include "VlanEtherTrafGenFullLoadDetnet.h"

#include <omnetpp/ccomponent.h>
#include <omnetpp/cexception.h>
#include <omnetpp/clog.h>
#include <omnetpp/cmessage.h>
#include <omnetpp/cnamedobject.h>
#include <omnetpp/cobjectfactory.h>
#include <omnetpp/cpacket.h>
#include <omnetpp/cpar.h>
#include <omnetpp/csimplemodule.h>
#include <omnetpp/cwatch.h>
#include <omnetpp/regmacros.h>
#include <omnetpp/simutil.h>
#include <cstdio>
#include <iostream>
#include <string>

#include "inet/linklayer/common/Ieee802Ctrl.h"

namespace detnetmod {

Define_Module(VlanEtherTrafGenFullLoadDetnet);

void VlanEtherTrafGenFullLoadDetnet::initialize() {
    // Signals
    sentPkSignal = registerSignal("sentPk");

    // Initialize sequence-number for generated packets
    seqNum = 0;

    // NED parameters
    etherType = &par("etherType");
    vlanTagEnabled = &par("vlanTagEnabled");
    pcp = &par("pcp");
    dei = &par("dei");
    vid = &par("vid");

    //NED parameters for packet generation
    srcip = par("srcip");
    destip = par("destip");
    srcport = par("srcport");
    destport = par("destport");

    packetLength = &par("packetLength");
    randomPacketLengthEnabled = par("randomPacketLengthEnabled");
    const char *destAddress = par("destAddress");
    if (!destMacAddress.tryParse(destAddress)) {
        throw new cRuntimeError("Invalid MAC Address");
    }

    // Statistics
    packetsSent = 0;
    WATCH(packetsSent);

    llcSocket.setOutputGate(gate("out"));

    llcSocket.open(-1, ssap);

}

void VlanEtherTrafGenFullLoadDetnet::handleMessage(cMessage *msg) {
    throw cRuntimeError("cannot handle messages.");
}

Packet* VlanEtherTrafGenFullLoadDetnet::generatePacket() {
    seqNum++;

    char msgname[40];
    sprintf(msgname, "%d-%ld", getId(), seqNum);


    // create new packet
    Packet *datapacket = new Packet(msgname,getId());
    long len;
    if (randomPacketLengthEnabled) {
        len = cComponent::intuniform(64, 1450, 0);
    } else {
        len = packetLength->intValue();
    }
    const auto& payload = makeShared<ByteCountChunk>(B(len));
    // set creation time
    auto timeTag = payload->addTag<CreationTimeTag>();
    timeTag->setCreationTime(simTime());

    datapacket->insertAtBack(payload);
///////////////////////////////////////////////////////////////////////////////////////////
//    auto udpHeader = makeShared<UdpHeader>(); // create new UDP header
//    udpHeader->setSourcePort(srcport);
//    udpHeader->setDestinationPort(destport);
//
//    Ipv4Address dest;
//    dest.set(destip);
//    Ipv4Address src ;
//    src.set(srcip);
//
//    udpHeader->setTotalLengthField(udpHeader->getChunkLength() + datapacket->getTotalLength());
//    udpHeader->setCrcMode(CRC_COMPUTED);
//    udpHeader->setCrc(0x0000); // make sure that the CRC is 0 in the Udp header before computing the CRC
//    auto udpData = datapacket->peekData();
//    auto Crc = computeCrc(&Protocol::ipv4, src, dest, udpHeader, udpData);
//    udpHeader->setCrc(Crc);
//
//    datapacket->addTagIfAbsent<PacketProtocolTag>()->setProtocol(&Protocol::udp);
//    datapacket->insertAtFront(udpHeader); // insert header into packet
//
////    datapacket->removeTagIfPresent<PacketProtocolTag>();
//
//
//    auto ipv4Header = makeShared<Ipv4Header>(); // create new IPv4 header
//
//    ipv4Header->setDestAddress(dest);
//    ipv4Header->setSrcAddress(src);
//    ipv4Header->setTimeToLive(32);
//    ipv4Header->setCrcMode(CRC_COMPUTED);
//    ipv4Header->setProtocol(&Protocol::udp);
//    ipv4Header->setTotalLengthField(ipv4Header->getChunkLength() + datapacket->getDataLength());
//    MemoryOutputStream ipv4HeaderStream;
//    Chunk::serialize(ipv4HeaderStream, ipv4Header);
//    uint16_t crc = TcpIpChecksum::checksum(ipv4HeaderStream.getData());
//    ipv4Header->setCrc(crc);
//    datapacket->addTagIfAbsent<PacketProtocolTag>()->setProtocol(&Protocol::ipv4);
//    datapacket->insertAtFront(ipv4Header);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//    datapacket->addTag<DispatchProtocolReq>()->setProtocol(&Protocol::ethernetMac); // send to protocol
//    datapacket->removeTagIfPresent<PacketProtocolTag>();
//    datapacket->addTagIfAbsent<PacketProtocolTag>()->setProtocol(&Protocol::ethernetMac);
    // TODO check if protocol is correct
    datapacket->addTagIfAbsent<DispatchProtocolReq>()->setProtocol(&Protocol::ieee8022);
    auto sapTag = datapacket->addTagIfAbsent<Ieee802SapReq>();
    sapTag->setSsap(ssap);
    sapTag->setDsap(dsap);

    auto macTag = datapacket->addTag<MacAddressReq>();
    macTag->setDestAddress(destMacAddress);

    uint8_t PCP;
    bool de;
    short VID;
    // create VLAN control info
    if (vlanTagEnabled->boolValue()) {
        auto ieee8021q = datapacket->addTag<VLANTagReq>();
        PCP = pcp->intValue();
        de = dei->boolValue();
        VID = vid->intValue();
        ieee8021q->setPcp(PCP);
        ieee8021q->setDe(de);
        ieee8021q->setVID(VID);
    }

    return datapacket;
}

void VlanEtherTrafGenFullLoadDetnet::requestPacket() {
    Enter_Method("requestPacket(...)");

    /*
     if(doNotSendFirstInitPacket) {
     doNotSendFirstInitPacket = false;
     return;
     }
     */

    Packet* packet = generatePacket();

    if(par("verbose")) {
        auto macTag = packet->findTag<MacAddressReq>();
        auto ieee8021qTag = packet->findTag<VLANTagReq>();
        EV_TRACE << getFullPath() << ": Send packet `" << packet->getName() << "' dest=" << macTag->getDestAddress()
        << " length=" << packet->getBitLength() << "B type= empty" << " vlan-tagged=false";
        if(ieee8021qTag) {
            EV_TRACE << " vlan-tagged=true" << " pcp=" << ieee8021qTag->getPcp()
            << " dei=" << ieee8021qTag->getDe() << " vid=" << ieee8021qTag->getVID();
        }
        EV_TRACE << endl;
    }
    emit(sentPkSignal, packet->getTreeId()); // getting tree id, because it doenn't get changed when packet is copied
    send(packet, "out");
    packetsSent++;
}

int VlanEtherTrafGenFullLoadDetnet::getNumPendingRequests() {
    // Requests are always served immediately,
    // therefore no pending requests exist.
    return 0;
}

bool VlanEtherTrafGenFullLoadDetnet::isEmpty() {
    // Queue is never empty
    return false;
}

cMessage* VlanEtherTrafGenFullLoadDetnet::pop() {
    return generatePacket();
}

uint16_t VlanEtherTrafGenFullLoadDetnet::computeCrc(const Protocol *networkProtocol, const L3Address& srcAddress, const L3Address& destAddress, const Ptr<const UdpHeader>& udpHeader, const Ptr<const Chunk>& udpData)
{
    auto pseudoHeader = makeShared<TransportPseudoHeader>();
    pseudoHeader->setSrcAddress(srcAddress);
    pseudoHeader->setDestAddress(destAddress);
    pseudoHeader->setNetworkProtocolId(networkProtocol->getId());
    pseudoHeader->setProtocolId(IP_PROT_UDP);
    pseudoHeader->setPacketLength(udpHeader->getChunkLength() + udpData->getChunkLength());
    // pseudoHeader length: ipv4: 12 bytes, ipv6: 40 bytes, other: ???
    if (networkProtocol == &Protocol::ipv4)
        pseudoHeader->setChunkLength(B(12));
    else if (networkProtocol == &Protocol::ipv6)
        pseudoHeader->setChunkLength(B(40));
    else
        throw cRuntimeError("Unknown network protocol: %s", networkProtocol->getName());

    MemoryOutputStream stream;
    Chunk::serialize(stream, pseudoHeader);
    Chunk::serialize(stream, udpHeader);
    Chunk::serialize(stream, udpData);
    uint16_t crc = TcpIpChecksum::checksum(stream.getData());

    // Excerpt from RFC 768:
    // If the computed  checksum  is zero,  it is transmitted  as all ones (the
    // equivalent  in one's complement  arithmetic).   An all zero  transmitted
    // checksum  value means that the transmitter  generated  no checksum  (for
    // debugging or for higher level protocols that don't care).
    return crc == 0x0000 ? 0xFFFF : crc;
}

} // namespace nesting
