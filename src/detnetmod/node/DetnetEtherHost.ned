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


import inet.applications.generic.IIpvxTrafficGenerator;
import inet.common.queue.Sink;
import inet.linklayer.contract.IEthernetInterface;
import inet.node.inet.StandardHost;
import nesting.common.queue.EtherTrafGenQueue;
import detnetmod.detnetmod.applications.VlanEtherTrafGenSchedDetnet;

//
// VLAN capable host module, that sends packets with the maximum throughput the
// physical medium allows. This module's main use case is to simulate a
// congestion scenarios on a switch.
//
module DetnetEtherHost
{
    parameters:
        @display("i=device/pc2;bgb=363,398");
        @networkNode();
        @labels(node,ethernet-node);
        *.interfaceTableModule = default("");
        eth.mac.queueModule = "^.^.trafGenQueueApp";
    gates:
        inout ethg @labels(EtherFrame-conn);
    submodules:
        trafGenQueueApp: VlanEtherTrafGenSchedDetnet {
            @display("p=192,69");
        }
        sink: Sink {
            @display("p=84,69");
        }
        eth: <default("VlanEthernetInterface")> like IEthernetInterface {
            parameters:
                @display("p=142,283,row,150;q=txQueue");
        }
    connections:
        eth.upperLayerIn <-- trafGenQueueApp.out;
        eth.upperLayerOut --> sink.in++;
        eth.phys <--> ethg;
}