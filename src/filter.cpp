/*!
 *  \author Manoel McCadden
 *  \date   11-27-25
 *  \file   filter.cpp
 *
 *  \brief
 *    Implementation file for simple filter creator
 */

#include "filter.h"

#include <cmath>
#include <math.h>

#include "helper.h"

using namespace std;

Filter::Filter() { }

Filter::~Filter() { }

void Filter::add_coefficent(const COEFFICENT &coefficent)
{
  coefficents.push_back(coefficent);
}

void Filter::apply_filter(CArray<float> &samples)
{
  size_t size = samples.size();
  float *input = new float[size];

  for(size_t i = 0; i < size; ++i)
  {
    input[i] = samples[i];
  }

  static_cast<void>(Logger(Logger::L_MSG
        , "Applying flter to a wave file of size: " + to_string(size)));
  for(size_t i = 0; i < size; ++i)
  {
    float output = 0.f;
    for(COEFFICENT &coefficent : coefficents)
    {
      // Done to avoid going past the bounds of the array
      if(coefficent.sampleDelay > i)
      {
        continue;
      }

      output += coefficent.coefficent * input[i - coefficent.sampleDelay];
    }

    if(output > 1.f)
    {
      static_cast<void>(Logger(Logger::L_MSG
            , "Error in audio filtering, output above 1.f, output at: " 
            + to_string(output)));
    }

    samples[i] = output;
  }
}

//===========//
// Band Pass //
//===========//

BandPass::BandPass(const float &_frequnecy, const float &_quality
    , const float &_samplingRate)
  : samplingRate(_samplingRate), frequency(_frequnecy), quality(_quality) 
  , gain(1.f) , a0(0.f), b1(0.f), b2(0.f) { }

BandPass::BandPass(const BandPass &other)
{
  *this = other;
}

BandPass::~BandPass() { }

BandPass &BandPass::operator=(const BandPass &other)
{
  if(this == &other)
  {
    return *this;
  }

  samplingRate = other.samplingRate;
  frequency = other.frequency;
  gain = other.gain;
  quality = other.quality;
  a0 = other.a0;
  b1 = other.b1;
  b2 = other.b2;

  return *this;
}

void BandPass::set_quality(const float &_quality)
{
  quality = _quality;
  update_values();
}

void BandPass::set_gain(const float &_gain)
{
  gain = _gain;
}

void BandPass::set_frequency(const float &_frequency)
{
  frequency = _frequency;
  update_values();
}

void BandPass::set_sampling_rate(const float &_samplingRate)
{
  samplingRate = _samplingRate;
  update_values();
}

void BandPass::update_values()
{
  if(samplingRate == 0 || std::isnan(samplingRate))
  {
    samplingRate = 44100;
  }

  const float PI = 4.f * atan(1);
  float omega = 2.f * PI * frequency / samplingRate;
  float sinOmega = sin(omega);
  float cosOmega = cos(omega);
  float alpha = sinOmega / (2.f * quality);

  b0 = alpha;
  b1 = 0;
  b2 = -alpha;
  a0 = 1 + alpha;
  a1 = -2 * cosOmega;
  a2 = 1 - alpha;

  // Normalizing all to a0
  b0 /= a0;
  b1 /= a0;
  b2 /= a0;
  a1 /= a0;
  a2 /= a0;
}

bool BandPass::is_valid() const
{
  if(frequency == 0 || samplingRate == 0)
  {
    return false;
  }

  return true;
}

const float &BandPass::get_sampling_rate() const
{
  return samplingRate;
}

// Biquad band-pass filter
void BandPass::apply_filter(CArray<float> &samples)
{
  if(samplingRate == 0)
  {
    static_cast<void>(Logger(Logger::L_ERR, "Invalid sampling rate of: 0"));
    return;
  }

  update_values();

  float x = 0.f, y = 0.f, x1 = 0.f, y1 = 0.f, x2 = 0.f, y2 = 0.f;
  for(size_t i = 0; i < samples.size(); ++i)
  {
    x = samples[i];

    y = b0 * x + b1 * x1 + b2 * x2 - a1 * y1 - a2 * y2;

    x2 = x1;
    x1 = x;
    y2 = y1;
    y1 = y;

    samples[i] = y * gain;
  }
}

//===========//
// Equalizer //
//===========//

Equalizer::Equalizer(const uint8_t &quanity, const float &_samplingRate
    , const float &_delay)
  : bandMax(quanity), quality(0.f), delay(_delay), samplingRate(_samplingRate) 
{
  bands.resize(quanity);
  calculate_quality(quanity);
}

Equalizer::Equalizer(const Equalizer &other)
{
  *this = other;
}

Equalizer::~Equalizer() { }

Equalizer &Equalizer::operator=(const Equalizer &other)
{
  if(this == &other)
  {
    return *this;
  }

  bandMax = other.bandMax;
  delay = other.delay;
  quality = other.quality;
  samplingRate = other.samplingRate;

  bands.resize(other.bands.size());
  for(size_t i = 0; i < other.bands.size(); ++i)
  {
    bands[i] = other.bands.at(i);
  }

  return *this;
}

void Equalizer::calculate_quality(const uint8_t &quanity)
{
  float delta = (log10(samplingRate) - log10(20.f)) 
    / static_cast<float>(quanity);
  quality = pow(10.f, delta / 2.f) / (pow(10.f, delta) - 1);
}

void Equalizer::add_coefficent(const float &frequency, const float &coefficent
        , const size_t &band)
{
  // NOTE: This should never occur if Equalizer is sized properly on
  // construction
  if(band >= bandMax)
  {
    calculate_quality(band);

    for(size_t i = 0; i < bands.size(); ++i)
    {
      bands[i].set_quality(quality);
    }

    bands.resize(band);
  }
  
  bands[band].set_quality(quality);
  bands[band].set_frequency(frequency);
  bands[band].set_gain(coefficent);
  bands[band].set_sampling_rate(samplingRate);
}

void Equalizer::apply_filter(CArray<float> &samples)
{
  // Check bands for invalid bands
  /*
  for(size_t i = 0; i < bands.size(); ++i)
  {
    if(bands[i].get_sampling_rate() == 0)
    {
      Logger(Logger::L_ERR, "Invalid band found in Equalizer at index: " 
          + to_string(i));
    }
  }
  */

  // Apply filter of all bands 
  CArray<float> returnArray;
  returnArray.resize(samples.size() + static_cast<size_t>(delay));
  for(size_t i = 0; i < returnArray.size(); ++i)
  {
    returnArray[i] = 0.f;
  }

  for(size_t i = 0; i < bands.size(); ++i)
  {
    CArray<float> output(samples);
    // 0Hz bands are left to help normalize audio and removed unwanted
    // distortion
    bands[i].apply_filter(output);

    // Apply delay
    // TODO: May need to look at this :)
    for(size_t j = 0; j < output.size(); ++j)
    {
      returnArray[j+static_cast<size_t>(delay)] += output[j];
    }
  }
  // Override with new output based on input
  samples = returnArray;
}
