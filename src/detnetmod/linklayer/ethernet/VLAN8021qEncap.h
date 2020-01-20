/*
 * VLAN8021qEncap.h
 *
 *  Created on: Oct 25, 2019
 *      Author: vamsi
 */

#ifndef DETNETMOD_LINKLAYER_ETHERNET_VLAN8021QENCAP_H_
#define DETNETMOD_LINKLAYER_ETHERNET_VLAN8021QENCAP_H_

#include <omnetpp.h>

#include "inet/linklayer/common/Ieee802Ctrl.h"
#include "inet/linklayer/ieee8021q/Ieee8021qHeader_m.h"
#include "inet/linklayer/common/MacAddressTag_m.h"
#include "inet/linklayer/ethernet/EtherFrame_m.h"
#include "inet/linklayer/ethernet/EtherEncap.h"
#include "inet/common/packet/Packet.h"
#include "inet/common/packet/Message_m.h"

#include "../common/VLANTag_m.h"
#include "../../ieee8021q/Ieee8021q.h"
#include "inet/linklayer/ieee8021q/Ieee8021qEncap.h"
//
//#include "nesting/linklayer/common/VLANTag_m.h"
//#include "nesting/ieee8021q/Ieee8021q.h"

using namespace omnetpp;
using namespace inet;
namespace detnetmod {

/**
 * See the NED file for a detailed description
 */
class VLAN8021qEncap: public Ieee8021qEncap {

protected:
//        virtual void initialize(int stage) override;
        virtual void handleMessage(cMessage *message) override;
//        virtual Ieee8021qHeader *findVlanTag(const Ptr<EthernetMacHeader>& ethernetMacHeader) override;
//        virtual Ieee8021qHeader *addVlanTag(const Ptr<EthernetMacHeader>& ethernetMacHeader) override;
//        virtual Ieee8021qHeader *removeVlanTag(const Ptr<EthernetMacHeader>& ethernetMacHeader) override;
//        virtual void parseParameters(const char *filterParameterName, const char *mapParameterName, std::vector<int>& vlanIdFilter, std::map<int, int>& vlanIdMap);
        virtual void processPacket(Packet *packet, std::vector<int>& vlanIdFilter, std::map<int, int>& vlanIdMap, cGate *gate) override;
public:
//    virtual ~VLAN8021qEncap();

};

} // namespace detnetmod

#endif /* DETNETMOD_LINKLAYER_ETHERNET_VLAN8021QENCAP_H_ */
