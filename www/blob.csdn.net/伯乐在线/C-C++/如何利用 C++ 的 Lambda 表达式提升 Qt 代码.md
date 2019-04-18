# 如何利用 C++ 的 Lambda 表达式提升 Qt 代码 - 文章 - 伯乐在线
原文出处： [oschina](https://www.oschina.net/translate/how-c-lambda-expressions-can-improve-your-qt-code?print)
![](http://jbcdn2.b0.upaiyun.com/2017/02/9a5d2dfa56dec0b520109c9c76f05ece.png)
Lambda 表达式是在 C++11 中加入的 C++ 特性。在这篇文章中我们将看到如何用 Lambda 表达式来简化 Qt 代码。Lambda 很强大，但也要小心它带来的陷阱。
## 首先，什么是 Labmda 表达式？
Lambda 表达式是在某个函数中直接定义的匿名函数。它可以用于任何需要传递函数指针的地方。
Lambda 表达式的语法如下：


```
[获取变量](参数) {
    lambda 代码
}
```
现在先忽略 “获取变量” 这部分。下面是一个简单的 Lambda，用于递增一个数：


```
[](int value) {
    return value + 1;
}
```
我们可以把这个 Lambda 用于像 [std::transform()](http://en.cppreference.com/w/cpp/algorithm/transform) 这样的函数，来为 vector 的每一个元素增值：


```
#include 
#include 
#include 
int main() {
    std::vector vect = { 1, 2, 3 };
    std::transform(vect.begin(), vect.end(), vect.begin(),
                   [](int value) { return value + 1; });    for(int value : vect) {
std::cout
```
打印结果：


```
2
3
4
```
### 获取变量
Lambda 表达式可以通过 “获取” 来使用当前作用域中的变量。下面是用 Lambda 来对 vector 求和的一个示例。


```
std::vector vect = { 1, 2, 3 };
int sum = 0;
std::for_each(vect.begin(), vect.end(), [&sum](int value) {
    sum += value;
});
```
你可以看到，我们获取了本地变量 sum，所以可以在 Lambda 内部使用它。sum 加了前缀 &，这表示我们通过引用获取 sum 变量：在 Lambda 内部，sum 是一个引用，所以对它进行的任何改变都会对 Lambda 外部的 sum 变量造成影响。
如果你不是需要引用，只需要变量的拷贝，只需要去掉 & 就好。
如果你想获取多个变量，只需要用逗号进行分隔，就像函数的参数那样。
目前还不能直接获取成员变量，但是你可以获取 this，然后通过它访问当前对象的所有成员。
在背后，Lambda 获取的变量会保存在一个隐藏的对象中。不过，如果编译器确认 Lambda 不会在当前局部作用域之外使用，它就会进行优化，直接使用局域变量。
有一个偷懒的办法可以获取所有局部变量。用 [&] 来获取它们的引用；用 [=] 来获取它们的拷贝。不过最好不要这样做，因为引用变更的生命周期很可能短于 Lambda 的生命周期，这会导致奇怪的错误。就算你获取的是一个变量的拷贝，但它本身是一个指针，也会导致崩溃。如果明确的列出你依赖的变量，会更容易避开这类陷阱。关于这个陷阱更多的信息，请看看 “[Effective Modern C++](http://shop.oreilly.com/product/0636920033707.do)” 的第 31 条。
## Qt 连接中的 Lambda
如果你在用[新的连接风格](http://doc.qt.io/qt-5/qt5-intro.html#new-connection-syntax) (你应该用，因为有非常好的类型安全！)，就可以在接收端使用 Lambda，这对于较小的处理函数来说简直太棒了。
下面是一个电话括号器的示例，用户可以输入数字然后拨出电话：


```
Dialer::Dialer() {
    mPhoneNumberLineEdit = new QLineEdit();
    QPushButton* button = new QPushButton("Call");
    /* ... */
    connect(button, &QPushButton::clicked,
            this, &Dialer::startCall);
}
void Dialer::startCall() {
    mPhoneService->call(mPhoneNumberLineEdit->text());
}
```
我们可以使用 Lambda 代替 startCall() 方法：


```
Dialer::Dialer() {
    mPhoneNumberLineEdit = new QLineEdit();
    QPushButton* button = new QPushButton("Call");
    /* ... */
    connect(button, &QPushButton::clicked, [this]() {
        mPhoneService->call(mPhoneNumberLineEdit->text());
    });
}
```
## 用 Lambda 代替 QObject::sender()
Lambda 也是 [QObject::sender()](http://doc.qt.io/qt-5/qobject.html#sender) 的一个非常好的替代方案。想像一下，如果我们的拨号器现在是一组的数字按钮的数组。
没使用 Labmda 的代码，在组合数字的时候会像这样：


```
Dialer::Dialer() {
    for (int digit = 0; digit setProperty("digit", digit);
        
        connect(button, &QPushButton::clicked,
                this, &Dialer::onClicked);
    }
    /* ... */
}
void Dialer::onClicked() {
    QPushButton* button = static_cast(sender());
    int digit = button->property("digit").toInt();
    mPhoneService->dial(digit);
}
```
我们可以使用 [QSignalMapper](http://doc.qt.io/qt-5/qsignalmapper.html) 并去掉 Dialer::onClicked() 方法，但使用 Labmda 会更灵活更简单。我们只需要获取与按钮对应的数字，然后在 Lambda 中直接就能调用 mPhoneService->dial()。


```
Dialer::Dialer() {
    for (int digit = 0; digit dial(digit);
            }
        );
    }
    /* ... */
}
```
## 不要忘了对象的生命周期！
看这段代码：


```
void Worker::setMonitor(Monitor* monitor) {
    connect(this, &Worker::progress,
            monitor, &Monitor::setProgress);
}
```
在这个小例子中，有一个 Worker 实例来向 Monitor 实例报告进度。到目前为止，还没什么问题。
现在假设 Worker::progress() 有一个 int 型的参数，并且 monitor 的另一个方法需要使用这个参数值。我们会尝试这样做：


```
void Worker::setMonitor(Monitor* monitor) {
    // Don't do this!
    connect(this, &Worker::progress, [monitor](int value) {
        if (value setProgress(value);
	} else {
	    monitor->markFinished();
	}
    });
}
```
看起来没问题……但是这段代码会导致崩溃！
Qt 的连接系统很智能，如果发送方和接收方中的任何一个被删除掉，它就会删除连接。在最初的 setMonitor() 中，如果 monitor 被删除了，连接也会被删除。但现在我们使用了 Lambda 来作为接收方： Qt 目前没有办法发现在 Lambda 中使用了 monitor。即使 monitor 被删除掉，Lambda 仍然会调用，结果应用就会在尝试引用 monitor 的时候发生崩溃。
为了避免崩溃发生，你要向 connect() 调用传入一个“context”参数，像这样：


```
void Worker::setMonitor(Monitor* monitor) {
    // Do this instead!
    connect(this, &Worker::progress, monitor,
            [monitor](int value) {
	if (value setProgress(value);
	} else {
	    monitor->markFinished();
	}
    });
}
```
这段代码中，我们把 monitor 作为上下文传入了 connect()。这不会对 Lambda 的执行造成影响，但是在 monitor 被删除之后，Qt 会注意到并解除 Worker::progress() 和 Lambda 之间的连接。
这个上下文还会用于检测连接是否在队列中。就像经典的 signal-slot 连接那样，如果上下文对象与发射信号的代码不在同一个线程，Qt 会将连接置入队列。
## 代替 QMetaObject::invokeMethod
你可能对一种异步调用 slot 的方法比较熟悉，它使用 [QMetaObject::invokeMethod](http://doc.qt.io/qt-5/qmetaobject.html#invokeMethod)。先定义一个类：


```
class Foo : public QObject {
public slots:
    void doSomething(int x);
};
```
你可以在 Qt 中使用 QMetaObject::invokeMethod 在事件循环返回时调用 Foo::doSomething()：


```
QMetaObject::invokeMethod(this, "doSomething",
                          Qt::QueuedConnection, Q_ARG(int, 1));
```
这段代码会工作，但是：
- 语法太丑
- 非类型安全
- 你必须定义作为 slot 的方法
可以通过在 [QTimer::singleShot()](http://doc.qt.io/qt-5/qtimer.html#singleShot-4) 中调用 Lambda 来代替上面的代码：


```
QTimer::singleShot(0, [this]() {
    doSomething(1);
});
```
这个效率会稍低一些，因为  QTimer::singleShot() 会在背后创建一个对象，不过，只要你不是要在一秒内调用很多次，这点性能损失可以忽略不计。显然利大于弊。
你同样可以在 Lambda 前面指定一个上下文，这在多线程中非常有用。但要小心：如果你使用低于 5.6.0 版本的 Qt，QTimer::singleShot() 有[一个 BUG](https://bugreports.qt.io/browse/QTBUG-48700) 在多线程中使用时会导致崩溃。我们找到了那个困难的办法……
## 关键点
- 连接 Qt 对象的时候使用 Lambda 比使用调度方法更好
- 在 connect() 调用中使用 Lambda 一定要有上下文
- 按需获取变量
希望你能喜欢这篇文章，并希望你现在就用漂亮的 Lambda 语法替换掉古板的旧语法！
