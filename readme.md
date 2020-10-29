# Eve!!!
A small client-server application that receives HTTP requests via a REST API. The requests should contain e-mail messages that are then sent to an SMTP server. This was a developed as part of an interview exercise.

## Requirements
- [Boost 1.73.0](https://github.com/boostorg)
- [nlohmann_json 3.2.0](https://github.com/nlohmann/json)

## Build and Compilation
Built with [CMake 3.19](https://cmake.org/) and compiled with gcc 9.3.0 in Ubuntu 20 LTS

## How to use
To run the server:

`./Eve-Server <address> <port> <smtp_address> <smtp_port>`

The API requests are explained on the index page of the server. The REST API uri is `/jsonApi`

## Assumptions/Decisions
- The SMTP client is very simple and does not require authentication.
- It's a small, simple server. The requests are handled asynchronously but it is mostly single-threaded (except when sending an e-mail via SMTP. Then it does so in a separate thread). For large scale it would be a good idea to build a pool of threads to serve several clients, for example.
- To simplify things the server storage is kept in memory only, nothing is saved to a file or a database. As soon as the server is shutdown: *poof*.
- The HTTP parser is taken from Boost's example. In hindsight it might've been better to use a library but I didn't want to have too many dependencies. It works for the simple requests this uses.
- I used clang-format to format the code because I want consistency while not worrying too much about it. The settings are based on Google's C++ guidelines.
- To simplify things I logged messages directly to std::cout and std:cerr. For a production server a more robbust logging system should be used.

## FAQ
- Why did you name this Eve?
  - I like to personify the software I build, it's fun! :)
- Why Boost?
  - Mostly to use the asio library as it simplifies socket connection.
- Why nlohmaan_json?
  - I didn't want to write a json parser as well so I looked up a library for this. This one seemed good enough for the job!

Thank you for checking out the Eve!!!
