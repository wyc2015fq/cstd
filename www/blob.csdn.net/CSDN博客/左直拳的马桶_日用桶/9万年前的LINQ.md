# 9万年前的LINQ - 左直拳的马桶_日用桶 - CSDN博客
2010年04月27日 16:45:00[左直拳](https://me.csdn.net/leftfist)阅读数：6491标签：[linq																[数据库																[string																[pascal																[equals																[list](https://so.csdn.net/so/search/s.do?q=list&t=blog)](https://so.csdn.net/so/search/s.do?q=equals&t=blog)](https://so.csdn.net/so/search/s.do?q=pascal&t=blog)](https://so.csdn.net/so/search/s.do?q=string&t=blog)](https://so.csdn.net/so/search/s.do?q=数据库&t=blog)](https://so.csdn.net/so/search/s.do?q=linq&t=blog)
个人分类：[.NET](https://blog.csdn.net/leftfist/article/category/94497)
LINQ出来已经9万年了，今天第一次用。
LINQ的作用看来是用于查询。说到查询，很自然想起SQL，数据库之类，但LINQ除了可以处理数据库的数据，**更主要的是用来处理字符串、数组、XML等对象的查找**。当初LINQ刚出来时，因为不了解，觉得很奇怪，数据库的操作早就被狂热的架构师们封装、区分了一层又一层，什么数据层，业务逻辑层，狗屁层，等等，现在来个LINQ,直接在代码里操作数据库，这不是扯淡吗？不是逆潮流而动吗？真是反人类、分科学、反他妈狗日的和谐啊，所以不学。
近来因为有找工作的压力，所以看了看，感觉视野比过去开阔了一些。高度决定视野，技术决定思维啊，知道了它的用途后，终于在工作中应用了一回。
应用场景：
记录用户的兴趣。我们准备了一大堆选项，比如，吃、喝、嫖、赌等等，在页面中以复选框（checkbox）来体现，用户可以多选。
显示这个页面的时候，很自然地，这堆选项都要输出，用户以前选过的，就打上勾。
思路：
数据一、**slove**,用户选过的数据，string类型，形如1-2-3：数字是ID值，中间以“-”分隔
数据二、**ctlist**，选项，List<>
    struct struLove//自定义结构，用于承载查询结果里的元素。不是非结构不可的。
    {
        public int Id;
        public string Name;
        public bool IsMyLove;
    }
……
string[] **arlove** = **slove**.Split('-');//形如1-2-3的字符串分拆成数组。ctlist已经是一个List<>。我示范的LINQ就是要联合arlove和ctlist，从中找出结果。
//传说中的LINQ开始
var setCt = from ct in**ctlist**
                      join love in **arlove** on ct.ID.ToString() equals love into gj
                      from subct in gj.DefaultIfEmpty()
                      select new struLove
                        {
                            Id = ct.ID,
                            Name = ct.Name,
                            IsMyLove = (subct == null ? false : true)
                        };
//传说中的LINQ结束
        this.cblDefault.Items.Clear();//cblDefault是<asp:CheckBoxList >控件
        foreach(var v in setCt)
        {
            ListItem item = new ListItem(v.Name,v.Id.ToString());
            item.Selected = v.IsMyLove;//用户以前选过的就打上勾
            this.cblDefault.Items.Add(item);
        }
这里面还用了左联接 DefaultIfEmpty()。要么不用，一用就高起点，呵呵。
如果不用LINQ，当然也没什么不可以，就两个嵌套循环，将用户的选中数据跟选项逐个比对。
以前学PASCAL的时候，有集合这种类型，后来在其他语言上都没发现。有了LINQ以后，感觉实现集合的功能比较容易了。
比较简单一点的：
```
int[] arB2 = new int[] { 0, 1, 2, 3, 4 };
        var setb2 = from b in arB2 
                        where b == 0
                        select b;
```
