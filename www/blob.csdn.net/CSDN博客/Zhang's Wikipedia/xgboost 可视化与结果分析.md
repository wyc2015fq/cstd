
# xgboost 可视化与结果分析 - Zhang's Wikipedia - CSDN博客


2019年04月04日 00:42:56[Inside_Zhang](https://me.csdn.net/lanchunhui)阅读数：47个人分类：[numpy-scipy-pandas-sklearn-xgb](https://blog.csdn.net/lanchunhui/article/category/8030594)



xgboost 的可视化，调用 xgb.to_graphviz 或 xgb.plot_tree 接口；
## 1. 指定 fmap（feature map）
[https://www.kaggle.com/mmueller/xgb-feature-importance-python](https://www.kaggle.com/mmueller/xgb-feature-importance-python)
按照 feature map 的格式，编写 fmap 文件：
def ceate_feature_map(features, fmap_filename):
    outfile = open(fmap_filename, 'w')
    i = 0
    for feat in features:
        outfile.write('{0}\t{1}\tq\n'.format(i, feat))
        i = i + 1
    outfile.close()
\# 调用
fmap_filename = 'xgb.fmap'
create_feature_map(X_train.columns, fmap_filename)
将 xgb.fmap 文件传入 xgb.plot_tree 文件中：
xgb.plot_tree(bst, fmap=fmap_filename)
指定图像的大小：
fig = ptl.gcf()
fig.set_size_inches(150, 100)
## 2. 结果分析
bst.get_dump()
查看各棵树的结构；
bst.predict 的参数：
pred_leaf=True，各棵树对各个样本的预测的叶子；


