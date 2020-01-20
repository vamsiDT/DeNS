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

package detnetmod.detnetmod.networklayer.diffserv;

simple PFPC
{
    parameters:
        @class(TokenBucketClass);
        string gateController = default("^.^.^.eth[0].queuing.gateController");
        string tgate = default("^.^.^.eth[0].queuing.tGates[7]");
        int NumClasses = default(0);
        int vqThreshold = default(10*412*8); // 5 Mtu
        double OutputLinkBandwidth = default(10); // 1Gbps
        double ClassPerflowReservation0 = default(0.001);
        double ClassPerflowReservation1 = default(0.001);
        double ClassPerflowReservation2 = default(0.001);
        double ClassPerflowReservation3 = default(0.001);
        double ClassPerflowReservation4 = default(0.001);
        double ClassPerflowReservation5 = default(0.001);
        double ClassPerflowReservation6 = default(0.001);
        double ClassPerflowReservation7 = default(0.001);
        double ClassPerflowReservation8 = default(0.001);
        double ClassPerflowReservation9 = default(0.001);
        double ClassPerflowReservation10 = default(0.001);
        double ClassPerflowReservation11 = default(0.001);
        double ClassPerflowReservation12 = default(0.001);
        double ClassPerflowReservation13 = default(0.001);
        double ClassPerflowReservation14 = default(0.001);
        double ClassPerflowReservation15 = default(0.001);
        double ClassPerflowReservation16 = default(0.001);
        double ClassPerflowReservation17 = default(0.001);
        double ClassPerflowReservation18 = default(0.001);
        double ClassPerflowReservation19 = default(0.001);
        double ClassPerflowReservation20 = default(0.001);
        double ClassPerflowReservation21 = default(0.001);
        double ClassPerflowReservation22 = default(0.001);
        double ClassPerflowReservation23 = default(0.001);
        double ClassPerflowReservation24 = default(0.001);
        double ClassPerflowReservation25 = default(0.001);
        double ClassPerflowReservation26 = default(0.001);
        double ClassPerflowReservation27 = default(0.001);
        double ClassPerflowReservation28 = default(0.001);
        double ClassPerflowReservation29 = default(0.001);
        double ClassPerflowReservation30 = default(0.001);
        double ClassPerflowReservation31 = default(0.001);
        double ClassPerflowReservation32 = default(0.001);
        double ClassPerflowReservation33 = default(0.001);
        double ClassPerflowReservation34 = default(0.001);
        double ClassPerflowReservation35 = default(0.001);
        double ClassPerflowReservation36 = default(0.001);
        double ClassPerflowReservation37 = default(0.001);
        double ClassPerflowReservation38 = default(0.001);
        double ClassPerflowReservation39 = default(0.001);
        double ClassPerflowReservation40 = default(0.001);
        double ClassPerflowReservation41 = default(0.001);
        double ClassPerflowReservation42 = default(0.001);
        double ClassPerflowReservation43 = default(0.001);
        double ClassPerflowReservation44 = default(0.001);
        double ClassPerflowReservation45 = default(0.001);
        double ClassPerflowReservation46 = default(0.001);
        double ClassPerflowReservation47 = default(0.001);
        double ClassPerflowReservation48 = default(0.001);
        double ClassPerflowReservation49 = default(0.001);
        double ClassPerflowReservation50 = default(0.001);
        double ClassPerflowReservation51 = default(0.001);
        double ClassPerflowReservation52 = default(0.001);
        double ClassPerflowReservation53 = default(0.001);
        double ClassPerflowReservation54 = default(0.001);
        double ClassPerflowReservation55 = default(0.001);
        double ClassPerflowReservation56 = default(0.001);
        double ClassPerflowReservation57 = default(0.001);
        double ClassPerflowReservation58 = default(0.001);
        double ClassPerflowReservation59 = default(0.001);
        double ClassPerflowReservation60 = default(0.001);
        double ClassPerflowReservation61 = default(0.001);
        double ClassPerflowReservation62 = default(0.001);
        double ClassPerflowReservation63 = default(0.001);
        @signal[arrival](type="long");
        @statistic[numActiveflows](title="hop count"; source="arrival"; record=vector,stats; interpolationmode=none);
        @display("i=block/subqueue");
    gates:
        input in[NumClasses+1];
        output out[NumClasses+1];
}
