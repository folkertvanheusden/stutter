#pragma once

// based on http://stackoverflow.com/questions/8079526/lowpass-and-high-pass-filter-in-c-sharp
class FilterButterworth
{
private:
	/// <summary>
	/// rez amount, from sqrt(2) to ~ 0.1
	/// </summary>
	double resonance;
	double frequency;
	int sampleRate;
	bool isHighPass;

	double c, a1, a2, a3, b1, b2;

	/// <summary>
	/// Array of input values, latest are in front
	/// </summary>
	double inputHistory[2];

	/// <summary>
	/// Array of output values, latest are in front
	/// </summary>
	double outputHistory[3];

public:
	FilterButterworth(const double frequency, const int sampleRate, const double isHighPass, const double resonance);

	double apply(const double newInput);
};
