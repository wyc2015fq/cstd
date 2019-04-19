# OPhone/Android应用程序内存泄漏确认与定位 - xqhrs232的专栏 - CSDN博客
2011年02月17日 13:39:00[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：2160
原文地址::[http://www.j2megame.com/html/xwzx/ty/1103.html](http://www.j2megame.com/html/xwzx/ty/1103.html)
1、如何确定[内存泄露](http://dev.10086.cn/cmdn/wiki/index.php?doc-view-1522)。
    突然有一天tester对我们说com.[Android](http://dev.10086.cn/cmdn/wiki/index.php?doc-view-1789).browser在反复启动/退出时有内存泄露。那我们首先需要重现一下。
[OPhone](http://dev.10086.cn/cmdn/wiki/index.php?doc-view-1401)[SDK](http://dev.10086.cn/cmdn/wiki/index.php?doc-view-1426)提供了两种命令procrank和dumpsys meminfo [pid]用来查看当前内存的使用情况。
如下：
- $ procrank                                                                 
-  PID      Vss      Rss      Pss      Uss cmdline                         
-  935   68544K   42160K   19570K   15840K system_server                   
-  1002   37600K   35124K   14912K   12804K oms.home                        
-  1221   33828K   33828K   12259K    9440K com.android.phone               
-  2537   31916K   31916K   11510K    9324K com.android.Browser             
-  2956   28768K   28768K    9034K    7152K com.hiapk.market                
- ...                                                                        
-  854     268K     268K      89K      84K /system/bin/servicemanager      
-  859     444K     444K      86K      28K /system/bin/sdm                 
-  920     268K     268K      85K      80K /system/bin/cbmon               
-  883     404K     404K      84K      28K /system/bin/sdm                 
-  857     276K     276K      81K      76K /system/bin/debuggerd           
- $                                                                          
- 
- $ dumpsys meminfo 2537                                                     
- Currently running services:                                                
-  meminfo                                                                  
- -----------------------------------------------------------------          
- DUMP OF SERVICE meminfo:                                                   
- Applications Memory Usage (kB):                                            
- Uptime: 12697585 Realtime: 49787601                                        
- 
- ** MEMINFO in pid 2537 [com.android.browser] **                            
-                     native   dalvik    other    total                      
-             size:     6432     4295      N/A    10727                      
-        allocated:     5823     3142      N/A     8965                      
-             free:        4     1153      N/A     1157                      
-            (Pss):     3396     1858     6166    11420                      
-  (shared dirty):     1260     4872      692     6824                      
-     (priv dirty):     3332     1612     1608     6552                      
- 
-  Objects                                                                   
-            Views:       50        ViewRoots:        1                      
-      AppContexts:        2       Activities:        1                      
-           Assets:        3    AssetManagers:        3                      
-    Local Binders:       14    Proxy Binders:       73                      
- Death Recipients:        2                                                 
-  OpenSSL Sockets:        0                                                 
- 
-  SQL                                                                       
-             heap:      275          dbFiles:        0                      
-        numPagers:        4   inactivePageKB:       82                      
-     activePageKB:        0                                                 
- $    
对于procrank我们主要参考Uss列，dumpsys info我们看priv dirty。 
    如果没有自动化测试工具，那只能手工重现这个问题。让我们做这个动作启动/退出浏览器50次。 做这之前我们来写一个脚本testleak.sh，记录这个过程browser内存的变化。这里我们仅仅用procrank观察，这已经足够了。
- testleak.sh                                                                
- 
- #!/system/bin/sh                                                           
- 
- while true; do                                                             
- date >> ./procrank.log                                                     
- procrank >> ./procrank.log                                                 
- echo >> ./procrank.log                                                     
- 
- sleep 5                                                                    
- done                              
这个脚本的用途是每5秒钟让系统输出一次procrank的结果到procrank.log里面。把这个脚本push到手机的某个目录下，执行：
- # ./testleak.sh &  
 重复测试步骤若干次，借助excel，我们可以从procrank.log把browser相关的结果找出来，假设是这样的。
- PID       Vss     Rss     Pss    Uss   cmdline                            
- ...                                                                        
- 2537    29048K 29048K 12735K 8628K   com.android.browser                
- 2537    29168K 29168K 12854K 8748K   com.android.browser                
- 2537    29088K 29088K 12775K 8668K   com.android.browser                
- 2537    29488K 29488K 13177K 9072K   com.android.browser                
- 2537    29348K 29348K 13037K 8932K   com.android.browser                
- 2537    29540K 29540K 13229K 9124K   com.android.browser                
- 2537    29564K 29564K 13253K 9148K   com.android.browser                
- 2537    29460K 29460K 13149K 9044K   com.android.browser                
- 2537    29460K 29460K 13149K 9044K   com.android.browser                
- 2537    29484K 29484K 13173K 9068K   com.android.browser                
- ...                                      
  我们做一张图，如下
![点击浏览下一页](http://dev.10086.cn/cmdn/wiki/uploads/201008/1282186723jXRqjgh9.jpg)
很明显内存在反复中稳定上升。如果内存在一段时间后没有下降，我们可以认为它是leak的。这里需要补充一下，有些内存的泄漏是微量的，可能需要做几百遍才能看到曲线的增长，另外测试机的使用时间也影响测试结果，一般来说，用过一段时间的手机上的测试结果更明显。
2、定位泄漏的位置。
 这里分两种情况，一种是纯[Java](http://dev.10086.cn/cmdn/wiki/index.php?doc-view-1306)的泄露，另外一种是底层（C/C++）的泄漏。如果通过代码走查的方式查找的效果不好，就需要一些辅助工具了。
    首先我们需要一个工具是mat，它是免费的，可以直接使用RCP版本或者安装其eclipse的插件，下载地址是[http://www.eclipse.org/mat/downloads.php](http://www.eclipse.org/mat/downloads.php) 。用它分析hprof文件，可以看出哪些object没有被释放。
    让我们先获得一个hprof文件，这个文件存放某个process当前内存使用的一个快照。
- # chmod 777 /data/misc                                                     
- # kill -10 2537                                                           
这样会在/data/misc目录下生成一个带当前时间的hprof文件，比如 
heap-dump-tm1275041385-pid2537.hprof 
    然后我们需要借助[OPhone SDK](http://dev.10086.cn/cmdn/wiki/index.php?doc-view-1887)提供的工具hprof-conv 来把上面的hprof转化为mat可以读取的格式。
- $ adb pull /data/misc/heap-dump-tm1275041385-pid2537.hprof desktop/                      
- ...SDK/tools# hprof-conv /data/misc/heap-dump-tm1275041385-pid2537.hprof pid2537.hprof      
用mat或eclipse打开（如果装mat插件的话），选择[Leak Suspects Report]，
![点击浏览下一页](http://dev.10086.cn/cmdn/wiki/uploads/201008/1282186723V4DPKtZI.jpg)
选择[Open Dominator Tree for entire heap]
![点击浏览下一页](http://dev.10086.cn/cmdn/wiki/uploads/201008/1282186723KDAN6eOC.jpg)
选择[Group result by...]，下拉列表中勾选[Group result by class loader]，
![点击浏览下一页](http://dev.10086.cn/cmdn/wiki/uploads/201008/1282186723p6Yr5zEd.jpg)
 我们主要看Objects这一列。
![点击浏览下一页](http://dev.10086.cn/cmdn/wiki/uploads/201008/1282186723TsXBONXA.jpg)
因为hprof保存的是内存快照，我们可以在程序启动前获得一个hprof文件，在做完50次的测试后再获得一个文件，两者对比，如果出现object的数量与次数一起递增，那么相对应的class就是泄漏的对象了，参照代码，很容易我们就能找到泄漏的地方。但是如果无论执行多少遍，objects的数量都没有变化，这样就不得不考虑是不是native的问题了。
    这种情况得特别对待，最直白的定位方法就是试，把认为[内存泄漏](http://dev.10086.cn/cmdn/wiki/index.php?doc-view-1673)的地方给注释掉，重新打包再测。其实如果有相当大把握的怀疑，这种方法是最有效的。但是很多次后还没有确定泄漏的范围就尝试其他方法吧。
    OPhone SDK提供了一些获得内存信息的api，使用如下:
- ActivityManager activityManager = (ActivityManager) getSystemService(ACTIVITY_SERVICE);    
- MemoryInfo info = new MemoryInfo();                                                        
- activityManager.getMemoryInfo(info);                                                       
- Log.i("memory free:", "" + info.availMem);                                                
在测试步骤涉及的相关代码中加上这些log，可以得出一系列内存变化的情况，这样也可以看出来是在哪里出现的问题。
解决泄漏不在本文讨论范围。 
附录：
·       VSS - Virtual Set Size 虚拟耗用内存（包含共享库占用的内存） 
·       RSS - Resident Set Size 实际使用物理内存（包含共享库占用的内存） 
·       PSS - Proportional Set Size 实际使用的物理内存（比例分配共享库占用的内存） 
USS - Unique Set Size 进程独自占用的物理内存（不包含共享库占用的内存）
//==================================================
网上文章推荐
1。Android_内存泄漏调试.pdf
2。Android内存泄漏问题的排查.pdf
3。Eclipse自带的MAT说明文档
4。Android 中用 bitmap 内存溢出处理 （一）
[http://www.androidegg.com/portal.php?mod=view&aid=1275](http://www.androidegg.com/portal.php?mod=view&aid=1275)
5。Android 中用 bitmap 内存溢出处理 （二）
[http://www.androidegg.com/portal.php?mod=view&aid=1274](http://www.androidegg.com/portal.php?mod=view&aid=1274)
