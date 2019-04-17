# Ubuntu 安装软件和centos 对比命令 - DoubleLi - 博客园






之前都是使用Redhat 或者Centos 等rpm的linux  ，开始使用ubuntu 很不习惯

1.  安装命令
Centos  : yum install httpd                  Ubuntu:  sudo apt-get install apache2  或者sudo aptitude install apache2
              yum list
              yum search                                      sudo apt-cache search


2. 安装包管理
Centos ： rpm -ql   httpd  查看rpm包安装位置
               rpm -qf  文件     查看文件所属rpm包
               rpm -qa|grep     查看是否安装了相应的rpm包

Ubuntu ： dpkg -l |grep     类似rpm -qa|grep
                dpkg-query -l git-core gnupg flex bison gperf build-essential zip curl zlib1g-dev libc6-dev lib32ncurses5-dev ia32-libs x11proto-core-dev libx11-dev lib32readline5-dev lib32z-dev libgl1-mesa-dev g++-multilib mingw32 tofrodos linux-libc-dev g++-4.4-multilib libc6-dev-i386 lib32z1-dev wine1.2 subversion

+++-======================================-======================================-============================================================================================
ii  bison                                  1:2.4.1.dfsg-3                         A parser generator that is compatible with YACC
ii  build-essential                        11.4build1                             Informational list of build-essential packages
ii  curl                                   7.19.7-1ubuntu1.3                      Get a file from an HTTP, HTTPS or FTP server
ii  flex                                   2.5.35-9                               A fast lexical analyzer generator.
ii  g++-4.4-multilib                       4.4.3-4ubuntu5.1                       The GNU C++ compiler (multilib files)
ii  g++-multilib                           4:4.4.3-1ubuntu1                       The GNU C++ compiler (multilib files)
ii  git-core                               1:1.7.0.4-1ubuntu0.2                   fast, scalable, distributed revision control system
ii  gnupg                                  1.4.10-2ubuntu1.4                      GNU privacy guard - a free PGP replacement
ii  gperf                                  3.0.3-1build3                          Perfect hash function generator
ii  ia32-libs                              2.7ubuntu26.1                          ia32 shared libraries for use on amd64 and ia64 systems
ii  lib32ncurses5-dev                      5.7+20090803-2ubuntu3                  developer's libraries for ncurses (32-bit)
ii  lib32readline5-dev                     5.2-7build1                            GNU readline and history libraries, development files (32-bit)
un  lib32z-dev                                                              (no description available)
ii  lib32z1-dev                            1:1.2.3.3.dfsg-15ubuntu1               compression library - 32 bit development
ii  libc6-dev                              2.11.1-0ubuntu7.12                     Embedded GNU C Library: Development Libraries and Header Files
ii  libc6-dev-i386                         2.11.1-0ubuntu7.12                     GNU C Library: 32-bit development libraries for AMD64
ii  libgl1-mesa-dev                        7.7.1-1ubuntu3.1                       A free implementation of the OpenGL API -- GLX development files
ii  libx11-dev                             2:1.3.2-1ubuntu3.1                     X11 client-side library (development headers)
ii  linux-libc-dev                         2.6.32-51.113                          Linux Kernel Headers for development
ii  mingw32                                4.2.1.dfsg-1ubuntu2                    Minimalist GNU win32 (cross) compiler
ii  subversion                             1.6.6dfsg-2ubuntu1.3                   Advanced version control system
ii  tofrodos                               1.7.8.debian.1-2                       Converts DOS <-> Unix text files, alias tofromdos
ii  wine1.2                                1.2.2-0ubuntu2~lucid1                  Microsoft Windows Compatibility Layer (Binary Emulator and Library)
ii  x11proto-core-dev                      7.0.16-1                               X11 core wire protocol and auxiliary headers
ii  zip                                    3.0-2                                  Archiver for .zip files
ii  zlib1g-dev                             1:1.2.3.3.dfsg-15ubuntu1               compression library - development

 以上命令类似rpm -ql


dpkg-query -L 包名  类似于rpm -ql  可以用来查看安装包都包含哪些内容都安装到哪里了









