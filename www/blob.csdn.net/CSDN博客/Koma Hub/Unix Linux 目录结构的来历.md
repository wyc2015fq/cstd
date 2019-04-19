# Unix/Linux 目录结构的来历 - Koma Hub - CSDN博客
2018年06月03日 14:36:56[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：331
                Unix（包含Linux）的初学者，常常会很困惑，不明白目录结构的含义何在。
![](https://img-blog.csdn.net/20180603143450394)
举例来说，根目录下面有一个子目录/bin，用于存放二进制程序。但是，/usr子目录下面还有/usr/bin，以及/usr/local/bin，也用于存放二进制程序；某些系统甚至还有/opt/bin。它们有何区别？
长久以来，我也感到很费解，不明白为什么这样设计。像大多数人一样，我只是根据《Unix文件系统结构标准》（Filesystem Hierarchy Standard），死记硬背不同目录的区别。
直到我读到了Rob Landley的简短解释，这才恍然大悟，原来Unix目录结构是历史造成的。
话说1969年，Ken Thompson和Dennis Ritchie在小型机PDP-7上发明了Unix。1971年，他们将主机升级到了PDP-11。
![](https://img-blog.csdn.net/20180603143459303)
当时，他们使用一种叫做RK05的储存盘，一盘的容量大约是1.5MB。
![](https://img-blog.csdn.net/20180603143506990)
没过多久，操作系统（根目录）变得越来越大，一块盘已经装不下了。于是，他们加上了第二盘RK05，并且规定第一块盘专门放系统程序，第二块盘专门放用户自己的程序，因此挂载的目录点取名为/usr。也就是说，根目录"/"挂载在第一块盘，"/usr"目录挂载在第二块盘。除此之外，两块盘的目录结构完全相同，第一块盘的目录（/bin, /sbin, /lib, /tmp...）都在/usr目录下重新出现一次。
后来，第二块盘也满了，他们只好又加了第三盘RK05，挂载的目录点取名为/home，并且规定/usr用于存放用户的程序，/home用于存放用户的数据。
从此，这种目录结构就延续了下来。随着硬盘容量越来越大，各个目录的含义进一步得到明确。
/：存放系统程序，也就是At&t开发的Unix程序。
/usr：存放Unix系统商（比如IBM和HP）开发的程序。
/usr/local：存放用户自己安装的程序。
/opt：在某些系统，用于存放第三方厂商开发的程序，所以取名为option，意为"选装"。
（以上内容转自：[https://mp.weixin.qq.com/s/sAMx6fXvsu04R8MFstdRtA](https://mp.weixin.qq.com/s/sAMx6fXvsu04R8MFstdRtA)）
/media：媒体：软碟、光碟、硬盘、DVD等均挂在在这个目录下面；
/mnt：暂时挂载某些额外的装置；
/opt：给第三方协力软件放置的目录；
/root：系统管理员（root）的家目录；
/sbin：包含开机、修复、还原系统所需的指令；
/srv：service的缩写，是一些网络服务器户动之后，这些服务器所需要取用的资料目录，常见的如www、FTP等；
/tmp：正在执行的程序暂时放置档案的地方；
/boot：开机时会用到的档案；
/dev：装置与周边设备；
/etc：主要的设定档、密码、账号、各种服务的启动档案；
/home：使用者目录；
/lib：系统的函式库；
/lib/modules/：防止核心相关的模组；
**查看硬盘分区以及文件系统信息指令：**
$ df -Th
**查看文件大小信息：**
$ du -sh dirname
**查看文件树：**
$ tree dirname
**下面以cygwin为例简单给出他的类Unix系统的目录结构：**
```
$ tree -d -L 1 home/Toa/ dev etc lib opt sbin srv usr var -o ~/tree.txt
home/Toa/
├── 公共
├── 模板
├── 视频
├── 图片
├── 文档
├── 下载
├── 音乐
└── 桌面
dev
├── fd -> /proc/self/fd
├── mqueue
└── shm
etc
├── alternatives
├── avahi
├── bash_completion.d
├── bonobo-activation
├── boxes
├── dbus-1
├── dconf
├── defaults
├── duplicity
├── fish
├── fonts
├── fstab.d
├── gconf
├── ggi
├── gimp
├── gnome-settings-daemon
├── gnome-vfs-2.0
├── gtk
├── gtk-2.0
├── gtk-3.0
├── highlight
├── httpd
├── ImageMagick
├── ImageMagick-6
├── inetd.d
├── libgda-5.0
├── libvirt
├── lighttpd
├── mc
├── my.cnf.d
├── nas
├── nginx
├── OpenCL
├── openldap
├── openwsman
├── pango
├── php.d
├── php-fpm.d
├── pkcs11
├── pki
├── postinstall
├── preremove
├── profile.d
├── pulse
├── rc.d
├── sasl2
├── setup
├── sgml
├── skel
├── smartd_warning.d
├── snmp
├── sound
├── speech-dispatcher
├── squid
├── ssl
├── stunnel
├── sysconfig
├── tcm
├── texmf
├── tirc
├── unbound
├── UPower
├── vfu
├── WindowMaker
├── X11
├── x3270
├── xdg
├── xinetd.d
└── xml
lib
├── apr-util-1
├── arj
├── aspell-0.60
├── atkmm-1.6
├── avahi
├── avogadro
├── babl-0.1
├── bonobo
├── bonobo-2.0
├── cairomm-1.0
├── caja
├── clang
├── clisp-2.49+
├── cmake
├── csih
├── datefudge
├── dbus-1.0
├── debug
├── dri
├── empathy
├── enchant
├── eog
├── evince
├── evolution
├── evolution-data-server
├── farstream-0.2
├── fbpanel
├── fcgi
├── filemanager-actions
├── finch
├── flexdll
├── folks
├── freerdp-1.0
├── gambas3
├── gawk
├── gcc
├── GConf
├── gconfmm-2.6
├── gdkmm-2.4
├── gdk-pixbuf-2.0
├── gedit
├── gegl-0.2
├── gems
├── gettext
├── ggi
├── gimp
├── gio
├── giomm-2.4
├── girepository-1.0
├── gjs
├── glade
├── glade3
├── glib-2.0
├── glibmm-2.4
├── gnome-applets
├── gnome-calculator
├── gnome-commander
├── gnome-js
├── gnome-keyring
├── gnome-panel
├── gnome-settings-daemon-3.0
├── gnome-todo
├── gnome-vfs-2.0
├── gnome-vfsmm-2.6
├── gnt
├── goffice
├── graphviz-2.40
├── grilo-0.2
├── grilo-0.3
├── groff
├── gstreamer-1.0
├── gtk
├── gtk-2.0
├── gtk-3.0
├── gtkglext-1.0
├── gtkmm-2.4
├── guile
├── gwenhywfar
├── hamster-time-tracker
├── hexchat
├── httpd
├── ibus
├── ibus-hangul
├── icu
├── ImageMagick-6.9.5
├── ImageMagick-6.9.9
├── imlib2
├── imsettings
├── indicator-applet
├── indicators
├── indicators3
├── infinoted-0.6
├── input-pad-1.0
├── input-pad-1.1
├── juffed
├── kconf_update_bin
├── kde4
├── keepassx
├── krb5
├── krb5-auth-dialog
├── lapack
├── libcanberra-0.30
├── libgda-5.0
├── libglade
├── libglademm-2.4
├── libgnomecanvasmm-2.6
├── libgnomemm-2.6
├── libgnomeuimm-2.6
├── libkkc
├── libpeas-1.0
├── libproxy
├── lighttpd
├── lua
├── lv
├── lxqt-panel
├── m17n
├── mesa-demos
├── midori
├── mime
├── mission-control-plugins.0
├── mutter
├── mysql
├── nautilus
├── ncurses
├── nginx
├── nx
├── ocaml
├── openbabel
├── openmpi
├── openssl-1.0.2
├── orbit-2.0
├── org.gnome.Characters
├── p7zip
├── pangomm-1.4
├── perl5
├── php
├── pidgin
├── pkcs11
├── pkgconfig
├── pstoedit
├── pulse-11.1
├── pulseaudio
├── purple-2
├── pygtk
├── python2.6
├── python2.7
├── python3.4
├── python3.6
├── qpdfview
├── qt3
├── qt4
├── qt5
├── qupzilla
├── remmina
├── rpm
├── rpm-plugins
├── ruby
├── sasl2_3
├── scons-2.4.0
├── seahorse
├── security
├── sigc++-2.0
├── speech-dispatcher
├── speech-dispatcher-modules
├── sqlite3
├── squid
├── strigi
├── stunnel
├── sushi
├── swipl-7.2.3
├── syslog-ng
├── systemd
├── tcl8
├── tcl8.5
├── tcl8.6
├── Ted
├── telepathy
├── texinfo
├── Tix8.4.3
├── tk8.6
├── urxvt
├── vala-0.30
├── vala-0.34
├── vala-0.36
├── vala-0.38
├── valadoc-0.38
├── varnish
├── vfu
├── voikko
├── w32api
├── w3m
├── wcd
├── weechat
├── wnres
├── wx
├── X11
├── xfce4
├── xinetd
├── xorg
├── zinnia
├── znc
└── zsh
opt
└── gcc-tools
sbin
srv
└── www
usr
├── bin
├── doc
├── i586-pc-msdosdjgpp
├── i686-pc-cygwin
├── i686-w64-mingw32
├── include
├── lib
├── libexec
├── local
├── openwin
├── sbin
├── share
├── src
├── ssl
├── tmp
├── x86_64-pc-cygwin
└── x86_64-w64-mingw32
var
├── cache
├── cron
├── empty
├── games
├── lib
├── lock
├── log
├── openldap
├── run
├── spool
├── tmp
└── varnish
313 directories
```
