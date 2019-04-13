
# 5 分钟上手 Tmux - 萌萌的It人 www.itmmd.com - CSDN博客


2014年11月22日 20:56:59[Jlins](https://me.csdn.net/dyllove98)阅读数：1085


如果你在做的项目需要你打开多个终端，那 Tmux（还有它的好基友 Tmuxinator）将会大大提高你的效率！Tmux 允许你在一个终端运行多个会话，Tmuxinator 则可以让你保存 tmux 的配置。
例如你是一个Rails的开发者， 你可以很轻松的打开不同的会话，同时处理几件事，比如运行Rails服务器，一个Rails终端，一个tailing 日志。
如果你是在一个[Clojure-based forum](http://www.flyingmachinestudios.com/programming/building-a-forum-with-clojure-datomic-angular/)工作时， 我也可以打开几个会话，
 一个shell，一个grunt服务器构建前端， 一个处理datomic数据库，一个用来部署，像下面这样：
![](http://ww1.sinaimg.cn/large/7cc829d3gw1em3tk5bb25j20kr0f5aba.jpg)
我可以敲入一个命令 mux ath，就可以全都启动好。 这比需要记住每个服务，然后手动单个启动要方便的多。
下面是一些上手 tmux 和tmuxinator的一些步骤
首先，使用安装tmux 参考[in this gist](https://gist.github.com/simme/1297707).
然后，使用 gem install  tmuxinator 安装 tmuxinator
然后在~/.tmuxinator/sample.yml 创建你的 第一个 tmuxinator配置文件，配置文件类似于下面这样，
|1
|2
|3
|4
|5
|6
|name:
 sample
|root:
 ~/path/to/your/project
|pre:
 git pull
|windows:
|-
 shell:
|-
 server: bundle exec rails s
|

在打开窗口前，在root的目录中运行pre选项中的命令， 你可以使用这个配置文件打开tmux的会话， 你可以使用C-b n 切换到下个窗口，`C-b p`切换到前一个窗口， C-b p 意味着你按下了控制键，然后按下了‘b’键， 想要离开一个tmux会话，可以使用  C-b
 d， 如果你离开了这个会话，它仍然在运行，任何你在一个窗口中启动的进程都依然在运行，我很少使用其他的命令，如果你需要，可以参考这里[tmux cheatsheet](http://cheat.errtheblog.com/s/tmux).
如果你要完全停掉一个会话，你需要杀死它， 使用tmux kill-session -t sample， sample是你的tmuxinator配置文件中name的选项名字，我创建了一个别名 alias “tmk”=”tmux kill-session -t”，然后就只要 tmk sample 就可以了
我希望这个工具对你有所帮助， 更多信息，参考[tmuxinator’s github repo](https://github.com/tmuxinator/tmuxinator)， 你可以干一些很炫的事，比如把屏幕切成多个，  这里还有一个参考书[a
 handy book](http://www.amazon.com/gp/product/1934356964/ref=as_li_tl?ie=UTF8&camp=1789&creative=390957&creativeASIN=1934356964&linkCode=as2&tag=aflyingmachin-20&linkId=KX7OZEGOV4WX5K7W)，
我希望这些小建议对你有所帮助。
![](http://images.cnitblog.com/blog/437282/201411/041405283617635.gif)
欢迎大家访问我的个人网站[萌萌的IT人](http://www.itmmd.com)

