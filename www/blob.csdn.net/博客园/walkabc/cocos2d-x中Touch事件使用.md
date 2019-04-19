# cocos2d-x中Touch事件使用 - walkabc - 博客园
# [cocos2d-x中Touch事件使用](https://www.cnblogs.com/hitfire/articles/3203298.html)
```
/*
 * Brick.cpp
 *
 *  Created on: 2013-7-20
 *      Author: freshman
 */
#include "Brick.h"
Brick::Brick() {
    _isTouchBegined = false;
}
Brick::~Brick() {
}
/**获取砖块所处的位置*/
CCRect Brick::getRect() {
    CCSize size = getContentSize();
    return CCRectMake( -size.width / 2, -size.height / 2, size.width, size.height);
}
Brick* Brick::createBrick(CCTexture2D *texture2D) {
    Brick* brick = new Brick();
    brick->initWithTexture(texture2D);
    brick->autorelease();
    return brick;
}
void Brick::onEnter() {
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(
            this, 0, true);
    CCSprite::onEnter();
}
void Brick::onExit() {
    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
    CCSprite::onExit();
}
bool Brick::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent) {
    if (_isTouchBegined) {
        return false;
    }
    CCRect rect = getRect();
    CCLog("getRect");
    if (!rect.containsPoint(this->convertTouchToNodeSpaceAR(pTouch))) {
        return false;
    }
    CCLog("ccTouchBegined");
    return _isTouchBegined = true;
}
void Brick::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent) {
    if (_isTouchBegined) {
        CCSize size = CCDirector::sharedDirector()->getVisibleSize();
        CCPoint point = CCDirector::sharedDirector()->getVisibleOrigin();
        CCRect rect = CCRectMake(point.x, point.y, size.width, size.height);
        point = pTouch->getLocation();
        if (rect.containsPoint(point)) {
            setPosition(point);
        }
    }
}
void Brick::ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent) {
    _isTouchBegined = false;
}
void Brick::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent) {
    if (!_isTouchBegined) {
        return;
    }
    CCLog("ccTouchEnded");
    _isTouchBegined = false;
}
```
```
1 /*
 2  * Brick.h
 3  *
 4  *  Created on: 2013-7-20
 5  *      Author: freshman
 6  */
 7 
 8 #ifndef BRICK_H_
 9 #define BRICK_H_
10 
11 #include "cocos2d.h"
12 USING_NS_CC;
13 
14 class Brick: public CCSprite, public CCTargetedTouchDelegate {
15 private:
16     bool _isTouchBegined;
17 public:
18     Brick();
19     virtual ~Brick();
20 
21     static Brick* createBrick(CCTexture2D *texture2d);
22     /**获取显示对象的矩形范围*/
23     CCRect getRect();
24 
25     virtual void onEnter();
26     virtual void onExit();
27     virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
28     virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
29     virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
30     virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);
31 };
32 
33 #endif /* BRICK_H_ */
```
实现四个ccTouchxx的方法就是可以使用touch时间机制了。
另外还要介绍一下onEnter和onExit两个方法，这两个方法在是什么时候会被调用呢？看下面源码
```
1 /* "add" logic MUST only be on this method
 2 * If a class want's to extend the 'addChild' behavior it only needs
 3 * to override this method
 4 */
 5 void CCNode::addChild(CCNode *child, int zOrder, int tag)
 6 {    
 7     CCAssert( child != NULL, "Argument must be non-nil");
 8     CCAssert( child->m_pParent == NULL, "child already added. It can't be added again");
 9 
10     if( ! m_pChildren )
11     {
12         this->childrenAlloc();
13     }
14 
15     this->insertChild(child, zOrder);
16 
17     child->m_nTag = tag;
18 
19     child->setParent(this);
20     child->setOrderOfArrival(s_globalOrderOfArrival++);
21 
22     if( m_bRunning )
23     {
　　　　　　/**没错就是在这个地方被调用*/
24         child->onEnter();
25         child->onEnterTransitionDidFinish();
26     }
27 }
```
```
1 void CCNode::detachChild(CCNode *child, bool doCleanup)
 2 {
 3     // IMPORTANT:
 4     //  -1st do onExit
 5     //  -2nd cleanup
 6     if (m_bRunning)
 7     {
 8        //没错就是这个时候调用了
 9         child->onExitTransitionDidStart();
10         child->onExit();
11     }
12 
13     // If you don't do cleanup, the child's actions will not get removed and the
14     // its scheduledSelectors_ dict will not get released!
15     if (doCleanup)
16     {
17         child->cleanup();
18     }
19 
20     // set parent nil at the end
21     child->setParent(NULL);
22 
23     m_pChildren->removeObject(child);
24 }
```
由此可见onEnter是在对象被添加到父节点的时候调用，而onExit是在该节点被移除出父节点时调用的，所以，我们一切初始化操作可以在onEnter中完成，而一些销毁的操作可以在onExit中完成。

