# 漫谈 Clustering (3): Gaussian Mixture Model高斯混合模型GMM - 李鑫o_O - CSDN博客





2016年05月13日 13:02:36[hustlx](https://me.csdn.net/HUSTLX)阅读数：1436







[漫谈 Clustering (3): Gaussian Mixture Model](http://blog.pluskid.org/?p=39)

[上一次](http://blog.pluskid.org/?p=17)我们谈到了用 k-means 进行聚类的方法，这次我们来说一下另一个很流行的算法：Gaussian Mixture Model (GMM)。事实上，GMM 和 k-means 很像，不过 GMM
 是学习出一些概率密度函数来（所以 GMM 除了用在 clustering 上之外，还经常被用于 density estimation ），简单地说，k-means 的结果是每个数据点被 assign 到其中某一个 cluster 了，而 GMM 则给出这些数据点*被 assign 到每个 cluster 的概率*，又称作 *soft assignment* 。

得出一个概率有很多好处，因为它的信息量比简单的一个结果要多，比如，我可以把这个概率转换为一个 score ，表示算法对自己得出的这个结果的把握。也许我可以对同一个任务，用多个方法得到结果，最后选取“把握”最大的那个结果；另一个很常见的方法是在诸如疾病诊断之类的场所，机器对于那些很容易分辨的情况（患病或者不患病的概率很高）可以自动区分，而对于那种很难分辨的情况，比如，49% 的概率患病，51% 的概率正常，如果仅仅简单地使用 50% 的阈值将患者诊断为“正常”的话，风险是非常大的，因此，在机器对自己的结果把握很小的情况下，会“拒绝发表评论”，而把这个任务留给有经验的医生去解决。

废话说了一堆，不过，在回到 GMM 之前，我们再稍微扯几句。我们知道，不管是机器还是人，学习的过程都可以看作是一种“归纳”的过程，在归纳的时候你需要有一些假设的前提条件，例如，当你被告知水里游的那个家伙是鱼之后，你使用“在同样的地方生活的是同一种东西”这类似的假设，归纳出“在水里游的都是鱼”这样一个结论。当然这个过程是完全“本能”的，如果不仔细去想，你也不会了解自己是怎样“认识鱼”的。另一个值得注意的地方是这样的假设并不总是完全正确的，甚至可以说总是会有这样那样的缺陷的，因此你有可能会把虾、龟、甚至是潜水员当做鱼。也许你觉得可以通过修改前提假设来解决这个问题，例如，基于“生活在同样的地方并且穿着同样衣服的是同一种东西”这个假设，你得出结论：在水里有并且身上长有鳞片的是鱼。可是这样还是有问题，因为有些没有长鳞片的鱼现在又被你排除在外了。

在这个问题上，机器学习面临着和人一样的问题，在机器学习中，一个学习算法也会有一个前提假设，这里被称作“[归纳偏执 (bias)](http://citeseerx.ist.psu.edu/viewdoc/summary?doi=10.1.1.19.5466)”（bias
 这个英文词在机器学习和统计里还有其他许多的意思）。例如线性回归，目的是要找一个函数尽可能好地拟合给定的数据点，它的归纳偏执就是“满足要求的函数必须是线性函数”。一个没有归纳偏执的学习算法从某种意义上来说毫无用处，就像一个完全没有归纳能力的人一样，在第一次看到鱼的时候有人告诉他那是鱼，下次看到另一条鱼了，他并不知道那也是鱼，因为两条鱼总有一些地方不一样的，或者就算是同一条鱼，在河里不同的地方看到，或者只是看到的时间不一样，也会被他认为是不同的，因为他无法归纳，无法提取主要矛盾、忽略次要因素，只好要求所有的条件都完全一样──然而哲学家已经告诉过我们了：世界上不会有任何样东西是完全一样的，所以这个人即使是有无比强悍的记忆力，也绝学不到任何一点**知识**。

这个问题在机器学习中称作“[过拟合 (Overfitting)](http://en.wikipedia.org/wiki/Overfitting_(machine_learning))”，例如前面的回归的问题，如果去掉“线性函数”这个归纳偏执，因为对于 N 个点，我们总是可以构造一个
 N-1 次多项式函数，让它完美地穿过所有的这 N 个点，或者如果我用任何大于 N-1 次的多项式函数的话，我甚至可以构造出无穷多个满足条件的函数出来。如果假定特定领域里的问题所给定的数据个数总是有个上限的话，我可以取一个足够大的 N ，从而得到一个（或者无穷多个）“超级函数”，能够 fit 这个领域内所有的问题。然而这个（或者这无穷多个）“超级函数”有用吗？只要我们注意到*学习*的目的（通常）不是解释现有的事物，而是从中*归纳*出*知识*，并能应用到*新的*事物上，结果就显而易见了。

没有归纳偏执或者归纳偏执太宽泛会导致 Overfitting ，然而另一个极端──限制过大的归纳偏执也是有问题的：如果数据本身并不是线性的，强行用线性函数去做回归通常并不能得到好结果。难点正在于在这之间寻找一个平衡点。不过人在这里相对于（现在的）机器来说有一个很大的优势：人通常不会孤立地用某一个独立的系统和模型去处理问题，一个人每天都会从各个来源获取大量的信息，并且通过各种手段进行整合处理，归纳所得的所有知识最终得以*统一*地存储起来，并能*有机*地组合起来去解决特定的问题。这里的“有机”这个词很有意思，搞理论的人总能提出各种各样的模型，并且这些模型都有严格的理论基础保证能达到期望的目的，然而绝大多数模型都会有那么一些“参数”（例如
 K-means 中的 k ），通常没有理论来说明参数取哪个值更好，而模型实际的效果却通常和参数是否取到最优值有很大的关系，我觉得，在这里“有机”不妨看作是所有模型的参数已经自动地取到了最优值。另外，虽然进展不大，但是人们也一直都期望在计算机领域也建立起一个统一的知识系统（例如[语意网](http://en.wikipedia.org/wiki/Semantic_Web)就是这样一个尝试）。

废话终于说完了，回到 GMM 。按照我们前面的讨论，作为一个流行的算法，GMM 肯定有它自己的一个相当体面的归纳偏执了。其实它的假设非常简单，顾名思义，Gaussian Mixture Model ，就是假设数据服从 Mixture Gaussian Distribution ，换句话说，数据可以看作是从数个 Gaussian Distribution 中生成出来的。实际上，我们在 [K-means](http://blog.pluskid.org/?p=17) 和 [K-medoids](http://blog.pluskid.org/?p=40) 两篇文章中用到的那个例子就是由三个
 Gaussian 分布从随机选取出来的。实际上，从[中心极限定理](http://en.wikipedia.org/wiki/Central_limit_theorem)可以看出，Gaussian 分布（也叫做正态 (Normal) 分布）这个假设其实是比较合理的，除此之外，Gaussian 分布在计算上也有一些很好的性质，所以，虽然我们可以用不同的分布来随意地构造
 XX Mixture Model ，但是还是 GMM 最为流行。另外，Mixture Model 本身其实也是可以变得任意复杂的，通过增加 Model 的个数，我们可以任意地逼近任何连续的概率密分布。

每个 GMM 由 ![K](http://blog.pluskid.org/latexrender/pictures/a5f3c6a11b03839d46af9fb43c97c188.png) 个 Gaussian 分布组成，每个 Gaussian 称为一个“Component”，这些 Component 线性加成在一起就组成了 GMM
 的概率密度函数：

由于在对数函数里面又有加和，我们没法直接用求导解方程的办法直接求得最大值。为了解决这个问题，我们采取之前从 GMM 中随机选点的办法：分成两步，实际上也就类似于 [K-means](http://blog.pluskid.org/?p=17) 的两步。
- 估计数据由每个 Component 生成的概率（并不是每个 Component 被选中的概率）：对于每个数据 ![x_i](http://blog.pluskid.org/latexrender/pictures/1ba8aaab47179b3d3e24b0ccea9f4e30.png) 来说，它由第 ![k](http://blog.pluskid.org/latexrender/pictures/8ce4b16b22b58894aa86c421e8759df3.png) 个
 Component 生成的概率为
其中 ![N_k = \sum_{i=1}^N \gamma(i, k)](http://blog.pluskid.org/latexrender/pictures/c5fe9d4e6d474cc30475519bcdb6e38c.png) ，并且 ![\pi_k](http://blog.pluskid.org/latexrender/pictures/18db600e9b6993dd9ec8642eb24695dd.png) 也顺理成章地可以估计为 ![N_k/N](http://blog.pluskid.org/latexrender/pictures/3f7d6c033eb91dbc9164fc35d2bf4688.png) 。

- 重复迭代前面两步，直到似然函数的值收敛为止。

当然，上面给出的只是比较“直观”的解释，想看严格的推到过程的话，可以参考 *Pattern Recognition and Machine Learning* 这本书的第九章。有了实际的步骤，再实现起来就很简单了。Matlab 代码如下：

（Update 2012.07.03：如果你直接把下面的代码拿去运行了，碰到 covariance 矩阵 singular 的情况，可以参见[这篇文章](http://freemind.pluskid.org/machine-learning/regularized-gaussian-covariance-estimation/)。）



```cpp
function varargout = gmm(X, K_or_centroids)
% ============================================================
% Expectation-Maximization iteration implementation of
% Gaussian Mixture Model.
%
% PX = GMM(X, K_OR_CENTROIDS)
% [PX MODEL] = GMM(X, K_OR_CENTROIDS)
%
%  - X: N-by-D data matrix.
%  - K_OR_CENTROIDS: either K indicating the number of
%       components or a K-by-D matrix indicating the
%       choosing of the initial K centroids.
%
%  - PX: N-by-K matrix indicating the probability of each
%       component generating each point.
%  - MODEL: a structure containing the parameters for a GMM:
%       MODEL.Miu: a K-by-D matrix.
%       MODEL.Sigma: a D-by-D-by-K matrix.
%       MODEL.Pi: a 1-by-K vector.
% ============================================================
 
    threshold = 1e-15;
    [N, D] = size(X);
 
    if isscalar(K_or_centroids)
        K = K_or_centroids;
        % randomly pick centroids
        rndp = randperm(N);
        centroids = X(rndp(1:K), :);
    else
        K = size(K_or_centroids, 1);
        centroids = K_or_centroids;
    end
 
    % initial values
    [pMiu pPi pSigma] = init_params();
 
    Lprev = -inf;
    while true
        Px = calc_prob();
 
        % new value for pGamma
        pGamma = Px .* repmat(pPi, N, 1);
        pGamma = pGamma ./ repmat(sum(pGamma, 2), 1, K);
 
        % new value for parameters of each Component
        Nk = sum(pGamma, 1);
        pMiu = diag(1./Nk) * pGamma' * X;
        pPi = Nk/N;
        for kk = 1:K
            Xshift = X-repmat(pMiu(kk, :), N, 1);
            pSigma(:, :, kk) = (Xshift' * ...
                (diag(pGamma(:, kk)) * Xshift)) / Nk(kk);
        end
 
        % check for convergence
        L = sum(log(Px*pPi'));
        if L-Lprev < threshold
            break;
        end
        Lprev = L;
    end
 
    if nargout == 1
        varargout = {Px};
    else
        model = [];
        model.Miu = pMiu;
        model.Sigma = pSigma;
        model.Pi = pPi;
        varargout = {Px, model};
    end
 
    function [pMiu pPi pSigma] = init_params()
        pMiu = centroids;
        pPi = zeros(1, K);
        pSigma = zeros(D, D, K);
 
        % hard assign x to each centroids
        distmat = repmat(sum(X.*X, 2), 1, K) + ...
            repmat(sum(pMiu.*pMiu, 2)', N, 1) - ...
            2*X*pMiu';
        [dummy labels] = min(distmat, [], 2);
 
        for k=1:K
            Xk = X(labels == k, :);
            pPi(k) = size(Xk, 1)/N;
            pSigma(:, :, k) = cov(Xk);
        end
    end
 
    function Px = calc_prob()
        Px = zeros(N, K);
        for k = 1:K
            Xshift = X-repmat(pMiu(k, :), N, 1);
            inv_pSigma = inv(pSigma(:, :, k));
            tmp = sum((Xshift*inv_pSigma) .* Xshift, 2);
            coef = (2*pi)^(-D/2) * sqrt(det(inv_pSigma));
            Px(:, k) = coef * exp(-0.5*tmp);
        end
    end
end
```



函数返回的 `Px` 是一个 ![N\times K](http://blog.pluskid.org/latexrender/pictures/3b4896f081441931f409246050d9cd97.png) 的矩阵，对于每一个 ![x_i](http://blog.pluskid.org/latexrender/pictures/1ba8aaab47179b3d3e24b0ccea9f4e30.png) ，我们只要取该矩阵第 ![i](http://blog.pluskid.org/latexrender/pictures/865c0c0b4ab0e063e5caa3387c1a8741.png) 行中最大的那个概率值所对应的那个
 Component 为 ![x_i](http://blog.pluskid.org/latexrender/pictures/1ba8aaab47179b3d3e24b0ccea9f4e30.png) 所属的 cluster 就可以实现一个完整的聚类方法了。对于最开始的那个例子，GMM 给出的结果如下：

![gmm](http://blog.pluskid.org/wp-content/uploads/2009/02/gmm.png)

相对于[之前 K-means 给出的结果](http://blog.pluskid.org/?p=17)，这里的结果更好一些，左下角的比较稀疏的那个 cluster 有一些点跑得比较远了。当然，因为这个问题原本就是完全有 Mixture Gaussian Distribution
 生成的数据，GMM （如果能求得全局最优解的话）显然是可以对这个问题做到的最好的建模。

另外，从上面的分析中我们可以看到 GMM 和 K-means 的迭代求解法其实非常相似（都可以追溯到 [EM 算法](http://en.wikipedia.org/wiki/Expectation-maximization_algorithm)，下一次会详细介绍），因此也有和
 K-means 同样的问题──并不能保证总是能取到全局最优，如果运气比较差，取到不好的初始值，就有可能得到很差的结果。对于 K-means 的情况，我们通常是重复一定次数然后取最好的结果，不过 GMM 每一次迭代的计算量比 K-means 要大许多，一个更流行的做法是先用 K-means （已经重复并取最优值了）得到一个粗略的结果，然后将其作为初值（只要将 K-means 所得的 centroids 传入 `gmm` 函数即可），再用
 GMM 进行细致迭代。

如我们最开始所讨论的，GMM 所得的结果（`Px`）不仅仅是数据点的 label ，而包含了数据点标记为每个 label 的概率，很多时候这实际上是非常有用的信息。最后，需要指出的是，GMM 本身只是一个模型，我们这里给出的迭代的办法并不是唯一的求解方法。感兴趣的同学可以自行查找相关资料。



