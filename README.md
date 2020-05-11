# C++ JSON examples

This bundles a json library (producing and parsing) along with some examples for how to use it. `main.cpp` contains all of the examples along with comments.

## The contents

_json_: all source files

_json/include_: all headers

_io_: helper for file io

## Compiling the examples

Run `make all`, then run the compiled binary with `./a.out`. You should see all of the example output.

## Using the library in your own project

Copy the `json` subdirectory into your project and make sure that `parser.cpp` and `printer.cpp` are compiled. Take a look at the included _Makefile_ or _CMakeLists.txt_. Then just include the desired headers from `json/include` and use them.

All json definitions are contained in the `JSON` namespace, so you need to either prefix with `JSON::` or use `using namespace JSON` in your class or method.

## License

All of the code in this repository is licensed under the MIT license (included) except for `json/include/utf8.h` which is licensed under the Boost Software License 1.0 and part of [this project](https://github.com/nemtrif/utfcpp). License [here](https://github.com/nemtrif/utfcpp/blob/master/LICENSE).