# Eigenvalue Decomposition of Symmetric Matrices（convex） - 家家的专栏 - CSDN博客





2013年12月09日 15:15:12[依海之燕](https://me.csdn.net/yihaizhiyan)阅读数：1068








# Eigenvalue Decomposition of Symmetric Matrices






Symmetric matrices are square with elements that mirror each other across the diagonal. They can be used to describe for example graphs with undirected, weighted edges between the nodes; or distance matrices (between say cities), and a host of other applications.
 Symmetric matrices are also important in optimization, as they are closely related to quadratic functions.


A fundamental theorem, the ***spectral theorem***, shows that we can decompose any symmetric matrix as a three-term product of matrices, involving an***orthogonal***transformation and a ***diagonal ***matrix. The theorem has a direct implication for quadratic functions: it allows a to decompose any quadratic function into a weighted sum of squared linear functions involving vectors that
 are mutually orthogonal. The weights are called the *eigenvalues* of the symmetric matrix.


The spectral theorem allows in particular to determine when a given quadratic function is ‘‘bowl-shaped’’, that is,**convex**. The spectral theorem also allows to find directions of maximal variance within a data set. Such directions are useful
 to visualize high-dimensional data points in two or three dimensions. This is the basis of a visualization method known as *principal component analysis* (PCA).

From：[https://inst.eecs.berkeley.edu/~ee127a/book/login/l_sym_main.html](https://inst.eecs.berkeley.edu/~ee127a/book/login/l_sym_main.html)







## Spectral theorem


An important result of linear algebra, called the *spectral theorem*, or *symmetric eigenvalue decomposition* (SED) theorem, states that for any symmetric matrix, there are exactly ![n](https://inst.eecs.berkeley.edu/~ee127a/book/login/eqs/14080042351-130.png) (possibly
 not distinct) eigenvalues, and they are all real; further, that the associated eigenvectors can be chosen so as to form an orthonormal basis. The result offers a simple way to decompose the symmetric matrix as a product of simple transformations.



Theorem: Symmetric eigenvalue decomposition



We can decompose any symmetric matrix ![A in mathbf{S}^n](https://inst.eecs.berkeley.edu/~ee127a/book/login/eqs/2839406567159830507-130.png) with the *symmetric eigenvalue decomposition* (SED) 
![A = sum_{i=1}^n lambda_i u_iu_i^T  = U Lambda U^T, ;; Lambda = mbox{bf diag}(lambda_1,ldots,lambda_n) .](https://inst.eecs.berkeley.edu/~ee127a/book/login/eqs/7755477976312104855-130.png)

where the matrix of ![U := [u_1 , ldots, u_n]](https://inst.eecs.berkeley.edu/~ee127a/book/login/eqs/7374396354641771307-130.png) is orthogonal (that is, ![U^TU=UU^T = I_n](https://inst.eecs.berkeley.edu/~ee127a/book/login/eqs/5392293284070413097-130.png)),
 and contains the eigenvectors of ![A](https://inst.eecs.berkeley.edu/~ee127a/book/login/eqs/8320025024-130.png), while the diagonal matrix ![Lambda](https://inst.eecs.berkeley.edu/~ee127a/book/login/eqs/5252633898115416342-130.png) contains
 the eigenvalues of ![A](https://inst.eecs.berkeley.edu/~ee127a/book/login/eqs/8320025024-130.png).




Here is a [proof](https://inst.eecs.berkeley.edu/~ee127a/book/login/thm_sed.html). The SED provides a decomposition of the matrix in simple terms, namely [dyads](https://inst.eecs.berkeley.edu/~ee127a/book/login/l_mats_special.html).


We check that in the SED above, the scalars ![lambda_i](https://inst.eecs.berkeley.edu/~ee127a/book/login/eqs/2768874965527882936-130.png) are the eigenvalues,
 and ![u_i](https://inst.eecs.berkeley.edu/~ee127a/book/login/eqs/2621286447068506710-130.png)’s are associated eigenvectors, since 
![Au_j = sum_{i=1}^n lambda_i u_iu_i^Tu_j = lambda_j u_j, ;; j=1,ldots,n.](https://inst.eecs.berkeley.edu/~ee127a/book/login/eqs/4147924235102652252-130.png)



The eigenvalue decomposition of a symmetric matrix can be efficiently computed with standard software, in time that grows proportionately to its dimension ![n](https://inst.eecs.berkeley.edu/~ee127a/book/login/eqs/14080042351-130.png) as ![n^3](https://inst.eecs.berkeley.edu/~ee127a/book/login/eqs/5261102140378497932-130.png).
 Here is the matlab syntax, where the first line ensure that matlab knows that the matrix ![A](https://inst.eecs.berkeley.edu/~ee127a/book/login/eqs/8320025024-130.png) is
 exactly symmetric.

From：[https://inst.eecs.berkeley.edu/~ee127a/book/login/l_sym_sed.html](https://inst.eecs.berkeley.edu/~ee127a/book/login/l_sym_sed.html)

[https://inst.eecs.berkeley.edu/~ee127a/book/login/glossary.html](https://inst.eecs.berkeley.edu/~ee127a/book/login/glossary.html)




