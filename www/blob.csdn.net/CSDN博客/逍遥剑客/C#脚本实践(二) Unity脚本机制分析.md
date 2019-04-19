# C#脚本实践(二): Unity脚本机制分析 - 逍遥剑客 - CSDN博客
2010年11月28日 19:31:00[xoyojank](https://me.csdn.net/xoyojank)阅读数：11918
目前我了解到使用Mono做脚本的只有Unity和SecondLife, SpringEngine也有个扩展实现.
当然, 最成熟的还是Unity
于是下载了一个看看它的脚本是怎么应用的
它的编辑器界面很简洁, 实际操作了一下才发现所有的功能其本上都是由inspector面板完成
所有的GameObject都是由Component所组成, 如Mesh/Physics/Audio/Script等等
脚本都是从MonoBehaviour派生的一个实现, 在其中实现上层定义的各种接口, 如Start/Update等
脚本中定义的变量会直接反映到Inspector面板上, 如
using UnityEngine;
using System.Collections;
public class RotateBehaviourScript : MonoBehaviour {
	public float speed = 5.0f;
	// Use this for initialization
	void Start () {
	
	}
	
	// Update is called once per frame
	void Update () {
		transform.Rotate(0, speed * Time.deltaTime, 0);
	}
}
![](http://hi.csdn.net/attachment/201011/28/0_1290942509ZqBJ.gif)
这得益于.Net语言反射机制的强大, 也是我为什么使用.net开发编辑器的原因之一
之前还一直在想Unity是怎么动态载入脚本的, 因为mono里我只找到一个mono_domain_assembly_open的函数
在看了下测试工程的文件夹结构, 发一个文件: New Unity Project/Library/ScriptAssemblies/Assembly-CSharp.dll
看来这个就是脚本编译后的东西. 用Reflector查看了一下, 果然我写的两个测试脚本都在其中:
![](http://hi.csdn.net/attachment/201011/28/0_1290943181vx9t.gif)
另外, UnityEngine.dll里的类全是InternalCall的声明, 这正好印证了上一篇C++导出到Mono的调用猜想
知道了这个, Unity的脚本机制也就差不多了: 
1. C++实现引擎底层
2. Mono这边把需要暴露给脚本的类/方法/变量都再声明一次(自动生成应该也是可行的)
3. 定义脚本基类, 所有脚本对象都从它派生
4. 编辑器中发现有脚本更改就在后台重新编译assembly dll, 编译完了用Mono API重新载入
嗯, 中间那一层还是有不少需要实现的东西~
