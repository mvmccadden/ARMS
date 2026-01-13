/*!
 *  \author Manoel McCadden
 *  \date   11-27-25
 *  \file   scene.h
 *
 *  \brief
 *    Interface of object and filter generation from scenes
 */

#pragma once

#include <vector>
#include <string>
#include <SFML/Graphics/RenderWindow.hpp>

#include "arms_math.h"
#include "filter.h"
#include "helper.h"

typedef class AudioRay AudioRay;
typedef class Object Object;
typedef class Filter Filter;
typedef class WaveFile WaveFile;
typedef struct RayGenerationInfo RayGenerationInfo;
typedef struct Vec2 Vec;
typedef struct SAMPLES SAMPLES;

using AudioRayVec = std::vector<std::vector<AudioRay *>>;
using ObjectVec = std::vector<Object *>;

class Scene
{
  public:
    Scene(const Vec2 &topLeftPos, const Vec2 &size, const Vec2 &scalar);
    Scene(const std::string &fileName, const Vec2 &topLeftPos
        , const Vec2 &size, const Vec2 &scalar
        , const bool &ignoreInputDir = false);
    ~Scene();

    /*!
     *  Opens a new scene based on a user given file path
     *
     *  \param fileName
     *    The name and path of the file being opened with the scene
     *  \param ignoreInputDir
     *    If the file should be searched for starting from the input directory
     *
     *  \returns
     *    The size of the new scene object
     */
    Vec2 open_scene(const std::string &fileName
        , const bool &ignoreInputDir = false);

    bool is_open() const;

    void apply_filter_to_wave(WaveFile &wave);
    void apply_t60_to_wave(WaveFile &wave);

    std::string get_name() const;
    
    void draw(sf::RenderWindow &window);
  private:
    /*!
     *  Adds a bandpass reverb filter based on user given delay to 
     *  a given input.
     *  Instead of overwriting the input it adds it to a given output.
     *
     *
     *  \param delay
     *    The delay for the comb filter
     *  \param frequency
     *    The frequency of the bandpass filter
     *  \param bandCount
     *    The number of bands in the bandpass filter
     *  \param coefficent
     *    The T60-based absorbtion coefficent
     *  \param outputScale
     *    Scales the output by a given value (used to ensure the output
     *    doesn't clip)
     *  \param input
     *    A copy of the input sample data
     *  \param output
     *    A reference to the output data that will be mixed together
     */
    void add_bandpass_reverb_filter(const uint16_t &delay
        , const float &frequency, const size_t &bandCount
        , const float &coefficent, const float &outputScale
        , const CArray<float> &input, CArray<float> &output);
    void generate_scene_filter();
    void clear();

    bool open = false;

    Vec2 relativePos;
    Vec2 relativeSize;
    Vec2 relativeScalar;
    unsigned currentSamplingRate;

    std::string name;

    CArray<Equalizer> filters;
    AudioRayVec audioRayVec;
    ObjectVec objects;
};

