function L_ind = SelectFeatures(h_candidateWeakClf,NineBlocks_MFeatures,tempnum_neg,tempnum_pos,K,M)
%函数功能：从M个候选特征(中心)中选择出一些最佳的,函数返回的是索引值，实际选择出来的特征中心是在函数外计算的
%参数：
%h_candidateWeakClf为所有样本所有特征的弱分类器输出
%NineBlocks_MFeatures存储每个子图像中的特征中心索引
%tempnum_neg和tempnum_pos为负样本和正样本数
%函数返回L_ind，为选择出来的所有特征(中心)索引

h_candidateWeakClf = h_candidateWeakClf/max(max(h_candidateWeakClf));%归一化，防止h_candidateWeakClf中元素过大时，使1/(1+exp(-x))为0
Label_y = zeros(tempnum_neg + 1,1);
Label_y(end,1) = 1;%负样本包标记为0,正样本包标记为1


KM_Rate = K/M;
L_ind = [];
for numB = 1:size(NineBlocks_MFeatures,1)%遍历子图像中的每一个
    one_ninthB_Features = NineBlocks_MFeatures{numB,1};%子图像中的特征中心
    subM       = size(one_ninthB_Features,2);%子图像中的特征中心的个数
    L = zeros(subM ,1);%subM个错误率(损失函数)
    for ind_subM = 1:subM
        subm =  one_ninthB_Features(ind_subM);%subm为子图像中的特征(中心)的索引，这个索引是相对于整个图像的所有特征(中心)的索引而言

        P_main     = zeros(tempnum_neg + 1,1);%tempnum_neg + 1为总包数，每一个负样本单独做为一个包，所有正样本做为一个包
        for i = 1:tempnum_neg%每一个负样本单独做为一个包
            temp_neg_p      = 1/(1 + exp(-h_candidateWeakClf(i,subm)));
            P_main(i)       = temp_neg_p;
        end
        temp_pos_p = zeros(tempnum_pos,1);
        temp_multi = 1;
        for j = 1:tempnum_pos%所有正样本做为一个包
            temp_pos_p(j,1) = 1/(1 + exp(-h_candidateWeakClf(tempnum_neg+j,subm)));
            temp_multi = temp_multi * (1 - temp_pos_p(j,1));
        end
        P_main(end,1) = 1 - temp_multi;
        L(ind_subM,1) = abs(sum(Label_y.*log(P_main + 0.000000001) + (1-Label_y).*log(1-P_main + 0.000000001),1));

    end
    [L_sort,L_ind_sub] = sort(L);
    selectNumBlock =floor(KM_Rate*subM);%从每个子图像中的所有已标记的特征中心中选择出selectNumBlock个
    if selectNumBlock>=1
        for temp_ind = 1:selectNumBlock
            L_ind = [L_ind;one_ninthB_Features(L_ind_sub(temp_ind))];%选择出来的所有特征(中心)索引
        end
    else
        continue;
    end
end



