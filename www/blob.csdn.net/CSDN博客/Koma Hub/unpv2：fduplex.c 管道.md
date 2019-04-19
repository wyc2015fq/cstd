# unpv2：fduplex.c 管道 - Koma Hub - CSDN博客
2019年01月02日 22:13:03[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：22
个人分类：[unpv2																[IPC](https://blog.csdn.net/Rong_Toa/article/category/7969461)](https://blog.csdn.net/Rong_Toa/article/category/8579412)
```cpp
#include	"../unpipc.h"
int
main(int argc, char **argv)
{
	int		fd[2], n;
	char	c;
	pid_t	childpid;
	Pipe(fd);		/* assumes a full-duplex pipe (e.g., SVR4) */
	if ( (childpid = Fork()) == 0) {		
		/* child */
		sleep(3);
		if ( (n = Read(fd[0], &c, 1)) != 1)
			err_quit("child: read returned %d", n);
		printf("child read %c\n", c);
		Write(fd[0], "c", 1);
		exit(0);
	}
	/* parent */
	Write(fd[1], "p", 1);
	if ( (n = Read(fd[1], &c, 1)) != 1)
		err_quit("parent: read returned %d", n);
	printf("parent read %c\n", c);
	exit(0);
}
```
