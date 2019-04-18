# 二十九、在storm上运行python程序（修正） - jiangjingxuan的博客 - CSDN博客





2017年01月25日 11:51:51[jiangjingxuan](https://me.csdn.net/jiangjingxuan)阅读数：2403












![](http://www.shareditor.com/uploads/media/default/0001/01/thumb_117_default_big.jpeg)



上一节部署python程序出现了一些错误，没能正确运行，本节基于稳定版storm做了修正，运行成功

请尊重原创，转载请注明来源网站[www.shareditor.com](http://www.shareditor.com)以及原始链接地址

## 部署0.9.5版本的storm集群



streamparse最新稳定版是基于0.9.5版本的storm的，所以我们需要把storm集群的版本回退到0.9.5，方法如下：
`wget http://apache.fayea.com/storm/apache-storm-0.9.5/apache-storm-0.9.5.tar.gz`
解压后修改conf/storm.yaml文件，添加如下配置项：

```
storm.zookeeper.servers:
    - "127.0.0.1"
nimbus.seeds: ["127.0.0.1"]
supervisor.slots.ports:
    - 6700
    - 6701
    - 6702
    - 6703
```

启动，执行：

```
[root@centos7vm apache-storm-0.9.5]# ./bin/storm nimbus &
[root@centos7vm apache-storm-0.9.5]# ./bin/storm supervisor &
[root@centos7vm apache-storm-0.9.5]# ./bin/storm ui &
```

启动需要花费数秒钟时间，直到打开http://localhost:8080能正常显示web页说明启动正常



## 创建wordcount并修改配置



确定安装的streamparse版本是

```
[root@centos7vm tmp]# sparse --version
sparse 2.1.4
```

执行
`[root@centos7vm tmp]# sparse quickstart wordcount`
生成了wordcount目录，进入wordcount目录修改config.json文件

请尊重原创，转载请注明来源网站[www.shareditor.com](http://www.shareditor.com)以及原始链接地址

把nimbus和workers配置成你的storm机器（填写别名或ip，需要提前建立好ssh host的无密码登陆），比如我的是：

```
{
    "library": "",
    "topology_specs": "topologies/",
    "virtualenv_specs": "virtualenvs/",
    "envs": {
        "prod": {
            "user": "",
            "nimbus": "centos",
            "workers": ["centos"],
            "log": {
                "path": "",
                "max_bytes": 1000000,
                "backup_count": 10,
                "level": "info"
            },
            "virtualenv_root": "/root/tmp/wordcount/virtualenvs/"
        }
    }
}
```



## 生成jar包



执行
`[root@centos7vm wordcount]# sparse jar`
会看到生成了_build/wordcount-0.0.1-SNAPSHOT-standalone.jar文件



## 部署任务



执行
`[root@centos7vm wordcount]# storm jar _build/wordcount-0.0.1-SNAPSHOT-standalone.jar streamparse.commands.submit_topology topologies/wordcount.clj`


查看日志

```
[root@centos7vm wordcount]# tail /data/apache-storm-0.9.5/logs/worker-6700.log
2016-05-16T08:53:38.579+0800 b.s.t.ShellBolt [INFO] ShellLog pid:74811, name:count-bolt elephant: 656
2016-05-16T08:53:38.580+0800 b.s.t.ShellBolt [INFO] ShellLog pid:74811, name:count-bolt dog: 690
2016-05-16T08:53:38.585+0800 b.s.t.ShellBolt [INFO] ShellLog pid:74811, name:count-bolt zebra: 659
2016-05-16T08:53:38.588+0800 b.s.t.ShellBolt [INFO] ShellLog pid:74811, name:count-bolt dog: 691
2016-05-16T08:53:38.595+0800 b.s.t.ShellBolt [INFO] ShellLog pid:74811, name:count-bolt elephant: 657
2016-05-16T08:53:38.596+0800 b.s.t.ShellBolt [INFO] ShellLog pid:74811, name:count-bolt dog: 692
2016-05-16T08:53:38.604+0800 b.s.t.ShellBolt [INFO] ShellLog pid:74811, name:count-bolt elephant: 658
2016-05-16T08:53:38.606+0800 b.s.t.ShellBolt [INFO] ShellLog pid:74811, name:count-bolt dog: 693
2016-05-16T08:53:38.635+0800 b.s.t.ShellBolt [INFO] ShellLog pid:74811, name:count-bolt zebra: 660
2016-05-16T08:53:38.640+0800 b.s.t.ShellBolt [INFO] ShellLog pid:74811, name:count-bolt dog: 694
```

看到如上日志，说明正常运行了




