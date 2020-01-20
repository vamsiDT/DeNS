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

package detnetmod.simulations.experiments_l3.performance_evaluation;

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
import inet.node.ethernet.EtherSwitch;
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
        hostPriority[10]: StandardHost {
            parameters:
            	numEthInterfaces=1;
            	@display("p=119.99125,77.2825,m,4,150,100;is=l");
        }
        hostX[2]: StandardHost {
            parameters:
            	numEthInterfaces=1;
            @display("p=91.518745,418.95248,c,100,10;is=l");
        }

        hostOther[10]: StandardHost {
            parameters:
            	numEthInterfaces=1;
            @display("p=119.99125,724.015,m,4,150,100;is=l");
        }

        sinkPriority[10]: StandardHost {
            parameters:
            	numEthInterfaces=1;
            @display("p=963.9975,77.2825,m,4,150,100;is=l");
        }
        sinkX[2]: StandardHost {
            parameters:
            	numEthInterfaces=1;
            @display("p=1482.6038,418.95248,c,100;is=l");
        }
        sinkOther[10]: StandardHost {
            parameters:
            	numEthInterfaces=1;
            @display("p=963.9975,724.015,m,4,150,100;is=l");
        }

        configurator: Ipv4NetworkConfigurator {
            @display("p=797.23,242.01625;is=s");
        }
        routerA: Router {
            parameters:
                numEthInterfaces = 3;
                @display("p=681.3062,492.16748;is=l");
        }
        routerB: Router {
            parameters:
                numEthInterfaces = 3;
                @display("p=913.15375,492.16748;is=l");
        }
        switchA: EtherSwitch {
            @display("p=504.37,386.4125;is=l");
        }
        switchB: EtherSwitch {
            @display("p=504.37,581.65247;is=l");
        }
        switchC: EtherSwitch {
            @display("p=1126.6975,386.4125;is=l");
        }
        switchD: EtherSwitch {
            @display("p=1126.6975,581.65247;is=l");
        }
    connections:
        for i=0..9 {
            hostOther[i].ethg[0] <--> C0 <--> switchB.ethg++;
            sinkOther[i].ethg[0] <--> C0 <--> switchD.ethg++;
        }

        for i=0..9 {
            hostPriority[i].ethg[0] <--> C0 <--> switchA.ethg++;
            sinkPriority[i].ethg[0] <--> C0 <--> switchC.ethg++;
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
