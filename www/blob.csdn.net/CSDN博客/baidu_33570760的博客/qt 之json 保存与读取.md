# qt 之json  保存与读取 - baidu_33570760的博客 - CSDN博客
2017年05月03日 17:51:20[carman_风](https://me.csdn.net/baidu_33570760)阅读数：417
转载自：http://blog.csdn.net/liang19890820/article/details/52797245
# 简述
许多游戏提供保存功能，使得玩家在游戏中的进度可以被保存，并在以后再玩的时候进行加载。保存游戏的过程通常涉及将每个游戏对象的成员变量序列化为文件。要实现这个功能，可以采取许多格式，其中之一就是 JSON - 使用 QJsonDocument。如果不希望保存的文件可读，或者不需要保持文件大小，还能够以二进制格式序列化文档，这就厉害了~O(∩_∩)O~。
下面，将演示如何以 JSON 和二进制格式来保存和加载一个简单的游戏。
- [简述](http://blog.csdn.net/liang19890820/article/details/52797245#%E7%AE%80%E8%BF%B0)
- [Character 类](http://blog.csdn.net/liang19890820/article/details/52797245#character-%E7%B1%BB)
- [Level 类](http://blog.csdn.net/liang19890820/article/details/52797245#level-%E7%B1%BB)
- [Game 类](http://blog.csdn.net/liang19890820/article/details/52797245#game-%E7%B1%BB)
- [使用](http://blog.csdn.net/liang19890820/article/details/52797245#%E4%BD%BF%E7%94%A8)
- [更多参考](http://blog.csdn.net/liang19890820/article/details/52797245#%E6%9B%B4%E5%A4%9A%E5%8F%82%E8%80%83)
# Character 类
Character 类表示游戏中的非玩家角色（NPC），并存储玩家的姓名、级别和类类型。
提供了 read() 和 write() 函数来序列化成员变量。
```
class Character
  {
  public:
      enum ClassType {
          Warrior, Mage, Archer
      };
      Character();
      Character(const QString &name, int level, ClassType classType);
      QString name() const;
      void setName(const QString &name);
      int level() const;
      void setLevel(int level);
      ClassType classType() const;
      void setClassType(ClassType classType);
      void read(const QJsonObject &json);
      void write(QJsonObject &json) const;
  private:
      QString mName;
      int mLevel;
      ClassType mClassType;
  };
```
我们感兴趣的是读和写函数的实现：
```
void Character::read(const QJsonObject &json)
  {
      mName = json["name"].toString();
      mLevel = json["level"].toDouble();
      mClassType = ClassType(qRound(json["classType"].toDouble()));
  }
```
``
在 read() 函数中，由 QJsonObject 参数分配 Character 的成员变量，可以使用 QJsonObject::operator 或者 QJsonObject::value() 来访问 JSON 对象中的值，它们均是 const 函数。如果指定的 key 无效，则返回 QJsonValue::Undefined。
**注意：**在尝试读取值之前，应该使用 QJsonObject::contains() 检测 key 是否有效，这里假设是有效的，所以没有检测。
```
void Character::write(QJsonObject &json) const
  {
      json["name"] = mName;
      json["level"] = mLevel;
      json["classType"] = mClassType;
  }
```
在 write() 函数中，处理与 read() 相反。将 Character 的值分配给 QJsonObject 对象。与访问值一样，也有两种方式来设置 QJsonObject 的值：QJsonObject::operator 和 QJsonObject::insert()，它们都会覆盖指定 key 对应的值。
# Level 类
接下来是 Level 类，表示游戏中的级别。
```
class Level
  {
  public:
      Level();
      const QList<Character> &npcs() const;
      void setNpcs(const QList<Character> &npcs);
      void read(const QJsonObject &json);
      void write(QJsonObject &json) const;
  private:
      QList<Character> mNpcs;
  };
```
游戏中有很多级别，每个级别都有几个 NPC，所以需要一个 QList 保存 Character 对象。其次，还提供了熟悉的 read() 和 write() 函数。
```
void Level::read(const QJsonObject &json)
  {
      mNpcs.clear();
      QJsonArray npcArray = json["npcs"].toArray();
      for (int npcIndex = 0; npcIndex < npcArray.size(); ++npcIndex) {
          QJsonObject npcObject = npcArray[npcIndex].toObject();
          Character npc;
          npc.read(npcObject);
          mNpcs.append(npc);
      }
  }
```
容器可以使用 QJsonArray 写入和读取 JSON。示例中，用关键字“npcs”及其相关联的值构造了一个 QJsonArray。然后，对于数组中的每个 QJsonValue 元素，调用 toObject() 来获取 Character 的 JSON 对象。最后，Character 对象可以读取其 JSON 并附加到 NPC 列表中。
```
void Level::write(QJsonObject &json) const
  {
      QJsonArray npcArray;
      foreach (const Character npc, mNpcs) {
          QJsonObject npcObject;
          npc.write(npcObject);
          npcArray.append(npcObject);
      }
      json["npcs"] = npcArray;
  }
```
write() 函数类似于 read()，除了功能相反以外。
# Game 类
建立了 Character 和 Level 类后，继续看 Game 类：
```
class Game
  {
  public:
      Game();
      // 游戏保存的格式 Json、Binary（二进制）
      enum SaveFormat {
          Json, Binary
      };
      const Character &player() const;
      const QList<Level> &levels() const;
      void newGame();
      bool loadGame(SaveFormat saveFormat);
      bool saveGame(SaveFormat saveFormat) const;
      void read(const QJsonObject &json);
      void write(QJsonObject &json) const;
  private:
      Character mPlayer;
      QList<Level> mLevels;
  };
```
首先，定义了 SaveFormat 枚举。这将允许指定游戏应该保存的格式：Json 或 Binary。
接下来，我们为玩家和级别提供了访问器。然后暴漏了三个函数：newGame()、saveGame() 和 loadGame()。
read() 和 write() 函数由 saveGame() 和 loadGame() 使用。
```
void Game::newGame() {
      mPlayer = Character();
      mPlayer.setName(QStringLiteral("Hero"));
      mPlayer.setClassType(Character::Archer);
      mPlayer.setLevel(15);
      mLevels.clear();
      Level village;
      QList<Character> villageNpcs;
      villageNpcs.append(Character(QStringLiteral("Barry the Blacksmith"), 10, Character::Warrior));
      villageNpcs.append(Character(QStringLiteral("Terry the Trader"), 10, Character::Warrior));
      village.setNpcs(villageNpcs);
      mLevels.append(village);
      Level dungeon;
      QList<Character> dungeonNpcs;
      dungeonNpcs.append(Character(QStringLiteral("Eric the Evil"), 20, Character::Mage));
      dungeonNpcs.append(Character(QStringLiteral("Eric's Sidekick #1"), 5, Character::Warrior));
      dungeonNpcs.append(Character(QStringLiteral("Eric's Sidekick #2"), 5, Character::Warrior));
      dungeon.setNpcs(dungeonNpcs);
      mLevels.append(dungeon);
  }
```
要设置一个新游戏，需要创建玩家并填充级别和对应的 NPC。
```
void Game::read(const QJsonObject &json)
  {
      mPlayer.read(json["player"].toObject());
      mLevels.clear();
      QJsonArray levelArray = json["levels"].toArray();
      for (int levelIndex = 0; levelIndex < levelArray.size(); ++levelIndex) {
          QJsonObject levelObject = levelArray[levelIndex].toObject();
          Level level;
          level.read(levelObject);
          mLevels.append(level);
      }
  }
```
``
read() 函数中首先要做的是告诉玩家读取自身的数据。然后清除级别列表，以便多次调用 loadGame() 不会导致旧级别的存在。
然后通过从 QJsonArray 读取每个 Level 来填充级别列表。
```
void Game::write(QJsonObject &json) const
  {
      QJsonObject playerObject;
      mPlayer.write(playerObject);
      json["player"] = playerObject;
      QJsonArray levelArray;
      foreach (const Level level, mLevels) {
          QJsonObject levelObject;
          level.write(levelObject);
          levelArray.append(levelObject);
      }
      json["levels"] = levelArray;
  }
```
将游戏写入 JSON 类似于如何写 Level。
```
bool Game::loadGame(Game::SaveFormat saveFormat)
  {
      QFile loadFile(saveFormat == Json
          ? QStringLiteral("save.json")
          : QStringLiteral("save.dat"));
      if (!loadFile.open(QIODevice::ReadOnly)) {
          qWarning("Couldn't open save file.");
          return false;
      }
      QByteArray saveData = loadFile.readAll();
      QJsonDocument loadDoc(saveFormat == Json
          ? QJsonDocument::fromJson(saveData)
          : QJsonDocument::fromBinaryData(saveData));
      read(loadDoc.object());
      return true;
  }
```
当在 loadGame() 中加载保存的游戏时，做的第一件事是根据保存文件的格式打开保存文件，save.json 用于 JSON，save.dat 用于二进制。如果无法打开文件，打印一个警告，返回 false。
由于 QJsonDocument 的 fromJson() 和 fromBinaryData() 函数都使用 QByteArray，因此无论保存格式如何，都可以用其中的一个来转换保存文件的整个内容。
在构造 QJsonDocument 之后，让 Game 对象读取自身数据，然后返回 true 以示成功。
```
bool Game::saveGame(Game::SaveFormat saveFormat) const
  {
      QFile saveFile(saveFormat == Json
          ? QStringLiteral("save.json")
          : QStringLiteral("save.dat"));
      if (!saveFile.open(QIODevice::WriteOnly)) {
          qWarning("Couldn't open save file.");
          return false;
      }
      QJsonObject gameObject;
      write(gameObject);
      QJsonDocument saveDoc(gameObject);
      saveFile.write(saveFormat == Json
          ? saveDoc.toJson()
          : saveDoc.toBinaryData());
      return true;
  }
```
毫不奇怪，saveGame() 看起来非常像 loadGame()。基于格式确定文件扩展名，如果文件打开失败，打印警告，返回 false。然后将 Game 对象写入一个 QJsonDocument，并调用 QJsonDocument::toJson() 或 QJsonDocument::toBinaryData() 来保存游戏，具体取决于指定的格式。
# 使用
现在准备进入 main() 函数：
```
int main(int argc, char *argv[])
  {
      // 因为只想展示使用 JSON 游戏的序列化，实际上游戏是不可玩的。因此，只需要 QCoreApplication，没有事件循环。
      QCoreApplication app(argc, argv);
      Game game;
      game.newGame();
      // 游戏开始，加载数据...
      // 假设玩家度过了快乐的时光，取得了伟大的成就，改变 Character、Level 和 Game 对象的内部状态。
      if (!game.saveGame(Game::Json))
          return 1;
      if (!game.saveGame(Game::Binary))
          return 1;
      Game fromJsonGame;
      if (!fromJsonGame.loadGame(Game::Json))
          return 1;
      Game fromBinaryGame;
      if (!fromBinaryGame.loadGame(Game::Binary))
          return 1;
      return 0;
  }
```
``
当玩家结束游戏后，保存游戏数据。为了演示，序列化为 JSON 和二进制。可以在与可执行文件相同的目录中检查文件的内容，但二进制保存文件将包含一些无用字符（这是正常的）。
为了显示可以再次加载保存的文件，为每种格式调用 loadGame()，失败时返回 1。假设一切顺利，返回 0 表示成功。
如你所见，使用 Qt 的 JSON 类进行序列化非常简单和方便。使用 QJsonDocument 比 QDataStream 的优点在于，不仅可以得到易读的 JSON 文件，如果需要，也可以选择使用二进制格式，而不需重写任何代码。
save.json 文件（JSON ）如下所示：
```
{
    "levels": [
        {
            "npcs": [
                {
                    "classType": 0,
                    "level": 10,
                    "name": "Barry the Blacksmith"
                },
                {
                    "classType": 0,
                    "level": 10,
                    "name": "Terry the Trader"
                }
            ]
        },
        {
            "npcs": [
                {
                    "classType": 1,
                    "level": 20,
                    "name": "Eric the Evil"
                },
                {
                    "classType": 0,
                    "level": 5,
                    "name": "Eric's Sidekick #1"
                },
                {
                    "classType": 0,
                    "level": 5,
                    "name": "Eric's Sidekick #2"
                }
            ]
        }
    ],
    "player": {
        "classType": 2,
        "level": 15,
        "name": "Hero"
    }
}
```
save.dat 文件（二进制）如下所示：
```
qbjs   ?     ?     levels<     4  ?     ?     npcs  ?     ?  `      T   ?      classType         Z   level ?   name   Barry the Blacksmith     $   0   \      P   ?  	 classType         Z   level ?   name   Terry the Trader     $   0   ?  ?     <     8     npcs          P      D   :        classType ?   level ?   name  
 Eric the Evil       (   \      P   ?     classType         ?   level ?   name   Eric's Sidekick #1   $   0   \      P   ?
```
