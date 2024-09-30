# OpenGL Model Viewer

OpenGL Model Viewer is a simple model viewer that uses OpenGL to render 3D models. It is written in C++ and uses the [GLFW](https://www.glfw.org/) library for window management and the [GLAD](https://glad.dav1d.de/) 
library for OpenGL function loading. The model viewer is capable of loading models in the [Wavefront OBJ](https://en.wikipedia.org/wiki/Wavefront_.obj_file) format and rendering them using OpenGL by utilizing the assimp library.

## Getting Started

### Prerequisites

The following libraries are required to build the project:

| Library | Info                                                                     | Link                                    |
|---------|--------------------------------------------------------------------------|-----------------------------------------|
| GLFW    | Lightweight utility library for use with OpenGL                          | [Website](https://github.com/glfw/glfw) |
| glad    | Library for OpenGL function loading                                      | [Website](https://github.com/Dav1dde/glad) |
| glm     | Mathematics library for graphics software based on the OpenGL            | [Website](https://github.com/g-truc/glm) |
| assimp  | Library to load various 3d file formats into a shared, in-memory format. | [Website](https://github.com/assimp/assimp) |
| inih    | Simple .INI file parser                                                  | [Website](https://github.com/benhoyt/inih) |

### Installing
* Clone the repository into the machine
```
git clone https://github.com/DavidNg32/OpenGL-ModelViewer.git
```

* Build the project using CMake
* Run the model viewer executable

### Program Details
* The program reads the configuration file `config.ini` to load the model file and the shader files.
* The program has a simple  camera system that allows the user to move the camera around the model.
* There is a placeholder model available for the user to run in the assets folder.
* If you want a custom model, you can add the obj. file to the assets folder and change the path in the main file.

## Acknowledgments
* [LearnOpenGL](https://learnopengl.com/) for the great and comprehensive tutorials on OpenGL.
