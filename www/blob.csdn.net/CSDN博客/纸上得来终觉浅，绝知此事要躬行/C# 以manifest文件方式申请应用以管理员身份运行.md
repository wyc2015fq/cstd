# C# 以manifest文件方式申请应用以管理员身份运行 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2018年06月27日 11:29:53[boonya](https://me.csdn.net/boonya)阅读数：292








这里是在VS2017下面操作为例，程序需要管理员权限运行，manifest是改动最小的方式。这里以RTVS项目为例进行讲解。

##### 添加manifest配置文件

项目右键添加新建项，如下图所示：

![](https://img-blog.csdn.net/20180627112428934?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Jvb255YQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)


##### 修改manifestUAC权限控制

项目结构如下：

![](https://img-blog.csdn.net/20180627112552111?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Jvb255YQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

需要修改的部分：

```
<trustInfo xmlns="urn:schemas-microsoft-com:asm.v2">
    <security>
      <requestedPrivileges xmlns="urn:schemas-microsoft-com:asm.v3">
        <!-- UAC 清单选项
             如果想要更改 Windows 用户帐户控制级别，请使用
             以下节点之一替换 requestedExecutionLevel 节点。n
        <requestedExecutionLevel  level="asInvoker" uiAccess="false" />
        <requestedExecutionLevel  level="requireAdministrator" uiAccess="false" />
        <requestedExecutionLevel  level="highestAvailable" uiAccess="false" />

            指定 requestedExecutionLevel 元素将禁用文件和注册表虚拟化。
            如果你的应用程序需要此虚拟化来实现向后兼容性，则删除此
            元素。
        -->
        <!--
        <requestedExecutionLevel level="asInvoker" uiAccess="false" />
        -->
        <requestedExecutionLevel  level="requireAdministrator" uiAccess="false" />
      </requestedPrivileges>
    </security>
  </trustInfo>
```

关键配置：
` <requestedExecutionLevel  level="requireAdministrator" uiAccess="false" />`这样我们的程序就可以不用右键管理员验证了，程序启动时即弹出授权提示认证，如下图：

![](https://img-blog.csdn.net/20180627112912495?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Jvb255YQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)






