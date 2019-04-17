# 可以返回执行结果的system函数加强版本 - DoubleLi - 博客园






在GNU Linux C编程中，要想进行系统命令的执行的话，只提供了system接口，但是此接口并不能得到命令执行后所输出的值，而只能够得到命令是否执行成功的结果。仅仅这样的功能还是不够的，有的时候是要必须通过命令的输出来判断下一步的结果或步骤的，那么怎么样能够得到system命令执行的结果呢？那就可以使用到popen函数和fgets函数进行命令的输出信息的获取了，实际例子如下：

注意：此接口只能够获取命令输出的最后一行的信息，若有多行输出信息将不能够全部获取到，此封装接口只适用于得到命令执行结果的最后一行的信息。

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

int super_system(const char * cmd, char *retmsg, int msg_len)
{
        FILE * fp;
        int res = -1;
        if (cmd == NULL || retmsg == NULL || msg_len < 0)
        {
                printf("Err: Fuc:%s system paramer invalid!\n", __func__);
                return 1;
        }
        if ((fp = popen(cmd, "r") ) == NULL)
        {
                perror("popen");
                printf("Err: Fuc:%s popen error: %s\n", __func__, strerror(errno));
                return 2;
        }
        else
        {
                memset(retmsg, 0, msg_len);
                while(fgets(retmsg, msg_len, fp));
                {
                        printf("Fuc: %s fgets buf is %s\n", __func__, retmsg);
                }
                if ( (res = pclose(fp)) == -1)
                {
                        printf("Fuc:%s close popen file pointer fp error!\n", __func__);
                        return 3;
                }
                //drop #012 from system result retmsg.
                retmsg[strlen(retmsg)-1] = '\0';
                return 0;
        }
}

int main()
{
    char *cmd = "whoami";
    char *cmd1 = "initctl list";
    char retmsg[1024] = {0};
    int ret = 0;
    ret  = super_system(cmd, retmsg, sizeof(retmsg));
    printf("system ret is %d retmsg is \n%s\n", ret, retmsg);
    return 0;
}

main函数中使用了whoami的命令，执行结果即是当前用户名。
执行结果：
linuxidc@ufo:~$ ./a.out 
Fuc: super_system fgets buf is linuxidc

system ret is 0 retmsg is 
linuxidc

/*********************************************************************
 * Author  : Samson
 * Date    : 03/13/2015
 * Test platform:
 *              3.13.0-24-generic
 *              GNU bash, 4.3.11(1)-release 
 * *******************************************************************/









