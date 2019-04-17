# Unity3D开发小贴士（十六）SVN插件 - 凯奥斯的注释 - CSDN博客





2017年05月27日 14:11:02[凯奥斯](https://me.csdn.net/ecidevilin)阅读数：935
所属专栏：[Unity3D开发小贴士](https://blog.csdn.net/column/details/12907.html)









这是针对Windows下的TortoiseSVN的插件，其他平台或软件的插件，以后可能会补上（也可能不补~~，看缘分吧~~）。



```
using UnityEngine;
using System.Diagnostics;
using UnityEditor;

#if UNITY_EDITOR_WIN
public static class TortoiseSVN
{

    const string SVN_EXE = "TortoiseProc.exe";
    const string PATH_CMD_FMT = "/command:{0} /path:{1} /closeonend:0";
    const string UPDATE_CMD = "update";
    const string COMMIT_CMD = "commit";

    static string GetCurrentPath()
    {
        return AssetDatabase.GetAssetPath(Selection.activeObject);
    }

    static string GetRootPath()
    {
        return Application.dataPath;
    }

    [MenuItem("Assets/SVN/Update", false, 100)]
    public static void Update()
    {
        Process.Start(SVN_EXE, string.Format(PATH_CMD_FMT, UPDATE_CMD, GetCurrentPath()));
    }

    [MenuItem("Assets/SVN/Commit", false, 101)]
    public static void Commit()
    {
        Process.Start(SVN_EXE, string.Format(PATH_CMD_FMT, COMMIT_CMD, GetCurrentPath()));
    }

    [MenuItem("Assets/SVN/UpdateAll", false, 200)]
    public static void UpdateAll()
    {
        Process.Start(SVN_EXE, string.Format(PATH_CMD_FMT, UPDATE_CMD, GetRootPath()));
    }

    [MenuItem("Assets/SVN/CommitAll", false, 201)]
    public static void CommitAll()
    {
        Process.Start(SVN_EXE, string.Format(PATH_CMD_FMT, COMMIT_CMD, GetRootPath()));
    }
}
#endif
```





