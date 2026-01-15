/*!
 *  \author Manoel McCadden
 *  \date   11-27-25
 *  \file   generator.h
 *
 *  \brief
 *    Implementation file for generating and calculating data
 */

#include "generator.h"

#include <cmath>
#include <string>

#include "source.h"
#include "listener2.h"
#include "barrier.h"

#include "audioray.h"

#include "arms_math.h"
#include "helper.h"

using namespace std;

const size_t NULL_LINES = 99;
const CArray<Vec2> DEFAULT_AMP;

struct ListenerData
{
  string pattern;
  float angle;
};

struct SourceData
{
  float direction;
  float cone;
  int checks;
  int rays;
};

Vec4 get_room_size(DataMap *dataMap)
{
  for(DataMap::DataMapIterator it = dataMap->get_children_begin()
      ; it != dataMap->get_children_end(); ++it)
  {
    if((*it)->get_name() == "Room")
    {
      for(DataMap::DataMapIterator childIt = (*it)->get_children_begin()
          ; childIt != (*it)->get_children_end(); ++childIt)
      {
        if((*childIt)->get_name() == "Size")
        {
          Vec2 size = *(*childIt)->get_casted_data<Vec2>();

          Vec2 scaleVec = DEFAULT_ROOM_SIZE / size;
          Vec2 sceneScalar = (scaleVec.x < scaleVec.y)
            ? Vec2{scaleVec.x, scaleVec.x * size.y / size.x } 
              : Vec2{scaleVec.y * size.x / size.y, scaleVec.y};

          Vec2 scaledScene = (scaleVec.x < scaleVec.y)
            ? size * scaleVec.x : size * scaleVec.y;

          return Vec4{scaledScene.x, scaledScene.y
            , sceneScalar.x, sceneScalar.y};
        }
      }
    }
  }

  return Vec4{DEFAULT_ROOM_SIZE.x, DEFAULT_ROOM_SIZE.y, 1.f, 1.f};
}

array<Vec2, 2> get_object_data(DataMap::DataMapIterator it)
{
  array<Vec2, 2> objData = {Vec2{0.f, 0.f}, Vec2{0.f, 0.f}};

  for(DataMap::DataMapIterator childIt = (*it)->get_children_begin()
        ; childIt != (*it)->get_children_end(); ++childIt)
  {
    if((*childIt)->get_name() == "Position")
    {
      objData[0] = *(*childIt)->get_casted_data<Vec2>();
    }
    else if((*childIt)->get_name() == "Size")
    {
      objData[1] = *(*childIt)->get_casted_data<Vec2>();
    }
  }

  return objData;
}

ListenerData get_listener_data(DataMap::DataMapIterator it)
{
  ListenerData data = {"Omni", 0.f};

  for(DataMap::DataMapIterator childIt = (*it)->get_children_begin()
        ; childIt != (*it)->get_children_end(); ++childIt)
  {
    if((*childIt)->get_name() == "Pattern")
    {
      data.pattern = *(*childIt)->get_casted_data<string>();
    }
    else if((*childIt)->get_name() == "Direction")
    {
      data.angle = *(*childIt)->get_casted_data<int>();
    }
  }

  return data;
}

SourceData get_source_data(DataMap::DataMapIterator it)
{
  SourceData data = {0.f, 30.f, 10, 30};

  for(DataMap::DataMapIterator childIt = (*it)->get_children_begin()
        ; childIt != (*it)->get_children_end(); ++childIt)
  {
    if((*childIt)->get_name() == "Cone")
    {
      data.cone = *(*childIt)->get_casted_data<int>();
    }
    else if((*childIt)->get_name() == "Direction")
    {
      data.direction = *(*childIt)->get_casted_data<int>();
    }
    else if((*childIt)->get_name() == "Checks")
    {
      data.checks = *(*childIt)->get_casted_data<int>();
    }
    else if((*childIt)->get_name() == "Rays")
    {
      data.rays = *(*childIt)->get_casted_data<int>();
    }
  }

  return data;
}

