# 有oracle 10g，但没有安装arcgis，又想使用空间数据库的解决方案 - 左直拳的马桶_日用桶 - CSDN博客
2016年04月01日 19:00:26[左直拳](https://me.csdn.net/leftfist)阅读数：2021
我在一台虚拟机中部署系统进行测试，配置如下： 
OS：WIN2008 R2 SP1 X64 
DB: oracle 12c 
结果系统报错，查找原因，原来是oracle里还不支持arcgis的一些所谓的空间数据库。想想也是，虚拟机里没装arcgis，当然不支持了。
`{"ORA-28595: Extproc 代理: DLL 路径无效\nORA-06512: 在 \"SDE.ST_GEOMETRY_SHAPELIB_PKG\", line 70\nORA-06512: 在 \"SDE.ST_GEOMETRY_OPERATORS\", line 68\n"}`
但是，难道我真的要装个arcgis吗？那么庞大的一个东西，能不能装成功还不知道呢！
在网上寻寻觅觅，基本都是说改那个所谓listener.ora文件，指向arcgis的dll。我先从开发服务器上拷了一份arcgis的bin到虚拟机，开发服务器上的系统一直运行良好，DB是oracle 10g。到了虚拟机以后，不行；怀疑是acgis的版本不支持oracle 12c，于是又拷了一份支持12c的arcgis的bin，也不成功！
结果从17点搞到21点，我眼冒金星，都快晕倒了！这个系统是部署来下周演示的，清明节不想加班，时间不多了；况且还有另外一个项目的文档要写。项目经理是招我进来的恩公，可不能辜负人家。我感到腹背受敌！
第二天，我冷静了下来。在宿主机的oracle 10g上试了一下，结果OK了。可能oracle 12c的做法并不是修改listener.ora，因为其他项目组有台服务器同时装了12c 和 arcgis的服务器上，我看它的listener.ora，并没有改动，但确实是可以支持SDE的。时间紧迫，只能以后再去寻找答案了。
记录一下有oracle 10g，但没有安装arcgis，又想使用空间数据库的解决方案：
1、将arcgis的bin拷贝到目标机器，如 
C:\ArcGIS\Desktop10.0\Bin
2、修改listener.ora，如 
 D:\oracle\product\10.2.0\db_1\NETWORK\ADMIN\listener.ora
```
(SID_DESC =
      (PROGRAM = EXTPROC)
      (ENV = "EXTPROC_DLLS=C:\ArcGIS\Desktop10.0\Bin\st_shapelib.dll")
      (SID_NAME = PLSExtProc)
      (ORACLE_HOME = D:\oracle\product\10.2.0\db_1)
    )
```
3、重启oracle的监听程序（仅重启监听程序就可以了）
如此，OK矣。你可以用sde账号登录oracle，运行
`select sde.ST_AsText(SDE.ST_Geometry('POINT (10 10)', 0)) from dual;`
可得正确结果。
