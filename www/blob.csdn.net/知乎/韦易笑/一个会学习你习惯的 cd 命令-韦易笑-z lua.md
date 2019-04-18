# 一个会学习你习惯的 cd 命令 - z.lua - 知乎
# 



终端下工作最烦躁的就是路径切换，每天不停的 `cd`，就像在泥里走路，今天给大家介绍一个好用的工具：[z.lua](https://link.zhihu.com/?target=https%3A//github.com/skywind3000/z.lua) ，用熟后路径切换就像是终端里溜冰。

[z.lua](https://link.zhihu.com/?target=https%3A//github.com/skywind3000/z.lua) 是一个会学习你使用习惯的 `cd` 命令，它会跟踪你在 shell 下访问过的路径，通过一套称为 Frecent 的机制（源自 Mozilla），经过一段简短的学习之后，`z.lua` 会帮你跳转到所有匹配正则关键字的路径里 Frecent 值最高的那条路径去。

正则将按顺序进行匹配，"z foo bar" 可以匹配到 /foo/bar ，但是不能匹配 /bar/foo。

**特性说明：**
- 性能比 **z.sh** 快三倍，比 **fasd** / **autojump** 快十倍以上。
- 支持 `Posix Shell`：`bash`, `zsh`, `sh`, `ash`, `ksh`, `busybox` 等等。
- 支持 `Fish Shell`，`Power Shell` 和 `Windows cmd`。
- 增强匹配算法，更准确的带你去到你想去的地方。
- 交互选择模式，如有多个匹配结果，`cd` 前允许你进行选择。
- 支持 `fzf` （可选）进行可视化结果筛选或者补全。
- **快速回到父目录**，代替反复 “cd ../../.." 。
- 高性能，地占用，为不同 Shell 提供统一的使用体验。
- 兼容 `lua` 5.1, 5.2 和 5.3 以上版本，兼容 `LuaJit`。
- 自包含且无额外依赖，单个 `z.lua` 文件完成所有工作。

**软件安装：**

将下面对应语句添加到你的`.bashrc`/`.zshrc`或者对应 shell 的 rc 脚本末尾：


```bash
eval "$(lua /path/to/z.lua --init bash enhanced once echo)"    # BASH 初始化
eval "$(lua /path/to/z.lua --init zsh enhanced once echo)"     # ZSH 初始化
eval "$(lua /path/to/z.lua --init posix enhanced once echo)"   # 其他 Posix Shell 初始化
source (lua /path/to/z.lua --init fish enhanced once echo | psub)  # Fish Shell 初始化
iex ($(lua /path/to/z.lua --init powershell enhanced once echo) -join "`n") # Power Shell 初始化
```


然后重新登陆，即可使用。




**基本使用：**

cd 到一个包含 foo 的目录：


```bash
z foo
```


cd 到一个以 foo 结尾的目录：


```bash
z foo$
```


对长路径使用多个关键字进行匹配：

假设路径历史数据库（~/.zlua）中有两条记录：


```
10   /home/user/work/inbox
30   /home/user/mail/inbox
```


`"z in"` 将会跳转到 `/home/user/mail/inbox` 因为它有更高的权重，同时你可以传递更多参数给 z.lua 来更加精确的指明，如 `"z w in"` 则会让你跳到 `/home/user/work/inbox`。




**交互选择：**

使用 -i 参数进行跳转时, 如果有多个匹配结果，那么 z.lua 会给你显示一个列表：


```bash
$ z -i soft
3: 0.25   /home/data/software
2: 3.75   /home/skywind/tmp/comma/software
1: 21     /home/skywind/software
> {光标位置}
```


然后你按照最前面的序号输入你想要去的地方，比如输入 3 就会进入 `/home/data/software`。如果你不输入任何东西直接按回车，那么将会直接退出而不进行任何跳转。

PS：交互选择同时支持 "-I" 参数（大写 i），让你可以使用 fzf 在多项结果进行快速选择（如果系统里面安装了的话）。




**快速回到父目录：**

`-b` 选项可快速回到父目录，避免重复输入 `cd ../../..`，先把 `z -b` 别名成 `zb` ：
- （没有参数）：cd 到项目根目录，即跳转到父目录中包含 (.git/.svn/.hg) 的地方。
- （单个参数）：cd 到最近一级以关键字开头的父目录中。
- （两个参数）：对当前目录进行字符串替换，然后 cd 过去。

使用举例：


```bash
# 一直向上退到项目根目录（就是里面有一个 .git 目录的地方）
~/github/lorem/src/public$ zb
  => cd ~/github/lorem

# cd 到第一个以 g 开头的父目录
~/github/vimium/src/public$ zb g
  => cd ~/github

# 快速回到 site 目录
~/github/demo/src/org/main/site/utils/file/reader/whatever$ zb si
  => cd ~/github/demo/src/org/main/site

# 将 jekyll 替换为 ghost
~/github/jekyll/test$ zb jekyll ghost
  => cd ~/github/ghost/test
```


打开环境变量 `$_ZL_ECHO`（用来显示跳转结果）后，可以搭配其他工具，在目标目录内执行命令，而并不需要改变当前工作目录（比如：ls `zb git` ）。

环境变量 `$_ZL_ROOT_MARKERS` 是个逗号分隔的列表，用来识别项目根目录，可改成：


```bash
export _ZL_ROOT_MARKERS=".git,.svn,.hg,.root,package.json"
```


这样在用 `zb` 时，可以回到包含 `.root` 文件，或者 `package.json` 文件的父目录。

**Bonus**：`zb ..` 相当于 `cd ..`，`zb ...` 相当于 `cd ../..`，而 `zb ....` 相当于 `cd ../../..` 等等。 最后 `zb ..20` 等同于调用 `cd ..` 二十次。




**最近常去的目录：**

`z.lua` 提供 `dirstack` 让你更便捷的访问最近刚刚去过的目录，而不需要输入任何关键字。这个方法叫做 `dirstack`，它记录着最近你刚刚去过的 10 条最新路径，然后是用 `z -`，`z --` 和 `z -{num}` 来操作：


```
# 显示当前的 dir stack
$ z --    
 0  /home/skywind/work/match/memory-match
 1  /home/skywind/.local/etc
 2  /home/skywind/software/vifm-0.9.1
 3  /home/skywind/work
 4  /home/skywind/work/match

# cd 到栈里 2 号路径
$ z -2
  => cd /home/skywind/software/vifm-0.9.1

# 弹出栈顶 (cd 到上一次的老路径)，和 "z -0" 相同
$ z -
  => cd -
```


这个 `dirstack` 是根据 z.lua 的路径历史数据计算的出来的，和具体的 shell 或者操作系统无关。你退出再登陆不会丢失这些路径历史，不同的 shell 之间也可以共享同一份记录。

此外，还能通过前面提到的 `-I` 和 `-t` 参数组和，使用 fzf 选择最近去过的地方：


```bash
alias zh='z -I -t .'
```


首先定义个新的别名 `zh`（回到历史路径的意思），我们用 `-t` 参数告诉 `z.lua` 按时间戳为权重排序，同时 `-I` 启用 fzf 进行搜索，最后句号代表任意路径。

当我们在命令行里输入 zh 时，就可用 fzf 进行历史路径操作了：
![](https://pic2.zhimg.com/v2-cbb960f61de165ca457bfb0cb94c18bd_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='674' height='313'></svg>)
第一列为上次访问距今多少秒，第二列是目录名。你可以试着敲几个字母，用 fzf 的字符串模糊匹配进行定位，或者用光标键的上和下（CTRL+J/K 也可以）来上下移动，最后按回车 cd 过去，或者 ESC 放弃。




**常用技巧：**

推荐一些常用别名，放在你的配置文件中：


```bash
alias zz='z -c' # 严格匹配当前路径的子路径
alias zi='z -i' # 使用交互式选择模式
alias zf='z -I' # 使用 fzf 对多个结果进行选择
alias zb='z -b' # 快速回到父目录
```


z.lua 可以方便的导入 [z](https://link.zhihu.com/?target=http%3A//z.sh/).sh 的数据（他们格式相同）：


```bash
cat ~/.z >> ~/.zlua
```


还可以导入 autojump 的数据：


```bash
FN="$HOME/.local/share/autojump/autojump.txt"
awk -F '\t' '{print $2 "|" $1 "|" 0}' $FN >> ~/.zlua
```





**用户评价：**

“我喜欢它的原理，我在命令行下面的行为十分有规律，并且我十分懒惰，懒得管理和维护一套书签或者路径别名。“

“总之，z.lua 做到了它所宣称的目标。如果你需要快速的在目录间切换，它可能是最好的选择。“

”我终于可以在自己的树莓派1上使用 autojump 类似的功能了，而不用像以前传统 autojump 那样每次打开终端都要等待 30 秒。”




更多内容，参考项目文档：

[skywind3000/z.lua](https://link.zhihu.com/?target=https%3A//github.com/skywind3000/z.lua)






