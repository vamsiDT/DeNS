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

package detnetmod.simulations.test;

//import detnetmod.detnetmod.applications.udp.UdpVlanServer;
import detnetmod.detnetmod.ethernetInterface.detnetSwitch;
import detnetmod.detnetmod.node.DetnetEtherSwitch;

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


network TestScenariodetnet3
{
    @display("bgb=1201.485,614.6325");
    types:
        channel C0 extends DatarateChannel
        {
            delay = 2us;
            datarate = 10Mbps;
        }
        channel C1 extends DatarateChannel
        {
            delay = 2us;
            datarate = 10Mbps;
        }
        channel C2 extends DatarateChannel
        {
            delay = 2us;
            datarate = 100Mbps;
        }
    submodules:
        switchA: detnetSwitch {
            parameters:
                @display("p=353.0375,131.955");
            gates:
                ethg[6];
        }
        switchB: detnetSwitch {
            parameters:
                @display("p=638.94,131.955");
            gates:
                ethg[6];
        }
        priority0: VlanEtherHostSched {
            @display("p=244.2325,67.135");
        }
        priority1: VlanEtherHostFullLoad {
            @display("p=336.8325,33.5675");
        }
        priority2: VlanEtherHostFullLoad {
            @display("p=199.09,158.5775");
        }

        nonpriority0: VlanEtherHostFullLoad {
            @display("p=145.845,363.45502");
        }
        nonpriority1: VlanEtherHostFullLoad {
                @display("p=277.15,403.675");
        }
        prioritysink0: VlanEtherHostQ {
            @display("p=920.2125,43.985");
        }
        prioritysink1: VlanEtherHostQ {
            @display("p=946.835,121.5375");
        }

        prioritysink2: VlanEtherHostQ {
            @display("p=920.2125,203.72");
        }

        nonprioritysink0: VlanEtherHostQ {
            @display("p=929.47253,346.0925");
        }
        nonprioritysink1: VlanEtherHostQ {
            @display("p=850.7625,424.8025");
        }

        configurator: Ipv4NetworkConfigurator {
            @display("p=477.48123,551.2875");
        }
    connections:
        priority0.ethg <--> C2 <--> switchA.ethg[0];
        priority1.ethg <--> C0 <--> switchA.ethg[1];
        priority2.ethg <--> C0 <--> switchA.ethg[2];

        nonpriority0.ethg <--> C1 <--> switchA.ethg[3];
        nonpriority1.ethg <--> C1 <--> switchA.ethg[4];
        prioritysink0.ethg <--> C2 <--> switchB.ethg[0];
        prioritysink1.ethg <--> C0 <--> switchB.ethg[1];
        prioritysink2.ethg <--> C0 <--> switchB.ethg[2];

        nonprioritysink0.ethg <--> C1 <--> switchB.ethg[3];
        nonprioritysink1.ethg <--> C1 <--> switchB.ethg[4];


        switchA.ethg[5] <--> C2 <--> switchB.ethg[5];

}
