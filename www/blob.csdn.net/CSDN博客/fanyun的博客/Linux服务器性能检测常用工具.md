# Linux服务器性能检测常用工具 - fanyun的博客 - CSDN博客
2019年02月23日 22:10:31[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：64
1.hdparm
hdparm是一个命令行工具，用于读取和设置IDE或SCSI硬盘参数。可以用来测试磁盘I/O读取性能。
安装
# yum install -y hdparm
使用
关键参数-T和-t
-T
用于以基准测试和比较为目的的缓存读取计时。要得到有意义的结果，应该在内存不少于2M，系统没有其它活动(没有其它活动的程序) 的条件下，重复操作2-3次。
它显示了不存取磁盘直接从Linux缓存读取数据的速度。该测试实际上检测了被测系统的处理器，缓存和内存的吞吐量。
如果标志"-t"也被指定，那么一个基于"-T"输出结果的修正量将被综合到"-t"操作报告的结果中。
-t
用于以基准测试和比较为目的的缓存读取计时。要得到有意义的结果，应该在内存不少于2M，系统没有其它活动(没有其它活动的程序)的条件下，重复操作2-3次。
它显示了不使用预先的数据缓冲从磁盘这项测量检测了Linux下没有任何文件系统开销时磁盘可以支持多快的连续数据读取。
为确保测量的精确，缓存在"-t"的过程中通过BLKFLSBUF控制被刷新。如果标志"-T"也被指定，那么一个基于"-T"数促结果的修正量将被综合到"-t"操作报告的结果中。
测试硬盘的读取速度
# hdparm -T /dev/sda
/dev/sda:
 Timing cached reads:   14866 MB in  2.00 seconds = 7441.38 MB/sec
# hdparm -t /dev/sda
/dev/sda:
 Timing buffered disk reads: 6146 MB in  3.00 seconds = 2047.04 MB/sec
# hdparm -tT /dev/sda
/dev/sda:
 Timing cached reads:   14794 MB in  2.00 seconds = 7404.78 MB/sec
 Timing buffered disk reads: 6422 MB in  3.00 seconds = 2140.42 MB/sec
其他常用命令
查看硬盘参数：
# hdparm /dev/sda
/dev/sda:
SG_IO: bad/missing sense data, sb[]:  70 00 05 00 00 00 00 0a 00 00 00 00 20 00 00 c0 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
 multcount     =  0 (off)
 readonly      =  0 (off)
 readahead     = 8192 (on)
 geometry      = 65270[柱面数]/255[磁头数]/63[扇区数], sectors = 1048576000[总扇区数], start = 0[起始扇区数]
检测硬盘的电源管理模式
# hdparm -C /dev/sda
/dev/sda:
SG_IO: bad/missing sense data, sb[]:  70 00 05 00 00 00 00 0a 00 00 00 00 20 00 00 c0 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
 drive state is:  standby
查询并设置硬盘多重扇区存取的扇区数，以增进硬盘的存取效率：
# hdparm  -m /dev/sda
/dev/sda:
SG_IO: bad/missing sense data, sb[]:  70 00 05 00 00 00 00 0a 00 00 00 00 20 00 00 c0 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
 multcount     =  0 (off)
 # hdparm -m 8 /dev/sda
查看当前硬盘写Cache状态
# hdparm -W  /dev/sda
/dev/sda:
SG_IO: bad/missing sense data, sb[]:  70 00 05 00 00 00 00 0a 00 00 00 00 20 00 00 c0 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
 write-caching = not supported
关闭硬盘的写Cache
# hdparm -W  0 /dev/sda
打开硬盘的写Cache
# hdparm -W  1 /dev/sda
查看帮助
# hdparm -h
hdparm  [options] [device ...]
device：设备文件，指定id驱动对应的设备文件名，可以通过 fdisk -l查看设备文件。例如: /dev/sda3
Options:
 -a   Get/set fs readahead
 -A   Get/set the drive look-ahead flag (0/1)
 -b   Get/set bus state (0 == off, 1 == on, 2 == tristate)
 -B   Set Advanced Power Management setting (1-255)
 -c   Get/set IDE 32-bit IO setting
 -C   Check drive power mode status
 -d   Get/set using_dma flag
 -D   Enable/disable drive defect management
 -E   Set cd/dvd drive speed
 -f   Flush buffer cache for device on exit
 -F   Flush drive write cache
 -g   Display drive geometry
 -h   Display terse usage information
 -H   Read temperature from drive (Hitachi only)
 -i   Display drive identification
 -I   Detailed/current information directly from drive
 -J   Get/set Western DIgital "Idle3" timeout for a WDC "Green" drive (DANGEROUS)
 -k   Get/set keep_settings_over_reset flag (0/1)
 -K   Set drive keep_features_over_reset flag (0/1)
 -L   Set drive doorlock (0/1) (removable harddisks only)
 -m   Get/set multiple sector count
 -M   Get/set acoustic management (0-254, 128: quiet, 254: fast)
 -n   Get/set ignore-write-errors flag (0/1)
 -N   Get/set max visible number of sectors (HPA) (VERY DANGEROUS)
 -p   Set PIO mode on IDE interface chipset (0,1,2,3,4,...)
 -P   Set drive prefetch count
 -q   Change next setting quietly
 -Q   Get/set DMA queue_depth (if supported)
 -r   Get/set device readonly flag (DANGEROUS to set)
 -R   Get/set device write-read-verify flag
 -s   Set power-up in standby flag (0/1) (DANGEROUS)
 -S   Set standby (spindown) timeout
 -t   Perform device read timings
 -T   Perform cache read timings
 -u   Get/set unmaskirq flag (0/1)
 -U   Obsolete
 -v   Use defaults; same as -acdgkmur for IDE drives
 -V   Display program version and exit immediately
 -w   Perform device reset (DANGEROUS)
 -W   Get/set drive write-caching flag (0/1)
 -x   Obsolete
 -X   Set IDE xfer mode (DANGEROUS)
 -y   Put drive in standby mode
 -Y   Put drive to sleep
 -z   Re-read partition table
 -Z   Disable Seagate auto-powersaving mode
 --dco-freeze      Freeze/lock current device configuration until next power cycle
 --dco-identify    Read/dump device configuration identify data
 --dco-restore     Reset device configuration back to factory defaults
 --direct          Use O_DIRECT to bypass page cache for timings
 --drq-hsm-error   Crash system with a "stuck DRQ" error (VERY DANGEROUS)
 --fallocate       Create a file without writing data to disk
 --fibmap          Show device extents (and fragmentation) for a file
 --fwdownload            Download firmware file to drive (EXTREMELY DANGEROUS)
 --fwdownload-mode3      Download firmware using min-size segments (EXTREMELY DANGEROUS)
 --fwdownload-mode3-max  Download firmware using max-size segments (EXTREMELY DANGEROUS)
 --fwdownload-mode7      Download firmware using a single segment (EXTREMELY DANGEROUS)
 --idle-immediate  Idle drive immediately
 --idle-unload     Idle immediately and unload heads
 --Istdin          Read identify data from stdin as ASCII hex
 --Istdout         Write identify data to stdout as ASCII hex
 --make-bad-sector Deliberately corrupt a sector directly on the media (VERY DANGEROUS)
 --offset          use with -t, to begin timings at given offset (in GiB) from start of drive
 --prefer-ata12    Use 12-byte (instead of 16-byte) SAT commands when possible
 --read-sector     Read and dump (in hex) a sector directly from the media
 --repair-sector   Alias for the --write-sector option (VERY DANGEROUS)
 --security-help   Display help for ATA security commands
 --trim-sector-ranges        Tell SSD firmware to discard unneeded data sectors: lba:count ..
 --trim-sector-ranges-stdin  Same as above, but reads lba:count pairs from stdin
 --verbose         Display extra diagnostics from some commands
 --write-sector    Repair/overwrite a (possibly bad) sector directly on the media (VERY DANGEROUS)
 -a<快取分区>：设定读取文件时，预先存入块区的分区数，若不加上<快取分区>选项，则显示目前的设定；
 -A<0或1>：启动或关闭读取文件时的快取功能；
 -c：设定IDE32位I/O模式；
 -C：检测IDE硬盘的电源管理模式；
 -d<0或1>：设定磁盘的DMA模式；
 -f：将内存缓冲区的数据写入硬盘，并清楚缓冲区；
 -g：显示硬盘的磁轨，磁头，磁区等参数；
 -h：显示帮助；
 -i：显示硬盘的硬件规格信息，这些信息是在开机时由硬盘本身所提供；
 -I：直接读取硬盘所提供的硬件规格信息；
 -k<0或1>：重设硬盘时，保留-dmu参数的设定；
 -K<0或1>：重设硬盘时，保留-APSWXZ参数的设定；
 -m<磁区数>：设定硬盘多重分区存取的分区数；
 -n<0或1>：忽略硬盘写入时所发生的错误；
 -p：设定硬盘的PIO模式；
 -P<磁区数>：设定硬盘内部快取的分区数；
 -q：在执行后续的参数时，不在屏幕上显示任何信息；
 -r<0或1>：设定硬盘的读写模式；
 -S<时间>：设定硬盘进入省电模式前的等待时间；
 -t：评估硬盘的读取效率（不使用预先的数据缓冲，而从磁盘连续读取的速度）；
 -T：评估硬盘快取的读取效率（不读取磁盘，直接从Linux缓存读取数据的速度）；
 -u<0或1>：在硬盘存取时，允许其他中断要求同时执行；
 -v：显示硬盘的相关设定；
 -w<0或1>：设定硬盘的写入快取；
 -X<传输模式>：设定硬盘的传输模式；
 -y：使IDE硬盘进入省电模式；
 -Y：使IDE硬盘进入睡眠模式；
 -Z：关闭某些Seagate硬盘的自动省电功能。
2.dd
安装：Centos7默认系统自带。
# dd [bs=<字节数>][cbs=<字节数>][conv=<关键字>][count=<区块数>][ibs=<字节数>][if=<文件>][obs=<字节数>][of=<文件>][seek=<区块数>][skip=<区块数>][--help][--version]
参数说明：
  bs=<字节数>   将ibs( 输入)与obs(输出)设成指定的字节数。
  cbs=<字节数>   转换时，每次只转换指定的字节数。
  conv=<关键字>   指定文件转换的方式。
  count=<区块数>   仅读取指定的区块数。
  ibs=<字节数>   每次读取的字节数。
  if=<文件>   从文件读取。
  obs=<字节数>   每次输出的字节数。
  of=<文件>   输出到文件。
  seek=<区块数>   一开始输出时，跳过指定的区块数。
  skip=<区块数>   一开始读取时，跳过指定的区块数。
  --help   帮助。
  --version   显示版本信息。
 常用参数：
if=xxx      从xxx读取，如if=/dev/zero,该设备无穷尽地提供0（不产生读磁盘IO）。
of=xxx      向xxx写出，可以写文件，可以写裸设备。如of=/dev/null，"黑洞"，它等价于一个只写文件。所有写入它的内容都会永远丢失(不产生写磁盘IO)
bs=8k       每次读或写的大小，即一个块的大小。
count=xxx   读写块的总数量。
注意：避免操作系统“写缓存”干扰测试结果，使用sync、fsync、fdatasync。
测试磁盘的IO读速度
# df -m
Filesystem              1M-blocks  Used Available Use% Mounted on
/dev/mapper/centos-root    477971  5492    472480   2% /
devtmpfs                     3903     0      3903   0% /dev
tmpfs                        3912     1      3912   1% /dev/shm
tmpfs                        3912    17      3896   1% /run
tmpfs                        3912     0      3912   0% /sys/fs/cgroup
/dev/sda1                    1014   133       882  14% /boot
使用dd指令，对磁盘进行连续写入，不使用内存缓冲区，每次写入2k的数据，总共写入100万次，产生2G大小的文件
# dd if=/dev/zero of=/test/test.dbf bs=2k count=1000000 conv=fdatasync
1000000+0 records in
1000000+0 records out
2048000000 bytes (2.0 GB) copied, 2.90199 s, 706 MB/s
测试磁盘的IO写速度
# time dd if=/dev/zero of=/test/test.dbf bs=2k count=1000000
1000000+0 records in
1000000+0 records out
2048000000 bytes (2.0 GB) copied, 2.24934 s, 910 MB/s
测试IO同时读和写的速度
# time dd if=/dev/sda1 of=/test/test.dbf bs=2k
524288+0 records in
524288+0 records out
1073741824 bytes (1.1 GB) copied, 1.31588 s, 816 MB/s
real    0m1.317s
user    0m0.042s
sys     0m1.275s
3.sysbench
CPU性能测试
# sysbench --test=cpu --cpu-max-prime=20000 --num-threads=8 run
内存性能测试
# sysbench --test=memory --num-threads=8 --memory-block-size=8192 --memory-total-size=2G run
磁盘I/O性能测试
# sysbench --test=fileio --num-threads=8 --file-rw-ratio=5 --file-total-size=500M --file-test-mode=rndrw prepare
# sysbench --test=fileio --num-threads=8 --file-rw-ratio=5 --file-total-size=500M --file-test-mode=rndrw run
清除文件
# sysbench --test=fileio --num-threads=8 --file-rw-ratio=5 --file-total-size=500M cleanup
# sysbench --test=fileio --file-num=16 --file-total-size=2G prepare
# sysbench --test=fileio --file-total-size=2G --file-test-mode=rndrd --max-time=180 --max-requests=100000000 --num-threads=16 --init-rng=on --file-num=16 --file-extra-flags=direct --file-fsync-freq=0 --file-block-size=16384 run
清除文件
# sysbench --test=fileio --file-num=16 --file-total-size=2G cleanup
4.CPU简单测试
CPU单线程简单测试
# cat simpleTestForCPU.sh 
#!/bin/sh
date
echo "scale=5000; 4*a(1)" | bc -l -q
date
5.sar
sar是Centos系统自带的命令，如果默认没有安装，可以通过执行下面命令安装
# yum -y install sysstat
统计内存使用情况，间隔1秒，统计3次
# sar -r 1 3  
