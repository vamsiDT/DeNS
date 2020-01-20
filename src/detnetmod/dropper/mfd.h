/*
 * mfd.h
 *
 *  Created on: Oct 5, 2019
 *      Author: vamsi
 */

#ifndef DETNETMOD_DROPPER_MFD_H_
#define DETNETMOD_DROPPER_MFD_H_

#include "inet/common/INETDefs.h"
#include "inet/common/queue/AlgorithmicDropperBase.h"
#include "inet/common/packet/Packet.h"
//#include "nesting/linklayer/common/VLANTag_m.h"

#include "../linklayer/common/VLANTag_m.h"
#include "../ieee8021q/Ieee8021q.h"

#include "nesting/ieee8021q/relay/FilteringDatabase.h"
#include "inet/linklayer/ethernet/switch/IMacAddressTable.h"
#include "inet/networklayer/contract/IInterfaceTable.h"

#include "nesting/ieee8021q/queue/gating/GateController.h"
#include "nesting/ieee8021q/queue/gating/TransmissionGate.h"
using namespace inet;
using namespace nesting;

namespace detnetmod {

/**
 * Implementation of Random Early Detection (RED).
 */
class INET_API Mfd : public AlgorithmicDropperBase
{
  protected:

    IInterfaceTable *ifTable = nullptr;
    IMacAddressTable *macTable = nullptr;
    FilteringDatabase* fdb;
    GateController* gateController;
    nesting::TransmissionGate* tgate;
    bool lbEnabled;
    int backupPort;

    typedef struct flow_table{
        uint32_t vqueue;
        uint32_t flowId;
        std::string type;
        double bwreq;
        simtime_t last_arrival;
        uint32_t threshold;
    }flow_table_t;

    typedef struct detnetactiveflow{
        flow_table_t *flow;
    }detnetactiveflow_t;

    typedef struct otheractiveflow{
        flow_table_t *flow;
    }otheractiveflow_t;

    //Parameters
    double DetnetBandwidth;
    double OutputBandwidth;
    uint32_t Threshold;


    simtime_t LastPacketArrival = 0;


  public:
    Mfd() {}

  protected:
    virtual ~Mfd();
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
    virtual bool shouldDrop(cPacket *packet) override;
    virtual bool Drop(cPacket *packet,Packet* netpacket);
    virtual void sendOut(cPacket *packet) override;

    // Detnet Activelist functions
    void AddDetnetActiveFlow(detnetactiveflow_t flow);
    virtual detnetactiveflow_t RemoveDetnetActiveFlow(void);

    // Other Activelist functions
    virtual void AddOtherActiveFlow(otheractiveflow_t flow);
    virtual otheractiveflow_t RemoveOtherActiveFlow(void);

    virtual flow_table_t *ClassifyFlow(Packet* netpacket,cPacket* packet,int flowId,uint32_t PacketLength,int pcp);

  private:
    double detnet_consumption;
    std::vector<flow_table_t>flow_table[1024];
    std::vector<detnetactiveflow_t>detnetactiveflows;
    std::vector<otheractiveflow_t>otheractiveflows;
};

} // namespace detnetmod



#endif /* DETNETMOD_DROPPER_MFD_H_ */
