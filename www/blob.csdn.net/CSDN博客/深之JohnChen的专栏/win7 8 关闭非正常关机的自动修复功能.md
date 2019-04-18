# win7/8 关闭非正常关机的自动修复功能 - 深之JohnChen的专栏 - CSDN博客

2017年07月10日 17:16:22[byxdaz](https://me.csdn.net/byxdaz)阅读数：6910


win7/8 关闭非正常关机的自动修复功能

1、桌面右键新建一个文档文本，双击打开文件新建文本文档，复制以下命令到文本里面!

bcdedit /set {current} bootstatuspolicy ignoreallfailures

bcdedit /set {current} recoveryenabled No

2、在界面的左上角点击“文件”“另存为”，打开保存界面。在“保存格式”一行中选择“所有文件”；再把“文件名称”改为“XXX.bat”的格式，保存为一个可运行的bat文件。

3、然后桌面就出现了一个bat文件，只要双击文件就可以运行（或右键点击管理员运行），系统窗口会一闪而过，接着可以删除该bat文件,电脑以后就不会出现修复模式了。

如果要重新启用， 可以用以下命令：

bcdedit /set {current} bootstatuspolicy displayallfailures

bcdedit /set {current} recoveryenabled off

```
//启用修复功能
char szCurrentPath[MAX_PATH] = { 0 };
GetModuleFileName(NULL, szCurrentPath,MAX_PATH);
char *pFindSplitFlag = strrchr(szCurrentPath, '\\');
if (pFindSplitFlag == NULL)
{
	pFindSplitFlag = strrchr(szCurrentPath, '/');
}
if (pFindSplitFlag != NULL)
{
	*pFindSplitFlag = '\0';
}
char szCommandPath[MAX_PATH] = { 0 };
sprintf_s(szCommandPath, "%s\\%s", szCurrentPath, "StartRepairSystem.bat");
ShellExecute(0, "runas", szCommandPath,"", "", SW_SHOW);
```

```
//停用修复功能
char szCurrentPath[MAX_PATH] = { 0 };
GetModuleFileName(NULL, szCurrentPath, MAX_PATH);
char *pFindSplitFlag = strrchr(szCurrentPath, '\\');
if (pFindSplitFlag == NULL)
{
	pFindSplitFlag = strrchr(szCurrentPath, '/');
}
if (pFindSplitFlag != NULL)
{
	*pFindSplitFlag = '\0';
}
char szCommandPath[MAX_PATH] = { 0 };
sprintf_s(szCommandPath, "%s\\%s", szCurrentPath, "StopRepairSystem.bat");
ShellExecute(0, "runas", szCommandPath,"", "", SW_SHOW);
```

