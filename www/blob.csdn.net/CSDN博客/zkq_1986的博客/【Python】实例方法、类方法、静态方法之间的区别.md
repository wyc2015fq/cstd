# 【Python】实例方法、类方法、静态方法之间的区别 - zkq_1986的博客 - CSDN博客





2018年01月08日 09:40:08[zkq_1986](https://me.csdn.net/zkq_1986)阅读数：145









在python中，方法分为三类实例、类、静态方法。代码如下：




> 

　class Test(object):





    def InstanceFun(self):


        print("InstanceFun");


        print(self);




    @classmethod


    def ClassFun(cls):


        print("ClassFun");


        print(cls);




    @staticmethod


    def StaticFun():


        print("StaticFun");








t = Test();　　　　　　　


t.InstanceFun();　　　　　　＃ 输出InstanceFun，打印对象内存地址“<__main__.Test object at 0x0293DCF0>”


Test.ClassFun();                       # 输出ClassFun，打印类位置 <class '__main__.Test'>


Test.StaticFun();                        # 输出StaticFun


t.StaticFun();                              # 输出StaticFun


t.ClassFun();                             # 输出ClassFun，打印类位置 <class '__main__.Test'>


Test.InstanceFun();                  # 错误，TypeError: unbound method instanceFun() must be called with Test instance as first argument



Test.InstanceFun(t);                 # 输出InstanceFun，打印对象内存地址“<__main__.Test object at 0x0293DCF0>”


t.ClassFun(Test);                      # 错误   classFun() takes exactly 1 argument (2 given)   







可以看到，在python中，两种方法的主要区别在于参数。实例方法隐含的参数为类实例self，而类方法隐含的参数为类本身cls。

静态方法无隐含参数，主要为了类实例也可以直接调用静态方法。


　　所以逻辑上类方法应当只被类调用，实例方法实例调用，静态方法两者都能调用。主要区别在于参数传递上的区别，实例方法悄悄传递的是self引用作为参数，而类方法悄悄传递的是cls引用作为参数。


　　python实现了一定的灵活性使得类方法和静态方法，都能够被实例和类二者调用。




实例能调用所有方法，类只能调用类和静态方法。



