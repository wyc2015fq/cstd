# 约束M是半正定的code - 家家的专栏 - CSDN博客





2013年12月24日 11:16:30[依海之燕](https://me.csdn.net/yihaizhiyan)阅读数：1146
个人分类：[数学 Matlab 基本知识](https://blog.csdn.net/yihaizhiyan/article/category/749753)









                
function posM=PosCone(M)

[V D]=eig(M);

D=max(D,0);

posM=V*D*inv(V);

end
            


