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
   git clone
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

To use Enderman in your C++ project, you need to include the Enderman header files and link against the Enderman library. Here is a simple example of how to create a basic web application using Enderman:

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

> This will create a web server which listens on port 3000 and responds with "Hello, World!" when a GET request is made to the "/hello" route.