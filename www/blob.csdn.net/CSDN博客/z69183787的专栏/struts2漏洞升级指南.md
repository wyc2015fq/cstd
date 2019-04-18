# struts2漏洞升级指南 - z69183787的专栏 - CSDN博客
2014年12月04日 15:17:25[OkidoGreen](https://me.csdn.net/z69183787)阅读数：874
因Struts2框架缺陷造成的远程执行漏洞, Struts2中WebWork框架使用XWork来支持Struts 2及其他应用。XWork处理用户请求参数数据时存在漏洞，远程攻击者可以利用此漏洞在系统上执行任意命令。
# 升级方法
1.maven管理jar包的可以直接在pom中把Convention插件实现struts2的零配置和spring中struts2插件升级为最新的版本，
<dependency>
            <groupId>org.apache.struts</groupId>
            <artifactId>struts2-convention-plugin</artifactId>
            <version>2.3.4</version>
        </dependency>
 <dependency>
            <groupId>org.apache.struts</groupId>
            <artifactId>struts2-spring-plugin</artifactId>
            <version>2.3.4</version>
        </dependency>
如果用到单元测试的话，要添加
<dependency>
            <groupId>org.springframework</groupId>
            <artifactId>spring-test</artifactId>
            <version>2.5.6</version>
        </dependency>
2.如果没有用maven，可以把ognl-2.7.3.jar、struts2-core-2.1.8.jar、struts2-spring-plugin-2.1.8.jar、xwork-core-2.1.6.jar、commons-lang-2.4.jar、javassist-3.4.GA.jar六个文件；（注：具体的对应自己的版本，删除文件之前请先将六个文件备份一下，已方便快速恢复。）替换成ognl-3.0.5.jar、struts2-core-2.3.4.jar、struts2-spring-plugin-2.3.4.jar、xwork-core-2.3.4.jar、javassist-3.11.0.GA.jar、commons-lang3-3.1.jar
3.需要注意的是升级后项目中有些地方可能会报错，其中有
导入的包路径org.apache.commons.lang.xwork.StringEscapeUtils改为org.apache.commons.lang.StringEscapeUtils
其他的根据实际情况改改，大功告成，
