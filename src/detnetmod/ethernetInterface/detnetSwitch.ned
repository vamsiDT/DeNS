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


import inet.common.packet.recorder.PcapRecorder;
import inet.common.queue.Delayer;
import inet.networklayer.common.InterfaceTable;
import inet.linklayer.contract.IEthernetInterface;
import inet.node.ethernet.EtherSwitch;
import inet.node.inet.Router;
import nesting.ieee8021q.clock.IClock;
import nesting.ieee8021q.relay.FilteringDatabase;
import nesting.ieee8021q.queue.gating.ScheduleSwap;
import nesting.ieee8021q.relay.RelayUnit;
import nesting.linklayer.ethernet.VlanEthernetInterfaceSwitchPreemptable;

//
// This module implements a switch that supports frame preemption.
//
module detnetSwitch
{
    parameters:
        @networkNode();
        @display("i=device/switch;bgb=,466");
        **.interfaceTableModule = default("");
    gates:
        inout ethg[];
    submodules:
        //        DetnetVlanEthernetInterfaceSwitchPreemptable
        eth[sizeof(ethg)]: <default("DetnetVlanEthernetInterfaceSwitchPreemptable")> like IEthernetInterface {
            parameters:
                mac.queueModule = "^.^.eth[" + string(index) + "].queuing.transmissionSelection";
                mac.mtu = 1500B;
                mac.promiscuous = true;
                queuing.tsAlgorithms[*].macModule = "^.^.^.eth[" + string(index) + "].mac";
                queuing.gateController.macModule = "^.^.^.eth[" + string(index) + "].mac";
                @display("p=132,391,r,200");
        }
        relayUnit: <default("ForwardingRelayUnit")> like RelayUnit {
            numberOfPorts = sizeof(ethg);
            @display("p=219,189");
        }
        processingDelay[sizeof(ethg)]: Delayer {
            @display("p=132,268,r,200;i=block/delay");
            delay = default(4us);
        }
        clock: <default("IdealClock")> like IClock {
            @display("p=182,31;is=s");
        }
        filteringDatabase: FilteringDatabase {
            @display("p=60,105;is=s");
        }
        scheduleSwap: ScheduleSwap {
            @display("p=182,105;i=block/switch;is=s");
        }
        interfaceTable: InterfaceTable {
            @display("p=60,31;is=s");
        }

        pcapRecorder: PcapRecorder {
            @display("p=60,198");
        }
    connections:
        for i=0..sizeof(ethg)-1 {
            eth[i].phys <--> ethg[i];
            eth[i].upperLayerOut --> processingDelay[i].in;
            processingDelay[i].out --> relayUnit.in[i];
            relayUnit.out[i] --> eth[i].upperLayerIn;
        }
}
