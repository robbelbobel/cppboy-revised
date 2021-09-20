# CPPBOY-revised

## Description

CPPBOY is a simple gameboy emulator written in C++ and is based on the depricated cppboy project. This project was started mainly to learn more about the way computers work. I chose the gameboy since it provides a good balance between difficulty and fun. The project is still very much work in progress and any help is welcome.

## *Prerequisites

To be able to build the CPPBOY-revised project you should have the following libaries installed:

* **SFML 2.5.1 or newer**

Building the CPPBOY-revised executable also requires you to have the latest version of **Make** and **CMake** installed on your computer.

## Build Instructions

Clone the CPPBOY repository

~~~bash
git clone https://github.com/robbelbobel/cppboy-revised
~~~

Navigate to the CPPBOY repository

~~~bash
cd cppboy-revised
~~~

Create a build folder and open it

~~~bash
mkdir build
cd build
~~~~

Initialize the build folder using cmake

~~~bash
cmake ..
~~~

Build the application with make

~~~bash
make
~~~

## Usage

The emulator should be executed in the terminal by using the following command:

~~~bash
./cppboy [ROMPATH]
~~~

*The emulator is unable to boot without a correct ROM path.*
