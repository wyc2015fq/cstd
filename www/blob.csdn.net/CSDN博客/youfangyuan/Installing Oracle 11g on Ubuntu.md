# Installing Oracle 11g on Ubuntu - youfangyuan - CSDN博客
2013年01月20日 19:42:07[youfangyuan](https://me.csdn.net/youfangyuan)阅读数：461
                
To install the Oracle 11g database on Ubuntu, first download Ubuntu with a generic kernel from Ubuntu, and then release the link. Once the process is over, download Oracle version 11gr2 from the official Oracle link. X server is required to install Oracle on
 Ubuntu but before this disable the "Deny TCP connections to X server feature, to enable the server database to connect to the desktop. 
The X server restarts soon after. Once this is done, update all Ubuntu "anf" programming packs, then connect it through SSH and enter the root after the commands are given and then restart it again. Installing Oracle requires many packages by connecting through
 SSH. Go to root and give the command. Proceed to update the shell to bash and then make the required modifications in the configuration of the system users and along with that, modify some system files by adding and integrating, and lastly, validating some
 important parameters, such as the size of shared memory. It is recommended to restart the system or force the handling of parameters. Proceed with installing Oracle after completing the post-installation updates to the system configuration file. 
[Linux] Installing Oracle 11g on Ubuntu
Download UBUNTU
Download Oracle
X server configuration
Updating packages
Installing packages needed for pre-installation
Update shell
Update the configuration of system users
Updating the system configuration
Installing Oracle
Post-installation configuration
See also: Install oracle 11g on ubuntu
Download UBUNTU
The installation was made on a UBUNTU 8.0.4.1 with a 2.6.24-24-generic kernel available for download from the following link 
Download Oracle
The Oracle version used is 11gr2, available for download from this link 
X server configuration
An X server is required for this installation of Oracle. Note that we must disable the "Deny TCP connections to X server" feature to enable the server database to connect to the desktop. 
To disable this option from your desktop:
Go to System > Administration > Login Window
In the Security tab, uncheck the option Deny TCP connections to X server
Once this is done it will restart the X server.
Updating packages
We must update UBUNTU and all packages:
Connect via ssh
Switch to root:
#sudo -s
Launch the following commands:
#apt-get update        
#apt-get dist-upgrade
Restart:
#reboot
Installing packages needed for pre-installation
A number of packages are required for installing Oracle: 
build-essential, libaio1, gawk, ksh, libmotif3, alien, libtool, and lsb-rpm. 
To install them simply connect through ssh, switch to root and issue the command: 
#apt-get install build-essential libaio1 gawk ksh libmotif3 alien libtool lsb-rpm
Update shell
It is necessary to update the shell to bash. To do this, use the following code: 
# cd /bin        
# ls -l /bin/sh        
lrwxrwxrwx 1 root root 4 2008-04-28 19:59 /bin/sh -> dash        
# ln -sf bash /bin/sh        
# ls -l /bin/sh        
lrwxrwxrwx 1 root root 4 2008-05-01 22:51 /bin/sh -> bash
Update the configuration of system users
It is necessary to make some changes to system configuration:
Go to the root directory level:
# cd        
   # pwd        
 /root
Add groups: oinstall, dba, nobody
addgroup oinstall        
    Adding group 'oinstall' (GID 1001) ...        
 Done.        
# addgroup dba        
 Adding group 'dba' (GID 1002) ...        
 Done.        
# addgroup nobody        
    Adding group 'nobody' (GID 1003) ...        
    Done.        
# usermod -g nobody nobody
Add oracle user
# useradd -g oinstall -G dba -p password -d /home/oracle -s /bin/bash oracle        
# passwd oracle        
    Password changed.        
# mkdir /home/oracle        
# chown -R oracle:dba /home/oracle
Create symbolic links
# ln -s /usr/bin/awk /bin/awk        
# ln -s /usr/bin/rpm /bin/rpm        
# ln -s /usr/bin/basename /bin/basename        
# mkdir /etc/rc.d        
# for i in 0 1 2 3 4 5 6 S ; do ln -s /etc/rc$i.d /etc/rc.d/rc$i.d ; done
Create ORACLE_BASE
# mkdir -p /u01/app/oracle        
# chown -R oracle:dba /u01
Updating the system configuration
We must also make some changes to some system files by adding a number of parameters such as size of shared memory to the /etc/sysctl.conf file:
fs.file-max = 65535        
kernel.shmall = 2097152        
kernel.shmmax = 2147483648        
kernel.shmmni = 4096        
kernel.sem = 250 32000 100 128        
net.ipv4.ip_local_port_range = 1024 65535        
net.core.rmem_default = 1048576        
net.core.rmem_max = 1048576        
net.core.wmem_default = 262144        
net.core.wmem_max = 262144
Furthermore, we must integrate the following to the /etc/security/limits.confce file:
oracle soft nproc 2047        
oracle hard nproc 16383        
oracle soft nofile 1023        
oracle hard nofile 65535        
Next, add the following to /etc/pam.d/login to validate parameters /etc/security/limits.conf:
session required /lib/security/pam_limits.so        
session required pam_limits.so
Before proceeding to the next stage either restart the system, which is recommended, or force the handling of parameters added via the command sysctl-p:
sysctl -p:         
# sysctl -p        
kernel.printk = 4 4 1 7        
kernel.maps_protect = 1        
fs.inotify.max_user_watches = 524288        
vm.mmap_min_addr = 65536        
net.ipv4.conf.default.rp_filter = 1        
net.ipv4.conf.all.rp_filter = 1        
fs.file-max = 65535        
kernel.shmall = 2097152        
kernel.shmmax = 2147483648        
kernel.shmmni = 4096        
kernel.sem = 250 32000 100 128        
net.ipv4.ip_local_port_range = 1024 65535        
net.core.rmem_default = 1048576        
net.core.rmem_max = 1048576        
net.core.wmem_default = 262144        
net.core.wmem_max = 262144
Installing Oracle
Once the above steps are complete, simply log on as Oracle user and launch the downloaded Oracle runInstaller executable (at directory level) and follow the installation steps. 
Post-installation configuration
Once installation is complete, it will make some updates to the system configuration file:
Add the following lines to /etc/profile:
export ORACLE_HOME=/u01/app/oracle/product/11.X.Y/db_home1        
export PATH=$PATH:/u01/app/oracle/product/11.X.Y/db_home1/bin
Add the following environment variables:
export ORACLE_BASE=/u01/app/oracle        
export ORACLE_HOME=/u01/app/oracle/product/11.X.Y/db_home1        
export ORACLE_OWNR=oracle        
export PATH=$PATH:$ORACLE_HOME/bin
            
