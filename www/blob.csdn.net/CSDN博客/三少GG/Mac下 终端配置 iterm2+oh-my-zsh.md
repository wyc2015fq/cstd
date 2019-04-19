# Mac下 终端配置 iterm2+oh-my-zsh - 三少GG - CSDN博客
2017年05月03日 23:52:47[三少GG](https://me.csdn.net/scut1135)阅读数：1929

[https://www.zhihu.com/question/21418449](https://www.zhihu.com/question/21418449) zsh优点
1.**Iterm2**
2.**Oh-my-zsh**
zsh本身非常强大，回复1 [https://www.zhihu.com/question/21418449](https://www.zhihu.com/question/21418449)
再加上此包管理    回复2   https://www.zhihu.com/question/21418449
除了oh my zsh外，zsh还有另一款plugin管理工具叫prezto：
[GitHub
 - sorin-ionescu/prezto: The configuration framework for Zsh](https://link.zhihu.com/?target=https%3A//github.com/sorin-ionescu/prezto)
看好多人说fish比zsh好，其实fish是拆箱即用，而zsh定制性强。所以fish给人第一印象就会很好，有很多炫酷的特性。
当我还在纠结 这个选择时，我曾在知乎提过一个问题：[fish比zsh好用吗？ - Linux](https://www.zhihu.com/question/41076349)
zsh支持递归
fish好比ubuntu、linux mint，zsh好比arch linux、gentoo。
如果用默认配置，那么zsh也没比bash强大到哪里去，而fish的默认配置以及很炫酷了。
作者：Ivan
链接：https://www.zhihu.com/question/21418449/answer/95753913
来源：知乎
著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。
#################################################
**fuck插件**
**Fish 不敌 zsh**
**[http://jmolivas.com/improve-your-shell-using-fish-and-oh-my-fish](http://jmolivas.com/improve-your-shell-using-fish-and-oh-my-fish)**
[https://hackercodex.com/guide/install-fish-shell-mac-ubuntu/](https://hackercodex.com/guide/install-fish-shell-mac-ubuntu/)
# Installing Fish on macOS
There are several ways to install Fish on macOS:
- traditional `.pkg` installer
- standalone Mac application
- Homebrew
- manual compilation
The first two options are available from the [Fish web site](http://fishshell.com/) and are straightforward enough to be considered self-explanatory.
That said, if you already use Homebrew (which I recommend) and have configured your environment as noted in the [macOS
 Setup Guide](http://hackercodex.com/guide/mac-osx-mavericks-10.9-configuration/), then you can install Fish as you would any other package:
brew install fish
If you would prefer to install the latest bleeding-edge version of Fish via Homebrew, run the following instead of the above command:
**brew install --HEAD fish**
Once installation has completed, add Fish to `/etc/shells`, which will require an administrative password:
echo"/usr/local/bin/fish"| sudo tee -a /etc/shells
To make Fish your default shell:
chsh -s /usr/local/bin/fish
Now that Fish is installed, you can proceed to the [Basic Configuration](https://hackercodex.com/guide/install-fish-shell-mac-ubuntu/#basic-configuration) section below.
