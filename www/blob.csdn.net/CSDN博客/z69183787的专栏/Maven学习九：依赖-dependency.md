# Maven学习九：依赖-dependency - z69183787的专栏 - CSDN博客
2014年03月27日 16:09:10[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1109
# Maven依赖
## 1. 依赖的配置
    根元素project下的dependencies可以包含一个或多个dependency元素，以声明一个或多个依赖。每个依赖可以包含的元素有：
groupId、artifactId**和**version：依赖的基本坐标，坐标三元素。
type：依赖的类型，对应于项目定义的packaging，大部分情况下不需要定义，使用默认值jar。
scope：依赖的范围。
optional：标记依赖是否可选。
exclusions：用来排除传递性依赖。
## 2. 依赖的范围
|依赖范围（scope）|对于编译classpath有效|对于测试classpath有效|对于运行时classpath有效|
|----|----|----|----|
|编译依赖范围：compile|Y|Y|Y|
|测试依赖范围：test|-|Y|-|
|已提供依赖范围：provided|Y|Y|-|
|运行时依赖范围：runtime|-|Y|Y|
|系统依赖范围：system|Y|Y|-|
另外还有导入依赖范围：import，该范围不会对三种classpath产生实际的影响。
## 3. 传递性依赖，和数学里的传递性，是同样的概念。
    当A有一个compile范围的B依赖，B有一个compile范围的C依赖，那么C就会成为A的compile范围依赖，C是A的一个传递性依赖。
    有了传递性依赖的机制，在使用某个依赖时就不需要考虑它依赖了什么，也不需要担心引入多余的依赖。Maven会解析各个直接依赖的POM，将那些必要的间接依赖，以传递性依赖的形式引入到当前的项目中。
    需要注意的是，可选依赖不会被传递。
## 4. 依赖范围影响传递性依赖
||compile|test|provided|runtime|
|----|----|----|----|----|
|compile|compile|-|-|runtime|
|test|test|-|-|test|
|provided|provided|-|provided|provided|
|runtime|runtime|-|-|runtime|
## 5. 依赖调解
    当两个依赖路径上有两个版本的依赖X时，有以下两个依赖调解原则：
第一原则：路径最近者优先；
第二原则：路径长度一样时，第一声明者优先。
## 6. 排除依赖
    当项目A依赖于项目B，但是不想引入传递性依赖C，而是自己显示的声明对项目C另一个版本的依赖，使用exclusions元素声明排除性依赖。
    exclusions可以包含一个或者多个exclusion子元素，声明exclusion时只需要groupId和artifactId，不需要version元素。
## 7. 归类依赖
    当项目中依赖了同一项目的不同模块，它们的版本都是相同的，因此在升级的时候，这些依赖的版本会一起升级。为了避免重复，且需要修改时只修改一处，可以通过归类依赖来解决。
    使用properties元素定义Maven属性，如springframework.version子元素，并定义其值。有了这个属性定义，maven运行时会将POM中所有的${springframwork.version}替换成定义的实际值。
## 8. 优化依赖
使用dependency:list和dependency:tree 帮助我们详细了解项目中所有依赖的具体信息。
使用dependency:analyze工具可以帮助分析当前项目的依赖。
analyze的结果中包含了两部分：
Used undeclared dependencies：项目中使用但未显式声明的依赖。这种依赖意味着潜在的风险；
Unused declared dependencise：项目中未使用的，但显式声明的依赖。对于这种依赖不能直接删除，因为analyze只会分析编译和测试需要的依赖，其他依赖它无法发现，因此需要仔细分析。
