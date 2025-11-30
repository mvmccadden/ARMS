/*!
 *  \author Manoel McCadden
 *  \date   11-27-25
 *  \file   wave.h
 *
 *  \brief
 *    Interface of wave file handling
 */

#pragma once

#include <cstddef>
#include <cstdint>
#include <memory.h>
#include <string>

// Default format for wave header
struct WAVE_HEADER 
{
  char riffLabel[4] = { 'R', 'I', 'F', 'F' };
  uint32_t riffSize = 0;
  char fileTag[4] = { 'W', 'A', 'V', 'E' };
  char fmtLabel[4] = { 'f', 'm', 't', ' ' };
  uint32_t fmtSize = 16;
  uint16_t audioFormat = 1;
  uint16_t channelCount = 1;
  uint32_t samplingRate = 44100;
  uint32_t bytesPerSecond = 88200;
  uint16_t bytesPerSample = 2;
  uint16_t bitsPerSample = 16;
  char dataLabel[4] = { 'd', 'a', 't', 'a' };
  uint32_t dataSize = 0;

  char headerData[44];

  /*!
   *  Set the new size of the Wave Header based on the number of given frames
   *
   *  \param size
   *    The frame count for the wave header
   *
   */
  void set_frameCount(unsigned size);

  /*!
   *  Set the new number of channels for the wave file
   *
   *  \param _channelCount
   *    The number of channels
   */
  void set_channelCount(unsigned _channelCount);

  /*!
   *  Set the new sampling rate for the wave file
   *
   *  \param _samplingRate 
   *    The sampling rate
   */
  void set_samplingRate(unsigned _samplingRate);

  /*!
   *  Set the new number of bits per sample for the wave file
   *
   *  \param bitsPerSample
   *    The number of bits per sample
   */
  void set_bitsPerSample(unsigned _bitPerSample);

  /*!
   *  Generates a WAVE file header using the data stored in the WAVE_HEADER
   *  struct.
   *
   *  \returns
   *    A pointer to a 44 char array of data storing the WAVE file header
   */
  char *generate_wave_header();
  
  /*!
   *  Reads in the data stored in the header data variable and saves it within
   *  the rest of the wave header struct for ease of access
   */
  void read_wave_header();
};

struct SAMPLES
{
  private:
    void resize();

    size_t frameCount = 0;
    float *sampleArray = nullptr;

  public:
    ~SAMPLES();

    float &operator[](const size_t &index);

    void resize(const size_t &_size);

    const float &at(const size_t &index) const;
    const float *front() const;
    const size_t size() const;

    void clear();
};

class WaveFile
{
  public:
    WaveFile();
    WaveFile(const std::string &fileName);
    ~WaveFile();
 
    void open_file(const std::string &fileName
        , const bool &ignoreInputDir = false);
    void output_to_file(const std::string &fileName);

    bool is_open() const;

    std::string get_name() const;

    unsigned get_sampling_rate() const;
    SAMPLES &get_samples();
  private:
    
    void convert_from_pcm_values(char *values);
    char *convert_to_pcm_values();

    bool open = false;

    std::string name;

    WAVE_HEADER header;
    SAMPLES samples;
};
