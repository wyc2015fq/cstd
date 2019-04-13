
# Keras处理已保存模型中的自定义层（或其他自定义对象） - AYE89 - 博客园|[AYE89](https://www.cnblogs.com/eniac1946/)
|coding & learning|
|posts - 153, comments - 3, trackbacks - 0, articles - 2

|
|导航
|[博客园](https://www.cnblogs.com/)|[首页](https://www.cnblogs.com/eniac1946/)|[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)|[联系](https://msg.cnblogs.com/send/AYE89)![订阅](//www.cnblogs.com/images/xml.gif)|[订阅](https://www.cnblogs.com/eniac1946/rss)|[管理](https://i.cnblogs.com/)
|公告


|[Keras处理已保存模型中的自定义层（或其他自定义对象）](https://www.cnblogs.com/eniac1946/p/8854434.html)
|Posted on|2018-04-16 10:26|[AYE89](https://www.cnblogs.com/eniac1946/)|阅读(|...|) 评论(|...|)|[编辑](https://i.cnblogs.com/EditPosts.aspx?postid=8854434)|[收藏](#)
|如果要加载的模型包含自定义层或其他自定义类或函数，则可以通过|custom_objects|参数将它们传递给加载机制：
|from keras.models|import load_model|\# 假设你的模型包含一个 AttentionLayer 类的实例
model = load_model(|'my_model.h5', custom_objects={|'AttentionLayer': AttentionLayer})|或者，你可以使用|[自定义对象作用域](https://keras.io/zh/utils/#customobjectscope)|：
|from keras.utils|import CustomObjectScope|with CustomObjectScope({|'AttentionLayer': AttentionLayer}):
    model = load_model(|'my_model.h5')|自定义对象的处理与|load_model|,|model_from_json|,|model_from_yaml|的工作方式相同：
|from keras.models|import model_from_json
model = model_from_json(json_string, custom_objects={|'AttentionLayer': AttentionLayer})







|[刷新评论](javascript:void(0);)|[刷新页面](#)|[返回顶部](#top)






|
|Powered by:
|博客园
|Copyright © AYE89
|
