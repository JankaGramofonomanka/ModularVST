#ifndef CV_MODULES
#define CVMODULES

#define _USE_MATH_DEFINES

#include <pluginterfaces/vst/vsttypes.h>

#include <cmath>
#include <vector>

namespace Steinberg {
namespace Synth {

//-----------------------------------------------------------------------------
/** A base class for all modules 
    (oscillators, filters, envelope generators, amplifiers etc.) */
//-----------------------------------------------------------------------------
class CVModule
{
protected:
    Vst::SampleRate sampleRate;

public:
    virtual void setSampleRate(Vst::SampleRate* _sampleRate) { sampleRate = *_sampleRate; }
    virtual float output()=0;

    virtual bool isOn() { return true; }        //this is for optimization
    virtual void clear() { return; }            //modules that have lists of inputs have to be able to clear them
};

//-----------------------------------------------------------------------------
/** A class to instantiate a module to use as a modulator when no real 
    modulator is connected,
    this will prevent checking if a modulator is conneted. */
//-----------------------------------------------------------------------------
class NullModule : public CVModule
{
public:
    virtual float output();
};

//-----------------------------------------------------------------------------
/** A simple oscillator with a constant frequency, no modulation */
//-----------------------------------------------------------------------------
class Camertone : public CVModule
{
    const float period;
    float increment;
    float phase;
protected:
    void setIncrement();
public:
    Camertone();
    virtual void setSampleRate(Vst::SampleRate* _sampleRate);
    virtual float output();
};

//-----------------------------------------------------------------------------
/** An oscillator with an input intended for modulation by a MIDI device 
    (see Synth::Keyboard in keyboards.h)
  * A base class for all oscillators and audio generating modules controlled
    by a MIDI device */
//-----------------------------------------------------------------------------
class Oscillator : public CVModule
{
    float baseFreq;
    float keyMod;
protected:
    const float period;
    float increment;
    float phase;
    void setIncrement();
public:
    Oscillator();
    virtual void setSampleRate(Vst::SampleRate* _sampleRate);
    virtual float output();

    virtual void setFrequency(Vst::ParamValue* freq);
    virtual void setKeyMod(float mod);
};

//-----------------------------------------------------------------------------
/** A class for other objects with one input and one output to inherit from */
//-----------------------------------------------------------------------------
class OneInputOneOutputModule : public CVModule
{
protected:
    CVModule* input;

public:
    OneInputOneOutputModule();
    virtual float output();

    virtual void setInput(CVModule* _input);

    virtual bool isOn();
};

//-----------------------------------------------------------------------------
/** A simple amplifier with a volume knob, no modulation */
//-----------------------------------------------------------------------------
class Amplifier : virtual public OneInputOneOutputModule
{
protected:
    float volume;

public:
    Amplifier();
    virtual float output();

    void setVolume(Vst::ParamValue* _volume);

    virtual bool isOn();
};

//-----------------------------------------------------------------------------
/** An amplifier without a gain knob, with a modulation input */
//-----------------------------------------------------------------------------
class ModOnlyAmp : virtual public OneInputOneOutputModule
{
protected:
    CVModule* modulator;

public:
    ModOnlyAmp();
    virtual float output();

    void setModulator(CVModule* mod);

    virtual bool isOn();
};

//-----------------------------------------------------------------------------
/** An amplifier with a gain knob and a modulation input */
//-----------------------------------------------------------------------------
class ModAmp : public Amplifier, public ModOnlyAmp
{
public:
    virtual float output();
    
    virtual bool isOn();
};

//-----------------------------------------------------------------------------
/** A base class for triggerable modules
    (envelope generator, trigger delays, etc.) */
//-----------------------------------------------------------------------------
class Triggerable : public CVModule
{
public:
    virtual void press()=0;
    virtual void release()=0;
};

//-----------------------------------------------------------------------------
/** A simple gate generator */
//-----------------------------------------------------------------------------
class Gate : public Triggerable
{
    bool on;
public:
    Gate() { on = false; }
    virtual float output();

    virtual bool isOn();

    virtual void press();
    virtual void release();
};

//-----------------------------------------------------------------------------
/** A gate generator with smooth attack and decay to avoid clicking */
//-----------------------------------------------------------------------------
class SmoothGate : public Triggerable
{
    float value;
    int phase;
    float increment;
public:
    SmoothGate();
    virtual void setSampleRate(Vst::SampleRate* _sampleRate);
    virtual float output();

    virtual bool isOn();

    virtual void press();
    virtual void release();
};

//-----------------------------------------------------------------------------
/** A simple ADSR envelope generator */
//-----------------------------------------------------------------------------
class LinearADSR : public Triggerable
{
    float value;
    int phase;
    
    //parameters
    float attackLevel;
    float decayLevel;
    float sustainLevel;
    float releaseLevel;

    //increments
    float attackIncrement;
    float decayIncrement;
    float releaseIncrement;

    virtual void setAttackIncrement();
    virtual void setDecayIncrement();
    virtual void setReleaseIncrement();

public:
    LinearADSR();
    virtual void setSampleRate(Vst::SampleRate* _sampleRate);
    virtual float output();
    
    virtual bool isOn();

    virtual void press();
    virtual void release();

    virtual void setAttack(Vst::ParamValue* _value);
    virtual void setDecay(Vst::ParamValue* _value);
    virtual void setSustain(Vst::ParamValue* _value);
    virtual void setRelease(Vst::ParamValue* _value);
};

//-----------------------------------------------------------------------------
/** A simple moxer*/
//-----------------------------------------------------------------------------
class Mixer : public CVModule
{
    float gain;
protected:
    int numInputs;
    std::vector<CVModule*> inputs;
public:
    Mixer();
    virtual float output();
    
    virtual void clear();

    void addInput(CVModule* input);
};

//-----------------------------------------------------------------------------
/** A simple oscillator with FM modulation */
//-----------------------------------------------------------------------------
class FMOsc : public Oscillator
{
    CVModule* modulator;
public:
    FMOsc();
    virtual float output();

    void setModulator(CVModule* mod);
};

//-----------------------------------------------------------------------------
/** An FM Operator with an envelope and multiple modulation inputs */
//-----------------------------------------------------------------------------
class FMOperator : public Oscillator
{
    FMOsc osc;
    Mixer mixer;
    ModAmp amp;
    LinearADSR envelope;
public:
    FMOperator();
    virtual void setSampleRate(Vst::SampleRate* _sampleRate);
    virtual float output();
    
    virtual void clear();

    virtual void setFrequency(Vst::ParamValue* freq);
    virtual void setKeyMod(float mod);

    void addModulator(CVModule* mod);
    void setVolume(Vst::ParamValue* volume);
    void setAttack(Vst::ParamValue* _value);
    void setDecay(Vst::ParamValue* _value);
    void setSustain(Vst::ParamValue* _value);
    void setRelease(Vst::ParamValue* _value);
    LinearADSR* getEnvelopeAddress();
};

} //namespace Synth
} //namespace Steinberg

#endif