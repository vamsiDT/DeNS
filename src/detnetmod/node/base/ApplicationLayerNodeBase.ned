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
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program; if not, see <http://www.gnu.org/licenses/>.
//

package detnetmod.detnetmod.node.base;

import detnetmod.detnetmod.applications.SetKind;
//import detnetmod.detnetmod.applications.IApplication;
import inet.applications.contract.IApp;

import inet.common.MessageDispatcher;
import inet.applications.udpapp.*;

@namespace();
//module ApplicationLayerNodeBase extends TransportLayerNodeBase
module ApplicationLayerNodeBase extends NetPerfDetnet
{
    parameters:
        int numApps = default(0);
        @figure[applicationLayer](type=rectangle; pos=250,6; size=1000,130; lineColor=#808080; cornerRadius=5; fillColor=#ffff00; fillOpacity=0.1);
        @figure[applicationLayer.title](type=text; pos=1245,11; anchor=ne; text="application layer");
		bool setKindApp = default(true);
        @display("bgb=1293.075,806.39996");
    submodules:
        app[numApps]: <> like IApp {
            parameters:
                @display("p=621.4563,40.56125,row,150");
        }
        at: MessageDispatcher if numApps!=0 {
            parameters:
                @display("p=750,146;b=1000,5,,,,1");
        }

        setFlowIdApp[numApps]: SetKind if numApps!=0 && setKindApp {
            @display("p=950.05255,40.56125;i=block/star");
        }
    connections allowunconnected:
        for i=0..numApps-1 {
            app[i].socketOut --> setFlowIdApp[i].UpperLayerIn if setKindApp;
            app[i].socketOut --> at.in++ if !setKindApp;
            setFlowIdApp[i].LowerLayerOut --> at.in++ if setKindApp;
            //            app[i].socketOut --> at.in++;
            app[i].socketIn <-- at.out++;
        }

        at.out++ --> udp.appIn if hasUdp && exists(at);
        at.in++ <-- udp.appOut if hasUdp && exists(at);

        at.out++ --> tcp.appIn if hasTcp && exists(at);
        at.in++ <-- tcp.appOut if hasTcp && exists(at);

        at.out++ --> sctp.appIn if hasSctp && exists(at);
        at.in++ <-- sctp.appOut if hasSctp && exists(at);

        at.out++ --> tn.in++ if exists(at);
        at.in++ <-- tn.out++ if exists(at);
}

