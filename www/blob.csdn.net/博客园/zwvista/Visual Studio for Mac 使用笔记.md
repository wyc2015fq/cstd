# Visual Studio for Mac 使用笔记 - zwvista - 博客园

## [Visual Studio for Mac 使用笔记](https://www.cnblogs.com/zwvista/p/9367689.html)

### 无法打开控制台运行程序

`Project -> Project Options -> Run -> Configurations -> Default`

勾选`Run on external console`

其结果是在`*.csproj.user`里面添加
`<ExternalConsole>true</ExternalConsole>`

[Can't debug a solution in VS 2017 for Mac (which was created on windows machine in VS 2017)](https://developercommunity.visualstudio.com/content/problem/63704/cant-debug-a-solution-in-vs-2017-for-mac-which-was.html)

### 打开多个实例或解决方案

无法打开多个实例，但是可以在实例中打开多个解决方案。

[如何打开 Visual Studio for Mac 的多个解决方案或实例？](https://visualstudio.microsoft.com/zh-hans/vs/support/mac/open-multiple-solutions-instances-visual-studio-mac/?rr=https%3A%2F%2Fwww.google.co.jp%2F)


