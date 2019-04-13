
# InstallShield脚本事件 - 致林 - 博客园






# [InstallShield脚本事件](https://www.cnblogs.com/bincoding/p/8670389.html)
脚本事件主要有三大类：Before Move Data(安装数据前)、Move Data(安装数据过程中)、After Move Data(安装数据后)。
OnBegin:在初始化后，被调用
OnFirstUIBefore:第一次安装时，在复制安装数据之前触发的事件，更改这个事件的脚本可自定义安装步骤（一般我们都是修改这个）
OnMainUIBefore:在修改或卸载时，在复制安装数据之前触发的事件，例如安装程序在安装时添加了服务,在卸载时安装程序不会将此服务反安装，这就需要脚本删除服务。
OnSQLLogin:Ms SQL SERVER数据库安装登录函数。
MoveData：一般很少有人会去改
OnFirstUIAfter: 第一次安装时，在复制安装数据之后触发的事件，例如有些地方需要设置路径，在安装前路径不存在，必须安装后才能设置。
OnMainUIAfter: 在修改或卸载时，在修改或卸载数据之后触发的事件
OnEnd:在安装完成之后触发的事件，即点击【完成】按钮后触发的事件。





