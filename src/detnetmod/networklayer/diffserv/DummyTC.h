/*
 * DummyTC.h
 *
 *  Created on: Dec 10, 2019
 *      Author: vamsi
 */

#ifndef DETNETMOD_NETWORKLAYER_DIFFSERV_DUMMYTC_H_
#define DETNETMOD_NETWORKLAYER_DIFFSERV_DUMMYTC_H_

#include "inet/common/INETDefs.h"
#include "inet/common/packet/Packet.h"


#include "inet/linklayer/ieee8022/Ieee8022LlcSocket.h"
#include "inet/common/IProtocolRegistrationListener.h"

using namespace inet;

namespace detnetmod {

class DummyTC: public cSimpleModule
{

  protected:
    int ssap = -1;
        int dsap = -1;

        Ieee8022LlcSocket llcSocket;

    virtual int numInitStages() const override { return NUM_INIT_STAGES; }
    virtual void initialize();
    virtual void handleMessage(cMessage *msg) override;
};

} // namespace detnetmod





#endif /* DETNETMOD_NETWORKLAYER_DIFFSERV_DUMMYTC_H_ */
