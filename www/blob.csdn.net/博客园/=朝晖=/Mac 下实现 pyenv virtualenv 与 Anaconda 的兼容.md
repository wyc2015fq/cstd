# Mac 下实现 pyenv/virtualenv 与 Anaconda 的兼容 - =朝晖= - 博客园
# [Mac 下实现 pyenv/virtualenv 与 Anaconda 的兼容](https://www.cnblogs.com/dhcn/p/8117250.html)
http://blog.csdn.net/vencent7/article/details/76849849
自己一直用的 pyenv 和 pyenv-virtualenv 管理不同的 python 环境。昨天朋友推荐我安装一个 Anaconda（通过官网下载安装带有图形界面Anaconda Navigator 的版本，并非通过 pyenv 安装），结果安装之后似乎与原来的 pyenv 不太兼容，在网上搜了一下，有遇到类似情况的：[https://stackoverflow.com/questions/40944277/pyenv-and-anaconda-issue-with-export-path](https://stackoverflow.com/questions/40944277/pyenv-and-anaconda-issue-with-export-path)
经过尝试，我找到的解决方案如下：
### 解决 PATH 环境变量的问题
anaconda 安装的时候会在 `.bash_profile` 的**最后一行**把自己的 PATH 加到最前，正是因为这个原因导致 pyenv 失效了（pyenv 也是通过把自己的 PATH加到最前来“劫持”`python`等命令实现多版本的管理），因为按照官方的安装说明，**pyenv 的几行命令应该放置在`.bash_profile`的最后**。
因此，编辑.bash_profile，使得 pyenv 的命令放置在 anaconda 的之后：
```bash
# added by Anaconda3 4.4.0 installer
export PATH="/Users/frederic/anaconda/bin:$PATH"
# pyenv
export PYENV_ROOT="$HOME/.pyenv"
export PATH="$PYENV_ROOT/bin:$PATH"
eval "$(pyenv init -)"
eval "$(pyenv virtualenv-init -)"
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
### 解决virtualenv识别不到anaconda的虚拟环境的问题
通过以上步骤，pyenv/virtualenv 的优先级已经比 anaconda 高了，因此如果你运行`python -version`看到的版本应该是通过 pyenv 指定的版本，而不是 anaconda 的版本。
然而，如何运行 anaconda 版本的 python 呢？
- 如果没有创建过环境而只有默认的 root 的话，我们需要先通过conda 或者anaconda navigator图形界面新建一个environment
- 把刚刚创建的 environment 文件夹软链接到`.pyenv/versions`，即让 pyenv/virtualenv可以识别到 anaconda 创建的环境（如果是通过 pyenv 安装的 anaconda 应该可以直接识别到，但是我的 anaconda 是独立安装的，因此 pyenv 没有检测到）：`ln -s ~/anaconda/envs/anaconda_env ~/.pyenv/versions/anaconda_env`
- 现在输入pyenv versions命令应该能看到 anaconda 的环境了，可以通过virtualenv的 activate/deactivate 命令或者 pyenv 的`pyenv shell/local/glocal [env_name]`命令切换版本

