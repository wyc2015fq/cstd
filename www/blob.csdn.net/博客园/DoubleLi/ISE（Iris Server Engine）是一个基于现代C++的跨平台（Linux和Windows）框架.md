# ISE（Iris Server Engine）是一个基于现代C++的跨平台（Linux和Windows）框架 - DoubleLi - 博客园






ISE（Iris Server Engine）是一个基于现代C++的跨平台（Linux和Windows）的高性能多线程并发网络服务器程序框架。它封装了琐碎的socket以及各种操作系统APIs，以面向对象方式向开发者提供稳定、高效、易扩展、易配置、易维护的程序框架。ISE的用户只需遵循接口的约定，挂接自己的业务程序，即可轻松开发出稳定、高效的网络服务器程序。

ISE的主要特点：
- 跨平台。目前支持Linux和Windows。
- 基于多线程并发，而非多进程。
- 支持TCP与UDP服务端程序开发。
- 基于Proactor模式的TCP服务接口。
- 基于请求分组与负载自适应的高并发UDP服务模型。
- 不支持IPv6，只支持IPv4。
- 提供通用数据库接口（DBI），并预置MySQL扩展。
- ISE是一个程序框架（Framework），而非一个程序库（Library）。

https://github.com/haoxingeng/ise









