# 怎么安装jdk6，怎么安装jdk-6u33-linux-i586.bin - V__KING__的专栏 - CSDN博客





2013年10月24日 15:30:53[v__king__](https://me.csdn.net/V__KING__)阅读数：3498








**1、为jdk-6u33-linux-i586.bin增加执行权限**


    chmod u+x /usr/lib/jvm/java/jdk-6u45-linux-i586.bin

**2、执行jdk-6u33-linux-i586.bin，将JDK6的相关文件解包至jdk1.6.0_33目录下**


    ./jdk-6u45-linux-i586.bin

**3、将jdk1.6.0_33复制到/usr/lib下**


    mkdir -p /usr/lib/jvm/


    cp -r jdk1.6.0_45 /usr/lib/jvm/jdk1.6.0_45

**4、安装JDK6**


    JDK6的bin文件中有许多可执行命令，根据需要，可以选择安装至/usr/bin目录下，比如，我安装了java/javac/javaws/jar四个命令。具体执行如下命令：


    update-alternatives --install /usr/bin/javac javac /usr/lib/jvm/jdk1.6.0_45/bin/javac 1


    update-alternatives --install /usr/bin/java java /usr/lib/jvm/jdk1.6.0_45/bin/java 1


    update-alternatives --install /usr/bin/javaws javaws /usr/lib/jvm/jdk1.6.0_45/bin/javaws 1


    update-alternatives --install /usr/bin/jar jar /usr/lib/jvm/jdk1.6.0_45/bin/jar 1


    update-alternatives --config javac


    update-alternatives --config java


    update-alternatives --config javaws


    update-alternatives --config jar


    注意：如果需要安装其它命令，按照上面的格式，根据需要进行添加即可。

**5、测试**


    ​执行java -version可以看到当前JDK的版本信息，表示安装成功。显示如下：


    vk@vk:~/# java -version


    java version "1.6.0_45"


    Java(TM) SE Runtime Environment (build 1.6.0_45-b04)


    Java HotSpot(TM) Server VM (build 20.8-b03, mixed mode)



