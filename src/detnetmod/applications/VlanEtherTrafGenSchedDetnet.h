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

#ifndef __NESTING_VETHERTRAFGENSCHED_H
#define __NESTING_VETHERTRAFGENSCHED_H

#include <omnetpp.h>
#include <memory>

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
#include "nesting/linklayer/common/VLANTag_m.h"
//#include "nesting/linklayer/common/Ieee8021QCtrl.h"
#include "nesting/ieee8021q/Ieee8021q.h"

#include "inet/common/packet/recorder/PcapDump.h"

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
using namespace nesting;

namespace detnetmod {

/**
 * See the NED file for a detailed description
 */
class VlanEtherTrafGenSchedDetnet: public cSimpleModule, public IClockListener {
private:

    /** Current schedule. Is never null. */
    std::unique_ptr<HostSchedule<Ieee8021QCtrl>> currentSchedule;

    /**
     * Next schedule to load after the current schedule finishes it's cycle.
     * Can be null.
     */
    std::unique_ptr<HostSchedule<Ieee8021QCtrl>> nextSchedule;

    /** Index for the current entry in the schedule. */
    long int index = 0;

    IClock *clock;

protected:

    // receive statistics
    long TSNpacketsSent = 0;
    long packetsReceived = 0;
    simsignal_t sentPkSignal;
    simsignal_t rcvdPkSignal;
    int ssap = -1;
    int dsap = -1;

    const char* srcip;
    const char* destip;
    int srcport;
    int destport;
    int vid;


    cMessage* jitterMsg = new cMessage("jitterMsg");
    double jitter;

    Ieee8022LlcSocket llcSocket;

    int seqNum = 0;

    virtual void initialize(int stage) override;
    virtual void sendPacket();
    virtual void receivePacket(Packet *msg);
    virtual void handleMessage(cMessage *msg) override;
    virtual void sendDelayed();

    virtual int numInitStages() const override;
    virtual simtime_t scheduleNextTickEvent();
    virtual uint16_t computeCrc(const Protocol *networkProtocol, const L3Address& srcAddress, const L3Address& destAddress, const Ptr<const UdpHeader>& udpHeader, const Ptr<const Chunk>& udpData);
public:
    virtual void tick(IClock *clock) override;

    ~VlanEtherTrafGenSchedDetnet();

    /** Loads a new schedule into the gate controller. */
    virtual void loadScheduleOrDefault(cXMLElement* xml);
};

} // namespace nesting

#endif /* __NESTING_VETHERTRAFGENSCHED_H */
