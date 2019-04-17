# Unity3D开发小贴士（十五）状态机组件 - 凯奥斯的注释 - CSDN博客





2016年12月19日 08:19:08[凯奥斯](https://me.csdn.net/ecidevilin)阅读数：999
所属专栏：[Unity3D开发小贴士](https://blog.csdn.net/column/details/12907.html)









# 在Unity3D中，我们除了可以为GameObject添加组建，还可以为AnimatorController中的状态添加组建。这样我们就可以在状态（机）的不同阶段内执行不同的操作。




导入Characters包，并将ThirdPersonController添加到场景中。

打开Animator窗口，单击场景中的ThirdPersonController对象，Animator中会出现如下图所示

![](https://img-blog.csdn.net/20161219071951106)


单击Airborne，Inspector中会出现

![](https://img-blog.csdn.net/20161219072117779)


点击Add Behaviour，便可以创建StateMachineBehaviour。

默认的脚本文件中包含这些：



```
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class TestStateBehaviour : StateMachineBehaviour {

	 // OnStateEnter is called when a transition starts and the state machine starts to evaluate this state
	//override public void OnStateEnter(Animator animator, AnimatorStateInfo stateInfo, int layerIndex) {
	//
	//}

	// OnStateUpdate is called on each Update frame between OnStateEnter and OnStateExit callbacks
	//override public void OnStateUpdate(Animator animator, AnimatorStateInfo stateInfo, int layerIndex) {
	//
	//}

	// OnStateExit is called when a transition ends and the state machine finishes evaluating this state
	//override public void OnStateExit(Animator animator, AnimatorStateInfo stateInfo, int layerIndex) {
	//
	//}

	// OnStateMove is called right after Animator.OnAnimatorMove(). Code that processes and affects root motion should be implemented here
	//override public void OnStateMove(Animator animator, AnimatorStateInfo stateInfo, int layerIndex) {
	//
	//}

	// OnStateIK is called right after Animator.OnAnimatorIK(). Code that sets up animation IK (inverse kinematics) should be implemented here.
	//override public void OnStateIK(Animator animator, AnimatorStateInfo stateInfo, int layerIndex) {
	//
	//}
}
```

OnStateEnter是在状态进入的时候会调用。

OnStateUpdate是状态激活后每一帧调用。

OnStateExit是在状态退出的时候会调用。

OnStateMove也会状态激活后每一帧调用，在Animator.OnAnimatorMove之后，用于处理或影响根动作（root motion）。

OnStateIK当设置动画IK（Inverse Kinematics反向动力学）时会调用，在Animator.OnAnimatorIK之后。

此外还有两个回调：

OnStateMachineEnter当进入状态机的时候会调用。


OnStateMachineExit当推出状态机的时候会调用。





我们还可以在Layer上添加StateMachineBehaviour，只要这一层上的某个状态满足了上面的那些时机时便会调用那些回调。




StateMachineBehaviour常常会用于：


1、当状态进入或退出时播放音效；

2、在适当的状态时执行特定的测试（例如着地检测）；

3、激活并控制与某个状态相关联的特效。




示例，一个简单的音效组件：



```
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class StateSound : StateMachineBehaviour {

	public AudioClip EnterSound;
	public AudioClip ExitSound;
	private AudioSource _audioSource; 

	private AudioSource lazyGetAudioSource(Animator animator)
	{
		if (!_audioSource)
		{
			_audioSource = animator.GetComponent<AudioSource> ();
		}
		if (!_audioSource) {
			_audioSource = animator.gameObject.AddComponent<AudioSource> ();
		}
		return _audioSource;
	}

	public override void OnStateEnter (Animator animator, AnimatorStateInfo stateInfo, int layerIndex)
	{
		if (EnterSound)
		{
			lazyGetAudioSource(animator).PlayOneShot (EnterSound);
		}
	}
	public override void OnStateExit (Animator animator, AnimatorStateInfo stateInfo, int layerIndex)
	{
		if (ExitSound)
		{
			lazyGetAudioSource(animator).PlayOneShot (ExitSound);
		}
	}
}
```


为Airborne添加并设置这个组件

![](https://img-blog.csdn.net/20161219081415192)


（图中Jump和Land音效也包含在Characters包中）



