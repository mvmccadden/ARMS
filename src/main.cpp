
#include <vector>

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

// Used for file diaglogue from mlabbe
#include <nfd.h>

#include "colors.h"
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
  sf::RenderWindow window = sf::RenderWindow(sf::VideoMode({800, 600})
    , "arms");
  window.setFramerateLimit(144);

  Vec2 scenePos = {20.f, 20.f};
  Vec2 sceneSize = {500.f, 500.f};
  Vec2 buttonContainerPos = {560.f, 20.f};
  Vec2 buttonContainerSize = {200.f, 240.f};

  // Init UI related objects and UI font
  init_font();
  vector<Object *> ui;
  ui.push_back(new Object(scenePos, sceneSize));
  ui.back()->set_color(sceneBackground);
  ui.push_back(new Object(buttonContainerPos, buttonContainerSize));
  ui.back()->set_color(uiBackground);

  Scene scene(scenePos, sceneSize);

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
  TextBox *sceneTitle = new TextBox("Scene: ", {580.f, 40.f}, {160.f, 20.f});
  TextBox *waveTitle= new TextBox("Wav: ", {580.f, 120.f}, {160.f, 20.f});

  ui.push_back(sceneTitle);
  // Button for selecting room scene
  ui.push_back(new Button("Select Scene", {580.f, 70.f}, {160.f, 20.f}
        , [&scene, &sceneTitle] 
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
            scene.open_scene(outPath, true);
          }
          else if(result != NFD_CANCEL)
          {
            Logger(Logger::L_MSG, "Error occured in wave file section");
          }

          if(outPath) free(outPath);
        }));

  ui.push_back(waveTitle);
  /*
   *  Button for selecting .wav file for input
   */
  ui.push_back(new Button("Select Wav", {580.f, 150.f}, {160.f, 20.f}
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
  ui.push_back(new Button("Generate Room Output", {580.f, 190.f}, {160.f, 20.f}
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
  ui.push_back(new Button("Generate T60 Output", {580.f, 220.f}, {160.f, 20.f}
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
