# matlab 矩阵的Z字形扫描 - 家家的专栏 - CSDN博客





2011年07月11日 11:08:20[依海之燕](https://me.csdn.net/yihaizhiyan)阅读数：3712








 A=[1 2 3 4; 5 6 7 8; 9 10 11 12; 13 14 15 16]
    y_img_index=1;    
    for dim_sum=2:1:8
        if(mod(dim_sum,2)==0)
            for i=1:1:4
                if dim_sum-i<=4 & dim_sum-i>0
                    imgtemp(y_img_index) = A(i,dim_sum-i)
                    y_img_index=y_img_index+1;
                end
            end
        else
            for i=1:1:4
                if dim_sum-i<=4 & dim_sum-i>0
                    imgtemp(y_img_index) = A(dim_sum-i,i)
                    y_img_index=y_img_index+1;
                end                   
            end   
        end
    end

 输入：

A =

     1     2     3     4
     5     6     7     8
     9    10    11    12
    13    14    15    16

输出：

imgtemp =     1     5     2     3     6     9    13    10     7     4     8    11    14    15    12    16





