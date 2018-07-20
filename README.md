## Dependencies

Cinder 0.9.1 uses VC 2013 (version 12.0).

https://libcinder.org/download - This library is the OpenGL implementation used for rendering.  The version for this project is 0.9.1.  Build the project libraries using the solution file in the ```proj/vc2013``` directory.

https://pocoproject.org/download/ - This library provides a variety of utilities for HTTPS, file access and JSON processing.

```
# Update the OPENSSL_DIR buildwin.cmd to refer to the installation directory for OpenSSL.
# Remove 'samples' from the build_vs120.cmd file to speed up compilation.
# Change 'shared' to 'all' in the build_vs120.cmd file to create static link libraries.
# Comment out MySQL and Postgres from the 'components' file, unless you have them installed.
$ .\build_vs120.cmd
```

https://pocoproject.org/docs/00200-GettingStarted.html - Follow the instructions for installing the OpenSSL code and building the libraries from POCO. 

https://www.boost.org/ - C++ boost libraries for use by POCO (Cinder depends on boost version 1.60)

```
$ .\bootstrap
$ .\b2 toolset=msvc-12.0
```

## Building

Create an empty VS2013 project using the TinderBox.exe in the ```tools\TinderBox-Win``` directory in the Cinder distribution.  This will create an empty project that you can use to add source files too.

Add the preprocessor definition _CRT_SECURE_NO_WARNINGS

Update the library and include directories in the project properties to point to your POCO installation.

## Environment Variables

The system relies on two environment variables, the GOOGLE_PLACES_API_KEY and DATA_ROOT.  The api key is used to help identify headquarter addresses for placement on the Geographic screen.  The data root variable specifies the location of the data JSON files.

```
$ export GOOGLE_PLACES_API_KEY=****
$ export DATA_ROOT=****
```

