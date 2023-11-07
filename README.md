# Simple Logger
<!-- vscode-markdown-toc -->
* [Overview](#overview)
* [Prerequisites](#prerequisites)
* [Usage](#usage)
* [Tests](#tests)
* [Roadmap](#roadmap)

<!-- vscode-markdown-toc-config
	numbering=false
	autoSave=true
	/vscode-markdown-toc-config -->
<!-- /vscode-markdown-toc --># Simple Logger

## Overview

A very lightweight logging interface  written in C++. There are no additional dependencies other than C++11 STL.
It supports a very simple thread-safe API for logging into the console (standard output, standard error)
and to a file. It also supports writing logs to multiple targets.

**Terminology**

* [_Logger_](./include/slog/logger.h) - A simple interface for dumping logs to various targets.
* [_Target_](./include/slog/target.h) - A log target is the one that writes the log messages to streams. New targets could be implemented by extending the Target API.
* [_Decorator_](./include/slog/decorators.h) - Adds additional prefixes to log messages like, timestamp, log level, pid, line number, etc.,
* [_Log level_](./include/slog/log_level.h) - Labels that indicate the severity or urgency of the various events in the application.

Currently supported features:
  - Logs formatted string with a variable-sized list of arguments. (smimlar to `printf`)
  - Multi-thread safe file target API
  - Logging to multiple targets
  - Multiple loggers sharing the same target
  - Filter messages to different targets based on the log level
  - Logging user-defined types

## Prerequisites

Prerequisites for building and running the simple logger code:
* git
* C++11 compiler (GCC g++)
* make

## Usage

* Getting source code from the repository
```sh
$ git clone https://github.com/avalluri/slogger.git
```

* Build an example application
```sh
$ cd slogger && make
```
The above command will generate `./output/sample-app` application binary.

* Running sample application would generate example log messages to both the console and to the file located at `./logs/sample-app,log`
```sh
$ ./output/sample-app
[Thu Nov  2 12:38:19 2023] [508754] [D] This debug message 0 should be visible on stdout!
[Thu Nov  2 12:38:19 2023] [508754] [D] This debug message 1 should be visible on stdout!
[Thu Nov  2 12:38:19 2023] [508754] [E] Unknown error occurred!!! Should be visible on stderr
[Thu Nov  2 12:38:19 2023] [508754] [E] Unknown error occurred!!! Should be visible on stderr
[Thu Nov  2 12:38:19 2023] [508754] [I] Info message. should appear both on stdout and in file
$ cat ./logs/sample-app.log
[Thu Nov  2 12:38:19 2023] [508754] [E] Unknown error occurred!!! Should be visible on stderr
[Thu Nov  2 12:38:19 2023] [508754] [I] Info message. should appear both on stdout and in file
[Thu Nov  2 12:38:19 2023] [508754] [D] This debug message is from logger2 to file: Hi!
```

## Tests

Unit tests are located under `./tests` folder. The tests are written using the CppUnit test framework.
To run the tests one must install `libcppunit` development package.

Run the below command to run the tests:
```sh
$ make test
g++ -std=c++11 -Wall -Wextra -g -Iinclude -Iinclude/slog -c -MMD examples/main.cpp  -o examples/main.o
g++ -std=c++11 -Wall -Wextra -g -Iinclude -Iinclude/slog -c -MMD tests/test_main.cpp  -o tests/test_main.o
g++ -std=c++11 -Wall -Wextra -g -Iinclude -Iinclude/slog -o output/slog-test tests/test_main.o src/log_level.o src/utils.o  -lcppunit
./output/slog-test
FileTargetTest::testFileTargetConstruct : OK
FileTargetTest::testFileTargetSymlink : OK
FileTargetTest::testFileTargetLogs : OK
FileTargetTest::testFileTargetConcurrent : OK
LogLevelTest::testLogLevelConstruct : OK
LogLevelTest::testLogLevelSetGet : OK
LogLevelTest::testLogLevelComparision : OK
LoggerTest::testWithDefaltOptions : OK
LoggerTest::testWithCustomOptions : OK
OK (9)
```
## Limitations

The code has below known limitations, which must be addressed in future releases.
Chacke with below [roadmap](#roadmap) section for further details:

* Code currently supports only Unix/Linux platforms.
* Support only in-tree sources. To use `slogger` for other projects, clone the `slogger` or copy `./include/slog` and `./src` folders to their source tree.
* No support for controlling the log message format, in other words, log message decorations.

## Roadmap

The below features are in the roadmap and will be part of the future source code release:

* Support distribution of `libslog.so` shared library.
* New decoration for adding log colors and source code location.
* `ofstream` target for `std::cout` type logging API.
* Flexible/configurable log decoratorion.
