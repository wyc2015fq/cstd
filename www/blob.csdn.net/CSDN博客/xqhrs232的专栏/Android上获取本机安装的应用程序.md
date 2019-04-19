# Android上获取本机安装的应用程序 - xqhrs232的专栏 - CSDN博客
2011年07月06日 18:38:40[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：840
原文地址::[http://hi.baidu.com/txl1417/blog/item/8dadf91fea97f6e21ad57677.html](http://hi.baidu.com/txl1417/blog/item/8dadf91fea97f6e21ad57677.html)
在google上输入以上的关键字+ android，可以搜到的代码:
List<PackageInfo> packs =getPackageManager().getInstalledPackages(0);
虽然，有些代码号称可以过滤掉系统自身的应用程序,但是只要细看代码就会发现，好像里面的那个布尔变量没有起到什么过滤的作用。
　　方法一:通过获取的安装包(包括安装的与系统自身的应用程序)，对其android.content.pm.PackageInfo的packageName 进行过滤,
但是Anrodi与google的包比较容易过滤掉，但需要对不同产商进行适配;
　　方法二:枚举 \data\app 文件夹，其下的*.apk文件 就是本机安装的软件，而\system\App下的*.apk文件就是系统自带的软件，
由于普通的程序是无法读取这两个文件夹的，需要root权限吧，因此该方法也行不通(用File.listFiles()+ android.permission.FACTORY_TEST 权限 还是不能枚举出文件)
虽然不能枚举文件夹 "\data\app\",但我们可以访问（read）文件夹"\data\app\"下的所有文件,因此可以用如下的方法来获取本机上已经安装的软件(排除系统的应用程序):
PackageManager pckMan = getPackageManager();
List<PackageInfo> packs =pckMan.getInstalledPackages(0);
count = packs.size();
String name;
int installedNum = 0;
for(int i = 0; i < count; i++) { 
PackageInfo p = packs.get(i); 
if(p.versionName == null) 
continue;
//判断该软件包是否在/data/app目录下
File f1 =new File( "/data/app/" +  p.packageName +".apk");
if(!f1.exists())continue;
installedNum++;
/**
* 应用程序的名字
*/
name =p.applicationInfo.loadLabel(pckMan).toString();
}
方法三、可以通过uid来过滤系统系统的安装包,因为android系统中的uid从 1000 ～ 9999 都是给系统程序保留的，所以只要判断package的uid > 10000即可判定该程序是非系统程序。
if(p.applicationInfo.uid > 10000)
//非系统程序
uid是程序在安装的时候由系统分配的。
方法四：这个方法应该是最优的。
ApplicationInfo appInfo = p.applicationInfo;
/**
* Value for{@link #flags}: if set, this application is installed in the
* device'ssystem image.
*/
if((appInfo.flags & ApplicationInfo.FLAG_SYSTEM) >0)
//系统程序
else
//不是系统程序
