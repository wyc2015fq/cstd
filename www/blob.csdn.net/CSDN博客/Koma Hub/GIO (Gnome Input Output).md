# GIO (Gnome Input/Output) - Koma Hub - CSDN博客
2019年03月02日 17:52:23[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：69
个人分类：[GNOME																[GTK](https://blog.csdn.net/Rong_Toa/article/category/7156203)](https://blog.csdn.net/Rong_Toa/article/category/8611720)
**Table of Contents**
[维基百科](#%E7%BB%B4%E5%9F%BA%E7%99%BE%E7%A7%91)
[Features](#Features)
[See also](#See%20also)
[KIO – an analogous KDE library](#KIO%C2%A0%E2%80%93%20an%20analogous%20KDE%20library)
[References](#References)
[External links](#External%20links)
[百度百科](#%E7%99%BE%E5%BA%A6%E7%99%BE%E7%A7%91)
[GNOME DEVELOPER](#GNOME%C2%A0DEVELOPER)
# 维基百科
**GIO** (**Gnome Input/Output**) is a [library](https://en.wikipedia.org/wiki/Library_(computing)), designed to present programmers with a modern and usable interface to a [virtual file system](https://en.wikipedia.org/wiki/Virtual_file_system). It allows applications to access local and remote files with a single consistent [API](https://en.wikipedia.org/wiki/Application_programming_interface), which was designed "to overcome the shortcomings of [GnomeVFS](https://en.wikipedia.org/wiki/GnomeVFS)" and be "so good that developers prefer it over raw [POSIX](https://en.wikipedia.org/wiki/POSIX) calls."[[2]](https://en.wikipedia.org/wiki/GIO_(software)#cite_note-2)
GIO serves as low-level system library for the [GNOME Shell](https://en.wikipedia.org/wiki/GNOME_Shell)/[GNOME](https://en.wikipedia.org/wiki/GNOME)/[GTK+](https://en.wikipedia.org/wiki/GTK%2B) software stack and is being developed by [The GNOME Project](https://en.wikipedia.org/wiki/The_GNOME_Project). It is maintained as a separate library, libgio-2.0, but it is bundled with [GLib](https://en.wikipedia.org/wiki/GLib). GIO is [free and open-source software](https://en.wikipedia.org/wiki/Free_and_open-source_software) subject to the requirements of the [GNU Lesser General Public License](https://en.wikipedia.org/wiki/GNU_Lesser_General_Public_License).
## Features
- The abstract file system model of GIO consists of a number of interfaces and base classes for I/O and files.
- There are a number of stream classes, similar to the input and output stream hierarchies that can be found in frameworks like Java.
- There are interfaces related to applications and the types of files they handle.
- There is a framework for storing and retrieving application settings.
- file type detection with xdgmime (xdg = X Desktop Group = [freedesktop.org](https://en.wikipedia.org/wiki/Freedesktop.org))[[3]](https://en.wikipedia.org/wiki/GIO_(software)#cite_note-3)
- file monitoring with [inotify](https://en.wikipedia.org/wiki/Inotify)[[4]](https://en.wikipedia.org/wiki/GIO_(software)#cite_note-4)
- file monitoring with [FAM](https://en.wikipedia.org/wiki/File_Alteration_Monitor)[[5]](https://en.wikipedia.org/wiki/GIO_(software)#cite_note-5)
- There is support for network programming, including name resolution, lowlevel socket APIs and highlevel client and server helper classes.
- There is support for connecting to [D-Bus](https://en.wikipedia.org/wiki/D-Bus), sending and receiving messages, owning and watching bus names, and making objects available on the bus.
Beyond these, GIO provides facilities for file monitoring, [asynchronous I/O](https://en.wikipedia.org/wiki/Asynchronous_I/O) and filename completion. In addition to the interfaces, GIO provides implementations for the local case. Implementations for various network file systems are provided by the [GVfs](https://en.wikipedia.org/wiki/GVfs) package as loadable modules.
## See also
## [KIO](https://en.wikipedia.org/wiki/KIO) – an analogous KDE library
## References
- **[^](https://en.wikipedia.org/wiki/GIO_(software)#cite_ref-1)** Withnall, Philip (21 January 2019). ["glib 2.58.3"](https://mail.gnome.org/archives/ftp-release-list/2019-January/msg00075.html). *GNOME ftp-release* (Mailing list). Retrieved 6 February 2019.
- **[^](https://en.wikipedia.org/wiki/GIO_(software)#cite_ref-2)**["GIO Reference Manual"](http://developer.gnome.org/gio/stable/).
- **[^](https://en.wikipedia.org/wiki/GIO_(software)#cite_ref-3)**["xdgmime in GIO git"](https://git.gnome.org/browse/glib/tree/gio/xdgmime).
- **[^](https://en.wikipedia.org/wiki/GIO_(software)#cite_ref-4)**["inotify in GIO git"](https://git.gnome.org/browse/glib/tree/gio/inotify).
- **[^](https://en.wikipedia.org/wiki/GIO_(software)#cite_ref-5)**["FAM in GIO git"](https://git.gnome.org/browse/glib/tree/gio/fam).
## External links
- [GIO Reference Manual](http://developer.gnome.org/gio/stable/)
- [GIO Technical Details](https://developer.gnome.org/gio/stable/ch01.html#gvfs-overview)
# 百度百科
GIO是一种致力于提供现代的、易用的VFS（[虚拟文件系统](https://baike.baidu.com/item/%E8%99%9A%E6%8B%9F%E6%96%87%E4%BB%B6%E7%B3%BB%E7%BB%9F)）API，其目标是克服GnomeVFS的缺陷和为开发者提供一种与原生态POSIX调用一样好的API。但GIO并不是POSIX API的复制，而是提供了一个高层的、以文档为中心的接口集。
GIO是GVFS的I/O库，计划用来取代Gnome-vfs。它附带glib作为一个独立的库，称为“libgio-2.0”。libgio包括文件i/o、文件类型等的抽象，同时也默认实现本地文件i/o。
# GNOME DEVELOPER
GIO Reference Manual
for GIO 2.58.3 The latest version of this documentation can be found on-line at [https://developer.gnome.org/gio/unstable/](https://developer.gnome.org/gio/unstable/).
[I. GIO Overview](https://developer.gnome.org/gio/stable/pt01.html)
[Introduction](https://developer.gnome.org/gio/stable/ch01.html)
[Writing GIO applications](https://developer.gnome.org/gio/stable/ch02.html)
[Compiling GIO applications](https://developer.gnome.org/gio/stable/ch03.html)
[Running GIO applications](https://developer.gnome.org/gio/stable/running-gio-apps.html)
[Extending GIO](https://developer.gnome.org/gio/stable/extending-gio.html)
[II. API Reference](https://developer.gnome.org/gio/stable/pt02.html)
[File Operations](https://developer.gnome.org/gio/stable/file_ops.html)
[GFile](https://developer.gnome.org/gio/stable/GFile.html) — File and Directory Handling
[GFileAttribute](https://developer.gnome.org/gio/stable/gio-GFileAttribute.html) — Key-Value Paired File Attributes
[GFileInfo](https://developer.gnome.org/gio/stable/GFileInfo.html) — File Information and Attributes
[GFileEnumerator](https://developer.gnome.org/gio/stable/GFileEnumerator.html) — Enumerated Files Routines
[GIOError](https://developer.gnome.org/gio/stable/gio-GIOError.html) — Error helper functions
[GMountOperation](https://developer.gnome.org/gio/stable/GMountOperation.html) — Object used for authentication and user interaction
[File System Monitoring](https://developer.gnome.org/gio/stable/file_mon.html)
[GFileMonitor](https://developer.gnome.org/gio/stable/GFileMonitor.html) — File Monitor
[File-related Utilities](https://developer.gnome.org/gio/stable/utils.html)
[GFilenameCompleter](https://developer.gnome.org/gio/stable/GFilenameCompleter.html) — Filename Completer
[Asynchronous I/O](https://developer.gnome.org/gio/stable/async.html)
[GCancellable](https://developer.gnome.org/gio/stable/GCancellable.html) — Thread-safe Operation Cancellation Stack
[GAsyncResult](https://developer.gnome.org/gio/stable/GAsyncResult.html) — Asynchronous Function Results
[GTask](https://developer.gnome.org/gio/stable/GTask.html) — Cancellable synchronous or asynchronous task and result
[GIOScheduler](https://developer.gnome.org/gio/stable/gio-GIOScheduler.html) — I/O Scheduler
[GSimpleAsyncResult](https://developer.gnome.org/gio/stable/GSimpleAsyncResult.html) — Simple asynchronous results implementation
[Data conversion](https://developer.gnome.org/gio/stable/conversion.html)
[GConverter](https://developer.gnome.org/gio/stable/GConverter.html) — Data conversion interface
[GCharsetConverter](https://developer.gnome.org/gio/stable/GCharsetConverter.html) — Convert between charsets
[GZlibCompressor](https://developer.gnome.org/gio/stable/GZlibCompressor.html) — Zlib compressor
[GZlibDecompressor](https://developer.gnome.org/gio/stable/GZlibDecompressor.html) — Zlib decompressor
[Streaming I/O](https://developer.gnome.org/gio/stable/streaming.html)
[GSeekable](https://developer.gnome.org/gio/stable/GSeekable.html) — Stream seeking interface
[GInputStream](https://developer.gnome.org/gio/stable/GInputStream.html) — Base class for implementing streaming input
[GOutputStream](https://developer.gnome.org/gio/stable/GOutputStream.html) — Base class for implementing streaming output
[GIOStream](https://developer.gnome.org/gio/stable/GIOStream.html) — Base class for implementing read/write streams
[GSimpleIOStream](https://developer.gnome.org/gio/stable/GSimpleIOStream.html) — A wrapper around an input and an output stream.
[GFileInputStream](https://developer.gnome.org/gio/stable/GFileInputStream.html) — File input streaming operations
[GFileOutputStream](https://developer.gnome.org/gio/stable/GFileOutputStream.html) — File output streaming operations
[GFileIOStream](https://developer.gnome.org/gio/stable/GFileIOStream.html) — File read and write streaming operations
[GFileDescriptorBased](https://developer.gnome.org/gio/stable/GFileDescriptorBased.html) — Interface for file descriptor based IO
[GFilterInputStream](https://developer.gnome.org/gio/stable/GFilterInputStream.html) — Filter Input Stream
[GFilterOutputStream](https://developer.gnome.org/gio/stable/GFilterOutputStream.html) — Filter Output Stream
[GMemoryInputStream](https://developer.gnome.org/gio/stable/GMemoryInputStream.html) — Streaming input operations on memory chunks
[GMemoryOutputStream](https://developer.gnome.org/gio/stable/GMemoryOutputStream.html) — Streaming output operations on memory chunks
[GBufferedInputStream](https://developer.gnome.org/gio/stable/GBufferedInputStream.html) — Buffered Input Stream
[GBufferedOutputStream](https://developer.gnome.org/gio/stable/GBufferedOutputStream.html) — Buffered Output Stream
[GDataInputStream](https://developer.gnome.org/gio/stable/GDataInputStream.html) — Data Input Stream
[GDataOutputStream](https://developer.gnome.org/gio/stable/GDataOutputStream.html) — Data Output Stream
[GUnixInputStream](https://developer.gnome.org/gio/stable/GUnixInputStream.html) — Streaming input operations for UNIX file descriptors
[GUnixOutputStream](https://developer.gnome.org/gio/stable/GUnixOutputStream.html) — Streaming output operations for UNIX file descriptors
[GWin32InputStream](https://developer.gnome.org/gio/stable/gio-GWin32InputStream.html) — Streaming input operations for Windows file handles
[GWin32OutputStream](https://developer.gnome.org/gio/stable/gio-GWin32OutputStream.html) — Streaming output operations for Windows file handles
[GConverterInputstream](https://developer.gnome.org/gio/stable/gio-GConverterInputstream.html) — Converter Input Stream
[GConverterOutputstream](https://developer.gnome.org/gio/stable/gio-GConverterOutputstream.html) — Converter Output Stream
[GPollableInputStream](https://developer.gnome.org/gio/stable/GPollableInputStream.html) — Interface for pollable input streams
[GPollableOutputStream](https://developer.gnome.org/gio/stable/GPollableOutputStream.html) — Interface for pollable output streams
[gpollableutils](https://developer.gnome.org/gio/stable/gio-gpollableutils.html) — Utilities for pollable streams
[File types and applications](https://developer.gnome.org/gio/stable/types.html)
[GContentType](https://developer.gnome.org/gio/stable/gio-GContentType.html) — Platform-specific content typing
[GAppInfo](https://developer.gnome.org/gio/stable/GAppInfo.html) — Application information and launch contexts
[GAppInfoMonitor](https://developer.gnome.org/gio/stable/GAppInfoMonitor.html) — Monitor application information for changes
[GDesktopAppInfo](https://developer.gnome.org/gio/stable/gio-Desktop-file-based-GAppInfo.html) — Application information from desktop files
[Volumes and Drives](https://developer.gnome.org/gio/stable/volume_mon.html)
[GVolumeMonitor](https://developer.gnome.org/gio/stable/GVolumeMonitor.html) — Volume Monitor
[GVolume](https://developer.gnome.org/gio/stable/GVolume.html) — Volume management
[GMount](https://developer.gnome.org/gio/stable/GMount.html) — Mount management
[GDrive](https://developer.gnome.org/gio/stable/GDrive.html) — Drive management
[Unix Mounts](https://developer.gnome.org/gio/stable/gio-Unix-Mounts.html) — UNIX mounts
[Icons](https://developer.gnome.org/gio/stable/icons.html)
[GIcon](https://developer.gnome.org/gio/stable/GIcon.html) — Interface for icons
[GFileIcon](https://developer.gnome.org/gio/stable/GFileIcon.html) — Icons pointing to an image file
[GBytesIcon](https://developer.gnome.org/gio/stable/GBytesIcon.html) — An icon stored in memory as a GBytes
[GLoadableIcon](https://developer.gnome.org/gio/stable/GLoadableIcon.html) — Loadable Icons
[GThemedIcon](https://developer.gnome.org/gio/stable/GThemedIcon.html) — Icon theming support
[GEmblemedIcon](https://developer.gnome.org/gio/stable/GEmblemedIcon.html) — Icon with emblems
[GEmblem](https://developer.gnome.org/gio/stable/GEmblem.html) — An object for emblems
[Failable Initialization](https://developer.gnome.org/gio/stable/failable_initialization.html)
[GInitable](https://developer.gnome.org/gio/stable/GInitable.html) — Failable object initialization interface
[GAsyncInitable](https://developer.gnome.org/gio/stable/GAsyncInitable.html) — Asynchronously failable object initialization interface
[Subprocesses](https://developer.gnome.org/gio/stable/subprocesses.html)
[GSubprocess](https://developer.gnome.org/gio/stable/GSubprocess.html) — Child processes
[GSubprocess Launcher](https://developer.gnome.org/gio/stable/GSubprocessLauncher.html) — Environment options for launching a child process
[Low-level network support](https://developer.gnome.org/gio/stable/networking.html)
[GSocket](https://developer.gnome.org/gio/stable/GSocket.html) — Low-level socket object
[GDatagramBased](https://developer.gnome.org/gio/stable/GDatagramBased.html) — Low-level datagram communications interface
[GInetAddress](https://developer.gnome.org/gio/stable/GInetAddress.html) — An IPv4/IPv6 address
[GInetAddressMask](https://developer.gnome.org/gio/stable/GInetAddressMask.html) — An IPv4/IPv6 address mask
[GSocketAddress](https://developer.gnome.org/gio/stable/GSocketAddress.html) — Abstract base class representing endpoints for socket communication
[GInetSocketAddress](https://developer.gnome.org/gio/stable/GInetSocketAddress.html) — Internet GSocketAddress
[GUnixSocketAddress](https://developer.gnome.org/gio/stable/GUnixSocketAddress.html) — UNIX GSocketAddress
[GSocketControlMessage](https://developer.gnome.org/gio/stable/GSocketControlMessage.html) — A GSocket control message
[GUnixFDList](https://developer.gnome.org/gio/stable/GUnixFDList.html) — An object containing a set of UNIX file descriptors
[GUnixFDMessage](https://developer.gnome.org/gio/stable/GUnixFDMessage.html) — A GSocketControlMessage containing a GUnixFDList
[GCredentials](https://developer.gnome.org/gio/stable/GCredentials.html) — An object containing credentials
[GUnixCredentialsMessage](https://developer.gnome.org/gio/stable/GUnixCredentialsMessage.html) — A GSocketControlMessage containing credentials
[GProxy](https://developer.gnome.org/gio/stable/GProxy.html) — Interface for proxy handling
[GProxyAddress](https://developer.gnome.org/gio/stable/GProxyAddress.html) — An internet address with proxy information
[gnetworking.h](https://developer.gnome.org/gio/stable/gio-gnetworking.h.html) — System networking includes
[High-level network functionallity](https://developer.gnome.org/gio/stable/highlevel-socket.html)
[GSocketClient](https://developer.gnome.org/gio/stable/GSocketClient.html) — Helper for connecting to a network service
[GSocketConnection](https://developer.gnome.org/gio/stable/GSocketConnection.html) — A socket connection
[GUnixConnection](https://developer.gnome.org/gio/stable/GUnixConnection.html) — A UNIX domain GSocketConnection
[GTcpConnection](https://developer.gnome.org/gio/stable/GTcpConnection.html) — A TCP GSocketConnection
[GTcpWrapperConnection](https://developer.gnome.org/gio/stable/GTcpWrapperConnection.html) — Wrapper for non-GSocketConnection-based, GSocket-based GIOStreams
[GSocketListener](https://developer.gnome.org/gio/stable/GSocketListener.html) — Helper for accepting network client connections
[GSocketService](https://developer.gnome.org/gio/stable/GSocketService.html) — Make it easy to implement a network service
[GThreadedSocketService](https://developer.gnome.org/gio/stable/GThreadedSocketService.html) — A threaded GSocketService
[GNetworkMonitor](https://developer.gnome.org/gio/stable/GNetworkMonitor.html) — Network status monitor
[TLS (SSL) support](https://developer.gnome.org/gio/stable/tls.html)
[TLS Overview](https://developer.gnome.org/gio/stable/gio-TLS-Overview.html) — TLS (aka SSL) support for GSocketConnection
[GTlsCertificate](https://developer.gnome.org/gio/stable/GTlsCertificate.html) — TLS certificate
[GTlsConnection](https://developer.gnome.org/gio/stable/GTlsConnection.html) — TLS connection type
[GTlsClientConnection](https://developer.gnome.org/gio/stable/GTlsClientConnection.html) — TLS client-side connection
[GTlsServerConnection](https://developer.gnome.org/gio/stable/GTlsServerConnection.html) — TLS server-side connection
[GDtlsConnection](https://developer.gnome.org/gio/stable/GDtlsConnection.html) — DTLS connection type
[GDtlsClientConnection](https://developer.gnome.org/gio/stable/GDtlsClientConnection.html) — DTLS client-side connection
[GDtlsServerConnection](https://developer.gnome.org/gio/stable/GDtlsServerConnection.html) — DTLS server-side connection
[GTlsBackend](https://developer.gnome.org/gio/stable/GTlsBackend.html) — TLS backend implementation
[GTlsDatabase](https://developer.gnome.org/gio/stable/GTlsDatabase.html) — TLS database type
[GTlsFileDatabase](https://developer.gnome.org/gio/stable/GTlsFileDatabase.html) — TLS file based database type
[GTlsInteraction](https://developer.gnome.org/gio/stable/GTlsInteraction.html) — Interaction with the user during TLS operations.
[GTlsPassword](https://developer.gnome.org/gio/stable/GTlsPassword.html) — TLS Passwords for prompting
[DNS resolution](https://developer.gnome.org/gio/stable/resolver.html)
[GResolver](https://developer.gnome.org/gio/stable/GResolver.html) — Asynchronous and cancellable DNS resolver
[GProxyResolver](https://developer.gnome.org/gio/stable/GProxyResolver.html) — Asynchronous and cancellable network proxy resolver
[GSimpleProxyResolver](https://developer.gnome.org/gio/stable/GSimpleProxyResolver.html) — Simple proxy resolver implementation
[GSocketConnectable](https://developer.gnome.org/gio/stable/GSocketConnectable.html) — Interface for potential socket endpoints
[GSocketAddressEnumerator](https://developer.gnome.org/gio/stable/GSocketAddressEnumerator.html)
[GProxyAddressEnumerator](https://developer.gnome.org/gio/stable/GProxyAddressEnumerator.html)
[GNetworkAddress](https://developer.gnome.org/gio/stable/GNetworkAddress.html) — A GSocketConnectable for resolving hostnames
[GNetworkService](https://developer.gnome.org/gio/stable/GNetworkService.html) — A GSocketConnectable for resolving SRV records
[GSrvTarget](https://developer.gnome.org/gio/stable/GSrvTarget.html) — DNS SRV record target
[Low-level D-Bus Support](https://developer.gnome.org/gio/stable/gdbus-lowlevel.html)
[D-Bus Utilities](https://developer.gnome.org/gio/stable/gio-D-Bus-Utilities.html) — Various utilities related to D-Bus
[D-Bus Addresses](https://developer.gnome.org/gio/stable/gio-D-Bus-Addresses.html) — D-Bus connection endpoints
[D-Bus Introspection Data](https://developer.gnome.org/gio/stable/gio-D-Bus-Introspection-Data.html) — Node and interface description data structures
[GDBusError](https://developer.gnome.org/gio/stable/gio-GDBusError.html) — Mapping D-Bus errors to and from GError
[GDBusMessage](https://developer.gnome.org/gio/stable/GDBusMessage.html) — D-Bus Message
[GDBusConnection](https://developer.gnome.org/gio/stable/GDBusConnection.html) — D-Bus Connections
[GDBusMethodInvocation](https://developer.gnome.org/gio/stable/GDBusMethodInvocation.html) — Object for handling remote calls
[GDBusServer](https://developer.gnome.org/gio/stable/GDBusServer.html) — Helper for accepting connections
[GDBusAuthObserver](https://developer.gnome.org/gio/stable/GDBusAuthObserver.html) — Object used for authenticating connections
[High-level D-Bus Support](https://developer.gnome.org/gio/stable/gdbus-convenience.html)
[Owning Bus Names](https://developer.gnome.org/gio/stable/gio-Owning-Bus-Names.html) — Simple API for owning bus names
[Watching Bus Names](https://developer.gnome.org/gio/stable/gio-Watching-Bus-Names.html) — Simple API for watching bus names
[GDBusInterface](https://developer.gnome.org/gio/stable/GDBusInterface.html) — Base type for D-Bus interfaces
[GDBusInterfaceSkeleton](https://developer.gnome.org/gio/stable/GDBusInterfaceSkeleton.html) — Service-side D-Bus interface
[GDBusProxy](https://developer.gnome.org/gio/stable/GDBusProxy.html) — Client-side D-Bus interface proxy
[GDBusObject](https://developer.gnome.org/gio/stable/GDBusObject.html) — Base type for D-Bus objects
[GDBusObjectSkeleton](https://developer.gnome.org/gio/stable/GDBusObjectSkeleton.html) — Service-side D-Bus object
[GDBusObjectProxy](https://developer.gnome.org/gio/stable/GDBusObjectProxy.html) — Client-side D-Bus object
[GDBusObjectManager](https://developer.gnome.org/gio/stable/GDBusObjectManager.html) — Base type for D-Bus object managers
[GDBusObjectManagerServer](https://developer.gnome.org/gio/stable/GDBusObjectManagerServer.html) — Service-side object manager
[GDBusObjectManagerClient](https://developer.gnome.org/gio/stable/GDBusObjectManagerClient.html) — Client-side object manager
[Settings](https://developer.gnome.org/gio/stable/settings.html)
[GSettings](https://developer.gnome.org/gio/stable/GSettings.html) — High-level API for application settings
[GSettingsBackend](https://developer.gnome.org/gio/stable/GSettingsBackend.html) — Interface for settings backend implementations
[GSettingsSchema, GSettingsSchemaSource](https://developer.gnome.org/gio/stable/gio-GSettingsSchema-GSettingsSchemaSource.html) — Introspecting and controlling the loading of GSettings schemas
[Resources](https://developer.gnome.org/gio/stable/resources.html)
[GResource](https://developer.gnome.org/gio/stable/GResource.html) — Resource framework
[Permissions](https://developer.gnome.org/gio/stable/permissions.html)
[GPermission](https://developer.gnome.org/gio/stable/GPermission.html) — An object representing the permission to perform a certain action
[GSimplePermission](https://developer.gnome.org/gio/stable/GSimplePermission.html) — A GPermission that doesn't change value
[Data Models](https://developer.gnome.org/gio/stable/data-models.html)
[GListModel](https://developer.gnome.org/gio/stable/GListModel.html) — An interface describing a dynamic list of objects
[GListStore](https://developer.gnome.org/gio/stable/GListStore.html) — A simple implementation of GListModel
[Win32 registry support](https://developer.gnome.org/gio/stable/registry.html)
[GWin32RegistryKey](https://developer.gnome.org/gio/stable/gio-GWin32RegistryKey.html) — W32 registry access helper
[Application support](https://developer.gnome.org/gio/stable/application.html)
[GApplication](https://developer.gnome.org/gio/stable/GApplication.html) — Core application class
[GApplicationCommandLine](https://developer.gnome.org/gio/stable/GApplicationCommandLine.html) — A command-line invocation of an application
[GActionGroup](https://developer.gnome.org/gio/stable/GActionGroup.html) — A group of actions
[GActionMap](https://developer.gnome.org/gio/stable/GActionMap.html) — Interface for action containers
[GSimpleActionGroup](https://developer.gnome.org/gio/stable/GSimpleActionGroup.html) — A simple GActionGroup implementation
[GAction](https://developer.gnome.org/gio/stable/GAction.html) — An action interface
[GSimpleAction](https://developer.gnome.org/gio/stable/GSimpleAction.html) — A simple GAction implementation
[GPropertyAction](https://developer.gnome.org/gio/stable/GPropertyAction.html) — A GAction reflecting a GObject property
[GRemoteActionGroup](https://developer.gnome.org/gio/stable/GRemoteActionGroup.html) — A GActionGroup that interacts with other processes
[GActionGroup exporter](https://developer.gnome.org/gio/stable/gio-GActionGroup-exporter.html) — Export GActionGroups on D-Bus
[GDBusActionGroup](https://developer.gnome.org/gio/stable/GDBusActionGroup.html) — A D-Bus GActionGroup implementation
[GMenuModel](https://developer.gnome.org/gio/stable/GMenuModel.html) — An abstract class representing the contents of a menu
[GMenu](https://developer.gnome.org/gio/stable/GMenu.html) — A simple implementation of GMenuModel
[GMenuModel exporter](https://developer.gnome.org/gio/stable/gio-GMenuModel-exporter.html) — Export GMenuModels on D-Bus
[GDBusMenuModel](https://developer.gnome.org/gio/stable/GDBusMenuModel.html) — A D-Bus GMenuModel implementation
[GNotification](https://developer.gnome.org/gio/stable/GNotification.html) — User Notifications (pop up messages)
[Extending GIO](https://developer.gnome.org/gio/stable/extending.html)
[GVfs](https://developer.gnome.org/gio/stable/GVfs.html) — Virtual File System
[GIOModule](https://developer.gnome.org/gio/stable/GIOModule.html) — Loadable GIO Modules
[Extension Points](https://developer.gnome.org/gio/stable/gio-Extension-Points.html) — Extension Points
[GIO Tools](https://developer.gnome.org/gio/stable/tools.html)
[gio-querymodules](https://developer.gnome.org/gio/stable/gio-querymodules.html) — GIO module cache creation
[gsettings](https://developer.gnome.org/gio/stable/gsettings-tool.html) — GSettings configuration tool
[glib-compile-schemas](https://developer.gnome.org/gio/stable/glib-compile-schemas.html) — GSettings schema compiler
[glib-compile-resources](https://developer.gnome.org/gio/stable/glib-compile-resources.html) — GLib resource compiler
[gdbus](https://developer.gnome.org/gio/stable/gdbus.html) — Tool for working with D-Bus objects
[gdbus-codegen](https://developer.gnome.org/gio/stable/gdbus-codegen.html) — D-Bus code and documentation generator
[gresource](https://developer.gnome.org/gio/stable/gresource-tool.html) — GResource tool
[gapplication](https://developer.gnome.org/gio/stable/gapplication-tool.html) — D-Bus application launcher
[gio](https://developer.gnome.org/gio/stable/gio.html) — GIO commandline tool
[GIO Testing](https://developer.gnome.org/gio/stable/testing.html)
[GTestDBus](https://developer.gnome.org/gio/stable/GTestDBus.html) — D-Bus testing helper
[III. Migrating to GIO](https://developer.gnome.org/gio/stable/migrating.html)
[Migrating from GnomeVFS to GIO](https://developer.gnome.org/gio/stable/ch33.html)
[Trash handling](https://developer.gnome.org/gio/stable/ch33.html#id-1.5.3.3)
[Operations on multiple files](https://developer.gnome.org/gio/stable/ch33s02.html)
[Mime monitoring](https://developer.gnome.org/gio/stable/ch33s03.html)
[Migrating from GConf to GSettings](https://developer.gnome.org/gio/stable/ch34.html)
[Before you start](https://developer.gnome.org/gio/stable/ch34.html#id-1.5.4.2)
[Conceptual differences](https://developer.gnome.org/gio/stable/ch34s02.html)
[GConfClient (and GConfBridge) API conversion](https://developer.gnome.org/gio/stable/ch34s03.html)
[Change notification](https://developer.gnome.org/gio/stable/ch34s04.html)
[Change sets](https://developer.gnome.org/gio/stable/ch34s05.html)
[Schema conversion](https://developer.gnome.org/gio/stable/ch34s06.html)
[Data conversion](https://developer.gnome.org/gio/stable/ch34s07.html)
[Migrating to GDBus](https://developer.gnome.org/gio/stable/ch35.html)
[Conceptual differences](https://developer.gnome.org/gio/stable/ch35.html#id-1.5.5.2)
[API comparison](https://developer.gnome.org/gio/stable/ch35s02.html)
[Owning bus names](https://developer.gnome.org/gio/stable/ch35s03.html)
[Creating proxies for well-known names](https://developer.gnome.org/gio/stable/ch35s04.html)
[Using gdbus-codegen](https://developer.gnome.org/gio/stable/gdbus-example-gdbus-codegen.html)
[Object Hierarchy](https://developer.gnome.org/gio/stable/gio-hierarchy.html)
[Index](https://developer.gnome.org/gio/stable/api-index-full.html)
[Annotation Glossary](https://developer.gnome.org/gio/stable/annotation-glossary.html)
Generated by GTK-Doc V1.29.1
