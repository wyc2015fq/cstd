# The linux-- function hijacking -- Based on LD_PRELOAD - xqhrs232的专栏 - CSDN博客
2016年10月20日 18:23:14[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：336
原文地址::[http://www.programering.com/a/MzN3YzMwATY.html](http://www.programering.com/a/MzN3YzMwATY.html)
Recently i am facing a problem, how to differentiate a problem of library-function from application problems.for solving this problem, we need to know some knowledge about share-library and basics in Linux.For dynamic libraries, they are loaded to memory at program running.
 There are many benefits for this.On the other hand, we have a chance to do something. For example:
 replace the function, which will be loaded,as our function.Usually,this is maybe a very professional problem, at least for me.But thanks for some masters, they have been provide a mature way to our goal.
1). How to replace a function of shared libraries?
the answer is LD_PRELOAD, this is a environment
 variable for GUN-Linker.It is used to indicated some pre-load shared libraries.This meaning that functions in
this libraries will get a higher priority than normal libraries.
Normally, we use this technique just want to intercept some functions.So we can do some other thing( evil thing?)
above the original function.But 
1). How can i get the original function?
some functions: dlopen(), dlsym(), dlclose() and dlerror().There have been many articles to explain them.
In the next section, I will try to translate a nice article. The original
 article is there
This article list as follows:
A shared database.
Two. Simple LD_PRELOAD (relative to the back of the processed LD_PRELOAD)
Production and use of shared library 2.1.
2.2. dlsym()
2.3. use restrictions
Three. Relevant hiding technology
3.1 Jynx-Kit
3.2 checking methods
Below is the text:
A shared database.
As we know, dynamic library linking is implemented in the program loading. On my computer this
 function is realized by the ld-linux-x86-64.so.X, but it is also possible for other models of ld-linux.so.X. If you have interest, can be verified
fluxiux@handgrep:~$ readelf -l /bin/ls 
[...]
  INTERP         0x0000000000000248 0x00000000004purposes00248 0x0000000000400248 
                 0x000000000000001c 0x000000000000001c  R      1 
      [Requesting program interpreter: /lib64/ld-linux-x86-64.so.2] 
[...]
(PS: where readelf is an assistant tool for reading elf files, dynamic loading procedures will generally specify a dynamic loading tool. This involves parsing the elf format, will try to make some simple analysis of ELF format in the next article. )
Compared with static compilation huge volume, dynamic compilation on a small number of. For some library functions which retain only a pointer to the related database,
 and contain no function entity. If you want to view aprogram included those of library calls, you can use the " command to check, for example; ldd":
<a target=_blank href="mailto:fluxiux@handgrep:~$">fluxiux@handgrep:~$</a> ldd /bin/ls 
    linux-vdso.so.1 =>  (0x00007fff0bb9a000) 
    libselinux.so.1 => /lib/x86_64-linux-gnu/libselinux.so.1 (0x00007f7842edc000) 
    librt.so.1 => /lib/x86_64-linux-gnu/librt.so.1 (0x00007f7842cd4000) 
    libacl.so.1 => /lib/x86_64-linux-gnu/libacl.so.1 (0x00007f7842acb000) 
    libc.so.6 => /lib/x86_64-linux-gnu/libc.so.6 (0x00007f7842737000) 
    libdl.so.2 => /lib/x86_64-linux-gnu/libdl.so.2 (0x00007f7842533000) 
    /lib64/ld-linux-x86-64.so.2 (0x00007f7843121000) 
    libpthread.so.0 => /lib/x86_64-linux-gnu/libpthread.so.0 (0x00007f7842314000) 
    libattr.so.1 => /lib/x86_64-linux-gnu/libattr.so.1 (0x00007f784210f000)
We look at an example (example, below)
#include <stdio.h> 
main() 
{ 
        printf("huhu la charrue"); 
}
Are compiled into a dynamic and static two
fluxiux@handgrep:~$ gcc toto.c -o toto-dyn 
fluxiux@handgrep:~$ gcc -static toto.c -o toto-stat 
fluxiux@handgrep:~$ ls -l | grep "toto-" 
-rwxr-xr-x  1 fluxiux fluxiux     8426 2011-10-28 23:21 toto-dyn 
-rwxr-xr-x  1 fluxiux fluxiux   804327 2011-10-28 23:21 toto-stat
We see " toto-stat" toto-dyn" is almost " 96 times, why is this?
fluxiux@handgrep:~$ ldd toto-stat 
    is not a dynamic executable
(because the " toto-stat" is static.)
Dynamic linking is a kind of very good method, gave us
 a lot of benefits, for example:
