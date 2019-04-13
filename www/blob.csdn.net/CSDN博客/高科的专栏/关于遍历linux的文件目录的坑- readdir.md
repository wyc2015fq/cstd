
# 关于遍历linux的文件目录的坑- readdir - 高科的专栏 - CSDN博客

2015年08月20日 12:21:29[高科](https://me.csdn.net/pbymw8iwm)阅读数：5917


去年给公司写了一个配置服务器，目的是解决运维的工作量太大，而且传送服务器需要的配置文件需要脚本传送到各个服（每个服ip不一样，需要scp），然后再刷新通知各个GameServer，中间有没有传送失败并不得知，而且维护相当麻烦，所以我写了这个服务器，所有区服的配置都在这里边，不同的区服通过编号创建不同的目录，在对应目录下放服务器的配置，架构我就不说了，有兴趣的可以私聊。
运行了很久了，突然遇到一个bug，不同的配置服务器发送的文件顺序是不一样。当初是配置名不能要求一样的，但是QA不小心在1服的etc下放了A配置，又在etc下的activities目录里又放了A配置，并且当初是用文件名作为主键，而不是绝对路径。因此会出现个别配置服务器跟新到的是etc下的A配置，有些更新到的是etc下activities的A配置，

```python
#include <iostream>
#include <string>
using namespace std;
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <dirent.h>
void search(string commonXmlPath){
{
    string ectPath = commonXmlPath;
    string subDistrictXmlPath;
    DIR* dir_pointer = opendir(ectPath.c_str());
    if (!dir_pointer)
            return;
    for (dirent* dp = readdir(dir_pointer); dp != NULL; dp = readdir(dir_pointer))
    {
            std::string filename = dp->d_name;
            if (filename == "." || filename == "..")
            {
                   continue;
            }
                        if (dp->d_type == 8)//?录镁
                        {
                                int split = filename.find('.');
                                if (split == -1)
                                        continue;
                                string pp =  ectPath + filename;
                                cout << pp.c_str() <<"\n";
                        }
                        if(dp->d_type == 4)//目录拢卢?么录?酶碌梅                        {
                                //每赂枚募镁录?路戮露
                                subDistrictXmlPath = ectPath + string("/") + dp->d_name;
                                search(subDistrictXmlPath);
                        }
    }
    //closedir(dir_pointer);
}
int main(){
        search(string("/data/home/android/configserver/common_xmls/etc"));
        return 0;
}
```
暂时写了这个程序测试，发现不同的服务器上运行的结果可能不一样，这种递归遍历的结果让你突然发现自己以前给自己埋了一个坑，以后会注意的，等我找出合理的排序规则函数吧

