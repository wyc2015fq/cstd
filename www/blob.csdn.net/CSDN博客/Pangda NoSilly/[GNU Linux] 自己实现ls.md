# [GNU/Linux] 自己实现ls - Pangda NoSilly - CSDN博客

2017年07月23日 22:28:19[hepangda](https://me.csdn.net/hepangda)阅读数：304
个人分类：[Linux																[C++](https://blog.csdn.net/hepangda/article/category/6946147)](https://blog.csdn.net/hepangda/article/category/6991396)


# 随便扯点什么

实际上刚开始让我实现`ls（1）`我是拒绝的，因为，你不能让我写，我就马上去写，第一我要看一下，因为我不愿意写完了以后再加一些特技上去，比如`ls`忽然“`Segmentation Fault`”了，很懵逼、很低能，这样用过`ls`的人出来一定会骂我，根本没有这样辣鸡的`ls`，就证明这`ls`一点也不好用，就是在骗人。后来组长告诉我只用实现`-a -l -R`就好，我写了两天左右，感觉还不错，后来我在使用的时候就没有加特技，就很舒服。

# 源代码

### src/Makefile

```
SOURCES = $(wildcard *.cpp)
OBJECTS = $(patsubst %.cpp, %.o, $(SOURCES))
CXXFLAG = -std=c++14 -I ../include -Wall -O2

pls: $(OBJECTS)
    g++ $(CXXFLAG) -o pls $(OBJECTS)

$(OBJECTS): $(SOURCES)
    g++ $(CXXFLAG) -c *.cpp

.PHONY: clean
clean :
    -rm $(OBJECTS)
```

### include/pangda/ls.h

```cpp
#pragma once
#include<string>
#include<fcntl.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<sys/ioctl.h>
#include<unistd.h>
#include<dirent.h>
#include<algorithm>
#include<vector>
#include<cmath>
#include<map>

using std::string;

//表示参数常量
const int PARAM_a = 0x01; //-a --all        显示所有文件
const int PARAM_A = 0x02; //-A --almost-all 显示除过.和..之外的所有文件
const int PARAM_f = 0x04; //-f              不进行排序
const int PARAM_r = 0x08; //-r              逆序排序显示
const int PARAM_R = 0x10; //-R              递归地显示目录下所有目录的文件
const int PARAM_l = 0x20; //-l              以列表的形式显示

struct list_type {
    int rows, cols, judge;          //几行几列
    std::vector<int> widths_list;   //宽度表，指每一列应该是多少宽度
};

struct param_t {
    std::string path;   //ls哪一个路径
    int param_flag;     //都有哪些参数
};

struct files_t {
    std::string fst_name;           //文件名
    unsigned short fst_namelen;     //文件名长度，用来计算列宽
    mode_t fst_mode;                //文件权限
    char fst_type;                  //文件类型，l表示链接，d表示目录，-表示其他
    long fst_creatime;              //文件的更改时间
    off_t fst_size;                 //文件大小
    nlink_t fst_linknum;            //文件被硬链接的次数
    std::string fst_uid;            //文件所属的用户的uid
    std::string fst_gid;            //文件所属的用户组的gid
};

struct filelist_t {
    static const bool FLT_LIST = true;
    static const bool FLT_MAP = false;
    bool type = FLT_MAP;                                //文件列表的类型，true表示nlist可用，false代表nmap可用
    std::vector<files_t> nlist;
    std::map<std::string,  std::vector<files_t> > nmap;
};

typedef std::vector<std::string> vstr_t;
typedef std::vector<int> vint_t;
typedef std::vector<std::vector<int> > vvint_t;
typedef std::vector<files_t> vft_t;

//函数定义
int output(param_t param, const filelist_t flt);
param_t set_param(const int argc, char *argv[]);
bool check_param(const param_t msg, const int param);
filelist_t build_filelist(const param_t param);
string get_username(uid_t uid);
string get_groupname(gid_t gid);
```

### src/ls.cpp

```cpp
#include<pangda/ls.h>

int main(int argc, char *argv[]) {
    param_t param = set_param(argc, argv);         //分析带有的参数
    if (param.path[param.path.size() - 1] != '/')
        param.path += '/';  //若要显示的目录不以/结尾，那么手动在之后加上'/'

    auto fli = build_filelist(param);   //根据参数构建文件列表
    output(param, fli);     //按指定参数形式输出文件列表
    return 0;
}
```

### src/ls_param.cpp

```cpp
#include<pangda/ls.h>
#include<cstring>

static const char *PARAMSTR_a = "all";
static const char *PARAMSTR_A = "almost-all";

//分析并返回是否具有参数，返回值是参数做|运算后的数值
static int analyse_param(const char *args) {
    if (args[0] != '-')     //若参数不以-开头，说明他不是一个参数
        return -1;
    if (args[1] == '-') {   //若以--开头，说明需要以字符串来判断
        if (strcmp(args + 2, PARAMSTR_a) == 0)
            return PARAM_a;
        if (strcmp(args + 2, PARAMSTR_A) == 0)
            return PARAM_A;
        return 0;
    }

    int ret = 0;
    while (*args++) {   //依次判断，因为可能出现-alRf的形式
        switch (*args) {
        case 'a':
            ret |= PARAM_a;
            break;
        case 'A':
            ret |= PARAM_A;
            break;
        case 'f':
            ret |= PARAM_f;
            break;
        case 'r':
            ret |= PARAM_r;
            break;
        case 'R':
            ret |= PARAM_R;
            break;
        case 'l':
            ret |= PARAM_l;
        }
    }
    return ret;
}

//检查msg是否具有param所指定的参数
bool check_param(const param_t msg, const int param) {
    return (msg.param_flag & param);
}

//设置参数
param_t set_param(int argc, char *argv[]) {
    param_t ret = { "./", 0 };  //若没有默认路径，则默认以./作为路径

    for (int i = 1; i < argc; i++) {
        int t = analyse_param(argv[i]);
        if (t == -1)
            ret.path = argv[i];     //若返回值是-1，说明这个参数是要显示的路径
        else
            ret.param_flag |= t;    //反之则说明应当设定参数
    }
    return ret;
}
```

### src/ls_common.cpp

```cpp
#include<pangda/ls.h>
#include<pwd.h>
#include<grp.h>
#include<errno.h>
#include<queue>
using std::pair;
using std::make_pair;
using std::queue;

//打开一个目录并返回DIR流
static DIR *open_folder(const param_t param) {
    DIR *ret;
    if ((ret = opendir(param.path.c_str())) == NULL) {  //尝试打开
        return NULL;    //出错则返回NULL
    }
    return ret;
}

//返回一个文件的属性字，即目录返回d，链接返回l等等
static char check_type(const mode_t mode) {
    if (S_ISLNK(mode))
        return 'l';
    if (S_ISDIR(mode))
        return 'd';
    if (S_ISCHR(mode))
        return 'c';
    if (S_ISBLK(mode))
        return 'b';
    return '-';
}

//以map形式构建输出文件列表
static filelist_t build_filelist_map(param_t param, DIR *where) {
    filelist_t ret;
    ret.type = false;
    queue<pair<string, DIR *> > q;
    where = opendir(param.path.c_str());
    q.push(make_pair(param.path, where));   //将第一个目录放入队列中
    while (!q.empty()) {
        pair<string, DIR *> qt = q.front();
        q.pop();

        files_t temp;
        struct stat stbuf;
        dirent *thisfile;
        bool param_a = check_param(param, PARAM_a);     //检查是否具有a参数
        bool param_A = check_param(param, PARAM_A);     //检查是否具有A参数

        if (qt.second == NULL) {    //若没能打开DIR流，则继续构建下一个
            continue;
        }

        while ((thisfile = readdir(qt.second)) != NULL) {
            temp.fst_name = thisfile->d_name;   //设置名字
            if (!(param_A || param_a)) {        //判断是否存在aA参数，若无这两个参数，直接跳过.开头的文件
                if (temp.fst_name[0] == '.')
                    continue;
            } else if (param_A) {
                if (temp.fst_name == "." || temp.fst_name == "..")
                    continue;
            }
            temp.fst_namelen = thisfile->d_reclen;
            string statp = thisfile->d_name;        //构建绝对路径的文件名
            if (qt.first[qt.first.size() - 1] == '/')
                statp = qt.first + statp;
            else
                statp = qt.first + '/' +statp;

            if (lstat(statp.c_str(), &stbuf) == -1) {   //尝试获得文件属性
                continue;
            }
            temp.fst_creatime = stbuf.st_ctime;
            temp.fst_gid = get_groupname(stbuf.st_gid);
            temp.fst_uid = get_username(stbuf.st_uid);
            temp.fst_linknum = stbuf.st_nlink;
            temp.fst_mode = stbuf.st_mode;
            temp.fst_type = check_type(stbuf.st_mode);
            temp.fst_size = stbuf.st_size;
            ret.nmap[qt.first].push_back(temp);     //将获得的文件输行放入列表中

            //若是一个目录，而且不是.和..目录，将之放入队列中，构建他的列表
            if (temp.fst_type == 'd' && temp.fst_name != "." && temp.fst_name != "..") {
                string newpath = qt.first + temp.fst_name;
                if (newpath[newpath.size() - 1] != '/')
                    newpath += '/';
                DIR *newdir = opendir(newpath.c_str());
                q.push(make_pair(newpath, newdir));
            }
        }
        closedir(qt.second);    //关闭文件流

        if (check_param(param, PARAM_f)) {  //若设定了f参数，那么就不进行排序
            continue;
        }

        //lambda：排序的比较函数 -> bool
        static auto sort_cmp = [](const files_t a, const files_t b) -> bool {
            return a.fst_name < b.fst_name;
        };
        static auto sort_revcmp = [](const files_t a, const files_t b) -> bool {
            return a.fst_name > b.fst_name;
        };

        if (check_param(param, PARAM_r))    //若设定了r参数，则使用逆序排序函数
            sort(ret.nmap[qt.first].begin(), ret.nmap[qt.first].end(), sort_revcmp);
        else    //反之使用正常的排序函数
            sort(ret.nmap[qt.first].begin(), ret.nmap[qt.first].end(), sort_cmp);
    }

    return ret;
}

//以list形式构建输出文件列表
static filelist_t build_filelist_list(param_t param, DIR *where) {
    filelist_t ret = { filelist_t::FLT_LIST };
    files_t temp;
    struct stat stbuf;
    dirent *thisfile;
    //检查是否具有指定参数
    bool param_a = check_param(param, PARAM_a);
    bool param_A = check_param(param, PARAM_A);

    chdir(param.path.c_str());
    if (where == NULL) {    //若没有打开文件流，则报错
        perror("ls");
        exit(-1);
    }

    //以下内容与map形式类似
    while ((thisfile = readdir(where)) != NULL) {
        temp.fst_name = thisfile->d_name;
        if (!(param_A || param_a)) {
            if (temp.fst_name[0] == '.')
                continue;
        } else if (param_A) {
            if (temp.fst_name == "." || temp.fst_name == "..")
                continue;
        }
        temp.fst_namelen = thisfile->d_reclen;

        if (lstat(thisfile->d_name, &stbuf) == -1) {
            return filelist_t();
        }
        temp.fst_creatime = stbuf.st_ctime;
        temp.fst_gid = get_groupname(stbuf.st_gid);
        temp.fst_uid = get_username(stbuf.st_uid);
        temp.fst_linknum = stbuf.st_nlink;
        temp.fst_mode = stbuf.st_mode;
        temp.fst_type = check_type(stbuf.st_mode);
        temp.fst_size = stbuf.st_size;
        ret.nlist.push_back(temp);
    }

    closedir(where);
    if (check_param(param, PARAM_f)) {
        return ret;
    }
    auto sort_cmp = [](const files_t a, const files_t b) -> bool {
        return a.fst_name < b.fst_name;
    };
    auto sort_revcmp = [](const files_t a, const files_t b) -> bool {
        return a.fst_name > b.fst_name;
    };

    if (check_param(param, PARAM_r))
        sort(ret.nlist.begin(), ret.nlist.end(), sort_revcmp);
    else
        sort(ret.nlist.begin(), ret.nlist.end(), sort_cmp);

    return ret;
}

filelist_t build_filelist(const param_t param) {
    DIR *where = open_folder(param);        //打开目录
    if (check_param(param, PARAM_R))        //若设定了R参数，则以map形式构建
        return build_filelist_map(param, where);
    return build_filelist_list(param, where);   //反之以list形式构建
}

//获得用户名
string get_username(uid_t uid) {
    passwd *ret = getpwuid(uid);
    if (ret == NULL)
        return "";
    return string(ret->pw_name);
}

//获得用户组名
string get_groupname(gid_t gid) {
    group *ret = getgrgid(gid);
    if (ret == NULL)
        return "";
    return string(ret->gr_name);
}
```

### src/ls_display.cpp

```cpp
#include<pangda/ls.h>
#include<cstdio>
#include<ctime>
#include<cstring>

//获得终端窗口的大小，用于计算列宽。
static winsize get_winsize() {
    winsize ret;
    ioctl(STDIN_FILENO, TIOCGWINSZ, (char *)&ret);
    return ret;
}

//根据文件的权限属性设定颜色
static int setc(mode_t mode, string filename) {
    if (S_ISDIR(mode))  //若为目录
        return printf("\033[36m");
    if (S_ISLNK(mode))  //若为符号链接
        return printf("\033[33;35m");
    if ((mode & S_IXUSR) || (mode & S_IXOTH) || (mode & S_IXGRP))   //若为可执行文件
        return printf("\033[32m");
    return printf("\033[0m");   //若为其他文件
}

//取消设定的颜色
static int canc() {
    return printf("\033[0m");
}

//test_tier辅助函数
inline static unsigned int find_pos(const int x, const int y, const int tier) {
    return (x + tier * y);
}

//根据宽度表尝试构建宽度表
static list_type test_tier(const int tier, const vint_t widths) {
    vvint_t build;
    list_type ret { int(ceil(widths.size() / double(tier))),
                    tier, 0, vint_t() };

    for (int i = 0; i < tier; i++) {
        build.push_back(vint_t());
        int col_max = 0;
        for (int j = 0; j < ret.rows; j++) {
            if (find_pos(i, j, tier) >= widths.size()) {
                break;
            }
            int t = widths[find_pos(i, j, tier)];

            build[i].push_back(t);
            if (t > col_max)
                col_max = t;
        }

        ret.widths_list.push_back(col_max);
        ret.judge += col_max + 2;
    }

    ret.judge -= 2;
    return ret;
}

//计算在当前文件下，列宽应该为多少
static list_type calc_cols(const vft_t files) {
    int limit = get_winsize().ws_col;   //获取终端的最大列宽
    vint_t widths;
    for (auto i : files) {
        widths.push_back(i.fst_name.size());    //构建文件长度表
    }

    list_type test;
    int t = 1;
    while (true) {  //测试一行中最多能有多少列
        test = test_tier(t + 1, widths);
        if (test.judge <= limit)
            t++;
        else
            break;
    }
    return test_tier(t, widths);
}

//按普通形式输出一个文件列表
static int outone_normal(const vft_t filist) {
    list_type lt = calc_cols(filist);   //计算该文件列表可以有多少列

    int i = 0;
    bool fir = true;
    for (auto it : filist) {
        if (fir)
            fir = false;
        else
            printf("  ");
        setc(it.fst_mode, it.fst_name); //根据权限来指定颜色
        printf("%-*s", lt.widths_list[i], it.fst_name.c_str());
        canc(); //取消掉颜色
        if (++i == lt.cols)
            putchar('\n'), i = 0, fir = true;
    }
    putchar('\n');
    return 0;
}

//输出是否具有指定的权限，若有输出right,反之输出-
static int put_power(const mode_t mode, const int power, const char right) {
    if (mode & power)
        return putchar(right);
    return putchar('-');
}

//计算数字有多少位
static int calc_numbits(int num) {
    char buf[500];
    sprintf(buf, "%d", num);
    return strlen(buf);
}

//根据长整型的时间返回字符串形式的时间
static string calc_time(long times) {
    string ret = "";
    char buf[500];
    strcpy(buf, ctime(×));
    int sz = strlen(buf);
    for (int i = 4; i < sz - 9; i++) {
        ret += buf[i];
    }
    return ret;
}

//按list形式输出一个list
static int outone_list(const vft_t filist) {
    vint_t wcalc[4];
    //简单的计算中间的列宽
    for (auto it : filist) {
        wcalc[0].push_back(it.fst_uid.size());
        wcalc[1].push_back(it.fst_gid.size());
        wcalc[2].push_back(calc_numbits(int(it.fst_size)));
        wcalc[3].push_back(calc_numbits(int(it.fst_linknum)));
    }
    int widths[4] {
        *max_element(wcalc[0].begin(), wcalc[0].end()),
        *max_element(wcalc[1].begin(), wcalc[1].end()),
        *max_element(wcalc[2].begin(), wcalc[2].end()),
        *max_element(wcalc[3].begin(), wcalc[3].end())
    };

    for (auto it : filist) {
        putchar(it.fst_type);                   //输出文件的类型
        put_power(it.fst_mode, S_IRUSR, 'r');   //输出权限
        put_power(it.fst_mode, S_IWUSR, 'w');
        put_power(it.fst_mode, S_IXUSR, 'x');
        put_power(it.fst_mode, S_IRGRP, 'r');
        put_power(it.fst_mode, S_IWGRP, 'w');
        put_power(it.fst_mode, S_IXGRP, 'x');
        put_power(it.fst_mode, S_IROTH, 'r');
        put_power(it.fst_mode, S_IWOTH, 'w');
        put_power(it.fst_mode, S_IXOTH, 'x');
        putchar(' ');
        printf("%*d ", widths[3], (int)it.fst_linknum);        
        printf("%-*s ", widths[0], it.fst_uid.c_str());
        printf("%-*s ", widths[1], it.fst_gid.c_str());
        printf("%*d ", widths[2], (int)it.fst_size);
        printf("%s ", calc_time(it.fst_creatime).c_str());
        setc(it.fst_mode, it.fst_name);
        printf("%s\n", it.fst_name.c_str());
        canc();
    }
    return 0;
}

//输出list
static int output_list(param_t param, const filelist_t filist) {
    if (check_param(param, PARAM_l))
        return outone_list(filist.nlist);
    return outone_normal(filist.nlist);
}

//输出map
static int output_map(param_t param, const filelist_t filist) {
    for (auto it : filist.nmap) {
        printf("%s:\n", it.first.c_str());
        if (check_param(param, PARAM_l))
            outone_list(it.second);
        else
            outone_normal(it.second);
        printf("\n");
    }
    return 0;
}

//通用output函数
int output(param_t param, const filelist_t flt) {
    if (flt.type == filelist_t::FLT_LIST)
        return output_list(param, flt);
    return output_map(param, flt);
}
```

# 一些小问题
- 参数可能是`ls -alRf`这样的形式，所以参数处理的时候要注意。
- 使用`lstat`函数的时候要注意文件是不是在当前工作目录下面，如果没有在当前工作目录下面，要记得构建路径，防止`lstat`函数出现错误。
- 输出的时候要计算列宽，不能自信指定多少的列宽够用，不然会出现严重的排版错误。
- 设置颜色之后要立即取消掉，不然在个别终端配色会导致空格上有颜色，所有情况下都会使`l`参数显示时下一个文件的权限等信息染上颜色。
- 任何可能出错的地方记得要有合理的错误处理，不然将很难定位到代码出现问题的具体地点。
- 关于调试：可以尝试着在`printf纠错法`找到具体函数之后，依次的在句子后面加上`fprintf(stderr, "xxx\n", ...); sleep(50);`来定位错误地点，若不`sleep`错误信息可能就跟其他输出混淆在一起。
- 做的还不够好啊QAQ。
- 所以说，它是长这样的： 
![列表显示](https://img-blog.csdn.net/20170723222623470?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaGVwYW5nZGE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![终端比较宽时](https://img-blog.csdn.net/20170723222642219?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaGVwYW5nZGE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![终端比较窄时](https://img-blog.csdn.net/20170723222657402?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaGVwYW5nZGE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

