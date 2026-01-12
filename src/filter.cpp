/*!
 *  \author Manoel McCadden
 *  \date   11-27-25
 *  \file   filter.cpp
 *
 *  \brief
 *    Implementation file for simple filter creator
 */

#include "filter.h"

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
  , a0(0.f), b1(0.f), b2(0.f)
{ 

}

BandPass::~BandPass() { }

void BandPass::set_quality(const float &_quality)
{
  quality = _quality;
  update_values();
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
  const float PI = 4.f * atan(1);
  float t = tan(PI * frequency * samplingRate);
  float tt = t * t;
  float optt = 1 + tt;
  float factor = 1.f/(quality*optt*t);
  a0 = factor * t;
  b1 = factor * 2.f * quality * (1 - tt);
  b2 = factor * (t - quality * optt);
}

void BandPass::apply_filter(CArray<float> &samples)
{
  float x = 0.f, y = 0.f, x1 = 0.f, y1 = 0.f, x2 = 0.f, y2 = 0.f;
  for(size_t i = 0; i < samples.size(); ++i)
  {
    x = samples[i];
    y = a0 * (x - x2) + b1 * y1 + b2 * y2;
    x2 = x1;
    x1 = x;
    y2 = y1;
    y1 = y;
    samples[i] = y;
  }
}

//===========//
// Equalizer //
//===========//

Equalizer::Equalizer(const uint8_t &quanity, const float &_samplingRate)
  : bandMax(quanity), quality(0.f), samplingRate(_samplingRate) 
{
  bands.resize(quanity);
  calculate_quality(quanity);
}

Equalizer::~Equalizer() { }

void Equalizer::calculate_quality(const uint8_t &quanity)
{
  float delta = (log10(samplingRate) - log10(20.f)) 
    / static_cast<float>(quanity);
  quality = pow(10.f, delta / 2.f) / (pow(10.f, delta) - 1);
}

void Equalizer::add_coefficent(const Filter::COEFFICENT &coefficent)
{
  if(coefficent.band > bandMax)
  {
    calculate_quality(coefficent.band);

    for(size_t i = 0; i < bands.size(); ++i)
    {
      bands[i].set_quality(quality);
    }

    bands.resize(coefficent.band);
  }
  
  bands[coefficent.band].set_quality(quality);
  bands[coefficent.band].set_frequency(coefficent.frequency);
  bands[coefficent.band].set_sampling_rate(samplingRate);
}

void Equalizer::apply_filter(CArray<float> &samples)
{
  // Apply filter of all bands 
  CArray<float> returnArray;
  for(size_t i = 0; i < bands.size(); ++i)
  {
    CArray<float> output(samples);
    bands[i].apply_filter(output);
    returnArray += output;
  }
  // Override with new output based on input
  samples = returnArray;
}
