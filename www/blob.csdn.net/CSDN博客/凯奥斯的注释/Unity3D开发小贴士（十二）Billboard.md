# Unity3D开发小贴士（十二）Billboard - 凯奥斯的注释 - CSDN博客





2016年11月19日 22:35:24[凯奥斯](https://me.csdn.net/ecidevilin)阅读数：1695
所属专栏：[Unity3D开发小贴士](https://blog.csdn.net/column/details/12907.html)









# 游戏中常常需要让某个对象始终朝向摄像机，在世界UI上使用的比较广泛，例如对话框或者血条。

常见的实现方法有两种。

一种是直接改变rotation：



```
using UnityEngine;
using System.Collections;

[ExecuteInEditMode]
public class BillboardRotation : MonoBehaviour {

	public Camera cameraLook;
	// Use this for initialization
	void Start () {
	
	}
	
	// Update is called once per frame
	void Update () {
		if (cameraLook == null) {
			cameraLook = Camera.main;
		}
		transform.rotation = cameraLook.transform.rotation;
	}
}
```

第二种是使用LookAt方法：


```
using UnityEngine;
using System.Collections;

[ExecuteInEditMode]
public class BillboardPosition : MonoBehaviour {
	public Camera cameraLook;
	// Use this for initialization
	void Start () {
	
	}
	
	// Update is called once per frame
	void Update () {
		if (cameraLook == null) {
			cameraLook = Camera.main;
		}
		transform.LookAt (cameraLook.transform.position);
	}
}
```








