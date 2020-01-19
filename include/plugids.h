//------------------------------------------------------------------------
// Project     : VST SDK
//
// Category    : Examples
// Filename    : plugids.h
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

#pragma once

namespace Steinberg {
namespace Synth {

// HERE are defined the parameter Ids which are exported to the host
enum SynthParams : Vst::ParamID
{
	kParamOp1_levelId = 100,
	kParamOp1_frequencyId = 101,
	kParamOp1_attackId = 102,
	kParamOp1_decayId = 103,
	kParamOp1_sustainId = 104,
	kParamOp1_releaseId = 105,
	
	kParamOp2_levelId = 106,
	kParamOp2_frequencyId = 107,
	kParamOp2_attackId = 108,
	kParamOp2_decayId = 109,
	kParamOp2_sustainId = 110,
	kParamOp2_releaseId = 111,

	kParamMasterVolumeId = 112,
};


// HERE you have to define new unique class ids: for processor and for controller
// you can use GUID creator tools like https://www.guidgenerator.com/

static const FUID MyProcessorUID(0x61931190, 0xF7E7435C, 0x889915D6, 0x75B5DFDD);
static const FUID MyControllerUID(0xCFEAFF24, 0x05DA434B, 0x84CBBDCD, 0xDEF36BE0);

//61931190-F7E7435C-889915D6-75B5DFDD
//CFEAFF24-05DA434B-84CBBDCD-DEF36BE0

//------------------------------------------------------------------------
} // namespace Synth
} // namespace Steinberg
