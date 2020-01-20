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

package detnetmod.simulations.experiments;


import detnetmod.detnetmod.ethernetInterface.detnetStandardSwitch;

import detnetmod.detnetmod.ethernetInterface.detnetSwitch;
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


network DetnetTestScenarioAttackLB
{
    @display("bgb=1201.485,614.6325");
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
        switchA: detnetStandardSwitch {
            parameters:
                @display("p=353.0375,131.955");

            gates:
                ethg[8];
        }
        switchB: detnetStandardSwitch {
            parameters:
                @display("p=638.94,131.955");
            gates:
                ethg[8];
        }
        switchC: detnetStandardSwitch {
            parameters:
                @display("p=477.48123,131.04375");

            gates:
                ethg[2];
        }
        switchD: detnetStandardSwitch {
            parameters:
                @display("p=477.48123,42.175");

            gates:
                ethg[2];
        }
        switchE: detnetStandardSwitch {
            parameters:
                @display("p=477.48123,242.50624");

            gates:
                ethg[2];
        }
        priority0: VlanEtherHostSchedDetnet {
            @display("p=244.2325,67.135");
        }
        priority1: VlanEtherHostSchedDetnet {
            @display("p=336.8325,33.5675");
        }
        priority2: VlanEtherHostFullLoadDetnet {
            @display("p=199.09,158.5775");
        }

        nonpriority0: VlanEtherHostFullLoadDetnet {
            @display("p=145.845,363.45502");
        }
        nonpriority1: VlanEtherHostFullLoadDetnet {
            parameters:
                @display("p=277.15,403.675");
        }
        prioritysink0: DetnetHost {
            @display("p=920.2125,43.985");
        }
        prioritysink1: DetnetHost {
            @display("p=946.835,121.5375");
        }

        prioritysink2: DetnetHost {
            @display("p=920.2125,203.72");
        }

        nonprioritysink0: DetnetHost {
            @display("p=929.47253,346.0925");
        }
        nonprioritysink1: DetnetHost {
            @display("p=850.7625,424.8025");
        }

        configurator: Ipv4NetworkConfigurator {
            @display("p=477.48123,551.2875");
        }
    //        nonprioritysink2: DetnetHost {
    //            @display("p=488.025,36.149998");
    //        }
    connections:
        priority0.ethg <--> C0 <--> switchA.ethg[0];
        priority1.ethg <--> C0 <--> switchA.ethg[1];
        priority2.ethg <--> C0 <--> switchA.ethg[2];

        nonpriority0.ethg <--> C1 <--> switchA.ethg[3];
        nonpriority1.ethg <--> C2 <--> switchA.ethg[4];

        prioritysink0.ethg[0] <--> C0 <--> switchB.ethg[0];
        prioritysink1.ethg[0] <--> C0 <--> switchB.ethg[1];
        prioritysink2.ethg[0] <--> C0 <--> switchB.ethg[2];

        nonprioritysink0.ethg[0] <--> C1 <--> switchB.ethg[3];
        nonprioritysink1.ethg[0] <--> C1 <--> switchB.ethg[4];


        switchA.ethg[5] <--> C2 <--> switchC.ethg[0];
        switchC.ethg[1] <--> C2 <--> switchB.ethg[5];
        
        switchA.ethg[6] <--> C2 <--> switchD.ethg[0];
        switchA.ethg[7] <--> C2 <--> switchE.ethg[0];
        
        switchB.ethg[6] <--> C2 <--> switchD.ethg[1];
        switchB.ethg[7] <--> C2 <--> switchE.ethg[1];
        
//        switchA.ethg[6] <--> C2 <--> nonprioritysink2.ethg[0];


}
