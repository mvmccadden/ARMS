/*!
 *  \author Manoel McCadden
 *  \date   11-27-25
 *  \file   filter.h
 *
 *  \brief
 *    Interface file for simple filter creator
 */

#pragma once

#include <vector>
#include <cstdint>

#include "helper.h"

typedef struct SAMPLES SAMPLES;

class Filter
{
  protected:
    struct COEFFICENT
    {
      union
      {
        float coefficent;
        float frequency = 0.f;
      };
      union
      {
        unsigned sampleDelay;
        unsigned band = 0u;
      };
    };

  public:
    Filter();
    ~Filter();

    virtual void add_coefficent(const COEFFICENT &coefficent);

    virtual void apply_filter(CArray<float> &samples);

  private:
    std::vector<COEFFICENT> coefficents;
};

class LowPass : public Filter
{
  public:
    LowPass(const float &cutoff);
    ~LowPass();

  private:
    float coefficent;
    float prevOutput;
};

class BandPass : public Filter
{
  public:
    /*!
     *  Default ctor for the BandPass filter
     *
     *  \param _frequnecy
     *    The central frequency of the band
     *  \param _quality
     *    The quality of the band
     *  \param _samplingRate
     *    The sampling rate of the filter
     */
    BandPass(const float &_frequency = 0.f, const float &_quality = 0.f
        , const float &_samplingRate = 0.f);
    ~BandPass();

    void set_quality(const float &_quality);
    void set_frequency(const float &_frequency);
    void set_sampling_rate(const float &_samplingRate);
    void update_values();

    void apply_filter(CArray<float> &samples) override;
  private:
    float samplingRate;
    float frequency;
    float quality;
    float a0, b1, b2;
};

class Equalizer : public Filter
{
  public:
    Equalizer(const uint8_t &quanity, const float &_samplingRate);
    ~Equalizer();

    void calculate_quality(const uint8_t &quanity);

    void add_coefficent(const Filter::COEFFICENT &coefficent) override;
    void apply_filter(CArray<float> &samples) override;

  private:
    uint8_t bandMax;
    float quality;
    float samplingRate;
    CArray<BandPass> bands;
};
