
#include "wave.h"

#include <fstream>

#include "helper.h"

using namespace std;

//==============================================================================
//  Wave Header 
//==============================================================================

void WAVE_HEADER::set_frameCount(unsigned size)
{
  dataSize = size * channelCount * bytesPerSample;
  riffSize = 36 + dataSize;
}

void WAVE_HEADER::set_channelCount(unsigned _channelCount)
{
  unsigned size = dataSize / channelCount / bytesPerSample;
  channelCount = _channelCount;
  set_frameCount(size);
}

void WAVE_HEADER::set_samplingRate(unsigned _samplingRate)
{
  samplingRate = _samplingRate;   
  bytesPerSecond = bytesPerSample * samplingRate;
}

void WAVE_HEADER::set_bitsPerSample(unsigned _bitPerSample)
{
  unsigned size = dataSize / channelCount / bytesPerSample;
  bitsPerSample = _bitPerSample;
  bytesPerSample = _bitPerSample / 8;
  bytesPerSecond = bytesPerSample * samplingRate;
  set_frameCount(size);
}

char *WAVE_HEADER::generate_wave_header()
{
  memcpy(headerData, riffLabel, sizeof(char) * 4);
  memcpy(headerData + 4, &riffSize, sizeof(uint32_t));
  memcpy(headerData + 8, fileTag, sizeof(char) * 4);
  memcpy(headerData + 12, fmtLabel, sizeof(char) * 4);
  memcpy(headerData + 16, &fmtSize, sizeof(uint32_t));
  memcpy(headerData + 20, &audioFormat, sizeof(uint16_t));
  memcpy(headerData + 22, &channelCount, sizeof(uint16_t));
  memcpy(headerData + 24, &samplingRate, sizeof(uint32_t));
  memcpy(headerData + 28, &bytesPerSecond, sizeof(uint32_t));
  memcpy(headerData + 32, &bytesPerSample, sizeof(uint16_t));
  memcpy(headerData + 34, &bitsPerSample, sizeof(uint16_t));
  memcpy(headerData + 36, dataLabel, sizeof(char) * 4);
  memcpy(headerData + 40, &dataSize, sizeof(uint32_t));

  return headerData;
}

void WAVE_HEADER::read_wave_header()
{
  memcpy(riffLabel, headerData, sizeof(char) * 4);
  memcpy(&riffSize, headerData + 4, sizeof(uint32_t));
  memcpy(fileTag, headerData + 8, sizeof(char) * 4);
  memcpy(fmtLabel, headerData + 12, sizeof(char) * 4);
  memcpy(&fmtSize, headerData + 16, sizeof(uint32_t));
  memcpy(&audioFormat, headerData + 20, sizeof(uint16_t));
  memcpy(&channelCount, headerData + 22, sizeof(uint16_t));
  memcpy(&samplingRate, headerData + 24, sizeof(uint32_t));
  memcpy(&bytesPerSecond, headerData + 28, sizeof(uint32_t));
  memcpy(&bytesPerSample, headerData + 32, sizeof(uint16_t));
  memcpy(&bitsPerSample, headerData + 34, sizeof(uint16_t));
  memcpy(dataLabel, headerData + 36, sizeof(char) * 4);
  memcpy(&dataSize, headerData + 40, sizeof(uint32_t));
}

//==============================================================================
//  Samples
//==============================================================================

SAMPLES::~SAMPLES()
{
  clear();
}

void SAMPLES::resize()
{
  resize(2 * frameCount);
}

void SAMPLES::resize(const size_t &size)
{
  float *newArray = new float[size];

  // In order to not copy into outside of the bounds of the new
  // array we will set the frameCount to the lower option between
  // it and size and "truncate" all values after
  if(frameCount > size)
  {
    frameCount = size;
  }

  memcpy(newArray, sampleArray, frameCount);

  clear();

  frameCount = size;
  sampleArray = newArray;
}

float &SAMPLES::operator[](const size_t &index)
{
  if(index > frameCount)
  {
    // NOTE: A warning that a reallocation is occuring should be added here
    static_cast<void>(Logger(Logger::L_WRN
          , string("Attempted to dereference invalid index, ")
            + "reallocating sample array to size of: " + to_string(index)));
    resize(index);
  }

  return sampleArray[index];
}

const float &SAMPLES::at(const size_t &index) const
{
  if(index > frameCount)
  {
    // NOTE: An error log should be added here
    static_cast<void>(Logger(Logger::L_ERR
          , string("Attempted to dereference invalid index, ")
            + "return last index of sample array instead"));
    return sampleArray[frameCount - 1];
  }

  return sampleArray[index];
}

const float *SAMPLES::front() const
{
  return sampleArray;
}

const size_t SAMPLES::size() const
{
  return frameCount;
}

