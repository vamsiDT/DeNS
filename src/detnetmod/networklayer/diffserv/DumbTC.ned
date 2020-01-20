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
module DumbTC like ITrafficConditioner
{
    parameters:
        @display("i=block/classifier");
    gates:
        input in;
        output out;
    submodules:
        dummyTC: DummyTC {
            @display("p=334,53");
        }
    connections:
        in --> dummyTC.in;
        dummyTC.out --> out;
}
