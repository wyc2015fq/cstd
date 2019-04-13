
# MacBook Pro 下Bash Shell 利用Alias 简化命令 - 阳光岛主 - CSDN博客

2014年06月22日 17:31:31[阳光岛主](https://me.csdn.net/sunboy_2050)阅读数：22283


Alias是linux中常用的别名命令，这么好的东东在mac中自然不会舍去。当有一些比较复杂的命令需要经常执行的时候，alias对效率的提升立竿见影。我的博客：[http://blog.ithomer.net](http://blog.ithomer.net/2014/06/macbook-pro-bash-shell-using-alias-simplify-commands/)
**示例1：**
例如我就经常使用goagent，每次都要执行”python /Users/xhat/Downloads/goagent/local/proxy.py”，这是一个很痛苦的过程，往往cd半天，有时候甚至忘了路径。效率极低。那么通过alias现在可以这样改进：
> 1.编辑~/.bash_profile，
> 通常这个文件是不存在的，那么新建它
> ，有的话就打开修改添加。

> 2.vim ~/.bash_profile 添加
> alias go=”python /Users/xhat/Downloads/goagent/local/proxy.py”
> ，这里需要注意的是，等号前后不能出现空格。

> 3.bash下执行”
> source ~/.bash_profile
> “，此条命令是使bash重新载入配置令刚才命令生效。

> 4.以后需要使用上述复杂命令，只需在bash中输入“go”即可。
要查看自定义的alias命令，直接在bash中输入alias即可查看。
![macbook-shell-alias-command](http://cdn.ithomer.net/wp-content/uploads/2014/06/macbook-shell-alias-command.png)
**示例2：**
ubuntu 里面有命令“ll”，直接代替了“ls -l”命令，使用起来非常方便，但是Mac Book Pro系统默认没有“ll”命令，于是需要自己用别名（alias）来重设。
alias 别名步骤如下：
1） sudo vim  /etc/profile（打开/etc/profile文件，并设为可写权限，并用管理员sudo权限修改）
2）alias ll=’ls -l’ （重置别名）
3）保存
4）source /etc/profile （source命令是修改文件/etc/profile生效）
5） 在shell终端，输入“alias”命令查看，如上图，或直接输入“ll”查看结果
![macbook-shell-alias-command-2](http://cdn.ithomer.net/wp-content/uploads/2014/06/macbook-shell-alias-command-2.png)