Barrier::EQCoefficents create_custom_coefficents(DataMap::DataMapIterator it)
{
  Barrier::EQCoefficents coefficent = {Barrier::INVALID_COEFFICENT_VALUE
    , sf::Color(0, 0, 0), ""};

  for(DataMap::DataMapIterator childIt = (*it)->get_children_begin()
        ; childIt != (*it)->get_children_end(); ++childIt)
  {
    // TODO: Make this handle actual array values
    if((*childIt)->get_name() == "Array")
    {
      // Get the custom coefficents
      CQueue<Vec2> *queue = (*childIt)->get_casted_data<CQueue<Vec2>>();
      // queueSize is gotten before looping to ensure it isn't updated in loop
      // every iteration and in turn causes the loop to iterate to few times
      size_t queueSize = queue->size();
      // Resize the coefficent array to match the queue size
      coefficent.frequencyCoefficents.resize(queueSize);
      for(size_t i = 0 ; i < queueSize; ++i)
      {
        coefficent.frequencyCoefficents[i] = queue->pop();
        Logger(Logger::L_WRN, "Custom coefficent value " 
            + to_string(coefficent.frequencyCoefficents[i].x)
            + " read as: " 
            + to_string(coefficent.frequencyCoefficents[i].y));
      }
    }
    else if((*childIt)->get_name() == "Color")
    {
      Vec3 vec3 = *(*childIt)->get_casted_data<Vec3>();
      coefficent.color = sf::Color(vec3.r, vec3.g, vec3.b);
    }
  }

  coefficent.name = *(*it)->get_casted_data<string>();
  static_cast<void>(Logger(Logger::L_WRN, "Custom coefficent name read as: " 
        + coefficent.name));

  if(coefficent.name == "")
  {
    Logger(Logger::L_WRN, "Custom coefficent was created with invalid data");
  }

  return coefficent;
}

vector<Object *> convert_DataMap_to_Object(DataMap *dataMap
    , const Vec2 &posOffset, const Vec2 &scalar)
{
  vector<Object *> objVec;

  if(dataMap->get_name() != "root")
  {
    return objVec;
  }
  
  for(DataMap::DataMapIterator it = dataMap->get_children_begin()
      ; it != dataMap->get_children_end(); ++it)
  {
    if((*it)->get_name() == "Coefficent")
    {
      Barrier::set_custom_coefficent(Barrier::get_next_free_custom_coefficent()
          , create_custom_coefficents(it));
    }
    else if((*it)->get_name() == "Source")
    {
      array<Vec2, 2> objData = get_object_data(it);
      SourceData data = get_source_data(it);

      objVec.push_back(new Source(objData[0] * scalar + posOffset 
            , objData[1] * scalar, data.direction, data.cone
            , data.checks, data.rays));
    }
    else if((*it)->get_name() == "Listener")
    {
      array<Vec2, 2> objData = get_object_data(it);
      ListenerData data = get_listener_data(it);

      objVec.push_back(new Listener(objData[0] * scalar + posOffset 
            , objData[1] * scalar, data.angle, data.pattern));
    }
    else if((*it)->get_name() == "Barrier")
    {
      string type = *(*it)->get_casted_data<string>();
      array<Vec2, 2> objData = get_object_data(it);

      objVec.push_back(new Barrier(objData[0] * scalar + posOffset 
            , objData[1] * scalar, type));
    }
  }

  return objVec;
}

Vec2 get_object_center(Object *obj)
{
  Vec2 position = obj->get_position();
  Vec2 size = obj->get_size();
  return position + size / 2.f;
}

/*!
 *  Detect if a collision occured on the calculated trajectory of the line.
 *
 *  TODO: Have a cone for the listener for collision detection
 *  in parallel maybe move the cone for src to the source object
 *  
 *  \param objVec
 *    A vector of all objects in the scene
 *  \param rayBegin
 *    A position of where the ray begins
 *  \param rayEnd
 *    A reference to the end position of the ray which will be updated to
 *    represent the collision point if a collision occurs
 *  
 *  \returns
 *    Returns a struct of collision info
 */
