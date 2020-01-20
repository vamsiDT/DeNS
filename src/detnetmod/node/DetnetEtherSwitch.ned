//
// Copyright (C) 2003 Andras Varga; CTIE, Monash University, Australia
//               2010 Zoltan Bojthe
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program; if not, see <http://www.gnu.org/licenses/>.
//

package detnetmod.detnetmod.node;

import detnetmod.detnetmod.ethernetInterface.detnetFw;
import inet.common.MessageDispatcher;
import inet.common.lifecycle.NodeStatus;
import inet.common.packet.recorder.PcapRecorder;
import inet.linklayer.configurator.L2NodeConfigurator;
import inet.linklayer.contract.IEthernetInterface;
import inet.linklayer.contract.IExternalInterface;
import inet.linklayer.contract.IMacAddressTable;
import inet.linklayer.contract.IMacRelayUnit;
import inet.linklayer.contract.ISpanningTree;
import inet.linklayer.ethernet.IEtherEncap;
import inet.networklayer.common.InterfaceTable;
import nesting.ieee8021q.clock.IClock;
import nesting.ieee8021q.clock.IdealClock;
import nesting.ieee8021q.queue.gating.ScheduleSwap;
import nesting.ieee8021q.relay.FilteringDatabase;

import inet.common.queue.Delayer;
import nesting.ieee8021q.relay.RelayUnit;
//
// Model of an Ethernet switch.
//
// The duplexChannel attributes of the MACs must be set according to the
// medium connected to the port; if collisions are possible (it's a bus or hub)
// it must be set to false, otherwise it can be set to true.
// By default used half duples CSMA/CD mac
//
//
module DetnetEtherSwitch
{
    parameters:
        @networkNode();
        @labels(node,ethernet-node);
        @display("i=device/switch");
        int numPcapRecorders = default(0);
        bool hasStatus = default(false);
        bool hasStp = default(false);
        bool csmacdSupport = default(true);  // by default use CSMA/CD --FIXME remove
        string fcsMode @enum("declared","computed") = default("declared");
        string spanningTreeProtocol = default("Stp");
        int numEthInterfaces = default(1);  // minimum number of ethernet interfaces
        int numExtInterfaces = default(0);
        eth[*].encap.typename = "EtherEncapDummy";
        eth[*].csmacdSupport = csmacdSupport;
        eth[*].mac.typename = default(csmacdSupport ? "EtherMac" : "EtherMacFullDuplex");
        *.fcsMode = fcsMode;
        ext[*].encapDecap.typename = default("");
        ext[*].rawSocket.protocol = default("ethernetMac");
        *.interfaceTableModule = default(absPath(".interfaceTable"));
        *.macTableModule = default(absPath(".macTable"));

        relayUnit.hasStp = hasStp;
        encap.registerProtocol = true;

    gates:
        inout ethg[numEthInterfaces] @labels(EtherFrame-conn);
    submodules:
        relayUnit: <default("ForwardingRelayUnit")> like RelayUnit {
            numberOfPorts = sizeof(ethg);
            @display("p=532,323");
        }
        processingDelay[sizeof(ethg)]: Delayer {
            @display("p=385,442,r,200;i=block/delay");
            delay = default(4us);
        }

        status: NodeStatus if hasStatus {
            @display("p=100,400;is=s");
        }
        l2NodeConfigurator: L2NodeConfigurator if hasStp {
            @display("p=100,300;is=s");
        }
        interfaceTable: InterfaceTable {
            @display("p=100,200;is=s");
        }
        pcapRecorder[numPcapRecorders]: PcapRecorder {
            @display("p=100,500;is=s");
        }
        macTable: <default("MacAddressTable")> like IMacAddressTable {
            @display("p=100,100;is=s");
        }
        relayUnit1: <default(firstAvailable("Ieee8021dRelay","MacRelayUnit"))> like IMacRelayUnit {
            @display("p=812,323;is=m");
        }
        stp: <spanningTreeProtocol> like ISpanningTree if hasStp {
            @display("p=474,92");
        }
        up: MessageDispatcher {
            parameters:
                @display("p=800,160;b=1200,5");
        }
        down: MessageDispatcher {
            parameters:
                @display("p=812,390;b=1200,5");
        }
        eth[sizeof(ethg)]: <default("detnetFw")> like IEthernetInterface {
            parameters:
                mac.mtu = 1500B;
                queuing.tsAlgorithms[*].macModule = "^.^.^.eth[" + string(index) + "].mac";
                queuing.gateController.macModule = "^.^.^.eth[" + string(index) + "].mac";
                mac.promiscuous = true;
                @display("p=250,450,row,150;q=txQueue");
                mac.queueModule = "^.^.eth[" + string(index) + "].queuing.transmissionSelection";
        }
        encap: <default("EtherEncap")> like IEtherEncap {
            @display("p=812,231");
        }
        ext[numExtInterfaces]: <default("ExtInterface")> like IExternalInterface {
            parameters:
                @display("p=1125,616,row,150;q=txQueue");
        }
        clock: <"IdealClock"> like IClock {
            @display("p=50,37");
        }
        scheduleSwap: ScheduleSwap {
            @display("p=127,38");
        }
        filteringDatabase: FilteringDatabase {
            @display("p=250,38");
        }
    connections:
        for i=0..sizeof(ethg)-1 {
            eth[i].phys <--> ethg[i];
            eth[i].upperLayerOut --> processingDelay[i].in;
            processingDelay[i].out --> relayUnit.in[i];
            relayUnit.out[i] --> eth[i].upperLayerIn;

        }

        if hasStp {
            stp.relayIn <-- up.out++; // relayUnit.stpOut;
            stp.relayOut --> up.in++; // relayUnit.stpIn;
        }
        for i=0..numExtInterfaces-1 {
            ext[i].upperLayerOut --> down.in++;
            ext[i].upperLayerIn <-- down.out++;
        }
        relayUnit1.ifOut --> down.in++;
        relayUnit1.ifIn <-- down.out++;

        encap.lowerLayerIn <-- relayUnit1.upperLayerOut;
        encap.lowerLayerOut --> relayUnit1.upperLayerIn;

        encap.upperLayerIn <-- up.out++;
        encap.upperLayerOut --> up.in++;

}

