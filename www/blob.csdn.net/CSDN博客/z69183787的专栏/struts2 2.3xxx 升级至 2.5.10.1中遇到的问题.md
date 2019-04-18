# struts2 2.3xxx 升级至 2.5.10.1中遇到的问题 - z69183787的专栏 - CSDN博客
2017年07月11日 17:21:58[OkidoGreen](https://me.csdn.net/z69183787)阅读数：2545
1、web.xml struts2的 filter和listener 包路径变动
2、2.5.10.1自带 xwork-core，xwork依赖可移除
3、struts2.5.10.1 开始 log4j变更为使用 log4j2版本，需要显示引入 log4j2 jar 和 log4j1 兼容版本
```
<!-- log -->
        <dependency>
            <groupId>org.apache.logging.log4j</groupId>
            <artifactId>log4j-core</artifactId>
            <version>2.7</version>
        </dependency>
        <dependency>
            <groupId>org.apache.logging.log4j</groupId>
            <artifactId>log4j-api</artifactId>
            <version>2.7</version>
        </dependency>
        <dependency>
            <groupId>org.apache.logging.log4j</groupId>
            <artifactId>log4j-1.2-api</artifactId>
            <version>2.7</version>
        </dependency>
```
4、struts 2.5.10.1 中 自带 asm5.1 ，cglib 需升级成 3.2.4 不然会冲突
其他可参考：[https://my.oschina.net/u/2552669/blog/868139](https://my.oschina.net/u/2552669/blog/868139)
