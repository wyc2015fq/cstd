# SEL类型 - LC900730的博客 - CSDN博客
2017年06月26日 17:52:18[lc900730](https://me.csdn.net/LC900730)阅读数：202标签：[oc](https://so.csdn.net/so/search/s.do?q=oc&t=blog)
个人分类：[oc](https://blog.csdn.net/LC900730/article/category/6990658)
什么是SEL类型？ 
1.SEL类型代表着方法的签名，在类对象的方法列表中存储着该签名与方法代码的对应关系； 
2.每个类的方法列表中都存储在类对象中 
3.根据一个SEL对象就可以找到方法的地址，进而调用方法； 
4.SEL定义： 
typedef struct objc_selector *SEL; 
[p test]; 
将test包装成sel类型的数据
```
Person.h
@property int age;
main.m中：
SEL sel=@selector(setAge:);
Person *p=[Person new];
//判断有没有开头的-方法
//如果实现了setAge方法，就会返回YES
BOOL flag=[p respondsToSelector:sel];
NSLog(@"flag=%i",flag); //flag=1
```
//如果是通过类来调用，那么会判断有没有实现+开头的方法，如果是对象调用，就会判断有没有实现-开头的方法
第二个作用：
```java
SEL sel2=@selector(demo);
Person *p=[Person new];
//调用p对象中的sel类型的方法
//没有参数的情况下
[p performSelector:sel2];
//有参数的情况下(方法的形参必须接受的是一个对象)
[p performSelector:sel withObject:@"138383838"]; //一个参数
[p performSelector:sel withObject:@"138383838" withObject:@"abcdg"];
```
第三个作用： 
调用传入对象的指定方法
```
Person.m
-(void)makeObject:(id)obj andSel:(SEL)sel{
    [obj performSelector:sel];
}
main.m中
Car *c=[Car new];
SEL sel=@selector(run);
Person *p=[Person new];
[p makeObject:c andSel:sel];
```
