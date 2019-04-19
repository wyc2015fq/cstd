# Q&A：ifconfig clone error - "SIOCIFCREATE2: Invalid argument" - Koma Hub - CSDN博客
2019年01月26日 18:59:22[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：259
[https://lists.freebsd.org/pipermail/freebsd-questions/2008-July/178114.html](https://lists.freebsd.org/pipermail/freebsd-questions/2008-July/178114.html)
Hi there,
I've been playing with a new FB7.0 setup (using the standard iso
distribution), and trying to create some vlans by doing cloning via
the rc.conf "cloned_interfaces" command - but that fails.  (I'm using
bce interfaces on a HP bl760c blade, if that makes any difference)
Doing manual testing of "ifconfig xyz0 create" causes the error
"ifconfig: SIOCIFCREATE2: Invalid argument" error to come up and the
interface then doesn't get created.
Has anyone gotten this error at all, and how did you get it resolved?
My digging further shows that a patch was committed to
src/sys/net/if_clone.c (rev 1.11) which adds a new argument for
parameter data to the if_clone_create() function.
But it looks like src/sys/net/if.c has not been updated to provide
this new parameter data to the if_clone_create function call, thereby
failing the call, and not setting the interface up.
Any help or pointing in the right direction would be appreciated!
Thanks,
  Ross.
-- 
R> My digging further shows that a patch was committed to
R> src/sys/net/if_clone.c (rev 1.11) which adds a new argument for
R> parameter data to the if_clone_create() function.
Ignore my further digging (I blame the holidays), I was looking at
if_clone_createif - not if_clone_create, which has a different number
of parameters.  The function if_clone_create checks out fine.
The issue looks to be something within /src/sbin/ifconfig/ifclone.c
and the ioctl setup of the interface.  <sigh>
R.
-- 
Ross wrote:
>* Hi there,*>>* I've been playing with a new FB7.0 setup (using the standard iso*>* distribution), and trying to create some vlans by doing cloning via*>* the rc.conf "cloned_interfaces" command - but that fails.  (I'm using*>* bce interfaces on a HP bl760c blade, if that makes any difference)*>>* Doing manual testing of "ifconfig xyz0 create" causes the error*>* "ifconfig: SIOCIFCREATE2: Invalid argument" error to come up and the*>* interface then doesn't get created.*>what command are you using exactly? it certainly works here.
[root at crab](http://lists.freebsd.org/mailman/listinfo/freebsd-questions)
(20:30:15 <~>) 0 # ifconfig em1.5
ifconfig: interface em1.5 does not exist
[root at crab](http://lists.freebsd.org/mailman/listinfo/freebsd-questions)
(20:30:20 <~>) 0 # ifconfig em1.5 create
[root at crab](http://lists.freebsd.org/mailman/listinfo/freebsd-questions)
(20:30:25 <~>) 0 # ifconfig em1.5
em1.5: flags=8842<BROADCAST,RUNNING,SIMPLEX,MULTICAST> metric 0 mtu 1500
         options=3<RXCSUM,TXCSUM>
         ether 00:07:e9:17:db:c9
         media: Ethernet autoselect (100baseTX <full-duplex>)
         status: active
         vlan: 5 parent interface: em1
[root at crab](http://lists.freebsd.org/mailman/listinfo/freebsd-questions)
(20:30:26 <~>) 0 # ifconfig em1.5 destroy
Or
[root at crab](http://lists.freebsd.org/mailman/listinfo/freebsd-questions)
(20:30:55 <~>) 0 # ifconfig bge0.5 create
[root at crab](http://lists.freebsd.org/mailman/listinfo/freebsd-questions)
(20:31:04 <~>) 0 # ifconfig bge0.5
bge0.5: flags=8842<BROADCAST,RUNNING,SIMPLEX,MULTICAST> metric 0 mtu 1500
         options=3<RXCSUM,TXCSUM>
         ether 00:e0:81:2d:a3:16
         media: Ethernet autoselect (100baseTX <full-duplex>)
         status: active
         vlan: 5 parent interface: bge0
[root at crab](http://lists.freebsd.org/mailman/listinfo/freebsd-questions)
(20:31:16 <~>) 0 # ifconfig bge0.5 destroy
[root at crab](http://lists.freebsd.org/mailman/listinfo/freebsd-questions)
(20:31:20 <~>) 0 #
Vince
>* Has anyone gotten this error at all, and how did you get it resolved?*>>* My digging further shows that a patch was committed to*>* src/sys/net/if_clone.c (rev 1.11) which adds a new argument for*>* parameter data to the if_clone_create() function.*>>* But it looks like src/sys/net/if.c has not been updated to provide*>* this new parameter data to the if_clone_create function call, thereby*>* failing the call, and not setting the interface up.*>>* Any help or pointing in the right direction would be appreciated!*>>* Thanks,*>*   Ross.*>
VH> what command are you using exactly? it certainly works here.
The standard ones!  Which has gotten me confused greatly.
Using yours as an example:
-=
[[root at controller](http://lists.freebsd.org/mailman/listinfo/freebsd-questions) ~]# ifconfig bce0.5 create
ifconfig: SIOCIFCREATE2: Invalid argument
[[root at controller](http://lists.freebsd.org/mailman/listinfo/freebsd-questions) ~]#
and playing a bit:
[[root at controller](http://lists.freebsd.org/mailman/listinfo/freebsd-questions) ~]# ifconfig bce0.5 create vlandev bce0
ifconfig: must specify a tag for vlan create
[[root at controller](http://lists.freebsd.org/mailman/listinfo/freebsd-questions) ~]# ifconfig bce0.5 create vlandev bce0 vlan 5
ifconfig: SIOCIFCREATE2: Invalid argument
[[root at controller](http://lists.freebsd.org/mailman/listinfo/freebsd-questions) ~]# ifconfig bce0
bce0: flags=8843<UP,BROADCAST,RUNNING,SIMPLEX,MULTICAST> metric 0 mtu 1500
        options=1bb<RXCSUM,TXCSUM,VLAN_MTU,VLAN_HWTAGGING,JUMBO_MTU,VLAN_HWCSUM,TSO4>
        ether 00:1e:0b:c7:e1:d0
        inet w.x.y.z netmask 0xffffff00 broadcast w.x.y.z
        media: Ethernet autoselect (1000baseSX <full-duplex>)
        status: active
[[root at controller](http://lists.freebsd.org/mailman/listinfo/freebsd-questions) ~]#
-=
I'm wondering if it's because of the bce driver or something along
those lines, and ioctl isn't able to pick up the root device or
something.
The annoying part is that these are new servers/network cards on these
boxes, so I don't have an equivalent FB6.x setup to test against.
(Other boxes running 6.x are working just fine).
R.
-- 
>>* Doing manual testing of "ifconfig xyz0 create" causes the error*>>* "ifconfig: SIOCIFCREATE2: Invalid argument" error to come up and the*>>* interface then doesn't get created.*
VH> what command are you using exactly? it certainly works here.
After enough tinkering around, I've found the issue.
Looks like I need to issue a "ifconfig bce0 -vlanhwtag" command to the
interface before issuing the vlan "ifconfig bce0.10 create" command.
Surprisingly, it looks like I can issue a "ifconfig bce0 vlanhwtag"
command afterwards, and everything is good.
Very weird.  But I'm happy I found a work around for the time being.
R.
-- 
CentOS的/usr/include里面没有**SIOCIFCREATE**
