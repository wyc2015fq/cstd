# 如何把自己的Jar包上传到 maven 官方仓库中 - z69183787的专栏 - CSDN博客
2018年09月17日 12:00:00[OkidoGreen](https://me.csdn.net/z69183787)阅读数：456
[https://www.sojson.com/blog/250.html](https://www.sojson.com/blog/250.html)
之前有同学在本站发布过“[Maven的Mirror和Repository 的详细讲解](http://www.sojson.com/blog/168.html)”，这种我们自己搭建仓库和私服，一般这种是服务于公司或者团体。但是针对小“ 团伙 ”或者单兵作战，或者我们希望开源，开发给其他人去用，那么我们可以直接提交到官方（`maven.org` ），然后`dependency` 去引入即可，如下图：
![](https://cdn.www.sojson.com/file/17-09-12-12-48-08/doc/0856314579)
当然，上面是已经开源好的，在Maven公共库里面是存在的。
# 一、Maven中央仓库提交 · 前奏准备
### 开始要注意这个几个Maven相关地址：
① [https://issues.sonatype.org](https://issues.sonatype.org/) 工单管理地址，就是申请上传资格和`groupId` 的地方。
② [https://oss.sonatype.org/#welcome](https://oss.sonatype.org/#welcome)  构件仓库，把jar包上传到这里，`Release` 之后就会同步到maven中央仓库。
③ [http://search.maven.org/](http://search.maven.org/)  最终表现在可以在这里搜索到。
部分网址第一次打开有点慢，请稍等。
# 二、创建工单
从[https://issues.sonatype.org](https://issues.sonatype.org/) 注册一个帐号（已经有请忽略），账号密码要记住，下面登录账号密码还要用这个，后续还要配置  [maven](http://www.sojson.com/tag_maven.html)  的`setting.xml` 中，然后创建一个工单，如下图（记住是先注册，下面是创建工单的图）。
![](https://cdn.www.sojson.com/file/17-09-12-13-02-23/doc/3298378353)
记住是先注册，再创建。
**其中需要注意的地方：**
①`Group Id` ，唯一标识，我用的是`com.github.xxxxx`  如果用的是其他的比如：`com.sojson.core` 之类的，管理员会问你这个是不是属于你的网站，告诉他就可以了（用`com.github.xxxxx` 这种比较方便）。官方文档：[http://central.sonatype.org/pages/choosing-your-coordinates.html](http://central.sonatype.org/pages/choosing-your-coordinates.html)
② `ProjectURL` ，填项目源码的地址，官方有一篇文章说，如果不想公布源码，那么填写一个只含`README` 的项目的地址就可以了。
*其实管理员主要就是审核Group Id，其他的不重要
然后点击左侧列表中的`Resolved recently` 可以找到你刚创建的工单。
![](https://cdn.www.sojson.com/file/17-09-12-13-06-55/doc/5892555936)
这时你的工单的状态`Status` 是`Open` ，等到审核通过状态会变为`RESOLVED` ，第一次审核要一天左右 ，因为时差原因，他们工作时间是我们的晚上，之后在创建工单如果`GroupId` 满足基本要求基本就是秒过。
# 三、配置Maven Settings.xml & pom.xml
1.配置  [Maven](http://www.sojson.com/tag_maven.html)  中的`conf/settings.xml`  ，然后在`servers` 中添加如下代码。
```java
```
- `<server>`
- `<id>sonatype-nexus-snapshots</id>`
- `<username>https://issues.sonatype.org的账号</username>`
- `<password>https://issues.sonatype.org的密码</password>`
- `</server>`
- `<server>`
- `<id>sonatype-nexus-staging</id>`
- `<username>https://issues.sonatype.org的账号</username>`
- `<password>https://issues.sonatype.org的密码</password>`
- `</server>`
2.配置使用项目中的 `pom.xml`
```java
```
- `<parent>`
- `<groupId>org.sonatype.oss</groupId>`
- `<artifactId>oss-parent</artifactId>`
- `<version>7</version>`
- `</parent>`
- `<licenses>`
- `<license>`
- `<name>The Apache Software License, Version 2.0</name>`
- `<url>http://www.apache.org/licenses/LICENSE-2.0.txt</url>`
- `<distribution>repo</distribution>`
- `</license>`
- `</licenses>`
- `<!--写你自己的，这只是个例子-->`
- `<scm> `
- `<url> https://github.com/CallThink/Tools</url>`
- `<connection>https://github.com/CallThink/Tools.git</connection>`
- `<developerConnection>https://github.com/CallThink/Tools</developerConnection>`
- `</scm>`
- `<!--写你自己的，这只是个例子-->`
- `<developers> `
- `<developer>`
- `<name>changfeng</name>`
- `<email> changfeng @allytel.com.cn</email>`
- `<url>https://github.com/CallThink/Tools</url>`
- `</developer>`
- `</developers>`
# 四、GPG环境
我们需要一个GPG环境，用来对上传的文件进行加密和签名，保证你的jar包不被篡改，下载地址：[https://www.gpg4win.org/](https://www.gpg4win.org/)
安装完成后 命令行窗口中输入 gpg –version 查看是否安装成功
成功后执行下列命令：
```java
```
- `C:\Windows\system32>gpg --gen-key`
- `gpg (GnuPG) 2.0.30; Copyright (C) 2015 Free Software Foundation, Inc.`
- `This is free software: you are free to change and redistribute it.`
- `There is NO WARRANTY, to the extent permitted by law.`
- `Please select what kind of key you want:`
- `(1) RSA and RSA (default)`
- `(2) DSA and Elgamal`
- `(3) DSA (sign only)`
- `(4) RSA (sign only)`
- `Your selection? 1`
- `RSA keys may be between 1024 and 4096 bits long.`
- `What keysize do you want? (2048) 2048`
- `Requested keysize is 2048 bits`
- `Please specify how long the key should be valid.`
- `0 = key does not expire`
- `<n> = key expires in n days`
- `<n>w = key expires in n weeks`
- `<n>m = key expires in n months`
- `<n>y = key expires in n years`
- `Key is valid for? (0) 0`
- `Key does not expire at all`
- `Is this correct? (y/N) y`
- `GnuPG needs to construct a user ID to identify your key.`
- `Real name: changfeng`
- `Email address: 1010667394@qq.com`
- `Comment: changfeng-chang`
- `You selected this USER-ID:`
- `"changfeng (changfeng-chang) <1010667394@qq.com>"`
- `Change (N)ame, (C)omment, (E)mail or (O)kay/(Q)uit? o`
- `You need a Passphrase to protect your secret key.`
- `We need to generate a lot of random bytes. It is a good idea to perform`
- `some other action (type on the keyboard, move the mouse, utilize the`
- `disks) during the prime generation; this gives the random number`
- `generator a better chance to gain enough entropy.`
- `We need to generate a lot of random bytes. It is a good idea to perform`
- `some other action (type on the keyboard, move the mouse, utilize the`
- `disks) during the prime generation; this gives the random number`
- `generator a better chance to gain enough entropy.`
- `gpg: key EFFF81F9 marked as ultimately trusted`
- `public and secret key created and signed.`
- `gpg: checking the trustdb`
- `gpg: 3 marginal(s) needed, 1 complete(s) needed, PGP trust model`
- `gpg: depth: 0 valid: 3 signed: 0 trust: 0-, 0q, 0n, 0m, 0f, 3u`
- `pub 2048R/EFFF81F9 2017-09-12`
- `Key fingerprint = B245 4560 4E6F EA74 FA7A 5ED2 5C48 1BA6 EFFF 81F9`
- `uid [ultimate] changfeng (changfeng-chang) <1010667394@qq.com>`
- `sub 2048R/8477CCD1 2017-09-12`
其中`EFFF81F9` 就是申请的`key`
过程中需要填写一个密码，记住他，下一步打包上传的时候需要它。
![](https://cdn.www.sojson.com/file/17-09-12-13-15-03/doc/7708679924)
# 五、上传发布Jar到Maven中央仓库
![](https://cdn.www.sojson.com/file/17-09-12-13-16-15/doc/5908293798)
BUILD SUCCESS成功之后执行下列命令（gpg --list-keys 命令可以查看自己的key）
![](https://cdn.www.sojson.com/file/17-09-12-13-16-49/doc/6286367427)
```java
```
- `C:\Windows\system32>gpg --keyserver hkp://keyserver.ubuntu.com:11371 --send-keys 5C1845F3`
- `gpg: sending key 5C1845F3 to hkp server keyserver.ubuntu.com`
会在中间仓库中`Staging Repositories` 菜单中看到自己刚刚上传的`jar` 包，如果没有错的话，选中它然后点`Release` 就可以同步到中央仓库了，一般过十分钟左右就能在[http://search.maven.org/](http://search.maven.org/)上边搜到了。
