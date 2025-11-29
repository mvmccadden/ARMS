/*!
 *  \author Manoel McCadden
 *  \date   11-27-25
 *  \file   unittests.h
 *
 *  \brief
 *    Handles a handful of unit test cases using default inputs
 */

#include "helper.h"

#include "wave.h"
#include "filter.h"

void test_wave_input_output(const std::string &fileName)
{
  WaveFile wave(fileName);
  wave.output_to_file(fileName + "_out");
}

void test_wave_with_simple_filter(const std::string &fileName)
{
  WaveFile wave(fileName);

  Filter filter;
  filter.add_coefficent({0.5f, 0});
  filter.apply_filter(wave.get_samples());

  wave.output_to_file(fileName + "_out");
}
