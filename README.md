This is 3D Model Viewer. 

[youtube sample](https://youtu.be/ZfcPf8NxYHo)
<img src="https://github.com/green-21/ModelViewer/assets/75987694/12723781-db7f-4753-b753-fc3292c00063"  width="600">


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