# Cocos2d-x事件探究 - walkabc - 博客园
# [Cocos2d-x事件探究](https://www.cnblogs.com/hitfire/articles/3166062.html)
不多说，上源码，setFrameSize这个函数大家都比较熟悉吧，没错，main函数中的关键函数，定义了游戏窗口的大小，但是它可不只是定义窗口大小的。
```
void CCEGLView::setFrameSize(float width, float height)
{
    bool eResult = false;
    int u32GLFWFlags = GLFW_WINDOW;
    //create the window by glfw.
    //check
    CCAssert(width!=0&&height!=0, "invalid window's size equal 0");
    //Inits GLFW
    eResult = glfwInit() != GL_FALSE;
    if (!eResult) {
        CCAssert(0, "fail to init the glfw");
    }
    /* Updates window hint */
    glfwOpenWindowHint(GLFW_WINDOW_NO_RESIZE, GL_TRUE);
    int iDepth = 16; // set default value
    /* Depending on video depth */
    switch(iDepth)
    {
        /* 16-bit */
        case 16:
        {
            /* Updates video mode */
            eResult = (glfwOpenWindow(width, height, 5, 6, 5, 0, 16, 8, (int)u32GLFWFlags) != false) ? true : false;
            break;
        }
        /* 24-bit */
        case 24:
        {
            /* Updates video mode */
            eResult = (glfwOpenWindow(width, height, 8, 8, 8, 0, 16, 8, (int)u32GLFWFlags) != false) ? true : false;
            break;
        }
        /* 32-bit */
        default:
        case 32:
        {
            /* Updates video mode */
            eResult = (glfwOpenWindow(width, height, 8, 8, 8, 8, 16, 8, (int)u32GLFWFlags) != GL_FALSE) ? true :false;
            break;
        }
    }
    /* Success? */
    if(eResult)
    {
        /* Updates actual size */
      //        glfwGetWindowSize(&width, &height);
        CCEGLViewProtocol::setFrameSize(width, height);        
        /* Updates its title */
        glfwSetWindowTitle("Cocos2dx-Linux");
        //set the init flag
        bIsInit = true;
        //register the glfw key event
        glfwSetKeyCallback(keyEventHandle);
        //register the glfw char event
        glfwSetCharCallback(charEventHandle);
        //register the glfw mouse event
        glfwSetMouseButtonCallback(mouseButtonEventHandle);
        //register the glfw mouse pos event
        glfwSetMousePosCallback(mousePosEventHandle);
        glfwSetWindowCloseCallback(closeEventHandle);
        //Inits extensions
        eResult = initExtensions();
        if (!eResult) {
            CCAssert(0, "fail to init the extensions of opengl");
        }
        initGL();
    }
}
```
glfwSetMouseButtonCallback(mouseButtonEventHandle);这个函数就是一个回调，在鼠标落下的时候回调。再往下看mouseButtonEventHandle函数
```
void mouseButtonEventHandle(int iMouseID,int iMouseState) {
    if (iMouseID == GLFW_MOUSE_BUTTON_LEFT) {
        CCEGLView* pEGLView = CCEGLView::sharedOpenGLView();
        //get current mouse pos
        int x,y;
        glfwGetMousePos(&x, &y);
        CCPoint oPoint((float)x,(float)y);
        /*
        if (!CCRect::CCRectContainsPoint(s_pMainWindow->m_rcViewPort,oPoint))
        {
            CCLOG("not in the viewport");
            return;
        }
        */
         oPoint.x /= pEGLView->m_fFrameZoomFactor;
         oPoint.y /= pEGLView->m_fFrameZoomFactor;
        int id = 0;
        if (iMouseState == GLFW_PRESS) {
            pEGLView->handleTouchesBegin(1, &id, &oPoint.x, &oPoint.y);
        } else if (iMouseState == GLFW_RELEASE) {
            pEGLView->handleTouchesEnd(1, &id, &oPoint.x, &oPoint.y);
        }
    }
}
```
再向下看
```
void CCEGLViewProtocol::handleTouchesBegin(int num, int ids[], float xs[], float ys[])
{
    CCSet set;
    for (int i = 0; i < num; ++i)
    {
        int id = ids[i];
        float x = xs[i];
        float y = ys[i];
        CCInteger* pIndex = (CCInteger*)s_TouchesIntergerDict.objectForKey(id);
        int nUnusedIndex = 0;
        // it is a new touch
        if (pIndex == NULL)
        {
            nUnusedIndex = getUnUsedIndex();
            // The touches is more than MAX_TOUCHES ?
            if (nUnusedIndex == -1) {
                CCLOG("The touches is more than MAX_TOUCHES, nUnusedIndex = %d", nUnusedIndex);
                continue;
            }
            CCTouch* pTouch = s_pTouches[nUnusedIndex] = new CCTouch();
            pTouch->setTouchInfo(nUnusedIndex, (x - m_obViewPortRect.origin.x) / m_fScaleX, 
                                     (y - m_obViewPortRect.origin.y) / m_fScaleY);
            
            //CCLOG("x = %f y = %f", pTouch->getLocationInView().x, pTouch->getLocationInView().y);
            
            CCInteger* pInterObj = new CCInteger(nUnusedIndex);
            s_TouchesIntergerDict.setObject(pInterObj, id);
            set.addObject(pTouch);
            pInterObj->release();
        }
    }
    if (set.count() == 0)
    {
        CCLOG("touchesBegan: count = 0");
        return;
    }
    m_pDelegate->touchesBegan(&set, NULL);
}
```
再向下看
```
void CCTouchDispatcher::touchesBegan(CCSet *touches, CCEvent *pEvent)
{
    if (m_bDispatchEvents)
    {
        this->touches(touches, pEvent, CCTOUCHBEGAN);
    }
}
```
```
//
// dispatch events
//
void CCTouchDispatcher::touches(CCSet *pTouches, CCEvent *pEvent, unsigned int uIndex)
{
    CCAssert(uIndex >= 0 && uIndex < 4, "");
    CCSet *pMutableTouches;
    m_bLocked = true;
    // optimization to prevent a mutable copy when it is not necessary
     unsigned int uTargetedHandlersCount = m_pTargetedHandlers->count();
     unsigned int uStandardHandlersCount = m_pStandardHandlers->count();
    bool bNeedsMutableSet = (uTargetedHandlersCount && uStandardHandlersCount);
    pMutableTouches = (bNeedsMutableSet ? pTouches->mutableCopy() : pTouches);
    struct ccTouchHandlerHelperData sHelper = m_sHandlerHelperData[uIndex];
    //
    // process the target handlers 1st
    //
    if (uTargetedHandlersCount > 0)
    {
        CCTouch *pTouch;
        CCSetIterator setIter;
        for (setIter = pTouches->begin(); setIter != pTouches->end(); ++setIter)
        {
            pTouch = (CCTouch *)(*setIter);
            CCTargetedTouchHandler *pHandler = NULL;
            CCObject* pObj = NULL;
            CCARRAY_FOREACH(m_pTargetedHandlers, pObj)
            {
                pHandler = (CCTargetedTouchHandler *)(pObj);
                if (! pHandler)
                {
                   break;
                }
                bool bClaimed = false;
                if (uIndex == CCTOUCHBEGAN)
                {
                    bClaimed = pHandler->getDelegate()->ccTouchBegan(pTouch, pEvent);
                    if (bClaimed)
                    {
                        pHandler->getClaimedTouches()->addObject(pTouch);
                    }
                } else
                if (pHandler->getClaimedTouches()->containsObject(pTouch))
                {
                    // moved ended canceled
                    bClaimed = true;
                    switch (sHelper.m_type)
                    {
                    case CCTOUCHMOVED:
                        pHandler->getDelegate()->ccTouchMoved(pTouch, pEvent);
                        break;
                    case CCTOUCHENDED:
                        pHandler->getDelegate()->ccTouchEnded(pTouch, pEvent);
                        pHandler->getClaimedTouches()->removeObject(pTouch);
                        break;
                    case CCTOUCHCANCELLED:
                        pHandler->getDelegate()->ccTouchCancelled(pTouch, pEvent);
                        pHandler->getClaimedTouches()->removeObject(pTouch);
                        break;
                    }
                }
                if (bClaimed && pHandler->isSwallowsTouches())
                {
                    if (bNeedsMutableSet)
                    {
                        pMutableTouches->removeObject(pTouch);
                    }
                    break;
                }
            }
        }
    }
    //
    // process standard handlers 2nd
    //
    if (uStandardHandlersCount > 0 && pMutableTouches->count() > 0)
    {
        CCStandardTouchHandler *pHandler = NULL;
        CCObject* pObj = NULL;
        CCARRAY_FOREACH(m_pStandardHandlers, pObj)
        {
            pHandler = (CCStandardTouchHandler*)(pObj);
            if (! pHandler)
            {
                break;
            }
            switch (sHelper.m_type)
            {
            case CCTOUCHBEGAN:
                pHandler->getDelegate()->ccTouchesBegan(pMutableTouches, pEvent);
                break;
            case CCTOUCHMOVED:
                pHandler->getDelegate()->ccTouchesMoved(pMutableTouches, pEvent);
                break;
            case CCTOUCHENDED:
                pHandler->getDelegate()->ccTouchesEnded(pMutableTouches, pEvent);
                break;
            case CCTOUCHCANCELLED:
                pHandler->getDelegate()->ccTouchesCancelled(pMutableTouches, pEvent);
                break;
            }
        }
    }
    if (bNeedsMutableSet)
    {
        pMutableTouches->release();
    }
    //
    // Optimization. To prevent a [handlers copy] which is expensive
    // the add/removes/quit is done after the iterations
    //
    m_bLocked = false;
    if (m_bToRemove)
    {
        m_bToRemove = false;
        for (unsigned int i = 0; i < m_pHandlersToRemove->num; ++i)
        {
            forceRemoveDelegate((CCTouchDelegate*)m_pHandlersToRemove->arr[i]);
        }
        ccCArrayRemoveAllValues(m_pHandlersToRemove);
    }
    if (m_bToAdd)
    {
        m_bToAdd = false;
        CCTouchHandler* pHandler = NULL;
        CCObject* pObj = NULL;
        CCARRAY_FOREACH(m_pHandlersToAdd, pObj)
         {
             pHandler = (CCTouchHandler*)pObj;
            if (! pHandler)
            {
                break;
            }
            if (dynamic_cast<CCTargetedTouchHandler*>(pHandler) != NULL)
            {                
                forceAddHandler(pHandler, m_pTargetedHandlers);
            }
            else
            {
                forceAddHandler(pHandler, m_pStandardHandlers);
            }
         }
 
         m_pHandlersToAdd->removeAllObjects();    
    }
    if (m_bToQuit)
    {
        m_bToQuit = false;
        forceRemoveAllDelegates();
    }
}
```
好了，事件已经派发到了没一个节点上面。cocos2d-x的事件机制就是这样的了。

