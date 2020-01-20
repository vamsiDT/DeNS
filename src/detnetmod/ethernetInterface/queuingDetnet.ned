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

package detnetmod.detnetmod.ethernetInterface;

import detnetmod.detnetmod.dropper.Dropper;
import detnetmod.detnetmod.networklayer.diffserv.PcpMarker;
import detnetmod.detnetmod.queuing.QueueDetnet;
import detnetmod.detnetmod.queuing.PifoDetnet;
import detnetmod.detnetmod.queuing.DetnetQueuingFrames;
import inet.common.queue.DropTailQueue;
import inet.common.queue.RedDropper;
import nesting.ieee8021q.queue.QueuingFrames;
import nesting.ieee8021q.queue.TransmissionSelection;
import nesting.ieee8021q.queue.framePreemption.LengthAwareQueue;
import nesting.ieee8021q.queue.gating.GateController;
import nesting.ieee8021q.queue.gating.TransmissionGate;
import nesting.ieee8021q.queue.transmissionSelectionAlgorithms.TSAlgorithm;
import inet.common.queue.IOutputQueue;

//
// This module implements a queuing network providing scheduling of congested
// packets for a given output port according to the IEEE802.1Q standard,
// described in chapter 8.6.6 - 8.6.8.
//
// For every output port of an IEEE802.1Q conform switch an instance of this
// module is used the queue packets. 
//
// @see ~QueuingFrames, ~TransmissionGate, ~Schedule, ~TransmissionSelection
// @see ~TSAlgorithm, ~GateController, ~LengthAwareQueue
//
module queuingDetnet
{
    parameters:
        @display("i=block/queue;bgb=1526.58,1031.2125");
        int numberOfQueues = default(8);
        string defaultTSA = "StrictPriority"; // Default transmission-selection-algorithm implementation
        string Dropper = "MfdDropper";
        int enableIgates = default(0);
    gates:
        input in;
        output pOut;
        output eOut;
    submodules:
        queuingFrames: DetnetQueuingFrames {
            @display("p=286.525,93.075");
        }
        queues[numberOfQueues]: <default("QueueDetnet")> like IOutputQueue {
            @display("p=284.7,348.575,r,120;q=l2queue");
//            transmissionSelectionAlgorithmModule = "^.tsAlgorithms[" + string(index) + "]";
        }
        tsAlgorithms[numberOfQueues]: <default(defaultTSA)> like TSAlgorithm {
            @display("p=284.7,487.275,r,120");
            gateModule = "^.tGates[" + string(index) + "]";
            queueModule = "^.queues[" + string(index) + "]";
        }
        tGates[numberOfQueues]: TransmissionGate {
            @display("p=284.7,611.375,r,120");
            transmissionSelectionAlgorithmModule = "^.tsAlgorithms[" + string(index) + "]";
            transmissionSelectionModule = "^.transmissionSelection";
            gateControllerModule = "^.gateController";
        }
        transmissionSelection: TransmissionSelection {
            @display("p=284.7,753.725,r,120");
            transmissionGateVectorModule = "^.tGates[0]";
        }
        gateController: GateController {
            @display("p=94.35,37.739998;is=s");
        }
        dropper: <default(Dropper)> like Dropper if typename != "" {
            parameters:
                numGates = numberOfQueues;
                @display("p=284.7,222.65001");
        }
    //        queuingFrames1: QueuingFrames {
    //            @display("p=504.7725,29.877499");
    //        }
    connections:
        in --> queuingFrames.in;
        for i=0..numberOfQueues-1 {
            queuingFrames.out++ --> dropper.in[i] if exists(dropper);
            dropper.out[i] --> queues[i].in if exists(dropper);
            queuingFrames.out++ --> queues[i].in if !exists(dropper);
            queues[i].out --> tsAlgorithms[i].in;
            tsAlgorithms[i].out --> tGates[i].in;
            tGates[i].out --> transmissionSelection.in++;
        }
        transmissionSelection.eOut --> eOut;
        transmissionSelection.pOut --> pOut;


}