const CollisionInfo detect_collisions(vector<Object *> &objVec
    , AudioRay *ray)
{
  CollisionInfo info;
  float intersectionDistance = -1.f;
  Vec2 rayBegin = ray->get_posA();
  Vec2 rayEnd = ray->get_posB();
  Vec2 newRayEnd(0.f, 0.f);

  for(Object *obj : objVec)
  {
    // NOTE: Currently ignoring source for collisions as rays start in middle of
    // source box, SOON NULL_LINES WILL BE USED MAYBE
    if(obj->get_type_name() == "Source")
    {
      continue;
    }

    Vec2 objPos = obj->get_position();
    Vec2 objSize = obj->get_size();
    std::array<Vec2, 4> objLines = 
    {
      Vec2{obj->get_position()},
      Vec2{objPos.x + objSize.x, objPos.y},
      Vec2{objPos.x + objSize.x, objPos.y + objSize.y},
      Vec2{objPos.x, objPos.y + objSize.y}
    };
    
    // Line-Line collision done for each wall of the barrier
    for(int i = 0; i < 4; ++i)
    {
      // DO NOT CHECK PARENT AT REFLECTION LINE AS THIS WILL 
      // CAUSE COLLISION ERRORS
      if(ray->get_parent() == obj && ray->get_parent_line() == i)
      {
        continue;
      }

      int nextPoint = (i + 1) % 4;
      float denominator = 
        (
          (objLines[nextPoint].y - objLines[i].y) * (rayEnd.x - rayBegin.x) 
          - (objLines[nextPoint].x - objLines[i].x) * (rayEnd.y - rayBegin.y)
        );
      
      // Ignore parallel lines
      if(denominator == 0)
      {
        continue;
      }

      // Normalized points of intersection
      float uA = 
        (
          (objLines[nextPoint].x - objLines[i].x) * (rayBegin.y - objLines[i].y) 
          - (objLines[nextPoint].y - objLines[i].y) * (rayBegin.x - objLines[i].x)
        ) / denominator;
      float uB = 
        (
          (rayEnd.x - rayBegin.x) * (rayBegin.y - objLines[i].y) 
          - (rayEnd.y - rayBegin.y) * (rayBegin.x - objLines[i].x)
        ) / denominator;

      if(uA >= 0 && uA <= 1 && uB >= 0 && uB <= 1)
      {
        // Calculate intersection
        Vec2 intersectionPos = rayBegin + ((rayEnd - rayBegin) * uA);
        float distance = (intersectionPos - rayBegin).magnitude();

        // Only update if closer intersection
        if(intersectionDistance > 0 && distance >= intersectionDistance)
        {
          continue;
        }
      
        intersectionDistance = distance;
        newRayEnd = intersectionPos;
        info.collision = true;
        info.parent = obj;
        info.parentLine = i;
        info.lineBegin = objLines[i];
        info.lineEnd = objLines[nextPoint];

      }
    }

    // Check if parent exists and then check if a listener.
    // If it is exit (THIS IS WHAT WE ARE WAITING FOR!!!)
    if(info.parent && info.parent->get_type_name() == "Listener")
    {
      static_cast<void>(Logger(Logger::L_MSG, "Listener Hit!"));
      float gain = dynamic_cast<Listener*>(info.parent)->get_directional_gain(
          {rayBegin.x - newRayEnd.x, rayBegin.y - newRayEnd.y});
      ray->scale_amp(gain);
      for(size_t i = 0; i < ray->get_amp().size(); ++i)
      {
        Logger(Logger::L_MSG, "Listener gain " 
              + to_string(ray->get_amp().at(i).x)
              + "Hz: " 
              + to_string(ray->get_amp().at(i).y));
      }
      break;
    }
  }

  if(!info.collision)
  {
    return info;
  }

  // Log Collision
  string collisionMsg = "Collision detected at: ( "
    + std::to_string(newRayEnd.x) + " , "
    + std::to_string(newRayEnd.y) + " )";
  //static_cast<void>(Logger(Logger::L_MSG, collisionMsg));
  
  ray->set_posB(newRayEnd);
  return info;
}

AudioRay *resolve_collision(AudioRay *ray, const CollisionInfo &info
    , const Vec2 &scalar)
{
  Vec2 posA = ray->get_posA();
  Vec2 posB = ray->get_posB();
  CArray<Vec2> amp = ray->get_amp();

  // Override amp with new added barrier amp if parent is a barrier
  // NOTE: This currently can only be a barrier as sources are ignored in
  // detection and listeners are handled before resolution seperatly
  amp = ray->add_amps(info.parent->get_absortion_coefficent());
  
  Vec2 incidentVec = posB - posA;

  // Transform the incident Vec into physical space based on scene size
  Vec2 phyIncident = incidentVec / scalar;

  Vec2 lineDirection = info.lineEnd / scalar - info.lineBegin / scalar;
  Vec2 normalVec(-lineDirection.y, lineDirection.x);
  normalVec.normalize();

  Vec2 reflectedVec = phyIncident 
    - normalVec * 2.f * phyIncident.dot(normalVec);
  // Scale the reflection into the scene
  Vec2 scaledReflection = reflectedVec * scalar;
  scaledReflection.normalize();
  Vec2 posC = posB + scaledReflection * DEFAULT_RAY_DISTANCE;

  return new AudioRay(info.parent, info.parentLine, amp, posB, posC);
}

