# matlab  集群并行计算~ - 家家的专栏 - CSDN博客





2012年12月20日 10:18:17[依海之燕](https://me.csdn.net/yihaizhiyan)阅读数：1887








开启matlab，然后启动并行。。。 

sched=findResource('scheduler','type','local');

sched.DataLocation = '/home/xuchunyan/buffer/blade05';

if matlabpool('size') <= 0

    matlabpool('open', 'local', 8);

end




code做相应改动。。。

for example：

计算kernel矩阵：

    dis=zeros(size(k, 2), size(k, 2));

    tempdis0 = zeros(1, size(k, 2));



    for i=1:size(k,2)

        feai = fea(i);

        parfor j=1:size(class,2)

            [tempdis0(j)] = computedis(feai, fea(j));

        end

        dis(i, :) = tempdis0;

        fprintf(1,'distance: i=%d \n',i);

    end




