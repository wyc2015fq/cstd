# [Cocos2D-x For WP8]Box2D物理引擎 - weixin_33985507的博客 - CSDN博客
2013年08月31日 16:26:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：2
    物理引擎通过为刚性物体赋予真实的物理属性的方式来计算运动、旋转和碰撞反映。为每个游戏使用物理引擎并不是完全必要的—简单的“牛顿”物理（比如加速和减速）也可以在一定程度上通过编程或编写脚本来实现。然而，当游戏需要比较复杂的物体碰撞、滚动、滑动或者弹跳的时候（比如赛车类游戏或者保龄球游戏），通过编程的方法就比较困难了。那么着时候使用物理系统可以为游戏带来一些很明显的优点：
1）更加真实的对现实世界的模拟，以牛顿力学为基础的游戏效果。
    a) 游戏中的精灵们运动起来更真实：相互碰撞，自由下落等各种效果更加真实。
    b) 玩家操作起来随机性增大，物理碰撞决定他操作的效果。游戏体验增强。
2） 系统化的碰撞处理机制。
    a)  碰撞算法最优化，提高同一场景中，大量碰撞的运算效率。
    b)  可以处理复杂形状的碰撞。
    c)  允许游戏逻辑处理程序在最适合的时刻处理碰撞，实现最佳的游戏体验。
    Box2D是一个用于模拟2D刚体物体的C++引擎。Box2D是一个物理引擎，模拟一个真实的物理环境，上面有重力加速度，摩擦力，刚体能概念，在这个环境里，只要定义好相应的刚体和重力，摩擦力等外部环境，他们就可以自己处理碰撞。因为Box2D只是一个物理引擎，所以可以用在很多不同的开发平台和不同的游戏引擎。那么在Cocos2D-x里面是支持Box2D的使用的。
    那么在Box2D中有下面的一些概念，要对这些概念给理解了才能更好地去使用Box2D这个物理引擎去进行编程。
** 1.世界：**世界是遵循物理的空间，以上的所有都存在于世界中，可以创建多个世界，但很少这样用。
创建世界需要两个步骤，一是生成重力向量，二是根据重力生成世界对象
//生成重力向量
b2Vec2 gravity;
gravity.Set(0.0f, -10.0f);
//生成世界对象
booldoSleep = true;
world = new b2World(gravity);
world->SetAllowSleeping(doSleep);
world->SetContinuousPhysics(true);
** 2.刚体：** 即是物理学中的质点，只有位置，没有大小。
    它又可以区分为以下几类1）静态刚体：静态刚体没有质量，没有速度，只可以手动来改变他的位置；2）棱柱刚体：棱柱刚体没有质量，但是可以有速度，可以自己更新位置；3）动态刚体：动态刚体有质量也有速度。
    物理引擎需要首先定义一个描述类，然后再根据描述类通过世界创建某个对象。创建刚体时需要有两个步骤，一是生成一个刚体定义，二是根据刚体定义生成刚体。在刚体创建时定义中的信息会被复制，也就是说创建完成后刚体只要没被释放掉，就还可以重复使用。
//定义刚体
b2BodyDef groundBodyDef;
groundBodyDef.position.Set(screenSize.width/2/PTM_RATIO, screenSize.height/2/PTM_RATIO);
//生成刚体
b2Body* groundBody = world->CreateBody(&groundBodyDef);
**    3.形状：**通过关联添加到刚体上，碰撞可以根据形状来判定，具有摩擦和恢复等材料特性。
b2PolygonShape groundBox;
// bottom
groundBox.SetAsBox(screenSize.width/2/PTM_RATIO, 0, b2Vec2(0, -screenSize.height/2/PTM_RATIO), 0);
** 4.关联：**是一种附加在刚体上的属性，一个刚体可以有多个关联，创建关联时，需要定义关联的信息，然后通过刚体创建关联，当关联被创建时关联定义中的信息也会被保留，也可以重用
//定义并创建关联
b2FixtureDef fixtureDef;
fixtureDef.shape = &dynamicBox;
fixtureDef.density = 1.0f;
fixtureDef.friction = 0.3f;
body->CreateFixture(&fixtureDef);
** 5.链接：**链接可以联系多个缸体，使得刚体之间相互影响，同样的，也需要首先定义信息，之后通过世界创建链接，同样的，信息也可以被保留，从而重用。另外链接还支持限制和马达，限制就是限制物体运动的角度，马达就是依照关节中的限制来约束物体链接有旋转，棱柱和距离等
b2RevoluteJointDef rjd;
rjd.Initialize(m_attachment, m_platform, b2Vec2(0.0f, 5.0f));
rjd.maxMotorTorque = 50.0f;
rjd.enableMotor = true;
m_world->CreateJoint(&rjd);
**  6.约束：**一个约束就是消除物体自由度的物理连接。在 2D 中，一个物体有 3 个自由度。如果我们把一个物体钉在墙上(像摆锤那样)，那我们就把它约束到了墙上。这样，此物体就只能绕着这个钉子旋转，所以这个约束消除了它 2 个自由度。还有一种不须你创建的接触约束，一个防止刚体穿透，以及用于模拟摩擦和恢复的特殊约束。
    在WP8里面使用Cocos2D-x里面使用Box2D引擎我们需要把Cocos2D-x封装好的Box2D引擎库引入到项目里面。如下所示：
