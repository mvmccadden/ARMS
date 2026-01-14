
#include <vector>

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

// Used for file diaglogue from mlabbe
#include <nfd.h>

#include "colors.h"
#include "generator.h"
#include "object.h"
#include "scene.h"
#include "textbox.h"
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
  /*
   *  Setup defaults for scene
   */
  sf::RenderWindow window = sf::RenderWindow(sf::VideoMode({1400, 1050})
    , "arms");
  window.setFramerateLimit(144);

  Vec2 sceneBkPos = {20.f, 20.f};
  Vec2 sceneBkSize = DEFAULT_ROOM_SIZE + Vec2{10.f, 10.f};
  Vec2 scenePos = {25.f, 25.f};
  Vec2 sceneSize = DEFAULT_ROOM_SIZE;
  Vec2 buttonContainerPos = {1100.f, 100.f};
  Vec2 buttonContainerSize = {250.f, 270.f};
  Vec2 buttonContainerPadding = {25.f, 25.f};
  Vec2 buttonContainerItemStart = buttonContainerPos + buttonContainerPadding;
  Vec2 buttonSize = {200.f, 26.f};
  float buttonContainerYSectionOffset = 45.f;
  float buttonContainerYItemOffset = 35.f;

  // Init UI related objects and UI font
  init_font();
  vector<Object *> ui;
  ui.push_back(new Object(sceneBkPos, sceneBkSize));
  ui.back()->set_color(uiBackground);
  Object *p_drawScene = new Object(scenePos, sceneSize);
  ui.push_back(p_drawScene);
  ui.back()->set_color(sceneBackground);
  ui.push_back(new Object(buttonContainerPos, buttonContainerSize));
  ui.back()->set_color(uiBackground);

  Scene scene(scenePos, sceneSize, {1.f, 1.f});

  // TEST: INPUT OUTPUT FOR WAVE
  //
  //test_wave_input_output("pluck");

  // TEST: SIMPLE FILTER ON WAVE
  //
  //test_wave_with_simple_filter("pluck");

  WaveFile wave;

  /*
   *  Setup scene UI buttons and text
   */
  TextBox *sceneTitle = new TextBox("Scene: "
      , buttonContainerItemStart, buttonSize);

  ui.push_back(sceneTitle);
  // Button for selecting room scene
  ui.push_back(new Button("Select Scene"
        , {buttonContainerItemStart.x
          , buttonContainerItemStart.y 
            + buttonContainerYItemOffset}
        , buttonSize
        , [&scene, &sceneTitle, &p_drawScene] 
        {
          nfdchar_t *outPath = NULL;
          nfdresult_t result = NFD_OpenDialog("txt", INPUT_DIR, &outPath);

          if(result == NFD_OKAY)
          {
            static_cast<void>(Logger(Logger::L_MSG
                  , "User selected new Scene file"));

            string filePath = outPath;
            size_t lastSlash = filePath.find_last_of("\\/") + 1;
            if(lastSlash != string::npos)
            {
              filePath = filePath.substr(lastSlash);
            }

            size_t dotPos = filePath.rfind('.');
            if(dotPos != string::npos)
           {
              filePath = filePath.substr(0, dotPos);
            }

            sceneTitle->set_title("Scene: " + filePath);
            Vec2 newSceneSize = scene.open_scene(outPath, true);
            p_drawScene->set_size(newSceneSize);
          }
          else if(result != NFD_CANCEL)
          {
            Logger(Logger::L_MSG, "Error occured in wave file section");
          }

          if(outPath) free(outPath);
        }));

  TextBox *waveTitle= new TextBox("Wav: "
      , {buttonContainerItemStart.x
        , buttonContainerItemStart.y 
          + buttonContainerYItemOffset + buttonContainerYSectionOffset}
      , buttonSize);
  ui.push_back(waveTitle);
  /*
   *  Button for selecting .wav file for input
   */
  ui.push_back(new Button("Select Wav"
      , {buttonContainerItemStart.x
        , buttonContainerItemStart.y 
          + 2 * buttonContainerYItemOffset + buttonContainerYSectionOffset}
        , buttonSize
        , [&wave, &waveTitle] 
        {
          nfdchar_t *outPath = NULL;
          nfdresult_t result = NFD_OpenDialog("wav", INPUT_DIR, &outPath);

          if(result == NFD_OKAY)
          {
            static_cast<void>(Logger(Logger::L_MSG
                  , "User selected new Wave file"));

            string filePath = outPath;
            size_t lastSlash = filePath.find_last_of("\\/") + 1;
            if(lastSlash != string::npos)
            {
              filePath = filePath.substr(lastSlash);
            }

            size_t dotPos = filePath.rfind('.');
            if(dotPos != string::npos)
            {
              filePath = filePath.substr(0, dotPos);
            }

            waveTitle->set_title("Wav: " + filePath);
            wave.open_file(outPath, true);
          }
          else if(result != NFD_CANCEL)
          {
            Logger(Logger::L_MSG, "Error occured in wave file section");
          }

          if(outPath) free(outPath);
        }));

  /*
   *  Button for generating an output.wav based on room and input.wav
   */
  ui.push_back(new Button("Generate Ray Output"
      , {buttonContainerItemStart.x
        , buttonContainerItemStart.y 
          + 2 * buttonContainerYItemOffset + 2 * buttonContainerYSectionOffset}
        , buttonSize
        , [&scene, &wave] 
        {
          if(!scene.is_open())
          {
            static_cast<void>(Logger(Logger::L_WRN
                  , "No valid Scene selected"));
            return;
          }
          if(!wave.is_open())
          {
            static_cast<void>(Logger(Logger::L_WRN
                  , "No valid Wav file selected"));
            return;
          }

          nfdchar_t *outPath = NULL;
          nfdresult_t result = NFD_SaveDialog("wav", OUTPUT_DIR, &outPath);

          if(result == NFD_OKAY)
          {
            WaveFile output(wave);
            static_cast<void>(Logger(Logger::L_MSG
                  , "User selected new Wave file"));
            scene.apply_filter_to_wave(output);
            output.output_to_file(outPath);
          }
          else if(result != NFD_CANCEL)
          {
            Logger(Logger::L_MSG, "Error occured in wave file section");
          }

          if(outPath) free(outPath);
        }));
  ui.push_back(new Button("Generate Sabine Output"
      , {buttonContainerItemStart.x
        , buttonContainerItemStart.y 
          + 3 * buttonContainerYItemOffset + 2 * buttonContainerYSectionOffset}
        , buttonSize
        , [&scene, &wave] 
        {
          if(!scene.is_open())
          {
            static_cast<void>(Logger(Logger::L_WRN
                  , "No valid Scene selected"));
            return;
          }
          if(!wave.is_open())
          {
            static_cast<void>(Logger(Logger::L_WRN
                  , "No valid Wav file selected"));
            return;
          }

          nfdchar_t *outPath = NULL;
          nfdresult_t result = NFD_SaveDialog("wav", OUTPUT_DIR, &outPath);

          if(result == NFD_OKAY)
          {
            WaveFile output(wave);
            static_cast<void>(Logger(Logger::L_MSG
                  , "User selected new Wave file"));
            scene.apply_t60_to_wave(output);
            output.output_to_file(outPath);
          }
          else if(result != NFD_CANCEL)
          {
            Logger(Logger::L_MSG, "Error occured in wave file section");
          }

          if(outPath) free(outPath);
        }));
  
  /*!
   *  Basic drawing of ui objects and scene objects
   */
  while (window.isOpen())
  { 
    while (const std::optional event = window.pollEvent())
    {
      if (event->is<sf::Event::Closed>())
      {
        window.close(); 
      }
    }

    window.clear(backgroundColor);

    for(Object *obj : ui)
    {
      obj->draw(window);
    }

    scene.draw(window);

    window.display();
  }
}
