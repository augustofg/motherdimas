# MotherDimas

No explanations needed.

## How to build

Dependencies:
* C++14 capable compiler;
* cmake;
* sdl2 library (Ubuntu packages: libsdl2-dev libsdl2-ttf-dev libsdl2-mixer-dev libsdl2-image-dev).

First, make sure you have initialized all git submodules:
```bash
$ git submodule update --init --recursive
```

### Building with cmake:

Release build:
```bash
$ mkdir -p build/release
$ cd build/release
$ cmake ../.. -DCMAKE_BUILD_TYPE=Release
$ cmake --build .
```

Debug build:
```bash
$ mkdir -p build/debug
$ cd build/debug
$ cmake ../.. -DCMAKE_BUILD_TYPE=Debug
$ cmake --build .
```

### Parallel builds

If your machine has multiple processors cores you can accelerate the build passing the ```-j<num_of_jobs>``` to ```make``` (assuming you aren't using MSVC). Example:
```bash
$ cmake --build . -- -j4 # create up to 4 parallel jobs
```

## Running

To run the game you need to inform the path to the assets directory. Supposing your work directory is under ```build/release``` you can specify the assets path in this way:

```bash
$ ./motherdimas --assets-path ../../assets/
```

## Acknowledges

The sound effects ```assets/construction-jackhammer-excavator.wav``` and ```assets/rocket-launch.wav``` are provided by the user InspectorJ and primeval_polypod respectively on [Freesound](https://freesound.org).
