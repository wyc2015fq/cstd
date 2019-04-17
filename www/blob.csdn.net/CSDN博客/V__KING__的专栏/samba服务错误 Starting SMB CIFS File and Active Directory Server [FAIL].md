# samba服务错误:Starting SMB/CIFS File and Active Directory Server. [FAIL] - V__KING__的专栏 - CSDN博客





2018年09月27日 12:21:51[v__king__](https://me.csdn.net/V__KING__)阅读数：441








出现错误：`Starting SMB/CIFS File and Active Directory Server. [FAIL]`

现象：samba服务两次启动，其中有一次没有起来显示fail

方案一：

```
sudo rm /etc/init/samba-ad-dc.conf
```

方案二：

```
echo manual |sudo tee /etc/init/samba-ad-dc.override
```



