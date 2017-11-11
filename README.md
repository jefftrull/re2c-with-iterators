# RE2C with Iterators

[RE2C](http://re2c.org/index.html) is a popular open-source lexer generator for C and C++, with a flexible interface. As a code generator, it needs to be configured to access the user's data stream. The examples on their website are fairly C-oriented, so I build this repo to illustrate usage with C++ iterators as the input source.

## To Build
The usual CMake flow. You will need Boost and RE2C; it will use the system defaults or you can supply a path to the `re2c` executable:

~~~
mkdir build
cd build
cmake -DRE2C=/path/to/re2c ..
make
~~~
