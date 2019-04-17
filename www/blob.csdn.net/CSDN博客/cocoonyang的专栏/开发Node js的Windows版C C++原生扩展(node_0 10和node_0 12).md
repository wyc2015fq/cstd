# 开发Node.js的Windows版C/C++原生扩展(node_0.10和node_0.12) - cocoonyang的专栏 - CSDN博客





2012年12月10日 18:46:09[cocoonyang](https://me.csdn.net/cocoonyang)阅读数：2026








## 引言 




Node.js[1]的C/C++原生扩展实质上就是动态连接库[2]。Node.js在Windows环境中默认的编译器是VC；在Linux系统中，Node.js默认的编译器是gcc。开发C/C++原生扩展的方法众多。Node.js官方网站介绍了使用node-gypy[4]开发C/C++原生扩展。引用[3]也介绍了多种方法开发C/C++原生扩展，在然而 [3]介绍的方法并不适用于Windows环境。本文介绍分别使用nmake 和CMake编译Windows环境中node.js原生扩展的方法。




以构建fann_node扩展为例，本文介绍了在Windows操作系统中通过编写makefile使用VS 2015 nmake开发Node.js的C/C++原生扩展。在参考[5]的基础上，本文使用CMake[6]和VISUAL C++ 2010 EXPRESS在Windows操作系统中开发Node.js的C/C++原生扩展。本文使用CMake创建VC工程文件,在CMakeLists.txt中设置工程属性，简化操作过程。




## 使用node-gyp构建Node.js扩展

 node-gyp是一个为Node.js编译原生扩展的跨平台命令行工具[4]。它使用Node.js语言编写。 node-gyp能编译多个版本的原生扩展。在编译过程中，node-gyp只下载相应版本的头文件和其它必要文件(在我的机器里是存放在c:/users/youraccount/.node_gyp/nodeVesion/)，原生扩展的版本与当地系统中Node.js版本没有依赖关系。



## 使用NMAKE构建Node.js原生扩展





在VS 2015 developer command prompt环境中，使用nmake构建node.js原生扩展。


 文件结构

../   

  |_ build  

      |_ Release   // put fann.node here

  |_ node  

      |_  ...      // node.js header files of corresponding version

  |_ lib  

      |_ node.lib 

      |_ fanndouble.lib 

      |_ fannfloat.lib 

      |_ fannfixed.lib

  |_ inc  

      |_ fann.h  ...   // fann library head files

      |_ nodefann.h 

  |_ src  

      |_ addon.cc  

      |_ nodefann.cc  

  |_ tmp   

      |_ Makefile  

Makefile



```
# Target     
PROGRAM = fann.node

LIBS = "../lib/"  
    
NODE_LIB = node.lib 
FANN_LIB = fanndouble.lib fannfloat.lib fannfixed.lib    
    
node = "../node/"    
inc = "../inc/"    
src = "../src/"      
INCLUDEDIRS =  /I $(inc) /I $(node) /I $(LIBS)
   
# Flags    
CPPOPT = $(INCLUDEDIRS) /w /EHsc /D_CRT_SECURE_NO_DEPRECATE    
    
# Compiler     
cc = cl     
CFLAGS =     
     
LFLAGS = /LIBPATH:$(LIBS)     
     
# list of source files     
CPPSOURCES =  addon.cc  ynodefann.cc      
    
# expands to list of object files            
CPPOBJECTS = $(CPPSOURCES:.cc=.obj)     
      
all: $(PROGRAM)    
    
$(PROGRAM): $(CPPOBJECTS)    
    link.exe /out:../build/Release/$(PROGRAM) /DLL  $(CPPOBJECTS) $(LFLAGS) $(NODE_LIB) $(FANN_LIB)    
        
addon.obj:     
    $(cc) $(CPPOPT) /c ../src/addon.cc         
  
ynodefann.obj:     
    $(cc) $(CPPOPT) /c ../src/ynodefann.cc         
            
clean:      
    del $(CPPOBJECTS) $(PROGRAM)
```
This method works fine for node.js 4.3.0





## 使用CMAKE构建Node.js原生扩展

(下述样例只适用node_0.12等老版本 )

[5]只介绍了如何向Node.js添加C++扩展函数， 本文以向Node.js添加一个C++扩展类为例，详细介绍在Windows操作系统中开发Node.js的C/C++原生扩展的过程。具体步骤如下：

1、安装CMake和Windows的VC编译器。我使用的是VISUAL C++ 2010 EXPRESS。

2、安装Windows版的Node.js。 最好是下载Node.js源代码编译生成Node.js可执行文件和库文件。

3、配置Node.js的C/C++原生扩展编译环境(node_0.10)

     nodemodule

       |__include

            |__c-ares

                  |__ares.h

                  |__ares_version.h

           |__ev

                  |__ev.h

           |__uv-private

                  |__eio.h

                  |__ngx-queue.h

                  |__tree.h

                  |__uv-unix.h

                  |__uv-win.h

           |__node.h

           |__ node_buffer.h

           |__node_object_wrap.h

           |__node_version.h

           |__uv.h

           |__v8-debug.h

           |__v8-preparser.h

           |__ v8-profiler.h

           |__ v8-testing.h

           |__v8.h

           |__v8stdint.h

       |__lib

           |__node.lib

      |__src

     |__tmp

     |__CMakeLists.txt




4、创建VISUAL C++ 2010 EXPRESS的DLL工程。 我使用CMake创建的VC工程文件。


4.1  CMakeLists.txt 



```
#======================================
# Project Name 
#======================================
SET( TARGETFILENAME "foo" )
PROJECT( ${TARGETFILENAME} ) 

#======================================
#  CMake minimum version requirement setting
#======================================
cmake_minimum_required(VERSION 2.6)


#======================================
#  mdo setting
#======================================
IF(COMMAND CMAKE_POLICY)
    CMAKE_POLICY(SET CMP0003 NEW)
ENDIF(COMMAND CMAKE_POLICY) 
   
#======================================
# node library 
#======================================
SET(NODE_LIBRARIES node.lib) 
SET(NODE_LIBRARY_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/lib )

#====================================== 
#  node library setting
#======================================
FIND_LIBRARY(NODE_LIBRARY 
    NAMES  node 
    PATHS  ${CMAKE_CURRENT_SOURCE_DIR}/lib )
    
#======================================                     
# - Setting search route
#======================================
INCLUDE_DIRECTORIES(
    ${CMAKE_CURRENT_BINARY_DIR}  
    include 
    include/c-ares
    include/ev
    include/un-private
)                         

SET( src
    src/mymodule.cc
    src/module.cc
)

#======================================
#
# Setting include library path
#
# Make sure it come before the target
#
#======================================
LINK_DIRECTORIES( ${NODE_LIBRARY_DIRS} )

ADD_LIBRARY ( ${TARGETFILENAME} SHARED 
     ${src}
)

#======================================
# changing target file suffix to  .node   
#====================================== 
SET_TARGET_PROPERTIES(${TARGETFILENAME} PROPERTIES SUFFIX ".node")

#======================================
# link library setting
#======================================
TARGET_LINK_LIBRARIES( ${TARGETFILENAME}
    ${NODE_LIBRARIES}  
)
```

5、编写C++代码

5.1  mymodule.h 

node_0.10

#ifndef NODE_MYMODULE_H_
#define NODE_MYMODULE_H_

#include <node.h>
#include <v8.h>

class MyModule: public node::ObjectWrap
{
  public:
    MyModule();
    virtual ~MyModule()；

  public:
    static void InitializeModule(v8::Handle target);
    static v8::Handle New(const v8::Arguments& args)；

  public:
    static v8::Handle hello(const v8::Arguments& args);

  protected:
    static v8::Persistent constructor_template;
};

#endif // NODE_MYMODULE_H_




node_0.12




```cpp
#ifndef NODE_MYMODULE_H_  
#define NODE_MYMODULE_H_  
  
#include <node.h>  
#include <v8.h>  
  
class MyModule: public node::ObjectWrap  
{  
  public:  
    MyModule();  
    virtual ~MyModule()；  
  
  public:  
    static void InitializeModule(v8::Handle target);  
    static void New(const v8::FunctionCallbackInfo<v8::Value>& args)；  
  
  public:  
    static void hello(const v8::FunctionCallbackInfo<v8::Value>& args);  
  
  protected:  
    static v8::Persistent<v8::Function> constructor;  
};  
  
#endif // NODE_MYMODULE_H_
```

5.2  mymodule.cc 

node_0.10

#include "mymodule.h"

#include <iostream>

Persistent MyModule::constructor_template;

MyModule::MyModule() 
{
}

MyModule::~MyModule() 
{
}

void MyModule::InitializeModule(Handle target)
{
    HandleScope scope;

    Local < FunctionTemplate > t = FunctionTemplate::New(MyModule::New);
    constructor_template = Persistent::New(t);
    constructor_template->InstanceTemplate()->SetInternalFieldCount(1);
    constructor_template->SetClassName(String::NewSymbol("MyModule"));

    NODE_SET_PROTOTYPE_METHOD(constructor_template, "hello",
        MyModule::hello);

    target->Set(String::NewSymbol("MyModule"), constructor_template->GetFunction());
}


v8::Handle MyModule::New(const v8::Arguments& args)
{
    v8::HandleScope scope;
    MyModule* ptr = new MyModule();

    ptr->Wrap(args.Holder());

    return args.This();
}

v8::Handle MyModule::hello(const v8::Arguments& args)
{
    HandleScope scope;
    bool result = true;

    std::cout << "hello." << std::endl;

    return scope.Close(v8::Boolean::New(result));
}


node_0.12




```cpp
#include "mymodule.h"

#include <iostream>

Persistent<Function> MyModule::constructor;

MyModule::MyModule() 
{
}

MyModule::~MyModule() 
{
}

void MyModule::InitializeModule(Handle<Object> exports)
{
    Isolate* isolate = Isolate::GetCurrent();

    // Prepare constructor template
    Local<FunctionTemplate> constructor_template = FunctionTemplate::New(isolate, New);
    constructor_template->SetClassName(String::NewFromUtf8(isolate, "MyModule"));

    constructor_template->InstanceTemplate()->SetInternalFieldCount(1);



    NODE_SET_PROTOTYPE_METHOD(constructor_template, "hello",
        MyModule::hello);

    constructor.Reset(isolate, constructor_template->GetFunction());
    exports->Set(String::NewFromUtf8(isolate, "MyModule"), constructor_template->GetFunction());

}


v8::Handle MyModule::New(const FunctionCallbackInfo<Value>& args)
{
	Isolate* isolate = Isolate::GetCurrent();
	HandleScope scope(isolate);


	if (args.IsConstructCall()) {
		// Invoked as constructor: `new MyModule(...)`
		MyModule* obj = new MyModule( );
		obj->Wrap(args.This());
		args.GetReturnValue().Set(args.This());
	} else {
		// Invoked as plain function `<span style="font-family: Arial, Helvetica, sans-serif;">MyModule</span>(...)`, turn into construct call.
		const int argc = 1;
		Local<Value> argv[argc] = { args[0] };
		Local<Function> cons = Local<Function>::New(isolate, constructor);
		args.GetReturnValue().Set(cons->NewInstance(argc, argv));
	}


}

void MyModule::hello(const FunctionCallbackInfo<Value>& args)
{
	Isolate* isolate = Isolate::GetCurrent();
	HandleScope scope(isolate);

	bool result = true;

	std::cout << "hello." << std::endl;

	args.GetReturnValue().Set(v8::Boolean::New(isolate,result));
	return
}
```

5.3  module.cc 

node_0.10

/*
 * moule.cc
 *
 */

#include <node.h>
#include <v8.h>
#include "mymodule.h"
 
using namespace v8;
using namespace node;

//extern "C" 
void init(Handle target) {
    HandleScope scope;

    MyModule::InitializeModule(target);
};

NODE_MODULE(foo, init)    // 模块名字 "foo" 必须和动态库名字一致

node_0.12




```cpp
/*
 * moule.cc
 *
 */

#include <node.h>
#include "mymodule.h"
 
using namespace v8;

void init(v8::Handle<v8::Object> target) {

    MyModule::InitializeModule(target);
};

NODE_MODULE(foo, init)    // 模块名字 "foo" 必须和动态库名字一致
```

6、编译

7、测试

7.1  测试脚本代码



```java
var MyModule = require ("./foo").MyModule;

var theModule = new MyModule();

console.log( theModule.hello() ) ;
```







参考文献



[1] Node.js.   （2012年12月12日）  http://nodejs.org/

[2] addons.   （2012年12月12日）  http://nodejs.org/api/addons.html

[3]   （2012年12月12日）  https://www.cloudkick.com/blog/2010/aug/23/writing-nodejs-native-extensions/

[4] node-gpy.（2012年12月12日）  https://github.com/TooTallNate/node-gyp

[5]   （2012年12月12日）  http://cnodejs.org/topic/4f71e3688a04d82a3d264b5a 

[6]  CMake   （2012年12月12日）  http://www.cmake.org/



