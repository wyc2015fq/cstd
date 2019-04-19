# python安装模块时报异常：Compression requires the (missing) zlib module - 宇宙浪子的专栏 - CSDN博客
2016年02月05日 10:18:56[_宇宙浪子_](https://me.csdn.net/bluehawksky)阅读数：11309
linux系统自带的python版本是2.6.6, 升级到最新版本2.7.11后，安装setuptools模块时报异常：
```python
Traceback (most recent call last):
  File "setup.py", line 169, in <module>
    dist = setuptools.setup(**setup_params)
  File "/usr/lib/python2.6/distutils/core.py", line 152, in setup
    dist.run_commands()
  File "/usr/lib/python2.6/distutils/dist.py", line 975, in run_commands
    self.run_command(cmd)
  File "/usr/lib/python2.6/distutils/dist.py", line 995, in run_command
    cmd_obj.run()
  File "/opt/app/py_package/setuptools-18.6.1/setuptools/command/install.py", line 67, in run
    self.do_egg_install()
  File "/opt/app/py_package/setuptools-18.6.1/setuptools/command/install.py", line 109, in do_egg_install
    self.run_command('bdist_egg')
  File "/usr/lib/python2.6/distutils/cmd.py", line 333, in run_command
    self.distribution.run_command(command)
  File "/usr/lib/python2.6/distutils/dist.py", line 995, in run_command
    cmd_obj.run()
  File "/opt/app/py_package/setuptools-18.6.1/setuptools/command/bdist_egg.py", line 222, in run
    dry_run=self.dry_run, mode=self.gen_header())
  File "/opt/app/py_package/setuptools-18.6.1/setuptools/command/bdist_egg.py", line 463, in make_zipfile
    z = zipfile.ZipFile(zip_filename, mode, compression=compression)
  File "/usr/lib/python2.6/zipfile.py", line 663, in __init__
    "Compression requires the (missing) zlib module"
RuntimeError: Compression requires the (missing) zlib module
[root@1921681058 setuptools-18.6.1]#
```
需要先安装zlib，再重新编译python，方式两种：
1. 用yum
yum install zlib
yum install zlib-devel
2. 下载zlib rpm包
下载地址：http://rpmfind.net/linux/RPM/index.html 
zlib安装完成后进入python源码目录执行：
make clean
make
sudo make install
