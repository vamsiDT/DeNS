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

import detnetmod.detnetmod.ethernetInterface.detnetSwitch;
import inet.networklayer.flooding.Flooding;
import inet.node.ethernet.EtherSwitch;
import inet.node.inet.Router;
import ned.DatarateChannel;
import nesting.node.ethernet.VlanEtherSwitchPreemptable;
import nesting.node.ethernet.VlanEtherHostSched;
import nesting.node.ethernet.VlanEtherHostFullLoad;
import nesting.node.ethernet.VlanEtherHostQ;


network TestScenariodetnet1
{
    @display("bgb=922,513");
    types:
        channel C extends DatarateChannel
        {
            delay = 0.1us;
            datarate = 1Gbps;
        }
    submodules:
        switchA: detnetSwitch {
            parameters:
                @display("p=165,121");
            gates:
                ethg[4];
        }
        switchB: detnetSwitch {
            parameters:
                @display("p=367,136");
            gates:
                ethg[4];
        }
        workstation1: VlanEtherHostFullLoad {
            @display("p=56,56");
        }
        workstation2: VlanEtherHostFullLoad {
            @display("p=149,23");
        }
        backupServer0: VlanEtherHostQ {
            @display("p=470,38");
        }
        backupServer1: VlanEtherHostQ {
            @display("p=527,104");
        }
        robotController: VlanEtherHostSched {
            @display("p=75.2375,200.2475");
        }
        roboticArm: VlanEtherHostQ {
            @display("p=480,211");
        }
    connections:
        robotController.ethg <--> C <--> switchA.ethg[0];
        workstation1.ethg <--> C <--> switchA.ethg[1];
        workstation2.ethg <--> C <--> switchA.ethg[2];

        roboticArm.ethg <--> C <--> switchB.ethg[0];
        backupServer0.ethg <--> C <--> switchB.ethg[1];
        backupServer1.ethg <--> C <--> switchB.ethg[2];


        switchA.ethg[3] <--> C <--> switchB.ethg[3];
}
