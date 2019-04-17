# Untiy3D开发小贴士（一）OnEnabled与OnDisabled - 凯奥斯的注释 - CSDN博客





2016年09月11日 15:48:45[凯奥斯](https://me.csdn.net/ecidevilin)阅读数：5476
所属专栏：[Unity3D开发小贴士](https://blog.csdn.net/column/details/12907.html)









设某个组件为NewBehaviour

OnEnabled被调用的情况：

1、GameObject添加组件的时候，即AddComponet<NewBehaviour>
 ；

2、包含NewBehaviour的Prefab被实例化的时候；

3、已添加NewBehaviour的GameObject由未激活到已激活的时候，即setActive(true) ；

4、NewBehaviour由不可用到可用的时候，即enabled=true。


其中2、3会调用该GameObject及其所有子对象的OnEnabled。

OnDisabled被调用的情况：

1、GameObject被销毁；

2、NewBehaviour被销毁；

3、已添加NewBehaviour的GameObject由已激活到未激活的时候，即setActive(false)；

4、NewBehaviour由不可用到可用的时候，即enabled=false。


其中1、3会调用该GameObject及其所有子对象的OnEnabled。


一个简单的测试代码：



```
using UnityEngine;
using System.Collections;

public class NewBehaviour : MonoBehaviour {
	static int idx = 0;
	void Awake()
	{
		Debug.Log ("Awake");
	}
	// Use this for initialization
	void Start () {
		if (idx == 0) {
			Debug.Log ("NB");
			NewBehaviour nb = new NewBehaviour ();
			Debug.Log ("SetActive");
			gameObject.SetActive (false);
			gameObject.SetActive (true);
			Debug.Log ("enabled");
			this.enabled = false;
			this.enabled = true;
			Debug.Log ("Destroy");
			Destroy (this);
			idx = 1;
		}
		if (idx == 1) {
			Debug.Log ("Add");
			gameObject.AddComponent<NewBehaviour> ();
			Debug.Log ("Destroy go");
			Destroy (gameObject);
			idx = 2;
		}
		if (idx == 2) {
			Debug.Log ("Instantiate");
			GameObject obj = Resources.Load<GameObject> ("New Prefab");
			Instantiate (obj);
			idx = 3;
		}
	}
	
	// Update is called once per frame
	void Update () {
	
	}

	void OnEnable()
	{
		Debug.Log ("OnEnable" + idx);
	}
	void OnDisable()
	{
		Debug.Log("OnDisable" + idx);
	}
}
```

此外，场景里新建一个GameObject，添加NewBehaviour，在Assets里新建Resources目录，将新建的GameObject保存为Prefab。


点击运行，打印结果：



```
Awake
OnEnable0
NB
[Warning]You are trying to create a MonoBehaviour using the 'new' keyword.  This is not allowed.  MonoBehaviours can only be added using AddComponent().  Alternatively, your script can inherit from ScriptableObject or no base class at all
SetActive
OnDisable0
OnEnable0
enabled
OnDisable0
OnEnable0
Destroy
OnDisable0
Add
Awake
OnEnable1
Destroy go
OnDisable1
Instantiate
Awake
OnEnable2
```

取消运行后，会增加一行：



`OnDisable3`
由此我们可以验证上面的论点，顺便得出以下结论：


1、Awake在OnEnable之前，OnEnable在Start之前。

2、New一个Component不仅会报警告，并且并不会调用OnEnable、OnDisable等方法。

最后补充几点：

1、被销毁时先调用OnDisable再调用OnDestroy。

2、当前组件会在Start执行完之后被销毁，具体销毁时机不明……总之不会执行Update。

3、如果把Destroy换成DestroyImmediate会报错，因为当前组件已经被销毁，gameObject无法被引用到。



