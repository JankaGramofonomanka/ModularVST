//------------------------------------------------------------------------
// Project     : VST SDK
//
// Category    : Examples
// Filename    : plugcontroller.cpp
// Created by  : Steinberg, 01/2018
// Description : HelloWorld Example for VST 3
//
//-----------------------------------------------------------------------------
// LICENSE
// (c) 2019, Steinberg Media Technologies GmbH, All Rights Reserved
//-----------------------------------------------------------------------------
// Redistribution and use in source and binary forms, with or without modification,
// are permitted provided that the following conditions are met:
// 
//   * Redistributions of source code must retain the above copyright notice, 
//     this list of conditions and the following disclaimer.
//   * Redistributions in binary form must reproduce the above copyright notice,
//     this list of conditions and the following disclaimer in the documentation 
//     and/or other materials provided with the distribution.
//   * Neither the name of the Steinberg Media Technologies nor the names of its
//     contributors may be used to endorse or promote products derived from this 
//     software without specific prior written permission.
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED 
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. 
// IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, 
// INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, 
// BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, 
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
// LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE 
// OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
// OF THE POSSIBILITY OF SUCH DAMAGE.
//-----------------------------------------------------------------------------

#include "../include/plugcontroller.h"
#include "../include/plugids.h"

#include "base/source/fstreamer.h"
#include "pluginterfaces/base/ibstream.h"

