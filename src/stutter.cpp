#include "DistrhoPlugin.hpp"
#include "filter.h"

START_NAMESPACE_DISTRHO

class Stutter : public Plugin
{
public:
	Stutter() : Plugin(1, 0, 0), statePosition(0), stateValue(0.0)
	{
		fbw = nullptr;

		sampleRateChanged(getSampleRate());
	}

	~Stutter() override
	{
		delete fbw;
	}

protected:
	const char* getLabel() const override
	{
		return "Stutter";
	}

	const char* getDescription() const override
	{
		return "Igorrr alike effect";
	}

	const char* getMaker() const override
	{
		return "vanheusden.com";
	}

	const char* getHomePage() const override
	{
		return "https://vanheusden.com/disthro/stutter";
	}

	const char* getLicense() const override
	{
		return "Apache License v2.0";
	}

	uint32_t getVersion() const override
	{
		return d_version(1, 0, 0);
	}

	int64_t getUniqueId() const override
	{
		return d_cconst('F', 'v', 'H', '0');
	}

	void initParameter(uint32_t index, Parameter& parameter) override
	{
		if (index != 0)
			return;

		parameter.hints  = kParameterIsAutomable;
		parameter.name   = "Stutter";
		parameter.symbol = "stutter";
		parameter.unit   = "ms";
		parameter.ranges.def = 100.0f;
		parameter.ranges.min = 1.0f;
		parameter.ranges.max = 1000.0f;
	}

	float getParameterValue(uint32_t index) const override
	{
		if (index != 0)
			return 0.0f;

		return fStutterness;
	}

	void setParameterValue(uint32_t index, float value) override
	{
		if (index != 0)
			return;

		fStutterness = value;
		stutternessInFrames = value * getSampleRate() / 1000;
	}

	void run(const float** inputs, float** outputs, uint32_t frames) override
	{
		if (!fbw)
			return;

		const float *const in = inputs[0];
		float *const out = outputs[0];

		for(uint32_t i=0; i<frames; i++) {
			if (statePosition >= stutternessInFrames) {
				if (statePosition >= stutternessInFrames * 2) {
					statePosition = 0;
					stateValue = out[i] = fbw->apply(in[i]);
				}
				else {
					out[i] = fbw->apply(stateValue);
				}
			}
			else {
				stateValue = out[i] = fbw->apply(in[i]);
			}

			statePosition++;
		}
	}

	void sampleRateChanged(double newSampleRate) override
	{
		stutternessInFrames = fStutterness * newSampleRate * 1000;

		delete fbw;
		fbw = new FilterButterworth(newSampleRate / 2 - 200, newSampleRate, false, sqrt(2.0));
	}

private:
	float fStutterness;
	uint32_t stutternessInFrames;

	uint32_t statePosition;
	float stateValue;

	FilterButterworth *fbw;

	DISTRHO_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Stutter)
};


Plugin* createPlugin()
{
	return new Stutter();
}

END_NAMESPACE_DISTRHO
