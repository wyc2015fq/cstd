# Libnfc - xqhrs232的专栏 - CSDN博客
2015年12月01日 09:31:05[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1134
原文地址::[http://nfc-tools.org/index.php?title=Libnfc](http://nfc-tools.org/index.php?title=Libnfc)

**Public platform independent Near Field Communication (NFC) library**
libnfc is the first [libre](http://en.wikipedia.org/wiki/Libre_software) low
 level [NFC SDK and Programmers API](http://nfc-tools.org/index.php?title=Libnfc:API) released under the [GNU
 Lesser General Public License](http://nfc-tools.org/index.php?title=Libnfc:License). 
It provides complete transparency and royalty-free use for everyone. 
This list shows the current [supported features](http://nfc-tools.org/index.php?title=Libnfc:Features). 
All major operating systems are supported, including GNU/Linux, Mac OS X and Windows. Compilation should work out of the box on POSIX-systems. 
This library supports [various NFC hardware devices](http://nfc-tools.org/index.php?title=Devices_compatibility_matrix): dongles,
 flat and OEM devices. 
The library currently supports modulations for [ISO/IEC 14443](http://nfc-tools.org/index.php?title=ISO14443) A and B, [FeliCa](http://nfc-tools.org/index.php?title=FeliCa), [Jewel/Topaz](http://nfc-tools.org/index.php?title=Jewel/Topaz) tags
 and Data Exchange Protocol (P2P) as target and as initiator.
## Contents
 [[hide](http://nfc-tools.org/index.php?title=Libnfc#)] 
- [1Download](http://nfc-tools.org/index.php?title=Libnfc#Download)- [1.1Stable
 release](http://nfc-tools.org/index.php?title=Libnfc#Stable_release)
- [1.2Development
 version](http://nfc-tools.org/index.php?title=Libnfc#Development_version)
- [1.3Third
 party packages](http://nfc-tools.org/index.php?title=Libnfc#Third_party_packages)
- [2Installation](http://nfc-tools.org/index.php?title=Libnfc#Installation)- [2.1POSIX
 systems](http://nfc-tools.org/index.php?title=Libnfc#POSIX_systems)- [2.1.1Manual
 installation](http://nfc-tools.org/index.php?title=Libnfc#Manual_installation)
- [2.1.2Debian
 / Ubuntu](http://nfc-tools.org/index.php?title=Libnfc#Debian_.2F_Ubuntu)
- [2.1.3FreeBSD](http://nfc-tools.org/index.php?title=Libnfc#FreeBSD)
- [2.1.4Gentoo](http://nfc-tools.org/index.php?title=Libnfc#Gentoo)
- [2.1.5Fedora
 12+](http://nfc-tools.org/index.php?title=Libnfc#Fedora_12.2B)- [2.1.5.1Using
 command line interface](http://nfc-tools.org/index.php?title=Libnfc#Using_command_line_interface)
- [2.1.5.2Using
 graphical tools](http://nfc-tools.org/index.php?title=Libnfc#Using_graphical_tools)
- [2.1.6Mac
 OS X](http://nfc-tools.org/index.php?title=Libnfc#Mac_OS_X)
- [2.2Windows](http://nfc-tools.org/index.php?title=Libnfc#Windows)
- [3Configuration](http://nfc-tools.org/index.php?title=Libnfc#Configuration)
- [4Usage](http://nfc-tools.org/index.php?title=Libnfc#Usage)
- [5Troubleshooting](http://nfc-tools.org/index.php?title=Libnfc#Troubleshooting)
- [6ChangeLog](http://nfc-tools.org/index.php?title=Libnfc#ChangeLog)
- [7Contribute](http://nfc-tools.org/index.php?title=Libnfc#Contribute)
- [8Sponsoring](http://nfc-tools.org/index.php?title=Libnfc#Sponsoring)
# Download
## Stable release
Stable releases can be found at [libnfc
 download section](http://bintray.com/nfc-tools/sources/libnfc).
## Development version
Fetch current version using [Git](https://en.wikipedia.org/wiki/Git_(software)):
git clone [https://github.com/nfc-tools/libnfc.git](https://github.com/nfc-tools/libnfc.git)
Note: If you want all libnfc hardware drivers, you will need to have libusb (library and headers) plus on *BSD and GNU/Linux systems, libpcsclite (library and headers).
Under MacOSX, GNU/Linux, *BSD and probably a lot of POSIX systems you can compile development version using:
autoreconf -vis
./configure --enable-doc
make
sudo make install
If you want to (re)generate documentation:
make doc
## Third party packages
- **Debian** packages by **Nobuhiro Iwamatsu** and **Ludovic Rousseau**- [packages](http://packages.debian.org/libnfc) in Jessie & Sid
- [package tracking](http://packages.qa.debian.org/libn/libnfc.html)
- [git repo](http://anonscm.debian.org/gitweb/?p=collab-maint/libnfc.git;a=summary) ([git://anonscm.debian.org/collab-maint/libnfc.git]())
# Installation
The following sections describe per platform how to install and use **libnfc**.
Because some dependencies (e.g. [libusb](http://libusb.wiki.sourceforge.net/) and optionnal [PCSC-Lite](http://pcsclite.alioth.debian.org/))
 are used, the requirements per platform can differ.
## POSIX systems
### Manual installation
First make sure you have required compilation dependencies, ie. you will need headers from:
- libusb
Download and extract libnfc archive
$ wget https://bintray.com/artifact/download/nfc-tools/sources/libnfc-x.x.x.tar.bz2
$ tar -xvzf libnfc-x.x.x.tar.gz
Configure and compile
$ cd libnfc-x.x.x
$ ./configure --prefix=/usr --sysconfdir=/etc
$ make
Please note that *./configure* have an installation path prefix set as */usr/local* and sysconfdir set as *$PREFIX/etc* as default. Use *--prefix* and *sysconfdir*option,
 if you want another setup (see *./configure --help*).
Make sure you have required run-time dependencies.
- libusb
Plug in NFC device, and test if libnfc detect it
$ libnfc/utils/nfc-scan-device -v
Place a tag on it and show it
$ libnfc/utils/nfc-list
Install **libnfc** into your system
$ sudo make install
Note: if you have any troubles, please read [README](https://github.com/nfc-tools/libnfc/blob/master/README) and
 browse this wiki before asking (dumb) question on forum and/or maling-lists...
### Debian / Ubuntu
First make sure you have required compilation dependencies.
 $ sudo apt-get install libusb-dev
Download and extract libnfc archive
git clone https://github.com/nfc-tools/libnfc.git
cd libnfc
git checkout libnfc-1.7.1
git clean -d -f -x
#rm ../libnfc*.deb
git remote|grep -q anonscm||git remote add anonscm git://anonscm.debian.org/collab-maint/libnfc.git
git fetch anonscm
git checkout remotes/anonscm/master debian
git reset
dpkg-buildpackage -uc -us -b
Make sure you have required run-time dependencies.
$ sudo apt-get install libusb-0.1-4
Install **libnfc** into your system
$ sudo dpkg -i ../libnfc*.deb
Plug in NFC device, place a tag on it and test your installation
$ nfc-list
### FreeBSD
*Romain Tartière* contributed a [FreeBSD
 port of libnfc](http://www.freshports.org/devel/libnfc/), thanks to him.
- To install the port:
 cd /usr/ports/devel/libnfc/ && make install clean
- To add the package:
 pkg_add -r libnfc
### Gentoo
Thanks to *Mike Auty*, **libnfc** have a ebuild to be easily installed on Gentoo using usual way:
 $ emerge libnfc
### Fedora 12+
Thanks to *François Kooman*, **libnfc** library is now included in Fedora 12+ distribution.
#### Using command line interface
 # yum install libnfc libnfc-devel libnfc-examples
- **libnfc-devel** contains development headers and libraries, its only necessary if you want to
compile your own applications requiring **libnfc**
- **libnfc-examples** contains the example tools like *nfc-list*.
#### Using graphical tools
- *System* -> *Administration* -> *Add/Remove Software*
- Search for **libnfc** and select the appropriate packages for installation.
### Mac OS X
For installing dependencies, MacPorts download manager is used (available at: [http://distfiles.macports.org/MacPorts/](http://distfiles.macports.org/MacPorts/))
Install MacPorts following instructions from [http://www.macports.org/install.php](http://www.macports.org/install.php)
Install pkg-config:
 $ sudo port install pkgconfig
Install glib:
 $ sudo port install glib
Install glib:
 $ sudo port install libusb
Make sure it's the x86_64 version (when on the latest OS X):
 $ lipo -info /usr/local/lib/libusb.dylib
As the libnfc's MacPort is not up-to-date, now follow **Manual installation** instructions to compile libnfc using usual autotools way.
**Warning**: We have previously posted a link to *darwinports .com*, according to MacPorts project: they are an impostor / phishing site designed to get donations from unsuspecting
 users. Donations made to that site do not benefit MacPorts project and the instructions shown there for installing software are incorrect. To know more about this issue: [http://trac.macports.org/wiki/DarwinPorts](http://trac.macports.org/wiki/DarwinPorts)
Official MacPorts' site: [http://www.macports.org/](http://www.macports.org/) If
 you are looking for how to use and install MacPorts, please visit: [http://guide.macports.org/](http://guide.macports.org/)
## Windows
See [Libnfc:Windows](http://nfc-tools.org/index.php?title=Libnfc:Windows)
# Configuration
Libnfc can be tuned by a config file as well as with environment variables. 
See the [configuration](http://nfc-tools.org/index.php?title=Libnfc:configuration) page.
# Usage
See [Examples](http://nfc-tools.org/index.php?title=Category:Libnfc:Examples) how
 to create code using libnfc.
For a list of projects using this library, see [Related
 links](http://nfc-tools.org/index.php?title=Related_links).
A few of them have their page on this wiki, check [Software
 category](http://nfc-tools.org/index.php?title=Category:Software)
# Troubleshooting
Having trouble? Check the [FAQs](http://nfc-tools.org/index.php?title=Libnfc:FAQ) and
 consult the [Troubleshooting](http://nfc-tools.org/index.php?title=Libnfc:Troubleshooting) page. 
Make sure you've checked the [configuration](http://nfc-tools.org/index.php?title=Libnfc:configuration) page too.
# ChangeLog
ChangeLog is directly available from libnfc's repository: [https://github.com/nfc-tools/libnfc/blob/master/ChangeLog](https://github.com/nfc-tools/libnfc/blob/master/ChangeLog)
If you want more detailed change log, you can use the repository log: [https://github.com/nfc-tools/libnfc/commits/master](https://github.com/nfc-tools/libnfc/commits/master)
# Contribute
There are many ways to contribute to libnfc, including but not limited to:
- reporting issues on [libnfc issues tracker](https://github.com/nfc-tools/libnfc/issues)
- providing patches by one of those ways:
- on [libnfc issues tracker](https://github.com/nfc-tools/libnfc/issues)
- using [nfc-tools developers discussion group](https://groups.google.com/d/forum/nfc-tools-devel)
- forking the git repository: [go here](https://github.com/nfc-tools/libnfc/commits/master),
 press the "Fork" button and commit patches to your own copy of libnfc and let us know about it in the issues tracker on the discussion group or push-request's GitHub feature.
- helping us to extend [tests coverage](http://nfc-tools.org/index.php?title=Libnfc:Testing)
# Sponsoring
You can also sponsor us with hardware donations, to do so please contact [romuald@libnfc.org](mailto:romuald@libnfc.org).
If you are manufacturer and you want your device to be supported in libnfc, feel free to send a mail to [romuald@libnfc.org](mailto:romuald@libnfc.org).
Thanks to manufacturers which already done it, see [sponsor
 page](http://nfc-tools.org/index.php?title=Sponsors).
[Categories](http://nfc-tools.org/index.php?title=Special:Categories): 
- [Libnfc](http://nfc-tools.org/index.php?title=Category:Libnfc)
- [Software](http://nfc-tools.org/index.php?title=Category:Software)
