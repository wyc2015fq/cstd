# [LintCode] Cosine Similarity 余弦公式 - Grandyang - 博客园







# [[LintCode] Cosine Similarity 余弦公式](https://www.cnblogs.com/grandyang/p/4837874.html)








Cosine similarity is a measure of similarity between two vectors of an inner product space that measures the cosine of the angle between them. The cosine of 0° is 1, and it is less than 1 for any other angle.

See wiki: [Cosine Similarity](https://en.wikipedia.org/wiki/Cosine_similarity)

Here is the formula:

![/media/problem/cosine-similarity.png](http://www.lintcode.com/media/problem/cosine-similarity.png)

Given two vectors A and B with the same size, calculate the cosine similarity.

Return `2.0000` if cosine similarity is invalid (for example A = [0] and B = [0]).


Have you met this question in a real interview? 

Yes


**Example**

Given A = `[1, 2, 3]`, B = `[2, 3 ,4]`.

Return `0.9926`.

Given A = `[0]`, B = `[0]`.

Return `2.0000`

` `

这道题让我们求两个向量之间的余弦值，而且给了我们余弦公式，唯一要注意的就是当余弦值不存在时，返回2.0，其余的照公式写即可，参见代码如下：



```
class Solution {
public:
    /**
     * @param A: An integer array.
     * @param B: An integer array.
     * @return: Cosine similarity.
     */
    double cosineSimilarity(vector<int> A, vector<int> B) {
        // write your code here
        double nA = norm(A), nB = norm(B), m = 0;
        if (nA == 0 || nB == 0) return 2.0;
        for (int i = 0; i < A.size(); ++i) {
            m += A[i] * B[i];
        }
        return m / (nA * nB);
    }
    double norm(vector<int> V) {
        int res = 0;
        for (int i = 0; i < V.size(); ++i) {
            res += V[i] * V[i];
        }
        return sqrt(res);
    }
};
```

















