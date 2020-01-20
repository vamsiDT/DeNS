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

#ifndef __MAIN_ETHERTRAFGENQUEUE_H_
#define __MAIN_ETHERTRAFGENQUEUE_H_

#include <omnetpp.h>
#include <list>

#include "inet/applications/ethernet/EtherTrafGen.h"
#include "inet/common/ModuleAccess.h"
#include "inet/common/InitStages.h"
#include "inet/common/packet/chunk/ByteCountChunk.h"
#include "inet/common/Protocol.h"
#include "inet/common/ProtocolTag_m.h"
#include "inet/linklayer/common/Ieee802SapTag_m.h"
#include "inet/common/TimeTag_m.h"
#include <omnetpp/cxmlelement.h>
#include <vector>
#include "nesting/common/schedule/HostSchedule.h"
#include "nesting/common/schedule/HostScheduleBuilder.h"
#include "nesting/ieee8021q/clock/IClock.h"

#include "inet/common/packet/recorder/PcapDump.h"
#include "inet/common/queue/IPassiveQueue.h"
#include "inet/linklayer/common/MacAddress.h"
#include "inet/common/lifecycle/ILifecycle.h"

#include "inet/networklayer/common/FragmentationTag_m.h"
#include "inet/networklayer/common/L3AddressResolver.h"
#include "inet/transportlayer/contract/udp/UdpControlInfo_m.h"
#include "inet/applications/base/ApplicationBase.h"
#include "inet/transportlayer/contract/udp/UdpSocket.h"
#include "inet/transportlayer/udp/UdpHeader_m.h"
#include "inet/transportlayer/udp/Udp.h"
#include "inet/networklayer/ipv4/Ipv4Header_m.h"
#include "inet/common/checksum/TcpIpChecksum.h"


using namespace omnetpp;
using namespace inet;
using namespace std;
using namespace nesting;

namespace detnetmod {

/** See the NED file for a detailed description */
class VlanEtherTrafGenFullLoadDetnet: public cSimpleModule, public IPassiveQueue {
protected:
    /** Sequence number for generated packets. */
    long seqNum;

    /** EtherMacBase requests a packet twice during init, tmp fix so that this module only delivers one packet */
    bool doNotSendFirstInitPacket = true;

    /** Destination MAC address of generated packets. */
    MacAddress destMacAddress;

    // Parameters from NED file
    cPar* etherType;
    cPar* vlanTagEnabled;
    cPar* pcp;
    cPar* dei;
    cPar* vid;
    int ssap = -1;
    int dsap = -1;
    const char* srcip;
    const char* destip;
    int srcport;
    int destport;

    Ieee8022LlcSocket llcSocket;

    cPar* packetLength;
    bool randomPacketLengthEnabled;

    /** Amount of packets sent for statistic. */
    long packetsSent;

    // signals
    simsignal_t sentPkSignal;
protected:
    virtual void initialize() override;

    virtual void handleMessage(cMessage *msg) override;
    virtual Packet* generatePacket();
    virtual uint16_t computeCrc(const Protocol *networkProtocol, const L3Address& srcAddress, const L3Address& destAddress, const Ptr<const UdpHeader>& udpHeader, const Ptr<const Chunk>& udpData);
public:
    virtual void requestPacket() override;
    virtual int getNumPendingRequests() override;
    virtual bool isEmpty() override;
    virtual void clear() {
    }
    ;
    virtual cMessage *pop() override;
    virtual void addListener(IPassiveQueueListener *listener) {
    }
    ;
    virtual void removeListener(IPassiveQueueListener *listener) {
    }
    ;
};

} // namespace detnetmod

#endif
