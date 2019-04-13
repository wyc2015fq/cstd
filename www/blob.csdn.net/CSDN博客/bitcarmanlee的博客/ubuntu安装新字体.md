
# ubuntu安装新字体 - bitcarmanlee的博客 - CSDN博客


2018年03月28日 16:24:02[bitcarmanlee](https://me.csdn.net/bitcarmanlee)阅读数：9954标签：[ubuntu																](https://so.csdn.net/so/search/s.do?q=ubuntu&t=blog)[terminator																](https://so.csdn.net/so/search/s.do?q=terminator&t=blog)[字体																](https://so.csdn.net/so/search/s.do?q=字体&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=terminator&t=blog)[
																					](https://so.csdn.net/so/search/s.do?q=ubuntu&t=blog)个人分类：[os																](https://blog.csdn.net/bitcarmanlee/article/category/6214152)
[
																								](https://so.csdn.net/so/search/s.do?q=ubuntu&t=blog)


ubuntu里新装的terminator里，字体实在是不忍直视。尤其是字母i，跟别的字母挤在一起，根本就看不清楚。所以特意下载了一个苹果的Monaco字体来代替。
linux系统的字体文件放在/usr/share/fonts/目录以及用户的~/.fonts和~/.local/share/fonts目录下，第一个位置为系统所用用户共享，将字体安装到这个目录需要管理员权限；后面两个位置则为当前登陆用户所有,安装字体到这个目录不需要管理员权限。
废话不多说，直接上教程，亲测可用：
1.安装到 /usr/share/fonts/
```python
wget https://github.com/fangwentong/dotfiles/raw/master/ubuntu-gui/fonts/Monaco.ttf
```
```python
sudo
```
```python
mkdir -p /usr/share/fonts/custom
```
```python
sudo
```
```python
mv Monaco.ttf /usr/share/fonts/custom
```
```python
sudo
```
```python
chmod
```
```python
744
```
```python
/usr/share/fonts/custom/Monaco.ttf
```
```python
sudo
```
```python
mkfontscale
```
```python
#生成核心字体信息
```
```python
sudo
```
```python
mkfontdir
```
```python
sudo
```
```python
fc-cache -fv
```
依次执行上述命令行，即可安装成功！然后在terminator中修改对应的字体即可！
2.安装到 ~/.fonts/ (安装到 ~/.local/share/fonts 原理相同)
```python
wget https://github
```
```python
.com
```
```python
/fangwentong/dotfiles/blob/ubuntu/fonts/Monaco
```
```python
.ttf
```
```python
?raw=true
mkdir -p ~/
```
```python
.fonts
```
```python
mv Monaco
```
```python
.ttf
```
```python
~/
```
```python
.fonts
```
```python
fc-cache -vf
```
```python
#刷新系统字体缓存
```
此部分逻辑没有亲自测试，没有root权限的同学们可以自己尝试！

