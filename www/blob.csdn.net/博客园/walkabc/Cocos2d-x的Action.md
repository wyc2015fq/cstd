# Cocos2d-x的Action - walkabc - 博客园
# [Cocos2d-x的Action](https://www.cnblogs.com/hitfire/articles/3176798.html)
在cocos2d-x中，CCNode的移动都是通过Action来执行的。下面介绍几款常见的Action，比如移动，旋转
```
CCLabelTTF* pLabel = CCLabelTTF::create("Hello World", "Arial", TITLE_FONT_SIZE);
    
    // position the label on the center of the screen
    pLabel->setPosition(ccp(origin.x + visibleSize.width/2,
                            origin.y + visibleSize.height - pLabel->getContentSize().height));
    // add the label as a child to this layer
    this->addChild(pLabel, 1);
    CCRotateBy *rotate = CCRotateBy::create(0.2f, 360.0f);
    CCRepeatForever *repeatForever = CCRepeatForever::create(rotate);
//    repeatForever->startWithTarget()
    pLabel->runAction(repeatForever);
```
上面的代码实现了Label重复的旋转。
```
CCLabelTTF* pLabel = CCLabelTTF::create("Hello World", "Arial", TITLE_FONT_SIZE);
    
    // position the label on the center of the screen
    pLabel->setPosition(ccp(origin.x + visibleSize.width/2,
                            origin.y + visibleSize.height - pLabel->getContentSize().height));
    // add the label as a child to this layer
    this->addChild(pLabel, 1);
    CCMoveTo *moveTo = CCMoveTo::create(5, CCPointMake(0, 0));
    pLabel->runAction(moveTo);
```
上面的代码实现了Label移动到(0, 0)点

