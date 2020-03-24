#include "Modules.h"

//==============================================================================
// MODULE
//==============================================================================

Module::Module()
{
	for (int i = 0; i < maxInputChannels; i++)
	{
		input[i] = 0.L;
	}
	for (int i = 0; i < maxOutputChannels; i++)
	{
		output[i] = 0.L;
	}
}

void Module::update(hpTime dt)
{
	printf("CALLED UPDATE OF BASE\n");
}

//==============================================================================
// SPEAKER
//==============================================================================

void Speaker::update(hpTime dt)
{
	output[0] = input[0];
}

//==============================================================================
// AMP
//==============================================================================

void Amp::update(hpTime dt)
{
	for (int i = 0; i < maxOutputChannels; i++)
		output[i] = gain * input[i];
}

//==============================================================================
// VCO
//==============================================================================

VCO::VCO()
{
	audibleWave = Waveform(square);
	lowFreqWave = Waveform(sine);
}

void VCO::update(hpTime dt)
{
	// convert time from ticks to seconds
	hpSignal deltaTime = dt * TICKS_TO_SECONDS;

	audibleWave.step(audibleFreq * pow(2.L, input[0]), deltaTime);
	output[0] = audibleWave.getValue();

	lowFreqWave.step(lowFreq * pow(2.L, input[1]), deltaTime);
	output[1] = lowFreqWave.getValue();
}

//==============================================================================
// TRANSFORM
//==============================================================================

Transform* Transform::calibrate(hpSignal minInput, hpSignal maxInput, hpSignal minOutput, hpSignal maxOutput)
{
	hpSignal avgInput = (minInput + maxInput) / 2.L;
	hpSignal avgOutput = (minOutput + maxOutput) / 2.L;
	shift = avgOutput - avgInput;

	hpSignal inputWidth = maxInput - minInput;
	hpSignal outputWidth = maxOutput - minOutput;
	scale = outputWidth / inputWidth;

	return this;
}

void Transform::update(hpTime dt)
{
	output[0] = input[0] * scale + shift;
}

//==============================================================================
// PATCH
//==============================================================================

Patch::Patch()
{
	src = dst = nullptr;
	outIdx = inIdx = 0;
}

Patch::Patch(Module* src, int outIdx, Module* dst, int inIdx)
{
	this->src = src;
	this->outIdx = outIdx;
	this->dst = dst;
	this->inIdx = inIdx;
}

void Patch::update()
{
	dst->input[inIdx] = src->output[outIdx];
}

void ADSR::update(hpTime dt)
{
	bool open = input[0] >= threshold;
	hpSignal& attack = input[1];
	hpSignal& decay = input[2];
	hpSignal& sustain = input[3];
	hpSignal& release = input[4];
}

Constant* Constant::calibrate(hpSignal values[maxOutputChannels])
{
	memcpy(this->values, values, maxOutputChannels);
	return this;
}

void Constant::update(hpTime dt)
{
	for (int i = 0; i < maxOutputChannels; i++)
		output[i] = values[i];
}
