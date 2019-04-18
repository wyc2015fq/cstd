# 用 C++ 开启技术创业之旅 - 文章 - 伯乐在线
本文由 [伯乐在线](http://blog.jobbole.com) - [Sam Lin](http://www.jobbole.com/members/heray1990) 翻译。未经许可，禁止转载！
英文出处：[James Perry](https://medium.com/swlh/starting-a-tech-startup-with-c-6b5d5856e6de)。欢迎加入[翻译组](https://github.com/jobbole/translation-project)。
我和一个大学的老朋友 Fedor Dzjuba ，创立了一家名为 Signal Analytics 的技术公司。我们通过构建自己的数据库系统来打造一个现代的、基于云的 [OLAP](https://en.wikipedia.org/wiki/OLAP_cube) 数据集（多维数据存储和检索）。
由于我主导技术层面并且我最熟悉 C++，所以决定用它来构建我们的 OLAP 引擎。虽然我最初的确用 Rust 来构建原型，但是那样做风险太大了（我应该另外写一篇文章来解释更多关于这个决定的细节）。
我的很多同行觉得很奇怪，因为我用 C++ 而没有用一种动态的语言（像 Ruby 或者 Python 那样具有高生产效率、使产品可以快速上市的语言）来构建一个基于云的服务。
我开始质疑自己对使用 C++ 的判断。于是我决定调查一下，看看使用 C++ 是好是坏。
### 生产效率
虽然 C++ 不是一种动态语言，但是现代的 C++（C++11/14）有类型推断。对于使用 C++ 编写代码存在很多误解，如，我们一定要使用原始指针来编写代码，一定要输入冗长的命名空间或者类型，一定要手动管理内存。C++ 使我们感到更高生产效率的一个关键特性是 [**auto**](http://en.cppreference.com/w/cpp/language/auto) 特性。我们不一定要输入冗长的命名空间和类名，它利用类型推断来推断出变量的类型。

C++
```
#include <chrono>
int main() {
  //here's the verbose version
  std::chrono::time_point<std::chrono::system_clock> start = std::chrono::system_clock::now();
  // benchmark something here
  std::chrono::time_point<std::chrono::system_clock> end = std::chrono::system_clock::now();
  // now here's the concise version
  auto start = std::chrono::system_clock::now();
  // benchmark something here
  auto end = std::chrono::system_clock::now();
}
```
人工内存管理是对 C++ 最普遍的误解。从 C++11 开始，推荐使用 [**std::shared_ptr**](http://en.cppreference.com/w/cpp/memory/shared_ptr) 或者 [**std::unique_ptr**](http://en.cppreference.com/w/cpp/memory/unique_ptr) 来实现自动内存管理。虽然自动内存管理需要小的运算代价来维护引用指针，但是这个代价是微乎其微的，并且它的安全性也值得付出这个代价。

C++
```
#include <iostream>
#include <memory>
void testPointers() {
    PagedData* a = new PagedData(); //raw pointer, ideally shouldn't be managed manually
    std::unique_ptr<PagedData> b = std::make_unique<PagedData>(); // only 1 unique owner can own at a time
    auto c = std::make_unique<PagedData>(); // same, but less verbose...
    std::shared_ptr<PagedData> d = std::make_shared<PagedData>(); // can be passed around liberally and have multiple owners
    auto e = std::make_shared<PagedData>(); // same, but less verbose...
    // a will now leak as we forgot to call delete
    // yet b/c and d/e will be cleaned up for us
    // notice the lack of new and delete for b/c and d/e? modern c++ prefers the absence of new/delete calls;
}
int main() {
    testPointers(); 
}
```
保证生产效率的最后一部分，是有可以快速构建一个服务或者产品的库。Python、Ruby 等语言有很棒的库来处理常见的基础结构。在我看来，目前的 C++ 标准库严重缺乏基础功能，并且某些 API 性能低下（例如，用 iostreams 读取文件）。好在 Facebook 有高质量的开源库帮助我们快速地发布我们的 OLAP 云服务。
[**Folly**](https://github.com/facebook/folly/)
这是一个很棒的 C++ 库，它包含很多高性能的类以供使用。我在我们的引擎中使用它们的 **fbvector** 和 **fbstring**，因为它们分别提供了比 **std::vector** 和 **std::string** 更好的性能。我们还使用了它们的 futures 和原子无锁数据结构。
Facebook 针对动态增长分配做了一个很聪明的举动来避免平方阶增长（在数学上可以简单地证明并解释为什么平方阶增长是不好的）。他们的容器使用 1.5x 而不是 2x 来增加内存尺寸，从而提高性能。
另外，阅读 Folly 的代码也使得我变成更好的 C++ 开发者，所以我强烈建议大家去阅读。
[**Proxygen**](https://github.com/facebook/proxygen)
Proxygen 是一个异步 HTTP 服务器，也是由 Facebook 开发的。我们使用 Proxygen 作为我们的 HTTP 服务器，以 JSON 的形式插入数据到我们的 OLAP 引擎和从我们的引擎中检索数据。它使得我们可以仅用一天时间就开发出一个高性能的 HTTP 服务器作为我们的引擎。我决定将它作为基准与用 Python 写的 Tornado 服务器进行对比。在一个 EC2 实例中测试 200 次 HTTP 连接，得出了下面的结果：
> 
C++/Proxygen =每秒 1,990,130 次请求
Python/Tornado = 每秒 41,329 次请求
它的 API 更加底层并且你必须自己编写 HTTP 路由，不过这是一项简单的任务。我们的 HTTP 主体处理函数大致如下：

C++
```
#include <folly/futures/Future.h>
void OlapHandler::onBody(std::unique_ptr<folly::IOBuf> body) noexcept {
  folly::fbstring queryStr = processBody(std::move(body));
  Future<OlapData> data = olapCube.query(queryStr);
  data.then(parseToJson).then(addToHttpBody);
}
```
[**Wangle**](https://github.com/facebook/wangle)
我们的 OLAP 引擎基本上是一个用于存储和查询多维数据的分布式数据库。该引擎使用 Wangle 作为一个应用服务器的基础框架。所有的逻辑被分层堆放在 Wangle 的处理函数中，链式地放到一个管道中或者从一个管道中取出。它与我们的 Proxygen HTTP 服务器通信，以处理数据查询和节点之间的通信。
它使用网状的服务器来共享相同的（对称的）二进制可执行文件，所以没有主从模式之分。每个节点既是主服务器也是从服务器，它们之间使用一套自定义的二进制数据协议来传递数据或者消息。
我们需要的库中唯一缺少的是用于完成协助调度存储器和引擎里的查询任务的 [fibers](https://en.wikipedia.org/wiki/Fiber_%28computer_science%29)。另外，虽然目前 Folly 和 Wangle 的开发者提供了实验性的版本，但是这还不能用于产品。
### 硬件和人力成本
我通过量化得出，基于我们的 HTTP 标准，1 台 C++ 服务器的原始计算能力大概相当于 40 台负载平衡的 Python 服务器的原始计算能力。因此，使用 C++ 可以挤压出基础硬件的所有计算能力，从而以 1/40 的折扣节省服务器成本。我想最初我们可以用 Python 来编写引擎，但是，经济上，它将会浪费人力成本和时间，因为，在某些阶段，我们不得不抛弃 Python 而使用 C++ 版本来满足我们对性能的需求。一旦被抛弃，用 Python 写的代码将没有任何经济价值。
总而言之，C++ 可能不会成为初创公司最流行的选择，但是我相信现代 C++ 提供的高级抽象和接近 C 的性能特点，可以使之成为一种切实可行的选择。我担心的是一旦代码库显著地增长时所需的构建时间，希望 C++17 模块可以缓解这个问题。
我希望这篇文章可以鼓励其他人为自己的企业去研究一下 C++。
> 
**打赏支持我翻译更多好文章，谢谢！**
[打赏译者](#rewardbox)
#### 打赏支持我翻译更多好文章，谢谢！
![](http://www.jobbole.com/wp-content/uploads/2016/04/2d041661f5c5720aed6862fa91af766d.png)
