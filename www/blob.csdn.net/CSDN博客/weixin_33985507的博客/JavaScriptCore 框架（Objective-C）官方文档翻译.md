# JavaScriptCore 框架（Objective-C）官方文档翻译 - weixin_33985507的博客 - CSDN博客
2016年06月02日 16:52:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：7
> 
通过 JavaScriptCore 框架，你可以在 Objective-C 或者基于 C 的程序中运行（evaluate） JavaScript 程序。它还能帮你插入一些自定义对象到 JavaScript 环境中去。
#### 一、类（Classes）
|类|功能|
|----|----|
|[NSObject](https://link.jianshu.com?t=https://developer.apple.com/library/ios/documentation/Cocoa/Reference/Foundation/Classes/NSObject_Class/index.html#//apple_ref/occ/cl/NSObject)（根类）|NSObject 是大部分 Objective-C 类的根类。|
|[JSContext](https://link.jianshu.com?t=https://developer.apple.com/library/ios/documentation/JavaScriptCore/Reference/JSContext_Ref/index.html#//apple_ref/occ/cl/JSContext)|一个 JSContext 对象代表一个 JavaScript 执行环境（execution environment）。|
|[JSManagedValue](https://link.jianshu.com?t=https://developer.apple.com/library/ios/documentation/JavaScriptCore/Reference/JSManagedValue_Ref/index.html#//apple_ref/occ/cl/JSManagedValue)|一个 JSManagedValue 对象包装了一个 JSValue 对象，JSManagedValue 对象通过添加“有条件的持有（conditional retain）”行为来实现自动内存管理。|
|[JSValue](https://link.jianshu.com?t=https://developer.apple.com/library/ios/documentation/JavaScriptCore/Reference/JSValue_Ref/index.html#//apple_ref/occ/cl/JSValue)|一个 JSValue 实例是一个 JavaScript 值（value）的引用（reference）。|
|[JSVirtualMachine](https://link.jianshu.com?t=https://developer.apple.com/library/ios/documentation/JavaScriptCore/Reference/JSVirtualMachine_Ref/index.html#//apple_ref/occ/cl/JSVirtualMachine)|一个 JSVirtualMachine 实例代表一个自包含的（self-contained） JavaScript 执行环境（execution environment）。|
#### 二、协议（Protocols）
[**JSExport**](https://link.jianshu.com?t=https://developer.apple.com/library/ios/documentation/JavaScriptCore/Reference/JSExport_Ref/index.html#//apple_ref/occ/intf/JSExport)
JSExport 协议提供了一些关于将 Objective-C 实例的类和它们的实例方法，类方法以及属性转成 JavaScript 代码的接口声明。
The JSExport protocol provides a declarative way to export Objective-C instance classes and their instance methods, class methods, and properties to JavaScript code.
#### 三、其他参考（Other Reference）
*[JSBase.h Reference](https://link.jianshu.com?t=https://developer.apple.com/library/ios/documentation/JavaScriptCore/Reference/JSBase_header_reference/index.html#//apple_ref/doc/uid/TP40011493)*
*[JSContextRef.h Reference](https://link.jianshu.com?t=https://developer.apple.com/library/ios/documentation/JavaScriptCore/Reference/JSContextRef_header_reference/index.html#//apple_ref/doc/uid/TP40011494)*
*[JSObjectRef.h Reference](https://link.jianshu.com?t=https://developer.apple.com/library/ios/documentation/JavaScriptCore/Reference/JSObjectRef_header_reference/index.html#//apple_ref/doc/uid/TP40011495)*
*[JSStringRef.h Reference](https://link.jianshu.com?t=https://developer.apple.com/library/ios/documentation/JavaScriptCore/Reference/JSStringRef_header_reference/index.html#//apple_ref/doc/uid/TP40011499)*
*[JSStringRefCF.h Reference](https://link.jianshu.com?t=https://developer.apple.com/library/ios/documentation/JavaScriptCore/Reference/JSStringRefCF_header_reference/index.html#//apple_ref/doc/uid/TP40011497)*
*[JSValueRef.h Reference](https://link.jianshu.com?t=https://developer.apple.com/library/ios/documentation/JavaScriptCore/Reference/JSValueRef_header_reference/index.html#//apple_ref/doc/uid/TP40011501)*
> 
**参考（Reference）**
[https://developer.apple.com/library/ios/documentation/Carbon/Reference/WebKit_JavaScriptCore_Ref/index.html#//apple_ref/doc/uid/TP40004754](https://link.jianshu.com?t=https://developer.apple.com/library/ios/documentation/Carbon/Reference/WebKit_JavaScriptCore_Ref/index.html#//apple_ref/doc/uid/TP40004754)
> 
**问题（Question）**
JavaScriptCore 框架存在的意义是什么？或者说作用是什么？
