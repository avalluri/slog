# Simple Logger
<!-- vscode-markdown-toc -->
* [Overview](#Overview)
* [Getting Started](#GettingStarted)
	* [Prerequisites](#Prerequisites)
* [Features](#Features)
* [Usage](#Usage)
* [Tests](#Tests)
* [Future Work](#FutureWork)

<!-- vscode-markdown-toc-config
	numbering=false
	autoSave=true
	/vscode-markdown-toc-config -->
<!-- /vscode-markdown-toc --># Simple Logger

## <a name='Overview'></a>Overview

A very light weight logging interface  written in C++. There are no additional dependencies other than C++11 STL.
It supports very simple thread-safe API for logging to console (standard output, standered error)
and to a file. It also supports writing logs to multiple targets.

**Terminology**

* [_Logger_](./include/slog/logger.h) - A simple interface for dumping logs to various targets.
* [_Target_](./include/slog/target.h) - A log target is the one which writes the log messages to streams. New targets could be implemented by extending the Target API.
* [_Decorator_](./include/slog/decorators.h) - Adds additional prefixes to log messages like, timestamp, log level, pid, line number etc.,
* [_Log level_](./include/slog/log_level.h) - Labels that indicate the severity or urgency of the various events in the application.

## <a name='Prerequisites'></a>Prerequisites

Prerequisites for building and running the simple logger code:
* git
* C++11 comipler
* make

## <a name='Usage'></a>Usage

* Getting source code from repositiory
```sh
$ git clone https://github.com/avalluri/slogger.git
```

* Build example application
```sh
$ cd slogger && make
```
The above command will generate `./output/sample-app` application binary.

* Running sample application would generate example log messages to both console and file located at `./logs/sample-app,log`
```sh
$ ./output/sample-app
[Tue Oct 31 21:59:05 2023] [344365] [I] log message should be visible on stdout!
[Tue Oct 31 21:59:05 2023] [344365] [I] some info message to file
[Tue Oct 31 21:59:05 2023] [344365] [D] Sample debug message...
$ cat ./logs/sample-app.log
[Tue Oct 31 21:59:05 2023] [344365] [I] some info message to file
[Tue Oct 31 21:59:05 2023] [344365] [D] Sample debug message...
[Tue Oct 31 21:59:05 2023] [344365] [D] This message is from logger2
```

## <a name='Tests'></a>Tests

Unit tests are located under `./tests` folder. The tests are written using CppUnit test framework.
To run the tests one must `libcppunit` development package.

Run below command to run the tests:
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
## <a name='Limitations'></a>Limitations

The code has below known limitatios, which must be addressed in future releases.
Chacke with below [roadmap](#roadmap) section for further details:

* Code currently supports only Unix/Linix platforms.
* Suppost only intree source. To use `slogger` to other projects, one hast either clone the `slogger` or copy `./include/slog` and `./src` folders to their source tree.
* No support for controlling the log message format, in other words log message decorations.

## <a name='Roadmap'></a>Roadmap

Below features are in the roadmap and will be part of the future source code release:

* Support distribution of `libslog.so` shared library.
* Loogging support variable number of arguments.
* New decoration for adding log colors and source code location.
* `ofstream` target for `std::cout` type logging API.
* Flexible/configurable log decoratorion.
