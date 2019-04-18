# Nano 的代替者 Micro 编辑器 - 知乎
# 



不少在 Linux 下有编辑需求，又没有时间学习 Vim/Emacs 的人都会选择 Nano 进行文本编辑。由于 Nano 失于过分简陋，以及采用 Emacs 键位并不见得所有人都习惯，很多时候也只能用 Nano 完成一些很初级的工作。

今天我们有了更好的选择，Micro 编辑器不但在使用上比 Nano 更简单，更贴近现代桌面编辑器的使用习惯，同时在功能上也比 Nano 强大很多。如果以 Windows 下的编辑器来对比的话，如果 Nano 比作 NotePad2，那 Micro 至少是 NotePad++，或者 EditPlus：
![](https://pic3.zhimg.com/v2-8bf4faaa0c6628acf445363a52140a7a_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='970' height='630'></svg>)
该编辑器的设计目标是做一款：“基于直觉的终端编辑器”，什么叫做“基于直觉”？看看下面的默认键位便知：
![](https://pic2.zhimg.com/v2-78324c61bbb8c17a4472dc807c6a82e9_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='456' height='599'></svg>)
扫一眼这张表就会发现，基本和你使用 Windows 下各种编辑器的键位是一模一样的，就连复制粘贴的体验也和 NotePad++/EditPlus 一模一样，上手即用，根本没有任何学习成本。

多文件编辑的话，Micro 的 tab 也和 NotePad++ 类似：
![](https://pic1.zhimg.com/v2-0cb57a520d430d137f6a0fdc530a57c4_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='970' height='630'></svg>)
多个 TAB 同时编辑多个文件，可以用 CTRL+T 然后 CTRL+o 再新的 tab 里编辑文件，然后 ALT+,/. 来左右切换 TAB，同时也支持鼠标，鼠标直接点击顶部的 tab 名称即可切换文件，Nano 根本做不到这么方便。

支持多窗口：
![](https://pic3.zhimg.com/v2-2296230730196235f03db7bfb4c4d102_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='970' height='630'></svg>)
使用 CTRL+W 或者直接鼠标点击，在不同的窗口间切换，Nano 根本就不支持窗口。此外独有的插件系统，可以使用 lua 为 micro 开发各种各样的插件，比如按 CTRL+E 进入命令模式，输入新命令为 micro 增加一个文件浏览器：

`plugin install filemanager`

然后重启 micro 后，用 CTRL+E 输入命令 tree，即可展开文件管理器：
![](https://pic2.zhimg.com/v2-7be9c1bcca5eb4a2b68d8d15dd398e65_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='970' height='630'></svg>)
文件管理器也可以直接用鼠标点击操作，不必完全使用键盘。除此之外还有各种格式化，高亮，编辑辅助，模糊搜索，CVS集成等插件可以尝试使用。

同样你可以直接再编辑器内 CTRL+E 输入 log 命令，打开下面的 log 窗口，并且用 run 运行一些 shell 命令，比如：
![](https://pic2.zhimg.com/v2-86d646154ec3f2e1c43ff83644e17891_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='970' height='630'></svg>)
之前运行的 "ls -la" 命令会把输出显示再 log 窗口里，便于你复制粘贴，看完 LOG 窗口，按 CTRL+Q 就可以关闭。

上面随意列举了几条，已经比 Nano 强大很多了，同时按键更顺手，比如我数次搞忘了 Nano 里面奇怪的复制粘贴应该怎么弄，但是我绝对不会忘记 Micro 里怎么做复制粘贴。还有更多功能大家可以慢慢探索，用过的人对 Micro 的评价：

>     "Finally a simple editor that just works, with amazing mouse support."
    "I really love this. This is definitely replacing nano for me."
    "Keep up the great work, you have saved me from Nano and Vim suffering!"
    "The micro text editor is dope."
    "Nice, readable source, with generous use of comments. I wonder if this is that 'idiomatic go' I keep hearing about."

总之 Micro 是一款比 Nano 更加友好和强大的编辑器，能够承载更多复杂的编辑需求。最后记得使用前把终端设置成 256 色：


```bash
export TERM=xterm-256color
```


这样色彩高亮会比默认的漂亮很多。


