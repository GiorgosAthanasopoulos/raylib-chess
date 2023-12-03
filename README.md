# Raylib Chess

A simple implementation of chess in raylib.

## NOTE

Although the project source files have .cpp extensions the project is mainly
written in plain c, apart from the use of vectors and some algorithms,
so the structure is more c-like (which would exlpain no oop definition of the
game logic or other c++ norms).

# USAGE - LINUX

- ```chmod +x bootstrap.sh```
- ```./bootstrap.sh```

# USAGE - Windows (Just use linux, and make your life ez...Anyway here goes...)

- Lets call the directory that we will use to clone the repositories in ```$PROJECTS```.
- ```cd $PROJECTS```
- Start by cloning [raylib-chess](https://github.com/GiorgosAthanasopoulos/raylib-chess) (the actual game): ```git clone https://github.com/giorgosathanasopoulos/raylib-chess```
- We need a compiler - lets use [W64Devkit](https://github.com/skeeto/w64devkit/):
  - Download the w64devkit zip from the [W64Devkit Releases](https://github.com/skeeto/w64devkit/releases) page.
  - Extract the folder in $PROJECTS.
  - So far the $PROJECTS directory should contain: ```raylib-chess``` and ```w64devkit``` sub-directories.
- Finally we need the raylib library. To make everything easy we will use a raylib game template that comes with makefiles out of the box (and that will automatically download raylib):
  - ```git clone https://github.com/raylib-extras/game-premake```
- That's all for the dependencies. Our ```$PROJECTS``` directory should contain ```raylib-chess```, ```w64devkit``` and ```game-premake``` sub-directories.
Lets configure and build/run our project:
-  Run the executable ```$PROJECTS/w64devkit/w64devkit/w64devkit.exe```
In the new terminal run:
- ```cd $PROJECTS/game-premake```
- ```premake-mingw.bat``` (This will download raylib for us - along with creating a makefile for our project).
Now lets copy raylib-chess in the game-premake src directory:
- ```rm ./game/src/*```
- ```cp ../raylib-chess/src/* ./game/src/```
- ```cp ../raylib-chess/_build/game-premake.make ./_build/```
- ```make```(You should see the raylib library being compiled),
- ```mkdir assets```
- ```cp ../raylib-chess/assets/* ./assets/```
- ```_bin/Debug/game-premake.exe```
