# u-boot裁减 - maopig的专栏 - CSDN博客
2011年11月16日 22:46:38[maopig](https://me.csdn.net/maopig)阅读数：1261标签：[flash																[application																[variables																[file																[网络																[cmd](https://so.csdn.net/so/search/s.do?q=cmd&t=blog)](https://so.csdn.net/so/search/s.do?q=网络&t=blog)](https://so.csdn.net/so/search/s.do?q=file&t=blog)](https://so.csdn.net/so/search/s.do?q=variables&t=blog)](https://so.csdn.net/so/search/s.do?q=application&t=blog)](https://so.csdn.net/so/search/s.do?q=flash&t=blog)
个人分类：[u-boot/驱动开发/linux](https://blog.csdn.net/maopig/article/category/930963)
[http://blog.csdn.net/menuconfig/article/details/2276078](http://blog.csdn.net/menuconfig/article/details/2276078)
u-boot裁减的主要方法是通过删除不需要的命令来减小可执行代码体积。比如说，如果不需要网络相关的功能，则可以将网络相关的所有命令全部删除，可以节约可观的flash空间。一般u-boot的大小为100k左右，通过删除命令（如果删除网络相关的命令），可以裁减到64k以内，只保留一些经常使用的命令。
另外，如果需要动态修改环境变量，环境变量还需要单独分配一个flash扇区用于保存。这样大约就需要64k+64k的flash空间。如果我们不需要动态修改环境变量，可以省去一个扇区的Flash开销。
以下是本人裁减u-boot的基本步骤：
1，删除除默认命令以外的命令：
删除include/configs/naribf533.h中CONFIG_COMMANDS的定义，直接采样默认的命令。可以将u-boot从108k裁减到79k。
2，删除其他不需要的命令，只保留常用命令：
通过修改include/cmd_confdefs.h中的CFG_CMD_NONSTD常量，进一步裁减u-boot命令。CFG_CMD_NONSTD为“non-standard”命令，除了这个常量定义的命令外，其余都是常用命令。我们可以通过在这个常量中添加不需要的命令来减少默认命令的数量。我们这里只保留以下命令，其余全部裁减掉：
?       - alias for 'help'
autoscr - run script from memory
base    - print or set address offset
bdinfo - print Board Info structure
bootm   - boot application image from memory
cmp     - memory compare
coninfo - print console devices and information
cp      - memory copy
crc32   - checksum calculation
echo    - echo args to console
erase   - erase FLASH memory
fl      - flush a file to FLASH memory
flinfo - print FLASH memory information
go      - start application at address 'addr'
help    - print online help
loadb   - load binary file over serial line (kermit mode)
loads   - load S-Record file over serial line
loop    - infinite loop on address range
md      - memory display
mm      - memory modify (auto-incrementing)
mtest   - simple RAM test
mw      - memory write (fill)
nm      - memory modify (constant address)
oc      - over clocking
printenv- print environment variables
protect - enable or disable FLASH write protection
reset   - Perform RESET of the CPU
run     - run commands in an environment variable
saveenv - save environment variables to persistent storage
setenv - set environment variables
sleep   - delay execution for some time
version - print monitor version
最终可以将uboot裁减到63k。
3，修改控制台返回信息和环境变量：
修改环境变量的存储扇区地址，并删除无用的环境变量或相应修改成对应于我们开发板的环境变量。
相应修改一些控制台返回信息。
4，如果不动态修改环境变量，建议删除saveenv命令，以免误擦除u-boot程序。同时将内核存放地址修改为20010000。
转自  [http://www.eefocus.com/myspace/blog/show_77755.html](http://www.eefocus.com/myspace/blog/show_77755.html)
