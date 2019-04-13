
# Manipulating Files With Tcl - Augusdi的专栏 - CSDN博客


2016年10月24日 14:13:59[Augusdi](https://me.csdn.net/Augusdi)阅读数：1336


﻿﻿
# Manipulating Files With Tcl
Tcl provides a thin layer of abstraction over the Unix file system. I saythinbecause you can actually see Unix through it, but it's enough of an abstraction to be portable to most modern operating systems. (This is probably due to the success of C, which has resulted in most modern operating systems providing support for Unix style flat files.) Extended Tcl adds commands to provide direct access to Unix system calls, making the language suitable for systems programming.
## File Descriptors
Upon success, the`open`command returns a file descriptor orfile identifier, which is a string. You should, in general, treat this string as a black box: you never need to look at it or print it, just sock it away in a variable to be passed as an argument to other commands.
### Standard File Descriptors
The Tcl interpreter makes the three standard Unix file descriptors available to your program. Standard input is available as the file descriptor`stdin`; standard output is available as the file descriptor`stdout`, and standard error is available as the file descriptor`stderr`.
## The File Abstraction
### Creating A File
A file is created implcitly in Tcl by opening it for writing. See[below](http://www.lib.uchicago.edu/keith/tcl-course/tcl-files.html#open).
### Removing A File
Standard Tcl has no means to remove a file. It's easily accomplished, however, by invoking the external, standard`rm`program:exec rm $filenameExtended Tcl has the`unlink`command, which takes a variable number of arguments, which are file names, and removes them.
### Opening a File
The`open`command takes two arguments, a pathname and an (optional) access type. The pathname can of course be absolute or relative.`open`supports two notations for the access method: both mimic different C library functions.If no second argument is given, the default is`r`.
#### String-Style Access Method
A single string argument is used to indicate the access method, one of:`r`Read only, no create`r+`Read / write, no create`w`Write only, truncate or create`w+`Read / write, truncate or create`a`Write only, append or create`a+`Read / write, append or create
#### List-Style Access Method
A list of any of the following flags may be provided as the second argument; exactly one of`RDONLY`,`WRONLY`, or`RDWR`must included in the list.`RDONLY`The file is opened for reading only.`WRONLY`The file is opened for writing only.`RDWR`The file is opened for reading and writing.`APPEND`The file pointer is positioned at EOF before*each*write.`CREAT`The file is created if it doesn't already exist.`EXCL`Used with`CREAT`, generates an error if the*does*already exist (exclusive access).`NONBLOCK`Do not block when opening the file; generally only applies to fifos and sockets.`TRUNC`If the file exists, truncate it to length zero upon open.`NOCTTY`Do not become the controlling terminal (this one is fancy and can be ignored.)No status code is returned to indicate success or failure: instead, any problems cause an error to be signalled.
Some examples:
\# These first two are equivalent:
set f [open /usr/dict/words r]
set f [open /usr/dict/words]
\# So are these two:
set tmpfile [open /tmp/temporary w]
set tmpfile [open /tmp/temporary [list WRONLY CREAT TRUNC]]
### Closing A File
All files are automatically closed when the process terminates, but sometimes you want to close files explictly. There are two typical reasons for this:A process only has a finite number of file descriptors.In some Unixes there may be no limit, but in most modern Unix systems the limit is around 64. So if you're processing a huge number of files, even just serially, you want to be sure to close them as you go.
Closing a file flushes buffers.If you want to be sure that the final buffer-full of data is flushed to disk as soon you're done writing, you should close the file. (But see[Buffering](http://www.lib.uchicago.edu/keith/tcl-course/tcl-files.html#flush)below for finer control.)
The`close`command just takes a file descriptor as an argument; an error is signalled if the file can't be closed or if it wasn't open.
### Writing To A File
The`puts`command takes a file descriptor and a string, and writes the string to the file with a trailing newline added (unless the -nonewline option is given). If invoked with only one argument, the string is written to standard output. These two commands are equivalent:puts stdout "Foo"
puts "Foo"
### Reading From A File
Tcl has two commands for reading from a file:`gets`is line-oriented, while`read`is block oriented (and suitable for random access I/O). Extended Tcl adds some other commands:`bsearch`searches a sorted file using binary search, and`lgets`is list-oriented (it reads Tcl lists).
#### The
#### gets
#### Command
The`gets`command comes in two flavors. The one-argument form takes a file descriptor as a parameter and returns the next line from the file. End of file is indicated as the empty string; since this is ambiguous, you may want to use the`eof`command to test whether or not end of file actually occurred.More useful is the two-argument form, in which a variable name is provided as a parameter. In this form the next line read is stored in the named variable, and the length of the line is returned, or -1 or end of file.
In either form, the terminating newline is removed.
Here is a typical use of`gets`in a loop to read an entire file line by line:
set f [open /home/keith/.profile]
while {[gets $f line] >= 0} {
    puts [string length $line]
}
close $f
#### The
#### read
#### Command
The`read`command takes two parameters, a file descriptor and acount. The second parameter is optional. If provided,`read`reads the nextcountbytes from the file (or up to end of file if there are fewer thancountbytes left). The bytes are returned as a string. If thecountargument is not provided, all the rest of the bytes in the file are read and returned.
#### The
#### bsearch
#### Command
The`bsearch`command does a binary search of an open file. In its simplest form, it takes a file descriptor and a string to search for, and returns the matching line if found or the empty string. It also takes an optional variable name like`gets`, in which case it returns a boolean value. (See TclX(7tcl) for complete details.)set f [open /usr/dict/words]
bsearch $f zygote
=> zygote
bsearch $f framistan
=> 
close $f
#### The
#### lgets
#### Command
The`lgets`command reads Tcl lists. It takes the same arguments as`gets`, and returns results the same way, but reads Tcl lists rather than lines.Its easy to be confused by this command. In particular, the same way that it's a mistake to treat an arbitrary string as if it were a list, it's a mistake to use`lgets`on a file that wasn't written full of lists by Tcl.
To write lists to a file in the form that`lgets`will appreciate only requires that*proper*lists be written to the file with`puts`. In this example, note the newline in the second list, which illustrates the difference between`lgets`and`gets`:
set f [open feh w]
puts $f [list 1 2 3 4]
puts $f [list foo bar \n baz]
close $f
set f [open feh r]
while {[lgets $f l] >= 0} {
    puts [llength $l]
}
close $f
## The
## eof
## Command
The end of file status of any file descriptor can be tested at any time with the`eof`command, which takes a file descriptor as an argument and returns a true (1) if end of file has been reached, or false (0) otherwise. Here is an alternate way to read all lines from a file, using eof:set f [open /home/keith/.profile]
while {![eof $f]} {
    puts [string length [gets $f]]
}
close $f
## Random Access
Tcl supports random access to files just like any other Unix programming language. The basic tools are:`seek`to set the file pointer;`tell`to read it; and`read`to read a record. Note that any command that reads from a file can be used in a random access fashion;`read`just usually makes more sense than`gets`.
### Moving the File Pointer: Seeking In A File
seekfdoffset?origin?The`seek`command positions the file pointer for a given file descriptorfdto a specified byteoffset. By default theoffsetis relative to the beginning of the file, but the optionaloriginparameter can be used to specify one of:start(the beginning of the file)
current(the current file pointer)
end(the end of the file)
`seek`always returns the empty string, so the only way to tell whether or not the seek suceeded is to use the`tell`command and check if you got to where you were going. This is a Tcl design bug, especially given that the Unix seek system call returns the status.
Here is some code that reads a file of 80-column card images:
\# Return the number of records of size $size in $fd
proc number-of-records {size fd} {
    \# Save current file pointer
    set fp [tell $fd]
    \# Seek to eof
    seek $fd 0 end
    \# Store file pointer
    set end [tell $fd]
    \# Restore current file pointer
    seek $fd $fp
    if [expr $end % $size != 0] {
	error "not an integral number of records"
    } else {
        expr $end / $size
    }
}
set f [open cards]
for {set n [number-of-records 80 $f]} {$n > 0} {incr n -1} {
    puts [string length [read $f 80]]
}
close $f
### Querying the File Pointer
tellfdThe`tell`command returns the current position of the file pointer for file descriptorfdas an integer.
## Buffering
flushfdBy default, files opened for writing by Tcl are block buffered. You can actually control the buffering with Extended Tcl commands, but the simplest approach is to use the flush command after a write. Flushing is generally unnecessary when working with files (and in fact will slow you down), but it's crucial when doing IPC.
## Current Working Directory
cd?dirname?pwdThe`cd`command changes the current working directory (CWD) of the Tcl process todirname(or to the user's home directory ifdirnameisn't given). The`pwd`command returns the CWD as a string result.
## Reading Directories
glob`?-nocomplain?``?--?`pattern?pattern ...?The`glob`command provides easy access to the contents of directories. It takes the same[glob patterns](http://www.lib.uchicago.edu/keith/tcl-course/pattern-matching.html)that we've seen before (e.g., in the`switch`command) and applies them to the filenames stored in a directory, returning a list of the filenames that match. If there are no matching filenames, an error is signalled (unless`-nocomplain`is specified). If thepatterns are specified as absolute pathnames, the returned filenames will be absolute as well.
## The
## file
## Command
The`file`command gathers a number of miscellaneous file manipulation commands in one place. There are commands for manipulating file names, for checking the existance of a file, and for querying the inode.
### Manipulating File Names
file dirnamenamefile extensionnamefile rootnamenamefile tailnameWhile Unix doesn't use mandatory filename extensions in the manner of MS/DOS, extensions are used conventionally, and so Tcl provides commands to manipulate them easily. A picture is worth a thousand words:file dirname /a/b/foo.html
=> /a/b
file extension /a/b/foo.html
=> .html
file rootname /a/b/foo.html
=> /a/b/foo
file tail /a/b/foo.html
=> foo.html
### File Predicates
file existsnamefile ownednamefile executablenamefile readablenamefile writablenamefile isfilenamefile isdirectorynameThe`file`command provides several predicates for testing files. All return a Boolean (1 or 0) value.`file readable`,`file writable`, and`file owned`test relative to the user running the Tcl script.
### Accessing The Inode
file statnamearraynamefile typenameThe`file stat`command reads the inode fornameand populates the array variablearraynamewith entries for each field in the inode.The fields of the inode are named as follows:
`nlink`The link count.`uid`The UID of the file's owner.`gid`The GID of the file's group.`mode`The mode of the file as a decimal (!) integer. This includes the user, group and other permissions, and also a few other bits (the setuid and setgid bits and the sticky bit).`size`Size in bytes.`atime`Last access time, as a decimal integer (seconds since the Unix epoch).`mtime`Last modification time, as above.`ctime`Last inode change time, as above.`ino`The I-number.`dev`The device the file resides on; together with the I-number, these these two values uniquely identify a file.The`file type`command returns a string which represents the type of the file, one of:`file`,`directory`,`characterSpecial`,`blockSpecial`,`fifo`,`link`, or`socket`.
## Accessing the Inode Via A File Descriptor
fstatfdfstatfditemfstatfdstatarrayvarExtended Tcl provides access to the contents of inodes via the`fstat`command. This command duplicates the`file stat`command, except that while`file stat`operates on a named file,`fstat`operates on a file descriptor.`fstat`comes in three flavors, each taking a mandatory file descriptor argument. In the first form, with no additional arguments, all the fields of the inode are returned as a keyed list. In the second form, only a specific value for the named field is returned, as a string. In the third form, all the fields are entered into the named array variable.
The fields of the inode are named as in the`file stat`command; the following extra fields are provided:
`type`Type of file; possible values are:`file`,`directory`,`characterSpecial`,`blockSpecial`,`fifo`,`link`, and`socket`.`tty`A boolean valuel, 1 if the file is a terminal.`remotehost`This field is only present if the file descriptor refers to a network connection; in this case, the value is a list whose first element is the network address of the remote host and the second element is the hostname (if available).[http://www2.lib.uchicago.edu/keith/tcl-course/topics/tcl-files.html](http://www2.lib.uchicago.edu/keith/tcl-course/topics/tcl-files.html)

