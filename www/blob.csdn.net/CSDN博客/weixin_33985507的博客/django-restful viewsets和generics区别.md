# django-restful:viewsets和generics区别 - weixin_33985507的博客 - CSDN博客
2017年12月30日 11:39:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：4
viewsets 和generics 都是对下层的APIView和mixin进行组合
mixin包含的类
```
CreateModelMixin  #新建一个信息
    ListModelMixin     #一个列表信息
    RetrieveModelMixin  #单独一个信息
    UpdateModelMixin  #更新一个信息
    DestroyModelMixin
```
首先是generices包含
```
GenericViewSet(viewsets)         -drf
    GenericAPIView               -drf
        APIView                  -drf
            View                 -django
GenericAPIView(views.APIView)  #对APIView进行封装
CreateAPIView(mixins.CreateModelMixin,            #组合一个create
                    GenericAPIView)
ListAPIView(mixins.ListModelMixin,
                  GenericAPIView)
RetrieveAPIView(mixins.RetrieveModelMixin,
                      GenericAPIView)
DestroyAPIView(mixins.DestroyModelMixin,
                     GenericAPIView)
UpdateAPIView(mixins.UpdateModelMixin,
                    GenericAPIView)
ListCreateAPIView(mixins.ListModelMixin,
                        mixins.CreateModelMixin,
                        GenericAPIView)
RetrieveUpdateAPIView(mixins.RetrieveModelMixin,
                            mixins.UpdateModelMixin,
                            GenericAPIView)
```
这样的组合  可以实现不同的功能  但是也有一个麻烦的地方
以 ListAPIView(mixins.ListModelMixin,GenericAPIView) 为例
```
class ListAPIView(mixins.ListModelMixin,
                  GenericAPIView):
    """
    Concrete view for listing a queryset.
    """
    def get(self, request, *args, **kwargs):
        return self.list(request, *args, **kwargs)
```
他需要写一个get 方法 与list进行绑定  所以有点麻烦
但是ViewSets就可以省略这一步
Viewsets中包含的方法
```
ViewSetMixin(object)
ViewSet(ViewSetMixin, views.APIView)
GenericViewSet(ViewSetMixin, generics.GenericAPIView)
ReadOnlyModelViewSet(mixins.RetrieveModelMixin,
                           mixins.ListModelMixin,
                           GenericViewSet)
ModelViewSet(mixins.CreateModelMixin,
                   mixins.RetrieveModelMixin,
                   mixins.UpdateModelMixin,
                   mixins.DestroyModelMixin,
                   mixins.ListModelMixin,
                   GenericViewSet)
```
ViewSets封装了两个类  ViewSetMixin(object) 和ViewSet(ViewSetMixin, views.APIView)
然后进行组合 就可以实现里面的方法  就不用在view中绑定HTTP和他们的关系了
而是在urls配置中进行绑定
有两种方法
第一就是
```
# 商品列表页
goods_list = GoodsListViewSet.as_view({
    # 绑定关系  还可以更简单
    'get': 'list',
})
urlpatterns = [
  url(r'^goods/', goods_list,name='goods_list),
]
```
第二种就更简单了
```
from rest_framework.routers import DefaultRouter
from goods.views import GoodsListViewSet
router = DefaultRouter()
# 配置goods的url
router.register(r'goods', GoodsListViewSet)
#以后再注册进再写一个router就行了
```
同时viewsets 还提供了一些action给我们 更加方便
