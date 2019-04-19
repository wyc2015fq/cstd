# C++的反射和序列化 - 逍遥剑客 - CSDN博客
2012年03月29日 23:21:13[xoyojank](https://me.csdn.net/xoyojank)阅读数：6791
Gamasutra上有篇文章讲得挺细的:[Sponsored Feature: Behind the Mirror - Adding Reflection to C++](http://www.gamasutra.com/view/feature/6379/sponsored_feature_behind_the_.php)
- RTTI
- - 在Class声明后加入RTTI宏的做法几乎快成各种引擎的标配了, 谁叫C++的rtti太弱来着.
- 之前只是认为自己实现RTTI的原因是dynamic_cast的效率不高和对象工厂的需要, 原来关掉这个编译选项还可以减少占用的内存
- Field(Property)
- - 实现反射必须有一些属性成员的描述类, 这些信息目前看到三种实现方法
- - 使用工具/编译器中间信息等根据C++代码解析生成(Havok冒似是这种实现). 不推荐, 见原文
- 使用各种模板+宏像RTTI那样进行注册, 查询方便, 缺点是会增加启动时间和内存占用, 难于单步调试
- 使用访问者模式, 在需要查询信息时才创建, 编码比较灵活, 但是访问起来比较慢
- 一般都是记录成员变量相对于class/struct的offset和size, 再根据实例指针计算出内存地址
- Method
- - 目前还没有看到把方法也做了反射的引擎, 到是[cppreflect](http://www.garret.ru/cppreflection/docs/reflect.html)里有实现. 编码上越简洁越好, 复杂的可以使用脚本代替
- Clone
- - 做了反射的另一个好处就是对象可以深度拷贝, 这也是Prefab/Template的基础要求
- Serialization
- - 有了反射做序列化相当的方便, 如果没有的话就像GameBryo那样做也凑合
- 把Name(或CRC)与Value一起序列化后就可以做到版本的兼容, 对于成员属性改动频繁的情况很实用
- 对于重复性的数据的二进制序列化可以进行压缩, 如数组
- 对于内存Buffer的XML序列化, 可以使用base64编码成字符串
- Inheritance
- - 数据继承, 或者要数据拷贝, 就是对象实例之间的数据有继承关系, 只保存相对于父实例(Template)不同的数据
- Others
- - 以前做Delegate实现的时候只接触过成员函数指针, 没想到还有个成员变量指针, C++真是博大精深
- Enum和Container需要特殊对待
- 如果要反射到.net的PropertyGrid, 可以使用ICustomTypeDescriptor构造动态属性对象, 或者使用Reflection.Emit动态编译生成类型
