# J2ME 的优化措施 - 逍遥剑客 - CSDN博客
2007年07月21日 15:24:00[xoyojank](https://me.csdn.net/xoyojank)阅读数：1516标签：[j2me																[优化																[数据结构																[byte																[嵌入式																[string](https://so.csdn.net/so/search/s.do?q=string&t=blog)](https://so.csdn.net/so/search/s.do?q=嵌入式&t=blog)](https://so.csdn.net/so/search/s.do?q=byte&t=blog)](https://so.csdn.net/so/search/s.do?q=数据结构&t=blog)](https://so.csdn.net/so/search/s.do?q=优化&t=blog)](https://so.csdn.net/so/search/s.do?q=j2me&t=blog)
个人分类：[游戏逻辑](https://blog.csdn.net/xoyojank/article/category/259557)
                常见的性能瓶颈：
1.文件的读写和网络的操作
2.CPU
3.内存
4.同步
5.异常
6.屏幕绘制
一、程序设计和编码
1.对象的生成和大小的调整
  （1）尽可能的使用静态变量
          如果类中的变量不会随他的实例变化而变化，就定义为静态的，从而使它所有的实例都共享这个变量
  （2）不要对已生成的变量做过多的改变
          如尽可能不要过多改变String对象的值
  （3）生成静态对象时，要分配给它合理的空间
          一些数据结构的初始化大小要合理
  （4）避免生成不常使用或生命周期短的对象或变量
         应该定义一个对象缓冲池，这样开销小得多
  （5）只在对象作用范围内进行初始化
         局部使用的变量尽量拿到局部范围来定义和初始化
2.异常
  尽量避免使用try/catch，能使用if,while代替就代替；
  尽可能的重用已经存在的异常对象。因为生成一个异常对象要消耗大部分的时间
3.输入和输出
  使用输入输出缓冲，尽可能的多使用缓存
  OutStream比Writer类开销小，因为它要实现UTF8到byte的转换
4.一些经验技巧
  使用局部变量
  图形重绘只绘制变化的地方（如“脏矩形技术”）
  避免get/set方法来访问变量
  避免在同一个循环中生成同一个变量或调用同一个函数
  尽可能的使用static,final,private等关键字
二、优化内存的使用
1.代码优化
  //a不为空
  a = new A();
  这样可能就会产生溢出，因为对象仍存在
  a = null;
  a = new A();
  对于经常使用的对象，如声音处理类，最好是用工厂方法来产生唯一对象
2.图片优化
  颜色位数能低则低
  多张规格一样的图片合并成一张
  大尺寸图片，可以分块载入，如地图
3.基本类型byte,int,long,float,double什么的，能省一点是点
  能不使用浮点数就不使用，因为大多数嵌入式处理器没有浮点运算的协处理器
三、减少应用程序体积
  减少类
  限制使用接口
  使用无名包
  使用源代码预处理器进行优化
  使用混淆器
  使用系统库，能不自己实现的就不自己实现
还有，不要过分的使用面向对象的设计方法，在J2ME里，效率是第一位的            
