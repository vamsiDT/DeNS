/*
 * QueueDetnet.h
 *
 *  Created on: Oct 3, 2019
 *      Author: vamsi
 */

#ifndef DETNETMOD_QUEUING_QUEUEDETNET_H_
#define DETNETMOD_QUEUING_QUEUEDETNET_H_

#include <omnetpp.h>
#include <list>

#include "inet/common/ModuleAccess.h"
#include "inet/common/queue/IQueueAccess.h"


#include "nesting/ieee8021q/Ieee8021q.h"
#include "nesting/ieee8021q/queue/transmissionSelectionAlgorithms/TSAlgorithm.h"
#include "nesting/ieee8021q/queue/framePreemption/IPreemptableQueue.h"

using namespace omnetpp;
using namespace inet;
using namespace nesting;
namespace detnetmod {
/**
 * See the NED file for a detailed description.
 */
//class nesting::TSAlgorithm;
class QueueDetnet: public nesting::LengthAwareQueue, public IQueueAccess{
protected:
    int bytelength;

protected:
    virtual void initialize() override;
    virtual void enqueue(cPacket* packet) override;
    virtual cPacket* dequeue() override;
    virtual int getLength() const override { return queue.getLength(); }
    virtual int getByteLength() const override { return bytelength; }

public:
    virtual ~QueueDetnet();

};

}

#endif /* DETNETMOD_QUEUING_QUEUEDETNET_H_ */
