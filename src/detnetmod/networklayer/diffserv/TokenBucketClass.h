/*
 * TokenBucketClass.h
 *
 *  Created on: Nov 5, 2019
 *      Author: vamsi
 */

#ifndef DETNETMOD_NETWORKLAYER_DIFFSERV_TOKENBUCKETCLASS_H_
#define DETNETMOD_NETWORKLAYER_DIFFSERV_TOKENBUCKETCLASS_H_


#include "inet/common/INETDefs.h"
#include "inet/common/packet/Packet.h"


#include "../../linklayer/common/VLANTag_m.h"
#include "../../ieee8021q/Ieee8021q.h"
#include "nesting/ieee8021q/queue/gating/GateController.h"
#include "nesting/ieee8021q/queue/gating/TransmissionGate.h"

using namespace inet;
using namespace nesting;

namespace detnetmod {

class INET_API TokenBucketClass : public cSimpleModule
{
  protected:

    typedef struct flow_table{
        uint32_t vqueue;
        uint32_t flowId;
        int type;
        double bwreq;
        simtime_t last_arrival;
        uint32_t threshold;
    }flow_table_t;

    typedef struct activeflow{
        flow_table_t *flow;
    }activeflow_t;

    //Parameters
    int NumClasses;
    std::vector<double>ClassBandwidthReservation;
    double OutputBandwidth;
    uint32_t Threshold;


    simtime_t LastPacketArrival = 0;
    int numActiveflows=0;
    GateController* gateController;
    nesting::TransmissionGate* tgate;


  public:
//    TokenBucketClass() {}

  protected:
    virtual ~TokenBucketClass();
//    virtual void initialize() override;
//    virtual void handleMessage(cMessage *msg) override;

    virtual bool shouldDrop(cPacket *packet);
    virtual bool Drop(cPacket *packet,Packet* netpacket);
    virtual void sendOut(cMessage *msg,int gateIndex);

    // Activelist functions
    void AddActiveFlow(activeflow_t flow, int Class);
    virtual activeflow_t RemoveActiveFlow(int Class);

    virtual flow_table_t *ClassifyFlow(Packet* netpacket,cPacket* packet,int flowId,uint32_t PacketLength, int Class);

    virtual int numInitStages() const override { return NUM_INIT_STAGES; }
    virtual void initialize();
    virtual void handleMessage(cMessage *msg) override;
//    virtual void refreshDisplay() const override;
//    virtual int meterPacket(cPacket *packet);

  private:
    simsignal_t arrivalSignal;

    double reservedBandwidth_consumption;
    std::vector<flow_table_t>flow_table[102400];
    std::vector<activeflow_t>reservedflows[63];
    std::vector<activeflow_t>besteffortflows;
};

} // namespace detnetmod



#endif /* DETNETMOD_NETWORKLAYER_DIFFSERV_TOKENBUCKETCLASS_H_ */
