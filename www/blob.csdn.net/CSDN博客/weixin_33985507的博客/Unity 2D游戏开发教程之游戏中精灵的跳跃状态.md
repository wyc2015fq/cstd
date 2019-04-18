# Unity 2D游戏开发教程之游戏中精灵的跳跃状态 - weixin_33985507的博客 - CSDN博客
2015年09月08日 10:43:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：5
## Unity 2D游戏开发教程之游戏中精灵的跳跃状态
## 精灵的跳跃状态
为了让游戏中的精灵有更大的活动范围，上一节为游戏场景添加了多个地面，于是精灵可以从高的地面移动到低的地面处，如图2-14所示。但是却无法从低的地面移动到高的地面，因为当前的游戏精灵只能左右移动，即left和right。为了解决这个问题，本节就来为精灵添加跳跃状态。
**![](https://images2015.cnblogs.com/blog/632949/201509/632949-20150908104112075-989309222.png)**
**图2-14  精灵从一个地面移动到另一个地面**
**（1****）**如果要为精灵添加跳跃状态，即jump，就不得不再引入其它状态：
- q   landing：用于表示精灵接触到地面的这种状态。为了阻止精灵在跳跃到空中的时候再次跳跃，就需要在精灵做跳跃动作之前，确认其接触到了地面。
**提示：**有些游戏允许精灵在空中的时候再跳跃一次，也就是所谓的二级跳。例如，《天天酷跑》中，精灵就需要有二级跳的能力，因为有些场景光是跳跃一次无法跳过悬崖，或者吃到金币，如图2-15所示。
**![](https://images2015.cnblogs.com/blog/632949/201509/632949-20150908104119294-1329053372.png)**
**图2-15  《天天酷跑》中，需要连续跳跃来越过断崖的精灵**
- q   falling：用于表示精灵在空中的状态，处于此种状态下的精灵只可能过渡到landing和kill状态，也就是说精灵在空中的时候，无法产生其它动作行为，如left和right。
**（2****）**为脚本PlayerStateController中定义的动画状态枚举类型添加3种新的状态：jump、landing和falling，并且设置当玩家按下键盘上的空格键时，精灵会进入跳跃状态。脚本PlayerStateController中的部分代码如下：
- 01     using UnityEngine;
- 02     using System.Collections;
- 03    
- 04     public class PlayerStateController : MonoBehaviour
- 05     {
- 06              //定义游戏人物的各种状态
- 07              public enum playerStates
- 08              {
- 09                       …                                                             //省略
- 10                       jump,                                                       //表示跳跃
- 11                       landing,                                                   //表示落地
- 12                       falling,                                                     //表示降落过程
- 13              }
- 14              …                                                                       //省略
- 15              void LateUpdate ()
- 16              {
- 17                       …                                                             //省略
- 18                       //当玩家按下键盘上的空格键时，进入跳跃状态
- 19                       float jump = Input.GetAxis("Jump");
- 20                       if(jump > 0.0f)
- 21                       {
- 22                                 if(onStateChange != null)
- 23                                          onStateChange(PlayerStateController.playerStates.jump);
- 24                       }
- 25              }
- 26     }
**（3****）**在Hierarchy视图里，新建一个Empty对象，命名为SceneryToggler，并拖动它到Player对象下，使其成为Player对象的子对象。为其添加Box Collider 2D组件，并在Inspector视图里，设置此组件的属性，如图2-16所示。
- q   选中Is Trigger复选框；
- q   Size：x:1,y:2；
- q   Center：x:0,y:1；
**![](https://images2015.cnblogs.com/blog/632949/201509/632949-20150908104131044-241374284.png)**
**图2-16  SceneryToggler对象上，组件属性的设置**
如此这般设置，相当于为Player对象添加了一个Collider检测框，如图2-17所示，可以看出除了精灵自身的Polygon Collider 2D的线框外，外层还包裹了矩形的线框。它在后面将用于检测精灵与场景中其它对象的碰撞。
**![](https://images2015.cnblogs.com/blog/632949/201509/632949-20150908104138997-755788437.png)**
**图2-17  精灵对象外层包裹的矩形线框**
读者此时心中一定在疑惑，既然精灵本身就有Collider，为什么还要添加一个Collider呢？答案是，精灵是有实体的，因此即使其它对象与精灵发生了接触，它们也不会“交叉”，因此就永远不会触发我们期望的landing状态。但是对于Empty对象而言，就没有这种担心了，因为它根本没有实体，因此会与其它对象发生“交叉”，然后触发landing状态。
**（4****）**在Unity里，单击Edit|Project Settings|Tags and Layers命令，打开Tag&Layers对话框，为Tags属性添加两个元素：Platform和DeathTrigger，如图2-18所示。
**![](https://images2015.cnblogs.com/blog/632949/201509/632949-20150908104147559-289642445.png)**
**图2-18  在Tag&Layers对话框里，为Tags属性添加两个元素**
然后修改场景里地面对象的Tag属性为Platform，Death Trigger对象的Tag属性为DeathTrigger。
**（5****）**在Project视图里的Scripts文件夹下，新建一个C#脚本，命名为PlayerColliderListener，为此脚本添加下面的代码：
- 01     using UnityEngine;
- 02     using System.Collections;
- 03    
- 04     public class PlayerColliderListener : MonoBehaviour
- 05     {
- 06              public PlayerStateListener targetStateListener = null;                          //表示精灵对象
- 07         //进入碰撞检测区域时，调用此函数
- 08              void OnTriggerEnter2D( Collider2D collidedObject )
- 09         {
- 10                       switch(collidedObject.tag)
- 11             {
- 12                                 //当精灵落到地面上时，触发landing动画状态
- 13                                 case "Platform":
- 14                                         targetStateListener.onStateChange(PlayerStateController.playerStates.landing);
- 15                                 break;
- 16                       }
- 17              }
- 18              //离开碰撞检测区域时，调用此函数
- 19              void OnTriggerExit2D( Collider2D collidedObject)
- 20              {
- 21                       switch(collidedObject.tag)
- 22                       {
- 23                                 //当精灵离开地面是，触发falling动画状态
- 24                                 case "Platform":
- 25                                         targetStateListener.onStateChange(PlayerStateController.playerStates.falling);
- 26                                 break;
- 27                                 //当精灵离开的是Death Trigger对象，则触发kill动画状态
- 28                                 case "DeathTrigger":
- 29                                         targetStateListener.onStateChange(PlayerStateController.playerStates.kill);
- 30                                 break;      
- 31                       }        
- 32              }
- 33     }
将此脚本赋予Player对象的子对象SceneryToggler，选中后者，然后在Inspector视图里设置此脚本组件的Target State Listener属性为Player对象，如图2-19所示。
**![](https://images2015.cnblogs.com/blog/632949/201509/632949-20150908104158512-518173129.png)**
**图2-19  设置脚本组件里的属性**
对于此脚本，有以下几点需要说明：
- q   脚本08行定义的OnTriggerEnter2D()，说明当与SceneryToggler对象发生碰撞的对象的Tag属性是Platform的时候（精灵与地面接触），精灵就会进入landing状态；
- q   脚本19行定义的OnTriggerExit2D()，说明当离开SceneryToggler对象检测范围的对象的Tag属性是Platform的时候（精灵离开地面），精灵就会进入falling状态；Tag属性为DeathTrigger的时候（精灵离开场景中的Death Trigger对象），精灵就会进入kill状态；
**提示：**如果读者再回头细细的查看精灵的死亡逻辑的话，就会发现精灵的死亡只是表象！本质上是精灵在一瞬间转移到了重生点所在的位置。正是因为游戏中是如此实现精灵死亡逻辑的，所以才需要检测碰撞对象的Tag属性是否为Death Trigger。
（6）为脚本PlayerStateListener添加对jump、landing和falling这些状态的处理，如下：
- 01     using UnityEngine;
- 02     using System.Collections;
- 03    
- 04     [RequireComponent(typeof(Animator))]
- 05     public class PlayerStateListener : MonoBehaviour
- 06     {
- 07              public float playerWalkSpeed = 3f;                               //表示精灵移动的速度
- 08              public GameObject playerRespawnPoint = null;                 //表示重生的点
- 09              **public float playerJumpForceVertical = 300f;**          //表示跳跃时，水平方向上，力的大小
- 10              **public float playerJumpForceHorizontal = 200f;**     //表示跳跃时，垂直方向上，力的大小
- 11              **private bool playerHasLanded = true;**                       //表示精灵是否落地
- 12              …                                                                                         //省略
- 13              //当角色的状态发生改变的时候，调用此函数
- 14              public void onStateChange(PlayerStateController.playerStates newState)
- 15              {
- 16                       //如果状态没有发生变化，则无需改变状态
- 17                       if(newState == currentState)
- 18                                 return;
- 19                       //判断精灵能否由当前的动画状态，直接转换为另一个动画状态
- 20                       if(!checkForValidStatePair(newState))
- 21                                 return;
- 22                       //通过修改Parameter中Walking的值，修改精灵当前的状态
- 23                       switch(newState)
- 24                       {
- 25                       …                                                             //省略
- 26                       case PlayerStateController.playerStates.jump:
- 27                                 if(playerHasLanded)
- 28                                 {
- 29                                          //确定精灵的跳跃方向
- 30                                          float jumpDirection = 0.0f;
- 31                                          if(currentState == PlayerStateController.playerStates.left)
- 32                                                   jumpDirection = -1.0f;
- 33                                          else if(currentState == PlayerStateController.playerStates.right)
- 34                                                   jumpDirection = 1.0f;
- 35                                          else
- 36                                                   jumpDirection = 0.0f;
- 37                                          //给精灵施加一个特定方向的力
- 38                                          rigidbody2D.AddForce(new Vector2(jumpDirection * playerJumpForceHorizontal,
- 39                                                                      playerJumpForceVertical));
- 40                                          playerHasLanded = false;
- 41                                 }
- 42                                 break;
- 43                       case PlayerStateController.playerStates.landing:
- 44                                 playerHasLanded = true;
- 45                                 break;
- 46                       }
- 47                       //记录角色当前的状态
- 48                       currentState = newState;
- 49              }   
- 50              //用于确认当前的动画状态能否直接转换为另一动画状态的函数
- 51              bool checkForValidStatePair(PlayerStateController.playerStates newState)
- 52              {
- 53                       bool returnVal = false;
- 54                       //比较两种动画状态
- 55                       switch(currentState)
- 56                       {
- 57                       …                                                    //省略
- 58                       case PlayerStateController.playerStates.jump:
- 59                                 if(    newState == PlayerStateController.playerStates.landing
- 60                                          || newState == PlayerStateController.playerStates.kill
- 61                                          )
- 62                                          returnVal = true;
- 63                                 else
- 64                                          returnVal = false;
- 65                                 break;
- 66                       case PlayerStateController.playerStates.landing:
- 67                                 if(    newState == PlayerStateController.playerStates.left
- 68                                          || newState == PlayerStateController.playerStates.right
- 69                                          || newState == PlayerStateController.playerStates.idle
- 70                                          )
- 71                                          returnVal = true;
- 72                                 else
- 73                                          returnVal = false;
- 74                                 break;             
- 75                       case PlayerStateController.playerStates.falling:   
- 76                                 if(      newState == PlayerStateController.playerStates.landing
- 77                                          || newState == PlayerStateController.playerStates.kill
- 78                                          )
- 79                                          returnVal = true;
- 80                                 else
- 81                                          returnVal = false;
- 82                                 break;             
- 83                       }         
- 84                       return returnVal;
- 85              }
- 86              …                                                             //省略
- 87     }
修改后的脚本，在脚本组件里就会多出两个属性，分别用于决定精灵跳跃时水平和垂直方向受到的力，如图2-20所示。
**![](https://images2015.cnblogs.com/blog/632949/201509/632949-20150908104210387-1605983551.png)**
**图2-20  修改后的脚本组件**
对于此脚本里新加入的代码，有以下几点需要说明：
- q   脚本09~11行，定义了3个变量：playerJumpForceVertical、playerJumpForceHorizontal和playerHasLanded，用于辅助新加入状态的逻辑实现；
- q   脚本14行，onStateChange()里添加的代码说明，精灵只有在落地的时候才能继续跳跃，而且脚本会依据精灵当前的运动朝向，来决定是朝左跳还是朝右跳，具体的方法是，给精灵的刚体施加一个力，精灵将在此力的作用下产生类似于跳跃的行为。
- q   脚本51行，checkForValidStatePair()里添加的代码说明，jump状态只能转换为landing和kill状态；landing状态只能转换为left、right和idle状态；falling状态只能转换为landing和kill状态；
**（7****）**运行游戏，按下空格的时候，精灵就会进入jump状态了。当精灵处于idle状态时，精灵会原地起跳；处于left状态时，会朝左跳，同理right状态；最终，精灵可以在多个地面间上窜下跳，如图2-21所示。
**![](https://images2015.cnblogs.com/blog/632949/201509/632949-20150908104218012-1710629130.png)**
**图2-21  精灵的跳跃状态（原地起跳、左跳和右跳）**
**提示：**当精灵与Death Trigger对象接触，并进入死亡状态时，Unity里的Console视图里会弹出一条警告信息：hitDeathTrigger has no receiver!，如图2-22所示。
**![](https://images2015.cnblogs.com/blog/632949/201509/632949-20150908104225669-1366361506.png)**
**图2-22  Console视图里弹出的警告信息**
这是由于脚本PlayerColliderListener里包含了方法OnTriggerExit2D()，代码如下：
- 01              //离开碰撞检测区域时，调用此函数
- 02              void OnTriggerExit2D( Collider2D collidedObject)
- 03              {
- 04                       switch(collidedObject.tag)
- 05                       {
- 06                                 //当精灵离开地面是，触发falling动画状态
- 07                                 case "Platform":
- 08                                         targetStateListener.onStateChange(PlayerStateController.playerStates.falling);
- 09                                 break;
- 10                                 //当精灵离开的是Death Trigger对象，则触发kill动画状态
- 11**                                 case "DeathTrigger":**
- 12**                                          targetStateListener.onStateChange(PlayerStateController.playerStates.kill);**
- 13**                                 break;**
- 14                       }
- 15              }
代码中加粗的部分直接激活了精灵的kill状态，因此不再需要hitDeathTrigger()方法去激活这个状态。因此，读者可以做两步操作，来解决Console视图里提示的警告信息。首先，移除Death Trigger对象上的Death Trigger Script组件，然后，移除脚本PlayerStateListener里定义的hitDeathTrigger()方法：
- 01              public void hitDeathTrigger()
- 02              {
- 03                       onStateChange(PlayerStateController.playerStates.kill);
- 04              }
**本文选自：**Unity 2D游戏开发快速入门大学霸内部资料，转载请注明出处，尊重技术尊重IT人！
