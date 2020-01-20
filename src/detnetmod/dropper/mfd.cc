/*
 * mfd.cc
 *
 *  Created on: Oct 5, 2019
 *      Author: vamsi
 */
#include "mfd.h"
#include "inet/common/INETUtils.h"
#include "cmath"

//#define INGRESS_GATE
#define CQF
//#define PIFO

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
//#include "nesting/ieee8021q/Ieee8021q.h"
//#include "nesting/linklayer/common/VLANTag_m.h"

#include "../linklayer/common/VLANTag_m.h"
#include "../ieee8021q/Ieee8021q.h"

#include "inet/common/ModuleAccess.h"


namespace detnetmod {

Define_Module(Mfd);

Mfd::~Mfd()
{
    DetnetBandwidth=0;
    OutputBandwidth=0;
    Threshold=0;
//    TableSize=0;
    LastPacketArrival=0;
}

void Mfd::initialize()
{
    AlgorithmicDropperBase::initialize();

    DetnetBandwidth = par("DetnetBandwidthReservation");
    Threshold = par("vqThreshold");
    OutputBandwidth = par("OutputLinkBandwidth");

    if(!Threshold)
        throw cRuntimeError("vqThrehold cannot be zero");

    macTable = getModuleFromPar<IMacAddressTable>(par("macTableModule"), this);
    ifTable = getModuleFromPar<IInterfaceTable>(par("interfaceTableModule"), this);

    gateController = getModuleFromPar<GateController>(par("gateController"), this);
    tgate=getModuleFromPar<nesting::TransmissionGate>(par("tgate"), this);

    fdb = getModuleFromPar<FilteringDatabase>(par("filteringDatabaseModule"),this);
    lbEnabled = par("lbEnabled");
    backupPort = par("backupPort");

}

void Mfd::handleMessage(cMessage *msg)
{

    cPacket *packet = check_and_cast<cPacket *>(msg);
    Packet *netpacket = check_and_cast<Packet *>(msg);

    if (Drop(packet,netpacket))
        dropPacket(packet);
    else
        sendOut(packet);
}


void
Mfd::AddDetnetActiveFlow(detnetactiveflow_t tail){
    detnetactiveflows.push_back(tail);
}

Mfd::detnetactiveflow_t
Mfd::RemoveDetnetActiveFlow(void){
    detnetactiveflow_t head = detnetactiveflows.front();
    detnetactiveflows.erase(detnetactiveflows.begin());
    return head;
}

void
Mfd::AddOtherActiveFlow(otheractiveflow_t tail){
    otheractiveflows.push_back(tail);
}

Mfd::otheractiveflow_t
Mfd::RemoveOtherActiveFlow(void){
    otheractiveflow_t head = otheractiveflows.front();
    otheractiveflows.erase(otheractiveflows.begin());
    return head;
}

Mfd::flow_table_t*
Mfd::ClassifyFlow(Packet* netpacket,cPacket* packet,int flowId,uint32_t PacketLength, int pcp){
    flow_table_t *flow;
    uint32_t mod = flowId%1024;
    uint32_t a = flow_table[mod].size();
    while(a){
        flow=&flow_table[mod][a-1];
        if(flow->flowId == flowId)return flow ;
        a--;
    }
    flow= new flow_table_t[1];
    flow->flowId=flowId;
    flow->vqueue=0;
    flow->type = "D";

    /////////////////////////////////////??? CHECK CHECK CHECK??//////////////////////////////////////////////////
//    inet::Packet *Packet = check_and_cast<inet::Packet *>(packet);
    if(!netpacket){
        throw cRuntimeError("cast error");
    }
//    auto vlanTagOut = netpacket->findTag<VLANTagReq>();

//    if(!vlanTagOut){
//        throw cRuntimeError("tag error");
//    }
    int isDetnet = pcp; //vlanTagOut->getPcp();
    if (isDetnet >=7){
        flow->type = "D";
        flow->bwreq=DetnetBandwidth;
    }
    else{
        flow->type = "O";
        flow->bwreq=0;
    }
    flow->threshold=Threshold;
    flow_table[mod].push_back(*flow);
    return flow;
}

//Useless function for Mfd module. Instead Drop is used.
bool Mfd::shouldDrop(cPacket *packet){
return true;
}

bool Mfd::Drop(cPacket *packet,Packet *netpacket)
{
    const int i = packet->getArrivalGate()->getIndex();

    int queueindex = i;
    int queueLength;
    queueLength = outQueues[i]->getLength();

//    EV_DETAIL << "queueindex="<< i << "\tlength=" << outQueues[i]->getLength();
    ASSERT(i >= 0 && i < numGates);
//EV_DETAIL<< "ID=" << netpacket->getId();
    simtime_t now = simTime();
    int pcp, vid;
    pcp = i;
    vid = 1;

//    int pcp,vid;
//    /////////////////////////////////////??? CHECK CHECK CHECK??//////////////////////////////////////////////////
//    netpacket->trimFront();
//    const auto& ethernetMacHeader = netpacket->removeAtFront<inet::EthernetMacHeader>();
//    auto DestAddress = ethernetMacHeader->getDest();
//
//            auto vlanHeaderS = ethernetMacHeader->getSTag();
//            auto vlanHeaderC = ethernetMacHeader->getCTag();
//            if(vlanHeaderS){
//                    pcp = vlanHeaderS->getPcp();
//                    vid = vlanHeaderS->getVid();
////                netpacket->trim();
//                    }
//                    else if (vlanHeaderC){
//                        pcp = vlanHeaderC->getPcp();
//                        vid = vlanHeaderS->getVid();
//
////                    netpacket->trim();
//                    }
//                    else{
//                        pcp=i;
//                        vid=1;
//
//                    }
////            netpacket->trim();
//            netpacket->insertAtFront(ethernetMacHeader);
//                                    auto oldFcs = netpacket->removeAtBack<inet::EthernetFcs>();
//                                    inet::EtherEncap::addFcs(netpacket, oldFcs->getFcsMode());


//                                    netpacket->trim();
//    auto vlanTagOut = netpacket->findTag<VLANTagReq>();
//    uint32_t pktId = packet->getKind();//vid*12345;
    std::string s = netpacket->getName();
    std::string delimiter = "-";
    uint32_t pktId;
    size_t pos = 0;
    std::string token;
    pos=s.find(delimiter);
    if(pos!= std::string::npos){
        token = s.substr(0,pos);
    }
//    EV_DETAIL << "token="<<token;
    if (!token.empty()){
        pktId = (std::stoi(token))*12345;
    }
    else
        pktId = 1;
//EV_DETAIL << "pktId="<< pktId << "flowid=" << pktId%1024;


//    EV_DETAIL << "PCP=" << pcp <<"\tgateindex=" << i << "vid=" << vid;
    flow_table_t *flow;
    uint32_t pktsize = packet->getBitLength();

  /*
  *******************************MFD ALGORITHM************************************
  */


    /*
    Flow Classification
    */
    flow = ClassifyFlow(netpacket,packet,pktId,pktsize,pcp);
//    std::ofstream outfilerx;
//        outfilerx.open("/home/vamsi/test.dat", std::ios_base::app);
//        outfilerx << pktId << std::endl;



      /*
      Loop which allocates reserved bandwidth for PRIORITY flows
      */
//#ifdef INGRESS_GATE
//      if(detnetactiveflows.size() > 0 && tgate->isGateOpen()){
//#endif
//#ifndef INGRESS_GATE
          if(detnetactiveflows.size()>0){
//#endif
        for(uint32_t i=0;i<detnetactiveflows.size();i++){
          detnetactiveflow_t d_flow = RemoveDetnetActiveFlow();
              flow_table_t *flow = d_flow.flow;
              double delta1 = (now  - flow->last_arrival).inUnit(SIMTIME_NS);
              flow->last_arrival=now;
              double req  = (flow->bwreq*delta1);

              if(flow->vqueue > req){
            flow->vqueue-=req;
            detnet_consumption+=req;
            AddDetnetActiveFlow(d_flow);
          }
              else{
            detnet_consumption+=flow->vqueue;
            flow->vqueue=0;
          }
        }
      }
      /*
      Credit is the total available bandwidth to be used (in bits)
      */
      double delta = (now  - LastPacketArrival).inUnit(SIMTIME_NS);
      double credit = (OutputBandwidth*delta);
      EV_DETAIL<< credit/8 << "\t"<< detnet_consumption/8 <<"\t"<< (credit-detnet_consumption)/8 ;
      /*
      Loop which allocates remaining unused bandwidth for NON-PRIORITY flows
      */

      if(credit > detnet_consumption && otheractiveflows.size() > 0){
          LastPacketArrival=now;
        flow_table_t *flow;
        otheractiveflow_t o_flow;
        float rem_credit=credit-detnet_consumption;
        float served;
        uint32_t old_nbl=otheractiveflows.size()+1;
        while(old_nbl>otheractiveflows.size() && otheractiveflows.size()>0){
        old_nbl=otheractiveflows.size();
        served = rem_credit/old_nbl;
        rem_credit=0;
        for (uint32_t i=0;i<old_nbl;i++){
          o_flow = RemoveOtherActiveFlow();
          flow=o_flow.flow;
          if(flow->vqueue > served){
            flow->vqueue -= served;
            AddOtherActiveFlow(o_flow);
          }
          else{
            rem_credit+=served-flow->vqueue;
            flow->vqueue=0;
          }
        }
      }
        detnet_consumption=0;
    }



//       load balancing [or packet replication]
//      if(lbEnabled && flow->type=="D"){
//          simtime_t remtime = gateController->CurrentScheduleIndexRemainingTime(queueindex);
//          double ns = remtime.inUnit(SIMTIME_NS);
//          if(flow->vqueue+2*pktsize >=flow->threshold || queueLength > 5 || ns < 2*(pktsize*1.0)/(OutputBandwidth)){
//              fdb->insert(DestAddress, simTime(),backupPort);
//          }
//      }


  // Packet Accepted or Dropped
//      && queueLength <= gateController->calculateMaxBit(7)
#ifdef INGRESS_GATE
    if( ( (flow->type == "D" && tgate->isGateOpen() ) || flow->type == "O" ) && flow->vqueue <= flow->threshold){
#endif

#ifndef INGRESS_GATE
    if(flow->vqueue <=flow->threshold){
#endif
        if(flow->vqueue == 0){
            if(flow->type=="D"){
                detnetactiveflow_t newflow;
                newflow.flow=flow;
                AddDetnetActiveFlow(newflow);
            }
            else{
                otheractiveflow_t newflow;
                newflow.flow=flow;
                AddOtherActiveFlow(newflow);
            }
        }
        flow->vqueue+=pktsize;
    }
    else{
        return true;
    }


    return false;
}

void Mfd::sendOut(cPacket *packet)
{

    int index = packet->getArrivalGate()->getIndex();
#ifdef PIFO
//
    simtime_t cycle = SimTime(200, SIMTIME_US);
    simtime_t d = simTime();
    simtime_t x = SimTime(d.inUnit(SIMTIME_US)%cycle.inUnit(SIMTIME_US),SIMTIME_US);
//    EV_DETAIL<< "$$$$$$$$$  $$$$$$$$$$$$$$$ "<< x.inUnit(SIMTIME_US) << endl;
if(index==7){
        if (( !tgate->isGateOpen() && (x<SimTime(180,SIMTIME_US)) )){
            index-=1;
        }
        else if (( tgate->isGateOpen() && (x>SimTime(40,SIMTIME_US)) )){
                    index-=1;
                }
//        else if (( tgate->isGateOpen() && (x<=SimTime(40,SIMTIME_US)) ))
//            index=7;
//        else if (( !tgate->isGateOpen() && (x>=SimTime(180,SIMTIME_US)) ))
//                    index=7;
//        else
//            EV_DETAIL<< "DAMN IT DAMN ITDAMN ITDAMN ITDAMN ITDAMN ITDAMN ITDAMN ITDAMN \nITDAMN ITDAMN ITDAMN ITDAMN ITDAMN ITDAMN ITDAMN ITDAMN ITDAMN \nITDAMN ITDAMN ITDAMN ITDAMN ITDAMN ITDAMN ITDAMN ITDAMN \nITDAMN ITDAMN ITDAMN ITDAMN ITDAMN ITDAMN ITDAMN ITDAMN ITDAMN ITDAMN \nITDAMN ITDAMN ITDAMN ITDAMN ITDAMN ITDAMN ITDAMN ITDAMN ITDAMN ITDAMN \nITDAMN ITDAMN ITDAMN ITDAMN ITDAMN ITDAMN ITDAMN \nITDAMN ITDAMN ITDAMN ITDAMN ITDAMN ITDAMN ITDAMN \nITDAMN ITDAMN ITDAMN ITDAMN ITDAMN ITDAMN ITDAMN \nITDAMN ITDAMN ITDAMN ITDAMN ITDAMN \nITDAMN IT"<< endl;
}
#endif
/////////// PIFO AND CQF HAVE SAME CODE HERE. HOWEVER, I STILL WANT TO DIVIDE IT FOR CODE UNDERSTANDING PURPOSES.
/////////// THE IDEA IS SAME. TO ENQUEUE IN QUEUE-7 IN ODD CYCLE AND QUEUE-6 IN EVEN CYCLE.
#ifdef CQF
//
    simtime_t cycle = SimTime(200, SIMTIME_US);
    simtime_t d = simTime();
    simtime_t x = SimTime(d.inUnit(SIMTIME_US)%cycle.inUnit(SIMTIME_US),SIMTIME_US);
if(index==7){
        if (( tgate->isGateOpen() )){
            index-=1;
        }
//        else if (( tgate->isGateOpen() && (x>SimTime(30,SIMTIME_US)) )){
//                    index-=1;
//                }
}
#endif
    send(packet, "out", index);
//    AlgorithmicDropperBase::sendOut(packet);

}

} // namespace detnetmod