vector<vector<AudioRay *>> generate_inital_audio_rays(Object *parent
    , const Vec2 &srcPos)
{
  Source *source = dynamic_cast<Source*>(parent);

  const float AR_TWOPI = static_cast<float>(8.0 * atan(1));
  // Convert values from degrees to radians
  const float coneSize = static_cast<float>(source->get_cone()) / 360.f 
    * AR_TWOPI;
  // Inverse of info.direction as the y-axis is flipped
  const float direction = static_cast<float>(-source->get_direction()) / 360.f 
    * AR_TWOPI;
  const float degreeIncrement = coneSize 
    / static_cast<float>(source->get_rays());

  vector<vector<AudioRay *>> returnVec;

  float currentDegree = direction - coneSize / 2.f;
  for(int i = 0; i < source->get_rays(); ++i)
  {
    Vec2 endPos = Vec2{cos(currentDegree), sin(currentDegree)} 
      * DEFAULT_RAY_DISTANCE;
    currentDegree += degreeIncrement;
    vector<AudioRay *> newVec;
    newVec.push_back(new AudioRay(parent, NULL_LINES, DEFAULT_AMP, srcPos
          , endPos));
    returnVec.push_back(newVec);
  }

  return returnVec;
}

/*!
 *  \depreacted 
 *    This funciton is deporcated and now incorporated into 
 *    the Scene class
 *  
 *
 *  Calculates the T60 time (time for sound to decay by 60dB).
 *
 *  A = Surface Area of Absorbtion Objects * Absorbtion Coefficents
 *  V = Volume of the room
 *
 *  T60 = ((0.161) * V) / A
 *
 *  NOTE: As we are doing a 2D model 
 *  we are considering the Z-axis to always be 1
 *
 *  \param objVec
 *    A vector of objects with absorbtion factor
 *
 *  \returns
 *    The T60 time in seconds for each band
 */
CArray<Vec2> calculate_t60_time(vector<Object *> &objVec)
{
  float absorbtionSurfaceArea = 0.f;
  float roomVolume = 0.f;
  // Using an audio ray to track absorbtion as it has all the built in
  // functionality needed
  AudioRay absorbtionRay(nullptr, 0, DEFAULT_AMP, {0.f, 0.f}, {0.f, 0.f});

  for(Object *obj : objVec)
  {
    absorbtionRay.add_amps(obj->get_absortion_coefficent());

    Vec2 size = obj->get_size();
    if(obj->get_type_name() == "Barrier")
    {
      Barrier *barrier = dynamic_cast<Barrier *>(obj);
      if(barrier && barrier->get_type_data() == Barrier::C_WALL)
      {
        roomVolume = size.w * size.h;
      }

      absorbtionSurfaceArea += size.w * 2.f;
      absorbtionSurfaceArea += size.h * 2.f;
    }
  }
  
  /*
  static_cast<void>(Logger(Logger::L_MSG, "Absorbtion Average: " 
      + to_string(absorbtionAverage)));
  */
  static_cast<void>(Logger(Logger::L_MSG, "Absorbtion Surface Area: " 
      + to_string(absorbtionSurfaceArea)));
  static_cast<void>(Logger(Logger::L_MSG, "Room Volume: " 
      + to_string(roomVolume)));
  CArray<Vec2> returnVec(absorbtionRay.get_amp());
  for(size_t i = 0; i < returnVec.size(); ++i)
  {
    returnVec[i].y = (0.161f * roomVolume) 
      / (absorbtionSurfaceArea * returnVec[i].y);
  }
  return returnVec;
}

/*!
 *  Calculates the average peak amplitude of each ray to find the average amount
 *  of gain that the listener recieves
 */
float calculate_listener_peak_amplitude(vector<vector<AudioRay *>> &raySet)
{
  float attenuation = 0.f;

  for(vector<AudioRay *> &rays : raySet)
  {
    attenuation += rays.back()->get_amp_average();
  }

  return attenuation;
}

