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
2024-08-14 15:35:54 - INFO [ClientLogger] Client initialized.
2024-08-14 15:35:54 - INFO [ApplicationLogger] Application initialized.
2024-08-14 15:36:04 - INFO [MainLogger] Executing GET request.
2024-08-14 15:36:04 - INFO [ApplicationLogger] Sending GET request to localhost:8080/
2024-08-14 15:36:04 - DEBUG [ClientLogger] Performing GET request to localhost:8080/
2024-08-14 15:36:04 - DEBUG [ClientLogger] Resolving localhost:8080
2024-08-14 15:36:04 - DEBUG [ClientLogger] Connecting to resolved address.
2024-08-14 15:36:04 - DEBUG [ClientLogger] Performing SSL handshake.
2024-08-14 15:36:04 - DEBUG [session_logger] Session created.
2024-08-14 15:36:04 - DEBUG [session_logger] Running session.
2024-08-14 15:36:04 - DEBUG [session_logger] Starting SSL handshake.
2024-08-14 15:36:04 - DEBUG [session_logger] Handshake successful.
2024-08-14 15:36:04 - DEBUG [session_logger] Reading request.
2024-08-14 15:36:04 - DEBUG [ClientLogger] Sending HTTP request.
2024-08-14 15:36:04 - DEBUG [ClientLogger] Receiving HTTP response.
2024-08-14 15:36:04 - DEBUG [session_logger] Request received successfully.
2024-08-14 15:36:04 - DEBUG [session_logger] Sending response.
2024-08-14 15:36:04 - DEBUG [session_logger] Response sent successfully.
2024-08-14 15:36:04 - INFO [ClientLogger] Received response: <html>
    <body>
        helloworld
    </body>
</html>

2024-08-14 15:36:04 - DEBUG [session_logger] Reading request.
2024-08-14 15:36:04 - INFO [ApplicationLogger] Received response: <html>
    <body>
        helloworld
    </body>
</html>

2024-08-14 15:36:04 - INFO [MainLogger] Received response: <html>
    <body>
        helloworld
    </body>
</html>

2024-08-14 15:36:04 - ERROR [session_logger] Error reading request: stream truncated
2024-08-14 15:36:14 - INFO [MainLogger] Executing GET request.
2024-08-14 15:36:14 - INFO [ApplicationLogger] Sending GET request to localhost:8080/
2024-08-14 15:36:14 - DEBUG [ClientLogger] Performing GET request to localhost:8080/
2024-08-14 15:36:14 - DEBUG [ClientLogger] Resolving localhost:8080
2024-08-14 15:36:14 - DEBUG [ClientLogger] Connecting to resolved address.
2024-08-14 15:36:14 - DEBUG [ClientLogger] Performing SSL handshake.
2024-08-14 15:36:14 - DEBUG [session_logger] Session created.
2024-08-14 15:36:14 - DEBUG [session_logger] Running session.
2024-08-14 15:36:14 - DEBUG [session_logger] Starting SSL handshake.
2024-08-14 15:36:14 - DEBUG [session_logger] Handshake successful.
2024-08-14 15:36:14 - DEBUG [session_logger] Reading request.
2024-08-14 15:36:14 - DEBUG [ClientLogger] Sending HTTP request.
2024-08-14 15:36:14 - DEBUG [ClientLogger] Receiving HTTP response.
2024-08-14 15:36:14 - DEBUG [session_logger] Request received successfully.
2024-08-14 15:36:14 - DEBUG [session_logger] Sending response.
2024-08-14 15:36:14 - DEBUG [session_logger] Response sent successfully.
2024-08-14 15:36:14 - DEBUG [session_logger] Reading request.
2024-08-14 15:36:14 - INFO [ClientLogger] Received response: <html>
    <body>
        helloworld
    </body>
</html>

2024-08-14 15:36:14 - INFO [ApplicationLogger] Received response: <html>
    <body>
        helloworld
    </body>
</html>

2024-08-14 15:36:14 - INFO [MainLogger] Received response: <html>
    <body>
        helloworld
    </body>
</html>

2024-08-14 15:36:14 - ERROR [session_logger] Error reading request: stream truncated
```

## Contributing

[Instructions for contributing]

## License

[License information]

## Acknowledgments

[Credits and acknowledgments]

