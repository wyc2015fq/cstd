# FBKVOController详解 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2017年05月18日 15:18:04[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：349
# 本文来自简书，原文地址：[http://www.jianshu.com/p/1f7d70ff2002](http://www.jianshu.com/p/1f7d70ff2002)
# 前言
前段时间刚到公司，公司在使用FBKVOController，本人一直在使用系统的KVO，没有使用过Facebook的这个框架，使用起来挺方便的，所以安利一波，并且读读源码，本文只是略读，了解了FBKVOController的结构和基本实现，可能他的设计思想还没有深入理解，以后慢慢探讨。
# FBKVOController的使用
```
//
//  ViewController.m
//  FBKVOControllerDemo
//
//  Created by 李林 on 2017/5/17.
//  Copyright © 2017年 lee. All rights reserved.
//
#import "ViewController.h"
#import <KVOController/KVOController.h>
@interface ViewController (){
    FBKVOController *_kvoCtrl;
}
@property (weak, nonatomic) IBOutlet UIButton *button;
@property (weak, nonatomic) IBOutlet UIView *colorView;
@property (nonatomic, assign) NSInteger index;
@end
@implementation ViewController
- (void)viewDidLoad {
    [super viewDidLoad];
    self.index = 0;
    [self.button addTarget:self action:@selector(buttonClick) forControlEvents:UIControlEventTouchUpInside];
    // FBKVOController
    _kvoCtrl = [FBKVOController controllerWithObserver:self];
    [_kvoCtrl observe:self keyPath:@"index" options:0 action:@selector(changeColor)];
}
- (void)buttonClick {
    self.index++;
}
- (void)changeColor {
    self.colorView.backgroundColor = [UIColor redColor];
}
@end
```
使用很简单，监测某个对象的值，然后将selector写入observe函数中，当值发生改变，就会调用通知的函数。效果如下图。
![](http://upload-images.jianshu.io/upload_images/2550732-a609802c24bdd127.gif?imageMogr2/auto-orient/strip)
FBKVOController.gif
# 源码简析
#### 项目结构简介
FBKVOController框架主要分两部分：一部分是FBKVOController，主要实现了键值观测，另一部分是NSObject+FBKVOController，主要是实现了初始化的方法。本文主要介绍FBKVOController，这也是这个框架的核心。（结构如下图）
![](http://upload-images.jianshu.io/upload_images/2550732-39d9addd25f0d342.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
KVOController结构
#### 键值观测与销毁
###### FBKVOController的结构
首先我们打开FBKVOController.h文件，发现三个类：FBKVOInfo、FBKVOSharedController和FBKVOController。
1.其中FBKVOInfo主要是对需要观测的信息的包装，包含了action、block、options等等，改类中重写了hash，isEqual等方法，用来判断是否两个Info是否一致。（后续会有用）
```
@implementation _FBKVOInfo
{
@public
  __weak FBKVOController *_controller;
  NSString *_keyPath;
  NSKeyValueObservingOptions _options;
  SEL _action;
  void *_context;
  FBKVONotificationBlock _block;
  _FBKVOInfoState _state;
}
```
2.FBKVOController是核心类，包含MapTable和pthread_mutex_lock，其中_objectInfosMap是存储一个对象对应的KVOInfo的映射关系，也就是说这里<id, NSMutableSet<_FBKVOInfo *> *> 中的id就是对象，
MutableSet就是KVOInfos，各种键值观测的包装。
```
@implementation FBKVOController
{
  NSMapTable<id, NSMutableSet<_FBKVOInfo *> *> *_objectInfosMap;
  pthread_mutex_t _lock;
}
```
3.FBKVOSharedController是一个实际操作类，负责将FBKVOController发送过来的信息转发给系统的KVO处理。
```
@implementation _FBKVOSharedController
{
  NSHashTable<_FBKVOInfo *> *_infos;
  pthread_mutex_t _mutex;
}
```
###### 键值观测的流程
`[_kvoCtrl observe:self keyPath:@"index" options:0 action:@selector(changeColor)];`
我们从写的例子开始走起，可以把FBKVOCOntroller的流程看一遍。
（PS：这也是我现在看框架原码的方法）
```
- (void)observe:(nullable id)object keyPath:(NSString *)keyPath options:(NSKeyValueObservingOptions)options action:(SEL)action
{
  NSAssert(0 != keyPath.length && NULL != action, @"missing required parameters observe:%@ keyPath:%@ action:%@", object, keyPath, NSStringFromSelector(action));
  NSAssert([_observer respondsToSelector:action], @"%@ does not respond to %@", _observer, NSStringFromSelector(action));
  if (nil == object || 0 == keyPath.length || NULL == action) {
    return;
  }
  // create info
  _FBKVOInfo *info = [[_FBKVOInfo alloc] initWithController:self keyPath:keyPath options:options action:action];
  // observe object with info
  [self _observe:object info:info];
}
```
这个函数
```
- (void)observe:(nullable id)object keyPath:(NSString
 *)keyPath options:(NSKeyValueObservingOptions)options action:(SEL)action
```
是FBKVOController的，首先是断言判断是否为空，然后创造一个FBKVOInfo，最后调用本身的observe方法，将包装的FBKVOInfo的info传过去。
```
- (void)_observe:(id)object info:(_FBKVOInfo *)info
{
  // lock
  pthread_mutex_lock(&_lock);
  NSMutableSet *infos = [_objectInfosMap objectForKey:object];
  // check for info existence
  _FBKVOInfo *existingInfo = [infos member:info];
  if (nil != existingInfo) {
    // observation info already exists; do not observe it again
    // unlock and return
    pthread_mutex_unlock(&_lock);
    return;
  }
  // lazilly create set of infos
  if (nil == infos) {
    infos = [NSMutableSet set];
    [_objectInfosMap setObject:infos forKey:object];
  }
  // add info and oberve
  [infos addObject:info];
  // unlock prior to callout
  pthread_mutex_unlock(&_lock);
  [[_FBKVOSharedController sharedController] observe:object info:info];
}
```
这个函数`- (void)_observe:(id)object info:(_FBKVOInfo *)info`是FBKVOController的，首先是加锁，防止读写干扰。然后我们查找一下这个object对应的MutableSet，如果有对应的KVOInfo的话，那么就不需要再添加入_objectInfosMap中了；如果没有，则创建info，并且加入_objectInfosMap中。最后解锁，将object传给_FBKVOSharedController处理。
```
- (void)observe:(id)object info:(nullable _FBKVOInfo *)info
{
  if (nil == info) {
    return;
  }
  // register info
  pthread_mutex_lock(&_mutex);
  [_infos addObject:info];
  pthread_mutex_unlock(&_mutex);
  // add observer
  [object addObserver:self forKeyPath:info->_keyPath options:info->_options context:(void *)info];
  if (info->_state == _FBKVOInfoStateInitial) {
    info->_state = _FBKVOInfoStateObserving;
  } else if (info->_state == _FBKVOInfoStateNotObserving) {
    // this could happen when `NSKeyValueObservingOptionInitial` is one of the NSKeyValueObservingOptions,
    // and the observer is unregistered within the callback block.
    // at this time the object has been registered as an observer (in Foundation KVO),
    // so we can safely unobserve it.
    [object removeObserver:self forKeyPath:info->_keyPath context:(void *)info];
  }
}
```
这个`- (void)observe:(id)object info:(nullable _FBKVOInfo *)info`函数是
###### 销毁
```
- (void)dealloc
{
  [self unobserveAll];
  pthread_mutex_destroy(&_lock);
}
```
停止所有的键值观测，销毁锁。
```
- (void)_unobserveAll
{
  // lock
  pthread_mutex_lock(&_lock);
  NSMapTable *objectInfoMaps = [_objectInfosMap copy];
  // clear table and map
  [_objectInfosMap removeAllObjects];
  // unlock
  pthread_mutex_unlock(&_lock);
  _FBKVOSharedController *shareController = [_FBKVOSharedController sharedController];
  for (id object in objectInfoMaps) {
    // unobserve each registered object and infos
    NSSet *infos = [objectInfoMaps objectForKey:object];
    [shareController unobserve:object infos:infos];
  }
}
```
这个函数`- (void)_unobserveAll`是FBKVOController的，首先是将objectInfoMaps中的Remove，在这个操作是加锁，防止读写。然后调用FBKVOSharedController的unobserve方法，停止观测。
# 致谢
本文主要参考的博客:
[https://github.com/facebook/KVOController](https://github.com/facebook/KVOController)
[http://chaosgsc.com/kvo.html](http://chaosgsc.com/kvo.html)
[https://satanwoo.github.io/2016/02/27/FBKVOController/](https://satanwoo.github.io/2016/02/27/FBKVOController/)
