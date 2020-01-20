/*
 * SetKind.h
 *
 *  Created on: Nov 5, 2019
 *      Author: vamsi
 */

#ifndef DETNETMOD_APPLICATIONS_SETKINDNETPERF_H_
#define DETNETMOD_APPLICATIONS_SETKINDNETPERF_H_


#include <omnetpp.h>

using namespace omnetpp;

namespace detnetmod {

class SetKindNetPerf: public cSimpleModule {
protected:
    int numSent=0;
    virtual void initialize();
    virtual void handleMessage(cMessage *msg);
};

} /*namespace detnetmod*/


#endif /* DETNETMOD_APPLICATIONS_SETKINDNETPERF_H_ */
