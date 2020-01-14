#ifndef KEYBOARDS
#define KEYBOARDS

#include <pluginterfaces/base/ftypes.h>

#include "cvmodules.h"

#include <cmath>
#include <vector>

namespace Steinberg {
namespace Synth {

const float SEMI_TONE_MULTIPLIER = pow(2.0, 1.0 / 12.0);

//-----------------------------------------------------------------------------
/** A base class for modules converting MIDI events to CV */
//-----------------------------------------------------------------------------
class Keyboard
{
public:
    virtual void keyOn(int16* pitch)=0;
    virtual void keyOff(int16* pitch)=0;
};

//-----------------------------------------------------------------------------
/** A simple monophonic keyboard */
//-----------------------------------------------------------------------------
class DumbMonoKeyboard : public Keyboard
{
protected:
    std::vector<Oscillator*> pitchReceivers;
    std::vector<Triggerable*> gateReceivers;

    void setPitch(int16* pitch);
    void triggerOn();
    void triggerOff();
public:
    virtual void keyOn(int16* pitch);
    virtual void keyOff(int16* pitch);

    void addPitchReceiver(Oscillator* osc);
    void addGateReceiver(Triggerable* gateReceiver);

    void clear();
};

//-----------------------------------------------------------------------------
float pitchToCV(int16* pitch);

//-----------------------------------------------------------------------------
/** A simple monophonic keyboard that prioritizes the highest pressed key */
//-----------------------------------------------------------------------------
class HighestDumbMonoKeyboard : public DumbMonoKeyboard
{
    int16 currentPitch;
public:
    HighestDumbMonoKeyboard();
    virtual void keyOn(int16* pitch);
    virtual void keyOff(int16* pitch);
};

//-----------------------------------------------------------------------------
/** A monophonic keyoard that prioritizes the last pressed key,
    this keyboard tracks all pressed keys.
*/
//-----------------------------------------------------------------------------
class LastMonoKeyboard : public DumbMonoKeyboard
{
    std::vector<int16> pressedKeys;
public:
    virtual void keyOn(int16* pitch);
    virtual void keyOff(int16* pitch);
};

} //namespace Synth
} //namespace Steinberg

#endif