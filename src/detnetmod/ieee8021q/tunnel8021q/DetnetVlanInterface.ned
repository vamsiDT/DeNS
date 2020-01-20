//
// Copyright (C) OpenSim Ltd.
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program; if not, see <http://www.gnu.org/licenses/>.
//

package detnetmod.detnetmod.ieee8021q.tunnel8021q;

import inet.linklayer.contract.IVlanInterface;
import inet.networklayer.common.InterfaceEntry;
@namespace();

module DetnetVlanInterface extends InterfaceEntry like IVlanInterface
{
    parameters:
        string interfaceTableModule;
        *.interfaceTableModule = default(absPath(interfaceTableModule));
        @display("i=block/tunnel");
        @class(inet::InterfaceEntry);
    gates:
        input upperLayerIn;
        output upperLayerOut;
    submodules:
        tunnel: tunnel8021q {
            @display("p=100,100");
        }
    connections:
        upperLayerIn --> tunnel.upperLayerIn;
        upperLayerOut <-- tunnel.upperLayerOut;
}
