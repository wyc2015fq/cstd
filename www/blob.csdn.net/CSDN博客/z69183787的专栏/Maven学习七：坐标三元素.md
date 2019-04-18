# Maven学习七：坐标三元素 - z69183787的专栏 - CSDN博客
2014年03月27日 16:00:05[OkidoGreen](https://me.csdn.net/z69183787)阅读数：867

       在Maven世界中，任何一个依赖、插件或者项目构建的输出，都可以称为构件，构件的逻辑表示方式是：坐标和依赖，构件的物理表示方式是：文件。Maven坐标就是提供了一组规则定义：世界上任何一个构件都可以使用Maven坐标唯一标识，Maven坐标的元素包括groupId、artifactId、version、packaging、classifier，其中groupId、artifactId、version可以算作必要三元素了。
## groupId**：**
    Maven项目隶属的实际项目，命名方式通常与域名反向一一对应。必须定义。
## artifactId**：**
    实际项目中的一个Maven项目（模块），推荐使用实际项目名称作为artifactId的前缀。必须定义。
## version**：**
    Maven项目当前所处的版本。必须定义。
## packaging**：**
    Maven项目的打包方式，打包方式通常与所生成构件的文件扩展名对应。当没有定义时，Maven会使用默认值jar。
##     classifier**：**
    帮助定义构件输出的一些附属附件。注意，不能直接定义项目的classifier，因为附属构件不是项目直接默认生成的，而是由附加的插件帮助生成的。
