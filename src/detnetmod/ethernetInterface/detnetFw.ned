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
import inet.common.queue.RedDropper;
import inet.linklayer.contract.IEthernetInterface;
import inet.linklayer.ethernet.EtherEncap;
import inet.linklayer.ethernet.IEtherEncap;
import inet.linklayer.ethernet.switch.MacRelayUnit;
import inet.networklayer.common.InterfaceEntry;
import inet.linklayer.ethernet.EtherMacFullDuplex;
import inet.node.ethernet.EtherHost2;
import inet.node.ethernet.EtherSwitch;
import nesting.ieee8021q.queue.Queuing;
import nesting.ieee8021q.queue.QueuingFrames;
import nesting.linklayer.ethernet.VLANEncap;
import inet.common.queue.Sink;

@namespace();
//
// TODO documentation
//
module detnetFw extends InterfaceEntry like IEthernetInterface
{
    parameters:
        @class(inet::InterfaceEntry);
        string interfaceTableModule;
        string address = default("auto"); // MAC address
        string fcsMode @enum("declared","computed") = default("declared");
        @display("i=block/ifcard;bgl=2;bgb=388,407");
        *.interfaceTableModule = default(absPath(interfaceTableModule));
        *.fcsMode = fcsMode;
        bool framePreemptionDisabled = default(true);

    gates:
        input upperLayerIn;
        output upperLayerOut;
        inout phys @labels(EtherFrame);
    submodules:
        mac: EtherMacFullDuplex {
            @display("p=120,342");
        }
        queuing: queuingDetnet {
            parameters:
                @display("p=239,70;q=l2queue");
        }
        vlanEncapE: VLANEncap {
            @display("p=120,229");
        }
        etherEncap: <default("EtherEncap")> like IEtherEncap {
            parameters:
                @display("p=113,92");
        }
        sink: Sink {
            @display("p=239,251");
        }
    //        etherEncap: EtherEncap {
    //            @display("p=120,85");
    //        }
    connections:
        queuing.pOut --> sink.in++;

        mac.phys <--> { @display("m=s"); } <--> phys;
        etherEncap.upperLayerOut --> upperLayerOut;
        upperLayerIn --> queuing.in;
        queuing.eOut --> etherEncap.upperLayerIn;
        etherEncap.lowerLayerOut --> vlanEncapE.upperLayerIn;
        vlanEncapE.upperLayerOut --> etherEncap.lowerLayerIn;
        vlanEncapE.lowerLayerOut --> mac.upperLayerIn;
        mac.upperLayerOut --> vlanEncapE.lowerLayerIn;
}

