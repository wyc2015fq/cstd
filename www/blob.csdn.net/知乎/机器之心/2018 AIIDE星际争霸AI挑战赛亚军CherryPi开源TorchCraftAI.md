# 2018 AIIDE星际争霸AI挑战赛亚军CherryPi开源TorchCraftAI - 知乎
# 



机器之心报道，作者：路。

> 不久前，2018 AIIDE StarCraft AI 挑战赛顺利结束。27 支队伍参加比赛，其中三星 SAIDA 获得冠军，Facebook 团队开发的 CherryPi 获得亚军，中科院自动化所开发的 CSE 获得季军。刚刚，Facebook 研究科学家、CherryPi 作者之一 Gabriel Synnaeve 宣布开源训练 StarCraft 智能体的 TorchCraftAI 平台，该平台允许构建智能体来（学习）玩《星际争霸：母巢之战》游戏。

TorchCraftAI 地址：[https://torchcraft.github.io/TorchCraftAI/](https://link.zhihu.com/?target=https%3A//torchcraft.github.io/TorchCraftAI/)

TorchCraftAI 包括以下重要特征：
- 用于构建 StarCraft 智能体的模块化框架，其中的模块可以接受攻击，被替换成其他模块或机器学习／强化学习模型。
- CherryPi：在星际争霸 AI 竞赛中取得了不俗的成绩，如 SSCAIT 2017-18 冠军、AIIDE 2018 亚军。
- 适用于星际争霸全场游戏、小游戏的强化学习环境，具备模型、训练循环和教程。
- TorchCraft 支持与 StarCraft 和 BWAPI 的 TCP 通信。
- 支持 Linux、Windows 和 OSX 操作系统。

**系统架构**

TorchCraftAI 基于两个现有星际争霸智能体开源项目：BWAPI 和 TorchCraft。

**BWAPI（Brood War API）**

BWAPI 是开发《星际争霸：母巢之战》竞赛智能体时广泛使用的编程界面。它提供人类玩家可观察的所有信息，且使得开发人员可以使用编程语言控制游戏智能体。《星际争霸：母巢之战》是一款 Windows 游戏，BWAPI StarCraft 智能体基于 Windows 开发，通常是 32-bit 动态库（DLL）或使用 BWAPI 客户端模式的 32-bit 可执行程序。

**TorchCraft**

TorchCraft 使用客户端-服务器架构实现与 BWAPI 的跨平台连接，从而促进对《星际争霸：母巢之战》的机器学习研究。服务器是 Windows DLL for BWAPI，客户端程序可以更加灵活地定义，如在 Linux 系统上运行的 C++ 程序或 Python 脚本。其运行模式的理论很简单：TorchCraft 客户端向服务器发送一串游戏命令，服务器执行对应的 BWAPI 调用，并回应更新后的游戏状态。

**TorchCraftAI**

TorchCraftAI 封装了 TorchCraft client C++ 库，提供游戏循环和游戏状态的抽象。其核心特征包括：
- 在一个流程中托管多个客户端的能力，从而能够并行玩多个星际争霸游戏。
- 能够从 TorchCraftAI 程序内部启动星际争霸游戏，从而使得写训练循环、测试和场景更加简便。
- 分布式强化学习库。

下图展示了两个 TorchCraftAI 智能体互搏的完整图：
![](https://pic4.zhimg.com/v2-b368d42ffda2a3dc9f4593fb8ba964db_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='930'></svg>)
**教程**

训练模型造建筑（通过强化学习训练造建筑模块）：
![](https://pic3.zhimg.com/v2-a8cf04879c3dcb67f065c7a4286f5c8e_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='810'></svg>)



地址：[https://torchcraft.github.io/TorchCraftAI/docs/bptut-intro.html](https://link.zhihu.com/?target=https%3A//torchcraft.github.io/TorchCraftAI/docs/bptut-intro.html)

训练模型战斗（通过强化学习使智能体掌握微操技能）：
![](https://pic4.zhimg.com/v2-faa0243a4f89e4e6bd1c237e82b2f873_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1072' height='804'></svg>)
地址：[https://torchcraft.github.io/TorchCraftAI/docs/microtut-intro.html](https://link.zhihu.com/?target=https%3A//torchcraft.github.io/TorchCraftAI/docs/microtut-intro.html)
*![](https://pic1.zhimg.com/v2-18cc987d5f379a82f1208b6d90722318_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='73' height='24'></svg>)*





