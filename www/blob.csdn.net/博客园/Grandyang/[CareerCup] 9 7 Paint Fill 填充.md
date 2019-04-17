# [CareerCup] 9.7 Paint Fill 填充 - Grandyang - 博客园







# [[CareerCup] 9.7 Paint Fill 填充](https://www.cnblogs.com/grandyang/p/4831734.html)







9.7 Implement the "paint fill" function that one might see on many image editing programs. That is, given a screen (represented by a two-dimensional array of colors), a point, and a new color, fill in the surrounding area until the color changes from the original color.



这道题是一道填充问题，有点类似于Flash中的油桶工具，就是给指定的位置填充颜色，如果要填充的颜色和原来的颜色相同，则不发生变换，如果不同的话，则把相连接的区域都填充为新的颜色。那么我们使用递归来做，首先判断要填充的颜色和该位置上原有的颜色是否相同，如果不同的话就开始填充，如果周围颜色和起始位置颜色相同也填充，参见代码如下：



```
enum Color { Black, White, Red, Yellow, Green };

class Solution {
public:
    bool paintFill(vector<vector<Color> > &screen, int x, int y, Color newColor) {
        if (screen[x][y] == newColor) return false;
        return paintFill(screen, x, y, screen[x][y], newColor);
    }
    bool paintFill(vector<vector<Color> > &screen, int x, int y, Color oldColor, Color newColor) {
        if (x < 0 || x >= screen.size() || y < 0 || y >= screen[0].size()) return false;
        if (screen[x][y] == oldColor) {
            screen[x][y] = newColor;
            paintFill(screen, x - 1, y, oldColor, newColor);
            paintFill(screen, x + 1, y, oldColor, newColor);
            paintFill(screen, x, y - 1, oldColor, newColor);
            paintFill(screen, x, y + 1, oldColor, newColor);
        }    
        return true;
    }
};
```














