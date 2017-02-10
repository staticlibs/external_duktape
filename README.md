Duktape library build for Staticlibs
====================================

This project is a part of [Staticlibs](http://staticlibs.net/).

This project contains a CMake wrapper for [Duktape library](https://github.com/svaarala/duktape).
Duktape sources [amalgamation](https://github.com/svaarala/duktape-releases) is included in `duktape` directory.

Link to the [API documentation](http://duktape.org/api.html).

How to build
------------

[CMake](http://cmake.org/) is required for building.

To build the library on Windows using Visual Studio 2013 Express run the following commands using
Visual Studio development command prompt 
(`C:\Program Files (x86)\Microsoft Visual Studio 12.0\Common7\Tools\Shortcuts\VS2013 x86 Native Tools Command Prompt`):

    git clone https://github.com/staticlibs/external_duktape.git
    cd external_duktape
    mkdir build
    cd build
    cmake ..
    msbuild external_duktape.sln

See [StaticlibsToolchains](https://github.com/staticlibs/wiki/wiki/StaticlibsToolchains) for 
more information about the toolchain setup and cross-compilation.

License information
-------------------

This project is released under the [Apache License 2.0](http://www.apache.org/licenses/LICENSE-2.0).

Changelog
---------

**2017-02-10**

 * version 1.6.1-1
 * initial public version
