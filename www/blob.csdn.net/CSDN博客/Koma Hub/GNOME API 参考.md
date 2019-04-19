# GNOME API 参考 - Koma Hub - CSDN博客
2019年01月14日 21:40:53[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：73
个人分类：[GTK																[Glib																[GNOME](https://blog.csdn.net/Rong_Toa/article/category/8611720)](https://blog.csdn.net/Rong_Toa/article/category/8611689)](https://blog.csdn.net/Rong_Toa/article/category/7156203)
GitHub：[https://github.com/GNOME](https://github.com/GNOME)
[https://developer.gnome.org/references](https://developer.gnome.org/references)
**目录**
[API 参考](#subindex-references)
[核心库](#api-base)
[核心](#api-core)
[用户界面](#api-ui)
[多媒体](#api-multimedia)
[交流](#api-communication)
[辅助功能](#api-accessibility)
[数据存储](#api-datastorage)
[实用工具](#api-utilities)
[其他](#api-others)
[其他](#%E5%85%B6%E4%BB%96)
[其他库](#api)
[交流](#api-communication)
[多媒体](#api-multimedia)
[实用工具](#api-utilities)
[内省](#introspection-bindings)
[其他](#%E5%85%B6%E4%BB%96)
[语言绑定](#api-bindings)
[C++](#c++-bindings)
[Vala](#vala-bindings)
[Java](#java-bindings)
[Python](#python-bindings)
[JavaScript](#javascript-bindings)
[GNOME 应用程序插件](#api-desktop-plugins)
[标准](#standards)
# API 参考
## 核心库
### 核心
[GLib Reference Manual](https://developer.gnome.org/glib/stable/)
GLib 为 C 语言编写的库和程序提供了核心应用程序组件。它提供了 GNOME 中使用的核心对象系统， main 循环的实现以及操作字符串和常用数据结构的一整套工具函数。
[Previous Versions](https://developer.gnome.org/glib/)
[GIO Reference Manual](https://developer.gnome.org/gio/stable/)
GIO 提供了一个现代、易用的 VFS API。它提供一个文件系统抽象，其允许应用程序使用单个一致的 API 访问本地和远程文件。
[Previous Versions](https://developer.gnome.org/gio/)
[GObject Reference Manual](https://developer.gnome.org/gobject/stable/)
GObject 提供了 Pango 和 GTK+ 使用的对象系统。
[Previous Versions](https://developer.gnome.org/gobject/)
### 用户界面
[Pango Reference Manual](https://developer.gnome.org/pango/stable/)
Pango 是 GNOME 应用程序中使用的核心文本和字体处理函数库。它对全球使用的不同书写系统有着广泛支持。
[Previous Versions](https://developer.gnome.org/pango/)
[GDK-PixBuf Reference Manual](https://developer.gnome.org/gdk-pixbuf/stable/)
GdkPixbuf 是一个图像加载和处理库。GdkPixbuf 文档同时包含了程序员指南和 API 参考。
[Previous Versions](https://developer.gnome.org/gdk-pixbuf/)
[GDK 3 Reference Manual](https://developer.gnome.org/gdk3/stable/)
一个中间层，其将 GTK+ 从窗口系统的细节中隔离。
[Previous Versions](https://developer.gnome.org/gdk3/)
[GDK 4 Reference Manual](https://developer.gnome.org/gdk4/stable/)
一个中间层，其将 GTK+ 从窗口系统的细节中隔离。
[Previous Versions](https://developer.gnome.org/gdk4/)
[GSK 4 Reference Manual](https://developer.gnome.org/gsk4/stable/)
An intermediate layer which isolates GTK+ from the details of the OpenGL or Vulkan implementation.
[Previous Versions](https://developer.gnome.org/gsk4/)
[GTK+ 3 Reference Manual](https://developer.gnome.org/gtk3/stable/)
GTK+ 是一个构建 GNOME 应用程序用户界面的主要库。它提供用来进行用户界面控制的用户界面控件和信号回调函数。
[Previous Versions](https://developer.gnome.org/gtk3/)
[GTK+ 4 Reference Manual](https://developer.gnome.org/gtk4/stable/)
GTK+ 是一个构建 GNOME 应用程序用户界面的主要库。它提供用来进行用户界面控制的用户界面控件和信号回调函数。
[Previous Versions](https://developer.gnome.org/gtk4/)
[WebKit2GTK+ 参考手册](http://webkitgtk.org/reference/webkit2gtk/stable/index.html) [外部资源]
GNOME 平台的网页内容渲染
[Cairo: A Vector Graphics Library](https://developer.gnome.org/cairo/stable/)
Cairo 是一个支持多种输出设备的 2D 绘图库。其设计为在所有输出媒体上产生一致的输出，同时在可能时利用显示硬件加速。
[Previous Versions](https://developer.gnome.org/cairo/)
[Clutter Reference Manual](https://developer.gnome.org/clutter/stable/)
Clutter 是一个基于 GObject 的库，用来创建快速、视觉效果丰富的图形用户界面。
[Previous Versions](https://developer.gnome.org/clutter/)
### 多媒体
[GStreamer 库参考手册](https://developer.gnome.org/gstreamer-libs/stable/)
同时支持音视频的强大多媒体应用开发框架。
[Previous Versions](https://developer.gnome.org/gstreamer-libs/)
[GStreamer 核心参考手册](https://developer.gnome.org/gstreamer/stable/)
同时支持音视频的强大多媒体应用开发框架。
[Previous Versions](https://developer.gnome.org/gstreamer/)
[GStreamer 核心插件参考手册](https://developer.gnome.org/gst-plugins-libs/stable/)
同时支持音视频的强大多媒体应用开发框架。
[Previous Versions](https://developer.gnome.org/gst-plugins-libs/)
[GUPnP A/V Reference Manual](https://developer.gnome.org/gupnp-av/unstable/)
用于简化 UPnP 音频/视频处理的小型工具库
[Previous Versions](https://developer.gnome.org/gupnp-av/)
[GUPnP DLNA Reference Manual](https://developer.gnome.org/gupnp-dlna/unstable/)
为使用 gupnp 的程序简化 DLNA 相关位操作的库。
[Previous Versions](https://developer.gnome.org/gupnp-dlna/)
[libcanberra Reference Manual](https://developer.gnome.org/libcanberra/unstable/)
libcanberra 是一个 XDG 声音主题和命名规范的实现，用以在自由桌面上生成事件声音，
[Previous Versions](https://developer.gnome.org/libcanberra/)
[librygel-core 参考手册](https://developer.gnome.org/librygel-core/unstable/)
[Previous Versions](https://developer.gnome.org/librygel-core/)
[librygel-renderer 参考手册](https://developer.gnome.org/librygel-renderer/unstable/)
[Previous Versions](https://developer.gnome.org/librygel-renderer/)
[librygel-renderer-gst 参考手册](https://developer.gnome.org/librygel-renderer-gst/unstable/)
[Previous Versions](https://developer.gnome.org/librygel-renderer-gst/)
[librygel-server 参考手册](https://developer.gnome.org/librygel-server/unstable/)
[Previous Versions](https://developer.gnome.org/librygel-server/)
### 交流
[GSSDP Reference Manual](https://developer.gnome.org/gssdp/stable/)
为 SSDP 资源发现和公告处理设计的 GObject API
[Previous Versions](https://developer.gnome.org/gssdp/)
[GUPnP IGD Reference Manual](https://developer.gnome.org/gupnp-igd/unstable/)
用于处理 UPnP 互联网网关设备端口映射的库。
[Previous Versions](https://developer.gnome.org/gupnp-igd/)
[GUPnP Reference Manual](https://developer.gnome.org/gupnp/stable/)
基于 gssdp 构建的 UPnP 核心 API
[Previous Versions](https://developer.gnome.org/gupnp/)
[libsoup Reference Manual](https://developer.gnome.org/libsoup/stable/)
libsoup 是一个 GNOME 的 HTTP 客户端/服务器库。它使用 GObjects 和 glib 主循环，以和 GNOME 程序很好的集成。
[Previous Versions](https://developer.gnome.org/libsoup/)
[telepathy-glib 参考手册](http://telepathy.freedesktop.org/doc/telepathy-glib/) [外部资源]
[Zapojit Reference Manual](https://developer.gnome.org/libzapojit/unstable/)
libzapojit 是一个 SkyDrive 和 Hotmail REST API 的 GLib/GObject 封装器。
[Previous Versions](https://developer.gnome.org/libzapojit/)
### 辅助功能
[ATK - Accessibility Toolkit](https://developer.gnome.org/atk/stable/)
ATK 提供了一套由其他工具集和应用程序实现的辅助功能接口。使用 ATK 接口，辅助工具能够完全地查看和控制运行中的应用程序。
[Previous Versions](https://developer.gnome.org/atk/)
### 数据存储
[GNU Data Access Reference Manual](https://developer.gnome.org/libgda/stable/)
[Previous Versions](https://developer.gnome.org/libgda/)
[libglom 参考手册](https://developer.gnome.org/libglom/stable/)
[Previous Versions](https://developer.gnome.org/libglom/)
[libgom Reference Manual](https://developer.gnome.org/gom/unstable/)
[Previous Versions](https://developer.gnome.org/gom/)
### 实用工具
[libxml2 参考手册](http://xmlsoft.org/html/libxml-lib.html) [外部资源]
强大而功能完善的 XML 处理库。
[libxslt 参考手册](http://xmlsoft.org/XSLT/html/libxslt-lib.html) [外部资源]
为 GNOME 项目开发的 XSLT C 库。XSLT 本身是定义 XML 转换的一种 XML 语言。Libxslt 基于 libxml2。
[dconf Reference Manual](https://developer.gnome.org/dconf/unstable/)
dconf 是一个低层配置系统。它的主要目的是在没有配置存储系统的平台上为 GSettings 提供后端。
[Previous Versions](https://developer.gnome.org/dconf/)
[GSF Reference Manual](https://developer.gnome.org/gsf/stable/)
结构化文件库(Structured File Library, GSF)是用于读写复合文件的 I/O 抽象。
[Previous Versions](https://developer.gnome.org/gsf/)
[GXPS Reference Manual](https://developer.gnome.org/libgxps/unstable/)
libgxps 是一个基于 GObject 的，用来操纵和渲染 XPS 文档的函数库。
[Previous Versions](https://developer.gnome.org/libgxps/)
[JSON-GLib Reference Manual](https://developer.gnome.org/json-glib/stable/)
JSON-GLib 是一个用于读取和解析 JSON 的库，使用了 GLb 和 GObject 数据类型和 API。
[Previous Versions](https://developer.gnome.org/json-glib/)
[libchamplain Reference Manual](https://developer.gnome.org/libchamplain/unstable/)
Libchamplain 是一个提供 ClutterActor 以显示地图的 C 函数库。
[Previous Versions](https://developer.gnome.org/libchamplain/)
[libchamplain-gtk Reference Manual](https://developer.gnome.org/libchamplain-gtk/unstable/)
Libchamplain-gtk 是一个构建在 libchamplain 上的 C 函数库，它提供一个 Gtk+ 构件以在 Gtk+ 程序中显示地图。
[Previous Versions](https://developer.gnome.org/libchamplain-gtk/)
[Libnotify Reference Manual](https://developer.gnome.org/libnotify/unstable/)
libnotify 是一个按照桌面通知规范定义的方法，向通知守护进程发送桌面通知的函数库。这些通知可以用于通知用户一个事件或者显示某些形式的信息，而不用妨碍用户的操作。
[Previous Versions](https://developer.gnome.org/libnotify/)
[Librsvg Reference Manual](https://developer.gnome.org/rsvg/stable/)
用以渲染 SVG 矢量图形的库。
[Previous Versions](https://developer.gnome.org/rsvg/)
[Libsecret Library Reference Manual](https://developer.gnome.org/libsecret/unstable/)
libsecret 是一个用于存储和获取密钥和其他私密信息的库。它使用 DBUS 与“私密服务”进行通讯。
[Previous Versions](https://developer.gnome.org/libsecret/)
[Peas Reference Manual](https://developer.gnome.org/libpeas/stable/)
libpeas 是一个基于 gobject 的插件引擎，目标是让所有的应用程序都有机会使用扩展。
[Previous Versions](https://developer.gnome.org/libpeas/)
[VTE Reference Manual](https://developer.gnome.org/vte/unstable/)
GNOME 终端使用的终端模拟器构件。
[Previous Versions](https://developer.gnome.org/vte/)
### 其他
[Cogl Reference Manual](https://developer.gnome.org/cogl/stable/)
COGL 是为 Clutter 开发的低层 OpenGL 抽象库(也是 Clutter 的一部分)。它主要由 Clutter 使用，以提供一个通用渲染 API，其能跨越 OpenGL >=1.4、OpenGL ES 1.1 和 OpenGL ES 2.0 透明的工作。
[Previous Versions](https://developer.gnome.org/cogl/)
### 其他
## 其他库
### 交流
[Easy Publish and Consume Reference Manual](https://developer.gnome.org/libepc/unstable/)
轻松发布和消费函数库(libepc)提供了一种使用 HTTPS 发布数据、通过DNS-SD 公布信息、查找并最终使用这些信息的简便方法。
[Previous Versions](https://developer.gnome.org/libepc/)
[GData Reference Manual](https://developer.gnome.org/gdata/unstable/)
[Previous Versions](https://developer.gnome.org/gdata/)
### 多媒体
[discident-glib Reference Manual](https://developer.gnome.org/discident-glib/unstable/)
基于 GObject 的，用于 Discident 和 EAN 查找服务的 API。
[Previous Versions](https://developer.gnome.org/discident-glib/)
[Grilo Reference Manual](https://developer.gnome.org/grilo/unstable/)
Grilo 是一个采用插件系统设计提供对多种多媒体内容来源访问的框架。
[Previous Versions](https://developer.gnome.org/grilo/)
### 实用工具
[Amtk 5 Reference Manual](https://developer.gnome.org/amtk/stable/)
Actions, Menus and Toolbars Kit for GTK+ applications
[Previous Versions](https://developer.gnome.org/amtk/)
[GMime 3.0 Reference Manual](https://developer.gnome.org/gmime/stable/)
GMime 是一个强大的 MIME(多用途互联网邮件扩展) 工具库。其用途是创建、编辑和解析 MIME 消息和结构。
[Previous Versions](https://developer.gnome.org/gmime/)
[gspell 1 Reference Manual](https://developer.gnome.org/gspell/stable/)
Spell-checking library for GTK+ applications
[Previous Versions](https://developer.gnome.org/gspell/)
[Poppler Reference Manual](https://developer.gnome.org/poppler/unstable/)
[Previous Versions](https://developer.gnome.org/poppler/)
[Sensor Proxy Reference Manual](https://developer.gnome.org/iio-sensor-proxy/stable/)
iio-sensor-proxy 是一个为加速度计和环境光传感器的 D-Bus 代理程序。
[Previous Versions](https://developer.gnome.org/iio-sensor-proxy/)
[Switcheroo Control Reference Manual](https://developer.gnome.org/switcheroo-control/stable/)
switcheroo-control is a D-Bus proxy for dual-GPU desktop integration.
[Previous Versions](https://developer.gnome.org/switcheroo-control/)
[Tepl 4 Reference Manual](https://developer.gnome.org/tepl/stable/)
Text editor product line
[Previous Versions](https://developer.gnome.org/tepl/)
### 内省
[libgirepository API Reference](https://developer.gnome.org/gi/stable/)
[Previous Versions](https://developer.gnome.org/gi/)
### 其他
[Cally Reference Manual](https://developer.gnome.org/cally/stable/)
[Previous Versions](https://developer.gnome.org/cally/)
[Camel Reference Manual](https://developer.gnome.org/camel/stable/)
[Previous Versions](https://developer.gnome.org/camel/)
[Cheese Reference Manual](https://developer.gnome.org/cheese/stable/)
[Previous Versions](https://developer.gnome.org/cheese/)
[Clutter Gst 3.0.4 Reference Manual](https://developer.gnome.org/clutter-gst/stable/)
[Previous Versions](https://developer.gnome.org/clutter-gst/)
[Clutter-Gtk 1.8.0 Reference Manual](https://developer.gnome.org/clutter-gtk/stable/)
[Previous Versions](https://developer.gnome.org/clutter-gtk/)
[Cogl 2.0 Reference Manual](https://developer.gnome.org/cogl-2.0-experimental/stable/)
[Previous Versions](https://developer.gnome.org/cogl-2.0-experimental/)
[Cogl GST 2.0 Reference Manual](https://developer.gnome.org/cogl-gst/stable/)
[Previous Versions](https://developer.gnome.org/cogl-gst/)
[Data Model API (libmodel)](https://developer.gnome.org/model/unstable/)
[Previous Versions](https://developer.gnome.org/model/)
[Devhelp Reference Manual](https://developer.gnome.org/devhelp/stable/)
[Previous Versions](https://developer.gnome.org/devhelp/)
[Evince Document Library Reference Manual](https://developer.gnome.org/libevdocument/stable/)
[Previous Versions](https://developer.gnome.org/libevdocument/)
[Evince View Library Reference Manual](https://developer.gnome.org/libevview/stable/)
[Previous Versions](https://developer.gnome.org/libevview/)
[Evolution Connector for Microsoft Exchange Programmer’s Reference Manual](https://developer.gnome.org/evolution-exchange/stable/)
[Previous Versions](https://developer.gnome.org/evolution-exchange/)
[Evolution Mail Composer Reference Manual](https://developer.gnome.org/evolution-mail-composer/stable/)
[Previous Versions](https://developer.gnome.org/evolution-mail-composer/)
[Evolution Mail Engine Reference Manual](https://developer.gnome.org/evolution-mail-engine/stable/)
[Previous Versions](https://developer.gnome.org/evolution-mail-engine/)
[Evolution Mail Formatter Reference Manual](https://developer.gnome.org/evolution-mail-formatter/stable/)
[Previous Versions](https://developer.gnome.org/evolution-mail-formatter/)
[Evolution Shell Reference Manual](https://developer.gnome.org/eshell/stable/)
[Previous Versions](https://developer.gnome.org/eshell/)
[Evolution Shell Reference Manual](https://developer.gnome.org/libeshell/stable/)
[Previous Versions](https://developer.gnome.org/libeshell/)
[Evolution Shell Reference Manual](https://developer.gnome.org/evolution-shell/stable/)
[Previous Versions](https://developer.gnome.org/evolution-shell/)
[Evolution Utilities Reference Manual](https://developer.gnome.org/libeutil/stable/)
[Previous Versions](https://developer.gnome.org/libeutil/)
[Evolution Utilities Reference Manual](https://developer.gnome.org/evolution-util/stable/)
[Previous Versions](https://developer.gnome.org/evolution-util/)
[Evolution-Data-Server Manual: Address Book Backend (libedata-book)](https://developer.gnome.org/libedata-book/stable/)
[Previous Versions](https://developer.gnome.org/libedata-book/)
[Evolution-Data-Server Manual: Address Book Client (libebook)](https://developer.gnome.org/libebook/stable/)
[Previous Versions](https://developer.gnome.org/libebook/)
[Evolution-Data-Server Manual: Address Book Data (libebook-contacts)](https://developer.gnome.org/libebook-contacts/stable/)
[Previous Versions](https://developer.gnome.org/libebook-contacts/)
[Evolution-Data-Server Manual: Backend Utilities (libebackend)](https://developer.gnome.org/libebackend/stable/)
[Previous Versions](https://developer.gnome.org/libebackend/)
[Evolution-Data-Server Manual: Calendar Backend (libedata-cal)](https://developer.gnome.org/libedata-cal/stable/)
[Previous Versions](https://developer.gnome.org/libedata-cal/)
[Evolution-Data-Server Manual: Calendar Client (libecal)](https://developer.gnome.org/libecal/stable/)
[Previous Versions](https://developer.gnome.org/libecal/)
[Evolution-Data-Server Manual: Graphical Utilities (libedataserverui)](https://developer.gnome.org/libedataserverui/stable/)
[Previous Versions](https://developer.gnome.org/libedataserverui/)
[Evolution-Data-Server Manual: Private](https://developer.gnome.org/libedbus-private/stable/)
[Previous Versions](https://developer.gnome.org/libedbus-private/)
[Evolution-Data-Server Manual: Utilities (libedataserver)](https://developer.gnome.org/libedataserver/stable/)
[Previous Versions](https://developer.gnome.org/libedataserver/)
[Evolution-Data-Server Reference Manual](https://developer.gnome.org/eds/stable/)
[Previous Versions](https://developer.gnome.org/eds/)
[GAIL Reference Manual](https://developer.gnome.org/gail-libgail-util/stable/)
[Previous Versions](https://developer.gnome.org/gail-libgail-util/)
[GCab Reference Manual](https://developer.gnome.org/gcab/unstable/)
[Previous Versions](https://developer.gnome.org/gcab/)
[Gck Library Reference Manual](https://developer.gnome.org/gck/stable/)
[Previous Versions](https://developer.gnome.org/gck/)
[Gcr Library Reference Manual](https://developer.gnome.org/gcr/stable/)
[Previous Versions](https://developer.gnome.org/gcr/)
[GdaBrowser hacking manual](https://developer.gnome.org/gda-browser/stable/)
[Previous Versions](https://developer.gnome.org/gda-browser/)
[gdbus-object-manager-example](https://developer.gnome.org/gdbus-object-manager-example/2.44/)
[Previous Versions](https://developer.gnome.org/gdbus-object-manager-example/)
[Geoclue Reference Manual](https://developer.gnome.org/geoclue/unstable/)
[Previous Versions](https://developer.gnome.org/geoclue/)
[geocode-glib Reference Manual](https://developer.gnome.org/geocode-glib/stable/)
[Previous Versions](https://developer.gnome.org/geocode-glib/)
[GFBGraph 0.2 Reference Manual](https://developer.gnome.org/gfbgraph/unstable/)
[Previous Versions](https://developer.gnome.org/gfbgraph/)
[Glade User Interface Designer Reference Manual](https://developer.gnome.org/gladeui/stable/)
[Previous Versions](https://developer.gnome.org/gladeui/)
[GNOME Desktop Library Reference Manual](https://developer.gnome.org/gnome-desktop/stable/)
[Previous Versions](https://developer.gnome.org/gnome-desktop/)
[GNOME Desktop Library Reference Manual](https://developer.gnome.org/gnome-desktop3/stable/)
[Previous Versions](https://developer.gnome.org/gnome-desktop3/)
[GNOME Dictionary Library Reference Manual](https://developer.gnome.org/gdict/stable/)
[Previous Versions](https://developer.gnome.org/gdict/)
[GNOME Docking Library Reference Manual](https://developer.gnome.org/gdl/stable/)
GNOME Devtools 库包提供了一个对接系统和多个工具，一般对 GNOME 开发工具和 GNOME 程序有用。
[Previous Versions](https://developer.gnome.org/gdl/)
[GNOME Online Accounts Reference Manual](https://developer.gnome.org/goa/stable/)
[Previous Versions](https://developer.gnome.org/goa/)
[Gnome Scan Reference Manual](https://developer.gnome.org/gnome-scan/unstable/)
[Previous Versions](https://developer.gnome.org/gnome-scan/)
[GNOME Software Reference Manual](https://developer.gnome.org/gnome-software/stable/)
[Previous Versions](https://developer.gnome.org/gnome-software/)
[GNOME To Do - Reference Manual](https://developer.gnome.org/gnome-todo/stable/)
[Previous Versions](https://developer.gnome.org/gnome-todo/)
[gnome-autoar Reference Manual](https://developer.gnome.org/gnome-autoar/unstable/)
[Previous Versions](https://developer.gnome.org/gnome-autoar/)
[gnome-bluetooth Reference Manual](https://developer.gnome.org/gnome-bluetooth/stable/)
[Previous Versions](https://developer.gnome.org/gnome-bluetooth/)
[gnome-builder Reference Manual](https://developer.gnome.org/libide/stable/)
[Previous Versions](https://developer.gnome.org/libide/)
[gnome-keyring Reference Manual](https://developer.gnome.org/gnome-keyring/stable/)
[Previous Versions](https://developer.gnome.org/gnome-keyring/)
[GOffice Reference Manual](https://developer.gnome.org/goffice/unstable/)
[Previous Versions](https://developer.gnome.org/goffice/)
[GooCanvas Reference Manual](https://developer.gnome.org/goocanvas/stable/)
[Previous Versions](https://developer.gnome.org/goocanvas/)
[GooCanvas Reference Manual](https://developer.gnome.org/goocanvas2/stable/)
[Previous Versions](https://developer.gnome.org/goocanvas2/)
[GP11 Reference Manual](https://developer.gnome.org/gp11/stable/)
[Previous Versions](https://developer.gnome.org/gp11/)
[Graphene Reference Manual](https://developer.gnome.org/graphene/stable/)
[Previous Versions](https://developer.gnome.org/graphene/)
[GSound Reference Manual](https://developer.gnome.org/gsound/stable/)
[Previous Versions](https://developer.gnome.org/gsound/)
[GtkGLExt Reference Manual](https://developer.gnome.org/gtkglext/stable/)
GtkGLExt 是 GTK+ 的 OpenGL 扩展。它提供了支持在 GTK+ 中进行 OpenGL 渲染的附加 GDK 对象和使 GTK+ 组件支持 OpenGL 的 GtkWidget API 扩展。
[Previous Versions](https://developer.gnome.org/gtkglext/)
[GtkSourceView 4 Reference Manual](https://developer.gnome.org/gtksourceview/stable/)
[Previous Versions](https://developer.gnome.org/gtksourceview/)
[gucharmap Reference Manual](https://developer.gnome.org/gucharmap/stable/)
[Previous Versions](https://developer.gnome.org/gucharmap/)
[GUPnP DLNA Gstreamer Reference Manual](https://developer.gnome.org/gupnp-dlna-gst/unstable/)
[Previous Versions](https://developer.gnome.org/gupnp-dlna-gst/)
[GUPnP DLNA Reference Manual](https://developer.gnome.org/gupnp-dlna-metadata/unstable/)
[Previous Versions](https://developer.gnome.org/gupnp-dlna-metadata/)
[libatspi Reference Manual](https://developer.gnome.org/libatspi/stable/)
[Previous Versions](https://developer.gnome.org/libatspi/)
[Libbrasero-burn Reference Manual](https://developer.gnome.org/libbrasero-burn/stable/)
[Previous Versions](https://developer.gnome.org/libbrasero-burn/)
[Libbrasero-media Reference Manual](https://developer.gnome.org/libbrasero-media/stable/)
[Previous Versions](https://developer.gnome.org/libbrasero-media/)
[Libcroco Reference Manual](https://developer.gnome.org/libcroco/unstable/)
[Previous Versions](https://developer.gnome.org/libcroco/)
[libcryptui Reference Manual](https://developer.gnome.org/libcryptui/stable/)
[Previous Versions](https://developer.gnome.org/libcryptui/)
[libgimp](https://developer.gnome.org/libgimp/stable/)
[Previous Versions](https://developer.gnome.org/libgimp/)
[libgit2-glib Reference Manual](https://developer.gnome.org/libgit2-glib/unstable/)
libgit2-glib 是一个 glib 库。它封装了 libgit2，使得在面向 GObject 的应用程序中开发管理 git 仓库更加容易。
[Previous Versions](https://developer.gnome.org/libgit2-glib/)
[LibGlabels 3.0 Reference Manual](https://developer.gnome.org/libglabels/stable/)
[Previous Versions](https://developer.gnome.org/libglabels/)
[LibGlbarcode 3.0 Reference Manual](https://developer.gnome.org/libglbarcode/stable/)
[Previous Versions](https://developer.gnome.org/libglbarcode/)
[libgrss Reference Manual](https://developer.gnome.org/libgrss/unstable/)
[Previous Versions](https://developer.gnome.org/libgrss/)
[Libgtop Reference Manual](https://developer.gnome.org/libgtop/stable/)
[Previous Versions](https://developer.gnome.org/libgtop/)
[libgweather Reference Manual](https://developer.gnome.org/libgweather/stable/)
[Previous Versions](https://developer.gnome.org/libgweather/)
[libmediaart Library Reference Manual](https://developer.gnome.org/libmediaart/unstable/)
[Previous Versions](https://developer.gnome.org/libmediaart/)
[libmm-glib Reference Manual](https://developer.gnome.org/libmm-glib/unstable/)
[Previous Versions](https://developer.gnome.org/libmm-glib/)
[libnm Reference Manual](https://developer.gnome.org/libnm/stable/)
[Previous Versions](https://developer.gnome.org/libnm/)
[libnm-glib Reference Manual](https://developer.gnome.org/libnm-glib/stable/)
[Previous Versions](https://developer.gnome.org/libnm-glib/)
[libnm-util Reference Manual](https://developer.gnome.org/libnm-util/stable/)
[Previous Versions](https://developer.gnome.org/libnm-util/)
[libnma Reference Manual](https://developer.gnome.org/libnma/stable/)
[Previous Versions](https://developer.gnome.org/libnma/)
[Liboobs (System configuration management) Reference Manual](https://developer.gnome.org/liboobs/stable/)
[Previous Versions](https://developer.gnome.org/liboobs/)
[libqmi-glib Reference Manual](https://developer.gnome.org/libqmi-glib/stable/)
[Previous Versions](https://developer.gnome.org/libqmi-glib/)
[librest Reference Manual](https://developer.gnome.org/rest/unstable/)
[Previous Versions](https://developer.gnome.org/rest/)
[libseahorse Reference Manual](https://developer.gnome.org/libseahorse/stable/)
[Previous Versions](https://developer.gnome.org/libseahorse/)
[Libwnck Reference Manual](https://developer.gnome.org/libwnck/stable/)
[Previous Versions](https://developer.gnome.org/libwnck/)
[Libxklavier Reference Manual](https://developer.gnome.org/libxklavier/stable/)
[Previous Versions](https://developer.gnome.org/libxklavier/)
[ModemManager Reference Manual](https://developer.gnome.org/ModemManager/unstable/)
[Previous Versions](https://developer.gnome.org/ModemManager/)
[Mutter Reference Manual](https://developer.gnome.org/meta/stable/)
[Previous Versions](https://developer.gnome.org/meta/)
[NetworkManager Reference Manual](https://developer.gnome.org/NetworkManager/stable/)
[Previous Versions](https://developer.gnome.org/NetworkManager/)
[OSTree Manual](https://developer.gnome.org/ostree/stable/)
[Previous Versions](https://developer.gnome.org/ostree/)
[Panel Applet Reference Manual](https://developer.gnome.org/libpanel-applet/stable/)
[Previous Versions](https://developer.gnome.org/libpanel-applet/)
[phodav Reference Manual](https://developer.gnome.org/phodav/stable/)
[Previous Versions](https://developer.gnome.org/phodav/)
[Seed Reference Manual](https://developer.gnome.org/seed/stable/)
[Previous Versions](https://developer.gnome.org/seed/)
[Shell Reference Manual](https://developer.gnome.org/shell/stable/)
[Previous Versions](https://developer.gnome.org/shell/)
[St Reference Manual](https://developer.gnome.org/st/stable/)
[Previous Versions](https://developer.gnome.org/st/)
[Totem Playlist Parser Reference Manual](https://developer.gnome.org/totem-pl-parser/stable/)
[Previous Versions](https://developer.gnome.org/totem-pl-parser/)
[Totem Reference Manual](https://developer.gnome.org/totem/stable/)
[Previous Versions](https://developer.gnome.org/totem/)
[Tracker Client Library Reference Manual](https://developer.gnome.org/libtracker-client/unstable/)
[Previous Versions](https://developer.gnome.org/libtracker-client/)
[Tracker Control Library Reference Manual](https://developer.gnome.org/libtracker-control/stable/)
[Previous Versions](https://developer.gnome.org/libtracker-control/)
[Tracker Extract Library Reference Manual](https://developer.gnome.org/libtracker-extract/0.16/)
[Previous Versions](https://developer.gnome.org/libtracker-extract/)
[Tracker Miner Library Reference Manual](https://developer.gnome.org/libtracker-miner/stable/)
[Previous Versions](https://developer.gnome.org/libtracker-miner/)
[Tracker Ontology Reference Manual](https://developer.gnome.org/ontology/stable/)
[Previous Versions](https://developer.gnome.org/ontology/)
[Tracker SPARQL Library Reference Manual](https://developer.gnome.org/libtracker-sparql/stable/)
[Previous Versions](https://developer.gnome.org/libtracker-sparql/)
[Yelp Reference Manual](https://developer.gnome.org/libyelp/stable/)
[Previous Versions](https://developer.gnome.org/libyelp/)
## 语言绑定
### C++
[gtksourceviewmm 文档](https://developer.gnome.org/gtksourceviewmm/stable/)
[Previous Versions](https://developer.gnome.org/gtksourceviewmm/)
[libsigc++ 文档](https://developer.gnome.org/libsigc++/stable/)
[Previous Versions](https://developer.gnome.org/libsigc++/)
[libxml++ 文档](https://developer.gnome.org/libxml++/stable/)
[Previous Versions](https://developer.gnome.org/libxml++/)
[glibmm 文档](https://developer.gnome.org/glibmm/stable/)
[Previous Versions](https://developer.gnome.org/glibmm/)
[gtkmm 文档](https://developer.gnome.org/gtkmm/3.22/)
[Previous Versions](https://developer.gnome.org/gtkmm/)
[atkmm 文档](https://developer.gnome.org/atkmm/stable/)
[Previous Versions](https://developer.gnome.org/atkmm/)
[gconfmm 文档](https://developer.gnome.org/gconfmm/stable/)
[Previous Versions](https://developer.gnome.org/gconfmm/)
[pangomm 文档](https://developer.gnome.org/pangomm/stable/)
[Previous Versions](https://developer.gnome.org/pangomm/)
[libvtemm 文档](https://developer.gnome.org/libvtemm/unstable/)
[Previous Versions](https://developer.gnome.org/libvtemm/)
[librsvgmm 文档](https://developer.gnome.org/librsvgmm/stable/)
[Previous Versions](https://developer.gnome.org/librsvgmm/)
[goocanvasmm 文档](https://developer.gnome.org/goocanvasmm/stable/)
[Previous Versions](https://developer.gnome.org/goocanvasmm/)
[gstreamermm 文档](https://developer.gnome.org/gstreamermm/stable/)
[Previous Versions](https://developer.gnome.org/gstreamermm/)
[gtkglextmm 文档](https://developer.gnome.org/gtkglextmm/stable/)
[Previous Versions](https://developer.gnome.org/gtkglextmm/)
[libgda-uimm 文档](https://developer.gnome.org/libgda-uimm/unstable/)
[Previous Versions](https://developer.gnome.org/libgda-uimm/)
[libgdamm 文档](https://developer.gnome.org/libgdamm/unstable/)
[Previous Versions](https://developer.gnome.org/libgdamm/)
[cluttermm 文档](https://developer.gnome.org/cluttermm/unstable/)
[Previous Versions](https://developer.gnome.org/cluttermm/)
[clutter-box2dmm 文档](https://developer.gnome.org/clutter-box2dmm/unstable/)
[Previous Versions](https://developer.gnome.org/clutter-box2dmm/)
[clutter-gtkmm 文档](https://developer.gnome.org/clutter-gtkmm/stable/)
[Previous Versions](https://developer.gnome.org/clutter-gtkmm/)
### Vala
[Vala API 参考](http://references.valadoc.org/) [外部资源]
GTK+，GNOME 和相关库的 Vala API 参考
### Java
[java-gnome API 文档](http://java-gnome.sourceforge.net/4.0/doc/api/overview-summary.html) [外部资源]
GTK+、GNOME 和相关库的 Java 接口
### Python
[PyGObject API Reference](http://lazka.github.io/pgi-docs/) [外部资源]
Python API Reference for GTK+, GNOME and related libraries
[PyGObject Documentation](https://pygobject.readthedocs.io/) [外部资源]
Documentation for the PyGObject Python bindings
### JavaScript
[JavaScript API browser](https://devdocs.baznga.org/) [外部资源]
## GNOME 应用程序插件
[Anjuta Developers Reference Manual](https://developer.gnome.org/libanjuta/stable/)
[Previous Versions](https://developer.gnome.org/libanjuta/)
[Epiphany Reference Manual](https://developer.gnome.org/epiphany/stable/)
[Previous Versions](https://developer.gnome.org/epiphany/)
[Evince Programmer's Reference Manual](https://developer.gnome.org/evince/stable/)
[Previous Versions](https://developer.gnome.org/evince/)
[Eye of GNOME Reference Manual](https://developer.gnome.org/eog/stable/)
[Previous Versions](https://developer.gnome.org/eog/)
[gedit Reference Manual](https://developer.gnome.org/gedit/stable/)
[Previous Versions](https://developer.gnome.org/gedit/)
[Nautilus Extension Reference Manual](https://developer.gnome.org/libnautilus-extension/stable/)
[Previous Versions](https://developer.gnome.org/libnautilus-extension/)
[Rhythmbox Development Reference Manual](https://developer.gnome.org/rhythmbox/stable/)
[Previous Versions](https://developer.gnome.org/rhythmbox/)
## 标准
[XDG 基础目录规范](https://developer.gnome.org/basedir-spec/)
多个规范指定了文件和文件格式。此 freedesktop.org 规范通过定义相对要定位文件的一个或多个基目录来指出在何处寻找这些文件。
[共享 MIME-info 数据库规范](https://developer.gnome.org/shared-mime-info-spec/)
freedesktop.org 规范尝试统一 X 桌面环境现在使用的 MIME 数据库系统。
[图标主题规范](https://developer.gnome.org/icon-theme-spec/)
freedesktop.org 规范描述了存储图标主题的一个通用方法。
[图标命名规范](https://developer.gnome.org/icon-naming-spec/)
freedesktop.org 规范描述了在一个图标主题中定义图标名及其内容的通用方法。
[扩展窗口管理器提示](https://developer.gnome.org/wm-spec/)
此 freedesktop.org 规范标准化了 X 桌面之间的 ICCCM 扩展。
[桌面应用程序自启动规范](https://developer.gnome.org/autostart-spec/)
此 freedesktop.org 规范描述了用户登录之后应用程序怎样自动启动以及媒体挂载之后它怎样请求执行一个特定应用程序或打开媒体上的一个指定文件。
[桌面通知规范](https://developer.gnome.org/notification-spec/)
此规范标准化了桌面通知服务的接口。
[桌面项规范](https://developer.gnome.org/desktop-entry-spec/)
桌面项提供了有关程序的信息，如名称、图标和描述等。这些文件用于程序启动器和创建可启动程序菜单。
[菜单规范](https://developer.gnome.org/menu-spec/)
freedesktop.org 规范描述了桌面项怎样构成菜单。
