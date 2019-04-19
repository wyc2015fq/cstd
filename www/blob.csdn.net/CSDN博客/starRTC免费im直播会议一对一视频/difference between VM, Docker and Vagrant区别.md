# difference between  VM, Docker and Vagrant区别 - starRTC免费im直播会议一对一视频 - CSDN博客
2017年06月29日 15:10:14[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：205
VM：
 VirtualBox, VMware
Docker
Vagrant
using which you can create VMs or container.
It
interacts with virtualization or Container platforms like VirtualBox, VMware or
 Docker to provision VMs or container.
vagrant cannot create VMs or containers without virtualization platforms like VirtualBox, VMware or Docker
The main advantage of vagrant is that it provides a mechanism to
reproduce
environments.
Once you write a VagrantFile to create the environment you desire then you can provide a copy of that VagrantFile
 to other people to create the exact replica of the environment you have created. 
 In the following figure VirtualBox is used as a platform to create VMs however Vagrant is used to configure
 the VM by setting OS version, Network address, Memory allocation, number of CPUs assigned etc.
[https://www.quora.com/Whats-the-difference-between-a-VM-Docker-and-Vagrant](https://www.quora.com/Whats-the-difference-between-a-VM-Docker-and-Vagrant)
