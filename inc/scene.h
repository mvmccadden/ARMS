/*!
 *  \author Manoel McCadden
 *  \date   11-27-25
 *  \file   scene.h
 *
 *  \brief
 *    Interface of object and filter generation from scenes
 */

#include <vector>
#include <string>
#include <SFML/Graphics/RenderWindow.hpp>

#include "arms_math.h"

typedef class AudioRay AudioRay;
typedef class Object Object;
typedef class Filter Filter;
typedef class WaveFile WaveFile;
typedef struct RayGenerationInfo RayGenerationInfo;
typedef struct Vec2 Vec;

using AudioRayVec = std::vector<std::vector<AudioRay *>>;
using ObjectVec = std::vector<Object *>;

class Scene
{
  public:
    Scene(const std::string &fileName, const Vec2 &topLeftPos
        , const Vec2 &size, const bool &ignoreInputDir = false);
    ~Scene();

    void open_scene(const std::string &fileName
        , const bool &ignoreInputDir = false);

    void apply_filter_to_wave(WaveFile &wave);

    std::string get_name() const;
    
    void draw(sf::RenderWindow &window);
  private:
    void generate_scene_filter();
    void clear();

    Vec2 relativePos;
    Vec2 relativeSize;
    unsigned currentSamplingRate;

    std::string name;

    Filter *filter;
    AudioRayVec audioRayVec;
    ObjectVec objects;
};

