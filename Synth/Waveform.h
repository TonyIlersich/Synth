#pragma once
#include "includes.h"

struct Waveform
{
	hpSignal phase;
	hpSignal(*waveFunction)(hpSignal t);

	inline Waveform(hpSignal(*waveFunction)(hpSignal t) = nullptr)
	{
		phase = 0.L;
		this->waveFunction = waveFunction;
	}

	inline void step(hpSignal f, hpSignal dt)
	{
		phase += dt * f;
		hpSignal garbage;
		phase = modfl(phase, &garbage);
	}

	inline hpSignal getValue()
	{
		if (waveFunction == nullptr)
			return 0.L;
		return waveFunction(phase);
	}
};

inline hpSignal sine(hpSignal t)
{
	return sin(TAO * t);
}

inline hpSignal square(hpSignal t)
{
	return t < .5L ? 1.L : -1.L;
}

//TODO: pulse, saw, triangle