# cocos2d-x中的touch事件 - walkabc - 博客园
# [cocos2d-x中的touch事件](https://www.cnblogs.com/hitfire/articles/3164186.html)
　　太晚了，先放上源码再说。
```
#include "Paddle.h"
Paddle::Paddle(void)
{
}
Paddle::~Paddle(void)
{
}
CCRect Paddle::rect()
{
    CCSize s = getTexture()->getContentSize();
    return CCRectMake(-s.width / 2, -s.height / 2, s.width, s.height);
}
Paddle* Paddle::paddleWithTexture(CCTexture2D* aTexture)
{
    Paddle* pPaddle = new Paddle();
    pPaddle->initWithTexture( aTexture );
    pPaddle->autorelease();
    return pPaddle;
}
bool Paddle::initWithTexture(CCTexture2D* aTexture)
{
    if( CCSprite::initWithTexture(aTexture) ) 
    {
        m_state = kPaddleStateUngrabbed;
    }
    
    return true;
}
void Paddle::onEnter()
{
    CCDirector* pDirector = CCDirector::sharedDirector();
    pDirector->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
    CCSprite::onEnter();
}
void Paddle::onExit()
{
    CCDirector* pDirector = CCDirector::sharedDirector();
    pDirector->getTouchDispatcher()->removeDelegate(this);
    CCSprite::onExit();
}    
bool Paddle::containsTouchLocation(CCTouch* touch)
{
    return rect().containsPoint(convertTouchToNodeSpaceAR(touch));
}
bool Paddle::ccTouchBegan(CCTouch* touch, CCEvent* event)
{
    if (m_state != kPaddleStateUngrabbed) return false;
    if ( !containsTouchLocation(touch) ) return false;
    
    m_state = kPaddleStateGrabbed;
    return true;
}
void Paddle::ccTouchMoved(CCTouch* touch, CCEvent* event)
{
    // If it weren't for the TouchDispatcher, you would need to keep a reference
    // to the touch from touchBegan and check that the current touch is the same
    // as that one.
    // Actually, it would be even more complicated since in the Cocos dispatcher
    // you get CCSets instead of 1 UITouch, so you'd need to loop through the set
    // in each touchXXX method.
    
    CCAssert(m_state == kPaddleStateGrabbed, "Paddle - Unexpected state!");    
    
    CCPoint touchPoint = touch->getLocation();
    
    setPosition( ccp(touchPoint.x, getPosition().y) );
}
CCObject* Paddle::copyWithZone(CCZone *pZone)
{
    this->retain();
    return this;
}
void Paddle::ccTouchEnded(CCTouch* touch, CCEvent* event)
{
    CCAssert(m_state == kPaddleStateGrabbed, "Paddle - Unexpected state!");    
    
    m_state = kPaddleStateUngrabbed;
} 
void Paddle::touchDelegateRetain()
{
    this->retain();
}
void Paddle::touchDelegateRelease()
{
    this->release();
}
```
```
#ifndef _PADDLE_H_
#define _PADDLE_H_
#include "cocos2d.h"
USING_NS_CC;
typedef enum tagPaddleState 
{
    kPaddleStateGrabbed,
    kPaddleStateUngrabbed
} PaddleState; 
class Paddle : public CCSprite, public CCTargetedTouchDelegate
{
    PaddleState        m_state;
public:
    Paddle(void);
    virtual ~Paddle(void);
    CCRect rect();
    bool initWithTexture(CCTexture2D* aTexture);
    virtual void onEnter();
    virtual void onExit();
    bool containsTouchLocation(CCTouch* touch);
    virtual bool ccTouchBegan(CCTouch* touch, CCEvent* event);
    virtual void ccTouchMoved(CCTouch* touch, CCEvent* event);
    virtual void ccTouchEnded(CCTouch* touch, CCEvent* event);
    virtual CCObject* copyWithZone(CCZone *pZone);
    virtual void touchDelegateRetain();
    virtual void touchDelegateRelease();
    static Paddle* paddleWithTexture(CCTexture2D* aTexture);
};
#endif
```

