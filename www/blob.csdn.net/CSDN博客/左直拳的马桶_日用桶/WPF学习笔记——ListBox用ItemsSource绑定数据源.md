# WPF学习笔记——ListBox用ItemsSource绑定数据源 - 左直拳的马桶_日用桶 - CSDN博客
2014年05月10日 13:37:27[左直拳](https://me.csdn.net/leftfist)阅读数：36003
作为一个WPF初学者，感到困难重重。在网上想查个ListBox绑定数据源的示例，结果出来一大堆代码，一看心就烦。
我给个简洁一点的代码：
后台代码：
```
protected class UserItem
        {
            public UserItem(int Id, string Name, bool IsActived)
            {
                this.Id = Id;
                this.Name = Name;
                this.IsActived = IsActived;
            }
            public int Id{get;set;}
            public string Name { get; set; }
            public bool IsActived { get; set; }
            public string BackGround
            {
                get
                {
                    return IsActived
                        ? "/test;component/Assets/Images/UserItemNull.png"
                        : "/test;component/Assets/Images/UserItemNullg.png";
                }
            }
        }
        void Init()
        {
            Lst.ItemsSource = new List<UserItem> 
            {
                new UserItem(1,"张三",true)
                ,new UserItem(2,"李四",true)
                ,new UserItem(3,"赵五",true)
                ,new UserItem(4,"钱六",true)
                ,new UserItem(5,"孙七",false)
                ,new UserItem(6,"李八",false)
                ,new UserItem(7,"王九",false)
                ,new UserItem(8,"陈十",false)
                ,new UserItem(9,"吴万",false)
                ,new UserItem(10,"刘十八",false)
            };
        }
```
页面代码：
```
<Grid>
        <ListBox x:Name="Lst">
            <ListBox.ItemTemplate>
                <DataTemplate>
                    <Button MouseDoubleClick="Button_MouseDoubleClick">
                        <Grid>
                            <Image Source="{Binding Path=BackGround}" />
                            <TextBlock Text="{Binding Path=Name}" Margin="70 10" FontSize="18"></TextBlock>
                        </Grid>
                    </Button>
                </DataTemplate>
            </ListBox.ItemTemplate>
</Grid>
```
作为一个从业15年的平庸老程序员，不得不感叹一下，这么多年来，开发语言换了又换，又分层，又解耦，又模式，不断折腾，变换名词，其实我们现在在开发的软件，十年前也在搞，难道现在的开发效率提高啦？出来的东西比以前更棒？不见得吧。同时我们搞的这些小软件，生命周期不过一两年，有什么狗屁维护的问题呢？有什么修改起来会很麻烦的问题呢？未免想得太多。
但我们就是要被迫这样子不停地学习、学习，直到升为管理层，或被淘汰。
