# iOS开发基础：开发两年的你也不会写的Block - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2016年06月24日 11:37:47[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：711
![](http://upload-images.jianshu.io/upload_images/2025746-113352fcbb42fbcf.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
阅读本文之前，请尝试写出一下问题：
- 声明一个Block，并调用它。
- 声明一个Block型的属性。
- 声明一个方法，接受一个Block型的参数，并写出调用时传入的Block实参。
- 实现一个Block的递归调用（Block调用自己）。
- 实现一个方法，将Block作为返回值。
现在拿出笔纸，关上屏幕，20分钟后见...
欢迎回来！如果你未能写出以上问题的答案，本文将给你答案。
## 本文使用以下符号，含义列举如下：
- `return_type` 表示返回的对象/关键字等(通常是`void`)
- `blockName` 表示block的名称
- `var_type` 表示参数的类型
- `varName` 表示参数名称
## 声明一个Block，作为局部变量：
```bash
return_type (^blockName)(var_type) = ^return_type (var_type varName) {
    // ...
};
blockName(var);
```
## 声明一个Block型的属性
注意与局部变量的声明比较，有细微差别，你能指出吗？
```bash
@property (copy) return_type (^blockName) (var_type);
```
## 在定义方法时，声明Block型的形参
这里提供的是Block的定义
```bash
- (void)yourMethod:(return_type (^)(var_type))blockName;
```
## Block作为实参
调用方法时，传入Block，这里需要的是Block的具体实现
```bash
[someObject doSomethingWithBlock: ^return_type (var_type varName)
{
    //...
}];
```
如果已经定义了局部变量的Block，也可以直接传入名称。
## 匿名Block
Block实现时，如上文的局部变量和实参，等号右边就是一个匿名Block，它没有`blockName`：
```bash
^return_type (var_type varName)
{
    //...
};
```
## typedef Block
利用`typedef`简化Block的声明：
```bash
typedef return_type (^BlockTypeName)(var_type);
```
然后可用`BlockTypeName`定义Block，类似类名的使用方法：
```bash
BlockTypeName aBlock = ^return_type (var_type) {
    //...
}
```
## 内联 Block
这种形式并不常用，匿名Block声明后立即被调用：
```bash
^return_type (var_type varName)
{
    //...
}(var);
```
内联 Block可用于代码分块，提高代码可读性，功能类似大括号的代码块，其它功能非常有限：
```
{
   //...
}
```
## 递归调用Block
Block内部调用自身，递归调用是很多算法基础，特别是在无法提前预知循环终止条件的情况下。**注意** 由于Block内部引用了自身，这里必须使用`__block`避免保留环问题。
```bash
__block return_type (^blockName)(var_type) = [^return_type (var_type varName)
{
    if (returnCondition)
    {
        blockName = nil;
        return;
    }
    // ...
    blockName(varName);
} copy];
blockName(varValue);
```
## Block作为返回值
方法的返回值是一个Block，可用于一些“工厂模式”的方法中：
```bash
- (return_type(^)(var_type))methodName
{
    // ...
}
```
文／溪石（简书作者）
原文链接：http://www.jianshu.com/p/dc5ced9bd7e7
著作权归作者所有，转载请联系作者获得授权，并标注“简书作者”。
