# 安全知识&kerberos初识 - Soul Joy Hub - CSDN博客

2016年07月31日 18:05:47[卓寿杰_SoulJoy](https://me.csdn.net/u011239443)阅读数：1459


# 问题一

> 
kinit alice 

      beeline -u “jdbc:hive2://baogang2:10000/default;principal=hive/baogang2@TDH” 

  请问这个beeline连接到inceptor中之后，当前用户是谁？principal=hive/baogang2@TDH指的又是什么？

- 当前用户是baogang2
- principal=hive/baogang2@TDH指的是在baogang2的权限下使用hive

# 问题二

> 
尝试写一个访问kerberos安全的hdfs根目录的java程序 

  2015年8月1日上午10点 之前可以使用如下集群 

  172.16.1.190~172.16.1.192 manager节点在 172.16.1.190上 admin/admin

```java
public static void main(String agrs[]) throw Exception{
    final Configuration conf = new Configuration();
    conf.addResource(new Path("/etc/admin/conf/core-site.xml"));
    conf.addResource(new Path("/etc/admin/conf/hdfs-site.xml"));

    UserGroupInformation.setConfiguration(conf);

    UserGroupInformation ugi = UserGroupInformation.LoginUserFromKeytab("admin","admin.keytab");
    UserGroupInformation ugi = UserGroupInformation.LoginUserFromKeytabAndReturnUGI("admin","admin.keytab");
    UserGroupInformation ugi = UserGroupInformation.LoginUserFromPasswordAndReturnUGI("admin","admin");

    ugi.doAs(new PrivilegedActionException<Void>(){
        @Override
        public void run() throw Exception{
            FileSystem fs = FileSystem.get(conf);
            FileStatus[] files = fs.listStatus(new Path("/user"));
            if(files != null){
                for(FileStatus file : files){
                    System.out.println(file.getPath());
                }
            }
            return null;
        }
    });
}
```

# 问题三

> 
kerberos的用户的密码会在网络上传播吗？为什么？

不会。

![这里写图片描述](https://img-blog.csdn.net/20160731175802061)

![这里写图片描述](https://img-blog.csdn.net/20160731170059788)
- Client向KDC发送自己的身份信息和密钥，KDC从Database得到TGT和Ticket（包含Session Key）， 并用该密钥将其加密回复给Client。此时只有真正的Client才能用密钥对它解密，得到Ticket和TGT。

![这里写图片描述](https://img-blog.csdn.net/20160731175538068)

![这里写图片描述](https://img-blog.csdn.net/20160731170526788)
- Client用从解密出来的Ticket中的Session Key加密信息生成Authenticator和TGT一起也发送给KDC。
- KDC检查TGT和Authenticator，从Database得到TGS（包含生成新的Session Key，即Service与KDC之间的密钥），并用Authenticator将其加密回复给Client。
- 只有真正的Client才能用Authenticator对它解密，得到TGS，并用新的Session Key替换旧的。
- Client再用新的Session Key对TGS加密，并发送给Service（如HDHS）。Service再用Session Key对其解密，得到信息。

