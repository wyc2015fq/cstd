
# Error Analysis - cocoonyang的专栏 - CSDN博客


2017年11月21日 11:06:23[cocoonyang](https://me.csdn.net/cocoonyang)阅读数：335


Error Analysis
[1] “Generally, errors can be divided into two broad and rough but useful classes: systematic and random.”
[2]As mentioned above, there are two types of errors associated with an experimental result: the “precision” and the “accuracy”. One well-known text explains the difference this way:
[2]The word “precision” will be related to the random error distribution associated with a particular experiment or even with a particular type of experiment. The word “accuracy” shall be related to the existence of systematic errors—differences between laboratories, for instance. For example, one could perform very precise but inaccurate timing with a high-quality pendulum clock that had the pendulum set at not quite the right length. E.M. Pugh and G.H. Winslow, p. 6.
## Mean Value
Supposed there are$N$measurements of same response quantity, x.

$$
x_1, x_{2}, \cdots, x_{N}
$$
If the errors were random then the errors in these results would differ in sign and magnitude.
mean value of the measurements

$$
\bar{x}= \frac{ \sum x_{i}}{N}
$$
some of the random variations coulde be expected to cancle out with others in the sum.
[1] This the best can be done to deal with random errors: repeat the measurement many times,
varying as many irrelevant parametes as possible and use the average as the best estimate of the true value of$x$. Mean value should gives you a result with less error than any other of the individual measurements.
## Standard Deviation
It means the most probable value of a Gaussian distribution.

$$
\sigma_{x} = \sqrt{\frac{ \sum (x_{i} - \bar{x})^{2} }{ N-1} }
$$
[1]”The true mean value of x is not being used to calculate the variance, but only the average of the measurements as the best estimate of the true mean value”$(x_{i} - \bar{x})^{2}$is alwayse a little bit smaller than$(x_{i} - \bar{x}_{true})^{2}$. In theory of probability (using the assumption that the data has a Gaussian distribution),it can be shown that this underestimate is corrected by using$N-1$instead of$N$.
## Error Calculation Process[3]
1) Calculate the mean (total of all samples divided by the number of samples)
2) Calculate each response’s deviation from the mean ( mean minus the individual measurement)
3) Square each deviation from the mean
4) Sum the square deviation
5) Divide square deviation sum by one less than the sample size
6) Take the square root of the number in step 5, and the result is standard deviation.
7) Divide the standard deviation by the square root of the sample size, and it is the standard error.
|ID|Response|Devriation|Square|
|---|---|---|---|
|1|10.1|0.1|0.01|
|2|10.3|-0.1|0.01|
|3|10.2|0.0|0.0|
|n=3|mean = 10.2|0.02|

$$
0.02 /(3-1) = 0.01
$$

$$
standard deviation = \sqrt{0.01} = 0.1
$$

$$
SE = 0.1/\sqrt{3} = 0.577
$$

$$
10.2 \pm 0.577
$$
[1][http://teacher.nsrl.rochester.edu/phy_labs/AppendixB/AppendixB.html](http://teacher.nsrl.rochester.edu/phy_labs/AppendixB/AppendixB.html)
[2][https://reference.wolfram.com/applications/eda/ExperimentalErrorsAndErrorAnalysis.html](https://reference.wolfram.com/applications/eda/ExperimentalErrorsAndErrorAnalysis.html)
[3][http://www.runet.edu/~biol-web/stats/standarderrorcalc.pdf](http://www.runet.edu/~biol-web/stats/standarderrorcalc.pdf)

