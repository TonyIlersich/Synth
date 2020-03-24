#pragma once

#include "includes.h"
#include "Waveform.h"

struct Module
{
	const static int maxInputChannels = 8, maxOutputChannels = 8;

	hpSignal input[maxInputChannels];
	hpSignal output[maxOutputChannels];

	Module();
	virtual void update(hpTime dt);
};

struct Speaker : Module
{
	void update(hpTime dt) override;
};

struct Constant : Module
{
	hpSignal values[maxOutputChannels];

	Constant* calibrate(hpSignal values[maxOutputChannels]);
	void update(hpTime dt) override;
};

struct Amp : Module
{
	hpSignal gain = 2;

	void update(hpTime dt) override;
};

struct VCO : Module
{
	Waveform audibleWave;
	Waveform lowFreqWave;

	hpSignal audibleFreq = 440;
	hpSignal lowFreq = 8;

	VCO();

	void update(hpTime dt) override;
};

struct Transform : Module
{
	hpSignal scale;
	hpSignal shift;

	Transform* calibrate(hpSignal minInput, hpSignal maxInput, hpSignal minOutput, hpSignal maxOutput);
	void update(hpTime dt) override;
};

struct ADSR : Module
{
	hpSignal threshold = .5L;

	void update(hpTime dt) override;
};

struct Patch
{
	Module *src, *dst;
	int outIdx, inIdx;

	Patch();
	Patch(Module* src, int outIdx, Module* dst, int inIdx);
	void update();
};
