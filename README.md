# beastapp

## Demo

![Alt text](img/demo.gif)


### 17 August 2024

    - [x] implement json endpoint with mock data
    - [x] implement file io for mock data
### 16 August 2024 Todo

- LLM querying
    - [x] query in application
    - [x] http(post) app->query
    - [x] use partial responses
    - [x] add llm queries to queue
## Project Description

**beastapp** is a versatile and scalable C++ application built on top of the Boost.Beast library. It provides a robust framework for handling HTTP and WebSocket communications with a focus on security, using SSL/TLS for encrypted connections. Designed for high-performance networking, **beastapp** is ideal for building server-side applications that require secure and efficient client-server communication.

The application is organized into distinct modules that handle various aspects of the networking stack, from managing SSL certificates to handling HTTP requests and WebSocket sessions. **beastapp's** modular architecture makes it easy to extend and customize, whether you're building a web server, an API service, or a real-time communication platform.

### Demo with partial responses

```
{
    "query_id": "9420340123726940416",
        "status": "{\"canceled\":false,\"completed\":false,\"partial_responses\":[\" The\",\" color\",\" of\",\" the\",\" sky\",\" is\",\" due\",\" to\",\" a\",\" process\",\" called\",\" scattering\",\".\",\" When\",\" sunlight\",\" passes\",\" through\",\" the\",\" Earth\",\"'\",\"s\",\" atmosphere\"],\"query_id\":\"9420340123726940416\",\"running\":true}"
}
```

### Ollama ML/LLM Integration

A key feature of beastapp is its integration with Ollama's machine learning language model (LLM) capabilities. This allows the application to perform advanced natural language processing (NLP) tasks directly within the server environment. The integration is designed to be seamless, enabling beastapp to send prompts to the LLM and receive sophisticated responses, such as explanations, insights, or other text-based information, making it ideal for AI-driven applications.

The LLM queries are efficiently handled using the **ollama-hpp**, which abstracts the complexities of interacting with the language model, allowing for straightforward integration with the rest of the application. This feature is especially useful for building AI-powered chatbots, customer support systems, or any other service requiring natural language understanding and generation.

## Getting Started

### Prerequisites

- c++17 or later
- Boost Libraries (specifically, boost_system, boost_filesystem, boost_thread, boost_asio, boost_beast)
- OpenSSL (provides SSL/TLS support)
- libcrypto (OpenSSL cryptographic library)
- make (for building the project)
- g++ or clang++ (C++ compiler)
- ollama.hpp

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

## Directory Structure

The project is organized into the following main directories:

### app/

This directory contains the core application logic. It includes the `Application` class, which manages the high-level operations of the application, such as sending HTTP requests and interacting with the network layer.

### net/

The `net` directory houses all networking-related components. This includes the HTTP client (`Client` class), server setup (`server` class), session management, SSL/TLS certificates, and any utilities related to network operations. The client is used for making HTTP requests, while the server handles incoming connections and requests.

### log/

The `log` directory is responsible for logging functionality. It contains the logger implementation, which is used across the application to record events, errors, and debug information. The logger is configurable and can output logs to both the console and files, depending on the setup.


## Contributing

Contributions to beastapp are welcome! If you have suggestions for improvements or would like to contribute new features, please feel free to submit a pull request or open an issue.

## License


## Acknowledgments

Special thanks to the Boost community for providing a powerful and flexible set of libraries that make C++ development more accessible and efficient. Also, thanks to the Ollama team/community for their excellent machine learning models that power the LLM integration in this project.


