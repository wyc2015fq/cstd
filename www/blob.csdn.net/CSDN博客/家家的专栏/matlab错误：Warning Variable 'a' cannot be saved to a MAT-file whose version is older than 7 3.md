# matlab错误：Warning: Variable 'a' cannot be saved to a MAT-file whose version is older than 7.3. - 家家的专栏 - CSDN博客





2013年11月04日 19:45:50[依海之燕](https://me.csdn.net/yihaizhiyan)阅读数：9801








save mat文件的时候出现错误：

matlab错误提示：
**Warning: Variable 'a' cannot be saved to a MAT-file whose versionis older than 7.3.To save this variable, use the -v7.3 switch.**
**Skipping..**.




解决办法：

**打开matlab，file/preferences，general选项卡-MAT-Files,选择第一个。**



save -v7.3 TrainData.mat trainPatches

参考网址：http://hi.baidu.com/jin07081329/item/a059e687bbab51e598255fcc








