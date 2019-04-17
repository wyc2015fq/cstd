# virtualenv安装tensorflow - Keith - CSDN博客





2016年11月23日 21:32:49[ke1th](https://me.csdn.net/u012436149)阅读数：6817








# 使用virtualenv装tensorflow

如果你有两个项目，一个需要`python2.7`开发，一个需要`python3.5`开发，那么`virtualenv`是一个很好的选择。

## 准备

如果你想创建一个`python2.7`和`python3.5`的虚拟环境的话。首先你的电脑上得装有`python2.7`和`python3.5`，而且需要装好`pip`和`pip3`。

## 安装

```
sudo pip install virtualenv #使用pip或pip3都可以，没啥影响
#创建环境，选择你想要的python版本（前提是你的电脑上已经安装好了）
virtualenv --no-site-packages --python=python3.5 ~/tensorflow

#激活环境
cd ~/tensorflow
source bin/activate
#安装tf,前面千万不要加sudo，执行任何pip命令都不要加sudo
pip install --upgrade https://storage.googleapis.com/tensorflow/linux/gpu/tensorflow-0.11.0-cp35-cp35m-linux_x86_64.whl
#退出环境
deactivate
```

## pycharm 与 virtualenv

出现创建项目对话框，点击小齿轮，`add local`，选择你刚才创建的虚拟环境的`python`解释器就可以了。

## virtualenvwrapper

s使用`virtualenv`的话，每次都要`source activate` 这样是相当麻烦的，有一个工具可以帮助我们简化切换到虚拟环境的步骤。

```python
1. sudo pip install virtualenvwrapper
2. 打开 .bashrc 添加
# virtualenv and virtualenvwrapper
export WORKON_HOME=$HOME/.virtualenvs
source /usr/local/bin/virtualenvwrapper.sh
3. source ~/.bashrc
4. 创建虚拟环境
mkvirtualenv name -p python3/2
5. 删除虚拟环境
rmvirtualenv name
6. 进入环境
workon name #你会看到console提示符发生了变化(name)user@PC:~/
7. 退出环境
deactivate
```

比`virtualenv`方便的多。



