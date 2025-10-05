# C Sqlite Clone

This project was undertaken for my own learning and educational purposes.

## Building the Executable

This project features the use of `CMakePresets.txt`. A feature introduced by Kitware to wrap common configure and build CLI commands into workflows which can be invoked with `cmake --workflow ${presetName}`.

Presets available:
- `ninja-release`
- `ninja-debug`
- `mingw`
- `unix`

Build with:

```bash
cmake --workflow ninja-release
```

Run with:

```bash
./build/ninja/release/c-sqlite-clone database.bin
```

Where `database.bin` is the binary file where the database will dump the serialized memory after exiting with `.exit`.
