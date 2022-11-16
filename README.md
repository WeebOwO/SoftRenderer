# SoftRenderer
 A simple renderer without graphic api like Vulcan, Directx and OpenGL. 
## Dependency
SDL2 for create window and draw point, you need to link that youself and change setting in CMakeLists.txt in line4 and line5.
``` cmake
 include_directories(dependency/SDL2-2.24.0/include/)
 link_directories(dependency/SDL2-2.24.0/lib/x86)
```

## Build
you need cmake and c++ compiler. And having those tools. You can run commands below to build this project.

```
 mkdir build
 cd build
 cmake ..
```

## Running example
<div>
<img src="asset/mylight.gif">
</div>
