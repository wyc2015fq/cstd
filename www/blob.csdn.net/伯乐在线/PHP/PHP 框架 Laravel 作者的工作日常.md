# PHP 框架 Laravel 作者的工作日常 - 文章 - 伯乐在线
本文由 [伯乐在线](http://blog.jobbole.com) - [佟大冬](http://www.jobbole.com/members/TJQ) 翻译，[黄利民](http://www.jobbole.com/members/huanglimin) 校稿。未经许可，禁止转载！
英文出处：[Taylor Otwell](https://medium.com/@taylorotwell/how-i-work-a22010d1ad82#.fbjc5ckar)。欢迎加入[翻译组](https://github.com/jobbole/translation-project)。
【导读】：PHP Web 框架 Laravel 作者 Taylor Otwell 介绍自己的工作日常，办公配置，开发工具等等。
![](http://wx2.sinaimg.cn/mw690/7cc829d3gy1fgiinzj7mej20go08cjrv.jpg)
经常有人问我有关日常工作的问题，例如我是如何管理时间的，我常用的工具有哪些，我喜欢用什么编辑器和配色方案。在这里，我将借这篇文章尝试回答这些问题。
### 普通的一天
我在家办公，全职从事 Laravel 的工作。通常早八点之前就到办公室了，第一件事就是回复昨晚收到的所有来自 Forge、Envoyer 和 Spark 的客户邮件，大约 15 封。回复邮件时，在电脑上用 Apple Mail，在手机上用Outlook。我喜欢清空收件箱之后再继续忙其他事情。接下来一整天，我还会定期查看邮件，确保收件箱干净明了。
回复完邮件，我就会查看 Laravel 的 GitHub 仓库（repository）中优秀的 pull 请求，我尽量让此类请求不超过 10-12 个。 通常我会在九点之前将所有的邮件和 pull request 处理完，当然其间 Mohamed 会帮我审查并分类 GitHub 上的问题及 pull request。
在合并或关闭了昨晚出现的 pull 请求之后，我便可以去做任何我想做的项目了，当然一般是和 Laravel 相关的。最近我主要在忙 Horizon，一个新开发的 Laravel 包，我希望在 Laracon US 会上多谈谈。
下午五点全天工作结束，之后我会和家人一起吃晚饭然后闲逛。晚上九点半左右，我会再次查看邮箱，处理下午五点之后收到的邮件，这通常只需要 10-15 分钟。这样可以减轻我明天早上处理邮件的工作量。
![](http://wx2.sinaimg.cn/mw690/7cc829d3gy1fgiio11zmoj20zk0k00tp.jpg)
*（这是之前 Laravel Sydney 活动，Taylor Otwell 远程互动）*
### 开发配置
我用来开发的机器主要是一台5k 屏 iMac，处理器 4 Ghz、RAM 16 GB。iMac 两边各放一台 24 寸 4k 显示器。我把两边的显示器背景调为深灰色，这样当我没在用时，它们看起来像是关闭的，不会很刺眼。
![](http://wx1.sinaimg.cn/large/7cc829d3gy1fgiinxspohj20m80goq4s.jpg)
我有一把 Steelcase Leap 座椅，一个 Swell 水瓶和一个 Fidget Cube（减压魔方）。天花板上的飞利浦 Hue 照亮整个办公室。
## 开发工具
**编辑器**
我常用的文本编辑器是 Sublime Text 3，主题为 Soda，配色方案为 Inspired GitHub。写代码主要用 Operator Mono 字体，用 SublimeLinter 检查 PHP 语法。
整体配置像这样：


```
{
    "added_words":
    [
        "Laravel"
    ],
    "caret_style": "phase",
    "color_scheme": "Packages/User/SublimeLinter/One Dark (SL).tmTheme",
    "ensure_newline_at_eof_on_save": true,
    "findreplace_small": true,
    "font_face": "OperatorMono-Book",
    "font_size": 15,
    "highlight_line": false,
    "hot_exit": false,
    "ignored_packages":
    [
        "SublimeLinter-jshint",
        "Vintage"
    ],
    "line_numbers": true,
    "line_padding_bottom": 9,
    "line_padding_top": 9,
    "phpunit-sublime-terminal": "iTerm",
    "remember_open_files": false,
    "show_definitions": false,
    "show_panel_on_build": false,
    "tab_size": 4,
    "tabs_small": false,
    "theme": "Soda Dark.sublime-theme",
    "translate_tabs_to_spaces": true,
    "trim_trailing_white_space_on_save": true,
    "word_wrap": false
}
```
在开发时，我通常会将 Subline 字体大小调到 10 左右。
**终端**
终端我用 iTerm2 和 Oh My Zsh，Oh My Zsh 采用内置主题 robbyrussell，iTerm2 的配色方案为 Arthur。我喜欢将 iTerm2 的行间距调宽一点，这样代码看起来不会太拥挤。像 Sbulime 一样，终端字体我用 Operator Mono。
![](http://wx2.sinaimg.cn/mw690/7cc829d3gy1fgiinz6xuoj20m809ht9s.jpg)
**数据库**
我用 Sequel Pro 与 MySQL 数据库进行交互，用 Medis 与 Redis 进行交互。我乐意为 Sequel Pro 支付数百美元，然而事实上它是免费的，真是令人惊讶。
**SaaS 服务**
我用各种服务运行 Forge 和 Envoyer。
- DigitalOcean 作为服务器
- 需要多个服务器就用 Linode
- Cloudflare 作为 DNS 和 SSL
- Ottomatik 用于数据库备份
- AWS 用于存储和消息队列服务
- Pusher 用于处理实时消息
- Authy 用于双重认证
**效率**
我主要用 Bear、Wunderlist（奇妙清单）和 Trello 来记录，让一切井然有序。Bear 替代了 Apple Notes，用来记长篇笔记。Wunderlist 主要用来记录个人清单和业余项目清单。Trello 用于追踪记录与 Laravel 相关的事，比如即将推出的新特性或做出的重要修改。
我还构建了一个小型 Laravel 应用程序，用来在 Telegram 上给我发送提醒，比如倒垃圾、更换家里的空气过滤器、每月缴税等等。这个小程序通过调用 Telegram API，用 Laravel 的命令调度功能和 Guzzle 库实现消息的发送。
**音乐**
我用 Spotify 听音乐，几个我最喜欢的播放列表是 Night Rider、Lush + Atmospheric 和 RetroWave / Outrun。
