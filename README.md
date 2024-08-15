# beastapp

## Project Description

**beastapp** is a versatile and scalable C++ application built on top of the Boost.Beast library. It provides a robust framework for handling HTTP and WebSocket communications with a focus on security, using SSL/TLS for encrypted connections. Designed for high-performance networking, **beastapp** is ideal for building server-side applications that require secure and efficient client-server communication.

The application is organized into distinct modules that handle various aspects of the networking stack, from managing SSL certificates to handling HTTP requests and WebSocket sessions. **beastapp's** modular architecture makes it easy to extend and customize, whether you're building a web server, an API service, or a real-time communication platform.
## Getting Started

### Prerequisites

- c++17 or later
- Boost Libraries (specifically, boost_system, boost_filesystem, boost_thread, boost_asio, boost_beast)
- OpenSSL (provides SSL/TLS support)
- libcrypto (OpenSSL cryptographic library)
- make (for building the project)
- g++ or clang++ (C++ compiler)

### Installation

1. Clone the repository:
    ```bash
    git clone https://github.com/diyooor/beastapp.git
    ```

### Running the Application

To run the application, use the following command:

```bash
make run
```

Directory Structure

The project is organized into the following main directories:

### app/

This directory contains the core application logic. It includes the `Application` class, which manages the high-level operations of the application, such as sending HTTP requests and interacting with the network layer.

### net/

The `net` directory houses all networking-related components. This includes the HTTP client (`Client` class), server setup (`server` class), session management, SSL/TLS certificates, and any utilities related to network operations. The client is used for making HTTP requests, while the server handles incoming connections and requests.

### log/

The `log` directory is responsible for logging functionality. It contains the logger implementation, which is used across the application to record events, errors, and debug information. The logger is configurable and can output logs to both the console and files, depending on the setup.

## Output

```bash
./bin/main 0.0.0.0 8080 www 2
2024-08-14 21:31:34 - DEBUG [MainLogger] Parsing command line arguments.
2024-08-14 21:31:34 - DEBUG [MainLogger] Initializing io_context.
2024-08-14 21:31:34 - DEBUG [MainLogger] Initializing SSL context.
2024-08-14 21:31:34 - INFO [ClientLogger] Client initialized.
2024-08-14 21:31:34 - DEBUG [MainLogger] Starting the WebSocket listener.
2024-08-14 21:31:34 - DEBUG [WebSocketListenerLogger] WebSocket listener started on 0.0.0.0:8081
2024-08-14 21:31:34 - DEBUG [WebSocketListenerLogger] WebSocket listener is ready to accept connections.
2024-08-14 21:31:34 - DEBUG [MainLogger] Starting the HTTP server.
2024-08-14 21:31:34 - DEBUG [MainLogger] Running io_context in the main thread.
2024-08-14 21:31:34 - DEBUG [MainLogger] Running io_context in a thread.
2024-08-14 21:31:52 - DEBUG [WebSocketListenerLogger] Connection accepted. Starting WebSocket session.
2024-08-14 21:31:52 - DEBUG [WebSocketSessionLogger] WebSocket session created.
2024-08-14 21:31:52 - DEBUG [WebSocketSessionLogger] Starting WebSocket session.
2024-08-14 21:31:52 - DEBUG [WebSocketSessionLogger] SSL handshake successful.
2024-08-14 21:31:52 - DEBUG [WebSocketSessionLogger] WebSocket handshake successful. Waiting for messages.
2024-08-14 21:31:55 - DEBUG [WebSocketSessionLogger] Received WebSocket message: test
2024-08-14 21:31:55 - DEBUG [WebSocketSessionLogger] WebSocket message sent successfully. Clearing buffer and reading next message.
2024-08-14 21:31:56 - DEBUG [WebSocketSessionLogger] Received WebSocket message: test
2024-08-14 21:31:56 - DEBUG [WebSocketSessionLogger] WebSocket message sent successfully. Clearing buffer and reading next message.
2024-08-14 21:31:57 - DEBUG [WebSocketSessionLogger] Received WebSocket message: test
2024-08-14 21:31:57 - DEBUG [WebSocketSessionLogger] WebSocket message sent successfully. Clearing buffer and reading next message.
2024-08-14 21:31:58 - ERROR [WebSocketSessionLogger] Error reading WebSocket message: stream truncated
```

## Contributing


## License


## Acknowledgments


