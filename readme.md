# Eve!!!
A small client-server application that receives HTTP requests via a REST API. The requests should contain e-mail messages that are then sent to an SMTP server. This was a developed as part of an interview exercise.

## Requirements
- [Boost 1.73.0](https://github.com/boostorg)
- [nlohmann_json 3.2.0](https://github.com/nlohmann/json)

## Build and Compilation
Built with [CMake 3.19](https://cmake.org/) and compiled with gcc 9.3.0 in Ubuntu 20 LTS

## How to use
**Run the server:**

`./Eve-Server <address> <port> <smtp_address> <smtp_port>`

The API requests are explained on the index page of the server. The REST API uri is `/jsonApi`

**Run the cpp client:**

For the client there are different configurations depending on the request:

```
./Eve-Client GET <address> <port> <email_id>
./Eve-Client GET_STATUS <address> <port> <email_id>
./Eve-Client POST <address> <port> <to_email> <from_email> <subject> <body>
./Eve-Client DELETE <address> <port> <email_id>
```

- GET: Returns the data (to/from emails, subject and body) of a saved e-mail.
- GET_STATUS: Returns the status of a saved e-mail (Sending, Sent, Failed, Gave up).
- POST: Saves an e-mail to the server, which sends it via SMTP. The client will pool the server every 10 seconds for the status until, either the E-mail is sent successfully, or the server gives up after too many failed attempts.
- DELETE: Removes an e-mail from the server.

**Run the Javascript/HTML client:**

Because of CORS the security flag needs to be disabled if the code is to be run in a browser. The interface contains 4 buttons to switch to the different types of requests. The buttons set the correct input boxes for the request. After filling them up, all you need to do is press the **send** button. Do not add `http://` to the Server input box as that is done automatically. The functionality is the same as the cpp client. It will also continuously pool the server for the e-mail status after sending.

Another thing to note about this client is that, because I am using Javascript modules, the `index.html` can not be run directly from the file system. This is because modules require the MIME of the script file to be a Javascript MIME type. The file system won't provide a MIME so the loading of the script will fail. To run this client I installed the [http-server](https://www.npmjs.com/package/http-server) package available via npm.

## Assumptions/Decisions
- The SMTP client is very simple and does not require authentication.
- It's a small, simple server. The requests are handled asynchronously but it is mostly single-threaded (except when sending an e-mail via SMTP. Then it does so in a separate thread). For large scale it would be a good idea to build a pool of threads to serve several clients, for example.
- The SMTP client sends the simplest form of messages. That means it does not use commands like EHLO and assumes no authentication is necessary. It also only supports a sender and a recipient. To clarify, the structure of supported messages is similar to [this example](https://en.wikipedia.org/wiki/Simple_Mail_Transfer_Protocol#SMTP_transport_example) found in wikipedia.
- To simplify things the server storage is kept in memory only, nothing is saved to a file or a database. As soon as the server is shutdown: *poof*.
- The HTTP parser is taken from Boost's example. In hindsight it might've been better to use a library but I didn't want to have too many dependencies and I thought that was not the most important part of the exercise. It works for the simple requests this uses but it's far from perfect. A better approach would be necessary on a production server.
- I used clang-format to format the code because I want consistency while not worrying too much about it. The settings are based on Google's C++ guidelines.
- To simplify things I logged messages directly to `std::cout` and `std:cerr`. For a production server a more robust logging system should be used.
- The client is pretty simple and blocking until a reply comes. I decided to not use much time formatting the json replies from the server as I believe that's a bit out of the scope of this exercise.

## FAQ
- Why did you name this Eve?
  - I like to personify the software I build, it's fun! :)
- Why Boost?
  - Mostly to use the asio library as it simplifies socket connection.
- Why nlohmaan_json?
  - I didn't want to write a json parser as well so I looked up a library for this. This one seemed good enough for the job!

Thank you for checking out the Eve!!!
