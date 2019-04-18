# const_cast应用 - nosmatch的专栏 - CSDN博客
2012年05月09日 10:22:56[nosmatch](https://me.csdn.net/HDUTigerkin)阅读数：356标签：[class](https://so.csdn.net/so/search/s.do?q=class&t=blog)
个人分类：[C++](https://blog.csdn.net/HDUTigerkin/article/category/1142667)
用法：const_cast<type_id> (expression)
　　该运算符用来修改类型的const或volatile属性。除了const 或volatile修饰之外， type_id和expression的类型是一样的。
　　一、常量指针被转化成非常量指针，并且仍然指向原来的对象；
　　二、常量引用被转换成非常量引用，并且仍然指向原来的对象；
　　三、常量对象被转换成非常量对象。
　　Voiatile和const类试。举如下一例：
　　class B
　　{
　　public:
　　int m_iNum;
　　B() {}
　　};
　　void foo()
　　{
　　const B b1;
　　//b1.m_iNum = 100; //compile error
　　B b2 = const_cast<B&>(b1);
　　/* 也可以做如下转换，体现出转换为指针类型 */
　　B *b3 = const_cast<B*>(&b1);
　　/* 或者左侧也可以用引用类型，如果对b3或b4的数据成员做改变，就是对b1的值在做改变 */
　　B &b4 = const_cast<B&>(b1);
　　b2. m_iNum = 200; //fine?
　　}
　　int main()
　　{
　　foo();
　　return 0;
　　}
　　上面的代码编译时会报错，因为b1是一个常量对象，不能对它进行改变；
　　使用const_cast把它转换成一个非常量对象，就可以对它的数据成员任意改变。注意：b1和b2是两个不同的对象。
            
