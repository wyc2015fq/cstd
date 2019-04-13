
# 话单采集测试(第一次更新修正用例部分，更新了程序Bug追加了一些功能) - .NET博文收藏 - CSDN博客


2008年06月10日 12:44:00[hejishan](https://me.csdn.net/hejishan)阅读数：727标签：[测试																](https://so.csdn.net/so/search/s.do?q=测试&t=blog)[list																](https://so.csdn.net/so/search/s.do?q=list&t=blog)[path																](https://so.csdn.net/so/search/s.do?q=path&t=blog)[initialization																](https://so.csdn.net/so/search/s.do?q=initialization&t=blog)[input																](https://so.csdn.net/so/search/s.do?q=input&t=blog)[string																](https://so.csdn.net/so/search/s.do?q=string&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=input&t=blog)[
																															](https://so.csdn.net/so/search/s.do?q=initialization&t=blog)
[
				](https://so.csdn.net/so/search/s.do?q=path&t=blog)
[
			](https://so.csdn.net/so/search/s.do?q=path&t=blog)
[
		](https://so.csdn.net/so/search/s.do?q=list&t=blog)
[
	](https://so.csdn.net/so/search/s.do?q=测试&t=blog)
话单文件采集测试：
1.基本测试：
基础流：设置本地时间，如：20071015 16:00:00 启动话单文件生成程序
2.规则测试：
备选流1：当日话单序号到达程序设定的最大值时，如：9999。话单文件序号将重置清零(20071015_0000)重新计数，测试是否可以正常采集
备选流2：跨天采集，当日话单文件到次日话单文件，如：20071015-20071016，修改测试机本地时间23:59:00
16日零时，话单文件序号将重置清零(20071016_0000)重新计数，测试是否可以正常采集
备选流3：当日(20071015)话单到达文件设定的最大值，还未到次日(20071016)，如：9999。
测试数据先将文件序号清零(20071015_0000)，查看是否可正常采集
其次，当到达次日时再次清零(20071016_0000)，查看是否可以正常采集
测试1=基础流+备选流1
测试2=基础流+备选流2
测试3=基础流+备选流3
3.容错性测试：
备选流1：跨月采集，设定话单文件为2007_10_31 23:58:00，查看是否可正常采集
备选流2：跨年采集，设定话单文件为2007_12_31 23:58:00，查看是否可正常采集
备选流3：当日(2007_12_31 23:58:00)话单到达文件设定的最大值，还未到次日(2008_01_01 00:00:00)，如：9999。
测试数据先将文件序号清零(20071231_0000)，查看是否可正常采集
其次，当到达次日时再次清零(20080101_0000)，查看是否可以正常采集
测试1=基础流+备选流1
测试2=基础流+备选流2
测试3=基础流+备选流3
4、最小话单序号采集，话单测试数据文件命名为：
第一组数据：6660 6661 0001 0002
第二组数据：6660 6661 0010 0011
第三组数据：6660 6661 0010 0011 0001
第四组数据：6660 6661 0010 0011 0035
5、话单文件序号有间隔
第一组数据：6660 6668 6669 6671
第二组数据：0000 0010 0005 0006
第三组数据：9999 0010 0014 0018
6、复合数据测试
跨天采集+当日序号清零
垮月采集+当日序号清零
跨年采集+当日序号清零
当日历史数据采集+当日序号清零
当月历史数据采集+当日序号清零
当年历史数据采集+当日序号清零
5、综合场景测试
复合所有采集规则
下面与各位分享一下，这个采集程序的部分Py脚本(有些地方写的很烂)
本次追加的功能：
1、更改的循环的方式，Python int类型不能支持百万级循环问题(改为多层嵌套，通过小时来控制循环时间)
2、修改了Sleep时间不支持毫秒级运算的问题
3、增加自动加载某些配置文件的方法
4、设置了可配置的话单最小序号控制阀值
5、修改了一些数据错误与Bug
6、增加了异常错误的日志分析
7、修改了更佳友好的操作界面，对一些输入值的判定条件进行了修改
config/config.ini
[config]
initialization_path=E://ftp//
data_prefixion=sdf
data_infix=rrse
data_postfix=.txt
initialization_number=0
run_count(hours)=1
min_circle_time=1
[workpath]
programme_workpath=C://Program Files//Huawei//infoX-InfoSpy//
ismg_probe_module=ismg_probe_module.ini
[switch]
the_second_day_min_count=0
the_current_day_min_count=1
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)\#-*- coding: cp936 -*-
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)\#===============================================================================
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)\#程序需求：话单时间片替换程序
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)\#1.可自动识别任意话单的时间位置
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)\#2.可自动识别各种话单文件的长度，生成已当前本地时间为时间片的话单数据(替换原始话单中的时间列)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)\#3.当日话单达到最大值9999时，话单文件序号自动清零重新计数，如：0000，0001
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)\#4.当话单文件跨天时，文件序号自动清零重新计数，如：0000，0001
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)\#5.提供参数配置功能(config.ini/run_log.txt/原始话单文件)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)\#6.提供日志输出功能:可查看最后生成的话单文件关键参数与循环计数
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)\#7.min_circle_time(最小间隔时间支持毫秒级运算)，可以指定任意位数的float参量
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)\#8.加入隔天话单数据重新计数后，随机产生最小话单序号的为任意值(在配置文件中制作开关)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)\#9.加入当天话单数据，随机产生跨号段序号(在配置文件中制作开关)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)\#===============================================================================
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)importos,time,sys
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)classDataBase():
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)def__init__(self):
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)config_path=open(".//config//config.ini",'r')
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)config_list=config_path.readlines()
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)\#通过配置文件获得话单数据的初始化文件路径
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)self.initialization_path=config_list[1][config_list[1].strip().find("=")+1:-1]
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)\#得到话单文件名前缀
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)self.data_prefixion=config_list[2][config_list[2].strip().find("=")+1:-1]
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)\#得到话单文件名日期
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)nowtime=time.localtime()
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)\#初始化参数
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)data1=""
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)foriinrange(3):
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)\#如果日期为单数需要补零
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)data1+="%02d"%nowtime[i]
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)self.data_data=data1
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)\#得到话单文件中缀
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)self.data_infix=config_list[3][config_list[3].strip().find("=")+1:-1]
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)\#得到话单文件后缀
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)self.data_postfix=config_list[4][config_list[4].strip().find("=")+1:-1]
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)\#得到话单文件当前数据号
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)self.data_number="%04d"%int(config_list[5][config_list[5].strip().find("=")+1:])
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)\#print self.data_number
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)self.data_path=self.initialization_path+"config/"+self.data_prefixion+self.data_data+self.data_infix+self.data_number+self.data_postfix
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)\#得到程序需要运行的次数
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)self.data_count=config_list[6][config_list[6].strip().find("=")+1:-1]
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)\#print self.data_count
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)\#得到话单文件生成的最小间隔时间
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)self.min_circle_time=config_list[7][config_list[7].strip().find("=")+1:-1]
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)\#得到程序工作路径
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)self.programme_workpath=config_list[10][config_list[10].strip().find("=")+1:-1]
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)\#得到短信网关配置文件名字
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)self.ismg_probe_module=config_list[11][config_list[11].strip().find("=")+1:-1]
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)\#得到隔天话单最小计数序号
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)self.the_second_day_min_count=config_list[14][config_list[14].strip().find("=")+1:-1]
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)\#得到当天话单数据间隔序号
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)self.the_current_day_min_count=config_list[15][config_list[14].strip().find("=")+2:]
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)defcollection_config(self):
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)lastseq=raw_input("请输入上一次网关采集模块LastSeq的值:")
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)\#循环判断输入条件，只有满足条件才能执行之后的程序
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)while(True):
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)try:
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)lastseq=int(lastseq)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)break
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)except:
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)lastseq=raw_input("LastSeq不能为空或者非数字型字符，请重新输入:")
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)continue
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)count=[]
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)path=self.programme_workpath+"config/"+self.ismg_probe_module
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)\#如果读取的文件失败，抛出异常供使用者进行检查
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)try:
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)list=open(path,'r')
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)data=list.readlines()
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)list.close()
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)foriinrange(len(data)-1):
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)if'LastSeq'indata[i]:
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)count.append(i)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)foriinrange(len(count)):
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)data.remove(data[count[i]])
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)data.insert(count[i],"LastSeq=%s"%(lastseq))
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)list_new=open(path,'w')
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)foryindata:
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)\#print y
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)list_new.writelines(y)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)list_new.close()
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)print"----------------------------------------------------------------"
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)print"ismg_probe_module.ini文件，LastSeq已被重置为%s!请重新启动网关采集模块！"%(lastseq)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)except:
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)print"路径错误！请检查程序工作路径是否正确,再开始执行程序=",path
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)defget_localtime(self):
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)\#获取本地时间，For Example：2008/10/02 00:44:11
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)nowtime=time.localtime()
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)\#print nowtime
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)data1,time1="",""
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)foriinrange(3):
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)\#如果获得时间是单数，则需要补位0
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)data1+=("%02d"%nowtime[i])+'/'
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)\#获得时间片的日期测试print data1[0:-1]
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)foriinrange(3,6):
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)\#如果获得日期是单数，则需要补位0
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)time1+=("%02d"%nowtime[i])+':'
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)\#获得时间片的时间测试print time1[0:-1]
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)\#时间片的数据组合
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)time_area=data1[0:-1]+""+time1[0:-1]
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)returntime_area
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)defdata_control(self):
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)\#得到需要重写话单文件的路径及命名self.data_count
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)count=0
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)data_time=self.data_data
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)data_number=self.data_number
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)dd=1
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)\#去掉了原来使用[i]循环，改为使用可配置LastCount来控制间隔循环次数
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)\#由于需求修改为隔天采集序号可能是任意值，序号生成可能乱序，为了测试这种情况
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)\#程序增加了对隔天最小话单序号的配置int(self.the_second_day_min_count)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)\#以及当天话单序号的间隔值int(self.the_current_day_min_count)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)last_count=0
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)foroinrange(int(self.data_count)):
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)foriinrange(int(float(3600)/float(self.min_circle_time))):
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)\#循环开始话单生成的序号,加入了可配置的隔天序号，话单数据重置后初始化单生成的序号(可以指定任意的值开始)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)\#i += 1
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)last_time=data_time
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)last_data_number=data_number
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)temp=dd
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)last_count=last_count+int(self.the_current_day_min_count)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)ifdd!=1:
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)last_count1=last_count-end_count
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)\#增加话单数据文件序号隔天重新计数
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)nowtime=time.localtime()
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)data_time=""
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)foryinrange(3):
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)\#如果日期为单数需要补零
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)data_time+="%02d"%nowtime[y]
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)\#如果当前取得时间是当天的
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)ifdata_time==last_time:
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)last_data_number=data_number
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)else:
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)\#重置循环次数,隔天生成的话单文件序号可配置last_count=
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)\#end_count = i
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)end_count=last_count
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)last_count=int(self.the_second_day_min_count)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)\#制作标识为
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)dd=2
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)\#如果不相等，则重置时间序号
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)last_data_number=data_number=0
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)\#处理生成的文件路径数字超过4位
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)rebulid_number="%04d"%int(str((int(last_data_number)+last_count))[-4:])
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)rebulid_path=self.initialization_path+self.data_prefixion+data_time+self.data_infix+rebulid_number+self.data_postfix
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)\#从初始化路径中得到数据
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)try:
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)tel_node=open(self.data_path,"r")
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)tel_node_new=open(rebulid_path,'w')
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)tel_list=tel_node.readlines()
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)foriinrange(len(tel_list)):
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)\#数据的第一行
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)new_list=tel_list[i].split(',')
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)forjinrange(len(tel_list[i].split(','))):
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)\#赛选存在日期的序列，i等于时间片序列的纵行位置
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)if'/'intel_list[i].split(',')[j]:
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)\#从原始列表中删除原始日期
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)new_list.remove(tel_list[i].split(',')[j])
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)\#替换重新组合的日期时间
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)time_area=self.get_localtime()
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)new_list.insert(j,time_area)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)\#得到新的数据列表print new_list
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)\#合成数据从列表中读出数据添加分割符
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)new_string=""
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)foriinnew_list:
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)new_string+=str(i)+','
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)\#print new_string[0:-1]
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)tel_node_new.writelines(new_string[0:-1])
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)count+=1
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)print"Execute!count=%s,current_time=%s,current_number=%s"%(count,data_time,rebulid_number)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)log_path=open(".//config//run_log.txt",'w')
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)log_path.writelines("Execute!count=%s,current_time=%s,current_number=%s"%(count,data_time,rebulid_number))
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)time.sleep(int(self.min_circle_time))
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)except:
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)errors=self.data_path
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)if__name__=="__main__":
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)control=DataBase()
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)\#control.collection_config()
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)print"----------------------------------------------------------------"
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)print"测试前请先核对您的文件路径是否正确！"
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)print"----------------------------------------------------------------"
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)printcontrol.data_path
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)print"----------------------------------------------------------------"
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)choose=raw_input("""确认请输入1 退出请输入2 请输入您需要执行的选择：""")
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)\#循环判断输入条件，只有满足条件才能执行之后的程序
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)while(True):
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)try:
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)choose=int(choose)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)break
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)except:
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)lastseq=raw_input("choose条件错误")
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)continue
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)ifchoose==1:
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)print"程序开始执行"
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)control.data_control()
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)ifchoose==2:
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)print"程序立即关闭"
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)sys.exit()
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)




