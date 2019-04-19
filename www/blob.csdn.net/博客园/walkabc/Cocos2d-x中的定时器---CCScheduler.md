# Cocos2d-x中的定时器---CCScheduler - walkabc - 博客园
# [Cocos2d-x中的定时器---CCScheduler](https://www.cnblogs.com/hitfire/articles/3163577.html)
　　每个Node对象都要一个protected的指针变量m_pScheduler，这个指针指向的是director->getScheduler();有源码证明　　
```
1 CCNode::CCNode(void)
 2 : m_fRotationX(0.0f)
 3 , m_fRotationY(0.0f)
 4 , m_fScaleX(1.0f)
 5 , m_fScaleY(1.0f)
 6 , m_fVertexZ(0.0f)
 7 , m_obPosition(CCPointZero)
 8 , m_fSkewX(0.0f)
 9 , m_fSkewY(0.0f)
10 , m_obAnchorPointInPoints(CCPointZero)
11 , m_obAnchorPoint(CCPointZero)
12 , m_obContentSize(CCSizeZero)
13 , m_sAdditionalTransform(CCAffineTransformMakeIdentity())
14 , m_pCamera(NULL)
15 // children (lazy allocs)
16 // lazy alloc
17 , m_pGrid(NULL)
18 , m_nZOrder(0)
19 , m_pChildren(NULL)
20 , m_pParent(NULL)
21 // "whole screen" objects. like Scenes and Layers, should set m_bIgnoreAnchorPointForPosition to true
22 , m_nTag(kCCNodeTagInvalid)
23 // userData is always inited as nil
24 , m_pUserData(NULL)
25 , m_pUserObject(NULL)
26 , m_pShaderProgram(NULL)
27 , m_eGLServerState(ccGLServerState(0))
28 , m_uOrderOfArrival(0)
29 , m_bRunning(false)
30 , m_bTransformDirty(true)
31 , m_bInverseDirty(true)
32 , m_bAdditionalTransformDirty(false)
33 , m_bVisible(true)
34 , m_bIgnoreAnchorPointForPosition(false)
35 , m_bReorderChildDirty(false)
36 , m_nScriptHandler(0)
37 , m_nUpdateScriptHandler(0)
38 , m_pComponentContainer(NULL)
39 {
40     // set default scheduler and actionManager
41     CCDirector *director = CCDirector::sharedDirector();
42     m_pActionManager = director->getActionManager();
43     m_pActionManager->retain();
44     /**这里设置了m_pScheduler的所指向的对象*/
45     m_pScheduler = director->getScheduler();
46     m_pScheduler->retain();
47 
48     CCScriptEngineProtocol* pEngine = CCScriptEngineManager::sharedManager()->getScriptEngine();
49     m_eScriptType = pEngine != NULL ? pEngine->getScriptType() : kScriptTypeNone;
50     m_pComponentContainer = new CCComponentContainer(this);
51 }
```
 　　如果想使用定时器，可以使用
```
1 void CCNode::schedule(SEL_SCHEDULE selector, float interval, unsigned int repeat, float delay)
```
　　设置好定时器，但是底层究竟是怎么运作的，下面给出说明
```
1 // Draw the Scene
 2 void CCDirector::drawScene(void)
 3 {
 4     // calculate "global" dt
 5     calculateDeltaTime();
 6 
 7     //tick before glClear: issue #533
 8     /**
 9      * 这个方法在游戏的每一帧调用
10      * 对的～～～就是在这个地方，我们之前设置的定时器被执行了
11      * */
12     if (! m_bPaused)
13     {
14         m_pScheduler->update(m_fDeltaTime);
15     }
16 
17     glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
18 
19     /* to avoid flickr, nextScene MUST be here: after tick and before draw.
20      XXX: Which bug is this one. It seems that it can't be reproduced with v0.9 */
21     if (m_pNextScene)
22     {
23         setNextScene();
24     }
25 
26     kmGLPushMatrix();
27 
28     // draw the scene
29     if (m_pRunningScene)
30     {
31         m_pRunningScene->visit();
32     }
33 
34     // draw the notifications node
35     if (m_pNotificationNode)
36     {
37         m_pNotificationNode->visit();
38     }
39     
40     if (m_bDisplayStats)
41     {
42         showStats();
43     }
44 
45     kmGLPopMatrix();
46 
47     m_uTotalFrames++;
48 
49     // swap buffers
50     if (m_pobOpenGLView)
51     {
52         m_pobOpenGLView->swapBuffers();
53     }
54     
55     if (m_bDisplayStats)
56     {
57         calculateMPF();
58     }
59 }
```
　　使用完毕记得取消该定时器　　
```
void CCNode::unschedule(SEL_SCHEDULE selector)
```
　　下面我们列举一个例子详细介绍一个该如何使用。
```
1 /**在node的派生类对象初始化的时候，添加上这一句*/
2 schedule(schedule_selector(HelloWorld::tick), 0.5f);
3 
4 /**在派生类中添加一个新方法*/
5 void HelloWorld::tick()
6 {
7     CCLog("tick1");
8 }
```
　　运行的话，就会每隔0.5秒会输出一句：
　　tick1
　　其实关于，定时器的底层还有一些东西没有介绍，不过不影响我们开发使用了。够用就行。

