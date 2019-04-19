# ioctl(), ioctl_socket() - Koma Hub - CSDN博客
2019年01月26日 18:25:17[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：39
个人分类：[Socket																[Linux kernel																[计算机网络](https://blog.csdn.net/Rong_Toa/article/category/7569489)](https://blog.csdn.net/Rong_Toa/article/category/7348890)](https://blog.csdn.net/Rong_Toa/article/category/7792858)
[http://www.qnx.com/developers/docs/6.5.0/index.jsp?topic=%2Fcom.qnx.doc.neutrino_lib_ref%2Fi%2Fioctl.html](http://www.qnx.com/developers/docs/6.5.0/index.jsp?topic=%2Fcom.qnx.doc.neutrino_lib_ref%2Fi%2Fioctl.html)
**目录**
[Synopsis:](#Synopsis%3A)
[Arguments:](#Arguments%3A)
[Library:](#Library%3A)
[Description:](#Description%3A)
[How ioctl() commands map to other commands](#How%C2%A0ioctl()%C2%A0commands%20map%20to%20other%20commands)
[Commands with special handling in ioctl_socket()](#Commands%20with%20special%20handling%20in%C2%A0ioctl_socket())
[Returns:](#Returns%3A)
[Errors:](#Errors%3A)
[Classification:](#Classification%3A)
[Caveats:](#Caveats%3A)
[See also:](#See%20also%3A)
*Control a device*
## Synopsis:
```cpp
#include <sys/ioctl.h> 
int ioctl( int fd, 
           int request, 
           ... );
int ioctl_socket( int fd, 
                  int request, 
                  ... );
```
## Arguments:
*fd*
An open file descriptor for the file or device that you want to manipulate.
*request*
What you want to do to the file or device; see below for a summary. The macros and definitions that you use in specifying a request are located in the file <sys/ioctl.h>.
Additional arguments
As required by the request.
## Library:
|Function|Library|Linking option to [qcc](http://www.qnx.com/developers/docs/6.5.0/topic/com.qnx.doc.neutrino_utilities/q/qcc.html)|
|----|----|----|
|*ioctl()*|libc|**-l c** (This library is usually included automatically.)|
|*ioctl_socket()*|libsocket|**-l socket**|
## Description:
The *ioctl()* function manipulates the underlying parameters of files. In particular, it can be used to control many of the operating attributes of files (such as the attributes of terminals).
The *ioctl_socket()* function is an optimized version of *ioctl()* that provides special handling for commands that use embedded pointers (see “[Commands with special handling in *ioctl_socket()*](http://www.qnx.com/developers/docs/6.5.0/topic/com.qnx.doc.neutrino_lib_ref/i/ioctl.html#ioctl_socket_handling),” below). It uses *ioctl()* for commands that don't require special handling. The *fd* argument must be a socket for *ioctl_socket()*.
|![Note:](http://www.qnx.com/developers/docs/6.5.0/topic/com.qnx.doc.neutrino_lib_ref/pointing.gif)|If you're using QNX Neutrino 6.4.1 or earlier, use *ioctl_socket()* instead of *ioctl()* in network applications such as packet filters.			In QNX Neutrino 6.5.0 and later, *ioctl()* handles embedded pointers, so you don't have to use *ioctl_socket()* instead.|
The *request* argument determines whether the subsequent arguments are an “in” or “out” parameter; it also specifies the size of the arguments in bytes.
### How *ioctl()* commands map to other commands
Some *ioctl()* commands map to calls to [*fcntl()*](http://www.qnx.com/developers/docs/6.5.0/topic/com.qnx.doc.neutrino_lib_ref/f/fcntl.html), [*tcgetattr()*](http://www.qnx.com/developers/docs/6.5.0/topic/com.qnx.doc.neutrino_lib_ref/t/tcgetattr.html), [*tcsetattr()*](http://www.qnx.com/developers/docs/6.5.0/topic/com.qnx.doc.neutrino_lib_ref/t/tcsetattr.html), and [*tcsetsid()*](http://www.qnx.com/developers/docs/6.5.0/topic/com.qnx.doc.neutrino_lib_ref/t/tcsetsid.html). Other commands are transformed into a different [*devctl()*](http://www.qnx.com/developers/docs/6.5.0/topic/com.qnx.doc.neutrino_lib_ref/d/devctl.html) command, and the rest are simply passed to *devctl()*. Here's a summary:
|*ioctl()* command|Description|Maps to:|
|----|----|----|
|FIOASYNC|Set or clear asynchronous I/O|*fcntl()*: F_GETFL (which becomes a *devctl()* DCMD_ALL_GETFLAGS), followed by *fcntl()*: F_SETFL (which becomes a *devctl()* DCMD_ALL_SETFLAGS), setting or clearing O_ASYNC|
|FIOCLEX|Set “close on exec” on a file descriptor|*fcntl()*: F_SETFD with FD_CLOEXEC|
|FIOGETOWN|Get the owner|*fcntl()*: F_GETOWN|
|FIONBIO|Set or clear non-blocking I/O|*fcntl()*: F_GETFL (which becomes a *devctl()* DCMD_ALL_GETFLAGS), followed by *fcntl()*: F_SETFL (which becomes a *devctl()* DCMD_ALL_SETFLAGS), setting or clearing O_NONBLOCK|
|FIONCLEX|Remove “close on exec”|*fcntl()*: F_SETFD with ~FD_CLOEXEC|
|FIONREAD|Get the number of bytes to read|*devctl()*: DCMD_CHR_ISCHARS|
|FIOSETOWN|Set the owner|*fcntl()*: F_SETOWN|
|NOSIOCGIFCONF|Get ifnet list|Passed to *devctl()*|
|OSIOCGIFADDR|Get ifnet address|Passed to *devctl()*|
|OSIOCGIFBRDADDR|Get broadcast address|Passed to *devctl()*|
|OSIOCGIFCONF|Get ifnet list|Passed to *devctl()*|
|OSIOCGIFDSTADDR|Get point-to-point address|Passed to *devctl()*|
|OSIOCGIFNETMASK|Get net address mask|Passed to *devctl()*|
|SIOCADDMULTI|Add multicast address|Passed to *devctl()*|
|SIOCADDRT|Add route|Passed to *devctl()*|
|SIOCAIFADDR|Add or change an interface alias|Passed to *devctl()*|
|SIOCALIFADDR|Add interface address|Passed to *devctl()*|
|SIOCATMARK|At out-of-band mark?|Passed to *devctl()*|
|SIOCDARP|Delete ARP entry|Passed to *devctl()*|
|SIOCDELMULTI|Delete multicast address|Passed to *devctl()*|
|SIOCDELRT|Delete route|Passed to *devctl()*|
|SIOCDIFADDR|Delete interface address|Passed to *devctl()*|
|SIOCDIFPHYADDR|Delete gif addresses|Passed to *devctl()*|
|SIOCDLIFADDR|Delete interface address|Passed to *devctl()*|
|SIOCGARP|Get ARP entry|Passed to *devctl()*|
|SIOCGDRVSPEC|Get driver-specific parameters|Passed to *devctl()*|
|SIOCGETSGCNT|Get the source group packet count|Passed to *devctl()*|
|SIOCGETVIFCNT|Get the packet count for a virtual interface|Passed to *devctl()*|
|SIOCGHIWAT|Get high watermark|Passed to *devctl()*|
|SIOCGIFADDR|Get the interface address|Passed to *devctl()*|
|SIOCGIFALIAS|Get interface alias|Passed to *devctl()*|
|SIOCGIFASYNCMAP|Get ppp asyncmap|Passed to *devctl()*|
|SIOCGIFBRDADDR|Get broadcast address|Passed to *devctl()*|
|SIOCGIFCAP|Get capabilities|Passed to *devctl()*|
|SIOCGIFCONF|Return a list of interface (transport layer) addresses|Passed to *devctl()*|
|SIOCGIFDLT|Get data link type|Passed to *devctl()*|
|SIOCGIFDSTADDR|Get point-to-point address|Passed to *devctl()*|
|SIOCGIFFLAGS|Get ifnet flags|Passed to *devctl()*|
|SIOCGIFGENERIC|Generic interface get op|Passed to *devctl()*|
|SIOCGIFMEDIA|Get net media; *ioctl_socket()* provides special handling for this command.|Passed to *devctl()*|
|SIOCGIFMETRIC|Get interface metric|Passed to *devctl()*|
|SIOCGIFMTU|Get ifnet MTU|Passed to *devctl()*|
|SIOCGIFNETMASK|Get net address mask|Passed to *devctl()*|
|SIOCGIFPDSTADDR|Get gif pdst address|Passed to *devctl()*|
|SIOCGIFPSRCADDR|Get gif psrc address|Passed to *devctl()*|
|SIOCGLIFADDR|Get interface address|Passed to *devctl()*|
|SIOCGLIFPHYADDR|Get gif addresses|Passed to *devctl()*|
|SIOCGLOWAT|Get low watermark|Passed to *devctl()*|
|SIOCGPGRP|Get process group|*fcntl()*: F_GETOWN|
|SIOCIFCREATE|Create clone interface|Passed to *devctl()*|
|SIOCIFDESTROY|Destroy clone interface|Passed to *devctl()*|
|SIOCIFGCLONERS|Get cloners; *ioctl_socket()* provides special handling for this command.|Passed to *devctl()*|
|SIOCSARP|Set ARP entry|Passed to *devctl()*|
|SIOCSDRVSPEC|Set driver-specific parameters|Passed to *devctl()*|
|SIOCSHIWAT|Set high watermark|Passed to *devctl()*|
|SIOCSIFADDR|Set ifnet address|Passed to *devctl()*|
|SIOCSIFASYNCMAP|Set ppp asyncmap|Passed to *devctl()*|
|SIOCSIFBRDADDR|Set broadcast address|Passed to *devctl()*|
|SIOCSIFCAP|Set capabilities|Passed to *devctl()*|
|SIOCSIFDSTADDR|Set point-to-point address|Passed to *devctl()*|
|SIOCSIFFLAGS|Set ifnet flags|Passed to *devctl()*|
|SIOCSIFGENERIC|Generic interface set op|Passed to *devctl()*|
|SIOCSIFMEDIA|Set net media|Passed to *devctl()*|
|SIOCSIFMETRIC|Set interface metric|Passed to *devctl()*|
|SIOCSIFMTU|Set ifnet MTU|Passed to *devctl()*|
|SIOCSIFNETMASK|Set net address mask|Passed to *devctl()*|
|SIOCSIFPHYADDR|Set gif address|Passed to *devctl()*|
|SIOCSLIFPHYADDR|Set gif addresses|Passed to *devctl()*|
|SIOCSLOWAT|Set low watermark|Passed to *devctl()*|
|SIOCSPGRP|Set process group|*fcntl()*: F_SETOWN|
|TCFLSH|Flush buffers|Passed to *devctl()*|
|TCGETA|Get the terminal's properties in a termio structure|*tcgetattr()*|
|TCGETS|Get the terminal's properties in a termios structure|*tcgetattr()*|
|TCSBRK|Send a break for a period of time|*devctl()* : DCMD_CHR_SERCTL|
|TCSETA|Set the terminal's properties|*tcsetattr()*|
|TCSETAF|Set the terminal's properties, waiting until all currently written data has been transmitted, and discarding any received but unread data|*tcsetattr()*|
|TCSETAW|Set the terminal's properties, waiting until all currently written data has been transmitted|*tcsetattr()*|
|TCSETS|Set the terminal's properties from a termios structure|*devctl()* : DCMD_CHR_TCSETATTR|
|TCSETSF|Drain the output, flush the input, and set|Passed to *devctl()*|
|TCSETSW|Drain output, set|Passed to *devctl()*|
|TCXONC|Perform a flow-control operation on a data stream|*devctl()*: DCMD_CHR_TCFLOW|
|TIOCCBRK|Clear the break bit|*tcsetattr()*|
|TIOCCDTR|Clear the Data Terminal Ready line|*devctl()* : DCMD_CHR_LINESTATUS, followed by DCMD_CHR_SERCTL|
|TIOCDRAIN|Wait until output has drained|*devctl()*: DCMD_CHR_TCDRAIN|
|TIOCEXCL|Set exclusive use of tty|*fcntl()*: F_SETLK|
|TIOCFLUSH|Flush buffers|*devctl()*: DCMD_CHR_TCFLUSH|
|TIOCGETA|Get the terminal's properties in a termios structure|*devctl()* : DCMD_CHR_TCGETATTR|
|TIOCGETC|Get special characters|*tcgetattr()*|
|TIOCGETP|Get the terminal's parameters in a sgttyb structure|*tcgetattr()*|
|TIOCGETPGRP|Get the process group of the tty (POSIX)|Passed to *devctl()*|
|TIOCGLTC|Get local special characters|*tcgetattr()*|
|TIOCGPGRP|Get the process group of the tty|*devctl()*: DCMD_CHR_TCGETPGRP|
|TIOCGSIZE|Get window size|*devctl()*: DCMD_CHR_GETSIZE|
|TIOCGWINSZ|Get window size|*devctl()*: DCMD_CHR_GETSIZE|
|TIOCHPCL|Hang up on last close|*tcsetattr()*|
|TIOCLGET|Get local modes|*tcgetattr()*|
|TIOCLSET|Set entire local mode word|*tcsetattr()*|
|TIOCMBIC|Clear modem port B interrupt|*devctl()* : DCMD_CHR_LINESTATUS, followed by DCMD_CHR_SERCTL|
|TIOCMBIS|Set modem port B interrupt|*devctl()* : DCMD_CHR_LINESTATUS, followed by DCMD_CHR_SERCTL|
|TIOCMGET|Set the state of the modem lines|*devctl()* : DCMD_CHR_SERCTL|
|TIOCMSET|Set all modem bits|Passed to *devctl()*|
|TIOCNOTTY|Make this terminal not be the controlling terminal for the process|*tcsetsid()*|
|TIOCNXCL|Reset exclusive use of tty|*fcntl()*: F_SETLK|
|TIOCOUTQ|Output queue size|*devctl()*: DCMD_CHR_OSCHARS|
|TIOCPKT|Pty: set/clear packet mode|Passed to *devctl()*|
|TIOCSBRK|Set break bit|Passed to *devctl()*|
|TIOCSCTTY|Make the terminal become the controlling tty|*tcsetsid()*|
|TIOCSDTR|Set the Data Terminal Ready line|*devctl()* : DCMD_CHR_LINESTATUS, followed by DCMD_CHR_SERCTL|
|TIOCSETA|Set the terminal's properties from a termios structure|*devctl()* : DCMD_CHR_TCSETATTR|
|TIOCSETAF|Drain the output, flush the input, and set|*devctl()*: DCMD_CHR_TCSETATTRF|
|TIOCSETAW|Drain output, set|*devctl()*: DCMD_CHR_TCSETATTRD|
|TIOCSETC|Set special characters|*tcsetattr()*|
|TIOCSETN|Similar to TIOCSETP, but the changes are made immediately without discarding any data.|*tcsetattr()*|
|TIOCSETP|Set the terminal's parameters in a sgttyb structure. No change is made until all currently written data has been transmitted, at which point any received but unread data is discarded.|*tcsetattr()*|
|TIOCSETPGRP|Set the process group of the tty (POSIX)|Passed to *devctl()*|
|TIOCSINUSE|Set exclusive use of tty|*fcntl()*: F_SETLK|
|TIOCSLTC|Set local special characters|*tcsetattr()*|
|TIOCSPGRP|Set the process group of the tty|*devctl()*: DCMD_CHR_TCSETPGRP|
|TIOCSSIZE|Set the window size|*devctl()*: DCMD_CHR_SETSIZE|
|TIOCSTART|Start output, like **Ctrl**-**Q**|Passed to *devctl()*|
|TIOCSTI|Simulate terminal input|*devctl()*: DCMD_CHR_TCINJECTC|
|TIOCSTOP|Stop output, like **Ctrl**-**S**|Passed to *devctl()*|
|TIOCSWINSZ|Set window size|*devctl()*: DCMD_CHR_SETSIZE|
|UIOCCMD|User control operator *n*|Passed to *devctl()*|
|![Note:](http://www.qnx.com/developers/docs/6.5.0/topic/com.qnx.doc.neutrino_lib_ref/pointing.gif)|If you're writing a resource manager, note that the [*iofunc_devctl_default()*](http://www.qnx.com/developers/docs/6.5.0/topic/com.qnx.doc.neutrino_lib_ref/i/iofunc_devctl_default.html) function handles the DCMD_ALL_GETFLAGS and DCMD_ALL_SETFLAGS commands; your resource manager will need to handle all other devctl commands that apply to it.|
### Commands with special handling in *ioctl_socket()*
The *ioctl_socket()* function provides special handling for the commands listed below. Unless otherwise noted, it eventually passes the command to [*devctl()*](http://www.qnx.com/developers/docs/6.5.0/topic/com.qnx.doc.neutrino_lib_ref/d/devctl.html).
These commands are for the Berkeley Packet Filter:
BIOCGDLTLIST
Get an array of the available types of the data link layer underlying the attached interface.
BIOCSETF
Set the BPF filter program.
These commands are for use with cryptography:
CIOCCRYPT
Request a symmetric-key (or unkeyed hash) operation.
CIOCGSESSION
Set up a session for symmetric crypto operations.
CIOCKEY
Perform an asymmetric crypto operation.
CRIOGET
Get a cloned file descriptor of /dev/crypto by calling [*open()*](http://www.qnx.com/developers/docs/6.5.0/topic/com.qnx.doc.neutrino_lib_ref/o/open.html). This isn't necessary in QNX Neutrino, but we've provided it for compatibility and portability.
The following commands are for use with the Packet Filtering interface; for more information, see the entry for [pf](http://www.qnx.com/developers/docs/6.5.0/topic/com.qnx.doc.neutrino_utilities/p/pf.html) in the Utilities Reference:
- DIOCGETSRCNODES
- DIOCGETSTATES
- DIOCIGETIFACES
- DIOCRADDADDRS
- DIOCRADDTABLES
- DIOCRCLRASTATS
- DIOCRCLRTSTATS
- DIOCRDELADDRS
- DIOCRDELTABLES
- DIOCRGETADDRS
- DIOCRGETASTATS
- DIOCRGETTABLES
- DIOCRGETTSTATS
- DIOCRINADEFINE
- DIOCRSETADDRS
- DIOCRSETTFLAGS
- DIOCRTSTADDRS
- DIOCXBEGIN
- DIOCXCOMMIT
- DIOCXROLLBACK
These commands are for use with sockets:
SIOCG80211
Get configuration or status information.
SIOCG80211NWID
Get the network ID.
SIOCG80211NWKEY
Get the values of the WEP keys.
SIOCG80211STATS, SIOCG80211ZSTATS
Get IEEE 80211 statistics.
SIOCGETVLAN
Get the vlan tag and parent for a given vlan interface.
SIOCGIFMEDIA
Get net media.
SIOCIFGCLONERS
Get cloners.
SIOCS80211
Set configuration or status information.
SIOCS80211NWID
Set the network ID.
SIOCS80211NWKEY
Set the WEP key.
SIOCSETVLAN
Set the vlan tag and parent for a given vlan interface.
## Returns:
A value based on the *request*, or -1 if an error occurs ([*errno*](http://www.qnx.com/developers/docs/6.5.0/topic/com.qnx.doc.neutrino_lib_ref/e/errno.html) is set).
## Errors:
EBADF
Invalid descriptor *fd*.
EINVAL
The *request* or optional variables aren't valid.
ENOBUFS
There wasn't enough memory available to allocate for data referred to by embedded pointers.
ENOTTY
The *fd* argument isn't associated with a character-special device; the specified *request* doesn't apply to the kind of object that the descriptor *fd* references.
## Classification:
[POSIX 1003.1 XSR](http://www.qnx.com/developers/docs/6.5.0/topic/com.qnx.doc.neutrino_lib_ref/summary.html#CLASSIFICATION)
|Safety:||
|----|----|
|Cancellation point|No|
|Interrupt handler|No|
|Signal handler|No|
|Thread|No|
## Caveats:
The *ioctl()* function is a Unix function that varies greatly from platform to platform.
## See also:
[*devctl()*](http://www.qnx.com/developers/docs/6.5.0/topic/com.qnx.doc.neutrino_lib_ref/d/devctl.html), [*fcntl()*](http://www.qnx.com/developers/docs/6.5.0/topic/com.qnx.doc.neutrino_lib_ref/f/fcntl.html), [*tcgetattr()*](http://www.qnx.com/developers/docs/6.5.0/topic/com.qnx.doc.neutrino_lib_ref/t/tcgetattr.html), [*tcsetattr()*](http://www.qnx.com/developers/docs/6.5.0/topic/com.qnx.doc.neutrino_lib_ref/t/tcsetattr.html), [*tcsetsid()*](http://www.qnx.com/developers/docs/6.5.0/topic/com.qnx.doc.neutrino_lib_ref/t/tcsetsid.html).
