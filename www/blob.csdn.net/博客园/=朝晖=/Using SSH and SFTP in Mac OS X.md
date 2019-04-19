# Using SSH and SFTP in Mac OS X - =朝晖= - 博客园
# [Using SSH and SFTP in Mac OS X](https://www.cnblogs.com/dhcn/p/7120946.html)
http://answers.stat.ucla.edu/groups/answers/wiki/7a848/
SH and SFTP are command line applications available from the Terminal (located at /Applications/Utilities/Terminal).
## SSH
SSH client is a program for logging into a remote machine and for executing commands on a remote machine.
### Connecting
**To connect using ssh type at the terminal:**
> ssh username@host_server
Username is your network account username and host_server is the remote server to which you're connecting.
## SFTP
SFTP is an interactive file transfer program, similar to ftp, which performs all operations over an encrypted ssh transport.
### Connecting
**To connect using sftp type at the terminal:**
> sftp username@host_server
Username is your network account username and host_server is the remote server to which you're connecting.
### Commands
**The commands available in SFTP are:**
|**Command**|**Description**|
|----|----|
|pwd|Print working directory of remote host|
|lpwd|Print working directory of local client|
|cd|Change directory on the remote host|
|lcd|Change directory on the local client|
|ls|List director on the remote host|
|lls|List directory on the local client|
|mkdir|Make directory on remote host|
|lmkdir|Make directory on local client|
|get|Receive file from remote host to local client|
|put|Send file from local client to remote host|
|help|Display help text|
### 最后推荐一个支持SFTP的FTP GUI客户端：FileZilla
