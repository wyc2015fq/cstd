# SVM 自定义kernel的一个简单的code 例子 - 家家的专栏 - CSDN博客





2014年03月26日 16:36:36[依海之燕](https://me.csdn.net/yihaizhiyan)阅读数：1257








                
function [ result_pre, result_r, max_result ] = kernel_SVM( train_data,test_data,train_label, test_label)

%train_data  : m*m train-train kernel matrix

% test_data   n*m test-train kernel matrix

% train_label m*1 train label

% test_label  n*1 test label



r=1e-10;

for i=1:50

    train_data_exp =exp(-r*train_data);

    test_data_exp =exp(-r*test_data);

    train_data_exp = [(1:size(index_train,2))', train_data_exp];

    test_data_exp = [(1:size(index_test,2))', test_data_exp];

    model_precomputed = svmtrain(train_label, train_data_exp, '-s 1 -t 4 -n 0.02 -c 1.21 -g 1');

    [predict_label_P, accuracy_P, dec_values_P] = svmpredict(test_label, test_data_exp, model_precomputed);

    %          if (accuracy_P(1,1)>max(result_pre))

    result_pre(result_index)=accuracy_P(1,1);

    result_r(result_index)=r;

    result_index=result_index+1;

    %          end

    r=r*sqrt(10);

end

max_result = max(result_pre);



end




