/*
 * macrelay.h
 *
 *  Created on: Oct 20, 2019
 *      Author: vamsi
 */

#ifndef DETNETMOD_IEEE8021Q_RELAY_MACRELAY_H_
#define DETNETMOD_IEEE8021Q_RELAY_MACRELAY_H_



#include <unordered_map>
#include <omnetpp.h>

//mac relay related headers
#include "inet/common/LayeredProtocolBase.h"
#include "inet/common/lifecycle/ModuleOperations.h"
#include "inet/common/packet/Packet.h"
#include "inet/linklayer/ethernet/EtherFrame_m.h"
#include "inet/linklayer/ethernet/switch/IMacAddressTable.h"
#include "inet/networklayer/contract/IInterfaceTable.h"

#include "inet/common/IProtocolRegistrationListener.h"
#include "inet/common/LayeredProtocolBase.h"
#include "inet/common/lifecycle/ModuleOperations.h"
#include "inet/common/ModuleAccess.h"
#include "inet/common/Protocol.h"
#include "inet/common/ProtocolTag_m.h"
#include "inet/common/StringFormat.h"
#include "inet/linklayer/common/InterfaceTag_m.h"
#include "inet/linklayer/ethernet/EtherFrame_m.h"
#include "inet/linklayer/ethernet/EtherMacBase.h"
#include "inet/linklayer/ethernet/Ethernet.h"
#include "inet/linklayer/ethernet/switch/MacRelayUnit.h"


#include "inet/common/ModuleAccess.h"
#include "inet/common/packet/Packet.h"
#include "inet/linklayer/common/MacAddressTag_m.h"
#include "nesting/ieee8021q/relay/FilteringDatabase.h"
#include "inet/linklayer/common/InterfaceTag_m.h"
#include "inet/linklayer/ethernet/EtherFrame_m.h"
#include "inet/linklayer/ethernet/EtherMacBase.h"
#include "inet/linklayer/ethernet/Ethernet.h"
#include "inet/common/Protocol.h"
#include "inet/common/ProtocolTag_m.h"
#include "inet/linklayer/ethernet/switch/MacRelayUnit.h"

#include "../../linklayer/common/VLANTag_m.h"
#include "../Ieee8021q.h"

//#include "nesting/linklayer/common/VLANTag_m.h"
//#include "nesting/ieee8021q/Ieee8021q.h"

using namespace omnetpp;
using namespace inet;
using namespace nesting;

namespace detnetmod {

/**
 * See the NED file for a detailed description
 */
class macrelay:public LayeredProtocolBase{
private:
    IInterfaceTable *ifTable = nullptr;
    IMacAddressTable *macTable = nullptr;
    FilteringDatabase* fdb;
    //TODO: Create parameter for filtering database aging
    simtime_t fdbAgingThreshold = 1000;
    int numberOfPorts;
    long numProcessedFrames = 0;
    long numDiscardedFrames = 0;
protected:
    virtual void initialize(int stage) override;
    virtual int numInitStages() const override { return NUM_INIT_STAGES; }
    virtual void updateDisplayString() const;
    /**
     * Updates address table with source address, determines output port
     * and sends out (or broadcasts) frame on ports. Includes calls to
     * updateTableWithAddress() and getPortForAddress().
     *
     * The message pointer should not be referenced any more after this call.
     */
    virtual void handleAndDispatchFrame(Packet *packet);

    void handleLowerPacket(Packet *packet) override;

    /**
     * Utility function: sends the frame on all ports except inputport.
     * The message pointer should not be referenced any more after this call.
     */
    virtual void broadcast(Packet *frame, int inputport);

    /**
     * Writes statistics.
     */
    virtual void finish() override;

    // for lifecycle:
    virtual void handleStartOperation(LifecycleOperation *operation) override { start(); }
    virtual void handleStopOperation(LifecycleOperation *operation) override { stop(); }
    virtual void handleCrashOperation(LifecycleOperation *operation) override { stop(); }
    virtual bool isUpperMessage(cMessage *message) override { return message->arrivedOn("upperLayerIn"); }
    virtual bool isLowerMessage(cMessage *message) override { return message->arrivedOn("ifIn"); }

    virtual bool isInitializeStage(int stage) override { return stage == INITSTAGE_LINK_LAYER; }
    virtual bool isModuleStartStage(int stage) override { return stage == ModuleStartOperation::STAGE_LINK_LAYER; }
    virtual bool isModuleStopStage(int stage) override { return stage == ModuleStopOperation::STAGE_LINK_LAYER; }

    virtual void start();
    virtual void stop();
    virtual void learn(MacAddress srcAddr, int arrivalInterfaceId);

//    virtual void initialize();
//    virtual void handleMessage(cMessage* msg);
//    virtual void processBroadcast(Packet* packet);
    virtual void processMulticast(Packet* packet,MacAddress destAddr,int arrivalInterfaceId);
//    virtual void processUnicast(Packet* packet);

public:
    //TODO: Fix filtering database aging parameter!
//  ForwardingRelayUnit() : fdb(1000) {};
};

} // namespace detnetmod



#endif /* DETNETMOD_IEEE8021Q_RELAY_MACRELAY_H_ */
