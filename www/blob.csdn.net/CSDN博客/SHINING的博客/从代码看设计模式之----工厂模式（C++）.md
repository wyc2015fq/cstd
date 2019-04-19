# 从代码看设计模式之----工厂模式（C++） - SHINING的博客 - CSDN博客
2013年06月19日 16:17:55[snsn1984](https://me.csdn.net/snsn1984)阅读数：3540
设计模式依照本人理解，就是真对一类问题一个更好的标准的解决方案，按照这个解决方案做出来的东西，后续方便扩展，方便维护，等等一系列方便。不按照这个方案行不行？当然也可以，杀猪杀屁股，各有各的杀法。这是我对设计模式的两点认识：第一，设计模式只是针对一类问题较好的一个解决方案，不唯一，不一定最好；第二，不按照设计模式也能解决问题。
遵照这个思想，我从四人帮的设计模式的书中扣了一些代码出来，调试通过，想以此让自己对设计模式的理解更为深入。毕竟四人帮书中的描述虽然经典，但是终归是没有看代码来的直接点。
下面就来看代码。首先，要看一下没有进行过优化的代码，这个代码是打算建立一个迷宫（Maze），迷宫的元素是墙、房间、门。它们都是MapSite的子类。同时一个房间有四个方向，有可能是墙也有可能是门。
#include <iostream> 
using namespace std; 
enum Direction { 
  North, 
  East, 
  South, 
  West, 
}; 
class MapSite { 
public: 
  virtual void Enter() = 0; 
}; 
class Room : public MapSite { 
public: 
  Room(int roomNo):_roomNumber(roomNo) {}; 
  MapSite *GetSide(Direction) const; 
  virtual void SetSide(Direction direction, MapSite *) { 
    cout << "Set the " 
         << _roomNumber 
         << " room " 
         << direction 
         << " side." 
         << endl; 
  }; 
  int GetRoomNo() { 
    return _roomNumber; 
  }; 
  virtual void Enter() { 
    cout << "Enter the Room." << endl; 
  }; 
private: 
  MapSite *_sides[4]; 
  int _roomNumber; 
}; 
class Wall : public MapSite { 
public: 
  Wall() {}; 
  virtual void Enter() { 
    cout << "Enter the wall." << endl; 
  }; 
}; 
class Door : public MapSite { 
public: 
  Door(Room* = 0, Room* = 0) {}; 
  virtual void Enter() { 
    cout << "Enter the door." << endl; 
  }; 
  Room *OtherSideFrom(Room*) {}; 
private: 
  Room *_room1; 
  Room *_room2; 
  bool _isOpen; 
}; 
class Maze { 
public: 
  Maze() {}; 
  void AddRoom(Room* room) { 
    cout << "Add the room" << room->GetRoomNo() << "." << endl; 
  }; 
  Room* RoomNo(int) const{}; 
}; 
class MazeGame { 
public: 
  Maze *CreateMaze(); 
}; 
Maze* MazeGame::CreateMaze() { 
  Maze* aMaze = new Maze; 
  Room* r1 = new Room(1); 
  Room* r2 = new Room(2); 
  Door* theDoor = new Door(r1, r2); 
  aMaze->AddRoom(r1); 
  aMaze->AddRoom(r2); 
  r1->SetSide(North, new Wall); 
  r1->SetSide(East, theDoor); 
  r1->SetSide(South, new Wall); 
  r1->SetSide(West, new Wall); 
  r2->SetSide(North, new Wall); 
  r2->SetSide(East, new Wall); 
  r2->SetSide(South, new Wall); 
  r2->SetSide(West, theDoor); 
  return aMaze; 
} 
int main() { 
  MazeGame mygame; 
  mygame.CreateMaze(); 
  return 0; 
}
这个源码就是普通的实现方式。按照工厂模式进行修改之后的代码如下：
#include <iostream> 
using namespace std; 
enum Direction { 
  North, 
  East, 
  South, 
  West, 
}; 
class MapSite { 
public: 
  virtual void Enter() = 0; 
}; 
class Room : public MapSite { 
public: 
  Room(int roomNo):_roomNumber(roomNo) {}; 
  MapSite *GetSide(Direction) const; 
  virtual void SetSide(Direction direction, MapSite *) { 
    cout << "Set the " 
         << _roomNumber 
         << " room " 
         << direction 
         << " side." 
         << endl; 
  }; 
  int GetRoomNo() { 
    return _roomNumber; 
  }; 
  virtual void Enter() { 
    cout << "Enter the Room." << endl; 
  }; 
private: 
  MapSite *_sides[4]; 
  int _roomNumber; 
}; 
class Wall : public MapSite { 
public: 
  Wall() {}; 
  virtual void Enter() { 
    cout << "Enter the wall." << endl; 
  }; 
}; 
class Door : public MapSite { 
public: 
  Door(Room* = 0, Room* = 0) {}; 
  virtual void Enter() { 
    cout << "Enter the door." << endl; 
  }; 
  Room *OtherSideFrom(Room*) {}; 
private: 
  Room *_room1; 
  Room *_room2; 
  bool _isOpen; 
}; 
class Maze { 
public: 
  Maze() {}; 
  void AddRoom(Room* room) { 
    cout << "Add the room" << room->GetRoomNo() << "." << endl; 
  }; 
  Room* RoomNo(int) const{}; 
}; 
class MazeFactory { 
public: 
  MazeFactory() {}; 
  virtual Maze* MakeMaze() const { 
    return new Maze; 
  }; 
  virtual Wall* MakeWall() const { 
    return new Wall; 
  }; 
  virtual Room* MakeRoom(int n) const { 
    return new Room(n); 
  }; 
  virtual Door* MakeDoor(Room* r1, Room* r2) const { 
    return new Door(r1, r2); 
  }; 
}; 
class MazeGame { 
public: 
Maze* CreateMaze(MazeFactory &factory) { 
  Maze* aMaze = factory.MakeMaze(); 
  Room* r1 = factory.MakeRoom(1); 
  Room* r2 = factory.MakeRoom(2); 
  Door* theDoor = factory.MakeDoor(r1, r2); 
  aMaze->AddRoom(r1); 
  aMaze->AddRoom(r2); 
  r1->SetSide(North, factory.MakeWall()); 
  r1->SetSide(East, theDoor); 
  r1->SetSide(South, factory.MakeWall()); 
  r1->SetSide(West, factory.MakeWall()); 
  r2->SetSide(North, factory.MakeWall()); 
  r2->SetSide(East, factory.MakeWall()); 
  r2->SetSide(South, factory.MakeWall()); 
  r2->SetSide(West, theDoor); 
  return aMaze; 
}; 
}; 
int main() { 
  MazeGame mygame; 
  MazeFactory factory; 
  mygame.CreateMaze(factory); 
  return 0; 
}
对比这两个源码就会发现，实现的功能是完全相同的，只不过第二种实现采用了工厂模式。设计模式既然好，肯定有它的好处，那我们接下来看看，如果是对这个程序进行扩展，要修改迷宫为房间里可以有炸弹的迷宫的时候，我们在采用了工厂模式之后，需要进行哪些修改。
#include <iostream>
using namespace std;
enum Direction {
  North,
  East,
  South,
  West,
};
class MapSite {
public:
  virtual void Enter() = 0;
};
class Room : public MapSite {
public:
  Room(int roomNo):_roomNumber(roomNo) {};
  MapSite *GetSide(Direction) const;
  virtual void SetSide(Direction direction, MapSite *) {
    cout << "Set the "
         << _roomNumber
         << " room "
         << direction
         << " side."
         << endl;
  };
  int GetRoomNo() {
    return _roomNumber;
  };
  virtual void Enter() {
    cout << "Enter the Room." << endl;
  };
private:
  MapSite *_sides[4];
  int _roomNumber;
};
class BombRoom : public Room {
public:
  BombRoom(int num):Room(num) {
    _roomNum = num;
  };
  virtual void SetSide(Direction direction, MapSite *) {
    cout << "Set the "
         << _roomNum
         << " bomb room "
         << direction
         << " side."
         << endl;
  };
private:
  int _roomNum;
};
class Wall : public MapSite {
public:
  Wall() {};
  virtual void Enter() {
    cout << "Enter the wall." << endl;
  };
};
class Door : public MapSite {
public:
  Door(Room* = 0, Room* = 0) {};
  virtual void Enter() {
    cout << "Enter the door." << endl;
  };
  Room *OtherSideFrom(Room*) {};
private:
  Room *_room1;
  Room *_room2;
  bool _isOpen;
};
class Maze {
public:
  Maze() {};
  void AddRoom(Room* room) {
    cout << "Add the room" << room->GetRoomNo() << "." << endl;
  };
  Room* RoomNo(int) const{};
};
class MazeFactory {
public:
  MazeFactory() {};
  virtual Maze* MakeMaze() const {
    return new Maze;
  };
  virtual Wall* MakeWall() const {
    return new Wall;
  };
  virtual Room* MakeRoom(int n) const {
    return new Room(n);
  };
  virtual Door* MakeDoor(Room* r1, Room* r2) const {
    return new Door(r1, r2);
  };
};
class BombMazeFactory : public MazeFactory {
public:
  virtual Room* MakeRoom(int n) const {
    return new BombRoom(n);
  };
};
class MazeGame {
public:
Maze* CreateMaze(MazeFactory &factory) {
  Maze* aMaze = factory.MakeMaze();
  Room* r1 = factory.MakeRoom(1);
  Room* r2 = factory.MakeRoom(2);
  Door* theDoor = factory.MakeDoor(r1, r2);
  aMaze->AddRoom(r1);
  aMaze->AddRoom(r2);
  r1->SetSide(North, factory.MakeWall());
  r1->SetSide(East, theDoor);
  r1->SetSide(South, factory.MakeWall());
  r1->SetSide(West, factory.MakeWall());
  r2->SetSide(North, factory.MakeWall());
  r2->SetSide(East, factory.MakeWall());
  r2->SetSide(South, factory.MakeWall());
  r2->SetSide(West, theDoor);
  return aMaze;
};
};
int main() {
  MazeGame mygame;
//  MazeFactory factory;
  BombMazeFactory factory;
  mygame.CreateMaze(factory);
  return 0;
}
可以看出改动量很小，而且可以随时把代码改回原来的生成普通迷宫的情况。如果是在最初没有采用工厂模式的代码上进行修改呢？那工作量是显而易见的了，有兴趣的可以自己试试，在这里不再累述。
后续会继续分析工厂方法模式。
