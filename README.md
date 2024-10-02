# aibeast

## Project Description

**aibeast** is a versatile and scalable C++ application built on top of the Boost.Beast library. It provides a robust framework for handling HTTP and WebSocket communications with a focus on security, using SSL/TLS for encrypted connections. Designed for high-performance networking, **aibeast** is ideal for building server-side applications that require secure and efficient client-server communication.

The application is organized into distinct modules that handle various aspects of the networking stack, from managing SSL certificates to handling HTTP requests and WebSocket sessions. **aibeast's** modular architecture makes it easy to extend and customize, whether you're building a web server, an API service, or a real-time communication platform.

### Ollama Integration

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
    git clone https://github.com/diyooor/aibeast.git
    ```

### Running the Application

To run the application, use the following command:

```bash
make run
```

## Contributing

Contributions to beastapp are welcome! If you have suggestions for improvements or would like to contribute new features, please feel free to submit a pull request or open an issue.

## License

Copyright <2024> <diyorsattarov>

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the “Software”), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

## Acknowledgments

Special thanks to the Boost community for providing a powerful and flexible set of libraries that make C++ development more accessible and efficient. Also, thanks to the Ollama team/community for their excellent machine learning models that power the LLM integration in this project.



