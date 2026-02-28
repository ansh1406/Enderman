# Enderman : A Web application framework built upon <u><a href="https://github.com/ansh1406/Http-Server">ansh1406/Http-Server</a></u>

## Overview

Enderman is a web application framework built on top of my HTTP server library ([Http-Server](https://github.com/ansh1406/Http-Server)). It provides a higher-level abstraction for building web applications, making it easier to handle routing, middleware, and other common web development tasks. Enderman is lightweight and efficient, allowing developers to create fast and responsive web applications.

Enderman is modular and extensible, so you can add features and functionality as needed. It provides a simple and intuitive API for handling HTTP requests and responses, which makes building complex web applications easier.

This project is educational. I decided to build Enderman to explore web frameworks in C++. I tried to make it as good as possible and hope you find it useful. Enderman is heavily inspired by Express.js, so you may see similar function names and behavior. If anything below sounds like bragging, please ignore it — I'm learning and experimenting.

> [WARNING]
> Enderman depends on my HTTP server library, which is designed for POSIX-compliant Linux systems; therefore, Enderman is only supported on Linux.

On a final note, if you're a student or new developer looking for an easy C++ project and are interested in contributing to this project or my HTTP library, please contact me at [anshswaroop14@gmail.com](mailto:anshswaroop14@gmail.com). I'm happy to discuss contributions.

## Features

- **Routing**: Enderman provides a powerful routing system that allows you to define routes and handle HTTP requests with ease. You can define routes for different HTTP methods (GET, POST, etc.) and specify route parameters for dynamic routing. It also supports wildcards.

- **Middleware**: Enderman supports middleware functions that can be used to modify the request and response objects before they are handled by the route handlers. This allows you to add functionality such as authentication, logging, and more.

- **Simplicity**: Enderman is designed to be simple and easy to use. Because it's written in C++, some parts may be less intuitive compared to higher-level languages.

- **Extensible**: Enderman is extensible, allowing you to add custom middleware, route handlers, and more.

- **JSON Support**: Enderman provides built-in support for handling JSON data, making it easy to work with APIs and other data formats.

- **Performance**: Enderman works close to the OS level, unlike many frameworks built on other runtimes, which can make it faster and more efficient.

## Installation
> [WARNING]
> Enderman is only compatible with Linux.

Prerequisites
- A POSIX-compliant Linux system
- A C++ compiler and CMake (e.g., `g++` and `cmake`)
- The `http` library (Http-Server) installed and discoverable by CMake

Install the `http` dependency first following its README. Then build and install Enderman:

From the project root:

```bash
git clone https://github.com/ansh1406/Enderman.git
cd Enderman
mkdir build && cd build
# Optionally enable plugins at configure time: -DENDERMAN_PLUGIN_JSON=ON
cmake ..
make
sudo make install
```

Notes
- To enable optional modules at configure time, pass CMake definitions such as `-DENDERMAN_PLUGIN_JSON=ON` when running `cmake ..`, or enable modules in the top-level `CMakeLists.txt` (search for options named `ENDERMAN*`).
- For a Release build, run: `cmake -DCMAKE_BUILD_TYPE=Release ..`

If you want to build with JSON plugin build and install the JSON module first.

- To build the JSON module:

```bash
cd modules/json
mkdir build && cd build
cmake ..
make
sudo make install
```

After installing a module (for example the JSON module), re-run CMake for the main project with the corresponding `-D` option (for example `-DENDERMAN_PLUGIN_JSON=ON`) and rebuild Enderman so the main library picks up the installed module.

## API Documentation

### Enderman

The API documentation is yet to be written, but inline comments in the public headers explain the functions in detail. Here is a brief overview:

- `Enderman`: The main class of the framework, used to create an application instance and define routes and middleware.
- `Request`: Represents a request, containing the method, URL, headers, and body.
- `Response`: Represents a response, allowing you to set the status code, headers, and body.
- `Body`: Abstract factory class for creating different response body types (e.g., text, JSON). You can create custom body types by inheriting from this class.

> All classes and functions are declared in the `enderman` namespace.

### JSON module

The JSON module provides support for handling JSON data in Enderman, but it is independent and can be used in any C++ project. It provides a simple API for parsing and generating JSON data.

Here is a brief overview of the JSON module:

- `Object`: Class representing a JSON value (primitive, array, or object).
- `parse_json`: Parses a JSON string and returns an `Object` representing the data.
- `stringify`: Converts an `Object` representing JSON data into an `std::string`.

> All classes and functions in the JSON module are declared in the `enderman_json` namespace.

## Usage

### Header files

- Core Enderman headers are located in the [`include/enderman`](include/enderman) directory.
- You only need to include `enderman.hpp`; it is designed as a single-header include.
- To use plugins, include the plugin headers in their respective `include/enderman` directories.

### Library

- The Enderman static library `libenderman.a` is located in the `build` directory after building the project.
- To include a module in the build, enable the optional module build in the top-level `CMakeLists.txt`; the resulting `libenderman.a` will include the module support.
- If you build Enderman without a specific module, you can still build that module separately and link against it in your project.
- Some plugins may depend on specific modules or other plugins.

### Building your project

- Ensure Enderman is built and installed at your desired path. By default, it installs system-wide using the OS default paths.

- Then you can use CMake to build your project by linking against the Enderman library with:

```cmake
find_package(enderman REQUIRED)
target_link_libraries(your_target PRIVATE enderman::enderman)
```

- Alternatively you can build directly using `g++`:
```bash
g++ your_file.cpp -lenderman -lhttp -o your_file
```

To use this, both Enderman and the `http` library must be installed on the system, or you must provide the library and include paths using `-L` and `-I`.

### Example Program

```cpp
#include <enderman/enderman.hpp>
#include <enderman/standard_bodies/standard_bodies.hpp>

using namespace enderman;

int main() {
    Enderman app;

    app.get("/hello", [](Request& req, Response& res) {
        TextBody::set_text(res, "Hello, World!").send();
    });

    app.listen(3000);

    return 0;
}
```

You can build this program with CMake by adding these lines to your `CMakeLists.txt`:

```cmake
find_package(enderman REQUIRED)
target_link_libraries(your_target PRIVATE enderman::enderman)
```

This will create a web server that listens on port 3000 and responds with "Hello, World!" when a GET request is made to the "/hello" route.
