
# 将netcat-openbsd替换成为netcat-traditional - bitcarmanlee的博客 - CSDN博客


2017年05月15日 20:33:30[bitcarmanlee](https://me.csdn.net/bitcarmanlee)阅读数：990


ubuntu里默认的nc命令指向的是netcat-openbsd。因为netcat-openbsd的用法与netcat-traditional用法有所不同，所以想切换到nc的传统版本中。
## 1.update-alternatives 命令
update-alternatives是Debian系统中专门维护系统命令链接符的工具，通过它可以很方便的设置系统默认使用哪个命令、哪个软件版本。
```python
update-alternatives --help
用法：update-alternatives [<选项> ...] <命令>
命令：
  -
```
```python
-install <链接> <名称> <路径> <优先级>
```
```python
[--slave <链接> <名称> <路径>] ...
                           在系统中加入一组候选项。
  -
```
```python
-remove <名称> <路径>   从 <名称> 替换组中去除 <路径> 项。
```
```python
-
```
```python
-remove-all <名称>      从替换系统中删除 <名称> 替换组。
```
```python
-
```
```python
-auto <名称>            将 <名称> 的主链接切换到自动模式。
```
```python
-
```
```python
-display <名称>         显示关于 <名称> 替换组的信息。
```
```python
-
```
```python
-query <名称>           机器可读版的 --display <名称>.
```
```python
-
```
```python
-list <名称>            列出 <名称> 替换组中所有的可用候选项。
```
```python
-
```
```python
-get-selections         列出主要候选项名称以及它们的状态。
```
```python
-
```
```python
-set-selections         从标准输入中读入候选项的状态。
```
```python
-
```
```python
-config <名称>          列出 <名称> 替换组中的可选项，并就使用其中
```
```python
哪一个，征询用户的意见。
  -
```
```python
-set <名称> <路径>      将 <路径> 设置为 <名称> 的候选项。
```
```python
-
```
```python
-all                    对所有可选项一一调用 --config 命令。
```
```python
<链接> 是指向 /etc/alternatives/<名称> 的符号链接。
    (如 /usr/bin/pager)
<名称> 是该链接替换组的主控名。
    (如 pager)
<路径> 是候选项目标文件的位置。
    (如 /usr/bin/less)
<优先级> 是一个整数，在自动模式下，这个数字越高的选项，其优先级也就越高。
选项：
  -
```
```python
-altdir <目录>          改变候选项目录。
```
```python
-
```
```python
-admindir <目录>        设置 statoverride 文件的目录。
```
```python
-
```
```python
-log <文件>             改变日志文件。
```
```python
-
```
```python
-force                  就算没有通过自检，也强制执行操作。
```
```python
-
```
```python
-skip-auto              在自动模式中跳过设置正确候选项的提示
```
```python
(只与 --config 有关)
  -
```
```python
-verbose                启用详细输出。
```
```python
-
```
```python
-quiet                  安静模式，输出尽可能少的信息。不显示输出信息。
```
```python
-
```
```python
-help                   显示本帮助信息。
```
```python
-
```
```python
-version                显示版本信息。
```
常用的选项包括display,install,config,remove等。
## 2.替换netcat-openbsd
1.先安装传统的netcat
```python
sudo
```
```python
apt-get install netcat
```
2.运行如下命令：
```python
sudo
```
```python
update-alternatives --config nc
```
此时会显示如下：
```python
有 2 个候选项可用于替换 nc (提供 /bin/nc)。
```
```python
选择       路径               优先级  状态
------------------------------------------------------------
```
```python
*
```
```python
0            /bin/nc.openbsd       50        自动模式
```
```python
1            /bin/nc.openbsd       50        手动模式
```
```python
2            /bin/nc.traditional   10        手动模式
```
```python
要维持当前值[
```
```python
*]请按回车键，或者键入选择的编号：
```
3.根据提示选择
我们现在是想切换到第2个，所以输入2
4.Done！

