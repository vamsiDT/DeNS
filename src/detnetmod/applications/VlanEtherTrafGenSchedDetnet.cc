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

#include "VlanEtherTrafGenSchedDetnet.h"

#define COMPILETIME_LOGLEVEL omnetpp::LOGLEVEL_TRACE

namespace detnetmod {

Define_Module(VlanEtherTrafGenSchedDetnet);

VlanEtherTrafGenSchedDetnet::~VlanEtherTrafGenSchedDetnet() {
    delete (jitterMsg);
}

void VlanEtherTrafGenSchedDetnet::initialize(int stage) {
    if (stage == INITSTAGE_LOCAL) {
        // Signals
        sentPkSignal = registerSignal("sentPk");
        rcvdPkSignal = registerSignal("rcvdPk");

        seqNum = 0;
        //WATCH(seqNum);

        jitter = par("jitter");

        // statistics
        TSNpacketsSent = packetsReceived = 0;
        WATCH(TSNpacketsSent);
        WATCH(packetsReceived);

        //NED parameters for packet generation
        srcip = par("srcip");
        destip = par("destip");
        srcport = par("srcport");
        destport = par("destport");

        vid = par("vid");

        cModule* clockModule = getModuleFromPar<cModule>(par("clockModule"),
                this);
        clock = check_and_cast<IClock*>(clockModule);

        llcSocket.setOutputGate(gate("out"));
    } else if (stage == INITSTAGE_LINK_LAYER) {
        //clock module reference from ned parameter

        currentSchedule = std::unique_ptr < HostSchedule
                < Ieee8021QCtrl >> (new HostSchedule<Ieee8021QCtrl>());
        cXMLElement* xml = par("initialSchedule").xmlValue();
        loadScheduleOrDefault(xml);

        currentSchedule = move(nextSchedule);
        nextSchedule.reset();

        clock->subscribeTick(this,
                scheduleNextTickEvent().raw() / clock->getClockRate().raw());

        llcSocket.open(-1, ssap);
    }
}

int VlanEtherTrafGenSchedDetnet::numInitStages() const {
    return INITSTAGE_LINK_LAYER + 1;
}

void VlanEtherTrafGenSchedDetnet::handleMessage(cMessage *msg) {
    if (msg->isSelfMessage()) {
        if (msg == jitterMsg) {
            sendDelayed();
        }

    } else {
        receivePacket(check_and_cast<Packet *>(msg));
    }
}

void VlanEtherTrafGenSchedDetnet::sendPacket() {

    char msgname[40];
    sprintf(msgname, "%d-%d", getId(), seqNum);

    // create new packet
//    Packet *datapacket = new Packet(msgname, IEEE802CTRL_DATA);
    Packet *datapacket = new Packet(msgname,getId());
    long len = currentSchedule->getSize(index);
    const auto& payload = makeShared<ByteCountChunk>(B(len));
    // set creation time
    auto timeTag = payload->addTag<CreationTimeTag>();
    timeTag->setCreationTime(simTime());

    datapacket->insertAtBack(payload);
//////////////////////////////////////////////////////////////////////////////////////////
//
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
////////////////////////////////////////////////////////////////////////////////////////////////


//    datapacket->addTag<DispatchProtocolReq>()->setProtocol(&Protocol::ethernetMac); // send to protocol
//    datapacket->removeTagIfPresent<PacketProtocolTag>();
//    datapacket->addTagIfAbsent<PacketProtocolTag>()->setProtocol(&Protocol::ethernetMac);
    // TODO check if protocol is correct
    datapacket->addTagIfAbsent<DispatchProtocolReq>()->setProtocol(&Protocol::ieee8022);
    auto sapTag = datapacket->addTagIfAbsent<Ieee802SapReq>();
    sapTag->setSsap(ssap);
    sapTag->setDsap(dsap);

    seqNum++;

    // get scheduled control data
    Ieee8021QCtrl header = currentSchedule->getScheduledObject(index);
    // create mac control info
    auto macTag = datapacket->addTag<MacAddressReq>();
    macTag->setDestAddress(header.macTag.getDestAddress());
    // create VLAN control info
    auto ieee8021q = datapacket->addTag<VLANTagReq>();
    ieee8021q->setPcp(header.q1Tag.getPcp());
    ieee8021q->setDe(header.q1Tag.getDe());
    ieee8021q->setVID(vid);
EV_TRACE << "pcp="<< (int)(header.q1Tag.getPcp())<< "vid="<< (int)(ieee8021q->getVID());
//    PcapDump dump;
//    dump.openPcap("/home/vamsi/src/detnet/detnetmod/simulations/out.pcap", 65535,0); // maximum length and PCAP type
//    dump.setFlushParameter(true);
//    dump.writePacket(simTime(), datapacket); // record with current time
//    dump.closePcap();


    EV_TRACE << getFullPath() << ": Send TSN packet '" << datapacket->getName()
                    << "' at time " << clock->getTime().inUnit(SIMTIME_US)
                    << endl;

    emit(sentPkSignal, datapacket->getTreeId()); // getting tree id, because it doenn't get changed when packet is copied
    send(datapacket, "out");
    TSNpacketsSent++;
}

void VlanEtherTrafGenSchedDetnet::receivePacket(Packet *msg) {
    EV_TRACE << getFullPath() << ": Received packet '" << msg->getName()
                    << "' with length " << msg->getByteLength() << "B at time "
                    << clock->getTime().inUnit(SIMTIME_US) << endl;

    packetsReceived++;
    emit(rcvdPkSignal, msg->getTreeId());

    delete msg;
}

void VlanEtherTrafGenSchedDetnet::tick(IClock *clock) {
    Enter_Method("tick()");
    // When the current schedule index is 0, this means that the current
    // schedule's cycle was not started or was just finished. Therefore in this
    // case a new schedule is loaded if available.
    if (index == currentSchedule->size() ) {
        // Load new schedule and delete the old one.
        if (nextSchedule) {
            currentSchedule = move(nextSchedule);
            nextSchedule.reset();
        }
        index = 0;
        clock->subscribeTick(this, scheduleNextTickEvent().raw() / clock->getClockRate().raw());

    }
    else {
        double delay = (double)rand() / (double)RAND_MAX;
        double jitter_delay = delay * jitter;
        scheduleAt(simTime() + jitter_delay, jitterMsg);

    }
}

void VlanEtherTrafGenSchedDetnet::sendDelayed() {

    sendPacket();
    index++;
    clock->subscribeTick(this, scheduleNextTickEvent().raw() / clock->getClockRate().raw());

}

/* This method returns the timeinterval between
 * the last sent frame and the frame to be sent next */
simtime_t VlanEtherTrafGenSchedDetnet::scheduleNextTickEvent() {
    if (currentSchedule->size() == 0) {
        return currentSchedule->getCycle();
    } else if (index == currentSchedule->size()) {
        return currentSchedule->getCycle() - currentSchedule->getTime(index - 1);
    } else if (index % currentSchedule->size() == 0) {
        return currentSchedule->getTime(index);
    } else {
        return currentSchedule->getTime(index % currentSchedule->size())
                - currentSchedule->getTime(index % currentSchedule->size() - 1);
    }
}

void VlanEtherTrafGenSchedDetnet::loadScheduleOrDefault(cXMLElement* xml) {
    std::string hostName =
            this->getModuleByPath(par("hostModule"))->getFullName();
    HostSchedule<Ieee8021QCtrl>* schedule;
    bool realScheduleFound = false;
    //try to extract the part of the schedule belonging to this host
    for (cXMLElement* hostxml : xml->getChildren()) {
        if (strcmp(hostxml->getTagName(), "defaultcycle") != 0
                && hostxml->getAttribute("name") == hostName) {
            schedule = HostScheduleBuilder::createHostScheduleFromXML(hostxml,
                    xml);

            EV_DEBUG << getFullPath() << ": Found schedule for name "
                            << hostName << endl;

            realScheduleFound = true;
            break;
        }
    }
    //load empty schedule if there is no part that affects this host in the schedule xml
    if (!realScheduleFound) {
        cXMLElement* defaultXml = par("emptySchedule").xmlValue();
        schedule = HostScheduleBuilder::createHostScheduleFromXML(defaultXml,
                xml);
    }
    std::unique_ptr<HostSchedule<Ieee8021QCtrl>> schedulePtr(schedule);

    nextSchedule.reset();
    nextSchedule = move(schedulePtr);

}

uint16_t VlanEtherTrafGenSchedDetnet::computeCrc(const Protocol *networkProtocol, const L3Address& srcAddress, const L3Address& destAddress, const Ptr<const UdpHeader>& udpHeader, const Ptr<const Chunk>& udpData)
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
