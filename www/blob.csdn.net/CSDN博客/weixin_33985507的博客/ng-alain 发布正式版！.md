# ng-alain 发布正式版！ - weixin_33985507的博客 - CSDN博客
2017年11月19日 17:19:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：9
[ng-alain](https://link.jianshu.com?t=https://cipchk.github.io/ng-alain)是一个基于[ng-zorro-antd](https://link.jianshu.com?t=https//ng.ant.design/)的企业后台脚手架，目标也非常简单，希望在Angular上面开发企业后台更简单、更快速。github地址是：[cipchk/ng-alain](https://link.jianshu.com?t=https://github.com/cipchk/ng-alain)。
一直以来对Ant Desig的理念非常喜欢，而一直出于工作的需要能够运用的机会本不多，直到 ng-zorro-antd 的出现，终于能够在内部真正实践。而 ng-alain 虽然时间很短，但在群里很多朋友的支持下，让其尽可能有一个比较可靠的实施。据我所知包括我们公司自己在内，已经有好几个企业开始转移至该脚手架上面来。
你可以非常简单的从 github 克隆一份最新代码，然后立即进入业务开发。
```
git clone --depth 1 https://github.com/cipchk/ng-alain.git
```
## 结构
ng-alain 实际是一个标准的 Angular Cli 构建的项目，因此对她就像是你前女友一样，一定不会感到陌生。
在此基础上，我将公共业务组件放在 `shared/components` 下面，且目前已经包括图表相关有几十个组件服务于企业后台常见业务服务型组件。
我们在处理时间、货币上面频繁极高，而提供一种非常简洁的这类数据渲染极重要。例如：`{{ time | date }}` 期望结果应该：`2017-11-19 04:25`；而 `{{ price | currenty }}` 期望的结果应该是：`￥303,634.13` 的时候，只需要这样：`{{ time | _date }}`。没错在 Angular 内置的管道里，我增加了以下划线开头的新管道来处理这类渲染。
以上我尽量保持一种简洁而又有效的方式，我想这也是 Ant Design 理念之一。
## 样式风格
ng-alain 有自己的一套样式风格定义，但这些定义都是基于 Ant Desig 设计理念；例如，两个横向按钮之间以 Ant Desig 的一种标准应该有 8px 的间距，一般我们会这样：
```
<button nz-button style="margin-right: 8px">A</button>
<button nz-button>B</button>
```
而对于 ng-alain 来说，应该是这样：
```
<button nz-button class="mr-sm">A</button>
<button nz-button>B</button>
```
假如你是在一个表单里出现时，你连 class 都可以省略。
而 ng-alain 默认设置了 **preserveWhitespaces: false** 因此你会发现俩俩之间真的只有 8px。
## Ant Design PRO
它是 React 发布不久的一个脚手架真的非常棒，没几天其 Star 就直接盖了 ng-zorro-antd 有点心疼 ng-zorro-antd 哈。
当然 React 能做的，Angular 自然也能做，因此 ng-alain 也实现了一套 And Design Pro，原本我打算加一个完全一样的 LayoutComponet。然，我发现 pro 的风格与ng-alain真的不谋而合，将 pro 内容区域在 ng-alain 中形成一种很好的契合，文章开头的图片就是来自 Pro 在 ng-alain 实现的分析页。
Pro 页面并不多，但涵盖面很广，ng-alain 除了用户页以外其他页面都已经实现完毕。
## ng-zorro-antd-extra 番外篇
维护 ng-zorro-antd 的阿里团队的确有点小慢，特别是在 Ant Design 马上要 3.0，而当前 ng-zorro-antd 还有很多值得优化以及一些部分核心组件还未上线，而别说 3.0 的计划。因此[ng-zorro-antd-extra](https://link.jianshu.com?t=https://github.com/cipchk/ng-zorro-antd-extra)是我自己维护一个基于 Ant Design 3.0 版本下实现部分非常急需的组件。
在组件接口上我保持 ng-zorro-antd 的风格，我会在未来提交 PR 至 ng-zorro-and。
## 千言万语
我要表达的话还非常多，只是为了一个记录正式版文章中并不适合讨论太多。或许你可以点开[DEMO](https://link.jianshu.com?t=https://cipchk.github.io/ng-alain)体验一下。
其中一些我认为值得注意的话已经整合在项目的 _document 下面。
## 未来
ng-alain 会作为我一个非常重要的开源项目持续更新，当然这一切的来源都是基于 ng-zorro-antd 的基础，但若你一样喜欢 Angular 以及 Ant Design 的理念，那为何不好好看一下 ng-alain，多多少少你能带走点什么。
未来我重点工作会在：
- 提供更丰富业务组件库
- 跟进 angular、ng-zorro-antd、Ant Design Pro
- 工具上的支持，包括：vscode snippets、Angular Cli 中间件等
- 重构及优化代码
