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

### Standard Types

- Int
- Double
- String
- Vec2 (double)
- Vec3 (int)
- Array
    - IntArray
    - DoubleArray
    - StringArray
    - Vec2Array

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
- Material Color -> a Vec3 
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

### Source (Speaker)

A *required* container that defines where the audio source will be located and
how rays will disperse from it.

#### Options

- Source **Position** -> a Vec2
- Source **Size** -> a Vec2
- Source **Direction** -> an Int
    - The direction the "Source Speaker" will point
- Source **Cone** -> an Int
    - The cone in which the "Source Speaker" will shoot out its inital rays
- Number of Ray **Checks** -> an Int
    - Number of times a ray can bounce before it is considered "dead"
- Number of **Rays** -> an Int
    - The number of rays that will be evenly dispersed within the source cone

**Example**
```
Source
{
  Position
  {
    Vec2 = 100, 100
  }
  Size
  {
    Vec2 = 30, 30
  }
  Direction
  {
    Int = 270
  }
  Cone
  {
    Int = 30
  }
  Checks
  {
    Int = 8
  }
  Rays
  {
    Int = 50
  }
}
```

### Listener (Microphone)

A *required* container that defines where the audio listener will be located and
what type of polar pattern it will use.

#### Options

- Listener **Postion** -> a Vec2
- Listener **Size** -> a Vec2
- Listener **Direction** -> an Int
    - The "forward" direction of the polar pattern (defaults to 0)
- Listener **Polar Pattern** -> a String
    - The polar pattern the Listener or Microphone will use to capture sound
      (defaults to Omni)

##### Polar Patterns

- Omni ("Omni")
- SubCardioid ("Sub")
- Cardioid ("Cardioid")
- SuperCardioid ("Super")
- HyperCardioid ("Hyper")
- BiDirectional ("Bi")

**Example**
```
Listener
{
  Position
  {
    Vec2 = 300, 300
  }
  Size
  {
    Vec2 = 30, 30
  }
  Direction
  {
    Int = 90
  }
  Pattern
  {
    String = Hyper
  }
}
```

### Barrier

An *optional* container that defines where a object that can block audio 
is located within the room and what acoustic properties it holds

#### Options

- Barrier **Material** -> a String
- Barrier Position -> a Vec2
- Barrier Size -> a Vec2

##### Default Materials

- Wood
- Rubber
- Wall

**Example**
```
Barrier
{
  String = Wood 
  Position
  {
    Vec2 = 200, 250
  }
  Size
  {
    Vec2 = 50, 100
  }
}
```

## Known Issues 
- Some Wave files may not work due to how their are internally formatted 

