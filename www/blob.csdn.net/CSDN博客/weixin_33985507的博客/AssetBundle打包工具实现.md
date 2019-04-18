# AssetBundle打包工具实现 - weixin_33985507的博客 - CSDN博客
2018年12月25日 19:49:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：9
[什么是AssetBundle，点击查看](https://www.jianshu.com/p/ee1d59a92c7f)
先看一下工具的使用
![642887-431257cbcd96be16.png](https://upload-images.jianshu.io/upload_images/642887-431257cbcd96be16.png)
商用为目的的打包工具
#### 同步读取AssetBundle
- 创建单例
```
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
public  class  Singleton<T> where T : class, new()
    {
        private static T _instance;
        private static readonly object syslock = new object();
        public static T Instance()
        {
            if (_instance == null)
            {
                lock (syslock)
                {
                    if (_instance == null)
                    {
                        _instance = new T();
                    }
                }
            }
            return _instance;
        }
    }
```
- 利用单例进行文件读取单例类的创建
```
using System.IO;
using UnityEngine;
/// <summary>
/// 文件读取
/// </summary>
public class LocalFileMgr : Singleton<LocalFileMgr> {
#if UNITY_EDITOR
 
#if UNITY_STANDALONE_WIN
    public readonly string LocalFilePath = Application.dataPath + "/../AssetBundles/Windows/";
#elif UNITY_ANDROID
    public readonly string LocalFilePath = Application.dataPath + "/../AssetBundles/Windows/Android/";
#elif UNITY_IPHONE
    public readonly string LocalFilePath = Application.dataPath + "/../AssetBundles/Windows/iOS/";
#endif
#elif UNITY_ANDROID || UNITY_IPHONE || UNITY_STANDALONE_WIN
    public readonly string LocalFilePath = Application.persistentDataPath + "/";
#endif
    /// <summary>
    /// 读取本地文件到byte数组
    /// </summary>
    /// <param name="path"></param>
    /// <returns></returns>
    public byte[] GetBuffer(string path)
    {
        byte[] buffer = null;
        using (FileStream fs = new FileStream(path, FileMode.Open))
        {
            buffer = new byte[fs.Length];
            fs.Read(buffer, 0, buffer.Length);
        }
       return buffer;
    }
}
```
- 加载文件资源
```
void Start () {
        byte[] buffer = LocalFileMgr.Instance().GetBuffer(@"F:\项目\Unity\YD_Destop_ImmersionClick\Assetbundles\Windows\Role\item.assetbundle");
        AssetBundle bundle = AssetBundle.LoadFromMemory(buffer);
        GameObject go = bundle.LoadAsset("Item") as GameObject;
        bundle.Unload(false);
        Instantiate(go);
    }
```
##### 同步加载封装
- 资源加载类
```
using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
/// <summary>
/// 加载资源包
/// </summary>
public class AssetBundlLoad:IDisposable  {
    private AssetBundle bundle;
    // 构造方法
    public AssetBundlLoad(string assetBundlePath)
    {
        string fullPath = LocalFileMgr.Instance().LocalFilePath + assetBundlePath;
        bundle = AssetBundle.LoadFromMemory(LocalFileMgr.Instance().GetBuffer(fullPath));
    }
    public void Dispose()
    {
        if (bundle != null) bundle.Unload(false);
    }
    // 
    public T LoadAsset<T>(string name) where T : UnityEngine.Object
    {
        if (bundle == null) return default(T);
        return bundle.LoadAsset(name) as T;
    }
}
```
- 资源加载管理类
```
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
public class AssetBundleMgr : Singleton<AssetBundleMgr> {
    public GameObject Load(string path, string name)
    {
        using (AssetBundlLoad loder = new AssetBundlLoad(path))
        {
            return loder.LoadAsset<GameObject>(name);
        }
    }
}
```
- 加载场景Scene
```
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
public class LoadScene : MonoBehaviour {
    void Start () {
        GameObject obj = AssetBundleMgr.Instance().Load(@"Role\item.assetbundle", "Item"); 
        Instantiate(obj);
    }
```
#### 异步读取AssetBundle
- 单例类改造
```
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
public  class  Singleton<T> where T : class, new()
    {
        private static T _instance;
        private static readonly object syslock = new object();
        public static T Instance
        {
        get {
            if (_instance == null)
            {
                lock (syslock)
                {
                    if (_instance == null)
                    {
                        _instance = new T();
                    }
                    
                }            
            }
            return _instance;
        }
                    
        }
```
- 异步加载控制器
```
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
public class AssetBundleMgr : Singleton<AssetBundleMgr> {
    public GameObject Load(string path, string name)
    {
        using (AssetBundlLoad loder = new AssetBundlLoad(path))
        {
            return loder.LoadAsset<GameObject>(name);
        }
    }
    public AssetBundleLoaderAsync LoadAsyncObj(string path, string name)
    {
        GameObject obj = new GameObject("AssetBundleLoadAsync");
        AssetBundleLoaderAsync async =  obj.AddComponent<AssetBundleLoaderAsync>();
        async.Init(path, name);
        return async;
    }
}
```
- Start异步加载
```
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
public class LoadScene : MonoBehaviour {
    void Start () {c'c
       // 异步加载 
        AssetBundleLoaderAsync async = AssetBundleMgr.Instance.LoadAsyncObj(@"Role\item.assetbundle", "Item");
        async.OnLoadComplete = OnLoadComplete;
    }
```
