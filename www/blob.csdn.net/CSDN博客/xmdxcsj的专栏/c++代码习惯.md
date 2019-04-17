# c++代码习惯 - xmdxcsj的专栏 - CSDN博客





2016年07月11日 22:05:44[xmucas](https://me.csdn.net/xmdxcsj)阅读数：555








[谷歌的代码规范](http://zh-google-styleguide.readthedocs.io/en/latest/google-cpp-styleguide/)中可以借鉴的一些规则

## 头文件
- 只有当函数只有10 行甚至更少时才会将其定义为内联函数
- 定义函数时，参数顺序为：输入参数在前，输出参数在后。 

dir/foo.cc 的主要作用是执行或测试dir2/foo2.h 的功能，foo.cc 中包含头文件的次序如下： 

dir2/foo2.h 

C 系统文件 

C++系统文件 

其他库头文件 

本项目内头文件
## 作用域
- 命名空间不使用using；命名空间将 除文件包含、全局标识的声明/定义以及类的前置声明外 的整个源文件封装起来

```cpp
// .h 文件
namespace mynamespace {
// 所有声明都置于命名空间中
// 注意不要使用缩进
class MyClass {
public:
    ...
    void Foo();
};
} // namespace mynamespace
// .cc 文件
namespace mynamespace {
// 函数定义都置于命名空间中
void MyClass::Foo() {
...
}
} // namespace mynamespace
```
- 局部变量那个声明的时候初始化，并且离第一次使用越近越好。
- 全局变量只允许内建类型，不能使用函数返回值初始化全局变量。

## 类
- 不在构造函数中做太多逻辑相关的初始化
- 仅在作为数据集合时使用struct
- 声明次序：public->protected->private
- 函数体尽量短小、紧凑，功能单一

## c++特性
- 按引用传递的参数必须加上const
- 禁止使用缺省函数参数
- 不要使用c++异常
- 禁止使用RTTI
- 使用static_cast<>()等C++的类型转换
- 

## 命名约定
- 文件名要全部小写，可以包含下划线（_）
- 类型命名每个单词以大写字母开头，不包含下划线，所有类型命名——类、结构体、类型定义（typedef）、枚举——使用相同约定
- 变量名一律小写，单词间以下划线相连，类的成员变量以下划线结尾
- 对全局变量没有特别要求，少用就好，可以以g_或其他易与局部变量区分的标志为前缀。
- 所有编译时常量（无论是局部的、全局的还是类中的）和其他变量保持些许区别，k 后接大写字母开头的单词
- 普通函数大小写混合，存取函数则要求与变量名匹配
- 命名空间的名称是全小写的
- 枚举值应全部大写，单词间以下划线相连：MY_EXCITING_ENUM_VALUE；枚举名称属于类型，因此大小写混合：UrlTableErrors。




