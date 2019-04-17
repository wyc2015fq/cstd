# C++和Java中枚举enum的用法 - Grandyang - 博客园







# [C++和Java中枚举enum的用法](https://www.cnblogs.com/grandyang/p/4781267.html)







在C++和java中都有枚举enum这个关键字，但是它们之间又不太一样。对于C++来说，枚举是一系列命名了的整型常量，而且从枚举值转化为对应的整型值是在内部进行的。而对于Java来说，枚举更像一个类的命名的实例，你可以自定义枚举的成员，枚举值转化为对应的整型值是再外部进行的。下面以我之前的一篇博客[8.1 Implement Blackjack 实现21点纸牌](http://www.cnblogs.com/grandyang/p/4781234.html)来说明，里面有一个扑克牌花色的枚举类：



```
// C++ defination
enum Suit {Club, Diamond, Heart, Spade};

// Java defination 
public enum Suit {
    Club(0), Diamond(1), Heart(2), Spade(3);
    private int _value;
    private Suit(int v) { _value = v; };
    public int getValue() { return _value; };
    public static Suit getSuitFromValue(int value) {}; // ...
};
```



上面是C++和Java实现花色枚举的实现对比，我们可以看出C++把转换过程都在内部完成了，而Java需要自己写出转换过程。对于C++的枚举来说，如果不特别赋值，那么对应的整型数是从0开始往后类推的，如果部分赋值了，就从赋的值往后类推，具体看参见[这里](https://msdn.microsoft.com/en-us/library/2dzy4k6e.aspx)，下面三行代码也很好的说明了这一点:



```
enum Suit { Diamonds, Hearts, Clubs, Spades }; // 0, 1, 2, 3

enum Suit { Diamonds = 1, Hearts, Clubs, Spades }; // 1, 2, 3, 4

enum Suit { Diamonds = 5, Hearts, Clubs = 4, Spades }; // 5, 6, 4, 5
```



对于C++来说除了enum关键字，还有一个enum class关键字，这个是枚举类，它和枚举的区别是枚举名称是局部的，而且并不会在内部转换成其他类型，比如整型。下面这段代码很好的说明了两者的区别：



```
enum Color { red, green, blue };                    // plain enum 
enum Card { red_card, green_card, yellow_card };    // another plain enum 
enum class Animal { dog, deer, cat, bird, human };  // enum class
enum class Mammal { kangaroo, deer, human };        // another enum class

void fun() {

    // examples of bad use of plain enums:
    Color color = Color::red;
    Card card = Card::green_card;

    int num = color;    // no problem

    if (color == Card::red_card) // no problem (bad)
        cout << "bad" << endl;

    if (card == Color::green)   // no problem (bad)
        cout << "bad" << endl;

    // examples of good use of enum classes (safe)
    Animal a = Animal::deer;
    Mammal m = Mammal::deer;

    int num2 = a;   // error
    if (m == a)         // error (good)
        cout << "bad" << endl;

    if (a == Mammal::deer) // error (good)
        cout << "bad" << endl;

}
```














