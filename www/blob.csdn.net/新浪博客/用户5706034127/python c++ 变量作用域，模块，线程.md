# python c++ 变量作用域，模块，线程_用户5706034127_新浪博客
||分类：[python](http://blog.sina.com.cn/s/articlelist_5706034127_2_1.html)|
int bar = 999;
int foo()
{
cout << bar << endl;
for (int bar = 0; bar < 5; bar++)
{
cout << bar << endl;
}
cout << bar << endl;
int bar = 535;
cout << bar << endl;
return 0;
}
void test()
{
int bar = 727222;
foo();
}
c++作用域按照大括号来划分，循环也算。
而且只是从定义的地方往前找。不是执行的地方。
def foo():
 for j in range(1):
 for i in range(9):
 print
i
 print i
python **变量的作用域是按照，类，函数，模块来的**，循环不算。
而且不是往前找，一个作用域内，定义一定要出现在使用操作之前。
locals() 返回局部变量
globals() 返回全局变量
foo.func_closure() 返回函数的闭包里面的变量
都是字典的形式返回的。
# A.py
def foo():
 global name
 print name
# B.py
from A import foo
name = 'xiaoming'
foo()
是错误的，LEGB，规则都是针对定义时刻的，
python每个模块，就相当于一个独立的执行单元。
它作为一个整体导入到其他模块中，除了函数参数，和导入到模块中的名字，
其它并不会和现有模块有任何的关联。
多线程和模块举例：
#nameFieldTest
name = 'Alice' 
# A.py
import nameFieldTest
def foo(i):
nameFieldTest.name = nameFieldTest.name+' hello '
 print
i,nameFieldTest.name
# B.py
import nameFieldTest, A,
 threading  #
threading 
是thread的高级模块
def bar(i):
nameFieldTest.name = nameFieldTest.name+' world '
 print
i,nameFieldTest.name
threads = []
 nloops =
5
 for i in
range(nloops-2):
t = threading.Thread(target=foo,args=(i,))
threads.append(t)
threads.append(threading.Thread(target=bar,args=(i+1,)))
threads.append(threading.Thread(target=bar,args=(i+1,)))
 for i in
range(nloops):
threads[i].start()
 for i in
range(nloops):
threads[i].join() 
多线程中只要不是函数内部的变量，就会起冲突。
导入模块，但是模块实体只有一个，其它的都是对本体的引用，而不是生成本地。
所以尽量也不要修改模块中的变量，会影响到其它使用该模块的地方。 
nameFieldTest是个本地变量，但是指向的是全局唯一的“nameFieldTest”模块，
nameFieldTest.name会修改全局唯一的模块，nameFieldTest = 2，则不会修改全局唯一模块，
只是本地变量指向了2，不在指向全局唯一“nameFieldTest”模块了。
