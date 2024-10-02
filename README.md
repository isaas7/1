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

              GLWTS(Good Luck With That Shit) Общественная Лицензия
                      Copyright (c) Все блять, кроме Автора

Всем разрешается копировать, распространять, изменять, совмещать, продавать,
публиковать, сублицензировать или творить любую другую хуйню с этим кодом
на СВОЙ СТРАХ и РИСК.

Автор абсолютно нихуя не понимает в предназначении данного кода.
Он может сработать, а может и нет - автору не ебет.


                    "УДАЧИ С ЭТОМ ДЕРЬМОМ" ОБЩЕСТВЕННАЯ ЛИЦЕНЗИЯ
          УСЛОВИЯ И ПОЛОЖЕНИЯ ДЛЯ КОПИРОВАНИЯ, РАСПРОСТРАНЕНИЯ И ИЗМЕНЕНИЯ

  0. Делайте ЛЮБУЮ ХУЙНЮ, ВЗБРЕДУЩУЮ ВАМ В ГОЛОВУ до тех пор, пока ВЫ НЕ ОСТАВЛЯЕТЕ
УЛИК ДЛЯ НАХОЖДЕНИЯ АВТОРА оригинального продукта, чтобы обвинить его или заставить
взять на себя ответственность.

НИ ПРИ КАКИХ ОБСТОЯТЕЛЬСТВАХ АВТОРЫ НЕ НЕСУТ ОТВЕТСТВЕННОСТИ ЗА ПРЕТЕНЗИИ,
УБЫТКИ ИЛИ ДРУГИЕ ОТВЕТСТВЕННОСТИ, ПОДНЯТЫЕ ВО ВРЕМЯ КОНТРАКТА И ВЫЗВАННЫЕ
ПРЯМОЙ ИЛИ КОСВЕННОЙ СВЯЗЬЮ С ПРОГРАММНЫМ ОБЕСПЕЧЕНИЕМ.

Удачи, и храни тебя Господь.

## Acknowledgments

Special thanks to the Boost community for providing a powerful and flexible set of libraries that make C++ development more accessible and efficient. Also, thanks to the Ollama team/community for their excellent machine learning models that power the LLM integration in this project.



