# django中的signals - Arthur的随笔 - CSDN博客
2011年11月22日 23:37:12[largetalk](https://me.csdn.net/largetalk)阅读数：8324
django中得signals和操作系统（linux）中的signal完全是两会事，后者的signal是软件中断，提供一种处理异步事件得方法，信号是系统定义好的，可用作进程间传递消息得一种方法，而django中的信号只是一个普通的类，不能跨进程，看其代码更像一个。。。callback。。。
django signal类定义在django/dispatch/dispatch.py中
```python
class Signal(object):
 
    def __init__(self, providing_args=None):
        #providing_args 定义receiver调用参数格式，为None也没关系
        self.receivers = []
        ......
    def connect(self, receiver, sender=None, weak=True, dispatch_uid=None):
        #看清楚了，其实就是把receiver保存起来，receiver是一个函数对象，就是该signal得handler
        ......
        if dispatch_uid:
            lookup_key = (dispatch_uid, _make_id(sender))
        else:
            lookup_key = (_make_id(receiver), _make_id(sender))
        ......
        self.lock.acquire()
        try:
            for r_key, _ in self.receivers:
                if r_key == lookup_key:
                    break
            else:
                self.receivers.append((lookup_key, receiver))
        finally:
            self.lock.release()
    def disconnect(self, receiver=None, sender=None, weak=True, dispatch_uid=None):
        #取消connect，把receiver从self.receivers删除就行了
        ......
         
    def send(self, sender, **named):
        #在事件发生时调用，发出信号，如有receive connect该信号，则调用之
        responses = []
        if not self.receivers:
            return responses
        for receiver in self._live_receivers(_make_id(sender)):
            response = receiver(signal=self, sender=sender, **named)
            responses.append((receiver, response))
        return responses
    def send_robust(self, sender, **named):
        #基本同上
        ......
    def _live_receivers(self, senderkey):
        #从self.receivers中找出相应得receiver
        ......
    def _remove_receiver(self, receiver):
        """
        Remove dead receivers from connections.
        """
        ......
```
整个信号得定义就这么简单，再来看看django中已定义好得一些signal, 在django/db/models/signal.py中
```python
from django.dispatch import Signal
class_prepared = Signal(providing_args=["class"])
pre_init = Signal(providing_args=["instance", "args", "kwargs"])
post_init = Signal(providing_args=["instance"])
pre_save = Signal(providing_args=["instance", "raw", "using"])
post_save = Signal(providing_args=["instance", "raw", "created", "using"])
pre_delete = Signal(providing_args=["instance", "using"])
post_delete = Signal(providing_args=["instance", "using"])
post_syncdb = Signal(providing_args=["class", "app", "created_models", "verbosity", "interactive"])
m2m_changed = Signal(providing_args=["action", "instance", "reverse", "model", "pk_set", "using"])
```
他们中比如pre_init是在model实例之前会触发， post_init是在model实例化之后触发。 pre_save是在保存之前触发， post_save是保存之后触发。他们真正得实现机制看代码django/db/models/base.py处， 如下：
```python
class Model(object):
   ......
    def __init__(self, *args, **kwargs):
        signals.pre_init.send(sender=self.__class__, args=args, kwargs=kwargs)
       ......
        signals.post_init.send(sender=self.__class__, instance=self)
    
    def save_base(self, raw=False, cls=None, origin=None, force_insert=False,
            force_update=False, using=None):
       
        ......
        if origin and not meta.auto_created:
            signals.pre_save.send(sender=origin, instance=self, raw=raw, using=using)
       ......
        # Signal that the save is complete
        if origin and not meta.auto_created:
            signals.post_save.send(sender=origin, instance=self,
                created=(not record_exists), raw=raw, using=using)
```
so, that all
ps. 增加一个自定义signal得介绍
```python
import django.dispatch
pizza_done = django.dispatch.Signal(providing_args=["toppings", "size"])
class PizzaStore(object):
    ...
    def send_pizza(self, toppings, size):
        pizza_done.send(sender=self, toppings=toppings, size=size)
        ...
def recv(sender, **kwargs):
    pass
pizza_done.connect(recv)
```
