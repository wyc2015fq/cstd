# Pipes - Keith - CSDN博客





2014年03月16日 20:18:29[ke1th](https://me.csdn.net/u012436149)阅读数：484标签：[linuxshellpipes](https://so.csdn.net/so/search/s.do?q=linuxshellpipes&t=blog)








# Pipes: A Brief Introduction



||A *pipe* is a form of [*redirection*](http://www.linfo.org/redirection.html) that is used in [Linux](http://www.linfo.org/linuxdef.html) and other [Unix-like](http://www.linfo.org/unix-like.html)[operating systems](http://www.linfo.org/operating_system.html) to send the output of one [program](http://www.linfo.org/program.html) to another program for further processing.Redirection is the transferring of [*standard output*](http://www.linfo.org/standard_output.html) to some other destination, such as another program, a [file](http://www.linfo.org/filedef.html) or a printer, instead of the display monitor (which is its default destination). Standard output, sometimes abbreviated *stdout*, is the destination of the output from [*command line*](http://www.linfo.org/command_line.html) (i.e., all-text mode) programs in Unix-like operating systems.Pipes are used to create what can be visualized as *a pipeline of commands*, which is a temporary direct connection between two or more simple programs. This connection makes possible the performance of some highly specialized task that none of the constituent programs could perform by themselves. A [*command*](http://www.linfo.org/command.html) is merely an instruction provided by a user telling a [computer](http://www.linfo.org/computer.html) to do something, such as launch a program. The command line programs that do the further processing are referred to as [*filters*](http://www.linfo.org/filters.html).This direct connection between programs allows them to operate simultaneously and permits [data](http://www.linfo.org/data.html) to be transferred between them continuously rather than having to pass it through temporary text files or through the display screen and having to wait for one program to be completed before the next program begins.|
|----|----|


from:[http://www.linfo.org/pipes.html](http://www.linfo.org/pipes.html)




