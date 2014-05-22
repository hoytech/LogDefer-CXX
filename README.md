LogDefer-CXX
============


Description
-----------

Log::Defer C++11 implementation

This is a logger implementation of the [Log::Defer](https://metacpan.org/pod/Log::Defer) format in C++11.

It uses Kazuho Oku's [picojson](https://github.com/kazuho/picojson/) library to serialise messages.


Usage
-----

The usage tries to be as similar as possible to the reference Perl implementation (although it assumes JSON where the perl one doesn't).

The basic usage is to create a new `LogDefer` object. The constructor requires one argument: A lambda callback function. This callback will be invoked when the `LogDefer` object's destructor is called. That is, when it goes out of scope or when the last reference to it has been destroyed.

Here is a simple example:

    {
      LogDefer logger([](const std::string &json) {
                        // Normally we'd write this to a file/syslog/etc
                        std::cout << json << std::endl;
                      });

      logger.info("Processed some stuff");
      logger.warn("some warning!");

    } // <-- logger goes out of scope so callback is invoked now


Todo
----

* Timers (will need to use weak pointers)
* Data (will need to expose picojson interface)
* Make sure that the desctructor can't throw exceptions



See Also
--------

The [Log::Defer](https://metacpan.org/pod/Log::Defer) perl reference implementation

Michael Pucyk's [LogDefer Python module](https://github.com/mikep/LogDefer)

One way to display these logs is the [log-defer-viz](https://metacpan.org/pod/log-defer-viz) visualisation tool


Author and Copyright
--------------------

Copyright (c) 2014 Doug Hoyte

This library is distributed under the 2-clause BSD license so you can basically do whatever you want with it.
