# 【设计模式】设计模式C++编程实现之观察者模式（ObserverPattern） - 江南烟雨 - CSDN博客
2013年04月18日 20:15:51[江南烟雨](https://me.csdn.net/xiajun07061225)阅读数：2576标签：[设计模式																[C++](https://so.csdn.net/so/search/s.do?q=C++&t=blog)](https://so.csdn.net/so/search/s.do?q=设计模式&t=blog)
个人分类：[设计模式](https://blog.csdn.net/xiajun07061225/article/category/1174429)
观察者模式定义：
定义了对象之间的一对多依赖，这样一来，当一个对象改变状态时，它的所有依赖者都会受到通知并自动更新。
观察者模式又叫做发布-订阅（Publish/Subscribe）模式、模型-视图（Model/View）模式、源-监听器（Source/Listener）模式或从属者（Dependents）模式。
观察者模式定义了一种一对多的依赖关系，让多个观察者对象同时监听某一个主题对象。这个主题对象在状态上发生变化时，会通知所有观察者对象，使它们能够自动更新自己
类图
![](https://img-my.csdn.net/uploads/201304/18/1366287358_8906.jpg)
图一 观察者模式类图
可以看出，在这个观察者模式的实现里有下面这些角色：
- 抽象主题（Subject）角色：主题角色把所有对观察考对象的引用保存在一个聚集里，每个主题都可以有任何数量的观察者。抽象主题提供一个接口，可以增加和删除观察者对象，主题角色又叫做抽象被观察者（Observable）角色，一般用一个抽象类或者一个接口实现。
- 抽象观察者（Observer）角色：为所有的具体观察者定义一个接口，在得到主题的通知时更新自己。这个接口叫做更新接口。抽象观察者角色一般用一个抽象类或者一个接口实现。在这个示意性的实现中，更新接口只包含一个方法（即Update()方法），这个方法叫做更新方法。
- 具体主题（ConcreteSubject）角色：将有关状态存入具体现察者对象；在具体主题的内部状态改变时，给所有登记过的观察者发出通知。具体主题角色又叫做具体被观察者角色（Concrete Observable）。具体主题角色通常用一个具体子类实现。
- 具体观察者（ConcreteObserver）角色：存储与主题的状态自恰的状态。具体现察者角色实现抽象观察者角色所要求的更新接口，以便使本身的状态与主题的状态相协调。如果需要，具体现察者角色可以保存一个指向具体主题对象的引用。具体观察者角色通常用一个具体子类实现。
观察者模式提供了一种对象设计，让主题和观察者之间松耦合。
当两个对象之间松耦合，他们依然可以交互，但是不太清楚彼此的细节。
关于观察者的一切，主题只知道观察者实现了某个接口（Observer）。主题不需要知道观察者的具体类是谁、做了些什么或其他任何细节。
任何时候我们可以增加新的观察者。因为主题唯一依赖的东西是一个实现Observer接口的对象列表。在有新的类型的观察者出现时，主题的代码不需要修改。所要做的就是在新的类里实现此观察者接口，然后注册为观察者即可。
改变主题或观察者其中一方，并不会影响另一方。因为两者是松耦合的。
气象站实例C++编程实现（来源于《Head First设计模式》）：
其类图：
![](https://img-my.csdn.net/uploads/201304/18/1366287630_9326.png)
代码：
```cpp
//观察者模式C++编程实现
//author：江南烟雨
//E-Mail：xiajunhust@gmail.com
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;
static int setConditionTimes = 0;//记录设置的总次数
//观察者（布告板）抽象接口
class Observer{
public:
	virtual void update(float temp,float humidity,float pressure) = 0;
};
//主题抽象接口
class Subject{
public:
	virtual void registerObserver(Observer *o) = 0;//注册观察者
	virtual void removeObserver(Observer *o) = 0;//删除观察者
	virtual void notifyObserver() = 0;//当主题状态改变时，通知所有观察者
};
//布告板显示
class DisplayElement{
public: 
	virtual void display()= 0;
};
//WeatherData具体类，继承自主题抽象类
class WeatherData:public Subject{
private:
	vector<Observer *> observers;
	float temperature;
	float humility;
	float pressure;
public:
	WeatherData(){}
	~WeatherData(){
		vector<Observer*>::iterator pos = observers.begin();
		for (;pos != observers.end();++pos)
		{
			delete (*pos);
		}
	}
	void registerObserver(Observer *o){
		observers.push_back(o);
	}
	void removeObserver(Observer *o){
		vector<Observer*>::iterator pos = find(observers.begin(),
			observers.end(),o);
		if (pos != observers.end())
		{
			observers.erase(pos);
		}
	}
	void notifyObserver()
	{
		vector<Observer*>::iterator pos = observers.begin();
		for (;pos != observers.end();++pos)
		{
			(*pos)->update(temperature,humility,pressure);
		}
	}
	//当从气象站得到更新观测值时，通知观察者
	void messurementsChanged(){
		notifyObserver();
	}
	//设置气象站观测值
	void setMessurements(float temperature,float humility,float pressure){
		++setConditionTimes;
		this->temperature = temperature;
		this->humility = humility;
		this->pressure = pressure;
		messurementsChanged();
	}
};
//布告板具体类：目前状况
class CurrentConditionDisplay:public Observer,public DisplayElement{
private:
	float temperature;
	float humility;
	
	Subject *weatherData;
public: 
	CurrentConditionDisplay(Subject *weatherData){
		this->weatherData = weatherData;
		weatherData->registerObserver(this);
	}
	~CurrentConditionDisplay(){}
	void update(float temp,float humi,float pressure){
		this->temperature = temp;
		this->humility = humi;
		display();
	}
	void display(){
		cout << "Current conditions: " << temperature << "F degrees and " <<
			humility << "% humility" << endl;
	}
};
//布告板具体类：气象统计类
class WeatherStatisticsDisplay:public Observer,public DisplayElement{
private:
	float avgTemperatuere,maxTemperature,minTemperature;
	Subject *weatherData;
public: 
	WeatherStatisticsDisplay(Subject *weatherData){
		this->weatherData = weatherData;
		weatherData->registerObserver(this);
	}
	~WeatherStatisticsDisplay(){}
	void update(float temp,float humi,float pressure){
		if (1 == setConditionTimes)
		{
			this->avgTemperatuere = temp;
			this->maxTemperature = temp;
			this->minTemperature = temp;
		}
		else{
			this->avgTemperatuere = (this->avgTemperatuere * (setConditionTimes - 1) + temp) / setConditionTimes;
			this->maxTemperature = this->maxTemperature > temp ? this->maxTemperature : temp;
			this->minTemperature = this->minTemperature < temp ? this->minTemperature : temp;
		}
		display();
	}
	void display(){
		cout << "Avg/Max/Min: temperature  " << avgTemperatuere << "/" << 
			maxTemperature << "/" <<minTemperature << endl;
	}
};
int main()
{
	WeatherData *weatherData = new WeatherData();
	CurrentConditionDisplay *currentConditionDisplay = new CurrentConditionDisplay(weatherData);
	WeatherStatisticsDisplay *weatherStatisticsDisplay = new WeatherStatisticsDisplay(weatherData);
	
	weatherData->setMessurements(80,65,30.4f);
	weatherData->setMessurements(82,70,29.2f);
	weatherData->setMessurements(78,90,29.2f);
	weatherData->removeObserver(weatherStatisticsDisplay);
	cout << "after remove observer WeatherStatisticsDisplay:" << endl;
	weatherData->setMessurements(80,65,30.4f);
	weatherData->setMessurements(82,70,29.2f);
	weatherData->setMessurements(78,90,29.2f);
	return 0;
}
```
运行结果（VS2008 + Win7）：
![](https://img-my.csdn.net/uploads/201304/18/1366287682_1227.jpg)
