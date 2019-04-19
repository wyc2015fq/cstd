# Ubuntu本地uwsgi配Django问题的解决 - =朝晖= - 博客园
# [Ubuntu本地uwsgi配Django问题的解决](https://www.cnblogs.com/dhcn/p/7115439.html)
     版本Ubuntu14.04，Django1.6.5 ubuntu本地Uwsgi调Django可能会报错：
```
-- unavailable modifier requested: 0 --
```
     解决方法:先安装一个组件：
```
apt-get install uwsgi-plugin-python
```
然后uwsgi设置文件里添加一行：
```
plugins=python
```
  目前2014年7月8号离Ubuntu14.04发布时间还不久，新版本可能还需要进化。pip里面的uwsgi安装时也会报错。
      部署调试的时候，一定要把Django Settings的debug打开。出了问题好诊断。
参考：http://stackoverflow.com/questions/10748108/nginx-uwsgi-unavailable-modifier-requested-0