namespace Steinberg {
namespace Synth {

//-----------------------------------------------------------------------------
tresult PLUGIN_API PlugController::initialize (FUnknown* context)
{
	tresult result = EditController::initialize (context);
	if (result == kResultTrue)
	{
		//---Create Parameters------------
		parameters.addParameter (STR16 ("Operator 1 level"), STR16 ("?"), 0, .5,
		                         Vst::ParameterInfo::kCanAutomate, SynthParams::kParamOp1_levelId, 0,
		                         STR16 ("Op1 level"));
		parameters.addParameter (STR16 ("Operator 1 frequency"), STR16 ("?"), 0, .5,
		                         Vst::ParameterInfo::kCanAutomate, SynthParams::kParamOp1_frequencyId, 0,
		                         STR16 ("Op1 frequency"));
		parameters.addParameter (STR16 ("Operator 1 attack"), STR16 ("seconds"), 0, .5,
		                         Vst::ParameterInfo::kCanAutomate, SynthParams::kParamOp1_attackId, 0,
		                         STR16 ("Op1 attack"));
		parameters.addParameter (STR16 ("Operator 1 decay"), STR16 ("seconds"), 0, .5,
		                         Vst::ParameterInfo::kCanAutomate, SynthParams::kParamOp1_decayId, 0,
		                         STR16 ("Op1 decay"));
		parameters.addParameter (STR16 ("Operator 1 sustain"), STR16 ("seconds"), 0, .5,
		                         Vst::ParameterInfo::kCanAutomate, SynthParams::kParamOp1_sustainId, 0,
		                         STR16 ("Op1 sustain"));
		parameters.addParameter (STR16 ("Operator 1 release"), STR16 ("seconds"), 0, .5,
		                         Vst::ParameterInfo::kCanAutomate, SynthParams::kParamOp1_releaseId, 0,
		                         STR16 ("Op1 release"));

		parameters.addParameter (STR16 ("Operator 2 level"), STR16 ("?"), 0, .5,
		                         Vst::ParameterInfo::kCanAutomate, SynthParams::kParamOp2_levelId, 0,
		                         STR16 ("Op2 level"));
		parameters.addParameter (STR16 ("Operator 2 frequency"), STR16 ("?"), 0, .5,
		                         Vst::ParameterInfo::kCanAutomate, SynthParams::kParamOp2_frequencyId, 0,
		                         STR16 ("Op2 frequency"));
		parameters.addParameter (STR16 ("Operator 2 attack"), STR16 ("seconds"), 0, .5,
		                         Vst::ParameterInfo::kCanAutomate, SynthParams::kParamOp2_attackId, 0,
		                         STR16 ("Op2 attack"));
		parameters.addParameter (STR16 ("Operator 2 decay"), STR16 ("seconds"), 0, .5,
		                         Vst::ParameterInfo::kCanAutomate, SynthParams::kParamOp2_decayId, 0,
		                         STR16 ("Op2 decay"));
		parameters.addParameter (STR16 ("Operator 2 sustain"), STR16 ("seconds"), 0, .5,
		                         Vst::ParameterInfo::kCanAutomate, SynthParams::kParamOp2_sustainId, 0,
		                         STR16 ("Op2 sustain"));
		parameters.addParameter (STR16 ("Operator 2 release"), STR16 ("seconds"), 0, .5,
		                         Vst::ParameterInfo::kCanAutomate, SynthParams::kParamOp2_releaseId, 0,
		                         STR16 ("Op2 release"));

		parameters.addParameter (STR16 ("Master Volume"), STR16 ("?"), 0, .5,
		                         Vst::ParameterInfo::kCanAutomate, SynthParams::kParamMasterVolumeId, 0,
		                         STR16 ("Volume"));

	}
	return kResultTrue;
}

//------------------------------------------------------------------------
tresult PLUGIN_API PlugController::setComponentState (IBStream* state)
{
	// we receive the current state of the component (processor part)
	// we read our parameters and bypass value...
	if (!state)
		return kResultFalse;

	IBStreamer streamer (state, kLittleEndian);

	//operator 1
	float op1_level = 0.f;
	if (streamer.readFloat (op1_level) == false)
		return kResultFalse;
	setParamNormalized (SynthParams::kParamOp1_levelId, op1_level);
	float op1_frequency = 0.f;
	if (streamer.readFloat (op1_frequency) == false)
		return kResultFalse;
	setParamNormalized (SynthParams::kParamOp1_frequencyId, op1_frequency);
	float op1_attack = 0.f;
	if (streamer.readFloat (op1_attack) == false)
		return kResultFalse;
	setParamNormalized (SynthParams::kParamOp1_attackId, op1_attack);
	float op1_decay = 0.f;
	if (streamer.readFloat (op1_decay) == false)
		return kResultFalse;
	setParamNormalized (SynthParams::kParamOp1_decayId, op1_decay);
	float op1_sustain = 0.f;
	if (streamer.readFloat (op1_sustain) == false)
		return kResultFalse;
	setParamNormalized (SynthParams::kParamOp1_sustainId, op1_sustain);
	float op1_release = 0.f;
	if (streamer.readFloat (op1_release) == false)
		return kResultFalse;
	setParamNormalized (SynthParams::kParamOp1_releaseId, op1_release);

	//operator 2
	float op2_level = 0.f;
	if (streamer.readFloat (op1_level) == false)
		return kResultFalse;
	setParamNormalized (SynthParams::kParamOp2_levelId, op2_level);
	float op2_frequency = 0.f;
	if (streamer.readFloat (op1_frequency) == false)
		return kResultFalse;
	setParamNormalized (SynthParams::kParamOp2_frequencyId, op2_frequency);
	float op2_attack = 0.f;
	if (streamer.readFloat (op1_attack) == false)
		return kResultFalse;
	setParamNormalized (SynthParams::kParamOp2_attackId, op2_attack);
	float op2_decay = 0.f;
	if (streamer.readFloat (op1_decay) == false)
		return kResultFalse;
	setParamNormalized (SynthParams::kParamOp2_decayId, op2_decay);
	float op2_sustain = 0.f;
	if (streamer.readFloat (op1_sustain) == false)
		return kResultFalse;
	setParamNormalized (SynthParams::kParamOp2_sustainId, op2_sustain);
	float op2_release = 0.f;
	if (streamer.readFloat (op1_release) == false)
		return kResultFalse;
	setParamNormalized (SynthParams::kParamOp2_releaseId, op2_release);

	//volume
	float masterVolume = 0.f;
	if (streamer.readFloat (masterVolume) == false)
		return kResultFalse;
	setParamNormalized (SynthParams::kParamMasterVolumeId, masterVolume);

	return kResultOk;
}

//------------------------------------------------------------------------
} // namespace
} // namespace Steinberg
