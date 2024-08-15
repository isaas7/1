# Beastapp

## Project Description

**Beastapp** 

## Getting Started

### Prerequisites

- [List of prerequisites]

### Installation

1. Clone the repository:
    ```bash
    git clone https://github.com/diyooor/beastapp.git
    ```
2. [Installation steps]

### Usage

[Instructions on how to use the application]

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

[Instructions for contributing]

## License

[License information]

## Acknowledgments

[Credits and acknowledgments]

