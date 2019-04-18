# 二十八、在storm上运行python程序 - jiangjingxuan的博客 - CSDN博客





2017年01月25日 11:51:25[jiangjingxuan](https://me.csdn.net/jiangjingxuan)阅读数：6332












![](http://www.shareditor.com/uploads/media/default/0001/01/thumb_117_default_big.jpeg)



storm是原生支持java的，但同时也支持其他语言，python和java一样有非常多且用起来更爽的类库，所以这一节我们尝试一下使用python来运行storm任务

请尊重原创，转载请注明来源网站[www.shareditor.com](http://www.shareditor.com)以及原始链接地址

## 安装lein



下载安装脚本
`wget https://raw.githubusercontent.com/technomancy/leiningen/stable/bin/lein`
把lein放到~/bin下并chmod +x lein增加可执行权限（默认我们的bash是会自动把~/bin加到PATH环境变量里的）

执行lein，自动下载安装所需的软件包



## 安装streamparser



如果没有安装pip（python包的管理工具），先安装pip

```
wget https://bootstrap.pypa.io/get-pip.py --no-check-certificate
python get-pip.py
```

这时pip就安装好了，执行
`pip -h`
能看到帮助信息



安装virtualenv命令
`pip install virtualenv`


安装python-dev，执行
`yum install python-devel`


安装streamparser，执行
`pip install streamparse`


注意：如果你是在直接使用root账户，那么需要在~/.bash_profile中添加

```bash
export LEIN_ROOT=1
```

这是因为在root账户下使用lein时会有警告提示并等待你输入回车才能继续，这样的话下面你执行sparse命令时会莫名其妙的被卡住

请尊重原创，转载请注明来源网站[www.shareditor.com](http://www.shareditor.com)以及原始链接地址

创建第一个项目，执行
`sparse quickstart wordcount`
运行：

```bash
cd wordcount
sparse run
```

先会有一段较长时间的编译过程，之后会不断输出log如下说明运行正常：

```
51774 [Thread-25] INFO  backtype.storm.task.ShellBolt - ShellLog pid:27202, name:count-bolt elephant: 1327
51776 [Thread-25] INFO  backtype.storm.task.ShellBolt - ShellLog pid:27202, name:count-bolt dog: 1335
51781 [Thread-27] INFO  backtype.storm.task.ShellBolt - ShellLog pid:27227, name:count-bolt cat: 1335
51783 [Thread-27] INFO  backtype.storm.task.ShellBolt - ShellLog pid:27227, name:count-bolt zebra: 1328
51784 [Thread-25] INFO  backtype.storm.task.ShellBolt - ShellLog pid:27202, name:count-bolt elephant: 1328
```

这都是在本地模拟运行，下面讲解如何部署到真正的storm集群运行



## 部署到storm并运行



修改config.json文件，配置好自己的storm集群，如下：

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
            "virtualenv_root": "/tmp/virtualenvs/"
        }
    }
}
```



然后执行sparse submit

等待较长时间之后如果出现了：Finished submitting topology: wordcount，说明部署成功



## 可能存在的问题



问题1：如果你在部署的单机storm上执行sparse submit还有可能报错：
`IOError: Local port: 6627 already in use, unable to open ssh tunnel to centos:6627`
这是因为端口冲突，解决方案就是你找到另外一台机器来安装streamparse并创建wordcount提交就可以了



问题2：如果你部署的storm是1.0.1版本，而sparse版本当前还不支持storm 1.0.1，因此即使部署上去了还是会报错不能正常执行，所以只能部署一个0.9.5版本的storm或者等待sparse什么时候支持1.0.1了

修改project.clj，把
`:dependencies  [[org.apache.storm/storm-core "0.9.5"]`
改成
`:dependencies  [[org.apache.storm/storm-core "1.0.1"]`





