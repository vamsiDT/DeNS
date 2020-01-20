/*
 * macrelay.cc
 *
 *  Created on: Oct 20, 2019
 *      Author: vamsi
 */
#include "macrelay.h"
//#define COMPILETIME_LOGLEVEL omnetpp::LOGLEVEL_TRACE
#include "inet/linklayer/ieee8022/Ieee8022LlcSocket.h"
namespace detnetmod{

Define_Module(macrelay);

void macrelay::initialize(int stage)
{
    LayeredProtocolBase::initialize(stage);

    if (stage == INITSTAGE_LOCAL) {
        numProcessedFrames = numDiscardedFrames = 0;

        macTable = getModuleFromPar<IMacAddressTable>(par("macTableModule"), this);
        ifTable = getModuleFromPar<IInterfaceTable>(par("interfaceTableModule"), this);

        fdb = getModuleFromPar<FilteringDatabase>(par("filteringDatabaseModule"),
                this);
        numberOfPorts = par("numberOfPorts");
        WATCH(numProcessedFrames);
        WATCH(numDiscardedFrames);
    }
    else if (stage == INITSTAGE_LINK_LAYER) {
        registerService(Protocol::ethernetMac, nullptr, gate("ifIn"));
        registerProtocol(Protocol::ethernetMac, gate("ifOut"), nullptr);
    }
}

void macrelay::handleLowerPacket(Packet *packet)
{
    handleAndDispatchFrame(packet);
    updateDisplayString();
}

void macrelay::updateDisplayString() const
{
    auto text = StringFormat::formatString(par("displayStringTextFormat"), [&] (char directive) {
        static std::string result;
        switch (directive) {
            case 'p':
                result = std::to_string(numProcessedFrames);
                break;
            case 'd':
                result = std::to_string(numDiscardedFrames);
                break;
            default:
                throw cRuntimeError("Unknown directive: %c", directive);
        }
        return result.c_str();
    });
    getDisplayString().setTagArg("t", 0, text);
}

void macrelay::broadcast(Packet *packet, int arrivalInterfaceId)
{
    EV_DETAIL << "Broadcast frame " << packet << endl;


    auto vlanTagIn = packet->removeTagIfPresent<VLANTagInd>();


    auto oldPacketProtocolTag = packet->removeTag<PacketProtocolTag>();
    packet->clearTags();
    auto newPacketProtocolTag = packet->addTag<PacketProtocolTag>();
    *newPacketProtocolTag = *oldPacketProtocolTag;
    if(vlanTagIn){
                    auto vlanTagOut = packet->addTag<VLANTagReq>();
                    vlanTagOut->setPcp(vlanTagIn->getPcp());
                    vlanTagOut->setVID(vlanTagIn->getVID());
                    vlanTagOut->setDe(vlanTagIn->getDe());
                    delete vlanTagIn;
                }
    delete oldPacketProtocolTag;

//    packet->removeTagIfPresent<DispatchProtocolReq>();
//        packet->addTagIfAbsent<PacketProtocolTag>()->setProtocol(protocol);
     packet->removeTagIfPresent<InterfaceReq>();
//     packet->addTagIfAbsent<InterfaceReq>()->setInterfaceId(outputInterfaceId);
//     auto macIndicator = packet->addTagIfAbsent<MacAddressInd>();
//     macIndicator->setSrcAddress(frame->getSrc());
//     macIndicator->setDestAddress(frame->getDest());

     packet->trim();



    int numPorts = numberOfPorts;
    for (int i = 0; i < numPorts; i++) {
        InterfaceEntry *ie = ifTable->getInterface(i);
            int outputInterfaceId = ie->getInterfaceId();
        if (ie->isLoopback() || !ie->isBroadcast())
            continue;

        if (arrivalInterfaceId != outputInterfaceId) {
            Packet *dupFrame = packet->dup();
            dupFrame->addTagIfAbsent<InterfaceReq>()->setInterfaceId(outputInterfaceId);
            emit(packetSentToLowerSignal, dupFrame);
            send(dupFrame, "ifOut");
        }
    }
    delete packet;
}

void macrelay::handleAndDispatchFrame(Packet *packet)
{
    //FIXME : should handle multicast mac addresses correctly

    const auto& frame = packet->peekAtFront<EthernetMacHeader>();
    int arrivalInterfaceId = packet->getTag<InterfaceInd>()->getInterfaceId();
    EV_DETAIL << arrivalInterfaceId << "\t" << "arrivalport";
    numProcessedFrames++;

        auto vlanTagIn = packet->removeTagIfPresent<VLANTagInd>();
        int pcpValue;
        // FIXME:DetnetModule
        if(vlanTagIn){
            pcpValue = vlanTagIn->getPcp();
            auto vlanTagOut = packet->addTag<VLANTagReq>();
            vlanTagOut->setPcp(pcpValue);
            vlanTagOut->setDe(vlanTagIn->getDe());
            vlanTagOut->setVID(vlanTagIn->getVID());
            delete vlanTagIn;
        }


    // update address table
    learn(frame->getSrc(), arrivalInterfaceId);

    // handle broadcast frames first
    if (frame->getDest().isBroadcast()) {
        EV << "Broadcasting broadcast frame " << frame << endl;
        broadcast(packet, arrivalInterfaceId);
        return;
    }
//TODO
    else if (frame->getDest().isMulticast()){
        processMulticast(packet,frame->getDest(),arrivalInterfaceId);
    }
    // Finds output port of destination address and sends to output port
    // if not found then broadcasts to all other ports instead

//    InterfaceEntry *ie = ifTable->getInterface(fdb->getPort(frame->getDest(), simTime()));

    int outputInterfaceId = macTable->getPortForAddress(frame->getDest());

//    int outputInterfaceId = ie->getInterfaceId();

    // should not send out the same frame on the same ethernet port
    // (although wireless ports are ok to receive the same message)
    if (arrivalInterfaceId == outputInterfaceId) {
        EV << "Output port is same as input port, " << packet->getFullName()
           << " dest " << frame->getDest() << ", discarding frame\n";
        numDiscardedFrames++;
        delete packet;
        return;
    }

    if (outputInterfaceId >= 0) {
        EV << "Sending frame " << frame << " with dest address " << frame->getDest() << " to port " << outputInterfaceId << endl;
//        auto oldPacketProtocolTag = packet->removeTag<PacketProtocolTag>();
//        packet->clearTags();
//        auto newPacketProtocolTag = packet->addTag<PacketProtocolTag>();
//        *newPacketProtocolTag = *oldPacketProtocolTag;
//        delete oldPacketProtocolTag;

        packet->removeTagIfPresent<DispatchProtocolReq>();
//        packet->addTagIfAbsent<PacketProtocolTag>()->setProtocol(protocol);
        packet->removeTagIfPresent<InterfaceReq>();
        packet->addTagIfAbsent<InterfaceReq>()->setInterfaceId(outputInterfaceId);
        auto macIndicator = packet->addTagIfAbsent<MacAddressInd>();
        macIndicator->setSrcAddress(frame->getSrc());
        macIndicator->setDestAddress(frame->getDest());

        packet->trim();
        emit(packetSentToLowerSignal, packet);
        send(packet, "ifOut");
    }
    else {
        EV << "Dest address " << frame->getDest() << " unknown, broadcasting frame " << packet << endl;
        broadcast(packet, arrivalInterfaceId);
    }
}

void macrelay::processMulticast(Packet* packet,MacAddress destAddr,int arrivalInterfaceId) {

    std::vector<int> forwardingPorts = fdb->getPorts(destAddr,simTime());

    if (forwardingPorts.at(0) == -1) {
        throw cRuntimeError(
                "Static multicast forwarding for packet didn't work. Entry in forwarding table was empty!");
    } else {
        std::string forwardingPortsString = "";
        for (auto forwardingPort : forwardingPorts) {
            // skip arrival gate
            if (forwardingPort == arrivalInterfaceId) {
                continue;
            }

            Packet* dupPacket = packet->dup();
//            auto oldPacketProtocolTag = dupPacket->removeTag<PacketProtocolTag>();
//            dupPacket->clearTags();
//            auto newPacketProtocolTag = dupPacket->addTag<PacketProtocolTag>();
//            *newPacketProtocolTag = *oldPacketProtocolTag;
//            delete oldPacketProtocolTag;
            dupPacket->addTagIfAbsent<InterfaceReq>()->setInterfaceId(forwardingPort);
            dupPacket->trim();
            emit(packetSentToLowerSignal, dupPacket);
            send(dupPacket, "ifOut");
            forwardingPortsString = forwardingPortsString.append(std::to_string(forwardingPort));
        }
        EV_INFO << getFullPath() << ": Forwarding multicast packet `" << packet
                       << "` to ports " << forwardingPortsString << endl;
    }
    delete packet;
}

void macrelay::start()
{
//    macTable->clearTable();
}

void macrelay::stop()
{
//    macTable->clearTable();
}

void macrelay::learn(MacAddress srcAddr, int arrivalInterfaceId)
{
//Learning MAC port mappings
    macTable->updateTableWithAddress(arrivalInterfaceId, srcAddr);

//fdb->insert(srcAddr, simTime(),arrivalInterfaceId);
}

void macrelay::finish()
{
    recordScalar("processed frames", numProcessedFrames);
    recordScalar("discarded frames", numDiscardedFrames);
}


} // namespace nesting
