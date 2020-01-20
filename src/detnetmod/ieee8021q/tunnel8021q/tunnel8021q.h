/*
 * tunnel8021q.h
 *
 *  Created on: Oct 21, 2019
 *      Author: vamsi
 */

#ifndef DETNETMOD_IEEE8021Q_TUNNEL8021Q_TUNNEL8021Q_H_
#define DETNETMOD_IEEE8021Q_TUNNEL8021Q_TUNNEL8021Q_H_

#include "inet/linklayer/vlan/VlanTunnel.h"

#include "../../linklayer/common/VLANTag_m.h"
#include "../Ieee8021q.h"

//#include "nesting/ieee8021q/Ieee8021q.h"
//#include "nesting/linklayer/common/VLANTag_m.h"

#include "inet/linklayer/ethernet/EthernetSocket.h"
#include "inet/networklayer/common/InterfaceEntry.h"

using namespace inet;
//using namespace nesting;

namespace detnetmod {

class tunnel8021q : public inet::VlanTunnel
{
protected:
    //parameters
    int vid;
    bool de;
    int pcp;

protected:
    virtual void initialize(int stage) override;
    virtual void handleMessage(cMessage *message) override;

    virtual void socketDataArrived(EthernetSocket *socket, Packet *packet) override;
    virtual void socketErrorArrived(EthernetSocket *socket, Indication *indication) override;
    virtual void socketClosed(EthernetSocket *socket) override {}
};

}

#endif /* DETNETMOD_IEEE8021Q_TUNNEL8021Q_TUNNEL8021Q_H_ */
