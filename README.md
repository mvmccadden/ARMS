# ARMS (Acoustic Room Modeling Simulation)

A simple acoustic room model built on SFML that uses physically modeled rays 
to demonstrate how rooms attenuate sound.

## Features

- Wav file I/O
- Physical ray modeled reverberance 
- Sabine Formula modeled reverberance
- Custom Scripting System
    - Source
        - Custom ray distribtion
    - Listener
        - Polar patterns
    - Barriers
    - Custom materials
    - Custom room sizes

## Getting Started

### Installation

#### Linux

1. Clone the repo
2. Install dependencies: *cmake*, *gcc*, *g++*
3. From the projects root directory run the following commands to build
    1. Create a new build directory `cmake -B [build directory]`
    2. Create a new build within the directory `cmake --build [build directory]`
4. Run the build `./[build directory]/bin/arms`

### Running Your First Simulation 

1. Press **Select Scene** and select one of the provided test scenes in *input/*
2. Press **Select Wave** and select one of the provided sample .wav files in
   *input/*
3. Generate output with either **Generate Ray Output** or **Generate Sabine
   Output** and save it where you like!

#### Next?

- Try comparing the outputs with each other and input!
- Try using your own .wav files!
- Try creating your own custom rooms!
- Try to simulate a recording setup or even a couple to see what gets the best
  sound!
- Contact me with feature requests and any bugs!
- Have fun!

## Scripting Guide

### Room

An *optional* container defining the room size of a scene in cm. 
The room size will default to 1000cm X 1000cm if this container is not defined 
in your scene.

**Example**
```
Room
{
    Size
    {
        Vec2 = 500, 500
    }
}
```

### Material 

An *optional* container defining a custom material that can be applied to any
barrier in a given scene.

#### Options
- Material Name -> a String
- Material Color -> a Color
- Material Coefficents -> a Vec2Array containing [frequency, absorbtion factor]

**Example**
```
Material
{
    String = Tile
    Color
    {
        Vec3 = 220, 220, 255
    }
    Vec2Array[4]
    {
        Vec2 = 125, 0.5
        Vec2 = 500, 0.3
        Vec2 = 1000, 0.1
        Vec2 = 2000, 0.2
    }
}
```

### Source 

A container that defines where the audio source will be located

- Position (a sub-container containing a Vec2 with its **2D position**)
- Size (a sub-container containing a Vec2 with its **2D Size**)

### Listener 

A container that defines where the audio listenr will be located

- Position (a sub-container containing a Vec2 with its **2D Position**)
- Size (a sub-container containing a Vec2 with its **2D Size**)
- Direction (a sub-container containing an Int with its **Polar Angle**)
- Pattern (a sub-container containing a String with its **Polar Pattern**)

####

### Barrier

A container that defines where a object that can block audio waves is located
within the room and what acoustic properties it holds

- Type (a **Default** or **Custom material** that this barrier will inherit 
  properties of)
- Position (a sub-container containing a Vec2 with its **2D position**)
- Size (a sub-container containing a Vec2 with its **2D size**)

#### Default Material

- Wood
- Rubber
- Wall

## Known Issues 
- Some Wave files may not work due to how their are internally formatted 

