//------------------------------------------------------------------------
// Project     : VST SDK
//
// Category    : Examples
// Filename    : plugprocessor.cpp
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

#include "../include/plugprocessor.h"
#include "../include/plugids.h"

#include "base/source/fstreamer.h"
#include "pluginterfaces/base/ibstream.h"
#include "pluginterfaces/vst/ivstparameterchanges.h"

#include "pluginterfaces/vst/ivstevents.h"

#include <cmath>

namespace Steinberg {
namespace Synth {

//-----------------------------------------------------------------------------
PlugProcessor::PlugProcessor ()
{
	// register its editor class
	setControllerClass (MyControllerUID);
}

//-----------------------------------------------------------------------------
tresult PLUGIN_API PlugProcessor::initialize (FUnknown* context)
{
	//---always initialize the parent-------
	tresult result = AudioEffect::initialize (context);
	if (result != kResultTrue)
		return kResultFalse;

	//---create Audio In/Out buses------
	// we want a MIDI Input and a Stereo Output
	addEventInput (STR16 ("AudioInput"));
	addAudioOutput (STR16 ("AudioOutput"), Vst::SpeakerArr::kStereo);

	return kResultTrue;
}

//-----------------------------------------------------------------------------
tresult PLUGIN_API PlugProcessor::setBusArrangements (Vst::SpeakerArrangement* inputs,
                                                            int32 numIns,
                                                            Vst::SpeakerArrangement* outputs,
                                                            int32 numOuts)
{
	// we only support one output bus
	if (numIns == 0 && numOuts == 1 && outputs[0] == Vst::SpeakerArr::kStereo)
	{
		return AudioEffect::setBusArrangements (inputs, numIns, outputs, numOuts);
	}
	return kResultFalse;
}

//-----------------------------------------------------------------------------
tresult PLUGIN_API PlugProcessor::setupProcessing (Vst::ProcessSetup& setup)
{
	// here you get, with setup, information about:
	// sampleRate, processMode, maximum number of samples per audio block
	sampleRate = setup.sampleRate;
	op1.setSampleRate(&sampleRate);
	op2.setSampleRate(&sampleRate);
	//gate.setSampleRate(&sampleRate);
	mixer.setSampleRate(&sampleRate);
	amp.setSampleRate(&sampleRate);

	return AudioEffect::setupProcessing (setup);
}

//-----------------------------------------------------------------------------
tresult PLUGIN_API PlugProcessor::setActive (TBool state)
{
	if (state) // Initialize
	{
		// Allocate Memory Here
		// Ex: algo.create ();

		keyboard.addPitchReceiver(&op1);
		keyboard.addPitchReceiver(&op2);
		//keyboard.addGateReceiver(&gate);
		keyboard.addGateReceiver(op1.getEnvelopeAddress());
		keyboard.addGateReceiver(op2.getEnvelopeAddress());

		//mixer.addInput(&op1);
		mixer.addInput(&op2);

		amp.setInput(&mixer);
		//amp.setModulator(&gate);

		
		op2.addModulator(&op1);
	}
	else // Release
	{
		op1.clear();
		op2.clear();
		mixer.clear();
		//gate.clear();
		amp.clear();
		keyboard.clear();
		
		// Free Memory if still allocated
		// Ex: if(algo.isCreated ()) { algo.destroy (); }
	}
	return AudioEffect::setActive (state);
}

//-----------------------------------------------------------------------------
void PlugProcessor::readParameterChanges(Vst::IParameterChanges* inputParameterChanges)
{
	if (inputParameterChanges)
	{
		int32 numParamsChanged = inputParameterChanges->getParameterCount ();
		for (int32 index = 0; index < numParamsChanged; index++)
		{
			Vst::IParamValueQueue* paramQueue =
			    inputParameterChanges->getParameterData (index);
			if (paramQueue)
			{
				Vst::ParamValue value;
				int32 sampleOffset;
				int32 numPoints = paramQueue->getPointCount ();
				switch (paramQueue->getParameterId ())
				{
					case SynthParams::kParamOp1_levelId:
						if (paramQueue->getPoint (numPoints - 1, sampleOffset, value) ==
						    kResultTrue)
							value *= (2 * M_PI);
							op1.setVolume(&value);
						break;
					case SynthParams::kParamOp1_frequencyId:
						if (paramQueue->getPoint (numPoints - 1, sampleOffset, value) ==
						    kResultTrue);
							value *= 880;
							op1.setFrequency(&value);
						break;
					case SynthParams::kParamOp1_attackId:
						if (paramQueue->getPoint (numPoints - 1, sampleOffset, value) ==
						    kResultTrue)
							value += 0.005;
							op1.setAttack(&value);
						break;
					case SynthParams::kParamOp1_decayId:
						if (paramQueue->getPoint (numPoints - 1, sampleOffset, value) ==
						    kResultTrue)
							op1.setDecay(&value);
						break;
					case SynthParams::kParamOp1_sustainId:
						if (paramQueue->getPoint (numPoints - 1, sampleOffset, value) ==
						    kResultTrue)
							op1.setSustain(&value);
						break;
					case SynthParams::kParamOp1_releaseId:
						if (paramQueue->getPoint (numPoints - 1, sampleOffset, value) ==
						    kResultTrue)
							value += 0.005;
							op1.setRelease(&value);
						break;

					case SynthParams::kParamOp2_levelId:
						if (paramQueue->getPoint (numPoints - 1, sampleOffset, value) ==
						    kResultTrue)
							value *= (2 * M_PI);
							op2.setVolume(&value);
						break;
					case SynthParams::kParamOp2_frequencyId:
						if (paramQueue->getPoint (numPoints - 1, sampleOffset, value) ==
						    kResultTrue);
							value *= 880;
							op2.setFrequency(&value);
						break;
					case SynthParams::kParamOp2_attackId:
						if (paramQueue->getPoint (numPoints - 1, sampleOffset, value) ==
						    kResultTrue)
							value += 0.005;
							op2.setAttack(&value);
						break;
					case SynthParams::kParamOp2_decayId:
						if (paramQueue->getPoint (numPoints - 1, sampleOffset, value) ==
						    kResultTrue)
							op2.setDecay(&value);
						break;
					case SynthParams::kParamOp2_sustainId:
						if (paramQueue->getPoint (numPoints - 1, sampleOffset, value) ==
						    kResultTrue)
							op2.setSustain(&value);
						break;
					case SynthParams::kParamOp2_releaseId:
						if (paramQueue->getPoint (numPoints - 1, sampleOffset, value) ==
						    kResultTrue)
							value += 0.005;
							op2.setRelease(&value);
						break;

					case SynthParams::kParamMasterVolumeId:
						if (paramQueue->getPoint (numPoints - 1, sampleOffset, value) ==
						    kResultTrue)
							amp.setVolume(&value);
						break;
				}
			}
		}
	}
}

//-----------------------------------------------------------------------------
void PlugProcessor::processEvents(Vst::IEventList* inputEvents)
{
	if (inputEvents)
	{
		for (int32 i = 0; i < inputEvents->getEventCount(); i++)
		{
			Vst::Event event;
			if (inputEvents->getEvent(i, event) == kResultTrue)
			{
				if (event.type == Vst::Event::kNoteOnEvent)
				{
					keyboard.keyOn(&event.noteOn.pitch);
				}
				else if (event.type == Vst::Event::kNoteOffEvent)
				{
					keyboard.keyOff(&event.noteOff.pitch);
				}
			}
		}
	}
}

//-----------------------------------------------------------------------------
void PlugProcessor::processAudio(Vst::AudioBusBuffers* outputs, int32 numSamples)
{
	for (int32 i = 0; i < numSamples; i++)
	{
		for (int32 j = 0; j < outputs[0].numChannels; j++)
		{
			outputs[0].channelBuffers32[j][i] = 0;
		}
	}
	
	for (int32 i = 0; i < numSamples; i++)
	{
		float value = amp.output();
		//float value = 0;
		for (int32 j = 0; j < outputs[0].numChannels; j++)
		{
			outputs[0].channelBuffers32[j][i] = value;
		}
	}
}

//-----------------------------------------------------------------------------
tresult PLUGIN_API PlugProcessor::process (Vst::ProcessData& data)
{
	//--- Read inputs parameter changes-----------
	readParameterChanges(data.inputParameterChanges);

	processEvents(data.inputEvents);
	//--- Process Audio---------------------
	//--- ----------------------------------
	if (data.numOutputs == 0)
	{
		// nothing to do
		return kResultOk;
	}

	if (data.numSamples > 0)
	{
		// Process Algorithm
		// Ex: algo.process (data.inputs[0].channelBuffers32, data.outputs[0].channelBuffers32,
		// data.numSamples);
		processAudio(data.outputs, data.numSamples);
	}
	return kResultOk;
}

//------------------------------------------------------------------------
tresult PLUGIN_API PlugProcessor::setState (IBStream* state)
{
	if (!state)
		return kResultFalse;

	// called when we load a preset or project, the model has to be reloaded

	IBStreamer streamer (state, kLittleEndian);

	//operator 1
	float op1_level = 0.f;
	if (streamer.readFloat (op1_level) == false)
		return kResultFalse;
	float op1_frequency = 0.f;
	if (streamer.readFloat (op1_frequency) == false)
		return kResultFalse;
	float op1_attack = 0.f;
	if (streamer.readFloat (op1_attack) == false)
		return kResultFalse;
	float op1_decay = 0.f;
	if (streamer.readFloat (op1_decay) == false)
		return kResultFalse;
	float op1_sustain = 0.f;
	if (streamer.readFloat (op1_sustain) == false)
		return kResultFalse;
	float op1_release = 0.f;
	if (streamer.readFloat (op1_release) == false)
		return kResultFalse;

	//operator 2	
	float op2_level = 0.f;
	if (streamer.readFloat (op1_level) == false)
		return kResultFalse;
	float op2_frequency = 0.f;
	if (streamer.readFloat (op1_frequency) == false)
		return kResultFalse;
	float op2_attack = 0.f;
	if (streamer.readFloat (op1_attack) == false)
		return kResultFalse;
	float op2_decay = 0.f;
	if (streamer.readFloat (op1_decay) == false)
		return kResultFalse;
	float op2_sustain = 0.f;
	if (streamer.readFloat (op1_sustain) == false)
		return kResultFalse;
	float op2_release = 0.f;
	if (streamer.readFloat (op1_release) == false)
		return kResultFalse;
	
	//volume
	float masterVolume = 0.f;
	if (streamer.readFloat (masterVolume) == false)
		return kResultFalse;

	
	//operator 1
	op1.setVolume((Vst::ParamValue*) &op1_level);
	op1.setFrequency((Vst::ParamValue*) &op1_frequency);
	op1.setAttack((Vst::ParamValue*) &op1_attack);
	op1.setDecay((Vst::ParamValue*) &op1_decay);
	op1.setSustain((Vst::ParamValue*) &op1_sustain);
	op1.setRelease((Vst::ParamValue*) &op1_release);
	
	//operator 2
	op2.setVolume((Vst::ParamValue*) &op2_level);
	op2.setFrequency((Vst::ParamValue*) &op2_frequency);
	op2.setAttack((Vst::ParamValue*) &op2_attack);
	op2.setDecay((Vst::ParamValue*) &op2_decay);
	op2.setSustain((Vst::ParamValue*) &op2_sustain);
	op2.setRelease((Vst::ParamValue*) &op2_release);

	//volume
	amp.setVolume((Vst::ParamValue*) &masterVolume);

	return kResultOk;
}

//------------------------------------------------------------------------
tresult PLUGIN_API PlugProcessor::getState (IBStream* state)
{
	// here we need to save the model (preset or project)

	//float toSaveParam1 = mParam1;

	IBStreamer streamer (state, kLittleEndian);
	//streamer.writeFloat (toSaveParam1);

	//operator 1
	float op1_level = 1;
	float op1_frequency = 440;
	float op1_attack = 0.005;
	float op1_decay = 0;
	float op1_sustain = 1;
	float op1_release = 0.005;

	//operator 2	
	float op2_level = 1;
	float op2_frequency = 440;
	float op2_attack = 0.005;
	float op2_decay = 0;
	float op2_sustain = 1;
	float op2_release = 0.005;
	
	//volume
	float masterVolume = 1;

	
	//operator 1
	streamer.writeFloat(op1_level);
	streamer.writeFloat(op1_frequency);
	streamer.writeFloat(op1_attack);
	streamer.writeFloat(op1_decay);
	streamer.writeFloat(op1_sustain);
	streamer.writeFloat(op1_release);
	
	//operator 2
	streamer.writeFloat(op2_level);
	streamer.writeFloat(op2_frequency);
	streamer.writeFloat(op2_attack);
	streamer.writeFloat(op2_decay);
	streamer.writeFloat(op2_sustain);
	streamer.writeFloat(op2_release);

	//volume
	streamer.writeFloat(masterVolume);

	return kResultOk;
}

//------------------------------------------------------------------------
} // namespace
} // namespace Steinberg
