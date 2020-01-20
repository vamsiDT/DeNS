/*
 * TokenBucketClass.cc
 *
 *  Created on: Nov 5, 2019
 *      Author: vamsi
 */

#include "TokenBucketClass.h"
#include "inet/common/INETUtils.h"
#include "cmath"

//#define INGRESS_GATE

#include <iostream>
#include <fstream>
#include <string>

#include "inet/linklayer/common/Ieee802Ctrl.h"
#include "inet/linklayer/ieee8021q/Ieee8021qHeader_m.h"
#include "inet/linklayer/common/MacAddressTag_m.h"
#include "inet/linklayer/ethernet/EtherFrame_m.h"
#include "inet/linklayer/ethernet/EtherEncap.h"
#include "inet/common/packet/Packet.h"
#include "inet/common/packet/Message_m.h"
#include "inet/common/packet/Packet.h"
#include "inet/linklayer/common/MacAddressTag_m.h"
#include "inet/common/packet/Message.h"
#include "inet/linklayer/common/Ieee802SapTag_m.h"

#include "../../linklayer/common/VLANTag_m.h"
#include "../../ieee8021q/Ieee8021q.h"

#include "inet/common/ModuleAccess.h"


namespace detnetmod {

Define_Module(TokenBucketClass);

TokenBucketClass::~TokenBucketClass()
{
    reservedBandwidth_consumption=0;
    OutputBandwidth=0;
    Threshold=0;
//    TableSize=0;
    LastPacketArrival=0;
}

void TokenBucketClass::initialize()
{
    Threshold = par("vqThreshold");
    OutputBandwidth = par("OutputLinkBandwidth");
    NumClasses = par("NumClasses");
    reservedBandwidth_consumption = 0;
    arrivalSignal = registerSignal("arrival");
    int i=0;
    while (i<NumClasses){
        std::string parameter = "ClassPerflowReservation";
        parameter.append(std::to_string(i));
        ClassBandwidthReservation.push_back(par(parameter.c_str()));
        i++;
    }
    gateController = getModuleFromPar<GateController>(par("gateController"), this);
    tgate=getModuleFromPar<nesting::TransmissionGate>(par("tgate"), this);
//    WATCH(besteffortflows.size());


    if(!Threshold)
        throw cRuntimeError("vqThrehold cannot be zero");
}

void TokenBucketClass::handleMessage(cMessage *msg)
{

    if (dynamic_cast<inet::Request*>(msg)) {
        delete msg;
        return;
    }

    cPacket *packet = check_and_cast<cPacket *>(msg);
    const int gateIndex = packet->getArrivalGate()->getIndex();
//    EV_INFO << "Packet Arrived on gate = ", gateIndex;
    Packet *netpacket = check_and_cast<Packet *>(msg);

    if (Drop(packet,netpacket)){
        PacketDropDetails details;
        details.setReason(CONGESTION);
//        emit(packetDroppedSignal, packet, &details);
        delete packet;
    }
    else
        sendOut(msg,gateIndex);
}


void
TokenBucketClass::AddActiveFlow(activeflow_t tail,int Class){
    if(Class < NumClasses){
        reservedflows[Class].push_back(tail);
    }
    else
        besteffortflows.push_back(tail);
}

TokenBucketClass::activeflow_t
TokenBucketClass::RemoveActiveFlow(int Class){
    activeflow_t head;
    if(Class < NumClasses){
        head = reservedflows[Class].front();
        reservedflows[Class].erase(reservedflows[Class].begin());
    }
    else{
        head = besteffortflows.front();
        besteffortflows.erase(besteffortflows.begin());
    }
    return head;
}

TokenBucketClass::flow_table_t*
TokenBucketClass::ClassifyFlow(Packet* netpacket,cPacket* packet,int flowId,uint32_t PacketLength, int Class){
    flow_table_t *flow;
    uint32_t mod = flowId%102400;
    uint32_t a = flow_table[mod].size();
    while(a){
        flow=&flow_table[mod][a-1];
        if(flow->flowId == flowId)return flow ;
        a--;
    }
    flow= new flow_table_t[1];
    flow->flowId=flowId;
    flow->vqueue=0;
    flow->type = Class;
    if(Class<NumClasses)
        flow->bwreq=ClassBandwidthReservation[Class];
    else
        flow->bwreq=0;

    flow->threshold=Threshold;
    flow_table[mod].push_back(*flow);
    return flow;
}

//Useless function for Mfd module. Instead Drop is used.
bool TokenBucketClass::shouldDrop(cPacket *packet){
return true;
}

bool TokenBucketClass::Drop(cPacket *packet,Packet *netpacket)
{

//    EV_DETAIL<< "num active flows = "<< besteffortflows.size()<<endl;
    const int i = packet->getArrivalGate()->getIndex();

    int Class = i;

    ASSERT(i >= 0 && i <= NumClasses);
//EV_DETAIL<< "ID=" << netpacket->getId();
    simtime_t now = simTime();

    std::string s = netpacket->getName();
    std::string delimiter = "-";
    int pktId;
    size_t pos = 0;
    std::string token;
    pos=s.find(delimiter);
    if(pos!= std::string::npos){
        token = s.substr(0,pos);
    }
    EV_DETAIL << "token="<<token;
    if (!token.empty()){
        pktId = (std::stoi(token))*123456;
    }
    else
        pktId = 1;
EV_DETAIL << "##pktId="<< pktId << "##";

    flow_table_t *flow;
    uint32_t pktsize = packet->getBitLength();

  /*
  *******************************MFD ALGORITHM************************************
  */


    /*
    Flow Classification
    */
    flow = ClassifyFlow(netpacket,packet,pktId,pktsize,Class);
    numActiveflows=0;
      /*
      Loop which allocates reserved bandwidth for PRIORITY flows
      */
      for(int i=0;i<NumClasses;i++){
          numActiveflows+=reservedflows[i].size();
          if(reservedflows[i].size() > 0){
              for(int j=0;j<reservedflows[i].size();j++){
                  activeflow_t d_flow = RemoveActiveFlow(i);
                  flow_table_t *flow = d_flow.flow;
                  double delta1 = (now  - flow->last_arrival).inUnit(SIMTIME_NS);
                  flow->last_arrival=now;
                  double req  = (flow->bwreq*delta1);

                  if(flow->vqueue > req){
                      flow->vqueue-=req;
                      reservedBandwidth_consumption+=req;
                      AddActiveFlow(d_flow,i);
                  }
                  else{
                      reservedBandwidth_consumption+=flow->vqueue;
                      flow->vqueue=0;
                  }
              }
          }
      }
      /*
      Credit is the total available bandwidth to be used (in bits)
      */
      double delta = (now  - LastPacketArrival).inUnit(SIMTIME_NS);
      double credit = (OutputBandwidth*delta);

      /*
      Loop which allocates remaining unused bandwidth for NON-PRIORITY flows
      */
      numActiveflows+=besteffortflows.size();

      emit(arrivalSignal, numActiveflows);
//      activeFlows.recordWithTimestamp(SimTime(), numActiveflows);
      if(credit > reservedBandwidth_consumption && besteffortflows.size() > 0){
          LastPacketArrival=now;
        flow_table_t *flow;
        activeflow_t o_flow;
        float rem_credit=credit-reservedBandwidth_consumption;
        float served;
        uint32_t old_nbl=besteffortflows.size()+1;
        while(old_nbl>besteffortflows.size() && besteffortflows.size()>0){
            old_nbl=besteffortflows.size();
            served = rem_credit/old_nbl;
            rem_credit=0;
            for (uint32_t i=0;i<old_nbl;i++){
                o_flow = RemoveActiveFlow(NumClasses);
                flow=o_flow.flow;
                if(flow->vqueue > served){
                    flow->vqueue -= served;
                    AddActiveFlow(o_flow,NumClasses);
                }
                else{
                    rem_credit+=served-flow->vqueue;
                    flow->vqueue=0;
                }
            }
        }
        reservedBandwidth_consumption=0;
      }

  // Packet Accepted or Dropped
#ifdef INGRESS_GATE
    if(flow->vqueue <=flow->threshold && ((Class==0 && tgate->isGateOpen())|| Class!=0)){
#endif
#ifndef INGRESS_GATE
    if(flow->vqueue <=flow->threshold){
#endif
        if(flow->vqueue == 0){
            activeflow_t newflow;
            newflow.flow=flow;
            AddActiveFlow(newflow,flow->type);
        }
        flow->vqueue+=pktsize;
    }
    else{
        EV_DETAIL<< "Dropping class:"<< Class ;
        return true;
    }

    return false;

}

void TokenBucketClass::sendOut(cMessage *msg,int gateIndex)
{
    cGate* outputGate = gate("out", gateIndex);
    send(msg, outputGate);

}

} // namespace detnetmod


