This is 3D Model Viewer. 


## Features
- Load model with albedo(diffuse) texture
  - extension: `glfw`
- Infinity XZ grid
- Orbit and zoom camera
- Transformation rendered model (trnaslation, scaling)

## Build
This project use third party library with [vcpkg](https://github.com/microsoft/vcpkg) (classic mode).
```
vcpkg install assimp:x64-windows
vcpkg install directxtk:x64-windows
vcpkg install directxmath:x64-windows
vcpkg install imgui:x64-windows
vcpkg install stb:x64-windows
```