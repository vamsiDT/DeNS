/*
 * PifoDetnet.h
 *
 *  Created on: Jan 3, 2020
 *      Author: vamsi
 */

#ifndef DETNETMOD_QUEUING_PIFODETNET_H_
#define DETNETMOD_QUEUING_PIFODETNET_H_


#include "inet/common/INETDefs.h"
#include "inet/common/queue/PassiveQueueBase.h"

#include "inet/common/ModuleAccess.h"
#include "inet/common/queue/IQueueAccess.h"
#include "nesting/ieee8021q/queue/framePreemption/IPreemptableQueue.h"
#include "nesting/ieee8021q/Ieee8021q.h"
#include "nesting/ieee8021q/queue/transmissionSelectionAlgorithms/TSAlgorithm.h"

using namespace inet;
using namespace nesting;
namespace detnetmod {

/**
 * Drop-front queue. See NED for more info.
 */
class PifoDetnet : public nesting::LengthAwareQueue, public IQueueAccess
{
  protected:

    int bytelength;
    cObject *enqueuedDetnet;

  protected:
    virtual void initialize() override;

    /**
     * Redefined from PassiveQueueBase.
     */
    virtual void enqueue(cPacket* packet) override;
    virtual cPacket* dequeue() override;

    virtual int getLength() const override { return queue.getLength(); }
    virtual int getByteLength() const override { return bytelength; }

  public:
      virtual ~PifoDetnet();

};

} // namespace detnetmod



#endif /* DETNETMOD_QUEUING_PIFODETNET_H_ */
