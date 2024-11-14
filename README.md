# Serialization

A simple C++ library to convert data types to and from bytes so that they can be transmitted over the network or stored on a file. It supports conversion to/from big endian and little endian. This library can also be used to parse various binary formats such as BMP, JPG, etc.

## How to use?

This is a header only library, so it does not require additional steps, just copy `serialization.h` from `include/` directory and use it in your project.

## Running tests
Install Meson, and a backend (such as Ninja) and run the following commands:
```
$ meson wrap install gtest
$ meson setup -Db_sanitize=address -Ddevelopment=true -Denable-tests=true -Db_lundef=false --reconfigure builddir
$ cd builddir
$ meson test
```
