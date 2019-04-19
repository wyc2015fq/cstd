# C#中protected的用法 - Jun5203 - CSDN博客
2019年02月13日 09:01:20[贾俊浩_Ellen](https://me.csdn.net/Ellen5203)阅读数：173
所属专栏：[C#](https://blog.csdn.net/column/details/29824.html)
我们现在学到的访问修饰符有三个，public、private、protected，其中public和private很好理解，但是protected应该如何使用呢？下面就跟着小编学习一下吧！
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190213084138106.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0VsbGVuNTIwMw==,size_16,color_FFFFFF,t_70)
根据以上代码，提示的错误为“无法通过“People”类型的限定符访问受保护的成员“[People.Name](http://People.Name)”；限定符必须是“Man”类型(或者从该类型派生)	”，大家对此肯定会有疑惑，子类不是继承了父类吗？为什么不能调用父类的属性？
这是因为`People a=new People();`是在子类的方法中生成的，而不是子类的实例，无法保护结构成员。那如何解决这个问题呢？请往下看！
```
class People
    {
        private string name;
        protected  string Name { get; set; }
    }
    class Man : People
    {
        public void printTest()
        {
            Man b=new Man();
            b.Name = "旺财";
        }
    }
```
大家找出其中的不同来了吗？
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190213085003275.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0VsbGVuNTIwMw==,size_16,color_FFFFFF,t_70)
在次示例中，Man是从People派生的。因此，可以从派生类直接访问基类的受保护成员。由此我们可以得出结论“受保护成员在其所在的类中可由派生类实例访问”。
