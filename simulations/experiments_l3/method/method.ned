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

package detnetmod.simulations.experiments_l3.method;

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


network method
{
    @display("bgb=1246.6887,742.3187");
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
        hostPriority[5]: DetnetHost {
            @display("p=122.025,124.05875,m,3,150,100;is=l");
        }
        hostX[3]: DetnetHost {
            @display("p=122.025,388.44626,c,100,10;is=l");
        }
        sinkPriority[5]: DetnetHost {
            @display("p=823.66876,124.05875,m,3,150,100;is=l");
        }
        sinkX[3]: DetnetHost {
            @display("p=823.66876,388.44626,c,100;is=l");
        }

        configurator: Ipv4NetworkConfigurator {
            @display("p=508.4375,160.66624;is=s");
        }
        switchA: detnetStandardSwitch {
            @display("p=333.535,335.56876;is=l");
        }
        switchB: detnetStandardSwitch {
            @display("p=608.09125,335.56876;is=l");
        }

    connections:
        for i=0..4 {
            hostPriority[i].ethg[0] <--> C1 <--> switchA.ethg++;
            sinkPriority[i].ethg[0] <--> C1 <--> switchB.ethg++;
        }
        for i=0..2 {
            hostX[i].ethg[0] <--> C1 <--> switchA.ethg++;
            sinkX[i].ethg[0] <--> C1 <--> switchB.ethg++;
        }

        switchA.ethg++ <--> C1 <--> switchB.ethg++;
}
