# [CareerCup] 8.4 Parking Lot 停车场问题 - Grandyang - 博客园







# [[CareerCup] 8.4 Parking Lot 停车场问题](https://www.cnblogs.com/grandyang/p/4788419.html)







8.4 Design a parking lot using object-oriented principles.



LintCode上的原题，请参见我的另一篇博客[Parking Lot 停车场问题](http://www.cnblogs.com/grandyang/p/5656870.html)。

这道题让我们实现一个停车位的数据结构，由于题目没给任何多余的信息，所以自由度很大，比如能停放什么种类的车，或是否是多层的等等。根据书中描述，这里我们做如下假设：

1. 停车场有多层，每层有多行停车位

2. 停车场可以停摩托车，小轿车和公交车

3. 停车场有摩托车位，紧凑型车位，和大型车位

4. 摩托车可以停在任何位置

5. 小轿车可以停在紧凑型车位和大型车位

6. 公交车只能停在同一行中连续的五个大型车位上，不能停在小位置上

有了这么多条件，我们就可以开始写各种类了。首先可定要有个基本类Vehicle，然后摩托车，小轿车和公交车都可以派生出来，每个派生类和基类不同的是需要的空位数不同，还有就是车型不同，那么就要把基类的判断是否能停在当前位置设为虚函数，在派生类中分别实现出来。我们还要用枚举类VehicleSize来标识车型。然后就是要有停车场类ParkingLot，层类Level，停车位类ParkingSpot，它们之间错综复杂的关系请参见下列代码：



```
enum class VehicleSize { Motorcycle, Compact, Large };

class Vehicle;
class Level;

class ParkingSpot {
public:
    ParkingSpot(Level *lvl, int r, int n, VehicleSize s): _level(lvl), _row(r), _spotNumber(n), _spotSize(s) {} // ...
    bool isAvailable() { return _vehicle == nullptr; };
    bool canFitVehicle(Vehicle *vehicle) {} // ...
    bool park(Vehicle *v) {} // ...
    int getRow() { return _row; }
    int getSpotNumber() { return _spotNumber; }
    void removeVehicle() {} // ... 

private:
    Vehicle *_vehicle = nullptr;
    VehicleSize _spotSize;
    int _row;
    int _spotNumber;
    Level *_level = nullptr;
};

class Vehicle {
public:
    Vehicle() {}
    int getSpotsNeeded() { return _spotsNeeded; }
    VehicleSize getSize() { return _size; }
    void parkInSpot(ParkingSpot s) { _parkingSpots.push_back(s); }
    void clearSpots() {} // ...
    virtual bool canFitInSpot(ParkingSpot spot) {}

protected:
    vector<ParkingSpot> _parkingSpots;
    string _licensePlate;
    int _spotsNeeded;
    VehicleSize _size;
};

class Bus: public Vehicle {
public:
    Bus() {
        _spotsNeeded = 5;
        _size = VehicleSize::Large;
    }
    bool canFitInSpot(ParkingSpot spot) { }
};

class Car: public Vehicle {
public:
    Car() {
        _spotsNeeded = 1;
        _size = VehicleSize::Compact;
    }
    bool canFitInSpot(ParkingSpot spot) { }
};

class Motorcycle: public Vehicle {
public:
    Motorcycle() {
        _spotsNeeded = 1;
        _size = VehicleSize::Motorcycle;
    }
    bool canFitInSpot(ParkingSpot spot) { }
};

class Level {
public:
    Level() {}
    Level(int flr, int numberSpots): _floor(flr), _availableSpots(numberSpots) {}
    Level(const Level* lvl) {
        *this = *lvl;
    }
    int availableSpots() { return _availableSpots; }
    bool parkVehicle(Vehicle vehicle) {} // ...
    void spotFreed() { ++_availableSpots; }

private:
    int _floor;
    vector<ParkingSpot> _spots;
    int _availableSpots = 0;
    static const int _SPOTS_PER_ROW = 10;
    bool parkStartingAtSpot(int num, Vehicle v) {} // ...
    int findAvailableSpots(Vehicle vehicle) {} // ...
};

class ParkingLot {
public:
    ParkingLot() {} // ...
    bool parkVehicle(Vehicle vehicle) {} // ...

private:
    vector<Level> _levels;
    const int _NUM_LEVELS = 5;
};
```














