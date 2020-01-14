#include "../include/keyboards.h"

namespace Steinberg {
namespace Synth {


//-----------------------------------------------------------------------------
void DumbMonoKeyboard::setPitch(int16* pitch) {
    for (int i = 0; i < pitchReceivers.size(); i++) {
        pitchReceivers[i]->setKeyMod(pitchToCV(pitch));
    }
}

void DumbMonoKeyboard::triggerOn() {
    for (int i = 0; i < gateReceivers.size(); i++) {
        gateReceivers[i]->press();
    }
}

void DumbMonoKeyboard::triggerOff() {
    for (int i = 0; i < gateReceivers.size(); i++) {
        gateReceivers[i]->release();
    }
}

void DumbMonoKeyboard::keyOn(int16* pitch) {
    setPitch(pitch);
    triggerOn();
}

void DumbMonoKeyboard::keyOff(int16* pitch) {
    triggerOff();
}

void DumbMonoKeyboard::addPitchReceiver(Oscillator* osc) {
    pitchReceivers.push_back(osc);
}

void DumbMonoKeyboard::addGateReceiver(Triggerable* gateReceiver) {
    gateReceivers.push_back(gateReceiver);
}

void DumbMonoKeyboard::clear() {
    pitchReceivers.clear();
    gateReceivers.clear();
}

//-----------------------------------------------------------------------------
float pitchToCV(int16* pitch)
{
    return pow(SEMI_TONE_MULTIPLIER, *pitch - 69);
}

//-----------------------------------------------------------------------------
HighestDumbMonoKeyboard::HighestDumbMonoKeyboard() {
    currentPitch = -1;
}

void HighestDumbMonoKeyboard::keyOn(int16* pitch) {
    if (*pitch > currentPitch) {
        currentPitch = *pitch;
        setPitch(pitch);
        triggerOn();
    }
}

void HighestDumbMonoKeyboard::keyOff(int16* pitch) {
    if (*pitch == currentPitch) {
        currentPitch = -1;
        triggerOff();
    }
}

//-----------------------------------------------------------------------------
void LastMonoKeyboard::keyOn(int16* pitch) {
    if (pressedKeys.size() == 0) {
        //the first eky was just pressed
        triggerOn();
    }
    
    for (int i = 0; i < pressedKeys.size(); i++) {
        if (pressedKeys[i] == *pitch) {
            //key alredy pressed
            return;
        }
    }

    pressedKeys.push_back(*pitch);

    setPitch(pitch);
}

void LastMonoKeyboard::keyOff(int16* pitch) {
    for (int i = 0; i < pressedKeys.size(); i++) {
        if (pressedKeys[i] == *pitch) {
            //remove the key from the list of pressed keys
            pressedKeys.erase(pressedKeys.begin() + i);
        }
    }
    if (pressedKeys.size() == 0) {
        //the last key was just released
        triggerOff();
    }
    else {
        //the second last key is now the last key
        setPitch(&pressedKeys.back());
    }
}

} //namespace Synth
} //namespace Steinberg