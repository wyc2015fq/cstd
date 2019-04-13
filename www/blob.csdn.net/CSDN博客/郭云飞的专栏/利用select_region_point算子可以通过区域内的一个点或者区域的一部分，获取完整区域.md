
# 利用select_region_point算子可以通过区域内的一个点或者区域的一部分，获取完整区域 - 郭云飞的专栏 - CSDN博客


2014年01月02日 18:00:37[guoyunfei20](https://me.csdn.net/guoyunfei20)阅读数：1683


如果知道区域内的一个点，获取包含该点的完整区域，实例：
```python
connection(WordReg, Regions)
while (1)
    get_mbutton(WindowHandle,Row,Col,Button)
    select_region_point(Regions, TarginRegion, Row, Col)
    dev_display(Image)
    dev_set_draw('fill')
    dev_set_color('green')
    dev_display(TarginRegion)
    stop()
endwhile
```
如果知道区域的一部分，可以通过得到区域轮廓，然后获取第一个点来实现，实例：

```python
while (1)
    draw_rectangle1(WindowHandle, Row1, Col1, Row2, Col2)
    gen_rectangle1(Rectangle, Row1, Col1, Row2, Col2)
    get_region_contour(Rectangle, Rows, Cols)
    tuple_length(Rows, L)
    if (L > 0)
        tuple_select(Rows, 0, Row)
        tuple_select(Cols, 0, Col)
    endif
    connection(WordReg, Regions)
    select_region_point(Regions, TarginRegion, Row, Col)
    dev_display(Image)
    dev_set_draw('fill')
    dev_set_color('green')
    dev_display(TarginRegion)
    stop()
endwhile
```


