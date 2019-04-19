# django connection router - Arthur的随笔 - CSDN博客
2011年11月29日 22:31:05[largetalk](https://me.csdn.net/largetalk)阅读数：1985
在多库情况下，django使用router指定sql将在哪个库执行，这也可以被用来做读写分离。
在django/db/__init__.py中有
```python
router = ConnectionRouter(settings.DATABASE_ROUTERS)
```
ConnectionRouter定义在django/db/utils.py中：
```python
class ConnectionRouter(object):
    def __init__(self, routers):
        self.routers = []
        for r in routers:
            #load and import class
            self.routers.append(router)
    def _router_func(action):
        def _route_db(self, model, **hints):
            chosen_db = None
            for router in self.routers:
                try:
                    method = getattr(router, action)
                except AttributeError:
                    # If the router doesn't have a method, skip to the next one.
                    pass
                else:
                    chosen_db = method(model, **hints)
                    if chosen_db:
                        return chosen_db
            try:
                return hints['instance']._state.db or DEFAULT_DB_ALIAS
            except KeyError:
                return DEFAULT_DB_ALIAS
        return _route_db
    db_for_read = _router_func('db_for_read')
    db_for_write = _router_func('db_for_write')
    def allow_relation(self, obj1, obj2, **hints):
        for router in self.routers:
            try:
                method = router.allow_relation
            except AttributeError:
                # If the router doesn't have a method, skip to the next one.
                pass
            else:
                allow = method(obj1, obj2, **hints)
                if allow is not None:
                    return allow
        return obj1._state.db == obj2._state.db
    def allow_syncdb(self, db, model):
        for router in self.routers:
            try:
                method = router.allow_syncdb
            except AttributeError:
                # If the router doesn't have a method, skip to the next one.
                pass
            else:
                allow = method(db, model)
                if allow is not None:
                    return allow
        return True
```
所以要实现分数据库，只需要一个实现db_for_read, db_for_write等函数的类就可以了
django源码中没有给出实现得例子，但文档中有两个例子： [https://docs.djangoproject.com/en/1.3/topics/db/multi-db/](https://docs.djangoproject.com/en/1.3/topics/db/multi-db/)
