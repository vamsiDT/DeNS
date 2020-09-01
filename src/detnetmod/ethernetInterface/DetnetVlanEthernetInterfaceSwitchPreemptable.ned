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

@namespace();

import detnetmod.detnetmod.linklayer.ethernet.VLAN8021qEncap;
import inet.common.MessageDispatcher;
import inet.common.queue.Delayer;
import inet.common.queue.IOutputQueue;
import inet.linklayer.ethernet.EtherEncap;
import inet.linklayer.ieee8021q.IIeee8021qEncap;
import nesting.ieee8021q.queue.Queuing;
import nesting.linklayer.ethernet.VLANEncap;
import nesting.linklayer.framePreemption.FrameForward;
import nesting.linklayer.framePreemption.EtherMACFullDuplexPreemptable;
import inet.linklayer.contract.IEthernetInterface;
import inet.linklayer.ethernet.IEtherEncap;
import inet.networklayer.common.InterfaceEntry;
import inet.linklayer.contract.ITrafficConditioner;
import detnetmod.detnetmod.ethernetInterface.EtherMACFullDuplexPreemptableMod;
//
// This module implements an ethernet interface that supports frame preemption.
//
module DetnetVlanEthernetInterfaceSwitchPreemptable extends InterfaceEntry like IEthernetInterface
{
    parameters:
        @class(inet::InterfaceEntry);
        string interfaceTableModule;
        string address = default("auto"); // MAC address
        string fcsMode @enum("declared","computed") = default("declared");
        @display("i=block/ifcard;bgl=2;bgb=600.8275,544.88");
        *.interfaceTableModule = default(absPath(interfaceTableModule));
        *.fcsMode = fcsMode;
        bool framePreemptionDisabled = default(true);
//        mac.queueModule = queueing

    gates:
        input upperLayerIn;
        output upperLayerOut;
        inout phys @labels(EtherFrame);
    submodules:

        ingressTC: <default("")> like ITrafficConditioner if typename != "" {
            @display("p=107.016,77.532");
        }
        egressTC: <default("")> like ITrafficConditioner if typename != "" {
            @display("p=445.536,77.532");
        }

        // mac supporting frame preemption
        mac: EtherMACFullDuplexPreemptableMod {
            @display("p=276.276,442.26");
        }
        queuing: queuingDetnet {
            parameters:
                @display("p=488.124,418.236;q=l2queue");
        }
        vlanEncap: <default("")> like IIeee8021qEncap if typename != "" {
            parameters:
                @display("p=370.188,264.264");
        }
        processingDelay: Delayer {
            @display("p=488.124,264.264");
        }
        etherEncap: <default("EtherEncap")> like IEtherEncap {
            parameters:
                @display("p=276.276,120.12");
        }
    connections:
        mac.upperLayerOut --> vlanEncap.lowerLayerIn if exists(vlanEncap);
        mac.upperLayerOut --> etherEncap.lowerLayerIn if !exists(vlanEncap);

        mac.phys <--> { @display("m=s"); } <--> phys;
        processingDelay.out --> queuing.in;

        etherEncap.lowerLayerOut --> vlanEncap.upperLayerIn if exists(vlanEncap);
        vlanEncap.upperLayerOut --> etherEncap.lowerLayerIn if exists(vlanEncap);

        queuing.eOut --> mac.upperLayerIn;
        queuing.pOut --> mac.upperLayerPreemptableIn;

        vlanEncap.lowerLayerOut --> processingDelay.in if exists(vlanEncap);
        etherEncap.lowerLayerOut --> processingDelay.in if !exists(vlanEncap);

        etherEncap.upperLayerOut --> { @display("m=n"); } --> upperLayerOut if !exists(ingressTC);
        if exists(ingressTC) {
            etherEncap.upperLayerOut --> ingressTC.in;
            ingressTC.out --> { @display("m=n"); } --> upperLayerOut;
        }

        upperLayerIn --> { @display("m=n"); } --> etherEncap.upperLayerIn if !exists(egressTC);
         // there is egressTC
        if exists(egressTC) {
            upperLayerIn --> { @display("m=n"); } --> egressTC.in;
            egressTC.out --> etherEncap.upperLayerIn;
        }
}
