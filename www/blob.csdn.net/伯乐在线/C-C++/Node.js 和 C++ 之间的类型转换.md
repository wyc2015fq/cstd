# Node.js 和 C++ 之间的类型转换 - 文章 - 伯乐在线
原文出处： [慎里，2016-09-20](http://taobaofed.org/blog/2016/09/20/type-casts-between-node-and-cpp/)
![Node.js 和 C++ 之间的类型转换](://img.alicdn.com/tfs/TB1h_SoNpXXXXcMXXXXXXXXXXXX-900-500.jpg)
我非常喜欢使用 Node.js，但是当涉及到计算密集型的场景时 Node.js 就不能够很好地胜任了。而在这样的情况下 C++ 是一个很好的选择，非常幸运 Node.js 官方提供了 [C/C++ Addons](https://nodejs.org/dist/latest-v6.x/docs/api/addons.html) 的机制让我们能够使用 V8 API 把 Node.js 和 C++ 结合起来。
虽然在 Node.js 官方网站有很多的关于怎么使用这些 API 的文档，但是在 JavaScript 和 C++ 之间传递数据是一件非常麻烦的事情，C++ 是强类型语言（”1024” 是字符串类型而不是整数类型），而 JavaScript 却总是默认的帮我们做一些类型转换。
JavaScript 的基本类型包括 String，Number，Boolean，null，undefined，V8 使用类继承的方式来定义这类型，这些类型都继承了 `Primitive` 类，而 `Primitive` 继承了 `Value`，v8 也支持整型（包括 `Int32` 和 `Uint32`），而所有的类型定义都可以从 V8 [类型文档](https://v8docs.nodesource.com/node-6.0/d0/d4a/classv8_1_1_primitive.html)中看到，除了基本的类型，还有 Object，Array，Map 等类型的定义。
基本类型的继承关系如下图：
![primitive](http://jbcdn2.b0.upaiyun.com/2017/01/5b1652e3cd0a8901ad6cb02b06cba290.png)
在 V8 中所有 JavaScript 值都是被放在 `Local` 对象中，通过这个对象指定了 JavaScript 运行时的内存单元。
下面这段代定义了一个 `Number` 类型的值，其中 Test 函数中声明的 isolate 变量代表着 V8 虚拟机中的堆内存，当创建新变量的时候就需要用到它，接下来的一行代码就通过 isolate 声明了一个 `Number` 类型的变量。


```
#include <node.h>
#include <v8.h>
using namespace v8;
void Test(const v8::FunctionCallbackInfo<v8::Value>& args) {
    Isolate* isolate = args.GetIsolate();
    // 声明变量
    Local<Number> retval = v8::Number::New(isolate, 1000);
}
void init(Local <Object> exports, Local<Object> module) {
    NODE_SET_METHOD(exports, "getTestValue", Test);
}
NODE_MODULE(returnValue, init)
```
看了 V8 [类型 API 文档](https://v8docs.nodesource.com/node-6.0/d0/d4a/classv8_1_1_primitive.html) 你会发现对于基本的 JavaScript 类型，只有变量的声明而没有变量的赋值。最初想可能觉得这个非常的奇怪，可是仔细想一想后发现这个是合理的。主要由以下几点原因：
- JavaScript 的基本类型是不可变类型，变量都是指向一个不可变的内存单元，var a = 10，则 a 指向的内存单元中包含的值为 5，重新赋值 a = 100，没有改变这个内存单元的值，而是使得 a 指向了另外一个内存单元，其中的值为 100。如果声明两个变量 x，y 的值都为 10，则他们指向的是同一个内存单元。
- 函数的传参都是传值，而不是传引用，当在 JavaScript 中调用 C++ 的函数时，如果参数是基本类型则每次都是把这个值拷贝过去，改变参数的值不会影响原来的值。
- 使用 `Local` 声明基本类型的变量都是对内存单元的引用，因为第一条原因不可能改变引用的值使其指向另外一个内存单元，因此不存在变量的重新赋值。
### 数据流向 C++ -> JavaScript
下面 demo 定义了一些常用的 JavaScript 类型，包括基本类型的以及 Object, Array, Fuction。


```
#include <node.h>
#include <v8.h>
using namespace v8;
void MyFunction(const v8::FunctionCallbackInfo<Value>& args) {
    Isolate* isolate = args.GetIsolate();
    args.GetReturnValue().Set(String::NewFromUtf8(isolate, "Hello World!"));
}
void Test(const v8::FunctionCallbackInfo<v8::Value>& args) {
    Isolate* isolate = args.GetIsolate();
    // Number 类型的声明
    Local<Number> retval = v8::Number::New(isolate, 1000);
    // String 类型的声明
    Local<String> str = v8::String::NewFromUtf8(isolate, "Hello World!");
    // Object 类型的声明
    Local<Object> obj = v8::Object::New(isolate);
    // 对象的赋值
    obj->Set(v8::String::NewFromUtf8(isolate, "arg1"), str);
    obj->Set(v8::String::NewFromUtf8(isolate, "arg2"), retval);
    // Function 类型的声明并赋值
    Local<FunctionTemplate> tpl = v8::FunctionTemplate::New(isolate, MyFunction);
    Local<Function> fn = tpl->GetFunction();
    // 函数名字
    fn->SetName(String::NewFromUtf8(isolate, "theFunction"));
    obj->Set(v8::String::NewFromUtf8(isolate, "arg3"), fn);
    // Boolean 类型的声明
    Local<Boolean> flag = Boolean::New(isolate, true);
    obj->Set(String::NewFromUtf8(isolate, "arg4"), flag);
    // Array 类型的声明
    Local<Array> arr = Array::New(isolate);
    // Array 赋值
    arr->Set(0, Number::New(isolate, 1));
    arr->Set(1, Number::New(isolate, 10));
    arr->Set(2, Number::New(isolate, 100));
    arr->Set(3, Number::New(isolate, 1000));
    obj->Set(String::NewFromUtf8(isolate, "arg5"), arr);
    // Undefined 类型的声明
    Local<Value> und = Undefined(isolate);
    obj->Set(String::NewFromUtf8(isolate, "arg6"), und);
    // null 类型的声明
    Local<Value> null = Null(isolate);
    obj->Set(String::NewFromUtf8(isolate, "arg7"), null);
    // 返回给 JavaScript 调用时的返回值
    args.GetReturnValue().Set(obj);
}
void init(Local <Object> exports, Local<Object> module) {
    NODE_SET_METHOD(exports, "getTestValue", Test);
}
NODE_MODULE(returnValue, init)
```
所有的 addon 都需要一个初始化的函数，如下面的代码：


```
void Initialize(Local<Object> exports);
NODE_MODULE(module_name, Initialize)
```
`Initialize` 是初始化的函数，`module_name` 是编译后产生的二进制文件名，上述代码的模块名为 `returnValue`。
上述代码通过 node-gyp 编译后（编译过程官方文档 [C/C++ Addons](https://nodejs.org/dist/latest-v6.x/docs/api/addons.html) 有详细的介绍），可以通过如下的方式调用。


```
// returnValue.node 这个文件就是编译后产生的文件，通过 NODE_MODULE(returnValue, init) 决定的文件名
const returnValue = require('./build/Release/returnValue.node');
console.log(returnValue.getTestValue());
```
运行结果如下:
![returnValue](http://jbcdn2.b0.upaiyun.com/2017/01/57efd3c0adcd3b9b798d9d81794598cb.png)
### 数据流向 javaScript -> C++
上面的 demo 展示了怎样在在 C++ 定义 JavaScript 类型，数据的是从 C++ 流向 JavaScript，反过来数据也需要从 javaScript 流向 C++，也就是调用 C++ 函数的时候需要传入一些参数。
下面的代码展示了参数个数判断，参数类型判断，以及参数类型装换成 V8 类型的过程，包括基本类型以及 Object, Array, Fuction。


```
#include <node.h>
#include <v8.h>
#include <iostream>
using namespace v8;
using namespace std;
void GetArgument(const FunctionCallbackInfo<Value>& args) {
    Isolate* isolate = args.GetIsolate();
    // 参数长度判断
    if (args.Length() < 2) {
        isolate->ThrowException(Exception::TypeError(
            String::NewFromUtf8(isolate, "Wrong number of arguments")));
        return;
    }
    // 参数类型判断
    if (!args[0]->IsNumber() || !args[1]->IsNumber()) {
        //抛出错误
        isolate->ThrowException(Exception::TypeError(
            String::NewFromUtf8(isolate, "argumnets must be number")));
    }
    if (!args[0]->IsObject()) {
        printf("I am not Object\n");
    }
    if (!args[0]->IsBoolean()) {
        printf("I am not Boolean\n");
    }
    if (!args[0]->IsArray()) {
        printf("I am not Array\n");
    }
    if (!args[0]->IsString()) {
        printf("I am not String\n");
    }
    if (!args[0]->IsFunction()) {
        printf("I am not Function\n");
    }
    if (!args[0]->IsNull()) {
        printf("I am not Null\n");
    }
    if (!args[0]->IsUndefined()) {
        printf("I am not Undefined\n");
    }
    // js Number 类型转换成 v8 Number 类型
    Local<Number> value1 = Local<Number>::Cast(args[0]);
    Local<Number> value2 = Local<Number>::Cast(args[1]);
    double value = value1->NumberValue() + value2->NumberValue();
    // js String 类型转换成 v8 String 类型
    Local<String> str = Local<String>::Cast(args[2]);
    String::Utf8Value utfValue(str);
    cout<<string(*utfValue)<<endl;
    // js Array 类型转换成 v8 Array 类型
    Local<Array> input_array = Local<Array>::Cast(args[3]);
    printf("%d, %f %f\n", input_array->Length(), input_array->Get(0)->NumberValue(), input_array->Get(1)->NumberValue());
    // js Object 类型转换成 v8 Object 类型
    Local<Object> obj = Local<Object>::Cast(args[4]);
    // 根据 key 获取对象中的值
    Local<Value> a = obj->Get(String::NewFromUtf8(isolate, "a"));
    Local<Value> b = obj->Get(String::NewFromUtf8(isolate, "b"));
    // js Array 类型转换成 v8 Array 类型
    Local<Array> c = Local<Array>::Cast(obj->Get(String::NewFromUtf8(isolate, "c")));
    cout<<a->NumberValue()<<"   "<<b->NumberValue()<<endl;
    printf("%d, %f %f\n", c->Length(), c->Get(0)->NumberValue(), c->Get(1)->NumberValue());
    // js String 类型转换成 v8 String 类型
    Local<String> cString = Local<String>::Cast(c->Get(2));
    String::Utf8Value utfValueD(cString);
    cout<<string(*utfValueD)<<endl;
    // 根据 key 获取对象中的值
    Local<Object> d = Local<Object>::Cast(obj->Get(String::NewFromUtf8(isolate, "d")));
    Local<String> dString1 = Local<String>::Cast(d->Get(String::NewFromUtf8(isolate, "m")));
    String::Utf8Value utfValued1(dString1);
    cout<<string(*utfValued1)<<endl;
    // 根据 key 获取对象中的值
    Local<String> dString2 = Local<String>::Cast(d->Get(String::NewFromUtf8(isolate, "n")));
    String::Utf8Value utfValued2(dString2);
    cout<<string(*utfValued2)<<endl;
    // js Booelan 类型转换成 v8 Boolean 类型
    Local<Boolean> FlagTrue = Local<Boolean>::Cast(args[5]);
    cout<<"Flag: "<<FlagTrue->BooleanValue()<<endl;
    // js Function 类型转换成 v8 Function 类型
    Local<Function> cb = Local<Function>::Cast(args[8]);
    const unsigned argc = 2;
    Local<Value> argv[2];
    argv[0] = a;
    argv[1] = b;
    cb->Call(Null(isolate), argc, argv);
    args.GetReturnValue().Set(value);
}
void Init(Local <Object> exports, Local <Object> module) {
    NODE_SET_METHOD(module, "exports", GetArgument);
}
NODE_MODULE(argumentss, Init)
```
通过 node-gyp 编译后，可以通过如下的方式调用。


```
const getArguments = require('./build/Release/arguments');
console.log(getArguments(2, 3, 'Hello Arguments', [1, 2, 3], {
        a: 10,
        b: 100,
        c: [23, 22, "我是33"],
        d: { m: '我是22', n: '我是23' }
    }, true, null, undefined,
    function myFunction(...args) {
        console.log('I am Function!');
        console.log(...args);
        console.log('I am Function!');
    }));
```
运行结果如下:
![argument](http://jbcdn2.b0.upaiyun.com/2017/01/d522ad4eb2e430e23485a902b8c746df.png)
关于其他的类型，我这里就就不一一介绍，V8 文档里面都有对应的 API。
### NAN
由于 V8 的 API 还没有彻底稳定下来，所以对于不同版本的 Node.js 类型相关的 API 会发生变化，而 [NAN](https://github.com/nodejs/nan) 帮我们做了封装，在编码的时候不需要关心版本问题，只需要引入相应的头文件即可。
引入头文件后，可以如下使用方式：


```
v8::Local<v8::Primitive> Nan::Undefined()
v8::Local<v8::Primitive> Nan::Null()
```
### 参考资料
- [Type conversions from JavaScript to C++ in V8](http://blog.scottfrees.com/type-conversions-from-javascript-to-c-in-v8)
- [node addon](https://nodejs.org/dist/latest-v6.x/docs/api/addons.html)
- [v8 types documentation](https://v8docs.nodesource.com/node-6.0/d0/d4a/classv8_1_1_primitive.html)
- [node-gyp](https://github.com/nodejs/node-gyp)
- [gyp user documentation](https://gyp.gsrc.io/docs/UserDocumentation.md)
- [nan](https://github.com/nodejs/nan)
