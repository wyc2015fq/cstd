# matlab pca解释 - qq229873466的博客 - CSDN博客

2017年05月06日 11:05:45[qq229873466](https://me.csdn.net/qq229873466)阅读数：207


[coeff,score,latent] = pca(x); % x为原始数据，行数为样本数，列数为特征数

x_bar = x - repmat(mean(x), size(x,1), 1); % 均值化

[u,s,d] = svd(cov(x)); % u === coeff, diag(s) === latent

xRot = x_bar * u; % xRot ===  score

PCA降维：

[coeff,score,latent] = pca(x);

x_pca = score(:, 1:20); % 降维后的数据，剩下20维

x_pca = x_bar * u(:, 1:20);

