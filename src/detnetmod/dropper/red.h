/*
 * red.h
 *
 *  Created on: Oct 2, 2019
 *      Author: vamsi
 */

#ifndef DETNETMOD_DROPPER_RED_H_
#define DETNETMOD_DROPPER_RED_H_

#include "inet/common/INETDefs.h"
#include "inet/common/queue/AlgorithmicDropperBase.h"
using namespace inet;

namespace detnetmod {

/**
 * Implementation of Random Early Detection (RED).
 */
class INET_API RedDropper : public AlgorithmicDropperBase
{
  protected:
    double wq = 0.0;
    double *minths = nullptr;
    double *maxths = nullptr;
    double *maxps = nullptr;
    double *pkrates = nullptr;
    double *count = nullptr;

    double avg = 0.0;
    simtime_t q_time;

  public:
    RedDropper() {}

  protected:
    virtual ~RedDropper();
    virtual void initialize() override;
    virtual bool shouldDrop(cPacket *packet) override;
    virtual void sendOut(cPacket *packet) override;
};

} // namespace detnetmod

#endif /* DETNETMOD_DROPPER_RED_H_ */
