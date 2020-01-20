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

package detnetmod.simulations.experiments_l3.activeflows_test;

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
        host: DetnetHost {
            @display("p=313.1975,262.35376,m,4,150,100;is=l");
        }
        sink: DetnetHost {
            @display("p=1110.4275,325.4,m,4,150,100;is=l");
        }
        configurator: Ipv4NetworkConfigurator {
            @display("p=797.23,242.01625;is=s");
        }
        routerA: DetnetRouter {
            parameters:
                numEthInterfaces = 2;
                @display("p=681.3062,492.16748;is=l");
        }
    connections:
            host.ethg[0] <--> C1 <--> routerA.ethg++;
            sink.ethg[0] <--> C0 <--> routerA.ethg++;

}
