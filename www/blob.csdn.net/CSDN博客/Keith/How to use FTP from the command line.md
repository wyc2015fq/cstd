# How to use FTP from the command line. - Keith - CSDN博客





2014年03月05日 21:06:25[ke1th](https://me.csdn.net/u012436149)阅读数：797标签：[ftp](https://so.csdn.net/so/search/s.do?q=ftp&t=blog)








## How to use FTP from the command line.

FTP (File Transfer Protocol) allows you to transfer files between your PC and other Internet systems (hosts). You can transfer files, work with local or remote directories, rename and display files, and execute system commands. Before you start, you must know
 how to log on to the remote system and have a userid and password on that system. 

**Note**: Some systems allow anonymous ftp access. To use **anonymous ftp**, use *anonymous* as your userid and*your e-mail address* as the logon password.

### Logging Onto and Off of a Remote System

To begin using Microsoft's FTP client, Open a command prompt and switch to the destination directory (where you want the download file).

To start an FTP session, enter: **ftp host_name**

where hostname is the name or IP address of the remote system.

You will then be asked to enter your userid and password.

Once you have successfully logged onto a remote system, you will be able to use ftp commands to view a listing of files on the remote system and transfer files between the two systems.

**Example:** Download i386.exe (Windows NT 3.5 Resource Kit) from ftp://ftp.microsoft.com/bussys/winnt/winnt-public/reskit/nt35/i386 to C:\Temp\Download


- Open a command prompt. Enter CD C:\Temp\Download (assuming that directory exists). 

Enter: **ftp ftp.microsoft.com**


You should now see a prompt similar to this: 

Connected to ftp.microsoft.com. 

220 Microsoft FTP Service 

User (ftp.microsoft.com:(none)):





- For the userid, Enter: **anonymous**


You should see a prompt similar to this: 

331 Anonymous access allowed, send identity (e-mail name) as password. 

Password:





- Enter: **userid@domain.com** as the password at the "Password:" prompt. 

Note: Any e-mail address in a userid@domain.com format should work. You will not be able to see the password as you type it.





- To download i386.exe from the bussys/winnt/winnt-public/reskit/nt35/i386 directory, Enter: **get bussys/winnt/winnt-public/reskit/nt35/i386/i386.exe**


Note: You could have also used **ls** to view the directory and file names, **cd bussys/winnt/winnt-public/reskit/nt35/i386** to switch directories, and **get i386.exe** to download the file from within that directory.





- To end the FTP session, Enter: **quit***or***bye**.




Note: Once you have extracted the resource kit, you will have to expand individual files 

example: expand choice.ex_ choice.exe

### FTP Commands

For a list of FTP commands, at the "ftp>" prompt, Enter: **help**
When using ftp from the command prompt, the following list of supported commands will be displayed: 

Note: Hover your mouse over a command to see what the output of "help *" is for that command.
|!|delete|literal|prompt|send|
|----|----|----|----|----|
|?|debug|ls|put|status|
|append|dir|mdelete|pwd|trace|
|ascii|disconnect|mdir|quit|type|
|bell|get|mget|quote|user|
|binary|glob|mkdir|recv|verbose|
|bye|hash|mls|remotehelp||
|cd|help|mput|rename||
|close|lcd|open|rmdir||

The question mark (**?**) command is equivalent to the help command. Typing **help** or **?** followed by the name of a command will display a brief description of the command's purpose.

The exclamation point (**!**) can be used to shell to the system (command) prompt. Type Exit to return to the FTP session. You can also issue a subset of system commands to perform as you shell out, e.g., ! dir %windir% | more. When the commands
 in the shell have completed, you will be returned to the FTP session.

The **pwd** command will list the current directory on the remote machine. To change directories on the remote machine, use the **cd** command. To create a new directory on the remote machine, use the **mkdir** command
 followed by the name you would like to assign to the new directory. The **lcd** command can be used to change directories on the local (PC) machine.

To display a listing of files on the remote system, enter: **ls***or***dir**.

To download a file (copy a file from the remote system to your PC), you can use the command **get** or **recv**followed by the name of the file you would like to download. Optionally, you can follow the filename with a second filename
 which will be assigned to the file when it is downloaded to your PC. To download multiple files, you can use the **mget** command followed by a descriptor for the files you would like to download (e.g.: *.f for all files ending in ".f" *or* *.*
 for all files). You will be prompted to indicate whether you would like to download each file in turn. To turn off this prompting, enter the **prompt** command prior to entering the mget command; you will receive the message "Interactive mode
 OFF" indicating that prompting has been deactivated.

By default, files are downloaded and uploaded in ASCII file transfer mode. To download or upload files using Binary format mode, enter the command **Binary** at the "ftp>" prompt prior to downloading or uploading the file(s). To return to ASCII
 file transfer mode, enter the **ASCII** command.

To upload a file (copy a file from your PC to the remote system), you can use the command **put** or **send** followed by the name of the file you would like to upload. Optionally, you can follow the filename with a second filename
 which will be assigned to the file when it is uploaded to the remote system. The **mput** command can be used to upload multiple files.

You can use the **close** or **disconnect** command to drop the current ftp connection without exiting from the command enironment and then use the **open** command to connect to a new host.

Much of this article was taken from the dead link,[http://www.cc.vt.edu/cc/us/docs/faqlib/windows95/clients/msftp.html](http://www.cc.vt.edu/cc/us/docs/faqlib/windows95/clients/msftp.html)


To see the original article, go to[http://web.archive.org/web/*/http://www.cc.vt.edu/cc/us/docs/faqlib/windows95/clients/msftp.html](http://web.archive.org/web/*/http://www.cc.vt.edu/cc/us/docs/faqlib/windows95/clients/msftp.html)


