# ARMS (Acoustic Room Modeling Simulation)

A simple acoustic room model built on SFML that uses physically modeled rays 
to demonstrate how rooms attenuate sound.

## Scripting Guide

### Info

A container for the simulations base settings include the following parameters.

- raycount (the number of rays being sent from the source)
- maxchecks (the number of bounces a ray can have before it is considered 'dead')
- conesize (the size of the cone the rays are being cast from, from the source)
- direction (the direciton of the cone that will case the rays, from the source)

#### Coefficent

A sub-container within **Info** that can be used to create custom barrier
types.

- Name (the name of the material)
- Factor (the coefficent absortion factor)
- Color (the color of the material that will be used to visualize it on screen)

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
