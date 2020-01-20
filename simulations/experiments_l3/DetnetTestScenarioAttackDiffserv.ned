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


network DetnetTestScenarioAttackDiffserv
{
    @display("bgb=1725.8588,753.0325");
    types:
        channel C0 extends DatarateChannel
        {
            delay = 0.1us;
            datarate = 1Gbps;
        }
        channel C1 extends DatarateChannel
        {
            delay = 0.1us;
            datarate = 1Gbps;
        }
        channel C2 extends DatarateChannel
        {
            delay = 0.1us;
            datarate = 1Gbps;
        }
    submodules:
        H0: DetnetHost {
            @display("p=87.917496,68.805");
        }
        H1: DetnetHost {
            @display("p=87.917496,214.06");
        }
        H2: DetnetHost {
            @display("p=87.917496,372.69376");
        }

        H3: DetnetHost {
            @display("p=87.917496,525.59375");
        }
        H4: DetnetHost {
            parameters:
                @display("p=87.917496,661.2925");
        }
        S0: DetnetHost {
            @display("p=1471.6625,68.805");
        }
        S1: DetnetHost {
            @display("p=1471.6625,227.43875");
        }

        S2: DetnetHost {
            @display("p=1471.6625,372.69376");
        }

        S3: DetnetHost {
            @display("p=1471.6625,525.59375");
        }
        S4: DetnetHost {
            @display("p=1471.6625,661.2925");
        }

        configurator: Ipv4NetworkConfigurator {
            @display("p=1261.4249,43.958748");
        }
        routerA: DetnetRouter {
            parameters:
                numEthInterfaces = 3;
                @display("p=640.26874,342.11374");
        }
        routerB: DetnetRouter {
            parameters:
                numEthInterfaces = 3;
                @display("p=902.11,342.11374");
        }
        switchA: detnetStandardSwitch {
            @display("p=382.25,236.995");
        }
        switchB: detnetStandardSwitch {
            @display("p=382.25,460.61124");
        }
        switchC: detnetStandardSwitch {
            @display("p=1217.4662,236.995");
        }
        switchD: detnetStandardSwitch {
            @display("p=1217.4662,460.61124");
        }
    connections:
//        for i=0..9 {
//            H0[i].ethg[0] <--> C0 <--> switchA.ethg++;
//        }
        routerA.ethg[2] <--> C0 <--> routerB.ethg[2];
		switchA.ethg++ <--> C0 <--> H0.ethg[0];
        switchA.ethg++ <--> C0 <--> H1.ethg[0];
        switchA.ethg++ <--> C0 <--> H2.ethg[0];
        switchB.ethg++ <--> C0 <--> H3.ethg[0];
        switchB.ethg++ <--> C0 <--> H4.ethg[0];
        routerA.ethg[0] <--> C0 <--> switchA.ethg++;
        routerA.ethg[1] <--> C0 <--> switchB.ethg++;
        switchC.ethg++ <--> C0 <--> S0.ethg[0];
        switchC.ethg++ <--> C0 <--> S1.ethg[0];
        switchC.ethg++ <--> C0 <--> S2.ethg[0];
        switchD.ethg++ <--> C0 <--> S3.ethg[0];
        switchD.ethg++ <--> C0 <--> S4.ethg[0];
        routerB.ethg[0] <--> C0 <--> switchC.ethg++;
        routerB.ethg[1] <--> C0 <--> switchD.ethg++;
}
