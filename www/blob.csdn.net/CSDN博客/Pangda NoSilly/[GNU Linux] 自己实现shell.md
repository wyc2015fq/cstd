# [GNU/Linux] 自己实现shell - Pangda NoSilly - CSDN博客

2017年07月25日 09:13:02[hepangda](https://me.csdn.net/hepangda)阅读数：382


# 写在前面

`shell`作为一种与内核对话的一种方式，为我们使用操作系统服务，提供了很多便利。在我们使用Linux时，`shell`是不得不接触的内容之一。为了学习和熟悉`Linux`进程相关的内核函数，我们可以尝试着自己实现一个`shell`。

# 源代码

### src/Makefile

```
SOURCES = $(wildcard *.cpp)
OBJECTS = $(patsubst %.cpp, %.o, $(SOURCES))
CXXFLAG = -std=c++14 -I ../include -O2 -lreadline

psh: $(OBJECTS)
    g++ $(CXXFLAG) -o psh $(OBJECTS)

$(OBJECTS): $(SOURCES)
    g++ $(CXXFLAG) -c $(SOURCES)

.PHONY: clean
clean :
    -rm $(OBJECTS)
```

### include/psh.h

```cpp
#pragma once
#ifndef _HEADER_PANGDA_SHELL__
#define _HEADER_PANGDA_SHELL__

#include<unistd.h>
#include<cstdio>
#include<string>
#include<vector>
#include<functional>
#include<map>

typedef std::vector<std::string> argument_t;

struct command_t {
    int is_right_cmd = 0;               //命令是否是正确的，若为0则说明是正确的，其他数字代表错误码
    std::string execfile;               //执行的文件名
    argument_t arguments;               //参数列表
    bool is_redirect_stdin = false;     //是否重定向了标准输入，即是否存在<语法元素
    std::string filename_in;            //新的标准输入名
    bool is_redirect_stdout = false;    //是否重定向了标准输出，即是否存在>语法元素
    bool stdout_mode = false;           //false表示截断输出，true表示追加输出
    std::string filename_out;           //新的标准输出文件名
    bool is_background = false;         //是否指定在后台运行，即是否存在&语法元素
    bool is_pipe = false;               //是否是一个管道，即是否存在|语法元素
    std::string pipe_prompt[2];         //保存管道命令
};

command_t parse_command(std::string command);
std::string string_trim(std::string s);
std::string get_tip();
int exec_command(command_t &cmd);
int psh_error(int error);
int shellfunc_exit(command_t);
int shellfunc_logout(command_t);
int shellfunc_cd(command_t);

#endif
```

### src/psh.cpp

```cpp
#include<pangda/psh.h>
#include<readline/readline.h>
#include<readline/history.h>
#include<cstdlib>
#include<signal.h>
extern std::map<std::string, std::function<int(command_t)> > shell_commands;

int main(int argc, char *argv[], char **envp) {
    //构建内建命令与实现函数的映射
    shell_commands["exit"] = shellfunc_exit;
    shell_commands["logout"] = shellfunc_logout;
    shell_commands["cd"] = shellfunc_cd;

    //阻断SIGINT SIGQUIT SIGSTOP SIGTSTP
    signal(SIGINT, SIG_IGN);
    signal(SIGQUIT, SIG_IGN);
    signal(SIGSTOP, SIG_IGN);
    signal(SIGTSTP, SIG_IGN);
    while (true) {
        std::string st = readline(get_tip().c_str());   //获得用户输入的内容
        //若用户输入的不是全空格，则将这条命令保存在历史记录中。
        //否则就不处理这条命令，直接获得下一条命令。
        if (string_trim(st) == "")
            continue;
        else
            add_history(st.c_str());

        //解析命令
        command_t cmd = parse_command(st);

        //若命令是管道，则分别执行两条管道命令
        if (cmd.is_pipe) {
            command_t pipe1 = parse_command(cmd.pipe_prompt[0]);
            command_t pipe2 = parse_command(cmd.pipe_prompt[1]);
            if (exec_command(pipe1) != 0)   //若管道的第一条命令就是错误的，不再执行第二条命令
                continue;
            exec_command(pipe2);
        } else if (cmd.is_right_cmd) {  //若解析命令之后发现命令存在错误，则进入错误处理程序
            psh_error(cmd.is_right_cmd);
            continue;
        }
        exec_command(cmd);      //交由解释器解释执行命令
    }
    return 0;
}
```

### src/psh_parser.cpp

```cpp
#include<pangda/psh.h>
#include<sstream>

//分割命令中的各个参数
static argument_t split_command(std::string command) {
    argument_t ret;
    std::stringstream out(command);     //构建字符串流
    std::string t;
    while (out >> t) {  //若流中仍然有内容
        ret.push_back(t);
    }
    return ret;
}

//分割语法，解决ls>c的问题
static void stylize_command(std::string &cmd) {
    for (auto i = 0u; i < cmd.length(); i++) {
        //offset用于计算偏移值，若在语法元素左右插入了空格，那么cmd的长度会发生变化
        //而i没有发生变化，因此需要计算偏移值。
        int offset = 0; 
        //若检测到了语法元素:><|&
        if (cmd[i] == '<' || cmd[i] == '|' || cmd[i] == '&') {
            if (i - 1 >= 0 && cmd[i - 1] != ' ') {  //若语法元素左方没有空格
                cmd.insert(i, " ");
                offset--;
            }
            if (i + 1 < cmd.length() && cmd[i + 1 + offset] != ' ') {   //若语法元素右方无空格
                cmd.insert(i + 1, " ");
            }
        } else if (cmd[i] == '>') {
            if (i - 1 >= 0 && cmd[i - 1] != ' ' && cmd[i - 1] != '>') {  //若语法元素左方没有空格
                cmd.insert(i, " ");
                offset--;
            }
            if (i + 1 < cmd.length() && cmd[i + 1 + offset] != ' ' && cmd[i + 1 + offset] != '>') {   //若语法元素右方无空格
                cmd.insert(i + 1, " ");
            }
        }
    }
}

//构建管道命令。管道的实现方式：将管道左右分别变为两个带重定向的命令，分别执行两个命令
static void pipe_buildcmd(command_t &origin_cmd) {
    auto in = origin_cmd.arguments;
    origin_cmd.pipe_prompt[0] = origin_cmd.pipe_prompt[1] = "";

    bool front = true;
    for (auto i : in) {
        if (i != "|") { //若未检测到|，说明仍然在管道左方
            if (front) {
                origin_cmd.pipe_prompt[0] += i + " ";
            } else {
                origin_cmd.pipe_prompt[1] += i + " ";
            }
        } else {
            front = false; 
        }
    }
    origin_cmd.pipe_prompt[0] += " > /tmp/psh_pipefile";    //补充管道左方命令
    origin_cmd.pipe_prompt[1] += " < /tmp/psh_pipefile";    //补充管道右方命令
}

//删除命令中语法元素所占参数表位置
static void setarg_command(command_t &cmdt) {
    std::vector<std::string> ret, in = cmdt.arguments;
    int sz = in.size();

    //若命令中不含任何语法元素，不含管道原因是若含有管道会提前返回，不在这里执行。
    if (!(cmdt.is_redirect_stdin || cmdt.is_redirect_stdout || cmdt.is_background)) {
        return;
    }

    //逐个检查参数列表中的内容
    for (int i = 0; i < sz; i++) {
        if (in[i] == ">" || in[i] == "<" || in[i] == ">>") { //若为<>，需要多跳过一个内容
            i++;
        } else if (in[i] == "&") {  //若为&，直接跳过即可，循环结束会自动i++
            continue;
        } else {
            ret.push_back(in[i]);   //没有语法元素说明是参数的一部分，应当保留
        }
    }
    cmdt.arguments = ret;   //改变原本的参数表
}

//标记语法元素
static void setmark_command(command_t &cmdt) {
    //从除过命令名之后的其他元素开始逐个检查
    for (auto it = cmdt.arguments.begin() + 1; it != cmdt.arguments.end(); it++) {
        //若出现>元素
        if (*it == ">" || *it == ">>") {
            cmdt.is_redirect_stdout = true; //标记>语法元素
            if (*it == ">>")
                cmdt.stdout_mode = true;
            if (it + 1 == cmdt.arguments.end()) {
                cmdt.is_right_cmd = 400; //错误400：>语法元素后没有跟任何内容，错误语法
                break;
            }
            cmdt.filename_out = *++it;  //存储重定向的文件名
            continue;
        } else if (*it == "<") {
            cmdt.is_redirect_stdin = true;  //标记<语法元素
            if (it + 1 == cmdt.arguments.end()) {
                cmdt.is_right_cmd = 401; //错误400：>语法元素后没有跟任何内容，错误语法
                break;
            }
            cmdt.filename_in = *++it; //存储重定向的文件名
            continue;
        } else if (*it == "|") {
            cmdt.is_pipe = true;    //标记|语法元素
            cmdt.is_right_cmd = 490;    //错误490：命令是一个管道，应当使用管道的执行方法
            pipe_buildcmd(cmdt);    //构建管道命令
            break;
        } else if (*it == "&") {
            if (it + 1 == cmdt.arguments.end()) {
                cmdt.is_background = true;  //标记&语法元素
            } else {
                cmdt.is_right_cmd = 403; //错误400：&语法元素出现在了命令中间，错误语法
                break;
            }
        }
    }
    if (cmdt.is_right_cmd || cmdt.is_pipe)  //若命令是错误的或者是一个管道，则交由各自的处理程序处理
        return;
    setarg_command(cmdt);   //删除句中出现的多余语法元素
}

command_t parse_command(std::string command) {
    command_t ret;
    stylize_command(command);   //让句中的语法元素与其他元素分割开来
    ret.arguments = split_command(command); //构建参数列表
    ret.execfile = ret.arguments[0];    //指定要执行的命令名
    setmark_command(ret);   //标识命令中出现的语法元素
    return ret;
}
```

### src/psh_shfunc.cpp

```cpp
#include<pangda/psh.h>
#include<sys/utsname.h>
#include<pwd.h>
#include<cstring>

//获得当前用户名
std::string get_username(uid_t uid) {
    passwd *ret = getpwuid(uid);
    if (ret == NULL)
        return "";
    return std::string(ret->pw_name);
}

//获得shell提示符
//返回类似于："[username@hostname folder] #"
std::string get_tip() {
    char hostname[65];
    gethostname(hostname, 65);  //获得主机名
    char *curdir = getcwd(NULL, 0); //获得当前工作目录
    std::string ret = "[" + get_username(getuid()) + "@" + hostname + " ";

    int lastpos = 0;
    for (auto i = 0u; i < strlen(curdir); i++) {
        if (curdir[i] == '/')
            lastpos = i;
    }
    std::string where = std::string(curdir).substr(lastpos + 1);//将工作目录前的内容删除
    //构建shell提示符

    if (where == "")
        where = "/";    //这里若where是空，那么这里工作目录在根目录下
    ret += where;
    ret += "]";
    ret += ((geteuid() == 0) ? "# " : "$ ");    //当以root权限运行时，提示符为'#'，否则为'$'
    free(curdir);   //释放getcwd()以malloc()开辟的空间
    return ret;
}

//根据错误码输出错误信息
int psh_error(int error) {
    switch (error) {
    case 1:
        break;
    case 100:
        printf("psh: command not found.\n");
        break;
    case 201:case 202:
        printf("psh: file doesn\'t exist.\n");
        break;
    case 300:
        printf("psh: environment error.\n");
        break;
    case 400:case 401:case 403:
        printf("psh: error present.\n");
        break;
    }
    return -1;
}

//去除字符串两侧多余的空格
std::string string_trim(std::string s) {
    if(s.empty()) {
        return s;
    }
    s.erase(0,s.find_first_not_of(" "));
    s.erase(s.find_last_not_of(" ") + 1);
    return s;
}

//内建命令：cd
int shellfunc_cd(command_t cmdt) {
    if (cmdt.arguments.size() == 1)  {
        cmdt.arguments.push_back(".");  //若cd无参数，默认给参数.
    }
    if (chdir(cmdt.arguments[1].c_str()) != 0) {    //切换shell的工作目录，若失败则输出原因。
        perror("psh");
        return -1;
    }
    return 0;
}

//内建命令：exit
int shellfunc_exit(command_t cmdt) {
    exit(0);
    return 0;
}

//内建命令：logout
int shellfunc_logout(command_t cmdt) {
    return shellfunc_exit(cmdt);   //logout功能与exit相同，直接跳转使若exit有更多功能不必再次复制
}
```

### src/psh_explain.cpp

```cpp
#include<pangda/psh.h>
#include<sys/wait.h>
#include<dirent.h>
#include<fcntl.h>
#include<cstring>

//shell_commands：构建内建命令与处理函数的映射
std::map<std::string, std::function<int(command_t)> > shell_commands;

//分割字符串，主要用于分割PATH环境变量。让a:b:c变成["a","b","c"]的列表，方便查找
static std::vector<std::string> split_string(std::string str, char sep) {
    std::vector<std::string> ret;
    unsigned int start = 0; //用于标记上一个分割符所在的位置
    for (auto i = 0u; i < str.length(); i++) {
        if (str[i] == sep) {    //若当前指向的位置就是分割符
            ret.push_back(str.substr(start, i - start));
            start = i + 1;
        }
    }
    ret.push_back(str.substr(start, str.length()));
    return ret;
}

static std::string find_exec(command_t &cmd) {
    //检查命令是否属于内建命令
    if (shell_commands.find(cmd.execfile) != shell_commands.end()) { 
        shell_commands[cmd.execfile](cmd);  //若属于内建命令直接执行相关函数
        cmd.is_right_cmd = 1; //错误1：命令是一个内建命令，无需处理
        return "";
    }
    //命令不属于内建命令，那么依次在环境变量目录中查找命令的可执行文件
    std::vector<std::string> envpath = split_string(getenv("PATH"), ':');   //构建目录列表
    envpath.push_back("./");    //将当前目录也放在查找列表中

    for (auto it : envpath) {
        DIR *dp = opendir(it.c_str());      //打开相应目录，开始查找
        dirent *dirp;
        if (dp == NULL) {
            cmd.is_right_cmd = 300;     //错误300：环境变量中配置的PATH目录有错误
            return "";
        }
        while ((dirp = readdir(dp)) != NULL) {
            if (it != "./" && cmd.execfile == dirp->d_name) {   //若不再当前目录下查找
                std::string ret = it;
                if (ret[ret.length() - 1] != '/') {
                    ret.push_back('/');
                }
                ret = ret + cmd.execfile;
                return ret;        
            }
            //在当前目录下要保证前方有./才去执行
            if (it == "./" && cmd.execfile.length() >= 2 && cmd.execfile.substr(2) == dirp->d_name) {
                if (cmd.execfile.substr(0, 2) != "./")
                    break;
                char *current_dir = getcwd(NULL, 0);
                cmd.execfile.erase(0, 1);
                std::string ret = current_dir + cmd.execfile;
                free(current_dir);
                return ret;
            }
        }
    }
    cmd.is_right_cmd = 100;    //错误100：命令不存在
    return "";
}

int exec_command(command_t &cmd) {
    //若命令存在错误，执行错误处理程序
    if (cmd.is_right_cmd) {
        psh_error(cmd.is_right_cmd);
        return -1;
    }

    std::string path = find_exec(cmd);  //查找到命令的绝对路径

    //若命令未查找到，那么执行错误处理程序
    if (cmd.is_right_cmd) {
        psh_error(cmd.is_right_cmd);
        return -1;
    }

    //构建符合系统调用要求的参数列表
    char *arglist[cmd.arguments.size() + 1];
    char args[200][256];
    for (auto i = 0u; i < cmd.arguments.size(); i++) {
        strcpy(args[i], cmd.arguments[i].c_str());
        arglist[i] = (char *)args[i];
    }
    arglist[cmd.arguments.size()] = (char *)NULL; //参数列表要以NULL结尾，不然会出错误

    pid_t child = fork();   //调用fork()

    if (child < 0) {    //若fork<0，说明fork出错
        psh_error(200);     //错误200：无法fork()出子进程
        return -1;
    }

    //若child>0，说明在执行的是父进程
    if (child > 0) {
        //如果是后台进程，那么不再管他，输出完他的id后继续读取下一条命令
        if (cmd.is_background) {
            printf("[Process id] %d\n", (int)child);
            return 0;
        } else {
            int ret;
            //等待子进程运行完毕，不使用wait()的原因是：
            //若在此之前有一个带有&语法元素的进程已经执行完毕，那么这里调用wait获取到的是上一个
            //执行完毕的后台进程的状态，这样可能造成当前命令还没有执行完毕，
            //父进程已经退出了等待程序开始等待新的输入。
            if (waitpid(child, &ret, 0) == -1) {    //若出现错误
                perror("psh");
                return -1;
            }   
            return 0;
        }
    }

    //若child=0，说明执行的是子进程
    if (child == 0) {
        if (cmd.is_redirect_stdin) {    //若有<语法元素
            int fd = open(cmd.filename_in.c_str(), O_RDONLY);   //打开重定向的stdin
            if (fd < 0) {
                psh_error(201); //错误201：打开文件出错
                exit(0);
            }
            //不使用dup的原因是，dup只会复制文件描述符，不可以指定新的文件描述符是什么
            //这里将新的描述符指定为标准输入的文件描述符，dup2会自动关闭原来的STDIN描述符
            //并将fd的文件描述符设置为STDIN_FILENO
            dup2(fd, STDIN_FILENO);
        }

        if (cmd.is_redirect_stdout) {   //若有>语法元素
            mode_t mode = S_IWUSR | S_IRUSR | S_IRGRP | S_IROTH;    //配置文件属性
            int flag = O_WRONLY | O_CREAT;
            flag |= (cmd.stdout_mode) ? O_APPEND : O_TRUNC;
            int fd = open(cmd.filename_out.c_str(), flag, mode);
            if (fd < 0) {
                psh_error(202); //错误202：打开文件出错
                exit(0);
            }
            //不使用dup的原因同stdin
            dup2(fd, STDOUT_FILENO);
        }

        int ret = execv(path.c_str(), arglist);

        if (ret == -1) {
            perror("psh");
            exit(-1);
        }
        exit(0);    //结束掉子进程
    }
    return -1;
}
```

# 一些要注意的地方
- 父进程中应当使用`waitpid`来等待子进程结束，原因是若去使用`wait`，当之前有一个后台运行的进程已经结束时，会获取到那个进程的状态，导致之后的提示符错位。
- 构建`argv[]`时，最后一个元素应该是`(char *)NULL`，否则会出现`Bad Address`的错误。
- 其实使用带`p`的`exec`族函数可以自动在`path`环境变量中查找，不需要手动进行。
- 记得忽略掉`SIGINT`、`SIGQUIT`、`SIGSTOP`、`SIGTSTP`信号。
- 不知道还有什么了QAQ。

![这里写图片描述](https://img-blog.csdn.net/20170725091205408?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaGVwYW5nZGE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

