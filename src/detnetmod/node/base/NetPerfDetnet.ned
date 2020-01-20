//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

package detnetmod.detnetmod.node.base;
// * --------------------------------------------------------------------------
// *
// *     //====//  //===== <===//===>  //====//
// *    //        //          //      //    //    SCTP Optimization Project
// *   //=====   //          //      //====//   ==============================
// *        //  //          //      //           University of Duisburg-Essen
// *  =====//  //=====     //      //
// *
// * --------------------------------------------------------------------------
// *
// *   Copyright (C) 2015 by Thomas Dreibholz
// *
// *   This program is free software: you can redistribute it and/or modify
// *   it under the terms of the GNU General Public License as published by
// *   the Free Software Foundation, either version 3 of the License, or
// *   (at your option) any later version.
// *
// *   This program is distributed in the hope that it will be useful,
// *   but WITHOUT ANY WARRANTY; without even the implied warranty of
// *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// *   GNU General Public License for more details.
// *
// *   You should have received a copy of the GNU General Public License
// *   along with this program.  If not, see <http://www.gnu.org/licenses/>.
// *
// *   Contact: dreibh@simula.no

import inet.applications.contract.INetPerfMeterApp;
import detnetmod.detnetmod.applications.SetKindNetPerf;
//
// NetPerfMeter application host.
//
module NetPerfDetnet extends TransportLayerNodeBase
{
    parameters:
        @figure[submodules];
        @display("i=device/pc2");
        int numNetPerfMeterApps = default(0);   // T.D. 17.11.09
        forwarding = default(false);  // disable routing by default
        *.routingTableModule = default("^.ipv4.routingTable");

    submodules:
        netPerfMeterApp[numNetPerfMeterApps]: <default("NetPerfMeter")> like INetPerfMeterApp {  // T.D. 17.11.09
            parameters:
                @display("p=847.35,225.22499,row,150");
        }
        setFlowIdNetPerf[numNetPerfMeterApps]: SetKindNetPerf {
            @display("p=1014.3,225.22499;i=block/star");
        }

    connections allowunconnected:
        for i=0..numNetPerfMeterApps-1 {
            netPerfMeterApp[i].tcpOut --> setFlowIdNetPerf[i].UpperLayerInTcp if hasTcp;
            setFlowIdNetPerf[i].LowerLayerOutTcp --> tcp.appIn if hasTcp;
            netPerfMeterApp[i].tcpIn <-- tcp.appOut if hasTcp;
            netPerfMeterApp[i].sctpOut --> setFlowIdNetPerf[i].UpperLayerInSctp if hasSctp;
            setFlowIdNetPerf[i].LowerLayerOutSctp --> sctp.appIn if hasSctp;
            netPerfMeterApp[i].sctpIn <-- sctp.appOut if hasSctp;
            netPerfMeterApp[i].udpOut --> setFlowIdNetPerf[i].UpperLayerInUdp if hasUdp;
            setFlowIdNetPerf[i].LowerLayerOutUdp --> udp.appIn if hasUdp;
            netPerfMeterApp[i].udpIn <-- udp.appOut if hasUdp;
        }
}
