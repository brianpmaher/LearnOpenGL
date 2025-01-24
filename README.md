# Learn OpenGL

This repository is a collection of my own example applications and notes for learning OpenGL.

I've picked up a little bit of graphics programming in the past, but this year, I'm setting out formally learn more about graphics programming and get into some more advanced graphics programming techniques.

## Index

- [Resources](#resources)
- [Project Setup](#project-setup)
- [Notes](#notes)
  - [Getting Started](#getting-started)

## Resources

- [OpenGL](https://www.opengl.org/)
- [Learn OpenGL](https://learnopengl.com/)
- [GLFW](https://www.glfw.org/documentation.html)

## Project Setup

This project has a few requirements that are up to you to source:

- A desktop platform that supports OpenGL 3.3
- A C++ compiler with C++20 language standard support
- [CMake](https://cmake.org/)

Each example is a separate application. See [CmakeLists.txt](./CMakeLists.txt) for a list of example applications.

This project was developed and tested to work on Windows. It likely doesn't work on other platforms without some additional work.

## Notes

### Getting Started

- OpenGL is a graphics API specification developed and maintained by [The Khronos Group](https://www.khronos.org/)
- Graphics card manufacturers implement the API
- Bugs in the API are often resolved with graphics driver updates
- Core and immediate mode profiles
  - Immediate mode is older, fixed-function interface
  - Immediate mode is deprecated
  - Using core-profile will cause immediate mode functions to throw errors
- OpenGL 3.3+ uses more modern approach
- OpenGL version is up to 4.6, but only adds functionality after 3.3 -- uses same more modern workflow
- Supports graphics card extensions to allow graphics card manufacturers to extend functionality beyond API spec
- Large state machine
  - State is referred to as **context**
  - Changing render mode often means changing context
- OpenGL **objects** an OpenGL abstraction that represent a collection of options
  - Objects can be thought of as a C struct
  - Interacting with objects often means binding an object to a location in the OpenGL context, then running operations on that context location
- First thing we need to do to render graphics is to create an OpenGL context and an app window to draw it in
  - OpenGL doesn't care how you open the window, but doing so is platform-specific
  - Many libraries that abstract this away, for this tutorial, we'll use GLFW
- Locations of OpenGL functions are not known at compile-time and need to be queried at run-time
  - Retrieving these functions are OS-specific and can be tedious
  - GLAD is a popular library for retrieving OpenGL functions
  - See [HelloWindow](./Source/Example/2_HelloWindow/Main.cpp) for window and OpenGL function loading
- OpenGL setup steps:
  - Create a window
  - Load GL functions
- With GLFW, must call `glfwMakeContextCurrent(window)` in order to load OpenGL functions and set the window as the main context on the current thread.
- Viewport
  - OpenGL needs to know the size of the rendering window with `glViewport(x, y, width, height)`
  - We could render to full size of window or subset of window
- Behind the scenes OpenGL uses data specified via `glViewport` to transform the 2D coordinates into screen coords
  - Example: Window (800, 600) and coord point of (-0.5, 0.5) would be mapped to (200, 450)
  - **Processed coordinates in OpenGL are between -1.0 and 1.0**
- When window resizes, we want to update `glViewport` to match new window size
  - For retina displays, width and height will end upw tih higher than original input values
- In order for application to keep window running, we create a render loop that keeps running until we tell it to stop
- `glfwPollEvents` checks if any window events are triggered (such as keyboard or mouse movement, or closing window)
- `glfwSwapBuffers` will swap the color buffer
  - Color buffer is a large 2D buffer that contains color values for each pixel in the window to render to the screen
- **Double buffering** - When an application draws in a single buffer, the resulting image may display flickering issues.
  - Resulting output image is not drawn in an instant, but drawn pixel by pixel
  - To circumvent, windowing apps apply a double buffer for rendering.
  - **front buffer** contains the final output image
  - **back buffer** current rendering buffer
  - When all rendering commands are finished, we **swap** the back buffer to the front buffer.
- We can clear the color buffer using `glClear(bufferBits)` and set the clear color with `glClearColor(r, g, b, a)`
