# 关于MCMC方法在贝叶斯结构学习中的一些理解 - ibelieve8013的博客 - CSDN博客





2018年03月29日 22:22:51[ibelieve8013](https://me.csdn.net/ibelieve8013)阅读数：1615








MCMC方法的关键是通过构造平稳分布为P的马尔科夫链来产生样本。在贝叶斯网络中，产生的样本就是各个贝叶斯结构，通过产生的样本（这些结构）中，选取可行的结构。关键的部分就在于如何仅仅通过训练集（每个节点一系列状态），来得到采样样本（最终结构）。

       MH算法是MCMC算法的重要代表，MH是通过上一轮采样结果来采样，获得候选样本，但是这个样本有可能被拒绝掉，为什么会被拒绝掉呢？因为算法中规定了一个U是在0-1范围内的随机数，如果接受概率大于它则接受。最终，，，（

这个拒绝的步骤，在我理解看来，是为了达到平稳分布的一个构造过程。由于在x*最终达到平稳状态时：细致平稳的要求是

      （1）

即达到这样的效果：

![](https://img-blog.csdn.net/20180329221903730?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2liZWxpZXZlODAxMw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

P转移到q和q转移到p的量是一样的，那么这个马尔科夫链是平稳的。

那么，由于一般的马尔科夫链不是平稳的，为了构造一个平稳的马尔科夫链，需要引入和一个接受概率。

如下所说：构造了一个接受率，那么马尔科夫链达到了平稳状态。

![](https://img-blog.csdn.net/20180329221932258?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2liZWxpZXZlODAxMw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)




但是接受率的写法为甚是

![](https://img-blog.csdn.net/20180329221947154?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2liZWxpZXZlODAxMw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

其实这也很简单：

![](https://img-blog.csdn.net/20180329221955682?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2liZWxpZXZlODAxMw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)




这个算法被应用在了贝叶斯结构学习中，通过贝叶斯工具箱，其中有一个learn_struct_mcmc.m函数，输入数据为训练集，每个节点可取的状态值，输出为采样的结构，接受率以及第t次迭代的边数。代码如下：
|function [sampled_graphs, accept_ratio, num_edges] =  learn_struct_mcmc(data, ns, varargin)%  MY_LEARN_STRUCT_MCMC  Monte Carlo  Markov Chain search over DAGs assuming fully observed data%  [sampled_graphs, accept_ratio, num_edges] = learn_struct_mcmc(data, ns, ...)% % data(i,m)  is the value of node i in case m.% ns(i) is  the number of discrete values node i can take on.%%  sampled_graphs{m} is the m'th sampled graph.%  accept_ratio(t) = acceptance ratio at iteration t%  num_edges(t) = number of edges in model at iteration t%% The  following optional arguments can be specified in the form of name/value  pairs:% [default  value in brackets]%% scoring_fn  - 'bayesian' or 'bic' [ 'bayesian' ]%              Currently, only networks with  all tabular nodes support Bayesian scoring.% type       - type{i} is the type of CPD to use  for node i, where the type is a string%              of the form 'tabular',  'noisy_or', 'gaussian', etc. [ all cells contain 'tabular' ]% params     - params{i} contains optional arguments  passed to the CPD constructor for node i,%              or [] if none.  [ all cells contain {'prior', 1}, meaning  use uniform Dirichlet priors ]%  discrete   - the list of discrete nodes  [ 1:N ]%  clamped    - clamped(i,m) = 1 if node i  is clamped in case m [ zeros(N, ncases) ]%  nsamples   - number of samples to draw  from the chain after burn-in [ 100*N ]% burnin     - number of steps to take before drawing  samples [ 5*N ]%  init_dag   - starting point for the  search [ zeros(N,N) ]%% e.g.,  samples = my_learn_struct_mcmc(data, ns, 'nsamples', 1000);%% Modified by  Sonia Leach (SML) 2/4/02, 9/5/03[n ncases] =  size(data);%n是节点数，ncase是样本数% set default  paramstype = cell(1,n);params = cell(1,n);%定义类型和参数for i=1:n type{i} = 'tabular';%params{i} =  { 'prior', 1}; params{i} = { 'prior_type', 'dirichlet', 'dirichlet_weight', 1 };endscoring_fn = 'bayesian';discrete = 1:n;clamped = zeros(n,  ncases);%定义一个和训练集一样大的零矩阵nsamples = 100*n;burnin = 5*n;dag = zeros(n);args = varargin;%arg是可变参数列表nargs =  length(args);for i=1:2:nargsswitch args{i},case'nsamples',   nsamples =  args{i+1};case'burnin',     burnin =  args{i+1};case'init_dag',   dag =  args{i+1};case'scoring_fn', scoring_fn = args{i+1};case'type',       type =  args{i+1}; case'discrete',   discrete =  args{i+1}; case'clamped',    clamped =  args{i+1}; case'params',     if  isempty(args{i+1}), params = cell(1,n); else params = args{i+1};   endendend% We  implement the fast acyclicity check described by P. Giudici and R. Castelo,%  "Improving MCMC model search for data mining", submitted to J.  Machine Learning, 2001.% SML: also  keep descendant matrix Cuse_giudici = 1;if use_giudici [nbrs, ops, nodes, A] =  mk_nbrs_of_digraph(dag);else [nbrs, ops, nodes] = mk_nbrs_of_dag(dag); A = [];endnum_accepts = 1;num_rejects = 1;T = burnin +  nsamples;accept_ratio =  zeros(1, T);%定义接受率矩阵num_edges =  zeros(1, T);%边数目的矩阵sampled_graphs =  cell(1, nsamples);%采样图%sampled_bitv  = zeros(nsamples, n^2);for t=1:T   %对总共的点数，进行take_step操作，得到accept [dag, nbrs, ops, nodes, A, accept] =  take_step(dag, nbrs, ops, ...                    nodes, ns, data, clamped,  A, ...                      scoring_fn, discrete,  type, params); num_edges(t) = sum(dag(:)); num_accepts = num_accepts + accept;%接受数累加 num_rejects = num_rejects + (1-accept);%拒绝数累加 accept_ratio(t) =  num_accepts/num_rejects;%重复更新接受率if t > burnin%如t超出了舍弃范围   sampled_graphs{t-burnin} = dag;%把图放进样本图中去。%sampled_bitv(t-burnin,  :) = dag(:)';endend%%%%%%%%%function [new_dag, new_nbrs, new_ops, new_nodes, A,  accept] = ...   take_step(dag, nbrs, ops, nodes, ns, data,  clamped, A,  ...     scoring_fn, discrete, type, params,  prior_w)use_giudici =  ~isempty(A);if use_giudici %如果矩阵A是非空，更新A [new_dag, op, i, j, new_A] =  pick_digraph_nbr(dag, nbrs, ops, nodes,A); % updates A [new_nbrs, new_ops, new_nodes] =  mk_nbrs_of_digraph(new_dag, new_A);else d = sample_discrete(normalise(ones(1,  length(nbrs)))); new_dag = nbrs{d}; op = ops{d}; i = nodes(d, 1); j = nodes(d, 2); [new_nbrs, new_ops, new_nodes] = mk_nbrs_of_dag(new_dag);endbf =  bayes_factor(dag, new_dag, op, i, j, ns,  data, clamped, scoring_fn, discrete, type, params);%bf是一个什么值？%R = bf *  (new_prior / prior) * (length(nbrs) / length(new_nbrs)); R = bf *  (length(nbrs) / length(new_nbrs)); u = rand(1,1);if u > min(1,R) %  reject the move 拒绝采样 accept = 0; new_dag = dag; new_nbrs = nbrs; new_ops = ops; new_nodes = nodes;else accept = 1;%接受采样的话，对A进行更新if use_giudiciA = new_A; % new_A already updated in pick_digraph_nbrendend%%%%%%%%%function bfactor = bayes_factor(old_dag, new_dag, op, i, j, ns,  data, clamped, scoring_fn, discrete, type, params)u =  find(clamped(j,:)==0);LLnew =  score_family(j, parents(new_dag, j), type{j}, scoring_fn, ns, discrete,  data(:,u), params{j});LLold = score_family(j,  parents(old_dag, j), type{j}, scoring_fn, ns, discrete, data(:,u),  params{j});bf1 = exp(LLnew -  LLold);%新得分-旧得分取指数if strcmp(op, 'rev')  % must also multiply in the changes to i's family u = find(clamped(i,:)==0); LLnew = score_family(i, parents(new_dag, i),  type{i}, scoring_fn, ns, discrete, data(:,u), params{i}); LLold = score_family(i, parents(old_dag, i),  type{i}, scoring_fn, ns, discrete, data(:,u), params{i}); bf2 = exp(LLnew - LLold);else bf2 = 1;endbfactor = bf1 *  bf2;%%%%%%%%  Giudici stuff follows %%%%%%%%%%% SML: This  now updates A as it goes from digraph it chosesfunction [new_dag, op, i, j, new_A] = pick_digraph_nbr(dag,  digraph_nbrs, ops, nodes, A)d =  sample_discrete(normalise(ones(1, length(digraph_nbrs))));%d =  myunidrnd(length(digraph_nbrs),1,1);i = nodes(d, 1); j  = nodes(d, 2);new_dag =  digraph_nbrs(:,:,d);op = ops{d};new_A =  update_ancestor_matrix(A, op, i, j, new_dag); %%%%%%%%%%%%%%% 这是对结构的三种操作：function A = update_ancestor_matrix(A,  op, i, j, dag)switch opcase'add', A = do_addition(A,  op, i, j, dag);case'del',  A = do_removal(A,  op, i, j, dag);case'rev',  A = do_removal(A,  op, i, j, dag); A = do_addition(A,  op, j, i, dag);end%%%%%%%%%%%%% 这是加边操作：function A = do_addition(A, op, i, j, dag)A(j,i) = 1; % i is an ancestor of janci =  find(A(i,:));if ~isempty(anci) A(j,anci) = 1; %  all of i's ancestors are added to Anc(j)endancj =  find(A(j,:));descj =  find(A(:,j)); if ~isempty(ancj)for k=descj(:)'   A(k,ancj) = 1; %  all of j's ancestors are added to each descendant of jendend%%%%%%%%%%%这是剪边操作function A = do_removal(A, op, i, j, dag)% find all  the descendants of j, and put them in topological order% SML:  originally Kevin had the next line commented and the %* lines% being used  but I think this is equivalent and much less expensive% I assume he  put it there for debugging and never changed it back...?descj =  find(A(:,j));%*  R = reachability_graph(dag);%*  descj = find(R(j,:));order =  topological_sort(dag);% SML:  originally Kevin used the %* line but this was extracting the% wrong  things to sort%*  descj_topnum = order(descj);[junk, perm] =  sort(order); %SML:node i is perm(i)-TH in orderdescj_topnum =  perm(descj); %SML:descj(i) is  descj_topnum(i)-th in order% SML: now  re-sort descj by rank in descj_topnum[junk, perm] =  sort(descj_topnum);descj =  descj(perm);% Update j  and all its descendantsA = update_row(A,  j, dag);for k = descj(:)'   A = update_row(A, k, dag);end%%%%%%%%%%%function A = old_do_removal(A, op, i, j, dag)% find all  the descendants of j, and put them in topological order% SML:  originally Kevin had the next line commented and the %* lines% being used  but I think this is equivalent and much less expensive% I assume he  put it there for debugging and never changed it back...?descj =  find(A(:,j)); %*  R = reachability_graph(dag);%*  descj = find(R(j,:)); order =  topological_sort(dag);descj_topnum =  order(descj);[junk, perm] =  sort(descj_topnum);descj =  descj(perm);% Update j  and all its descendantsA = update_row(A,  j, dag);for k = descj(:)' A = update_row(A, k, dag);end%%%%%%%%%升级function A = update_row(A, j, dag)% We compute  row j of AA(j, :) = 0;ps = parents(dag,  j);if ~isempty(ps) A(j, ps) = 1;endfor k=ps(:)' anck = find(A(k,:));if ~isempty(anck)   A(j, anck) = 1;endend%%%%%%%%function A = init_ancestor_matrix(dag)order =  topological_sort(dag);A =  zeros(length(dag));for j=order(:)' A = update_row(A, j, dag);end|
|----|

![](https://img-blog.csdn.net/20180329222022878?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2liZWxpZXZlODAxMw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

总结一下，再说说我的疑惑，MH算法是用列举完全的方法把所有的边操作列举出来，然后再对这个集合进行采样吗？当节点数目多的时候怎么办呢？这是O（n^2）数量级的数目吧。那么是否有好的办法可以改进此抽样算法？

若用MCMC采样，需要对每一条边进行采样，当有N个节点，则有N*(N-1)/2条边需要被采样，每条边是3个值，分别代表顺、逆、无边。而组合出来的结构种类数目一共是（3^（n*(n-1)/2）），当然这里没有考虑去除环的结构。

在《基于MCMC贝叶斯网络学习算法》中提到，在构造马尔科夫链中，一个结构G和一个对G改变了一条边的集合nbr(G)，是马尔科夫链的两个状态。此文中相对于经典MH算法的改进在于：由于搜索空间巨大，先利用条件互信息，条件独立性测试进行了节点相互位置的固定，再对边进行添加，反转，（没有无边状态，由于独立性测试已经确定依赖关系）。最后此方法学习结果和原来的基本一致，接受率也一致，但是文中也并没有提到新算法是否节约了运算时间？优点没有实际的例子做支撑，私下觉得略有一点草率。另，这篇文章的目标是把此算法用在文档分类中，这个可以再深入研究一下，应该属于自然语言处理范围，也是概率图模型的一种主要应用场景了。





