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

package detnetmod.detnetmod.node;

import inet.linklayer.ethernet.EtherMacFullDuplex;
import inet.linklayer.ethernet.IEtherEncap;
import inet.networklayer.common.InterfaceTable;
import inet.linklayer.contract.IEthernetInterface;
import nesting.application.ethernet.VlanEtherTrafGenSched;
import nesting.ieee8021q.clock.IClock;
import nesting.ieee8021q.queue.gating.ScheduleSwap;
import nesting.linklayer.ethernet.VLANEncap;

import detnetmod.detnetmod.applications.VlanEtherTrafGenSchedDetnet;
import nesting.linklayer.ethernet.VlanEthernetInterface;


//
// Simple Host with the capability to send out frames according to a given schedule
//
// @see ~VlanEtherTrafGen
//
module VlanEtherHostSchedDetnet
{
    parameters:
        string fcsMode @enum("declared","computed") = default("declared");
        @networkNode();
        @labels(node,ethernet-node);
        @display("i=device/pc2;bgb=385,371");
        *.interfaceTableModule = default(absPath(".interfaceTable"));
        *.fcsMode = fcsMode;
    gates:
        inout ethg @labels(EtherFrame-conn);
    submodules:
        trafGenSchedApp: VlanEtherTrafGenSchedDetnet {
            @display("p=212.768,69.296005");
        }
        interfaceTable: InterfaceTable {
            parameters:
                @display("p=330,215;is=s");
        }
        clock: <default("IdealClock")> like IClock {
            @display("p=330,69");
        }
        scheduleSwap: ScheduleSwap {
            usedInHost = true;

            @display("p=330,149;b=29,25");
        }
        eth: <default("VlanEthernetInterface")> like IEthernetInterface {
            parameters:
                @display("p=138,271,row,150;q=txQueue");
//                mac.queueModule = "^.^.eth.queuing.transmissionSelection";
//                mac.mtu = 1500B;
//                mac.promiscuous = true;
//                queuing.tsAlgorithms[*].macModule = "^.^.^.eth.mac";
//                queuing.gateController.macModule = "^.^.^.eth.mac";
        }
    connections:
        trafGenSchedApp.out --> eth.upperLayerIn;
        eth.upperLayerOut --> trafGenSchedApp.in;

        eth.phys <--> ethg;
}