void SAMPLES::clear()
{
  if(sampleArray)
  {
    delete []sampleArray;
  }

  frameCount = 0;
}

//==============================================================================
//  WaveFile 
//==============================================================================

WaveFile::WaveFile(const string &fileName)
{
  open_file(fileName);
}

WaveFile::~WaveFile() { }

void WaveFile::open_file(const string &fileName, const bool &ignoreInputDir)
{
  fstream file;
  if(ignoreInputDir)
  {
    file.open(fileName);
    size_t lastSlash = fileName.find_last_of("\\/");
    if(lastSlash != string::npos) 
    {
      name = fileName.substr(lastSlash);
    }
    else
    {
      name = "output";
    }

    size_t dotPos = name.rfind('.');
    if(dotPos != string::npos)
    {
      name = name.substr(0, dotPos);
    }
  }
  else
  {
    file.open(INPUT_DIR + fileName + ".wav");
    name = fileName;
  }

  if(!file)
  {
    static_cast<void>(Logger(Logger::L_ERR
          , "Failed to access given wave file" ));
    return;
  }

  file.read(header.headerData, 44);
  header.read_wave_header();

  // Using strncmp as the char arrays arn't 0 terminated so they will read the
  // next value in struct causing errors
  if(strncmp(header.fileTag, "WAVE", 4) != 0 
      || strncmp(header.riffLabel, "RIFF", 4) != 0)
  {
    static_cast<void>(Logger(Logger::L_ERR
          , "Invalid wave file passed in for reading" ));
    return;
  }

  // TODO: Account for possible other chunk formats of wave files
  if(strncmp(header.dataLabel, "data", 4) != 0)
  {
    static_cast<void>(Logger(Logger::L_ERR
          , "Invalid wave file passed in, data not in expected location" ));
    return;
  }

  char *values = new char[header.dataSize];
  file.read(values, header.dataSize);

  convert_from_pcm_values(values);

  delete []values;

  file.close();

}

void WaveFile::output_to_file(const string &fileName)
{
  ofstream file(OUTPUT_DIR + fileName + ".wav");

  if(!file)
  {
    static_cast<void>(Logger(Logger::L_ERR
          , "Failed to create given wave file" ));
    return;
  }

  char *pcmValues = convert_to_pcm_values();
  header.generate_wave_header();

  file.write(header.headerData, 44);
  file.write(pcmValues, header.dataSize);

  file.close();
}

string WaveFile::get_name() const
{
  return name;
}

unsigned WaveFile::get_sampling_rate() const
{
  return header.samplingRate;
}

SAMPLES &WaveFile::get_samples()
{
  return samples;
}

void WaveFile::convert_from_pcm_values(char *values)
{
  char *data = values;
  size_t frameCount = header.dataSize / header.channelCount / header.bytesPerSample;
  samples.resize(frameCount);

  if(header.bitsPerSample == 8)
  {
    for(size_t i = 0; i < frameCount; ++i)
    {
      uint8_t sample = 0u;
      // Using memcpy instead of a reinterpret cast to avoid undefined behavior
      // NOTE: This isn't a worry with an 8-bit size but we are doing it for
      // consistency
      memcpy(&sample, data, sizeof(uint8_t));
      samples[i] = (static_cast<float>(sample) - 128.f) / 128.f;
      data += sizeof(uint8_t);
    }
  }
  else
  {
    for(size_t i = 0; i < frameCount; ++i)
    {
      int16_t sample = 0;
      // Using memcpy instead of a reinterpret cast to avoid undefined behavior
      memcpy(&sample, data, sizeof(int16_t));
      samples[i] = static_cast<float>(sample) / 32768.f;
      data += sizeof(int16_t);
    }
  }
}

char *WaveFile::convert_to_pcm_values()
{
  size_t frameCount = samples.size();
  header.dataSize = frameCount * header.channelCount * header.bytesPerSample;
  header.riffSize = 36 + header.dataSize;
  char *values = new char[header.dataSize];
  char *data = values;

  if(header.bitsPerSample == 8)
  {
    for(size_t i = 0; i < frameCount; ++i)
    {
      uint8_t sample = static_cast<uint8_t>(samples[i] * 127.f + 128.f);
      // Using memcpy instead of a reinterpret cast to avoid undefined behavior
      // NOTE: This isn't a worry with an 8-bit size but we are doing it for
      // consistency
      memcpy(data, &sample, sizeof(uint8_t));
      data += sizeof(uint8_t);
    }
  }
  else
  {
    for(size_t i = 0; i < frameCount; ++i)
    {
      int16_t sample = static_cast<int16_t>(samples[i] * 32767.f);
      // Using memcpy instead of a reinterpret cast to avoid undefined behavior
      memcpy(data, &sample, sizeof(int16_t));
      data += sizeof(int16_t);
    }
  }
  
  return values;
}
