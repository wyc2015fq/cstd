# 发布项目至maven中央仓库汇总（流程+问题） - z69183787的专栏 - CSDN博客
2018年08月23日 15:15:03[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1489
[https://zhuanlan.zhihu.com/p/41650855](https://zhuanlan.zhihu.com/p/41650855)
和很多人一样，开发好自己的开源项目之后。希望上传至Maven中央仓库，最近折腾这个事情也是大费周章，主要源于网上太多教程和帖子复制来粘贴去。还有遇到的很多问题，没法得到很好的解决。于是我尝试成功之后，趁着记忆清晰，写下这篇教程，希望帮助更多的人。
首先感谢几篇教程，给了我不少帮助。
[发布项目到Maven中央仓库的最佳实践​www.jianshu.com![图标](https://pic4.zhimg.com/v2-81419c62bc7f82503038b5533560f3c3_180x120.jpg)](https://link.zhihu.com/?target=https%3A//www.jianshu.com/p/5f6135e1925f)[人类身份验证 - SegmentFault​segmentfault.com](https://link.zhihu.com/?target=https%3A//segmentfault.com/a/1190000009450347)
下面我总结下整个过程。
发布项目需要的准备事情：
1.去sonatype申请账号，然后创建一个issue。
2.安装gpg秘钥管理工具，生成秘钥并发布。
3.maven 和pom.xml 的依赖配置和setting.xml的配置
## **1.sonatype申请账号**
Sonatype是一个通过Nexus对开源项目提供托管服务的网站。先去
[https://issues.sonatype.org/secure/Dashboard.jspa​link.jianshu.com](https://link.zhihu.com/?target=https%3A//link.jianshu.com/%3Ft%3Dhttps%253A%252F%252Fissues.sonatype.org%252Fsecure%252FDashboard.jspa)
这里，可能需要翻墙，自行搭建梯子。没有账号就点击sign up注册一个。
![](https://pic4.zhimg.com/80/v2-66c92faacda2dafdecfed2a10e54f9e8_hd.jpg)
注册完了之后，登陆网站。点击create，创建一个issue。填写一下Summary， Group Id， Project URL， SCM URL的内容。
project选择**Community Support - Open Source Project Repository Hosting (OSSRH)**
Issue TypeRequired 是New Project
其他按照按常规填写，写下星号的就可以了。group ID 最好是你个人拥有的域名，审核时候需要询问你是否是域名主人。
然后就创建issue。
新建的issue状态是open的，然后你需要等待工作人员审核，因为有时差，所以可能会隔天。英文水平不好的，可以借助谷歌翻译等软件。
审核通过，你会收到一封邮件。上面是一些仓库的地址。
![](https://pic4.zhimg.com/80/v2-9d4661c9508fb98191d543d64f33e874_hd.jpg)
## **2.gpg秘钥安装**
2.1 安装gpg
gpg（GunPG）是一款用于生成秘钥的加密软件。
Windows下载地址：
[GnuPG - Download​www.gnupg.org](https://link.zhihu.com/?target=https%3A//www.gnupg.org/download/)
Ubuntu可以使用如下命令进行安装：
> sudo apt-get install gnupg
mac可以使用brew安装：
> brew install gpg
**gpg 常用的命令**
- `gpg --version` 检查安装成功没
- `gpg --gen-key` 生成密钥对
- `gpg --list-keys` 查看公钥
- `gpg --keyserver hkp://keyserver.ubuntu.com:11371 --send-keys 公钥ID` 将公钥发布到 PGP 密钥服务器
- `gpg --keyserver hkp://keyserver.ubuntu.com:11371 --recv-keys 公钥ID` 查询公钥是否发布成功
2.2 创建秘钥并发布
运行命令gpg --gen-key 生成密钥对，按照提示输入真实姓名，邮箱等。然后生成秘钥时候，会让你输入两次密码，这个密码要记住。后续会用到。
完成后运行命令gpg --list-keys ，查看本地秘钥
![](https://pic3.zhimg.com/80/v2-20a1c6ce2253b9e1fa93baa82d57856c_hd.jpg)
gpg --list-keys
然后发布秘钥至秘钥服务器。使用上面最后两个命令发布和验证。
## **3.maven和pom.xml配置**
3.1 pom配置
pom.xml 中必须包括：name、description、url、licenses、developers、scm 等基本信息
```
<distributionManagement>
        <repository>
            <id>repository</id>
            <url>https://oss.sonatype.org/service/local/staging/deploy/maven2/</url>
        </repository>
        <snapshotRepository>
            <id>ossrh</id>
            <url>https://oss.sonatype.org/content/repositories/snapshots</url>
        </snapshotRepository>
    </distributionManagement>
    <profiles>
        <profile>
            <id>disable-javadoc-doclint</id>
            <activation>
                <jdk>[1.8,)</jdk>
            </activation>
          <!-- java8版本导致javadoc打包编译失败时候，添加-->
            <properties>
                <javadoc.opts>-Xdoclint:none</javadoc.opts>
            </properties>
        </profile>
        <profile>
            <id>release</id>
            <build>
                <plugins>
                    <plugin>
                        <groupId>org.sonatype.plugins</groupId>
                        <artifactId>nexus-staging-maven-plugin</artifactId>
                        <version>1.6.3</version>
                        <extensions>true</extensions>
                        <configuration>
                            <serverId>ossrh</serverId>
                            <nexusUrl>https://oss.sonatype.org/</nexusUrl>
                            <autoReleaseAfterClose>true</autoReleaseAfterClose>
                        </configuration>
                    </plugin>
                    <plugin>
                        <groupId>org.apache.maven.plugins</groupId>
                        <artifactId>maven-release-plugin</artifactId>
                        <version>2.3.2</version>
                        <configuration>
                            <autoVersionSubmodules>true</autoVersionSubmodules>
                            <useReleaseProfile>false</useReleaseProfile>
                            <releaseProfiles>release</releaseProfiles>
                            <goals>deploy</goals>
                        </configuration>
                    </plugin>
                    <plugin>
                        <groupId>org.apache.maven.plugins</groupId>
                        <artifactId>maven-compiler-plugin</artifactId>
                        <version>3.0</version>
                        <configuration>
                            <source>1.8</source>
                            <target>1.8</target>
                        </configuration>
                    </plugin>
                    <plugin>
                        <groupId>org.apache.maven.plugins</groupId>
                        <artifactId>maven-gpg-plugin</artifactId>
                        <version>1.5</version>
                        <executions>
                            <execution>
                                <id>sign-artifacts</id>
                                <phase>verify</phase>
                                <goals>
                                    <goal>sign</goal>
                                </goals>
                            </execution>
                        </executions>
                    </plugin>
                    <plugin>
                        <groupId>org.apache.maven.plugins</groupId>
                        <artifactId>maven-source-plugin</artifactId>
                        <version>2.2.1</version>
                        <executions>
                            <execution>
                                <id>attach-sources</id>
                                <goals>
                                    <goal>jar-no-fork</goal>
                                </goals>
                            </execution>
                        </executions>
                    </plugin>
                    <plugin>
                        <groupId>org.apache.maven.plugins</groupId>
                        <artifactId>maven-javadoc-plugin</artifactId>
                        <version>2.10.4</version>
                        <executions>
                            <execution>
                                <id>attach-javadocs</id>
                                <phase>package</phase>
                                <goals>
                                    <goal>jar</goal>
                                </goals>
                              <!-- java8版本导致javadoc打包编译失败时候，添加-->
                                <configuration>
                                    <additionalparam>${javadoc.opts}</additionalparam>
                                </configuration>
                            </execution>
                        </executions>
                    </plugin>
                </plugins>
            </build>
        </profile>
    </profiles>
    <licenses>
        <license>
            <name>The Apache Software License, Version 2.0</name>
            <url>http://www.apache.org/licenses/LICENSE-2.0.txt</url>
            <distribution>repo</distribution>
        </license>
    </licenses>
    <scm>
        <url>https://gitee.com/ironzheng/pikachu.git</url>
        <connection>https://gitee.com/ironzheng/pikachu.git</connection>
        <developerConnection>http://pikachu.luway.cn</developerConnection>
    </scm>
    <developers>
        <developer>
            <name>yourname</name>
            <email>youremail</email>
            <url>yoururl</url>
        </developer>
    </developers>
```
这里给出pom.xml示例,其中开发者信息修改成自己的。
**3.2 maven的setting.xml配置**
需要添加sonatype的身份认证，就是你一开始申请账号时候的用户名和密码。id要和pom.xml中**distributionManagement** 中的**snapshotRepository和repository保持一致。**
```
<server>  
      <id>id</id>  
      <username>yourname</username>
      <password>yourpass!</password>
    </server>
```
**3.3 发布项目**
**运行下面命令，进行发布。**
```
mvn clean install deploy -P release -Dgpg.passphrase=生成秘钥时候你的密码
```
一般如果pom.xml中没有添加自动release的插件的话，你需要登陆
[Nexus Repository Manager​oss.sonatype.org](https://link.zhihu.com/?target=https%3A//oss.sonatype.org/)
手动操作发布。
```
这是自动发布的插件                 
  <plugin>
                        <groupId>org.sonatype.plugins</groupId>
                        <artifactId>nexus-staging-maven-plugin</artifactId>
                        <version>1.6.3</version>
                        <extensions>true</extensions>
                        <configuration>
                            <serverId>ossrh</serverId>
                            <nexusUrl>https://oss.sonatype.org/</nexusUrl>
                            <autoReleaseAfterClose>true</autoReleaseAfterClose>
                        </configuration>
                    </plugin>
```
登陆网站之后，输入你的账号密码，点击左侧的Staging Repositories ->在搜索栏输入你的 groupId -> 勾选你的构件并点击 close -> 点击 tab 栏的 release。
然后就成功发布你的项目了，过个半天一天应该就能找到搜到你的项目了。
## **问题总结**
开发时候总会遇到各种各样的问题，这里持续做些总结。
1.自动发布时候遇到这个问题。
![](https://pic2.zhimg.com/80/v2-a3c165707de4bf9f4aa80bfa0d836eda_hd.jpg)
这个一般是秘钥不对导致的，处理办法是重新生成秘钥和上传验证。再试一次应该可以解决。
2.发布秘钥时候，no route to host
这个可能是由于网络问题导致无法访问秘钥服务器，办法是翻墙或者换个网络，和换个秘钥服务器地址。下面是候选地址
- [pool.sks-keyservers.net](https://link.zhihu.com/?target=https%3A//link.jianshu.com/%3Ft%3Dhttp%3A//pool.sks-keyservers.net)
- [ha.pool.sks-keyservers.net](https://link.zhihu.com/?target=https%3A//link.jianshu.com/%3Ft%3Dhttp%3A//ha.pool.sks-keyservers.net)
- [p80.pool.sks-keyservers.net:80](https://link.zhihu.com/?target=https%3A//link.jianshu.com/%3Ft%3Dhttp%3A//p80.pool.sks-keyservers.net%3A80)
3.javadoc问题
由于java8版本可能会导致注解的一些不符合规范，可以添加我示例pom.xml中的注释来解决这个问题。
```
<profile>
            <id>disable-javadoc-doclint</id>
            <activation>
                <jdk>[1.8,)</jdk>
            </activation>
          <!-- java8版本导致javadoc打包编译失败时候，添加-->
            <properties>
                <javadoc.opts>-Xdoclint:none</javadoc.opts>
            </properties>
        </profile>
```
4.如果遇到Java home路径问题，可以手动制定java路径来解决。
```
<properties>
        <log4j.version>2.5</log4j.version>
        <javadocExecutable>${java.home}/../bin/javadoc</javadocExecutable>
    </properties>
```
