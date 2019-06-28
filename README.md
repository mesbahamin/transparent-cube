# Transparent Cube


## Build

### Compile on Linux

- Install clang and glfw3 with your system's package manager.
- `make run` to compile and run the release build.


### Compile on Windows

- Install and set up clang. The sanest set of instructions I've found is
  [here][1].
- Download the latest 64-bit Windows pre-compiled binary package from [here][2].
- Unzip it and copy the `include` and `lib-vc2019` folders into
  `lib/glfw_windows/`. The folder structure should look like this:

```
lib/glfw_windows/
|-- include
|   +-- GLFW
|       |-- glfw3.h
|       +-- glfw3native.h
+-- lib-vc2019
    |-- glfw3.dll
    |-- glfw3dll.lib
    +-- glfw3.lib
```

- `.\build.bat && .\out\release\transparent-cube.exe` to compile and run the
  release build.


## References

### WebAssembly and WebGL

- [Notes on working with C and WebAssembly][3]
- [Compiling C to WebAssembly Without Emscripten][4]


### Approximating Trigonometric Functions

- [Handmade Hero Day 400: Introduction to Function Approximation with Andrew Bromage][5]
- [Fast and accurate sine/cosine - DevMaster.net Forums][6]


[1]: https://gist.github.com/InNoHurryToCode/955d63db0d79699fed63fe18eeebf17e
[2]: https://www.glfw.org/download.html
[3]: https://aransentin.github.io/cwasm/
[4]: https://dassur.ma/things/c-to-webassembly/
[5]: https://guide.handmadehero.org/code/day440/
[6]: https://web.archive.org/web/20080228213915/http://www.devmaster.net/forums/showthread.php?t=5784
