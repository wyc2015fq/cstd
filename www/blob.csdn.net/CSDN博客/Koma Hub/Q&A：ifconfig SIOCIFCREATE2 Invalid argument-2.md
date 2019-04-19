# Q&A：ifconfig: SIOCIFCREATE2: Invalid argument - 2 - Koma Hub - CSDN博客
2019年01月26日 19:05:57[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：239
个人分类：[CentOS																[Linux																[Socket																[计算机网络](https://blog.csdn.net/Rong_Toa/article/category/7569489)](https://blog.csdn.net/Rong_Toa/article/category/7792858)](https://blog.csdn.net/Rong_Toa/article/category/7156254)](https://blog.csdn.net/Rong_Toa/article/category/8122413)
[https://sourceforge.net/p/tuntaposx/mailman/message/30457237/](https://sourceforge.net/p/tuntaposx/mailman/message/30457237/)
When I do "ifconfig tap0 create" I get the response "ifconfig: SIOCIFCREATE2: Invalid argument"
The overlying thing I want to get working is that I am a user of GNS3 on a mac.
There's a way to connect the GNS3 network topology to a cloud with a tap0 interface, then the virtual labs can communicate through that to the macbook's networks and thereby gain LAN/WAN access (for real, not virtual).
So, that's my objective, tap0 interface being the key to it.
When I create the "cloud" in GNS3 and assign it the tap0 interface, it also complains of failure to create the interface, which is what started me focusing on the CLI and attempting to create it before launching GNS3 to use it.
It seems at least for 10.8 Mountain Lion that tuntap doesn't exactly fail to load as a kernel extension, but it has so far failed to provide a kernel-level usable set of interfaces, at least from my experiences using the tgz binary or from source.
Thanks everyone! 
-jre
On Feb 6, 2013, at 8:52 PM, Kris Hepler <khepler@...> wrote:
> Your ifconfig syntax isn't quite correct.  I don't have access to a
> 10.8 system, but if memory serves, the following should help:
> 
> To create an interface it's simply "ifconfig tap0 create".  You can
> follow that with an "ifconfig inet 192.168.20.1 255.255.255.0" to set
> an address, and then "ifconfig tap0 up" to bring it up.  You can also
> try combining all three commands into "ifconfig tap0 create inet
> 192.168.20.1 255.255.2550 up", but I'm not sure you can combine the
> create command with anything else.  When you finish using the
> interface, clean up with "ifconfig tap0 destroy".
> 
> Depending on what you want to do, however, you may not need create the
> interface yourself.  Most VPN software create interfaces
> automatically.  If you're just playing with TAP interfaces, keep
> reading "man ifconfig" until the syntax makes sense, and then maybe
> move on to "man route" if you want to tunnel traffic.
> 
> 
> On Feb 6, 2013, at 1:50 PM, "Joseph R. Erlewein" <joe@...> wrote:
> 
>> Hello,
>> I downloaded the tgz  tuntap_20111101.pkg and it seemed to install OK.
>> kext's seemed to load OK with no errors in system.log, etc.
>> 
>> Lots of /dev/tapx devices.
>> 
>> However, ifconfig -a didn't show anything, I could not configure using ifconfig tap0 up, ifconfig tap0 create 192.168.20.1 255.255.255.0 or ifconfig tap0 192.168.20.1.255.255.255.0.
>> 
>> Additionally I downloaded source, built and installed ok (make pkg did not work, but i had already done make test, make and make install so I neglected to build a pkg for later when i hit this issue) and THAT even seemed to load ok also with no errors, but same as above, ifconfig is unable to create/configure the interface:
>> 
>> user@...:~ $ sudo kextload /Library/Extensions/tap.kext
>> Password:
>> user@...:~ $ sudo ifconfig tap0 up
>> ifconfig: interface tap0 does not exist
>> user@...:~ $ sudo ifconfig tap0 create 192.168.20.1 255.255.255.0
>> ifconfig: SIOCIFCREATE2: Invalid argument
>> user@...:~ $ sudo ifconfig tap0 192.168.20.1 255.255.255.0
>> ifconfig: interface tap0 does not exist
>> user@...:~ $ sudo ifconfig tap0
>> ifconfig: interface tap0 does not exist
>> user@...:~ $ sudo ls /dev/tap
>> tap0   tap10  tap12  tap14  tap2   tap4   tap6   tap8
>> tap1   tap11  tap13  tap15  tap3   tap5   tap7   tap9
>> user@...:~ $ sudo ls /dev/tap
>> 
>> 
>> Any ideas?
>> 
>> Thanks -
>> 
>> -jre
>> 
>> 
>> ------------------------------------------------------------------------------
>> Free Next-Gen Firewall Hardware Offer
>> Buy your Sophos next-gen firewall before the end March 2013
>> and get the hardware for free! Learn more.
>> [http://p.sf.net/sfu/sophos-d2d-feb](http://p.sf.net/sfu/sophos-d2d-feb)
### Thread view
Hello,
I downloaded the tgz  tuntap_20111101.pkg and it seemed to install OK.
kext's seemed to load OK with no errors in system.log, etc.
Lots of /dev/tapx devices.
However, ifconfig -a didn't show anything, I could not configure using ifconfig tap0 up, ifconfig tap0 create 192.168.20.1 255.255.255.0 or ifconfig tap0 192.168.20.1.255.255.255.0.
Additionally I downloaded source, built and installed ok (make pkg did not work, but i had already done make test, make and make install so I neglected to build a pkg for later when i hit this issue) and THAT even seemed to load ok also with no errors, but same as above, ifconfig is unable to create/configure the interface:
user@...:~ $ sudo kextload /Library/Extensions/tap.kext
Password:
user@...:~ $ sudo ifconfig tap0 up
ifconfig: interface tap0 does not exist
user@...:~ $ sudo ifconfig tap0 create 192.168.20.1 255.255.255.0
ifconfig: SIOCIFCREATE2: Invalid argument
user@...:~ $ sudo ifconfig tap0 192.168.20.1 255.255.255.0
ifconfig: interface tap0 does not exist
user@...:~ $ sudo ifconfig tap0
ifconfig: interface tap0 does not exist
user@...:~ $ sudo ls /dev/tap
tap0   tap10  tap12  tap14  tap2   tap4   tap6   tap8   
tap1   tap11  tap13  tap15  tap3   tap5   tap7   tap9   
user@...:~ $ sudo ls /dev/tap
Any ideas? 
Thanks -
-jre
Your ifconfig syntax isn't quite correct.  I don't have access to a
10.8 system, but if memory serves, the following should help:
To create an interface it's simply "ifconfig tap0 create".  You can
follow that with an "ifconfig inet 192.168.20.1 255.255.255.0" to set
an address, and then "ifconfig tap0 up" to bring it up.  You can also
try combining all three commands into "ifconfig tap0 create inet
192.168.20.1 255.255.2550 up", but I'm not sure you can combine the
create command with anything else.  When you finish using the
interface, clean up with "ifconfig tap0 destroy".
Depending on what you want to do, however, you may not need create the
interface yourself.  Most VPN software create interfaces
automatically.  If you're just playing with TAP interfaces, keep
reading "man ifconfig" until the syntax makes sense, and then maybe
move on to "man route" if you want to tunnel traffic.
On Feb 6, 2013, at 1:50 PM, "Joseph R. Erlewein" <joe@...> wrote:
> Hello,
> I downloaded the tgz  tuntap_20111101.pkg and it seemed to install OK.
> kext's seemed to load OK with no errors in system.log, etc.
>
> Lots of /dev/tapx devices.
>
> However, ifconfig -a didn't show anything, I could not configure using ifconfig tap0 up, ifconfig tap0 create 192.168.20.1 255.255.255.0 or ifconfig tap0 192.168.20.1.255.255.255.0.
>
> Additionally I downloaded source, built and installed ok (make pkg did not work, but i had already done make test, make and make install so I neglected to build a pkg for later when i hit this issue) and THAT even seemed to load ok also with no errors, but same as above, ifconfig is unable to create/configure the interface:
>
> user@...:~ $ sudo kextload /Library/Extensions/tap.kext
> Password:
> user@...:~ $ sudo ifconfig tap0 up
> ifconfig: interface tap0 does not exist
> user@...:~ $ sudo ifconfig tap0 create 192.168.20.1 255.255.255.0
> ifconfig: SIOCIFCREATE2: Invalid argument
> user@...:~ $ sudo ifconfig tap0 192.168.20.1 255.255.255.0
> ifconfig: interface tap0 does not exist
> user@...:~ $ sudo ifconfig tap0
> ifconfig: interface tap0 does not exist
> user@...:~ $ sudo ls /dev/tap
> tap0   tap10  tap12  tap14  tap2   tap4   tap6   tap8
> tap1   tap11  tap13  tap15  tap3   tap5   tap7   tap9
> user@...:~ $ sudo ls /dev/tap
>
>
> Any ideas?
>
> Thanks -
>
> -jre
>
>
> ------------------------------------------------------------------------------
> Free Next-Gen Firewall Hardware Offer
> Buy your Sophos next-gen firewall before the end March 2013
> and get the hardware for free! Learn more.
> [http://p.sf.net/sfu/sophos-d2d-feb](http://p.sf.net/sfu/sophos-d2d-feb)
When I do "ifconfig tap0 create" I get the response "ifconfig: SIOCIFCREATE2: Invalid argument"
The overlying thing I want to get working is that I am a user of GNS3 on a mac.
There's a way to connect the GNS3 network topology to a cloud with a tap0 interface, then the virtual labs can communicate through that to the macbook's networks and thereby gain LAN/WAN access (for real, not virtual).
So, that's my objective, tap0 interface being the key to it.
When I create the "cloud" in GNS3 and assign it the tap0 interface, it also complains of failure to create the interface, which is what started me focusing on the CLI and attempting to create it before launching GNS3 to use it.
It seems at least for 10.8 Mountain Lion that tuntap doesn't exactly fail to load as a kernel extension, but it has so far failed to provide a kernel-level usable set of interfaces, at least from my experiences using the tgz binary or from source.
Thanks everyone! 
-jre
On Feb 6, 2013, at 8:52 PM, Kris Hepler <khepler@...> wrote:
> Your ifconfig syntax isn't quite correct.  I don't have access to a
> 10.8 system, but if memory serves, the following should help:
> 
> To create an interface it's simply "ifconfig tap0 create".  You can
> follow that with an "ifconfig inet 192.168.20.1 255.255.255.0" to set
> an address, and then "ifconfig tap0 up" to bring it up.  You can also
> try combining all three commands into "ifconfig tap0 create inet
> 192.168.20.1 255.255.2550 up", but I'm not sure you can combine the
> create command with anything else.  When you finish using the
> interface, clean up with "ifconfig tap0 destroy".
> 
> Depending on what you want to do, however, you may not need create the
> interface yourself.  Most VPN software create interfaces
> automatically.  If you're just playing with TAP interfaces, keep
> reading "man ifconfig" until the syntax makes sense, and then maybe
> move on to "man route" if you want to tunnel traffic.
> 
> 
> On Feb 6, 2013, at 1:50 PM, "Joseph R. Erlewein" <joe@...> wrote:
> 
>> Hello,
>> I downloaded the tgz  tuntap_20111101.pkg and it seemed to install OK.
>> kext's seemed to load OK with no errors in system.log, etc.
>> 
>> Lots of /dev/tapx devices.
>> 
>> However, ifconfig -a didn't show anything, I could not configure using ifconfig tap0 up, ifconfig tap0 create 192.168.20.1 255.255.255.0 or ifconfig tap0 192.168.20.1.255.255.255.0.
>> 
>> Additionally I downloaded source, built and installed ok (make pkg did not work, but i had already done make test, make and make install so I neglected to build a pkg for later when i hit this issue) and THAT even seemed to load ok also with no errors, but same as above, ifconfig is unable to create/configure the interface:
>> 
>> user@...:~ $ sudo kextload /Library/Extensions/tap.kext
>> Password:
>> user@...:~ $ sudo ifconfig tap0 up
>> ifconfig: interface tap0 does not exist
>> user@...:~ $ sudo ifconfig tap0 create 192.168.20.1 255.255.255.0
>> ifconfig: SIOCIFCREATE2: Invalid argument
>> user@...:~ $ sudo ifconfig tap0 192.168.20.1 255.255.255.0
>> ifconfig: interface tap0 does not exist
>> user@...:~ $ sudo ifconfig tap0
>> ifconfig: interface tap0 does not exist
>> user@...:~ $ sudo ls /dev/tap
>> tap0   tap10  tap12  tap14  tap2   tap4   tap6   tap8
>> tap1   tap11  tap13  tap15  tap3   tap5   tap7   tap9
>> user@...:~ $ sudo ls /dev/tap
>> 
>> 
>> Any ideas?
>> 
>> Thanks -
>> 
>> -jre
>> 
>> 
>> ------------------------------------------------------------------------------
>> Free Next-Gen Firewall Hardware Offer
>> Buy your Sophos next-gen firewall before the end March 2013
>> and get the hardware for free! Learn more.
>> [http://p.sf.net/sfu/sophos-d2d-feb](http://p.sf.net/sfu/sophos-d2d-feb)
ifconfig tapX create is _not_ the correct way to create network interfaces.
The idea is that interfaces only get created when a program opens the
corresponding /dev/tapX or /dev/tunX device. To try, do exec 3<>/dev/tap0
on a root shell.
I'm not sure how GNS3 is doing this, but you'll somehow need to teach it to
open /dev/tapX and read/write packets from there.
On Thu, Feb 7, 2013 at 8:03 PM, Mattias Nissler
<mattias.nissler@...>wrote:
> ifconfig tapX create is _not_ the correct way to create network interfaces.
>
> The idea is that interfaces only get created when a program opens the
> corresponding /dev/tapX or /dev/tunX device. To try, do exec 3<>/dev/tap0
> on a root shell.
>
> I'm not sure how GNS3 is doing this, but you'll somehow need to teach it
> to open /dev/tapX and read/write packets from there.
>
>
> On Thu, Feb 7, 2013 at 5:20 PM, Joe Erlewein <joe@...> wrote:
>
>> When I do "ifconfig tap0 create" I get the response "ifconfig:
>> SIOCIFCREATE2: Invalid argument"
>>
>>
>> The overlying thing I want to get working is that I am a user of GNS3 on
>> a mac.
>> There's a way to connect the GNS3 network topology to a cloud with a tap0
>> interface, then the virtual labs can communicate through that to the
>> macbook's networks and thereby gain LAN/WAN access (for real, not virtual).
>>
>> So, that's my objective, tap0 interface being the key to it.
>>
>> When I create the "cloud" in GNS3 and assign it the tap0 interface, it
>> also complains of failure to create the interface, which is what started me
>> focusing on the CLI and attempting to create it before launching GNS3 to
>> use it.
>>
>>
>> It seems at least for 10.8 Mountain Lion that tuntap doesn't exactly fail
>> to load as a kernel extension, but it has so far failed to provide a
>> kernel-level usable set of interfaces, at least from my experiences using
>> the tgz binary or from source.
>>
>>
>> Thanks everyone!
>>
>> -jre
>>
>> On Feb 6, 2013, at 8:52 PM, Kris Hepler <khepler@...> wrote:
>>
>> > Your ifconfig syntax isn't quite correct.  I don't have access to a
>> > 10.8 system, but if memory serves, the following should help:
>> >
>> > To create an interface it's simply "ifconfig tap0 create".  You can
>> > follow that with an "ifconfig inet 192.168.20.1 255.255.255.0" to set
>> > an address, and then "ifconfig tap0 up" to bring it up.  You can also
>> > try combining all three commands into "ifconfig tap0 create inet
>> > 192.168.20.1 255.255.2550 up", but I'm not sure you can combine the
>> > create command with anything else.  When you finish using the
>> > interface, clean up with "ifconfig tap0 destroy".
>> >
>> > Depending on what you want to do, however, you may not need create the
>> > interface yourself.  Most VPN software create interfaces
>> > automatically.  If you're just playing with TAP interfaces, keep
>> > reading "man ifconfig" until the syntax makes sense, and then maybe
>> > move on to "man route" if you want to tunnel traffic.
>> >
>> >
>> > On Feb 6, 2013, at 1:50 PM, "Joseph R. Erlewein" <joe@...>
>> wrote:
>> >
>> >> Hello,
>> >> I downloaded the tgz  tuntap_20111101.pkg and it seemed to install OK.
>> >> kext's seemed to load OK with no errors in system.log, etc.
>> >>
>> >> Lots of /dev/tapx devices.
>> >>
>> >> However, ifconfig -a didn't show anything, I could not configure using
>> ifconfig tap0 up, ifconfig tap0 create 192.168.20.1 255.255.255.0 or
>> ifconfig tap0 192.168.20.1.255.255.255.0.
>> >>
>> >> Additionally I downloaded source, built and installed ok (make pkg did
>> not work, but i had already done make test, make and make install so I
>> neglected to build a pkg for later when i hit this issue) and THAT even
>> seemed to load ok also with no errors, but same as above, ifconfig is
>> unable to create/configure the interface:
>> >>
>> >> user@...:~ $ sudo kextload /Library/Extensions/tap.kext
>> >> Password:
>> >> user@...:~ $ sudo ifconfig tap0 up
>> >> ifconfig: interface tap0 does not exist
>> >> user@...:~ $ sudo ifconfig tap0 create 192.168.20.1
>> 255.255.255.0
>> >> ifconfig: SIOCIFCREATE2: Invalid argument
>> >> user@...:~ $ sudo ifconfig tap0 192.168.20.1
>> 255.255.255.0
>> >> ifconfig: interface tap0 does not exist
>> >> user@...:~ $ sudo ifconfig tap0
>> >> ifconfig: interface tap0 does not exist
>> >> user@...:~ $ sudo ls /dev/tap
>> >> tap0   tap10  tap12  tap14  tap2   tap4   tap6   tap8
>> >> tap1   tap11  tap13  tap15  tap3   tap5   tap7   tap9
>> >> user@...:~ $ sudo ls /dev/tap
>> >>
>> >>
>> >> Any ideas?
>> >>
>> >> Thanks -
>> >>
>> >> -jre
>> >>
>> >>
>> >>
>> ------------------------------------------------------------------------------
>> >> Free Next-Gen Firewall Hardware Offer
>> >> Buy your Sophos next-gen firewall before the end March 2013
>> >> and get the hardware for free! Learn more.
>> >> [http://p.sf.net/sfu/sophos-d2d-feb](http://p.sf.net/sfu/sophos-d2d-feb)
>>
>>
>>
>> ------------------------------------------------------------------------------
>> Free Next-Gen Firewall Hardware Offer
>> Buy your Sophos next-gen firewall before the end March 2013
>> and get the hardware for free! Learn more.
>> [http://p.sf.net/sfu/sophos-d2d-feb](http://p.sf.net/sfu/sophos-d2d-feb)
>>
>
>
