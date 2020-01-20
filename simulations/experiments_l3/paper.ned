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

package detnetmod.simulations.experiments_l3;

import detnetmod.detnetmod.ethernetInterface.detnetStandardSwitch;

import detnetmod.detnetmod.ethernetInterface.detnetSwitch;
import detnetmod.detnetmod.networklayer.DetnetRouter;
import detnetmod.detnetmod.node.DetnetEtherSwitch;
import detnetmod.detnetmod.node.VlanEtherHostSchedDetnet;
import detnetmod.detnetmod.node.VlanEtherHostFullLoadDetnet;

import detnetmod.detnetmod.node.DetnetHost;
import inet.examples.inet.ipv4hook.MyHost;
import inet.linklayer.vlan.VlanInterface;
import inet.networklayer.arp.ipv4.GlobalArp;
import inet.networklayer.configurator.ipv4.Ipv4FlatNetworkConfigurator;
import inet.networklayer.configurator.ipv4.Ipv4NetworkConfigurator;
import inet.networklayer.flooding.Flooding;
import inet.node.inet.Router;
import inet.node.inet.StandardHost;
import ned.DatarateChannel;
import nesting.node.ethernet.VlanEtherSwitchPreemptable;
import nesting.node.ethernet.VlanEtherHostSched;
import nesting.node.ethernet.VlanEtherHostFullLoad;
import nesting.node.ethernet.VlanEtherHostQ;


network paper
{
    @display("bgb=1623.0988,1000.65875");
    types:
        channel C0 extends DatarateChannel
        {
            delay = 0.1us;
            datarate = 1Gbps;
        }
        channel C1 extends DatarateChannel
        {
            delay = 0.1us;
            datarate = 10Gbps;
        }
        channel C2 extends DatarateChannel
        {
            delay = 0.1us;
            datarate = 10Gbps;
        }
    submodules:
        hostPriority[8]: DetnetHost {
            @display("p=116.7075,153.44875,m,4,150,100;is=l");
        }
        hostX[2]: DetnetHost {
            @display("p=88.611244,494.92624,c,100,10;is=l");
        }

        hostOther[8]: DetnetHost {
            @display("p=116.7075,799.6625,m,4,150,100;is=l");
        }

        sinkPriority[8]: DetnetHost {
            @display("p=961.7562,153.44875,m,4,150,100;is=l");
        }
        sinkX[2]: DetnetHost {
            @display("p=1480.4562,494.92624,c,100;is=l");
        }
        sinkOther[8]: DetnetHost {
            @display("p=961.7562,799.6625,m,4,150,100;is=l");
        }

        configurator: Ipv4NetworkConfigurator {
            @display("p=793.1787,317.70374;is=s");
        }
        routerA: DetnetRouter {
            parameters:
                numEthInterfaces = 3;
                @display("p=678.6325,568.40875;is=l");
        }
        routerB: DetnetRouter {
            parameters:
                numEthInterfaces = 3;
                @display("p=909.8862,568.40875;is=l");
        }
        switchA: detnetStandardSwitch {
            @display("p=501.40997,462.50748;is=l");
        }
        switchB: detnetStandardSwitch {
            @display("p=501.40997,657.01996;is=l");
        }
        switchC: detnetStandardSwitch {
            @display("p=1123.85,462.50748;is=l");
        }
        switchD: detnetStandardSwitch {
            @display("p=1123.85,657.01996;is=l");
        }
    connections:
        for i=0..7 {
            hostOther[i].ethg[0] <--> C1 <--> switchB.ethg++;
            sinkOther[i].ethg[0] <--> C1 <--> switchD.ethg++;
        }

        for i=0..7 {
            hostPriority[i].ethg[0] <--> C1 <--> switchA.ethg++;
            sinkPriority[i].ethg[0] <--> C1 <--> switchC.ethg++;
        }
        for i=0..1 {
            hostX[i].ethg[0] <--> C1 <--> switchA.ethg++;
            sinkX[i].ethg[0] <--> C1 <--> switchC.ethg++;
        }

        routerA.ethg++ <--> C2 <--> routerB.ethg++;

        routerA.ethg++ <--> C1 <--> switchA.ethg++;
        routerA.ethg++ <--> C1 <--> switchB.ethg++;

        routerB.ethg++ <--> C1 <--> switchC.ethg++;
        routerB.ethg++ <--> C1 <--> switchD.ethg++;
}
