# Helpful C Tools：source、executables、debugging and performance tuning - Koma Hub - CSDN博客
2019年02月11日 11:44:00[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：114
个人分类：[ANSI C																[C/C++](https://blog.csdn.net/Rong_Toa/article/category/7156199)](https://blog.csdn.net/Rong_Toa/article/category/8653945)
context comes from 《expert C Programming》
**目录**
[Tools to Examine Source](#Tools%20to%20Examine%20Source)
[Tools to Examine Executables](#Tools%20to%20Examine%20Executables)
[Tools to Help with Debugging](#Tools%20to%20Help%20with%20Debugging)
[Tools to Help with Performance Tuning](#Tools%20to%20Help%20with%20Performance%20Tuning)
### Tools to Examine Source
**Tool****Where to Find It****What It Does**
**cb **Comes with the compiler C program beautifier. Run your source through this filter to put it in a standard layout and indentation. Comes from Berkeley. indent   Does the same things cb does. Comes from AT & T. cdecl This book Unscrambles C declarations. cflow Comes with the compiler Prints the caller/callee relationships of a program. cscope Comes with the compiler An interactive ASCII-based C program browser. We use it in the OS group to check the impact of changes to header files. It provides quick answers to questions like: "How many commands use libthread?" or "Who are all the kmem readers?" ctags /usr/bin Creates a tags file for use in vi editor. A tags file speeds up examining program source by maintaining a table of where most objects are located. lint Comes with the compiler A C program checker. sccs /usr/ccs/bin A source code version control system. vgrind /usr/bin A formatter for printing nice C listings.
### Tools to Examine Executables
**Tool****Where to Find It****What It Does**
dis /usr/ccs/bin Object code disassembler dump -Lv /usr/ccs/bin Prints dynamic linking information ldd /usr/bin Prints the dynamic libraries this file needs nm /usr/ccs/bin Prints the symbol table of an object file strings /usr/bin Looks at the strings embedded in a binary. Useful for looking at the error messages a binary can generate, built-in file names, and (sometimes) symbol names or version and copyright information. sum /usr/bin Prints checksum and block count for a file. An-swers questions like: "Are two executables the same version?" "Did the transmission go OK?"
### Tools to Help with Debugging
**Tool****Where to Find It****What It Does**
truss /usr/bin The SVr4 version of trace. This tool prints out the system calls that an executable makes. Use it to see what a binary is doing, and why it's stuck or failing. This is a great help! ps /usr/bin Displays process characteristics. ctrace Comes with the compiler Modifies your source to print lines as they are executed. A great tool for small programs! debugger Comes with the compiler Interactive debugger. file /usr/bin Tells you what a file contains (e.g., executable, data, ASCII, shell script, archive, etc.).
### Tools to Help with Performance Tuning
**Tool****Where to Find It****What It Does**
collector Comes with the compiler (SunOS only) Collects runtime performance data under the control of the debugger. analyzer Comes with the compiler (SunOS only) Analyzes collected performance data. gprof /usr/ccs/bin Displays the call-graph profile data (identifies the computeintensive functions). prof /usr/ccs/bin Displays the percentage of time spent in each routi ne. tcov Comes with the compiler Displays a count of how often each statement is executed (identifies the compute-intensive loops within a function). time /usr/bin/time Displays the total real and CPU time used by a program.
