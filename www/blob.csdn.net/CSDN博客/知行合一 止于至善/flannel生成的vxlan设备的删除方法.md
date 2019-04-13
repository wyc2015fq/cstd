
# flannel生成的vxlan设备的删除方法 - 知行合一 止于至善 - CSDN博客

2019年03月27日 06:49:34[liumiaocn](https://me.csdn.net/liumiaocn)阅读数：230所属专栏：[深入浅出kubernetes](https://blog.csdn.net/column/details/12761.html)



flannel所生成的vxlan设备，删除的时候与docker0的删除方式不同，因为是vxlan设备，所以需要使用ip link del进行删除，本文对操作方法进行简单记录。
# 停止flanneld服务并确认
使用systemctl停止flanneld服务并确认结果
```python
[
```
```python
root@host131 ~
```
```python
]
```
```python
# systemctl stop flanneld
```
```python
[
```
```python
root@host131 ~
```
```python
]
```
```python
#
```
```python
[
```
```python
root@host131 ~
```
```python
]
```
```python
# systemctl status flanneld
```
```python
● flanneld.service - Flanneld Service
   Loaded: loaded
```
```python
(
```
```python
/usr/lib/systemd/system/flanneld.service
```
```python
;
```
```python
enabled
```
```python
;
```
```python
vendor preset: disabled
```
```python
)
```
```python
Active: inactive
```
```python
(
```
```python
dead
```
```python
)
```
```python
since Sun 2019-03-24 12:40:39 CST
```
```python
;
```
```python
5min ago
     Docs: https://github.com/coreos/flannel
  Process: 14059 ExecStart
```
```python
=
```
```python
/usr/local/bin/flanneld
```
```python
$FLANNELD_OPTS
```
```python
(
```
```python
code
```
```python
=
```
```python
exited, status
```
```python
=
```
```python
0/SUCCESS
```
```python
)
```
```python
Main PID: 14059
```
```python
(
```
```python
code
```
```python
=
```
```python
exited, status
```
```python
=
```
```python
0/SUCCESS
```
```python
)
```
```python
Mar 24 12:40:25 host131 flanneld
```
```python
[
```
```python
14059
```
```python
]
```
```python
: I0324 12:40:25.423725   14059 main.go:300
```
```python
]
```
```python
Wrote subnet
```
```python
file
```
```python
to /run/flannel/subnet.env
Mar 24 12:40:25 host131 flanneld
```
```python
[
```
```python
14059
```
```python
]
```
```python
: I0324 12:40:25.423747   14059 main.go:304
```
```python
]
```
```python
Running backend.
Mar 24 12:40:25 host131 flanneld
```
```python
[
```
```python
14059
```
```python
]
```
```python
: I0324 12:40:25.437930   14059 vxlan_network.go:60
```
```python
]
```
```python
watching
```
```python
for
```
```python
new subnet leases
Mar 24 12:40:25 host131 flanneld
```
```python
[
```
```python
14059
```
```python
]
```
```python
: I0324 12:40:25.438838   14059 main.go:396
```
```python
]
```
```python
Waiting
```
```python
for
```
```python
22h59m59.982339138s to renew lease
Mar 24 12:40:39 host131 systemd
```
```python
[
```
```python
1
```
```python
]
```
```python
: Stopping Flanneld Service
```
```python
..
```
```python
.
Mar 24 12:40:39 host131 flanneld
```
```python
[
```
```python
14059
```
```python
]
```
```python
: I0324 12:40:39.588573   14059 main.go:337
```
```python
]
```
```python
shutdownHandler sent cancel signal
```
```python
..
```
```python
.
Mar 24 12:40:39 host131 flanneld
```
```python
[
```
```python
14059
```
```python
]
```
```python
: I0324 12:40:39.588785   14059 main.go:404
```
```python
]
```
```python
Stopped monitoring lease
Mar 24 12:40:39 host131 flanneld
```
```python
[
```
```python
14059
```
```python
]
```
```python
: I0324 12:40:39.588801   14059 main.go:322
```
```python
]
```
```python
Waiting
```
```python
for
```
```python
all goroutines to
```
```python
exit
```
```python
Mar 24 12:40:39 host131 flanneld
```
```python
[
```
```python
14059
```
```python
]
```
```python
: I0324 12:40:39.588935   14059 main.go:325
```
```python
]
```
```python
Exiting cleanly
```
```python
..
```
```python
.
Mar 24 12:40:39 host131 systemd
```
```python
[
```
```python
1
```
```python
]
```
```python
: Stopped Flanneld Service.
```
```python
[
```
```python
root@host131 ~
```
```python
]
```
```python
#
```
# 使用ifconfig将设备停止
`[root@host131 ~]# ip addr s flannel.1
3: flannel.1: <BROADCAST,MULTICAST,UP,LOWER_UP> mtu 1450 qdisc noqueue state UNKNOWN group default 
    link/ether 8e:ef:b7:55:ed:43 brd ff:ff:ff:ff:ff:ff
    inet 172.200.240.0/32 scope global flannel.1
       valid_lft forever preferred_lft forever
    inet6 fe80::8cef:b7ff:fe55:ed43/64 scope link 
       valid_lft forever preferred_lft forever
[root@host131 ~]# ifconfig flannel.1 down
[root@host131 ~]# ip addr s flannel.1
3: flannel.1: <BROADCAST,MULTICAST> mtu 1450 qdisc noqueue state DOWN group default 
    link/ether 8e:ef:b7:55:ed:43 brd ff:ff:ff:ff:ff:ff
    inet 172.200.240.0/32 scope global flannel.1
       valid_lft forever preferred_lft forever
[root@host131 ~]#`
# 使用ip link del删除vxlan设备
```python
[
```
```python
root@host131 ~
```
```python
]
```
```python
# ip link del flannel.1
```
```python
[
```
```python
root@host131 ~
```
```python
]
```
```python
#
```
```python
[
```
```python
root@host131 ~
```
```python
]
```
```python
# ip addr s flannel.1
```
```python
Device
```
```python
"flannel.1"
```
```python
does not exist.
```
```python
[
```
```python
root@host131 ~
```
```python
]
```
```python
#
```
# 注意事项
docker0由于是网桥设备，所以可以使用brctl进行删除，具体命令为：
> brctl delbr docker0
如果使用此种方式删除flannel产生的vxlan设备，会得到如下提示：
```python
[
```
```python
root@host131 ~
```
```python
]
```
```python
# brctl delbr flannel.1
```
```python
can't delete bridge flannel.1: Operation not permitted
```
```python
[
```
```python
root@host131 ~
```
```python
]
```
```python
#
```
# 所使用package
如果没有相关命令，在centos下使用如下方式进行安装即可。
|命令|所在package|centos下安装命令|
|---|---|---|
|ifconfig|net-tools|yum install net-tools|
|brctl|bridge-utils|yum install bridge-utils|
# 参考文章
[https://github.com/coreos/flannel/issues/733](https://github.com/coreos/flannel/issues/733)

