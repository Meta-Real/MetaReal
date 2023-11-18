# MetaReal Compiler version 1.0.0
[MetaReal](https://meta-real.github.io) is a powerful, easy-to-use, and flexible programming language designed for the metaverse.
## Installation
### Setup On Windows
Download and install [MSYS2](https://www.msys2.org). <br>
Install mingw64-gcc, cmake, and make in the MSYS2 mingw64 terminal window.
```
pacman -S gcc
pacman -S cmake
pacman -S make
```
The rest of the steps are identical between different operating systems.
### MetaReal Installation
Go to the project directory and open up the terminal window (MSYS2 mingw64 terminal window in Windows). <br>
Create a directory called `build` and open it.
```
mkdir build
cd build
```
Build the project and make it.
```
cmake ..
make
```
Go back to the project directory and run the MetaReal executable file.
```
./MetaReal
```
