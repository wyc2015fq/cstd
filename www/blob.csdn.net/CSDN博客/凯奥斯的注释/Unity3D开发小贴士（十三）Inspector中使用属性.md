# Unity3D开发小贴士（十三）Inspector中使用属性 - 凯奥斯的注释 - CSDN博客





2016年11月21日 22:05:43[凯奥斯](https://me.csdn.net/ecidevilin)阅读数：2580
所属专栏：[Unity3D开发小贴士](https://blog.csdn.net/column/details/12907.html)









我们知道Unity的组件类中，public的变量可以直接在Inspector中编辑，而其他访问级别的变量，可以为它们添加[SerializeField]特性来实现同样的效果。但是如果我们希望一个变量改变的时候调用一个属性（Property）的set访问器该怎样实现呢？




首先我们需要自定义一个特性（参考[C#语法小知识（七）特性](http://blog.csdn.net/ecidevilin/article/details/52551302)）：



```
using UnityEngine;
using System.Collections;
using System;

[AttributeUsage(AttributeTargets.Field)]  
public class SetPropertyAttribute : PropertyAttribute {
	public string propertyName { get; private set;}
	public bool dirty { get; set;}
	public SetPropertyAttribute(string propertyName_)
	{
		propertyName = propertyName_;
	}
}
```

为这个特性添加了一个特性AttributeUsage，表示它的用途，只用在Field（字段/成员变量）上。
SetPropertyAttribute继承自PropertyAttribute，这是UnityEngine命名空间下的一个特性，用来在编辑器的PropertyDrawer中使用，也就是我们下面要实现的。



```
using UnityEditor;
using UnityEngine;
using System;
using System.Collections;
using System.Reflection;

[CustomPropertyDrawer(typeof(SetPropertyAttribute))]
public class AccessPropertyDrawer : PropertyDrawer {

	public override void OnGUI(Rect position, SerializedProperty property, GUIContent label)
	{
		// Rely on the default inspector GUI
		EditorGUI.BeginChangeCheck ();
		EditorGUI.PropertyField(position, property, label);

		// Update only when necessary
		SetPropertyAttribute setProperty = attribute as SetPropertyAttribute;
		if (EditorGUI.EndChangeCheck ()) {
			setProperty.dirty = true;

		} else if (setProperty.dirty) {
			object obj = property.serializedObject.targetObject;
			Type type = obj.GetType();
			PropertyInfo pi = type.GetProperty(setProperty.propertyName);
			if (pi == null)
			{
				Debug.LogError("Invalid property name: " + setProperty.propertyName + "\nCheck your [SetProperty] attribute");
			}
			else
			{
				pi.SetValue(obj, fieldInfo.GetValue(obj), null);
			}
			setProperty.dirty = false;
		}
	}
}
```
CustomPropertyDrawer用了指定这个类是SetPropertyAttribute的Drawer，所有添加了[SetProperty]特性的字段都会经过这个Drawer来绘制。


在BeginChangeCheck和EndChangeCheck之间使用PropertyField来绘制字段，就可以检查字段的值是否发生了变化。

但是变化并不会立即体现出来，需要等下次绘制的时候再调用属性的set访问器。



最后我们测试一下：



```
using UnityEngine;
using System.Collections;

public class NewBehaviourScript : MonoBehaviour {

	[SerializeField]
	[SetProperty("Test")]
	protected int _test;

	public int Test
	{
		get { return _test;}
		set { _test = value;
			Debug.Log ("Test"+_test);}
	}

	// Use this for initialization
	void Start () {
	
	}
	
	// Update is called once per frame
	void Update () {
	
	}
}
```



（注：本文参考自[https://github.com/LMNRY/SetProperty](https://github.com/LMNRY/SetProperty)。~~哈哈哈，其实就是抄的……~~）



