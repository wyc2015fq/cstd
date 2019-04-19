# C#添加删除IIS的主机头 - 左直拳的马桶_日用桶 - CSDN博客
2011年12月15日 17:12:35[左直拳](https://me.csdn.net/leftfist)阅读数：3080
用c#可以管理IIS的主机头，按需要动态添加或删除主机头。
```
#region 添加主机头
        public static void AddHostHeader(int siteid, string ip, int port, string domain)//增加主机头（站点编号.ip.端口.域名）
        {
            DirectoryEntry site = new DirectoryEntry("IIS://localhost/W3SVC/" + siteid);
            PropertyValueCollection serverBindings = site.Properties["ServerBindings"];
            string headerStr = string.Format("{0}:{1}:{2}", ip, port, domain);
            if (!serverBindings.Contains(headerStr))
            {
                serverBindings.Add(headerStr);
            }
            site.CommitChanges();
        }
        #endregion
        #region 删除主机头
        public static void DeleteHostHeader(int siteid, string ip, int port, string domain)//删除主机头（站点编号.ip.端口.域名）
        {
            DirectoryEntry site = new DirectoryEntry("IIS://localhost/W3SVC/" + siteid);
            PropertyValueCollection serverBindings = site.Properties["ServerBindings"];
            string headerStr = string.Format("{0}:{1}:{2}", ip, port, domain);
            if (serverBindings.Contains(headerStr))
            {
                serverBindings.Remove(headerStr);
            }
            site.CommitChanges();
        }
        #endregion
```
其中，站点编号是个数字。我不知道有什么好的方法可以获取这个编号，我的方法是看日志存放在哪个目录。如图
![](http://hi.csdn.net/attachment/201112/15/0_13239402123X39.gif)
红色部分数字，就是该网站的编号。一般地，默认网站是1。
IP，一般这里不用指定，用字符串“”表示就可以了，代表未分配。
那么调用的方法就是
`AddHostHeader(35783205, "", 80, “www.test.com”)`
注意：域名这里前后不能存在空格，否则网站不能启动，提示“参数错误”。所以最好的方法是添加前先处理一下：
```
void AddIt(string siteId,string domain)
        {
            string mydomain = domain.Trim();
            AddHostHeader(siteId, "", 80, mydomain);
        }
```
