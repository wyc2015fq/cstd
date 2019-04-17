# [CareerCup] 5.8 Draw Horizonatal Line 画横线 - Grandyang - 博客园







# [[CareerCup] 5.8 Draw Horizonatal Line 画横线](https://www.cnblogs.com/grandyang/p/4752053.html)







5.8 A monochrome screen is stored as a single array of bytes, allowing eight consecutive pixels to be stored in one byte.The screen has width w, where w is divisible by 8 (that is, no byte will be split across rows).The height of the screen, of course, can be derived from the length of the array and the width. Implement a function drawHorizontall_ine(byte[] screen, int width, int xl, int x2, int y) which draws a horizontal line from (xl, y)to(x2, y).



这道题给了我们一个字节数组，用来表示一个单色的屏幕，并给定我们两点坐标，让我们画一条线段。这让我想起了小学的时候，机房的那个电脑只能用图龟在屏幕上画线(呀，暴露年龄了-.-|||)，当然那时候我不可能知道原理的。言归正传，这道题给我们的点的y坐标都相同，就是让我们画一条直线，大大降低了难度。当然我们可以按位来操作，但是这样的解题就不是出题者要考察的本意了，我们需要直接对byte处理。思路是首先算出起点和终点之间有多少字节是可以完全填充的，先把这些字节填充好，然后再分别处理开头和结尾的字节，参见代码如下：



```
class Solution {
public:
    void drawLine(vector<unsigned char> &screen, int width, int x1, int x2, int y) {
        int start_offset = x1 % 8, first_full_byte = x1 / 8;
        int end_offset = x2 % 8, last_full_byte = x2 / 8;
        if (start_offset != 0) ++first_full_byte;
        if (end_offset != 0) --last_full_byte;
        for (int i = first_full_byte; i <= last_full_byte; ++i) {
            screen[(width / 8) * y + i] = (unsigned char) 0xFF;
        }
        unsigned char start_mask = (unsigned char) 0xFF >> start_offset;
        unsigned char end_mask = (unsigned char) 0xFF >> (8 - end_offset);
        if (start_offset != 0) {
            int byte_idx = (width / 8) * y + first_full_byte - 1;
            screen[byte_idx] |= start_mask;
        }
        if (end_offset != 7) {
            int byte_idx = (width / 8) * y + last_full_byte + 1;
            screen[byte_idx] |= end_mask;
        }
    }
};
```














