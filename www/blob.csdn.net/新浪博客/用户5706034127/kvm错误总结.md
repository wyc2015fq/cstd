# kvm错误总结_用户5706034127_新浪博客
|||
1，无法链接到/var/run/libvirt/libvirt-sock，解决方案如下所示，可能是权限，也可能是服务没哟启动。
![kvm错误总结](http://s9.sinaimg.cn/mw690/006e9W8vzy7cTBCWsL688&690)
2，无法链接到，用户目录下的，注意和前一个区别，是用户目录下的，libvirt-sock
![kvm错误总结](http://s11.sinaimg.cn/mw690/006e9W8vzy7cTBCNvAK5a&690)
这个原因看virsh命令的   -c
 就知道，没有指定到正确的URI解决方案如下：
所有命令加上uri即可：：virsh -c qemu:///system list --all
 即可。
![kvm错误总结](http://s6.sinaimg.cn/mw690/006e9W8vzy7cTBwejo9d5&690)
![kvm错误总结](http://s16.sinaimg.cn/mw690/006e9W8vzy7cTBw8WT51f&690)
具体本地配置 certificates，可以搜索上面的关键词，解决方案。
即便是virt-install命令也是可以制定uri的，，，如：：
virt-install  --connect=qeum:///system
![kvm错误总结](http://s7.sinaimg.cn/mw690/006e9W8vzy7cTRwAm7Y46&690)
3，virsh define 失败，找不到emulator，，
![kvm错误总结](http://s6.sinaimg.cn/mw690/006e9W8vzy7cTBw5b6da5&690)
在不同的机器上，emulator的名字和位置都不一样，所以解决方案如下：
![kvm错误总结](http://s3.sinaimg.cn/mw690/006e9W8vzy7cTBw1BVo92&690)
![kvm错误总结](http://s8.sinaimg.cn/mw690/006e9W8vzy7cTBwkbMb57&690)
4，，或许是我无知，，竟然，kvm还不支持，ubuntu只支持，rh，醉了，那还叫上面虚拟机啊。
![kvm错误总结](http://s11.sinaimg.cn/mw690/006e9W8vzy7cTBvXVWi3a&690)
5，，，，好的博客
# Failed to connect to the hypervisor
：：[http://wiki.libvirt.org/page/Failed_to_connect_to_the_hypervisor](http://wiki.libvirt.org/page/Failed_to_connect_to_the_hypervisor)
