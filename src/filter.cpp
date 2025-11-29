/*!
 *  \author Manoel McCadden
 *  \date   11-27-25
 *  \file   filter.cpp
 *
 *  \brief
 *    Implementation file for simple filter creator
 */

#include "filter.h"

#include "helper.h"
#include "wave.h"

using namespace std;

Filter::Filter() { }

Filter::~Filter() { }

void Filter::add_coefficent(const COEFFICENT &coefficent)
{
  coefficents.push_back(coefficent);
}

void Filter::apply_filter(SAMPLES &samples)
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
