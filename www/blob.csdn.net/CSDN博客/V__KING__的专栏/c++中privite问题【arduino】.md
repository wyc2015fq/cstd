# c++中privite问题【arduino】 - V__KING__的专栏 - CSDN博客





2013年10月10日 15:53:08[v__king__](https://me.csdn.net/V__KING__)阅读数：1064








构造函数中用了pinMode()函数进行了初始化。


_pin在Morse类中是privite，所以在

void setup()

{   

  pinMode(morse._pin,OUTPUT);


}

是错误的。**因为privite是只能被本类中成员所访问，其它类啊，函数啊，都不能访问。**





morse.dot()是可以访问的


|//morse.cpp#include "WProgram.h"#include "Morse.h"Morse::Morse(int pin){  pinMode(pin, OUTPUT);  _pin = pin;}void Morse::dot(){  digitalWrite(_pin, HIGH);  delay(250);  digitalWrite(_pin, LOW);  delay(250);  }void Morse::dash(){  digitalWrite(_pin, HIGH);  delay(1000);  digitalWrite(_pin, LOW);  delay(250);}|
|----|
|//morse.h#ifndef Morse_h#define Morse_h#include "WProgram.h"class Morse{public:Morse(int pin);void dot();void dash();private:int _pin;};#endif|
|----|









