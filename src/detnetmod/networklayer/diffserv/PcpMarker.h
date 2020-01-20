/*
 * PcpMarker.h
 *
 *  Created on: Nov 7, 2019
 *      Author: vamsi
 */

#ifndef DETNETMOD_NETWORKLAYER_DIFFSERV_PCPMARKER_H_
#define DETNETMOD_NETWORKLAYER_DIFFSERV_PCPMARKER_H_


#include "inet/common/INETDefs.h"
#include "inet/common/packet/Packet.h"


#include "../../linklayer/common/VLANTag_m.h"
#include "../../ieee8021q/Ieee8021q.h"

#include "inet/linklayer/ieee8022/Ieee8022LlcSocket.h"
#include "inet/common/IProtocolRegistrationListener.h"

using namespace inet;

namespace detnetmod {

class PcpMarker: public cSimpleModule
{

  protected:
    int ssap = -1;
        int dsap = -1;

        Ieee8022LlcSocket llcSocket;

    int NumClasses;
//    virtual ~PcpMarker();
    virtual int numInitStages() const override { return NUM_INIT_STAGES; }
    virtual void initialize();
    virtual void handleMessage(cMessage *msg) override;


  private:
    std::vector<int>PCPValues;

};

} // namespace detnetmod





#endif /* DETNETMOD_NETWORKLAYER_DIFFSERV_PCPMARKER_H_ */
