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
simple PcpMarker
{
    parameters:
        @class(PcpMarker);
        int NumGates = default(1);
        int MarkPcp0 = default(7);
        int MarkPcp1 = default(1);
        int MarkPcp2 = default(1);
        int MarkPcp3 = default(1);
        int MarkPcp4 = default(1);
        int MarkPcp5 = default(1);
        int MarkPcp6 = default(1);
        int MarkPcp7 = default(1);
        int MarkPcp8 = default(1);
        int MarkPcp9 = default(1);
        int MarkPcp10 = default(1);
        int MarkPcp11 = default(1);
        int MarkPcp12 = default(1);
        int MarkPcp13 = default(1);
        int MarkPcp14 = default(1);
        int MarkPcp15 = default(1);
        int MarkPcp16 = default(1);
        int MarkPcp17 = default(1);
        int MarkPcp18 = default(1);
        int MarkPcp19 = default(1);
        int MarkPcp20 = default(1);
        int MarkPcp21 = default(1);
        int MarkPcp22 = default(1);
        int MarkPcp23 = default(1);
        int MarkPcp24 = default(1);
        int MarkPcp25 = default(1);
        int MarkPcp26 = default(1);
        int MarkPcp27 = default(1);
        int MarkPcp28 = default(1);
        int MarkPcp29 = default(1);
        int MarkPcp30 = default(1);
        int MarkPcp31 = default(1);
        int MarkPcp32 = default(1);
        int MarkPcp33 = default(1);
        int MarkPcp34 = default(1);
        int MarkPcp35 = default(1);
        int MarkPcp36 = default(1);
        int MarkPcp37 = default(1);
        int MarkPcp38 = default(1);
        int MarkPcp39 = default(1);
        int MarkPcp40 = default(1);
        int MarkPcp41 = default(1);
        int MarkPcp42 = default(1);
        int MarkPcp43 = default(1);
        int MarkPcp44 = default(1);
        int MarkPcp45 = default(1);
        int MarkPcp46 = default(1);
        int MarkPcp47 = default(1);
        int MarkPcp48 = default(1);
        int MarkPcp49 = default(1);
        int MarkPcp50 = default(1);
        int MarkPcp51 = default(1);
        int MarkPcp52 = default(1);
        int MarkPcp53 = default(1);
        int MarkPcp54 = default(1);
        int MarkPcp55 = default(1);
        int MarkPcp56 = default(1);
        int MarkPcp57 = default(1);
        int MarkPcp58 = default(1);
        int MarkPcp59 = default(1);
        int MarkPcp60 = default(1);
        int MarkPcp61 = default(1);
        int MarkPcp62 = default(1);
        int MarkPcp63 = default(1);
        @display("i=misc/marker2");
    gates:
        input in[NumGates];
        output out;
}
