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

package detnetmod.detnetmod.dropper;


simple MfdDropper like Dropper
{
    parameters:
        int numGates = default(1);
		@class(Mfd);
		double DetnetBandwidthReservation=default(10); // 1Gbps
		int vqThreshold = default(10*412*8); // 5 Mtu
		double OutputLinkBandwidth = default(10); // 1Gbps
        string macTableModule = default("^.^.^.macTable"); // The path to the MacAddressTable module
        string interfaceTableModule = default("^.^.^.interfaceTable"); // The path to the InterfaceTable module
		string filteringDatabaseModule = default("^.^.^.filteringDatabase"); // Path to the ~FilteringDatabase module
		string gateController = default("^.gateController");
		string tgate = default("^.tGates[7]");
		bool lbEnabled = default(false);
		int backupPort = default(5);
        @display("i=block/downarrow");
        @signal[packetDropped](type=cPacket);
        @statistic[packetDropCongestion](title="packet drop: congestion"; source=packetDropReasonIsCongestion(packetDropped); record=count,sum(packetBytes),vector(packetBytes); interpolationmode=none);

    gates:
        input in[numGates];
        output out[numGates];
}
