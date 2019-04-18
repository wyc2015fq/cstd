# ACE_Configuration_Heap类 - 深之JohnChen的专栏 - CSDN博客

2011年12月23日 14:03:52[byxdaz](https://me.csdn.net/byxdaz)阅读数：2885标签：[string																[import																[cmd																[os																[平台																[pair](https://so.csdn.net/so/search/s.do?q=pair&t=blog)](https://so.csdn.net/so/search/s.do?q=平台&t=blog)](https://so.csdn.net/so/search/s.do?q=os&t=blog)](https://so.csdn.net/so/search/s.do?q=cmd&t=blog)](https://so.csdn.net/so/search/s.do?q=import&t=blog)](https://so.csdn.net/so/search/s.do?q=string&t=blog)
个人分类：[ACE网络编程](https://blog.csdn.net/byxdaz/article/category/628882)


该类用语访问合操纵配置信息,它们的接口由ACE_Configuration类来定义的;

1、ACE_Configuration_Heap:它可以用在几乎所有的平台上,在内存中保存所有配置信息.可以对内存的分配进行定制,使用持久的后备存储,但是最常用的是动态分配的堆内存,所以这个类的名字里面含有一个Heap;

2、ACE_Configuration_Win32Registry:它只能用在Windows平台上,它实现了ACE_Configuration类的接口,可以访问合操纵注册表中的数据;

3、ACE_Registry_ImpExp:这个类使用了一种文本格式,其中的每个值都有类型信息.

4、ACE_Ini_ImpExp:这个类使用的是较老的Windows"INI"文件的格式,其中的值没有相关联的类型信息,因此,不管原来的类型是什么,使用ACE_Ini_ImpExp导出的配置数据总是作为字符串数据导入;

下面的例子是ACE_Configuration_Heap类的使用样例,用于从配置文件中读取配置信息;配置文件的格式如下:

[SECTION]

"Key1"="Value1"

"Key2"="Value2"

"Key2"="Value2"

例子代码:
#include "ace/Get_Opt.h"
#include "ace/Log_Msg.h"
#include "ace/SString.h"
#include "ace/OS_String.h"
#include "ace/Configuration.h"
#include "ace/Configuration_Import_Export.h"

int ACE_TMAIN(int argc, ACE_TCHAR** argv)
{
 int opt = 0;
 ACE_TCHAR strCfgFile[128] = "";
 ACE_TCHAR strSegName[128] = "";

 ACE_TCHAR options[] = "f:F:s:S:vV";
 ACE_Get_Opt cmd_opts(argc, argv, options);

 //set long options
 cmd_opts.long_option(ACE_TEXT("config"), 'f', ACE_Get_Opt::ARG_REQUIRED);
 cmd_opts.long_option(ACE_TEXT("segment"), 's', ACE_Get_Opt::ARG_REQUIRED);

 cmd_opts.long_option(ACE_TEXT("CONFIG"), 'f', ACE_Get_Opt::ARG_REQUIRED);
 cmd_opts.long_option(ACE_TEXT("SEGMENT"), 's', ACE_Get_Opt::ARG_REQUIRED);

 while((opt = cmd_opts()) != EOF)
 {
  switch(opt)
  {
   case 'f':
   case 'F':
    {
     ACE_OS_String::strcpy(strCfgFile, cmd_opts.opt_arg());
    }
    break;

   case 's':
   case 'S':
    {
     ACE_OS_String::strcpy(strSegName, cmd_opts.opt_arg());
    }
    break;

   case 'v':
   case 'V':
    {
     ACE_DEBUG((LM_INFO, ACE_TEXT("test the configuration with ace\n")));
    }
    break;
  }
 }

 ACE_DEBUG( (LM_INFO, ACE_TEXT("You entered ---> CfgFileName: %s;  SegName: %s\n"), strCfgFile, strSegName) );

 //declare a configuration object
 ACE_Configuration_Heap config;

 if(config.open() == -1)
 {
  ACE_ERROR_RETURN( (LM_ERROR, ACE_TEXT("--->failed to open the configuration object[%p]"), ACE_TEXT("open_config")), -1);
 }

 //declare a importer of config, and import the configuration-file
 ACE_Registry_ImpExp config_importer(config);
 if(config_importer.import_config(ACE_TEXT(strCfgFile)) == -1)
 {
  ACE_ERROR_RETURN( (LM_ERROR, ACE_TEXT("--->failed to import config-file %s[%p]\n"), strCfgFile, ACE_TEXT("import_config()")), -2);
 }

 //declare a section-key pair, and open this section with the config-object;
 ACE_Configuration_Section_Key status_section;
 if(config.open_section(config.root_section(), ACE_TEXT(strSegName), 0, status_section) == -1)
 {
  ACE_ERROR_RETURN( (LM_ERROR, ACE_TEXT("--->failed to open section %s[%p]\n"), strSegName, ACE_TEXT("open_section(LOG_IMPORT)")), -3 );
 }

 //get port
 ACE_TString port = "";
 if(config.get_string_value(status_section, ACE_TEXT("Port"), port) == -1)
 {
  ACE_ERROR_RETURN( (LM_ERROR, ACE_TEXT("--->failed to get HOST PORT[%p]\n"), ACE_TEXT("get_string_value(Port)")), -4 );
 }

 //get host ip
 ACE_TString strHostIP = "";
 if(config.get_string_value(status_section, ACE_TEXT("HostIP"), strHostIP) == -1)
 {
  ACE_ERROR_RETURN( (LM_ERROR, ACE_TEXT("--->failed to get HOST IP[%p]\n"), ACE_TEXT("get_string_value()")), -5 );
 }

 ACE_DEBUG( (LM_INFO, ACE_TEXT("IP: %s\nPort: %d\n"), strHostIP.c_str(), ACE_OS::atoi(port.c_str())) );
 return 0;
}


