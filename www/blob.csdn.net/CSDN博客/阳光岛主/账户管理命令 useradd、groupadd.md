
# 账户管理命令 useradd、groupadd - 阳光岛主 - CSDN博客

2013年08月14日 23:16:40[阳光岛主](https://me.csdn.net/sunboy_2050)阅读数：18251



**内容提要：**
1. 掌握用户的 增/删/改 命令
2. 掌握组的 增/删/改 命令


**组管理**
**1）groupadd**
groupadd 用于添加组账号。格式如下：groupadd [-g GID] GROUP
其中：
GROUP：是要添加的组名
-g：用于指定 GID，默认为使用当前最大的 GID 加1
举例： 添加一个新组 leader
\# groupadd leader
\# grep leader /etc/group
leader:x:504:
\# grep leader /etc/gshadow
leader:!::

**2）groupmod**
groupmod 用于修改系统已存在的组账号。格式如下：groupmod [options] GROUP
其中：
GROUP：为要修改的组账户名
常用选项：
-g GID：重新指派 GID
-n NEW_GROUP：更改组名为 NEW_GROUP
举例： 将 leader 组更名为 leaders
\# groupmod -n leaders leader
\# grep leader /etc/group
leaders:x:504:
将 leaders 组的 GID 改为 3000
\# groupmod -g 3000 leaders
\# grep leader /etc/group
leaders:x:3000:

**3）groupdel**
groupdel 用于删除系统已存在的组账号。格式如下：groupdel GROUP
其中：GROUP 为要删除的组账户名
例如：删除 leaders 组
\# groupdel leaders
\# grep leaders /etc/group
\# grep leaders /etc/gshadow


**用户管理**
**1）useradd**
useradd 用于添加用户账号或设置添加用户使用的默认信息。格式如下：
格式1：useradd [options] LOGIN
格式2：useradd -D
格式3：useradd -D [options]
说明：
格式1：用于添加用户账号，LOGIN 为用户登录账号
格式2：用于显示添加用户使用的默认信息
格式3：用于设置添加用户使用的默认信息

添加用户选项：
选项 说明
-u UID 指定新用户的 UID，默认为使用当前最大的 UID 加1
-g GROUP 指定新用户的主组
-G GROUP1[,GROUP2,…[,GROUPN]]] 指定新用户的附加组
-d HOME_DIR 指定新用户的登录目录
-s SHELL 指定新用户使用的 Shell，默认为 bash

改变用户默认值选项：
在配置文件 /etc/login.defs 和 /etc/default/useradd 中存放了用户默认数据。

**使用举例：**
1、创建一个新用户 newuser，同时创建用户自家目录
\# useradd newuser
2、创建一个新用户 tom，同时加入 staff 附加组中
\# useradd -G staff tom
3、创建一个新用户 webmaster，不创建用户自家目录，指定登录目录 /www，同时加入 apache 附加组中
\# useradd -d /www -M -G apache webmaster

**2）****usermod**
usermod 用于修改系统已存在的组账号。格式如下：usermod [options] LOGIN
其中：
LOGIN：为要修改的用户账号名
选项：-c、-d、-e、-f、-g、-G、-s、-u 的含义与 useradd 选项的含义相同
添加了如下选项：
-l NEW_LOGIN：更改用户登录名为 NEW_LOGIN
-c COMMENT 说明用户的附加信息，如全名等
-e EXPIRE_DATE 指定用户的登录失效时间，格式为 YYYY-MM-DD
-f INACTIVE 指定在密码过期后多少天即关闭该账号, 默认值为 -1，即不做限制
-k SKEL_DIR 指定 skel 目录，默认存放在 /etc/skel/
-m 创建新用户的自家目录，默认值
-M 不创建新用户的自家目录
选项 说明
-b BASE_DIR
定义使用者所属目录的上级目录。用户名称会附加在BASE_DIR后面用来建立新使用者的目录。 当
然使用-d后则此选项无效。
-e
EXPIRE_DATE
定义使用者账号失效日期。
-f INACTIVE 定义在密码过期后多少天即关闭该账号。
-g GROUP 定义新账号起始组名或GID。组名必须为现已存在的组名。GID也必须为现已存在的GID。
-s SHELL 定义用户默认使用的 shell。
-L：锁定用户账号
-U：对用户账号解除锁定

**举例：**
1、将 newuser2 添加到组 staff 中
\# usermod -G staff newuser2
2、修改 newuser 的用户名为 newuser1
\# usermod -l newuser1 newuser
3、锁定账号 newuser1
\# usermod -L newuser1
4、解除对 newuser1 的锁定
\# usermod -U newuser1

**3）userdel**
userdel 用于删除系统已存在的组账号。格式如下：userdel [-r] LOGIN
其中：
LOGIN：为要删除的用户账号名
-r：同时删除用户的自家目录和 mail 的假脱机文件
例如：
1、删除用户 webmaster
\# userdel webmaster
2、删除用户 newuser1，同时删除其自家目录
\# userdel -r newuser1



