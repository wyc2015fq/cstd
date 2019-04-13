
# sort_region——对区域进行排序 - 郭云飞的专栏 - CSDN博客


2014年05月16日 09:27:30[guoyunfei20](https://me.csdn.net/guoyunfei20)阅读数：4065


The operatorsort_regionsorts the regions with respect to their relative position. All
 sorting methods with the exception of'character'use one point of the region. With the help of the parameterRowOrCol='row'these points will be sorted according to their row and then according to their column. By using'column', the column value will be used first. The following values are available for the parameterSortMode:'character'
The regions will be treated like characters in a row and will be sorted according to their order in the line: If two regions overlap horizontally, they will be sorted with respect to their column
 values, otherwise they will be sorted with regard to their row values. To be able to sort a line correctly, all regions in the line must overlap each other vertically. Furthermore, the regions in adjacent rows must not overlap.
'first_point'
The point with the lowest column value in the first row of the region.
'last_point'
The point with the highest column value in the last row of the region.
'upper_left'
Upper left corner of the surrounding rectangle.
'upper_right'
Upper right corner of the surrounding rectangle.
'lower_left'
Lower left corner of the surrounding rectangle.
'lower_right'
Lower right corner of the surrounding rectangle.
The parameterOrderdetermines
 whether the sorting order is increasing or decreasing: using'true'the order will be increasing, using'false'the order will be decreasing.

