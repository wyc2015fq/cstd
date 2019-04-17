# Solve problem 'SURF' is not a member of 'cv' - Grandyang - 博客园







# [Solve problem 'SURF' is not a member of 'cv'](https://www.cnblogs.com/grandyang/p/4385683.html)








`SIFT` and `SURF` were moved to `nonfree` module. You need to add` `

```
#include <opencv2/nonfree/nonfree.hpp>
```

header and link with `opencv_nonfree` library.