vector<vector<AudioRay *>> generate_audio_rays_from_scene(
    vector<Object *> &objVec, const Vec2 &relativePos
    , const Vec2 &relativeSize, const Vec2 &scalar)
{
  // Set defaults and get the source object
  Object *parent;
  Vec2 srcPos = {0.f, 0.f};
  vector<vector<AudioRay *>> rayVec;
  vector<vector<AudioRay *>> returnVec;

  for(Object *obj : objVec)
  {
    if(obj->get_type_name() == "Source")
    {
      parent = obj;
      srcPos = get_object_center(obj);
      break;
    }
  }

  Source *source = dynamic_cast<Source*>(parent);

  if(!parent || !source)
  {
    static_cast<void>(Logger(Logger::L_ERR, string("No valid Source object ")
        + " found in given audio vector during scene audio ray generation!"));
    return returnVec;
  }

  int maxChecks = source->get_checks();

  // Add a wall for collision detection
  Barrier wall(relativePos, relativeSize, "wall");
  objVec.push_back(&wall);

  // Generate the inital waves in the TODO: given cone
  rayVec = generate_inital_audio_rays(parent, srcPos);

  float listenerAmp = 0.f;

  // Generate subsequent rays based off collisions
  for(vector<AudioRay *> _rayVec : rayVec)
  {
    // First check for the inital collision
    AudioRay *ray = _rayVec.front();
    float amp = map_range_to(ray->get_amp_average(), 0.f, 1.f, 60.f, 160.f);
    /*
    Logger(Logger::L_ERR, "Average AMP: " + to_string(ray->get_amp_average()));
    */
    ray->set_color(sf::Color(0.f, amp, 0.f, amp));
    CollisionInfo collisionInfo = detect_collisions(objVec, ray); 
    // Then loop until either the collision max is hit meaning we probably 
    // can't hit the listener or we hit the listener
    for(int i = 0; i < maxChecks && collisionInfo.collision
        && (collisionInfo.parent 
          && !(collisionInfo.parent->get_type_name() == "Listener")); ++i)
    {
      AudioRay *newRay = resolve_collision(ray, collisionInfo, scalar);
      amp = map_range_to(newRay->get_amp_average(), 0.f, 1.f, 60.f, 160.f);
      newRay->set_color(sf::Color(0.f, amp, 0.f, amp));
      ray = newRay;
      _rayVec.push_back(ray);
      collisionInfo = detect_collisions(objVec, ray);
    }

    if(_rayVec.back()->get_amp_average() < 0.f || _rayVec.back()->get_amp_average() > 1.f)
    {
      Logger(Logger::L_ERR, "INVALID VEC AMP");
    }

    // TODO: Instead of adding to a new vec on successful hit lets remove from
    // vec
    if(collisionInfo.parent && _rayVec.back()->get_amp_average() > 0.f
        && collisionInfo.parent->get_type_name() == "Listener")
    {
      returnVec.push_back(_rayVec);
    }
    else 
    {
      for(AudioRay * _ray : _rayVec)
      {
        delete _ray;
      }
    }
  }

  // Get smallest vec size and set color to be bolded
  struct SmallestVecSize
  {
    int index;
    int size;
  } smallestVecSize {0, maxChecks};
  int totalSizes = 0.f;

  // Get the louded ray
  struct LoudestRay
  {
    int index;
    float amp;
  } loudestRay {0, 0.f};

  for(int i = 0; i < returnVec.size(); ++i)
  {
    totalSizes += returnVec[i].size();
    if(returnVec[i].size() < smallestVecSize.size)
    {
      smallestVecSize.index = i;
      smallestVecSize.size = returnVec[i].size();
    }
    float amp = returnVec[i].back()->get_amp_average();
    if(loudestRay.amp < amp)
    {
      loudestRay.amp = amp;
      loudestRay.index = i;
    }
  }

  // Loudest and Smallest Ray
  // NOTE: Accounting for case of no rays found and scene being 'invalid'
  if(loudestRay.index == smallestVecSize.index && returnVec.size() > 0)
  {
    for(AudioRay *ray : returnVec[loudestRay.index])
    {
      float amp = map_range_to(ray->get_amp_average(), 0.f, 1.f, 130.f, 230.f);
      ray->set_color(sf::Color(amp, 0.f, amp, amp)); 
    }
  }
  else if(returnVec.size() > 0)
  {
    // Loudest Ray
    for(AudioRay *ray : returnVec[loudestRay.index])
    {
      float amp = map_range_to(ray->get_amp_average(), 0.f, 1.f, 130.f, 230.f);
      ray->set_color(sf::Color(amp, amp, 0.f, amp)); 
    }
  
    // Smallest Ray
    for(AudioRay *ray : returnVec[smallestVecSize.index])
    {
      float amp = map_range_to(ray->get_amp_average(), 0.f, 1.f, 130.f, 230.f);
      ray->set_color(sf::Color(0.f, amp, amp, amp)); 
    }
  }

  static_cast<void>(Logger(Logger::L_MSG
        , "Number of rays that hit the listener: " 
        + to_string(returnVec.size())));

  listenerAmp = calculate_listener_peak_amplitude(returnVec);

  // Remove the added wall since we don't need to draw it
  objVec.pop_back();

  return returnVec;
}
