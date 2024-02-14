# First-Hit Raytracer

## About
Raytracing program that allows user the option to view a scene of rendered objects using a camera. Two different camera types are supported, _Orthographic_ and _Perspective_, to switch between cameras, press `p` while the program is executing. The scene has three different types of lighting: ambient, spectral, and diffuse. A vector of lights is used to determine the total lighting on the scene's objects. It has support for three different types of geometric objects: triangles, spheres, and planes. A vector of objects is used to hold and display these objects using the _intersect()_ method.

## Animations



https://github.com/atduffaut/raytracer/assets/62037622/3d3a3e80-77c5-4781-a29a-30800e0c80f4



https://github.com/atduffaut/raytracer/assets/62037622/7cdc673d-a828-4e4d-b5db-797f1875adc3





## Building and Compilation:

1. Install [MacPorts](https://www.macports.org/install.php)

2. Install glew, glfw, and eigen3 using MacPorts, the following commands:

   `sudo port install glew`

   `sudo port install glfw`

   `sudo port install eigen3`

4. Include `opt/local/include` when compiling, and link `opt/local/lib`
   These are the locations that MacPorts automatically installs libraries

5. Note [.vscode/tasks.json](.vscode/tasks.json) for the full compilation command