![](https://images0.cnblogs.com/blog/152755/201308/31161357-40341b6193244e7cb1f7e4fa35168cea.png)
    然后我们需要按照下面的步骤去在Cocos2D-x里面进行编程：
**（1）创建一个world对象，这个world对象管理物理仿真中的所有对象。**
    一旦我们已经创建了这个world对象，接下来需要往里面加入一些body对象。body对象可以随意移动，可以是怪物或者飞镖什么的，只要是参与碰撞的游戏对象都要为之创建一个相应的body对象。当然，也可以创建一些静态的body对象，用来表示游戏中的台阶或者墙壁等不可以移动的物体。
**（2）创建body对象。**
- 你首先创建一个body定义结构体，用以指定body的初始属性，比如位置或者速度。
- 一旦创建好body结构体后，你就可以调用world对象来创建一个body对象了。
- 然后，你为body对象定义一个shape，用以指定你想要仿真的物体的几何形状。
- 接着创建一个fixture定义，同时设置之前创建好的shape为fixture的一个属性，并且设置其它的属性，比如质量或者摩擦力。
- 最后，你可以使用body对象来创建fixture对象，通过传入一个fixture的定义结构就可以了。
- 请注意，你可以往单个body对象里面添加很多个fixture对象。这个功能在你创建特别复杂的对象的时候非常有用。比如自行车，你可能要创建2个轮子，车身等等，这些fixture可以用关节连接起来。
　　只要你把所有需要创建的body对象都创建好之后，box2d接下来就会接管工作，并且高效地进行物理仿真。
**（3）运作物理引擎。**
    周期性地调用world对象的step函数。一般会通过scheduleUpdate()方法，在游戏每一帧发生的时候都调用一次update函数，然后再update函数里面处理精灵的位置更新等。
示例代码，当点击屏幕的时候将会产生一个精灵往下面掉落下去：
```
class TestLayer : public cocos2d::CCLayer
{
protected:
    cocos2d::CCSprite* cat;
    b2World* world;
public:
    TestLayer(void);
    ~TestLayer(void);
    void addNewSpriteWithCoords(cocos2d::CCPoint p);
    void update(cocos2d::ccTime dt);
    virtual void ccTouchesEnded(cocos2d::CCSet* touches, cocos2d::CCEvent* event);
};
TestLayer::TestLayer()
{
    setTouchEnabled( true );
    CCSize screenSize = CCDirector::sharedDirector()->getWinSize();
    // 定义重力向量D
    b2Vec2 gravity;
    gravity.Set(0.0f, -10.0f);
    
    bool doSleep = true;
    // 通过重力构建生成世界
    world = new b2World(gravity);
    world->SetAllowSleeping(doSleep);
    world->SetContinuousPhysics(true);
    // 构建地面及墙壁，因为要构建一个空心的物体，因此我们不能直接定义，而是分别定义长方体里的四个边。
    // box2d采取的现实世界的米作为计量长度的单位，所以我们要把我们的像素级的长度单位转换为米的单位就要除以PTM_RATIO（定义32像素为1米）。
    // #define PTM_RATIO 32
    b2BodyDef groundBodyDef;
    groundBodyDef.position.Set(screenSize.width/2/PTM_RATIO, screenSize.height/2/PTM_RATIO); // bottom-left corner
    
    //创建刚体并把刚体添加到世界上
    b2Body* groundBody = world->CreateBody(&groundBodyDef);
    // 定义刚体的形状
    b2PolygonShape groundBox;
    // bottom
    groundBox.SetAsBox(screenSize.width/2/PTM_RATIO, 0, b2Vec2(0, -screenSize.height/2/PTM_RATIO), 0);
     groundBody->CreateFixture(&groundBox, 0);
    
    // top
    groundBox.SetAsBox(screenSize.width/2/PTM_RATIO, 0, b2Vec2(0, screenSize.height/2/PTM_RATIO), 0);
    groundBody->CreateFixture(&groundBox, 0);
    // left
    groundBox.SetAsBox(0, screenSize.height/2/PTM_RATIO, b2Vec2(-screenSize.width/2/PTM_RATIO, 0), 0);
    groundBody->CreateFixture(&groundBox, 0);
    // right
    groundBox.SetAsBox(0, screenSize.height/2/PTM_RATIO, b2Vec2(screenSize.width/2/PTM_RATIO, 0), 0);
    groundBody->CreateFixture(&groundBox, 0);
    //Set up sprite
    //CCSpriteBatchNode 中的所有CCSprite只会被渲染1次，因此可以提高游戏的FPS
    CCSpriteBatchNode *mgr = CCSpriteBatchNode::create("cat.png", 150);
    addChild(mgr, 0, 1);
    
    addNewSpriteWithCoords( CCPointMake(screenSize.width/2, screenSize.height/2) );
    
    CCLabelTTF *label = CCLabelTTF::create("Tap screen", "Marker Felt", 32);
    addChild(label, 0);
    label->setColor( ccc3(0,0,255) );
    label->setPosition( CCPointMake( screenSize.width/2, screenSize.height-50) );
    //定时更新，每一帧都会调用一次update函数
    scheduleUpdate();
}
TestLayer::~TestLayer()
{
    delete world;
    world = NULL;
}
//在当前的位置来产生一个精灵
void TestLayer::addNewSpriteWithCoords(CCPoint p)
{
    //创建精灵放到SpriteBatchNode里面
    CCSpriteBatchNode* batch = (CCSpriteBatchNode*)getChildByTag(1);
    CCSprite *sprite = CCSprite::createWithTexture(batch->getTexture());
    batch->addChild(sprite);
    //设置精灵的位置在当前的点击位置上
    sprite->setPosition( CCPointMake( p.x, p.y) );
    //定义动态刚体，然后创建到世界上去
    b2BodyDef bodyDef;
    //使刚体能够在力的作用下运行，刚体有三种：静态的、运动的、动态的
    bodyDef.type = b2_dynamicBody;
    //设置刚体的初始位置
    bodyDef.position.Set(p.x/PTM_RATIO, p.y/PTM_RATIO);
    //刚体所引用的数据就是我们所生成的精灵
    bodyDef.userData = sprite;
    b2Body *body = world->CreateBody(&bodyDef);
    
    //定义刚体的形状 
    b2PolygonShape dynamicBox;
    dynamicBox.SetAsBox(.5f, .5f);//These are mid points for our 1m box
    
    //定义刚体的纹理
    b2FixtureDef fixtureDef;
    //绑定形状
    fixtureDef.shape = &dynamicBox;    
    //设置密度
    fixtureDef.density = 1.0f;
    //设置摩擦
    fixtureDef.friction = 0.3f;
    body->CreateFixture(&fixtureDef);
}
void TestLayer::update(ccTime dt)
{
    int velocityIterations = 8;
    int positionIterations = 1;
    //Box2d是通过定期调用step来更新动画的，step的第一个参数是时间步，第二个参数是速度迭代次数，推荐8次，超过10次的基本看不出效果的提升，第三个参数是位置迭代
    world->Step(dt, velocityIterations, positionIterations);
    
    //遍历整个世界，找出对应精灵的刚体，进行位置更新
    for (b2Body* b = world->GetBodyList(); b; b = b->GetNext())
    {
        if (b->GetUserData() != NULL) {
            //Synchronize the AtlasSprites position and rotation with the corresponding body
            CCSprite* myActor = (CCSprite*)b->GetUserData();
            myActor->setPosition( CCPointMake( b->GetPosition().x * PTM_RATIO, b->GetPosition().y * PTM_RATIO) );
            myActor->setRotation( -1 * CC_RADIANS_TO_DEGREES(b->GetAngle()) );
        }    
    }
}
//点击屏幕事件
void TestLayer::ccTouchesEnded(CCSet* touches, CCEvent* event)
{
    //在点击的位置上创建一个新的精灵Add a new body/atlas sprite at the touched location
    CCSetIterator it;
    CCTouch* touch;
    //循环获取点击的位置
    for( it = touches->begin(); it != touches->end(); it++) 
    {
        touch = (CCTouch*)(*it);
        if(!touch)
            break;
        CCPoint location = touch->getLocationInView();
        
        location = CCDirector::sharedDirector()->convertToGL(location);
        //在当前的位置来产生一个精灵
        addNewSpriteWithCoords( location );
    }
}
```
运行的效果：
![](https://images0.cnblogs.com/blog/152755/201308/31162548-b16a716f3dc246339abe146434d550e5.png)
