
#include <vector>

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

// Used for file diaglogue from mlabbe
#include <nfd.h>

#include "object.h"
#include "scene.h"
#include "button.h"

#include "unittests.h"

using namespace std;

void output_file(Scene &scene, WaveFile &wave)
{
  scene.apply_filter_to_wave(wave);
  wave.output_to_file("output_test");
}

int main()
{
  sf::RenderWindow window = sf::RenderWindow(sf::VideoMode({800, 600})
    , "amrs");
  window.setFramerateLimit(144);

  Vec2 scenePos = {20.f, 20.f};
  Vec2 sceneSize = {500.f, 500.f};

  Button::set_font("static/Roboto-Black.ttf");

  vector<Object *> ui;
  ui.push_back(new Object(scenePos, sceneSize));
  ui.back()->set_color(sf::Color(200, 200, 200));

  Scene scene("testscene1", scenePos, sceneSize);

  // TEST: INPUT OUTPUT FOR WAVE
  //
  //test_wave_input_output("pluck");

  // TEST: SIMPLE FILTER ON WAVE
  //
  //test_wave_with_simple_filter("pluck");


  // TEST: Using pluck to test wave manipulation in scene
  WaveFile wave("pluck");

  ui.push_back(new Button("Select Scene", {610.f, 20.f}, {100.f, 20.f}
        , [&scene] 
        {
          nfdchar_t *outPath = NULL;
          nfdresult_t result = NFD_OpenDialog(NULL, INPUT_DIR, &outPath);

          if(result == NFD_OKAY)
          {
            static_cast<void>(Logger(Logger::L_MSG, "User selected new Wave file"));
            scene.open_scene(outPath, true);
          }
          else if(result != NFD_CANCEL)
          {
            Logger(Logger::L_MSG, "Error occured in wave file section");
          }

          free(outPath);
        }));
  ui.push_back(new Button("Select Wave", {610.f, 100.f}, {100.f, 20.f}
        , [&wave] 
        {
          nfdchar_t *outPath = NULL;
          nfdresult_t result = NFD_OpenDialog(NULL, INPUT_DIR, &outPath);

          if(result == NFD_OKAY)
          {
            static_cast<void>(Logger(Logger::L_MSG, "User selected new Wave file"));
            wave.open_file(outPath, true);
          }
          else if(result != NFD_CANCEL)
          {
            Logger(Logger::L_MSG, "Error occured in wave file section");
          }

          free(outPath);
        }));
  ui.push_back(new Button("Generate Output", {610.f, 140.f}, {100.f, 20.f}
        , [&scene, &wave] 
        {
          scene.apply_filter_to_wave(wave);
          wave.output_to_file(wave.get_name() + "_out");
        }));
  
  while (window.isOpen())
  { 
    while (const std::optional event = window.pollEvent())
    {
      if (event->is<sf::Event::Closed>())
      {
        window.close(); 
      }
    }

    window.clear(sf::Color(237, 231, 225));

    for(Object *obj : ui)
    {
      obj->draw(window);
    }

    scene.draw(window);

    window.display();
  }
}
