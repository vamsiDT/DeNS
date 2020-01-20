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
import detnetmod.detnetmod.node.VlanEtherHostSchedDetnet;

import detnetmod.detnetmod.node.DetnetHost;
import inet.examples.inet.ipv4hook.MyHost;
import inet.linklayer.ieee8022.Ieee8022Llc;
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


network TestScenariodetnet5
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
        switchA: detnetSwitch {
            parameters:
                @display("p=415.72498,82.84375");
            gates:
                ethg[2];
        }
        switchB: detnetSwitch {
            parameters:
                @display("p=659.7375,82.84375");
            gates:
                ethg[2];
        }
        priority0: VlanEtherHostSchedDetnet {
            @display("p=158.15625,158.15625");
        }
        configurator: Ipv4NetworkConfigurator {
            @display("p=87.362495,460.91248");
        }
        prioritysink0: DetnetHost {
            @display("p=1067.9313,228.95");
        }
    connections:
        priority0.ethg <--> C2 <--> switchA.ethg[0];
        prioritysink0.ethg[0] <--> C2 <--> switchB.ethg[0];
        //                prioritysink0.detnetinout[0] <--> C2 <--> switchB.ethg[0];
        switchA.ethg[1] <--> C2 <--> switchB.ethg[1];

}
