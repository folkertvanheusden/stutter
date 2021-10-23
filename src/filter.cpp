#include <math.h>
#include <string.h>

#include "filter.h"


FilterButterworth::FilterButterworth(const double frequency, const int sampleRate, const double isHighPass, const double resonance)
{
	this -> resonance = resonance;
	this -> frequency = frequency;
	this -> sampleRate = sampleRate;
	this -> isHighPass = isHighPass;

	if (isHighPass) {
		c = tan(M_PI * frequency / sampleRate);
		a1 = 1.0 / (1.0 + resonance * c + c * c);
		a2 = -2.0 * a1;
		a3 = a1;
		b1 = 2.0 * (c * c - 1.0) * a1;
		b2 = (1.0 - resonance * c + c * c) * a1;
	}
	else {
		c = 1.0 / tan(M_PI * frequency / sampleRate);
		a1 = 1.0 / (1.0 + resonance * c + c * c);
		a2 = 2.0 * a1;
		a3 = a1;
		b1 = 2.0 * (1.0 - c * c) * a1;
		b2 = (1.0 - resonance * c + c * c) * a1;
	}

	memset(inputHistory, 0x00, sizeof inputHistory);
	memset(outputHistory, 0x00, sizeof outputHistory);
}

double FilterButterworth::apply(const double newInput)
{
	double newOutput = a1 * newInput + a2 * inputHistory[0] + a3 * inputHistory[1] - b1 * outputHistory[0] - b2 * outputHistory[1];

	inputHistory[1] = inputHistory[0];
	inputHistory[0] = newInput;

	outputHistory[2] = outputHistory[1];
	outputHistory[1] = outputHistory[0];
	outputHistory[0] = newOutput;

	return outputHistory[0];
}
