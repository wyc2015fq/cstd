# 20171221 读mod_wsgi 官文档WSGIPythonHome的读书笔记 - weixin_33985507的博客 - CSDN博客
2017年12月22日 04:00:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：20
官文档的位置
http://modwsgi.readthedocs.io/en/develop/configuration-directives/WSGIPythonHome.html
一句话，它 就是WSGIDeamonProcess 中的 [option 中]python-home 的参数
**例如在我 的子试验02中，就是**
**/var/www/firstapp/venv**
**而 官方文档： http://modwsgi.readthedocs.io/en/develop/user-guides/virtual-environments.html?highlight=virtual**
**里的例子就是 WSGIDaemonProcess myapp python-home=/usr/local/venvs/myapp**
****另外 注意 这个 home== 的地址后面 没有 （NOT） “/”符号****
WSGIPythonHome
Description:Absolute path to Python prefix/exec_prefix directories.
Syntax:WSGIPythonHome*   prefix|prefix:exec_prefix*
Context tag: server config
Used to indicate to Python when it is initialised where its library filesare installed. This should be defined where the Python executable is not inthePATHof the user that Apache runs as, or where a system hasmultiple versions of Python installed in different locations in the filesystem, especially different installations of the same major/minor version,and the installation that Apache finds in itsPATHis not the desiredone.
This directive can also be used to indicate a Python virtual environmentcreated using a tool such asvirtualenv, to be used for the whole ofmod_wsgi.
