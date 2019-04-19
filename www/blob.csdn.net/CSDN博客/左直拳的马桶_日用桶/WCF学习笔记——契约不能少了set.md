# WCF学习笔记——契约不能少了set - 左直拳的马桶_日用桶 - CSDN博客
2014年05月23日 14:30:37[左直拳](https://me.csdn.net/leftfist)阅读数：1918
我定义的WCF契约里，有一个类，里面的属性，有一个因为只读，所以只写了个get。结果客户端就报错。
```
[DataContract]
    public class UserItem
    {
        public UserItem(int Id, string Name, bool IsActived)
        {
            this.Id = Id;
            this.Name = Name;
            this.IsActived = IsActived;
        }
        [DataMember]
        public int Id { get; set; }
        [DataMember]
        public string Name { get; set; }
        [DataMember]
        public bool IsActived { get; set; }
        [DataMember]
        public string BackGround
        {
            get
            {
                return IsActived
                    ? "UserItemNull.png"
                    : "UserItemNullg.png";
            }
        }
    }
```
刚开始，以为是构造函数的存在，但测试发现不是。
最后发现是最后这个属性"BackGround"没有set！
既然如此，补上一个呗：
```
[DataContract]
    public class UserItem
    {
        public UserItem(int Id, string Name, bool IsActived)
        {
            this.Id = Id;
            this.Name = Name;
            this.IsActived = IsActived;
        }
        [DataMember]
        public int Id { get; set; }
        [DataMember]
        public string Name { get; set; }
        [DataMember]
        public bool IsActived { get; set; }
        [DataMember]
        public string BackGround
        {
            get
            {
                return IsActived
                    ? "UserItemNull.png"
                    : "UserItemNullg.png";
            }
            private set
            {
                ;
            }
        }
    }
```
问题解决。            
