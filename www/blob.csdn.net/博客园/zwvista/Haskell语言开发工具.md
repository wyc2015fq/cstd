# Haskell语言开发工具 - zwvista - 博客园

## [Haskell语言开发工具](https://www.cnblogs.com/zwvista/p/9310880.html)

### Stack

[How to Script with Stack](https://haskell-lang.org/tutorial/stack-script)
[Originate Guides - Haskell Tool Stack](https://github.com/Originate/guide/blob/master/haskell/stack-tutorial.md)

### 配置 Intellij Idea

[IntelliJ plugin for Haskell](https://github.com/rikvdkleij/intellij-haskell)
[The Haskell Tool Stack](https://docs.haskellstack.org/en/stable/README/)
- 安装 IntelliJ plugin for Haskell

Preferences -> Plugins -> Browse Repositories -> IntelliJ-Haskell -> Install
- 创建工程

stack new my-project

cd my-project

stack setup

stack build

stack exec my-project-exe
- 导入工程

Import Project

在向导的第一页选 Import project from external model -> Haskell Stack

在向导的第二页设置 Project SDK: Configure... -> /usr/local/bin/stack -> Haskell Tool Stack

Finish
- 编译运行

Edit Configuration -> + Haskell Stack / Haskell Stack Runner
- 添加包

在 package.yaml 中的 dependencies: 下添加所需依赖的包。

### cabal, stack 与代理服务器

在使用代理服务器的情况下执行 cabal 或者 stack 需要设置环境变量 HTTP_PROXY 和 HTTPS_PROXY。

在连接 http 网址时使用 HTTP_PROXY 环境变量中所设置的代理服务器。

在连接 https 网址时使用 HTTPS_PROXY 环境变量中所设置的代理服务器。


