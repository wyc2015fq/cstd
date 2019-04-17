# ifconfig报错：SIOCSIFFLAGS: Operation not permitted - DoubleLi - 博客园






# insmod mt7601Usta.ko 

rtusb init rt2870 --->
usbcore: registered new interface driver rt2870



# iwconfig
lo        no wireless extensions.

eth0      no wireless extensions.

ra0       Ralink STA  ESSID:"11n-AP"  Nickname:"MT7601STA"
          Mode:Auto  Frequency=2.412 GHz  Access Point: Not-Associated   
          Bit Rate:1 Mb/s   
          RTS thr:off   Fragment thr:off
          Encryption key:off
          Link Quality=10/100  Signal level:0 dBm  Noise level:0 dBm
          Rx invalid nwid:0  Rx invalid crypt:0  Rx invalid frag:0
          Tx excessive retries:0  Invalid misc:0   Missed beacon:0



# ifconfig ra0 up
unlink cmd rsp urb
ifconfig: SIOCSIFFLAGS: Operation not permitted



原因：/etc/Wireless/RT2870STA/RT2870STA.dat 该文件找不到导致的。



解决：将驱动对应的dat文件拷贝对应目录，就没问题了

$cp RT2870STA.dat  /etc/Wireless/RT2870STA/RT2870STA.dat









