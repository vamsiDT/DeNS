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
import inet.linklayer.contract.ITrafficConditioner;
import inet.linklayer.ethernet.EtherEncap;
import inet.networklayer.diffserv.BehaviorAggregateClassifier;


//
// TODO documentation
//
module detnetDiffserv like ITrafficConditioner
{
    parameters:
        int numClasses;
        string interfaceTableModule;
        @display("i=block/classifier");
        *.interfaceTableModule = default(absPath(interfaceTableModule));
        string fcsMode @enum("declared","computed") = default("declared");
        *.fcsMode = fcsMode;
    gates:
        input in;
        output out;
    submodules:
        DscpClassifier: BehaviorAggregateClassifier {
            @display("p=110,138");
        }
        pfpc: PFPC {
            parameters:
                NumClasses = numClasses;
                @display("p=291,138");
        }
        pcpMarker: PcpMarker {
            parameters:
                NumGates = numClasses+1;
                @display("p=453,138");
        }
    connections:
        for i=1..numClasses {
            DscpClassifier.outs++ --> pfpc.in++;
            pfpc.out++ --> pcpMarker.in++;
        }
        DscpClassifier.defaultOut --> pfpc.in++;
        pfpc.out++ --> pcpMarker.in++;
        pcpMarker.out --> out;

        in --> DscpClassifier.in;
}

