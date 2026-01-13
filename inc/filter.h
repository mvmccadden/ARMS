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
    virtual ~Filter();

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
    BandPass(const float &_frequency = 0.f, const float &_quality = 1.f
        , const float &_samplingRate = 44100.f);
    BandPass(const BandPass &other);
    ~BandPass();

    BandPass &operator=(const BandPass &other);

    void set_quality(const float &_quality);
    void set_gain(const float &gain);
    void set_frequency(const float &_frequency);
    void set_sampling_rate(const float &_samplingRate);
    void update_values();

    bool is_valid() const;

    void apply_filter(CArray<float> &samples) override;
  private:
    float samplingRate;
    float frequency;
    float quality;
    float gain;
    float a0, a1, a2, b0, b1, b2;
};

class Equalizer : public Filter
{
  public:
    Equalizer(const uint8_t &quanity = 1
        , const float &_samplingRate = 44000
        , const float &delay = 0.f);
    Equalizer(const Equalizer &other);
    ~Equalizer();

    Equalizer &operator=(const Equalizer &other);

    void calculate_quality(const uint8_t &quanity);

    void add_coefficent(const float &frequency, const float &coefficent
        , const size_t &band);
    void apply_filter(CArray<float> &samples) override;
    void set_delay(const float &delay);

  private:
    uint8_t bandMax;
    float delay;
    float quality;
    float samplingRate;
    CArray<BandPass> bands;
};
