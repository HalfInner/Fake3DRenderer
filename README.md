# Fake 3D Renderer

Primitive & Naive 3D engine based on OpenGL.  
![demo of application](readme_resources/Demo.gif)

## Support 

 * Provides OpenGl Abstraction 
 * Primitive camera actions: moving and zooming
 * Phong lighting 
 
## Getting Started

The project was divided into two parts:
 1. General requirements
    * Any library - OpenGL/glfw3/glew32/stb
    * Any programming language - C++
    * Result: statically linked 32/64 bit Windows application
    * Camera support
    * Lighting support
 2. Camera support
    * Moving up-down left-right forward-backward
    * Zoom In & Zoom Out
    * Rotate via axises X, Y, Z
    * I/O - Keyboard 
 3. Lighting Support
    * Light Reflection Algorithm - Phong
    * Analyse Reflection
        * Smooth surface
        * Rough surface
        * Intensity
 4. Official Report - attachment is not project at the moment
    * Manual - how to run, keys
    * Project description
    * Algorithms analysis
        * Prediction vs Reality 
            * Algorithms 
            * Pool Ball
        * Effect of set ups border values   
 5. Additional
    * Movable sun - analysis reflection behaviour
    * Resizable window


### Prerequisites

* C++17
* Cmake / Makefile
* free time

### Installing
Windows:
```
./configure.sh
./build.sh
```

Linux/Mac: _TBD_

## Running the tests

No tests =O

## Steering

Camera:
```
 * W S A D  Forward/Backward/Left/Right
 * Q E      Up/Down
 * Z C      Zoom In/Out
 * R T      Rotate Left/Right
 * F G      Rotate Up/Down
 * B V      Pivot Left/Right
```
Light & Reflection:
```
 * - =      Soften/Harden light source
 * [ ]      Soften/Harden light ambient
 * ; ‘      Soften/Harden light diffuse
 * . /      Soften/Harden light reflection
 * M ,      Soften/Harden light focus
 * Enter    Start/Stop rotary of Sun
```
Others:
```
 * Space    Overlay/Take off the 'PoolBall8.jpg'
 * Escape   Close application
```

## Authors

* **Kajetan Brzuszczak** 

## Thanks

1. [Learn OpenGL Site](https://learnopengl.com) - great place with knowledge 
2. Course Materials 

## License

This project is licensed under the MIT License - see the [LICENSE.md](LICENSE.md) file for details

## Acknowledgments

* Computer Graphics & Visualization - University of Technology in Warsaw
