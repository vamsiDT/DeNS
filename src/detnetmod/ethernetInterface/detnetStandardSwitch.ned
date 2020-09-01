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

package detnetmod.detnetmod.ethernetInterface;

import detnetmod.detnetmod.ieee8021q.tunnel8021q.DetnetVlanInterface;
import inet.linklayer.contract.IMacAddressTable;
import inet.common.MessageDispatcher;
import inet.common.packet.recorder.PcapRecorder;
import inet.common.queue.Delayer;
import inet.linklayer.contract.ISpanningTree;
import inet.linklayer.contract.IVlanInterface;
import inet.linklayer.ethernet.EtherEncap;
import inet.linklayer.ethernet.EtherEncapDummy;
import inet.linklayer.ethernet.switch.MacRelayUnit;
import inet.linklayer.ieee8021d.stp.Stp;
import inet.networklayer.common.InterfaceTable;
import inet.linklayer.contract.IEthernetInterface;
import inet.node.ethernet.EtherSwitch;
import inet.node.inet.Router;
import nesting.ieee8021q.clock.IClock;
import nesting.ieee8021q.relay.FilteringDatabase;
import nesting.ieee8021q.queue.gating.ScheduleSwap;
import nesting.ieee8021q.relay.RelayUnit;
import nesting.linklayer.ethernet.VlanEthernetInterfaceSwitchPreemptable;
import detnetmod.detnetmod.ieee8021q.relay.macrelay;

//
// This module implements a switch that supports frame preemption.
//
module detnetStandardSwitch
{
    parameters:
        @networkNode();
        @display("i=device/switch;bgb=1665.8324,1250.97");
        *.interfaceTableModule = default(absPath(".interfaceTable"));
        *.macTableModule = default(absPath(".macTable"));
        string fcsMode @enum("declared","computed") = default("declared");
        bool hasStp = default(false);
        string spanningTreeProtocol = default("Stp");
        relayUnit.hasStp = hasStp;
        etherEncap.registerProtocol = true;
        *.fcsMode = fcsMode;
        //        eth[*].etherEncapE.typename = default("EtherEncapDummy");
        //        eth[*].etherEncapP.typename = default("EtherEncapDummy");
        eth[*].etherEncap.typename = default("EtherEncapDummy");
        eth[*].vlanEncap.typename = default("VLAN8021qEncap");
        int numVlanInterfaces = default(0);
        bool isEthDetnet = default(true);
    gates:
        inout ethg[];
    submodules:

        eth[sizeof(ethg)]: <default("DetnetVlanEthernetInterfaceSwitchPreemptable")> like IEthernetInterface {
            parameters:
                mac.queueModule = "^.^.eth[" + string(index) + "].queuing.transmissionSelection";
                mac.mtu = 1500B;
                mac.promiscuous = true;
                queuing.tsAlgorithms[*].macModule = "^.^.^.eth[" + string(index) + "].mac";
                queuing.gateController.macModule = "^.^.^.eth[" + string(index) + "].mac";
                queuing.transmissionSelection.switchparam = false;
                @display("p=208.99376,1086.7676,r,200");
        }

        vlan[numVlanInterfaces]: <default("DetnetVlanInterface")> like IVlanInterface {
            parameters:
                @display("p=1362.1476,880.23254,r,200");
        }

        clock: <default("IdealClock")> like IClock {
            @display("p=349.14252,98.350006;is=s");
        }
        filteringDatabase: FilteringDatabase {
            @display("p=208.99376,169.65376;is=s");
        }
        scheduleSwap: ScheduleSwap {
            @display("p=312.26126,312.26126;i=block/switch;is=s");
        }
        interfaceTable: InterfaceTable {
            @display("p=132.7725,41.79875;is=s");
        }
        macTable: <default("MacAddressTable")> like IMacAddressTable {
            @display("p=90.973755,312.26126;is=s");
        }

        pcapRecorder: PcapRecorder {
            @display("p=135.23125,595.0175");
        }
        down: MessageDispatcher {
            @display("p=779.42377,607.3113;b=954.14374,12.0625");
        }
        relayUnit: macrelay {
            parameters:
                numberOfPorts = sizeof(ethg);
                @display("p=779.42377,489.29126");
        }
        etherEncap: EtherEncap {
            @display("p=779.42377,312.26126");
        }
        up: MessageDispatcher {
            @display("p=779.42377,194.24126;b=923.9875,12.0625");
        }
        stp: <spanningTreeProtocol> like ISpanningTree if hasStp {
            @display("p=779.42377,61.468754");
        }

    connections:
        for i=0..sizeof(ethg)-1 {
            eth[i].phys <--> ethg[i];
            eth[i].upperLayerOut --> down.in++;

            down.out++ --> eth[i].upperLayerIn;

        }
        for i=0..numVlanInterfaces-1 {
            vlan[i].upperLayerOut --> down.in++;
            down.out++ --> vlan[i].upperLayerIn;
        }
        relayUnit.upperLayerOut --> etherEncap.lowerLayerIn;
        etherEncap.lowerLayerOut --> relayUnit.upperLayerIn;

        relayUnit.ifOut --> down.in++;
        down.out++ --> relayUnit.ifIn;
        etherEncap.upperLayerOut --> up.in++;
        up.out++ --> etherEncap.upperLayerIn;
        if hasStp {
            stp.relayOut --> up.in++;
            up.out++ --> stp.relayIn;
        }
}
