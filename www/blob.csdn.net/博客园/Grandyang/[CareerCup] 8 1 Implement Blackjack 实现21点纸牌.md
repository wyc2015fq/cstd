# [CareerCup] 8.1 Implement Blackjack 实现21点纸牌 - Grandyang - 博客园







# [[CareerCup] 8.1 Implement Blackjack 实现21点纸牌](https://www.cnblogs.com/grandyang/p/4781234.html)







8.1 Design the data structures for a generic deck of cards. Explain how you would subclass the data structures to implement blackjack.



这道题让我们设计一个21点纸牌游戏的数据结构，用面向对象的思想来设计。那么既然21点是一种特定的纸牌游戏，它可以是从普通纸牌的基础上派生出来的。所以我们先实现最基本的纸牌类Card，里面包括值和花色，还有一些基本的判断或标记可用性的函数。然后就是基本的牌堆类Deck，可以用来加入牌，洗牌，发牌以及算剩余牌数。还需要一个当前手牌类Hand，可以计算当前分数，可以加牌等。然后就是它们的派生类21点纸牌类BlackJackCard，包括计算值，计算最大最小值，判断是不是Ace等等，然后就是21点手牌类BlackjackHand，计算当前得分，判断是否爆了，是否是21点等等。以下代码为书上代码，有些函数体写实现，所以暂时无法用具体运行。



```
// C++ defination
enum Suit {Club, Diamond, Heart, Spade};

template<class T>
class Deck {
public:
    void setDeckOfCards(vector<T> deckOfCards) {}; // ... 
    void shuffle() {}; // ... 
    int remainingCards() {
        return _cards.size() - _dealtIndex;
    }
    vector<T> dealHead(int number) {}; // ... 
    T dealCard() {}; // ... 
private:
    vector<T> _cards;
    int _dealtIndex = 0;
};

class Card {
public:
    Card(int c, Suit s): _faceValue(c), _suit(s) {};
    virtual int value() = 0;
    Suit suit() { return _suit; };
    bool isAvailable() { return _available; };
    void markUnavailable() { _available = false; };
    void markAvailable() { _available = true; };
protected:
    int _faceValue;
    Suit _suit;
private:
    bool _available = true;
};

template<class T>
class Hand {
public:
    int score() {
        int score = 0;
        for (T card : cards) {
            score += card.value();
        }
        return score;
    }
    void addCard(T card) {
        cards.add(card);
    }
protected:
    vector<T> cards;
};

class BlackJackCard: public Card {
public:
    BlackJackCard(int c, Suit s): Card(c,s) {};
    int value() {
        if (isAce()) return 1;
        else if (_faceValue >= 11 && _faceValue <= 13) return 10;
        else return _faceValue;
    }
    int minValue() {
        if (isAce()) return 1;
        else return value();
    }
    int maxValue() {
        if (isAce()) return 11;
        else return value();
    }
    bool isAce() {
        return _faceValue == 1;
    }
    bool isFaceCard() {
        return _faceValue >= 11 && _faceValue <= 13;
    }
};

class BlackjackHand: public Hand<BlackJackCard> {
public:
    int score() {
        vector<int> scores = possibleScores();
        int maxUnder = INT_MIN, minOver = INT_MAX;
        for (auto a : scores) {
            if (a > 21 && a < minOver) {
                minOver = a;
            } else if (a <= 21 && a > maxUnder) {
                maxUnder = a;
            }
        }
        return maxUnder == INT_MIN ? minOver : maxUnder;
    }
    bool busted() { return score() > 21; };
    bool is21() { return score() == 21; };
    bool isBlackJack() {}; // ...
private:
    vector<int> possibleScores() {}; // ...
};
```














