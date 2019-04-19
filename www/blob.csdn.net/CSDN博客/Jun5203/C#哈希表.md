# C#哈希表 - Jun5203 - CSDN博客
2019年02月14日 11:39:34[贾俊浩_Ellen](https://me.csdn.net/Ellen5203)阅读数：65
个人分类：[【C#】](https://blog.csdn.net/Ellen5203/article/category/8309127)
所属专栏：[C#](https://blog.csdn.net/column/details/29824.html)
遇见好几次哈希表，但都不怎么明白，今天就让我明白一回，下面跟着小编来学习吧！
**芝士**
- **哈希表以键值对的形式存值，key—键，Value—值**
- **键值对均是object类型**
例：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190214085141627.jpg)
- **键值对中的键就是为了找数据用的，必须提供，不允许重复**
例：
```
Hashtable ht = new Hashtable();
            ht.Add(123, 123);
            ht.Add("老宋", 18);
            ht.Add('x', 12);
            ht.Add("老王", "老付");
            Console.WriteLine("添加成功");
            Console.ReadKey();
```
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190214083547399.jpg)
运行完上述代码后再加一行代码`ht.Add("老王", "小杨");`
```
Hashtable ht=new Hashtable();
            ht.Add(123, 123);
            ht.Add("老宋",18);
            ht.Add('x',12);
            ht.Add("老王","老付");
            ht.Add("老王", "小杨");
            Console.WriteLine("添加成功");
            Console.ReadKey();
```
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190214083524687.jpg)
通过这个例子，我们发现在加入`ht.Add("老王", "小杨");`后，程序报异常，这是为什么呢？因为在程序中已经存在一个“老王”的键，而又加入一个“老王”的键，键相同所以报异常。这也说明在哈希表中，key不能相同，一旦相同肯定会报异常。
- **Hashtable使用键作为寻找的方式，是一种无序的结构（显示的时候可能是无序的）**
- **使用<hashtable实例名>[键]，将返回object类型，得到由键对应的数据，也就是说知道key，也就知道了Value，如果删掉key，Value也就没有了**
例：
```
Hashtable ht = new Hashtable();
            ht.Add("小赵", "老马");
            ht.Add(12,1002);
            ht.Add("小王",132);
            foreach (var key in ht.Keys )
            {
                Console.WriteLine("key{0}-----value{1}",key,ht[key]);
            }
            Console.WriteLine("添加成功");
            Console.ReadKey();
```
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190214111434317.jpg)
- 
**强转，使用里氏代换原则**
- 
**补充**
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190214111726959.jpg)
不知道存的Key是什么类型
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190214112435252.png)
方法：先写上var，然后进行调试就会显示出所代表的类型，然后进行更改。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190214112811351.png)
**小编有话说**
我们每走一步，都是一个新的起点，这一个个起点连接成我们一生的轨迹。不要害怕开始，经历了起步时的艰难，方能产生飞跃的嬗变；不要畏惧结束，所有的结局都是一个新的开端。到头来我们会发现，人生如圆，终点亦是起点。不要奢望太多，得到的终归要失去；不要敬畏太甚，能够主宰你的，永远是你自己。
