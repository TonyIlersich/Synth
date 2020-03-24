#include "includes.h"
#include "Modules.h"

struct SynthBoard
{
private:

	const hpTime timeStep = 200 * NANOSECOND;

	std::vector<Module*> mods;
	std::vector<Patch> patches;
	hpTime t;

	// patch a module's output to another module's input
	void patch(int srcModIdx, int outIdx, int dstModIdx, int inIdx)
	{
		patches.push_back(Patch(mods[srcModIdx], outIdx, mods[dstModIdx], inIdx));
	}

public:

	// create and hook up modules
	SynthBoard()
	{
		// set time at zero
		t = TIME_ZERO;

		// use module 0 as output
		mods.push_back(new Speaker());

		// use module 1 as oscillator
		mods.push_back(new VCO());
	
		// use module 2 as transform
		mods.push_back((new Transform())->
			calibrate(-1.L, 1.L, 0L, 2L));

		// patch output 0 of oscillator to speaker
		patch(1, 0, 0, 0);

		// patch output 1 of oscillator into input 0 of oscillator
		patch(1, 1, 1, 0);

		// patch output 1 of oscillator to input 0 of transform
		//patch(1, 1, 2, 0);

		// patch output 0 of transform into input 0 of oscillator
		//patch(2, 0, 1, 0);
	}

	// free modules
	~SynthBoard()
	{
		for (size_t i = 0; i < mods.size(); i++)
		{
			free(mods[i]);
			mods[i] = nullptr;
		}
	}

	// simulate the synth board until the next sample can be generated and return it
	sf::Int16 getNextSample(hpTime dt)
	{
		// calculate time to measure sample
		hpTime tf = t + dt;

		// simulate synth at one iteration per microsecond
		for (; t < tf; t += timeStep)
		{
			// update modules
			for (size_t i = 0; i < mods.size(); i++)
			{
				mods[i]->update(timeStep);
			}

			// update patch cables
			for (size_t i = 0; i < patches.size(); i++)
			{
				patches[i].update();
			}
		}

		// get sample from output module
		hpSignal sample = mods[0]->output[0] * .5L;
		return (sf::Int16)(sample * INT16_MAX);
	}
};

struct Synth : public sf::SoundStream
{
private:

	// used for analytics
	sf::Clock clock;

	// sound object used to play
	sf::Sound sound;

	// used to generate sound samples
	SynthBoard board;

	const static int samplesPerSecond = 44100;
	const static int samplesPerChunk = 2000;
	sf::Int16 samples[samplesPerChunk];

	// generate samples
	bool onGetData(Chunk& data) override
	{
		clock.restart();

		for (int i = 0; i < samplesPerChunk; i++)
		{
			// calculate the time interval
			const hpTime t0 = SECOND * i / samplesPerSecond;
			const hpTime t1 = SECOND * (i + 1) / samplesPerSecond;
			const hpTime dt = t1 - t0;

			// simulate the synthboard
			samples[i] = board.getNextSample(dt);
		}

		data.sampleCount = samplesPerChunk;
		data.samples = samples;

		printf("%.1f samples/sec\n", samplesPerChunk / clock.getElapsedTime().asSeconds());

		return true;
	}

	// reposition
	void onSeek(sf::Time timeOffset) override { }

public:

	Synth()
	{
		initialize(1, samplesPerSecond);
	}
};

int main()
{
	Synth synth;
	synth.play();
	printf("press enter to stop\n");
	getchar();
	printf("waiting for buffer to clear...\n");
	synth.stop();
	printf("press enter to exit");
	getchar();
	return 0;
}