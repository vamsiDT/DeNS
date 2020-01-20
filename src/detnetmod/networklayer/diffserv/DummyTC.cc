/*
 * DummyTC.cc
 *
 *  Created on: Dec 10, 2019
 *      Author: vamsi
 */




/*
 * PcpMarker.cc
 *
 *  Created on: Nov 7, 2019
 *      Author: vamsi
 */


#include "DummyTC.h"
#include "inet/common/packet/Message.h"
#include "inet/common/packet/Packet.h"
#include "inet/common/packet/Message_m.h"
#include "inet/common/IProtocolRegistrationListener.h"
#include "inet/common/ModuleAccess.h"
#include "inet/common/ProtocolGroup.h"
#include "inet/common/ProtocolTag_m.h"

namespace detnetmod {

Define_Module(DummyTC);

void DummyTC::initialize() {

    llcSocket.setOutputGate(gate("out"));

    llcSocket.open(-1, ssap);
}
void DummyTC::handleMessage(cMessage *msg)
{

    if (dynamic_cast<inet::Request*>(msg)) {
        send(msg,gate("out"));
        return;
    }
    Packet *packet = check_and_cast<Packet *>(msg);
    send(msg,gate("out"));
}
}
