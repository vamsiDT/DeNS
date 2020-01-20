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
import inet.linklayer.ethernet.EtherMacFullDuplex;
//import nesting.linklayer.framePreemption.EtherMACFullDuplexPreemptable;

package detnetmod.detnetmod.ethernetInterface;
@namespace();
//
// This module extends the INET EtherMacFullDuplex module and adds frame preemption capabilities.
//
simple EtherMACFullDuplexPreemptableMod extends EtherMacFullDuplex
{
    parameters:
        @class(nesting::EtherMACFullDuplexPreemptable);
        bool enablePreemptingFrames = default(false); // frame preemption off or on
        @signal[preemptCurrentFrameSignal](type=long); // type=unique packet id
        @signal[transmittedExpressFrameSignal](type=long); // type=unique packet id
        @signal[startTransmissionExpressFrameSignal](type=long); // type=unique packet id
        @signal[transmittedPreemptableFrameSignal](type=long); // type=unique packet id
        @signal[transmittedPreemptableFramePartSignal](type=long); // type=unique packet id
        @signal[transmittedPreemptableNonFinalSignal](type=long); // type=unique packet id
        @signal[transmittedPreemptableFinalSignal](type=long); // type=unique packet id
        @signal[transmittedPreemptableFullSignal](type=long); // type=unique packet id
        @signal[expressFrameEnqueuedWhileSendingPreemptableSignal];
        @signal[eMacDelay](type=simtime_t; unit=s);
        @signal[pMacDelay](type=simtime_t; unit=s);
        @signal[receivedExpressFrame](type=long); // type=unique packet id
        @signal[receivedPreemptableFrameFull](type=long); // type=unique packet id

		@statistic[txExpressFrame](title="txExpressFrame"; source=transmittedExpressFrameSignal ;record=vector; interpolationmode=none);
		@statistic[txPreemptableFrameFinal](title="txPreemptableFrameFinal"; source=transmittedPreemptableFinalSignal ;record=vector; interpolationmode=none);
		@statistic[txPreemptableFrameFull](title="txPreemptableFrameFull"; source=transmittedPreemptableFullSignal ;record=vector; interpolationmode=none);
		@statistic[receivedPreemptableFrameFull](title="receivedPreemptableFrameFull"; record=vector; interpolationmode=none);
		@statistic[receivedExpressFrame](title="receivedExpressFrame"; record=vector; interpolationmode=none);
		@statistic[startTxExpressFrames](title="startTxExpressFrames"; source=startTransmissionExpressFrameSignal; record=vector; interpolationmode=none);

        @statistic[eMacDelay](title="eMacDelay"; record=histogram,vector; interpolationmode=none);
        @statistic[pMacDelay](title="pMacDelay"; record=histogram,vector; interpolationmode=none);
        @statistic[preemptions](title="preemptions"; source=preemptCurrentFrameSignal; record=count; interpolationmode=none);
        @statistic[expressFrames](title="expressFrames"; source=transmittedExpressFrameSignal; record=count; interpolationmode=none);
        @statistic[preemptableFrames](title="preemptableFrames"; source=transmittedPreemptableFrameSignal; record=count; interpolationmode=none);
        @statistic[mPackets](title="mPackets"; source=transmittedPreemptableFramePartSignal; record=count; interpolationmode=none);
        @statistic[mPacketsNonFinal](title="mPackets non-final"; source=transmittedPreemptableNonFinalSignal; record=count; interpolationmode=none);
        @statistic[mPacketsFinal](title="mPackets final"; source=transmittedPreemptableFinalSignal; record=count; interpolationmode=none);
        @statistic[mPacketsFull](title="mpackets full"; source=transmittedPreemptableFullSignal; record=count; interpolationmode=none);
        @statistic[expressFramesEnqueued](title="express frames enqueued"; source=expressFrameEnqueuedWhileSendingPreemptableSignal; record=count; interpolationmode=none);

    gates:
        input upperLayerPreemptableIn @labels(EtherFrame);    // to ~EtherLLC or ~EtherEncap or ~IMACRelayUnit
//        output upperLayerPreemptableOut @labels(EtherFrame);    // to ~EtherLLC or ~EtherEncap or ~IMACRelayUnit
}

