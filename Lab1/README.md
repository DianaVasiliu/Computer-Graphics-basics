# Lab 1

## Installing the required libraries

Download the neccessary files for the following libraries:

1. [FreeGLUT](https://www.transmissionzero.co.uk/software/freeglut-devel/) - for VS, download the MSVC package
2. [GLEW](http://glew.sourceforge.net/)

Extract the archives to a new folder. In this folder, there will be at least 3 subfolders: `bin`, `include`, `lib`.

Navigate to the Visual Studio install directory. It should look like this:

`C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Tools\MSVC\<your_version>`

In here, you should find the 3 subfolders. Copy the files from the extracted folders to the corresponding folders in here.

### `.h` files (`include` folder)

**Important**: the `include` files from the libraries must be copied into `C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Tools\MSVC\<your_version>\include\GL` (keep the `GL` folder). It is important for the includes in our future programs.

### `.dll` files (`bin` folder)

If you have a **32-bit** Windows: copy the files into `C:\Windows\SysWOW64`.

If you have a **64-bit** Windows: copy the files into `C:\Windows\System32`.

### `.lib` files (`lib` folder)

If you have a **32-bit** Windows: copy the files into `C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Tools\MSVC\<your_version>\lib\x86`.

If you have a **64-bit** Windows: copy the files into `C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Tools\MSVC\<your_version>\lib\x64`.

## Creating a new project

Create a new empty project in Visual Studio.

**Note**: For every new project, you must link the required libraries.

`Project -> Properties -> Configuration Properties -> Linker -> Input -> Additional Dependencies`

You must add here `freeglut.lib` and `glew32.lib`. If it doesn't work, try adding the complete paths.

Now, to test if the libraries are linked, run the `00_test_version.cpp` file. It will print the supported versions for OpenGL and GLSL.
