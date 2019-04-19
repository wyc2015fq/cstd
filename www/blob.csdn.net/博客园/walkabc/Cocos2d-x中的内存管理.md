# Cocos2d-x中的内存管理 - walkabc - 博客园
# [Cocos2d-x中的内存管理](https://www.cnblogs.com/hitfire/articles/3177957.html)
cocos2d-x中内置了很多的容器类，每个容器类都是容纳CCObject的对象，在对象被添加到容器中时，他的引用就增加了一。
下面举一个例子说明：
CCArray中有addObject方法，负责把一个CCObject对象添加到这个容器中，在使用addObject添加进来的对象的引用都会加一。
```
void CCArray::addObject(CCObject* object)
{
    ccArrayAppendObjectWithResize(data, object);
}
```
```
/** Appends objects from plusArr to arr. Capacity of arr is increased if needed. */
void ccArrayAppendArrayWithResize(ccArray *arr, ccArray *plusArr)
{
    ccArrayEnsureExtraCapacity(arr, plusArr->num);
    ccArrayAppendArray(arr, plusArr);
}
```
```
/** Appends an object. Behavior undefined if array doesn't have enough capacity. */
void ccArrayAppendObject(ccArray *arr, CCObject* object)
{
    CCAssert(object != NULL, "Invalid parameter!");
    object->retain();
    arr->arr[arr->num] = object;
    arr->num++;
}
```
可以看到在CCObject的对象被添加到容器中的时候哦，retain()方法被调用，对象的引用增加了。
所以在之后，应该主动release掉这个引用。

