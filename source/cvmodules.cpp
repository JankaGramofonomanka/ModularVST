#include "../include/cvmodules.h"

#include <cmath>

namespace Steinberg {
namespace Synth {

NullModule NULL_MODULE;

//-----------------------------------------------------------------------------
Camertone::Camertone() : period(2 * M_PI) {
    increment = 0;
    phase = 0; 
}

void Camertone::setSampleRate(Vst::SampleRate* _sampleRate) {
    CVModule::setSampleRate(_sampleRate);
    setIncrement();
}

void Camertone::setIncrement() {
    increment = period * 440 / sampleRate;
}

float Camertone::output() {
    phase = std::fmod(phase + increment, period);
    return sin(phase);
}

//-----------------------------------------------------------------------------
float NullModule::output() {
    return 0;
}
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
Oscillator::Oscillator() : period(2 * M_PI) {
    increment = 0;
    phase = 0;
    baseFreq = 440;
    keyMod = 1;
}

void Oscillator::setSampleRate(Vst::SampleRate* _sampleRate) {
    CVModule::setSampleRate(_sampleRate);
    setIncrement();
}

void Oscillator::setFrequency(float* freq) {
    baseFreq = *freq;
    setIncrement();
}

void Oscillator::setKeyMod(float mod) {
    keyMod = mod;
    setIncrement();
}

void Oscillator::setIncrement() {
    increment = period * keyMod * baseFreq / sampleRate;
}

float Oscillator::output() {
    phase = std::fmod(phase + increment, period);
    return sin(phase);
}

//-----------------------------------------------------------------------------
OneInputOneOutputModule::OneInputOneOutputModule() {
    input = (CVModule*) &NULL_MODULE;
}

float OneInputOneOutputModule::output() { return input->output(); }

void OneInputOneOutputModule::setInput(CVModule* _input) { input = _input; }

//-----------------------------------------------------------------------------
Amplifier::Amplifier() {
    volume = 1;
}

float Amplifier::output() {
    if (isOn()) {
        return volume * input->output();
    }
    return 0;
}

void Amplifier::setVolume(Vst::ParamValue* _volume) { volume = *_volume; }

bool Amplifier::isOn() { return volume > 0; }

//-----------------------------------------------------------------------------
ModOnlyAmp::ModOnlyAmp() {
    modulator = (CVModule*) &NULL_MODULE;
}

bool ModOnlyAmp::isOn() {
    return modulator->isOn() || modulator == &NULL_MODULE;
}

float ModOnlyAmp::output() {
    if (isOn()) {
        return modulator->output() * input->output();    
    }
    return 0;
}

void ModOnlyAmp::setModulator(CVModule* mod) {
    modulator = mod;
}

//-----------------------------------------------------------------------------
bool ModAmp::isOn() {
    return volume > 0 && (modulator->isOn() || modulator == &NULL_MODULE);
}

float ModAmp::output() {
    if (isOn()) {
        float modOut = modulator->output();
        return volume * modOut * input->output();    
    }
    return 0;
}

//-----------------------------------------------------------------------------
float Gate::output() { return on; }

bool Gate::isOn() { return on; }

void Gate::press() { on = true; }

void Gate::release() { on = false; }

//-----------------------------------------------------------------------------
SmoothGate::SmoothGate() {
    value = 0;
    phase = 0;
    increment = 0.005;
}

bool SmoothGate::isOn() { return phase != 0; }

void SmoothGate::press() { 
    phase = 1; 
}

void SmoothGate::release() { phase = 3; }

float SmoothGate::output() {
    switch (phase) 
    {
    case 0:
        break;
    case 1:
        if (value >= 1) {
            phase = 2;
            value = 1;
            break;
        }
        value += increment;
        break;
    case 2:
        return 1;
        break;
    case 3:
        if (value <= 0) {
            phase = 0;
            value = 0;
            break;
        }
        value -= increment;
        break;
    }
    return value;
}

//-----------------------------------------------------------------------------
Mixer::Mixer() {
    numInputs = 0;
}

float Mixer::output() {
    float output = 0;

    for (int i = 0; i < numInputs; i++) {
        output += inputs[i]->output();
    }
    output *= gain;

    return output;
}

void Mixer::addInput(CVModule* input) {
    inputs.push_back(input);
    numInputs ++;
    gain = 1.0 / numInputs; 
}

void Mixer::clear() { inputs.clear(); }

//-----------------------------------------------------------------------------
FMOsc::FMOsc() {
    modulator = (CVModule*) &NULL_MODULE;
}

float FMOsc::output() {
    phase = std::fmod(phase + increment, period);
    return sin(phase + modulator->output());
}

void FMOsc::setModulator(CVModule* mod) {
    modulator = mod;
}

//-----------------------------------------------------------------------------
FMOperator::FMOperator() {
    osc.setModulator(&mixer);
    amp.setInput(&osc);
    amp.setModulator(&envelope);
}

void FMOperator::setSampleRate(Vst::SampleRate* _sampleRate) {
    CVModule::setSampleRate(_sampleRate);
    osc.setSampleRate(_sampleRate);
    mixer.setSampleRate(_sampleRate);
    amp.setSampleRate(_sampleRate);
    envelope.setSampleRate(_sampleRate);
}

void FMOperator::setFrequency(float* freq) { osc.setFrequency(freq); }

void FMOperator::setKeyMod(float mod) { osc.setKeyMod(mod); }

//void FMOperator::setIncrement() { osc.setIncrement(); }

void FMOperator::addModulator(CVModule* mod) { mixer.addInput(mod); }

void FMOperator::setVolume(Vst::ParamValue* volume) { amp.setVolume(volume); }

Triggerable* FMOperator::getEnvelopeAddress() { return &envelope; }

float FMOperator::output() { return amp.output(); }

void FMOperator::clear() { mixer.clear(); }

} //namespace Synth
} //namespace Steinberg