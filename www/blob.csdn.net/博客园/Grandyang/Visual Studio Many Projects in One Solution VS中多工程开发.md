# Visual Studio Many Projects in One Solution VS中多工程开发 - Grandyang - 博客园







# [Visual Studio Many Projects in One Solution VS中多工程开发](https://www.cnblogs.com/grandyang/p/7765653.html)







在用VS开发项目的时候，我们可以将不同的模块拆分开来，分别建立单独的Project来开发调试，整个放到一个Solution中就行了。这样做的好处是结构清晰，每个功能模块可以单独调试，而不用编译整个Solution，尤其当项目很大的时候，能节省很多编译时间。而且各个项目之间相互独立，对于每个模块，我们可以选择生成静态库Static Library或是应用程序Application，便于维护。那么下面来看如何将一个大项目拆分为若干个子项目：



1. Add new project in same solution, and drag all related "*.cpp" and "*.h" files into new project

2. Go to **Property** -> **C/C++** -> **General** -> **Additional Include Directories**, add the directory of new created projects in the main project. Also, if sub-projects reference other projects, add the directories in the sub-projects too.

3. If some functions are called in another project, the current project probably needs to be a static library. So go to **Property** -> **General** -> **Configuration Typ**e -> **Static library (.lib)**. Add the exported "*.lib" file in another project, go to **Property** -> **Linker** -> **Input** -> **Additional Dependencies**.














