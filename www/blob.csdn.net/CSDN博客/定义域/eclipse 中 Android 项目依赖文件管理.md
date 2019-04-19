# eclipse 中 Android 项目依赖文件管理 - 定义域 - CSDN博客
2015年09月09日 16:45:26[定义域](https://me.csdn.net/u010278882)阅读数：1649
## 管理方式
![ADT eclipse 依赖](https://img-blog.csdn.net/20150909154800144)
eclipse中的Android项目中，可见到如上图所示的四类依赖管理形式：
- 
**Android x.x.x** 中的是android.jar，x.x.x 是编译项目的SDK版本。
- 
**Android  Dependencies** 和  **Android Private Libraries** 是Android提供的依赖管理方式（与 3. Referenced Libraries 相对）。
> 
**说明**
Android项目的依赖文件可以通过两种形式提供：
1.[项目依赖](https://img-blog.csdn.net/20150909161650582)
2.[jar包依赖](https://img-blog.csdn.net/20150909161827136)
其中，前者包含的是*项目依赖*生成的jar，比如某个项目引用Volley项目，那么volley.jar就会出现在这里；后者包含的是本项目中的jar包依赖，即*libs*目录中的jar，比如Android项目构建引导自动添加的依赖，比如Android支持库 android-support-v4.jar、Android注解支持库 annotation.jar，或者自己添加的第三方jar，比如百度地图的开发SDK包。
- 
**Referenced Libraries** 是eclipse提供的依赖管理方式。通过 *Java Build*  添加到项目中的jar包就是这种方式。
### **比较和选用**
> 
方式2和方式3的jar包依赖都来源于项目中的 **libs** 目录，那么为什么会受不同的管理方式管理呢？
如果jar包直接加入到 **libs** 目录，则受方式2管理；如果加入后并使用 **Java Build** 将其加入到 Build Path，则受方式3管理。
所以要统一使用方式3，即Android依赖管理插件管理依赖文件，直接添加jar包到项目的 **libs** 目录即可，不用再进行其他操作。
## **问题**
> 
`1`. 上面的比较中给出了项目中新增的依赖文件如何纳入Android依赖管理方式中。那么，对于已经混合使用了两种管理方式的项目，如何统一成方式3的管理呢？
```
可按如下操作进行：
a. 确保项目中的jar包都存储在 libs 目录中（*详见问题 2*）
b. 项目右键 - 属性 - Java Build Path - Libraries 选项卡
c. 删除其中除以 Android 为前缀的条目之外的所有条目
d. 菜单栏 - Project - Clear
```
> 
`2`. 为什么我按照上面的描述操作，可是相关的jar包还是在Referenced Libraries 中呢？
```
可能的原因是：项目的jar包所在目录名不是 **libs** 而是 **lib**。
这实际上ADT工具的不足。主要将 lib 目录改名为 libs 目录即可。
```
> 
千万不要删除 Android xxx 的依赖
