# Enderman : A Web application framework built upon <u><a href="https://github.com/ansh1406/Http-Server">ansh1406/Http-Server</a></u>

## Overview

Enderman is a web application framework built on top of my own HTTP server library ([Http-Server](https://github.com/ansh1406/Http-Server)). It provides a higher-level abstraction for building web applications, making it easier to handle routing, middleware, and other common web development tasks. Enderman is designed to be lightweight and efficient, allowing developers to create fast and responsive web applications with ease.

Enderman is designed to be modular and extensible, allowing developers to easily add new features and functionality as needed. It provides a simple and intuitive API for handling HTTP requests and responses, making it easy to build complex web applications.

This is a completely educational project for myself. I wanted to try this and than so I decided to build Enderman but despite that I've still tried my best to make it as good as possible. I hope you find it useful and enjoy using it as much as I enjoyed building it. And yes Enderman is highly inspired by Express.js so you'll see many similar function names. (But I can't be sure if they are implemented similarly but still most of them behave the same). So if I do some unintetional bragging in the following part, please ignore it. I'm trying to imitate other libraries.

> [WARNING] Enderman uses my own http server library which is only desinged for POSIX complient linux system thus making Enderman only compatible with linux system.

On the final note, If you're also a student or a new developer who is looking for easy to start projects in C++ and are interested in contributing in this or my http library project, please contact me at [anshswaroop14@gmail.com](mailto:anshswaroop14@gmail.com). We can discuss further. I'm highly optimistic about this project.

## Features

- **Routing**: Enderman provides a powerful routing system that allows you to define routes and handle HTTP requests with ease. You can define routes for different HTTP methods (GET, POST, etc.) and specify route parameters for dynamic routing. It also supports wildcards.

- **Middleware**: Enderman supports middleware functions that can be used to modify the request and response objects before they are handled by the route handlers. This allows you to add functionality such as authentication, logging, and more.

- **Simplistic**: Enderman is designed to be simple and easy to use. But still being in C++ there are some parts that are a bit less intuitive.

- **Extensible**: Enderman is designed to be extensible, allowing you to easily add new features and functionality as needed. You can create your own middleware functions, route handlers, and more.

- **Json Support**: Enderman provides built-in support for handling JSON data, making it easy to work with APIs and other data formats.

- **Performance**: Enderman works directly on the OS unlike many other web frameworks which are built on top of other languages or runtimes, thus making it faster and more efficient.

## Installation

> [WARNING] Enderman will only compatible with linux system.

To install Enderman, you can clone the repository and build it using CMake. Here are the steps to get started:

1. Clone the repository:
   ```bash
   git clone --recurse-submodules https://github.com/ansh1406/Enderman.git
   ```
2. Navigate to the project directory:
   ```bash
   cd Enderman
   ```
3. Create a build directory and navigate to it:
   ```bash
   mkdir build
   cd build
   ```
4. Build the project using CMake:
   ```bash
   cmake ..
   make
   ```
5. After the build process is complete, you can find the Enderman library in the `build/lib` directory. You can link this library in your C++ projects to use Enderman.

6. In the top level CMakeLists.txt file, you can select optional build for modules.

## API Documentation

### Enderman

The API documentation is yet to be written but inline comments in the public hearest explain the functions in detail.
Here is a brief overview-

- `Enderman`: The main class of the framework, which is used to create an instance of the application and define routes and middleware.
- `Request`: Represents an Enderman request, containing information such as the request method, URL, headers, and body.
- `Response`: Represents an Enderman response, allowing you to set the status code, headers, and body of the response.
- `Body`: Abstract factory class for creating different types of response bodies (e.g., text, JSON, etc.). You can create your own custom body types by inheriting from this class.

> All classes and functions are declared in the `enderman` namespace.

### Json module

The JSON module is created to provide support for handling JSON data in Enderman. But it is completely independent of enderman can can be used in any C++ project. It provides a simple and intuitive API for parsing and generating JSON data.

Here is a brief overview of the JSON module:

- `Object`: Class representing a JSON object, which can be a primitive, an array or a nested object.
- `parse_json`: Function to parse a JSON string and return an `Object` representing the JSON data.
- `stringify`: Function to convert an `Object` representing JSON data into a C++ std::string.

> All classes and functions in the JSON module are declared in the `enderman_json` namespace.

## Usage

### Header files

- Core enderman header files are located in the [`include/enderman`](include/enderman) directory.
- You only need to include the `enderman.hpp` header files it is designed as a single include.
- For using plugins you need to include the header files of the plugin. They are located in the `include/enderman` directory of each plugin's respective directory.

### Library

- The Enderman library `libenderman.a` is located in the `build` directory after building the project.
- If you want to use a module you can select the optional build for that module in the top level CMakeLists.txt file it will built `libenderman.a` with the module support.
- If you have built the project without the support of a specific module, you can still use that module by building it separately and linking against it in your project.
- Some plugins may depend on specific modules or other plugins.

### Building your project

- Build enderman as a static library.
- Add the path to the Enderman header files to your project's include path. For example, if you have cloned the Enderman repository to `/path/to/Enderman`, you would add `/path/to/Enderman/include` to your include path. This would create a nice way to include the header files in your project, for example:
  ```cpp
  #include <enderman/enderman.hpp>
  ```
- For each plugin you want to use add the plugins include folder to your include path. For example, if you want to use the standard_bodies plugin, you would add `/path/to/Enderman/plugins/standard_bodies/include` to your include path. Then in your source code you can include the header file of the plugin, like this:
  ```cpp
  #include <enderman/standard_bodies/standard_bodies.hpp>
  ```
- While compiling/building your project, link against the Enderman library. For example, if you are using g++, you would add `-L/path/to/Enderman/build -lenderman` to your compiler flags to link against the Enderman library.

- You may also need to link against module specific libraries if you had built enderman without the support of that module.

- Some plugins may also depend on specific modules or other plugins. Like `json` plugin depends on `json` module so you need to link against the `json` module library if you want to use the `json` plugin. Or build enderman with `BUIILD_MODULE_JSON` option enabled.

### Example Program

```cpp
#include <enderman/enderman.hpp>
#include <enderman/standard_bodies/standard_bodies.hpp>

using namespace enderman;

int main() {
    Enderman app;

    app.get("/hello", [](Request& req, Response& res) {
        auto response_body = std::make_shared<TextBody>(TextBody());
        response_body->text = "Hello, World!";
        res.set_body(response_body).send();
    });

    app.listen(3000);

    return 0;
}
```

You can build this program like this:

```bash
g++ -I/path/to/Enderman/include -L/path/to/Enderman/build -lenderman -o my_app my_app.cpp
```

This will create a web server which listens on port 3000 and responds with "Hello, World!" when a GET request is made to the "/hello" route.