■update libraries and still support programs that want to use older, non-backward-compatible versions of those libraries,
■override specific libraries or even specific functions in a library when executing a particular program,
■do all this while programs are running using existing libraries.
For the shared library name, we have some habits. If a database name is " soname", then usually prefixed with a " lib", a suffix ".So", and a version number. 
(PS: although only agreed: but you don't do that, your library cannot be correctly identified)
Now we can look at the LD_PRELOAD., 
Two. A simple LD_PRELOAD application
As we know, the library files generally stored in the " directory; /lib". So if you want to modify a database, the most easy to think of the way is to find the library source
 code, modify the re compiled again. But in addition to this project, we have another cool method, that is using Linux to provide an external interface our: LD_PRELOAD. (have a look first below this.)
Production and use of shared library 2.1.
If you want to override the " printf" behavior, can do to write one of your " printf" function
        #define _GNU_SOURCE
        #include <stdio.h>
	
        int printf(const char *format, ...)
        {
              exit(153);
        }
Then compile compile it into a shared library. Like this, 
        fluxiux@handgrep:~$ gcc -Wall -fPIC -c -o my_printf.o my_printf.c 
        my_printf.c: In function 'printf':
        my_printf.c:6:2: warning: implicit declaration of function 'exit'
        my_printf.c:6:2: warning: incompatible implicit declaration of built-in function 'exit'
        fluxiux@handgrep:~$ gcc -shared -fPIC -Wl,-soname -Wl,libmy_printf.so -o libmy_printf.so  my_printf.o
Then we modify an environment variable before running its own testing procedures
        fluxiux@handgrep:~$ export LD_PRELOAD=$PWD/libmy_printf.so
        fluxiux@handgrep:~$ ./toto-dyn
You will see the " printf" behavior has changed, it does not print the
 " Huhu La charrue". Okay, let us have a look " ltrace" how to say
        fluxiux@handgrep:~$ ltrace ./toto-dyn 
        __libc_start_main(0x4015f4, 1, 0x7fffa88d0908, 0x402530, 0x4025c0 <unfinished ...>
        printf("huhu la charrue" <unfinished ...>
        +++ exited (status 153) +++
Fun, our " printf" on the system " printf" be called before the. Now a new problem, if our goal was simply to change the " printf" behavior, but does not destroy the original function. How do you do, rewriting the entire function?!! That is obviously not suitable,
 in order to deal with this problem, several functions can see below. 
2.2. dlsym()
In the library of " libdl" there are several interesting function
dlopen() : The loading of a library
dlsym() : Pointer access a specific symbol
dlclose() : Unloading a database
Here, because the library has been in the loader is
 loaded, we only need to call the " dlsym" can. We give " dlsym" " RTLD_NEXT" parameter, used to get to the original " printf" function pointer. Like this
	[...]
			typeof(printf) *old_printf;
	[...]		 
			//DO HERE SOMETHING VERY EVIL
			old_printf = dlsym(RTLD_NEXT, "printf");
	[...]
Then we need to make a special treatment to the
 format string (parameters corresponding to the general need not be so troublesome), after treatment can be directly used
        #include <stdio.h>
        #include <dlfcn.h>
        #include <stdlib.h>
        #include <stdarg.h>
	
        int printf(const char *format, ...)
        {
                  va_list list;
                  char *parg;
                  typeof(printf) *old_printf;
                  // format variable arguments
                  va_start(list, format);
                  vasprintf(&parg, format, list);
                  va_end(list);
	
                  //DO HERE SOMETHING VERY EVIL
	
                  // get a pointer to the function "printf"
                  old_printf = dlsym(RTLD_NEXT, "printf");
                  (*old_printf)("%s", parg); // and we call the function with previous arguments
	
                  free(parg);
        }
Re compile
        fluxiux@handgrep:~$ gcc -Wall -fPIC -c -o my_printf.o my_printf.c 
        my_printf.c: In function 'printf':
        my_printf.c:21:1: warning: control reaches end of non-void function
        fluxiux@handgrep:~$ gcc -shared -fPIC -Wl,-soname -Wl libmy_printf.so -ldl -o libmy_printf.so my_printf.o 
Try it again
        fluxiux@handgrep:~$ export LD_PRELOAD=$PWD/libmy_printf.so
        fluxiux@handgrep:~$ ./toto-dyn 
        huhu la charrue
On such a call to the " printf" users do not want things quietly changed. But this mechanism has some defects. 
2.3. use restrictions
Although this method is very cool, but there are some limitations. For example, is not valid for static compilation procedures. Because of the function of the static compiled program does not need to connect the dynamic library. But, if the file SUID or the
 SGID bit is set to 1, the loading time will ignore the LD_PRELOAD (this is the developer of LD for security reasons do). 
Three. Relevant hiding technology
3.1 Jynx-Kit
About two weeks ago, we introduce a new hiding technology. It uses an automatic operation of the shell script, and has not been rkhunter and chkrootkit check. Let us have a look the actual code, in the " ld_poison.c", has 14 functions is being held hostage.
[...]
    old_fxstat = dlsym(RTLD_NEXT, "__fxstat");
    old_fxstat64 = dlsym(RTLD_NEXT, "__fxstat64");
    old_lxstat = dlsym(RTLD_NEXT, "__lxstat");
    old_lxstat64 = dlsym(RTLD_NEXT, "__lxstat64");
    old_open = dlsym(RTLD_NEXT,"open");
    old_rmdir = dlsym(RTLD_NEXT,"rmdir");
    old_unlink = dlsym(RTLD_NEXT,"unlink"); 
    old_unlinkat = dlsym(RTLD_NEXT,"unlinkat");
    old_xstat = dlsym(RTLD_NEXT, "__xstat");
    old_xstat64 = dlsym(RTLD_NEXT, "__xstat64");
    old_fdopendir = dlsym(RTLD_NEXT, "fdopendir");
    old_opendir = dlsym(RTLD_NEXT, "opendir");
    old_readdir = dlsym(RTLD_NEXT, "readdir");
    old_readdir64 = dlsym(RTLD_NEXT, "readdir64");
[...]
We can tune a " open" function to have a look, see " in the interior; __xstat" is called
[...]
    struct stat s_fstat;
[...]
    old_xstat(_STAT_VER, pathname, &s_fstat);
[...]
The following is a check operation, check the file group ID, path, and file name. (we want to make sure that the file is not a " ld.so.preload", because we want to hide it). If we want to hide files, will not return results to the user
[...]
    if(s_fstat.st_gid == MAGIC_GID || (strstr(pathname, MAGIC_DIR) != NULL) || (strstr(pathname, CONFIG_FILE) != NULL)) {
        errno = ENOENT;
        return -1;
    }
[...]
According to this idea will be above the function again after processing, we can to the user (attacker) to hide some files and behavior. But we have what method can be used to check to see here?
3.2 check the occult
Whether the hiding method Is it right? Enough let you shine at the moment, but it does take rkhunter and chkrootkit. Because the two kinds of tool use is symbolic of the inspection way based on the method, this really is not the best.
See the example below: 
First of all, if we are in the clear LD_PRELOAD variables, generates a checksum files to a specified file. Like this
        fluxiux@handgrep:~$ sha1sum toto-dyn 
        a659c72ea5d29c9a6406f88f0ad2c1a5729b4cfa  toto-dyn
        fluxiux@handgrep:~$ sha1sum toto-dyn > toto-dyn.sha1
Then check that the file checksum in the setting of LD_PRELOAD, like this
        fluxiux@handgrep:~$ export LD_PRELOAD=$PWD/libmy_printf.so
        fluxiux@handgrep:~$ sha1sum -c toto-dyn.sha1 
        toto-dyn: OK
(looks through the authentication of the document)
But is this really the right?
Obviously not, because we actually have not changed the program files, so check what time the file and will be the same. If the tool is based on latent checksum, then no doubt this is not. Other inspection techniques are: check suspicious files, symbol, port
 binding detection, but failed, because the concealment method is too flexible, and in Jynx we have a sort of port knocking to open the remote shell for our host.
Well, for those things we can do what? All the library, check the LD_PRELOAD direction or " /etc/ld.so.preload". We know that " dlsym" often used to find the original function, all can be
$ strace ./bin/ls
[...]
open("/home/fluxiux/blabla/Jynx-Kit/ld_poison.so", O_RDONLY) = 3
read(3, "\177ELF\2\1\1\0\0\0\0\0\0\0\0\0\3\0>\0\1\0\0\0\240\n\0\0\0\0\0\0"..., 832) = 832
fstat(3, {st_mode=S_IFREG|0755, st_size=17641, ...}) = 0
mmap(NULL, 2109656, PROT_READ|PROT_EXEC, MAP_PRIVATE|MAP_DENYWRITE, 3, 0) = 0x7f5e1a586000
mprotect(0x7f5e1a589000, 2093056, PROT_NONE) = 0
mmap(0x7f5e1a788000, 8192, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_FIXED|MAP_DENYWRITE, 3, 0x2000) = 0x7f5e1a788000
close(3) 
[...]
open("/lib/x86_64-linux-gnu/libdl.so.2", O_RDONLY) = 3
[...]
Analysis of the " ld_poison.so" file, where we saw a lot of replacement, they are likely to shelter evil people and countenance evil practices. Have a look string information of these binaries, are likely to provide us with some interesting implications. Of
 course, if they are skillfully handled, it can't be helped. But from another perspective, the normal procedures need to hide their string information. 
fluxiux@handgrep:~/blabla/Jynx-Kit$ strings ld_poison.so
[...]
libdl.so.2
[...]
dlsym
fstat
[...]
lstat hooked.
ld.so.preload
xochi <-- sounds familiar
[...]
/proc/%s <-- hmmm... strange!
[...]
This is called proof hiding method Jynx-kit, think through the hidden symbol based checking to cope with the above is not too realistic. While the heuristic test results will be good. 
The original here
