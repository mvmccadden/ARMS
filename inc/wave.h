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

#include "helper.h"

// Default format for wave header
/*!
 *  \struct WAVE_HEADER
 *
 *  \brief
 *    A simple container for wave header data.
 *    All its information is public for easy access but, it is
 *    expected of the user to know what they are doing when modifying it.
 *
 *    Included are some methods for setting
 *    - FrameCount
 *    - ChannelCount
 *    - SamplingRate
 *    - BitsPerSample
 *
 *    This will also set their respective related properties.
 *
 *    Additionally, included are two metods for generating and reading
 *    wave data for input and output.
 */
struct WAVE_HEADER 
{
  /*!
   *  Default ctor for wave header setting all inital values
   */
  WAVE_HEADER();
  /*!
   *  Copies a given header into a new header, generating a 
   *  new file header with the same data.
   *
   *  \param header
   *    The header being copied
   */
  WAVE_HEADER(const WAVE_HEADER& header);

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
  void read_wave_header(std::fstream &file);
};

struct SAMPLES
{
  private:
    /*!
     *  Default resize that doubles the size of the array
     */
    void resize();

    size_t frameCount = 0;
    float *sampleArray = nullptr;

  public:
    /*!
     *  Default ctor for SAMPLES class, creating an empty array of samples
     */
    SAMPLES();
    /*!
     *  Deep copies in a given sample array into a newly created array
     *
     *  \param samples
     *    The sample array being copied over
     */
    SAMPLES(const SAMPLES &samples);
    ~SAMPLES();

    /*!
     *  Performs a deep copy of the given sample data into this SAMPLES object.
     *  This operation will overwrite all current data with the new deep copied
     *  data.
     *
     *  \param samples
     *    The sample being copied from
     *
     *  \returns
     *    A reference to the current sample object
     */
    SAMPLES &operator=(const SAMPLES &samples);

    /*!
     *  Gives the user a reference to a value at a given index within
     *  the sample array.
     *
     *  NOTE: This will warn the user if the user attempts to dereference an 
     *  index outside of the bounds of the array, BUT it will not throw an
     *  error and will instead resize the array if possible.
     *
     *  \param index
     *    The index we are fetching from within the array
     *
     *  \returns
     *    A reference to the float stored at the given index
     */
    float &operator[](const size_t &index);

    /*!
     *  Resizes the array to a given size
     *
     *  \param _size
     *    The size the array is being resized to
     */
    void resize(const size_t &_size);

    /*!
     *  A more robust approach to getting a value at a given index which wil
     *  report an error if the index is outside the bounds of the array and
     *  return the last value in the array.
     *
     *  \param index
     *    The index being fetched from within the array
     *
     *  \returns
     *    A const-reference to the value at the given index
     */
    const float &at(const size_t &index) const;
    /*!
     *  \returns
     *    A pointer to the front of the array
     */
    const float *front() const;
    /*!
     *  \returns
     *    The size of the array
     */
    const size_t size() const;

    /*!
     *  Clears the array of its contents, setting the size to 0 and array to
     *  nullptr
     */
    void clear();
};

class WaveFile
{
  public:
    /*!
     *  Default ctor for WaveFile, setting allvalues to their desired defaults
     */
    WaveFile();
    /*!
     *  Opens a given file for reading and creates a WAVE_HEADER and SAMPLES
     *  based on the files data
     *
     *  \param fileName
     *    The name of the file within the input directory
     */
    WaveFile(const std::string &fileName);
    /*!
     *  Performs a deep copy of a WaveFile, copying its WAVE_HEADER and SAMPLES
     *
     *  \param wave
     *    The wave file being deep copied
     */
    WaveFile(const WaveFile &wave);
    ~WaveFile();
 
    /*!
     *  Opens a .wav file at a given location, reading the header and samples
     *  and overwriting any current data
     *
     *  \param fileName
     *    The name or path to the file being opened
     *  \param ignoreInputDir
     *    Indicates if the given file is NOT in the input directory
     */
    void open_file(const std::string &fileName
        , const bool &ignoreInputDir = false);
    /*!
     *  Outputs the WaveFile class data into a given filepath
     *
     *  \param fileName
     *    The path of the file that will be outputed to
     */
    void output_to_file(const std::string &fileName);

    /*!
     *  \returns
     *    If the WaveFile has currently open data
     */
    bool is_open() const;
    /*!
     *  \returns
     *    The name of the file that is currently open
     */
    std::string get_name() const;

    /*!
     *  \returns
     *    The sampling rate of the currently open WaveFile
     */
    unsigned get_sampling_rate() const;
    /*!
     *  \returns
     *    A reference to the CArray samples of the currently open WaveFile
     */
    CArray<float> &get_samples();
  private:
    /*!
     *  Converts pcm values to float sample values and places them within 
     *  the SAMPLES struct
     *
     *  \param values
     *    A pointer to an array of chars that represent the pcm values
     */
    void convert_from_pcm_values(char *values);
    /*!
     *  Converts the float values stored within the SAMPLES struct into .wav
     *  file valid pcm values
     */
    char *convert_to_pcm_values();

    bool open = false;

    std::string name;

    WAVE_HEADER header;
    CArray<float> samples;
};
