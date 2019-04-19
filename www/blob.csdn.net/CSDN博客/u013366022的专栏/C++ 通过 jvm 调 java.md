# C++ 通过 jvm 调 java - u013366022的专栏 - CSDN博客
2017年10月26日 17:06:30[slitaz](https://me.csdn.net/u013366022)阅读数：151
# [Call Java functions from C++ by using Java Native Interface (JNI) on Ubuntu 12.04](http://tlab.hatenablog.com/entry/2013/01/12/125702)
[Linux](http://tlab.hatenablog.com/archive/category/Linux)[Programming](http://tlab.hatenablog.com/archive/category/Programming)
#### Code
/home/shinya/jni_example/Main.[java](http://d.hatena.ne.jp/keyword/java)
```java
public class Main {
  public static void main(String[] args) {
    System.out.println("Hello, world!");
    if (1 <= args.length) {
      System.out.println(args[0]);
    }
  }
}
```
/home/shinya/jni_example/main.cc
```cpp
#include <jni.h>
#include <cassert>
#include <iostream>
int main(int argc, char** argv) {
  const int kNumOptions = 3;
  JavaVMOption options[kNumOptions] = {
    { const_cast<char*>("-Xmx128m"), NULL },
    { const_cast<char*>("-verbose:gc"), NULL },
    { const_cast<char*>("-Djava.class.path=/home/shinya/jni_example"), NULL }
  };
  JavaVMInitArgs vm_args;
  vm_args.version = JNI_VERSION_1_6;
  vm_args.options = options;
  vm_args.nOptions = sizeof(options) / sizeof(JavaVMOption);
  assert(vm_args.nOptions == kNumOptions);
  JNIEnv* env = NULL;
  JavaVM* jvm = NULL;
  int res = JNI_CreateJavaVM(&jvm, reinterpret_cast<void**>(&env), &vm_args);
  if (res != JNI_OK) {
    std::cerr << "FAILED: JNI_CreateJavaVM " << res << std::endl;
    return -1;
  }
  const char* kClassName = "Main";
  jclass cls = env->FindClass(kClassName);
  if (cls == NULL) {
    std::cerr << "FAILED: FindClass" << std::endl;
    return -1;
  }
  const char* kMethodName = "main";
  jmethodID mid =
      env->GetStaticMethodID(cls, kMethodName, "([Ljava/lang/String;)V");
  if (mid == NULL) {
    std::cerr << "FAILED: GetStaticMethodID" << std::endl;
    return -1;
  }
  const jsize kNumArgs = 1;
  jclass string_cls = env->FindClass("java/lang/String");
  jobject initial_element = NULL;
  jobjectArray method_args = env->NewObjectArray(kNumArgs, string_cls, initial_element);
  jstring method_args_0 = env->NewStringUTF("Hello, Java!");
  env->SetObjectArrayElement(method_args, 0, method_args_0);
  env->CallStaticVoidMethod(cls, mid, method_args);
  jvm->DestroyJavaVM();
  return 0;
}
```
#### Check
$ cd /home/shinya/jni_example
$ javac Main.java
$ LIBPATH=/usr/lib/jvm/java-6-openjdk-amd64/jre/lib/amd64/server
$ export LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:${LIBPATH}
$ g++ -Wall main.cc -L${LIBPATH}-ljvm
$ ./a.out
Hello, world!
Hello, Java!
