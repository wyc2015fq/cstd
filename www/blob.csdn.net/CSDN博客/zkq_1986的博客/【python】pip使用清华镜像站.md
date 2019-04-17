# 【python】pip使用清华镜像站 - zkq_1986的博客 - CSDN博客





2019年04月13日 21:51:49[zkq_1986](https://me.csdn.net/zkq_1986)阅读数：26
个人分类：[程序设计语言](https://blog.csdn.net/zkq_1986/article/category/6534515)









## pypi 镜像使用帮助

pypi 镜像每 5 分钟同步一次。

### 临时使用

```
pip install -i https://pypi.tuna.tsinghua.edu.cn/simple some-package
```

注意，`simple` 不能少, 是 `https` 而不是 `http`

### 设为默认

升级 pip 到最新的版本 (>=10.0.0) 后进行配置：

```
pip install pip -U
pip config set global.index-url https://pypi.tuna.tsinghua.edu.cn/simple
```

如果您到 pip 默认源的网络连接较差，临时使用本镜像站来升级 pip：

```
pip install -i https://pypi.tuna.tsinghua.edu.cn/simple pip -U
```



