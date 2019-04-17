# （转自丕子）核函数-Kernel Function汇总 - 家家的专栏 - CSDN博客





2011年09月27日 16:05:28[依海之燕](https://me.csdn.net/yihaizhiyan)阅读数：4147








转自：[http://www.zhizhihu.com/html/y2010/2292.html](http://www.zhizhihu.com/html/y2010/2292.html)



![](http://www.dtreg.com/SvmDimensionMap.jpg)Kernel
 Functions


Below is a list of some kernel functions available from the existing literature. As was the case with previous articles, every
[LaTeX notation](http://pt.wikipedia.org/wiki/LaTeX) for the formulas below are readily available from
their [alternate text html tag](http://en.wikipedia.org/wiki/Alt_attribute). I can not guarantee all of them are perfectly correct, thus use them at your own risk. Most of them have links to articles where they have been originally used or proposed.

### 1. Linear Kernel

The Linear kernel is the simplest kernel function. It is given by the inner product <x,y> plus an optional constant c. Kernel algorithms using a linear kernel are often equivalent to their non-kernel counterparts, i.e.
[KPCA](http://crsouza.blogspot.com/2010/01/kernel-principal-component-analysis-in.html) with linear kernel is the same as [standard PCA](http://crsouza.blogspot.com/2009/09/principal-component-analysis-in-c.html).

![k(x, y) = x^T y + c](http://latex.codecogs.com/gif.latex?k%28x,%20y%29%20=%20x%5ET%20y%20+%20c)

### 2. Polynomial Kernel

The Polynomial kernel is a non-stationary kernel. Polynomial kernels are well suited for problems where all the training data is normalized.

![k(x, y) = (\alpha x^T y + c)^d](http://latex.codecogs.com/gif.latex?k%28x,%20y%29%20=%20%28%5Calpha%20x%5ET%20y%20+%20c%29%5Ed)

Adjustable parameters are the slope alpha, the constant term c and the polynomial degree d.

### 3. Gaussian Kernel

The Gaussian kernel is an example of radial basis function kernel.

![k(x, y) = \exp\left(-\frac{ \lVert x-y \rVert ^2}{2\sigma^2}\right)](http://latex.codecogs.com/png.latex?k%28x,%20y%29%20=%20%5Cexp%5Cleft%28-%5Cfrac%7B%20%5ClVert%20x-y%20%5CrVert%20%5E2%7D%7B2%5Csigma%5E2%7D%5Cright%29)

Alternatively, it could also be implemented using

![k(x, y) = \exp\left(- \gamma \lVert x-y \rVert ^2 )](http://latex.codecogs.com/png.latex?k%28x,%20y%29%20=%20%5Cexp%5Cleft%28-%20%5Cgamma%20%5ClVert%20x-y%20%5CrVert%20%5E2%20%29)

The adjustable parameter sigma plays a major role in the performance of the kernel, and should be carefully tuned to the problem at hand. If overestimated, the exponential will behave almost linearly and the higher-dimensional projection will start to lose
 its non-linear power. In the other hand, if underestimated, the function will lack regularization and the decision boundary will be highly sensitive to noise in training data.

### 4. Exponential Kernel

The exponential kernel is closely related to the Gaussian kernel, with only the square of the norm left out. It is also a radial basis function kernel.

![k(x, y) = \exp\left(-\frac{ \lVert x-y \rVert }{2\sigma^2}\right)](http://latex.codecogs.com/png.latex?k%28x,%20y%29%20=%20%5Cexp%5Cleft%28-%5Cfrac%7B%20%5ClVert%20x-y%20%5CrVert%20%7D%7B2%5Csigma%5E2%7D%5Cright%29)

### 5. Laplacian Kernel

The Laplace Kernel is completely equivalent to the exponential kernel, except for being less sensitive for changes in the sigma parameter. Being equivalent, it is also a radial basis function kernel.

![k(x, y) = \exp\left(- \frac{\lVert x-y \rVert }{\sigma}\right)](http://latex.codecogs.com/png.latex?k%28x,%20y%29%20=%20%5Cexp%5Cleft%28-%20%5Cfrac%7B%5ClVert%20x-y%20%5CrVert%20%7D%7B%5Csigma%7D%5Cright%29)

It is important to note that the observations made about the sigma parameter for the Gaussian kernel also apply to the Exponential and Laplacian kernels.

### 6. ANOVA Kernel

The ANOVA kernel is also a radial basis function kernel, just as the Gaussian and Laplacian kernels. It is said to
[perform well in multidimensional regression problems](http://www.nicta.com.au/research/research_publications?sq_content_src=%2BdXJsPWh0dHBzJTNBJTJGJTJGcHVibGljYXRpb25zLmluc2lkZS5uaWN0YS5jb20uYXUlMkZzZWFyY2glMkZmdWxsdGV4dCUzRmlkJTNEMjYxJmFsbD0x) (Hofmann, 2008).

![k(x, y) =  \sum_{k=1}^n  \exp (-\sigma (x^k - y^k)^2)^d](http://latex.codecogs.com/png.latex?k%28x,%20y%29%20=%20%5Csum_%7Bk=1%7D%5En%20%5Cexp%20%28-%5Csigma%20%28x%5Ek%20-%20y%5Ek%29%5E2%29%5Ed)

### 7. Hyperbolic Tangent (Sigmoid) Kernel

The Hyperbolic Tangent Kernel is also known as the Sigmoid Kernel and as the Multilayer Perceptron (MLP) kernel. The Sigmoid Kernel comes from the
[Neural Networks](http://en.wikipedia.org/wiki/Neural_network) field, where the bipolar sigmoid function is often used as an
[activation function](http://en.wikipedia.org/wiki/Activation_function) for artificial neurons.

![k(x, y) = \tanh (\alpha x^T y + c)](http://latex.codecogs.com/png.latex?k%28x,%20y%29%20=%20%5Ctanh%20%28%5Calpha%20x%5ET%20y%20+%20c%29)

It is interesting to note that a SVM model using a sigmoid kernel function is equivalent to a two-layer, perceptron neural network. This kernel was quite popular for support vector machines due to its origin from neural network theory. Also, despite being
 only conditionally positive definite, it has been found to [perform well in practice](http://perso.lcpc.fr/tarel.jean-philippe/publis/jpt-icme05.pdf).

There are two adjustable parameters in the sigmoid kernel, the slope alpha and the intercept constant c. A common value for alpha is 1/N, where N is the data dimension. A more detailed study on sigmoid kernels can be found in the
[works by Hsuan-Tien and Chih-Jen](http://www.csie.ntu.edu.tw/~cjlin/papers/tanh.pdf).

### 8. Rational Quadratic Kernel

The Rational Quadratic kernel is less computationally intensive than the Gaussian kernel and can be used as an alternative when using the Gaussian becomes too expensive.

![k(x, y) = 1 - \frac{\lVert x-y \rVert^2}{\lVert x-y \rVert^2 + c}](http://latex.codecogs.com/png.latex?k%28x,%20y%29%20=%201%20-%20%5Cfrac%7B%5ClVert%20x-y%20%5CrVert%5E2%7D%7B%5ClVert%20x-y%20%5CrVert%5E2%20+%20c%7D)

### 9. Multiquadric Kernel

The Multiquadric kernel can be used in the same situations as the Rational Quadratic kernel. As is the case with the Sigmoid kernel, it is also an example of an non-positive definite kernel.

![k(x, y) = \sqrt{\lVert x-y \rVert^2 + c^2}](http://latex.codecogs.com/png.latex?k%28x,%20y%29%20=%20%5Csqrt%7B%5ClVert%20x-y%20%5CrVert%5E2%20+%20c%5E2%7D)

### 10. Inverse Multiquadric Kernel

The Inverse Multi Quadric kernel. As with the Gaussian kernel, it results in a kernel matrix with full rank ([Micchelli, 1986](http://www.springerlink.com/content/w62233k766460945/)) and thus forms a infinite dimension feature space.

![k(x, y) = \frac{1}{\sqrt{\lVert x-y \rVert^2 + \theta^2}}](http://latex.codecogs.com/png.latex?k%28x,%20y%29%20=%20%5Cfrac%7B1%7D%7B%5Csqrt%7B%5ClVert%20x-y%20%5CrVert%5E2%20+%20c%5E2%7D%7D)

### 11. Circular Kernel

The circular kernel comes from a statistics perspective. It is an example of an isotropic stationary kernel and is positive definite in R2.

![k(x, y) = \frac{2}{\pi} \arccos ( - \frac{ \lVert x-y \rVert}{\sigma}) - \frac{2}{\pi} \frac{ \lVert x-y \rVert}{\sigma} \sqrt{1 - \left(\frac{ \lVert x-y \rVert^2}{\sigma} \right)}](http://latex.codecogs.com/png.latex?k%28x,%20y%29%20=%20%5Cfrac%7B2%7D%7B%5Cpi%7D%20%5Carccos%20%28%20-%20%5Cfrac%7B%20%5ClVert%20x-y%20%5CrVert%7D%7B%5Csigma%7D%29%20-%20%5Cfrac%7B2%7D%7B%5Cpi%7D%20%5Cfrac%7B%20%5ClVert%20x-y%20%5CrVert%7D%7B%5Csigma%7D%20%5Csqrt%7B1%20-%20%5Cleft%28%5Cfrac%7B%20%5ClVert%20x-y%20%5CrVert%5E2%7D%7B%5Csigma%7D%20%5Cright%29%7D)
![\mbox{if}~ \lVert x-y \rVert < \sigma \mbox{, zero otherwise}](http://latex.codecogs.com/png.latex?%5Cmbox%7Bif%7D~%20%5ClVert%20x-y%20%5CrVert%20%3C%20%5Csigma%20%5Cmbox%7B,%20zero%20otherwise%7D)

### 12. Spherical Kernel

The spherical kernel is similar to the circular kernel, but is positive definite in R3.

![k(x, y) = 1 - \frac{3}{2} \frac{\lVert x-y \rVert}{\sigma} + \frac{1}{2} \left( \frac{ \lVert x-y \rVert}{\sigma} \right)^3](http://latex.codecogs.com/png.latex?k%28x,%20y%29%20=%201%20-%20%5Cfrac%7B3%7D%7B2%7D%20%5Cfrac%7B%5ClVert%20x-y%20%5CrVert%7D%7B%5Csigma%7D%20+%20%5Cfrac%7B1%7D%7B2%7D%20%5Cleft%28%20%5Cfrac%7B%20%5ClVert%20x-y%20%5CrVert%7D%7B%5Csigma%7D%20%5Cright%29%5E3)

![\mbox{if}~ \lVert x-y \rVert < \sigma \mbox{, zero otherwise}](http://latex.codecogs.com/png.latex?%5Cmbox%7Bif%7D~%20%5ClVert%20x-y%20%5CrVert%20%3C%20%5Csigma%20%5Cmbox%7B,%20zero%20otherwise%7D)

### 13. Wave Kernel

The Wave kernel is also [symmetric positive semi-definite](http://www.lib.ncsu.edu/theses/available/etd-02262008-213801/unrestricted/etd.pdf) ([Huang, 2008](http://www.lib.ncsu.edu/theses/available/etd-02262008-213801/unrestricted/etd.pdf)).

![k(x, y) = \frac{\theta}{\lVert x-y \rVert \right} \sin \frac{\lVert x-y \rVert }{\theta}](http://latex.codecogs.com/png.latex?k%28x,%20y%29%20=%20%5Cfrac%7B%5Ctheta%7D%7B%5ClVert%20x-y%20%5CrVert%20%5Cright%7D%20%5Csin%20%5Cfrac%7B%5ClVert%20x-y%20%5CrVert%20%7D%7B%5Ctheta%7D)

### 14. Power Kernel

The Power kernel is also known as the (unrectified) triangular kernel. It is an example of scale-invariant kernel ([Sahbi and Fleuret, 2004](http://hal.archives-ouvertes.fr/docs/00/07/19/84/PDF/RR-4601.pdf)) and is also only conditionally positive
 definite.

![k(x,y) = - \lVert x-y \rVert ^d](http://latex.codecogs.com/png.latex?k%28x,y%29%20=%20-%20%5ClVert%20x-y%20%5CrVert%20%5Ed)

### 15. Log Kernel

The Log kernel seems to be particularly interesting for images, but is only conditionally positive definite.

![k(x,y) = - log (\lVert x-y \rVert ^d + 1)](http://latex.codecogs.com/png.latex?k%28x,y%29%20=%20-%20log%20%28%5ClVert%20x-y%20%5CrVert%20%5Ed%20+%201%29)

### 16. Spline Kernel

The [Spline](http://en.wikipedia.org/wiki/Spline_%28mathematics%29) kernel is given as a piece-wise cubic polynomial, as
[derived in the works by Gunn (1998)](http://www.svms.org/tutorials/Gunn1998.pdf).

![k(x, y) = 1 + xy + xy~min(x,y) - \frac{x+y}{2}~min(x,y)^2+\frac{1}{3}\min(x,y)^3](http://latex.codecogs.com/png.latex?k%28x,%20y%29%20=%201%20+%20xy%20+%20xy~min%28x,y%29%20-%20%5Cfrac%7Bx+y%7D%7B2%7D~min%28x,y%29%5E2+%5Cfrac%7B1%7D%7B3%7D%5Cmin%28x,y%29%5E3)

However, what it actually mean is:

![k(x,y) = \prod_{i=1}^d 1 + x_i y_i + x_i y_i \min(x_i, y_i) - \frac{x_i + y_i}{2} \min(x_i,y_i)^2 + \frac{\min(x_i,y_i)^3}{3}](http://latex.codecogs.com/png.latex?k%28x,y%29%20=%20%5Cprod_%7Bi=1%7D%5Ed%201%20+%20x_i%20y_i%20+%20x_i%20y_i%20%5Cmin%28x_i,%20y_i%29%20-%20%5Cfrac%7Bx_i%20+%20y_i%7D%7B2%7D%20%5Cmin%28x_i,y_i%29%5E2%20+%20%5Cfrac%7B%5Cmin%28x_i,y_i%29%5E3%7D%7B3%7D)

With![x,y \in R^d](http://latex.codecogs.com/png.latex?x,y%20%5Cin%20R%5Ed)

### 17. B-Spline (Radial Basis Function) Kernel

The B-Spline kernel is defined on the interval [−1, 1]. It is given by the recursive formula:

![k(x,y) = B_{2p+1}(x-y)](http://latex.codecogs.com/png.latex?k%28x,y%29%20=%20B_%7B2p+1%7D%28x-y%29)

![\mbox{where~} p \in N \mbox{~with~} B_{i+1} := B_i \otimes  B_0.](http://latex.codecogs.com/png.latex?%5Cmbox%7Bwhere~%7D%20p%20%5Cin%20N%20%5Cmbox%7B~with~%7D%20B_%7Bi+1%7D%20:=%20B_i%20%5Cotimes%20B_0.)

In the [work by Bart Hamers](ftp://ftp.esat.kuleuven.ac.be/pub/SISTA/hamers/PhD_bhamers.pdf) it is given by:![k(x, y) = \prod_{p=1}^d B_{2n+1}(x_p - y_p)](http://latex.codecogs.com/png.latex?k%28x,%20y%29%20=%20%5Cprod_%7Bp=1%7D%5Ed%20B_%7B2n+1%7D%28x_p%20-%20y_p%29)

Alternatively, Bn can be computed using the explicit expression ([Fomel, 2000](http://sepwww.stanford.edu/public/docs/sep105/sergey2/paper_html/node5.html)):

![B_n(x) = \frac{1}{n!} \sum_{k=0}^{n+1} \binom{n+1}{k} (-1)^k (x + \frac{n+1}{2} - k)^n_+](http://latex.codecogs.com/png.latex?B_n%28x%29%20=%20%5Cfrac%7B1%7D%7Bn%21%7D%20%5Csum_%7Bk=0%7D%5E%7Bn+1%7D%20%5Cbinom%7Bn+1%7D%7Bk%7D%20%28-1%29%5Ek%20%28x%20+%20%5Cfrac%7Bn+1%7D%7B2%7D%20-%20k%29%5En_+)

Where x+ is defined as the [truncated power function](http://en.wikipedia.org/wiki/Truncated_power_function):

![x^d_+ = \begin{cases} x^d, & \mbox{if }x > 0 \\  0, & \mbox{otherwise} \end{cases}](http://latex.codecogs.com/png.latex?x%5Ed_+%20=%20%5Cbegin%7Bcases%7D%20x%5Ed,%20&%20%5Cmbox%7Bif%20%7Dx%20%3E%200%20%5C%5C%200,%20&%20%5Cmbox%7Botherwise%7D%20%5Cend%7Bcases%7D)

### 18. Bessel Kernel

The [Bessel](http://en.wikipedia.org/wiki/Bessel_function) kernel is well known in the theory of function spaces of fractional smoothness. It is given by:

![k(x, y) = \frac{J_{v+1}( \sigma \lVert x-y \rVert)}{ \lVert x-y \rVert ^ {-n(v+1)} }](http://latex.codecogs.com/png.latex?k%28x,%20y%29%20=%20%5Cfrac%7BJ_%7Bv+1%7D%28%20%5Csigma%20%5ClVert%20x-y%20%5CrVert%29%7D%7B%20%5ClVert%20x-y%20%5CrVert%20%5E%20%7B-n%28v+1%29%7D%20%7D)

where J is the [Bessel function of first kind](http://en.wikipedia.org/wiki/Bessel_function#Bessel_functions_of_the_first_kind_:_J.CE.B1). However, in the [Kernlab for R documentation](http://rss.acs.unt.edu/Rdoc/library/kernlab/html/dots.html), the Bessel kernel is said to be:

![k(x,x') = - Bessel_{(nu+1)}^n (\sigma |x - x'|^2)](http://latex.codecogs.com/png.latex?k%28x,x%27%29%20=%20-%20Bessel_%7B%28nu+1%29%7D%5En%20%28%5Csigma%20%7Cx%20-%20x%27%7C%5E2%29)

### 19. Cauchy Kernel

The Cauchy kernel comes from the [Cauchy distribution](http://en.wikipedia.org/wiki/Cauchy_distribution) ([Basak, 2008](http://figment.cse.usf.edu/~sfefilat/data/papers/WeAT4.2.pdf)). It is a long-tailed kernel and can be used to give long-range influence and sensitivity over the high dimension space.

![k(x, y) = \frac{1}{1 + \frac{\lVert x-y \rVert^2}{\sigma} }](http://latex.codecogs.com/png.latex?k%28x,%20y%29%20=%20%5Cfrac%7B1%7D%7B1%20+%20%5Cfrac%7B%5ClVert%20x-y%20%5CrVert%5E2%7D%7B%5Csigma%7D%20%7D)

### 20. Chi-Square Kernel

The Chi-Square kernel comes from the [Chi-Square distribution](http://en.wikipedia.org/wiki/Chi-square_distribution).

![k(x,y) = 1 - \sum_{i=1}^n \frac{(x_i-y_i)^2}{\frac{1}{2}(x_i+y_i)}](http://latex.codecogs.com/png.latex?k%28x,y%29%20=%201%20-%20%5Csum_%7Bi=1%7D%5En%20%5Cfrac%7B%28x_i-y_i%29%5E2%7D%7B%5Cfrac%7B1%7D%7B2%7D%28x_i+y_i%29%7D)

### 21. Histogram Intersection Kernel

The Histogram Intersection Kernel is also known as the Min Kernel and has been proven useful in image classification.

![k(x,y) = \sum_{i=1}^n \min(x_i,y_i)](http://latex.codecogs.com/png.latex?k%28x,y%29%20=%20%5Csum_%7Bi=1%7D%5En%20%5Cmin%28x_i,y_i%29)

### 22. Generalized Histogram Intersection

The Generalized Histogram Intersection kernel is built based on the [Histogram Intersection Kernel](http://www-video.eecs.berkeley.edu/Proceedings/ICIP2003/papers/cr1967.pdf)for image classification but applies in a much larger variety of contexts ([Boughorbel, 2005](http://perso.lcpc.fr/tarel.jean-philippe/publis/jpt-icip05.pdf)). It is given by:

![k(x,y) = \sum_{i=1}^m \min(|x_i|^\alpha,|y_i|^\beta)](http://latex.codecogs.com/png.latex?k%28x,y%29%20=%20%5Csum_%7Bi=1%7D%5Em%20%5Cmin%28%7Cx_i%7C%5E%5Calpha,%7Cy_i%7C%5E%5Cbeta%29)

### 23. Generalized T-Student Kernel

The Generalized T-Student Kernel [has been proven to be a Mercel Kernel](http://ralyx.inria.fr/2004/Raweb/imedia/uid84.html), thus having a positive semi-definite Kernel matrix ([Boughorbel, 2004](http://ralyx.inria.fr/2004/Raweb/imedia/uid84.html)). It is given by:

![k(x,y) = \frac{1}{1 + \lVert x-y \rVert ^d}](http://latex.codecogs.com/png.latex?k%28x,y%29%20=%20%5Cfrac%7B1%7D%7B1%20+%20%5ClVert%20x-y%20%5CrVert%20%5Ed%7D)

### 24. Bayesian Kernel

The Bayesian kernel could be given as:

![k(x,y) = \prod_{l=1}^N \kappa_l (x_l,y_l)](http://latex.codecogs.com/png.latex?%5C150dpi%20k%28x,y%29%20=%20%5Cprod_%7Bl=1%7D%5EN%20%5Ckappa_l%20%28x_l,y_l%29)

where

![\kappa_l(a,b) = \sum_{c \in \{0;1\}} P(Y=c \mid X_l=a) ~ P(Y=c \mid X_l=b)](http://latex.codecogs.com/png.latex?%5Ckappa_l%28a,b%29%20=%20%5Csum_%7Bc%20%5Cin%20%5C%7B0;1%5C%7D%7D%20P%28Y=c%20%5Cmid%20X_l=a%29%20~%20P%28Y=c%20%5Cmid%20X_l=b%29)

However, it really depends on the problem being modeled. For more information, please
[see the work by Alashwal, Deris and Othman](http://www.waset.org/journals/ijci/v5/v5-2-14.pdf), in which they used a SVM with Bayesian kernels in the prediction of protein-protein interactions.

### 25. Wavelet Kernel

The Wavelet kernel ([Zhang et al, 2004](http://see.xidian.edu.cn/faculty/zhangli/publications/WSVM.pdf)) comes from
[Wavelet theory](http://en.wikipedia.org/wiki/Wavelet) and is given as:

![k(x,y) = \prod_{i=1}^N h(\frac{x_i-c_i}{a}) \:  h(\frac{y_i-c_i}{a})](http://latex.codecogs.com/png.latex?k%28x,y%29%20=%20%5Cprod_%7Bi=1%7D%5EN%20h%28%5Cfrac%7Bx_i-c%7D%7Ba%7D%29%20%5C:%20h%28%5Cfrac%7By_i-c%7D%7Ba%7D%29)

Where a and c are the wavelet dilation and translation coefficients, respectively (the form presented above is a simplification, please see the original paper for details). A translation-invariant version of this kernel can be given as:

![k(x,y) = \prod_{i=1}^N h(\frac{x_i-y_i}{a})](http://latex.codecogs.com/png.latex?k%28x,y%29%20=%20%5Cprod_%7Bi=1%7D%5EN%20h%28%5Cfrac%7Bx_i-y_i%7D%7Ba%7D%29)

Where in both h(x) denotes a mother wavelet function. In the paper by Li Zhang, Weida Zhou, and Licheng Jiao, the authors suggests a possible h(x) as:

![h(x) = cos(1.75x)exp(-\frac{x^2}{2})](http://latex.codecogs.com/png.latex?h%28x%29%20=%20cos%281.75x%29exp%28-%5Cfrac%7Bx%5E2%7D%7B2%7D%29)

Which they also prove as an admissible kernel function.

[via](http://crsouza.blogspot.com/)




