# Unity5.1 新的网络引擎UNET（七）  UNET 单人游戏转换为多人 - weixin_33985507的博客 - CSDN博客
2017年06月14日 15:16:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：5

﻿﻿
单人游戏转换为多人   孙广东   2015.7.12 
本文档描写叙述将单人游戏转换为使用新的网络系统的多人游戏的步骤。这里描写叙述的过程是简化，对于一个真正的游戏事实上须要更高级别版本号的实际流程，如今介绍的是不工作就像这的每一场比赛，但它提供了基本配方。
1、NetworkManager 安装
• 向场景加入一个新的游戏对象并将它重命名为"NetworkManager"。
• 为新的游戏对象加入NetworkManager 组件。
• 将 NetworkManagerHUD 组件加入到游戏物体。这将为管理网络游戏状态提供默认 UI。
2、Player Prefab 安装
• 在游戏中Find player的预制体prefab。或者从player对象创建一个预置。
• NetworkIdentity 组件加入到player prefab上。
• 检查 NetworkIdentity 的 LocalPlayerAuthority box 。
• 在player prefab的NetworkManager 组件上为 “Spawn Info” 设置 playerPrefab
• 从现场删除player 对象实例，假设它存在于现场
3、Player Movement
• 将 NetworkTransform 组件加入到 player prefab
• 更新输入和控制脚本来遵守 isLocalPlayer
• 修复camera 使用衍生出的player 和 isLocalPlayer
比如。此脚本仅仅处理local player输入:
```
using UnityEngine;
using UnityEngine.Networking;
public class Controls : NetworkBehaviour
{
    void Update()
    {
        if (!isLocalPlayer)
        {
            // exit from update if this is not the local player
            return;
        }
        // handle player input for movement
    }
}
```
4、Basic Player Game State
• 使脚本包括关键数据到NetworkBehaviours ， 而不是 MonoBehaviours 
• 使重要成员变量到 SyncVars
5、Networked Actions
• 使脚本运行NetworkBehaviours重要操作成而不是 MonoBehaviours 
• Update函数运行player操作的重要命令
6、Non-Player Objects
改动非玩家prefabs，像敌人等:
• 加入 NetworkIdentify 组件
• 加入 NetworkTransform 组件
• NetworkManager注冊 spawnable 预置体
• 更新脚本与游戏状态和操作
7、Spawners
• 可能更改的spawner 脚本要继承自 NetworkBehaviours
• 改动spawners 仅仅运行在server上，使用 isServer 属性或 OnStartServer() 函数
• 为创建对象调用 NetworkServer.Spawn()
8、Player的产生的位置
• 加入一个新的游戏对象。并将它放在玩家的应该開始的位置
• 新的游戏对象加入 NetworkStartPosition 组件
9、Lobby
• Create Lobby Scene创建大厅现场
• Import multiplayer-lobby package导入多人游戏大厅包
• 加入 GuiLobbyManager 预置到场景
• Configure the manager配置 管理器
       •Scenes
      • Prefabs
      • spawners

