# Java嵌入式数据库H2学习总结(三)——在Web应用中嵌入H2数据库 - 孤傲苍狼 - 博客园



# [孤傲苍狼](https://www.cnblogs.com/xdp-gacl/)

只为成功找方法，不为失败找借口！




## [Java嵌入式数据库H2学习总结(三)——在Web应用中嵌入H2数据库](https://www.cnblogs.com/xdp-gacl/p/4190424.html)



　　H2作为一个嵌入型的数据库，它最大的好处就是可以嵌入到我们的Web应用中，和我们的Web应用绑定在一起，成为我们Web应用的一部分。下面来演示一下如何将H2数据库嵌入到我们的Web应用中。

## 一、搭建测试环境和项目

### 1.1、搭建JavaWeb测试项目

　　创建一个【H2DBTest】JavaWeb项目，找到H2数据库的jar文件，如下图所示：

![](data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAqgAAAByCAIAAAAQ4LRrAAAdx0lEQVR4nO2dy28cyX3H+6iL/wAtcpEPAXLOJYcECOBLjgECBQjszSXZOAcjyNoBvPau7YEhbCIgNgLESUCt1o8FNvFDu9qVKFnUk6KWQ1FDSqRIDh8ixYdEjigOhxQ5L3I4Uzk0d9TTVfWrXz+mu2fm+8UXu83qql89uqc+Vd3kyNrZzR/7Vat3txzOs72ld55XV0L8Uu9QgvjyjsP8U7psO+SpnU2HXaU2/dgZcPdFi3f0bsnWGoQo1T3OtZo4RXjDYf4pXTbSBYf5pXwE58cv6E01fr3FheB+nnRv61141lYXHN4prDndcmrb5bWmC61uCbK9psu5Q7hAeLXF269daHXrqdZGFlab3tnWuqD3zvaK04UWt+bMrzRdyK8U8ivW3ONbMAzDMAz3iC0BQRAEQVDPyNopvLBd+NI/+nAAhmEYhuGutBr8cS9HIAiCIAhqiwB+CIIgCOohAfwJkGWFbAiCIAjSyAz+hx99temlpaVSqRRXW7tW4aIa4IcgCIL0MoD/4UdfdeZOnzs5PDwM9ocsgB+CIAiKShT40+dOCiGEOO+wSJ876TIeAwQVwA9BEARFJRP498+L/X9vcavS505++O4bvh8DWF1BKX+9eF0K4IcgCIKikgn8W2fW7v6lvMt3+fOzJ4eHh1vitkpbfespZ+bmsSsIJyyr5yHFEWQv5OoUpRzdNAbntMZbfgiCIKiXZOW3121vf+kW8M+eSZ87uXDnW8fZaxV3gNwZIcSvfnjy7Nmz7tAOVGurVwHSeaAkJSey8Sw/jlFEL+QfFZWG1AxXNAiCIAiSZQB/feaf0+dOVnbPtr7pd3jk67WD//7g3ZNvv/22O7REbvlHIkMHgV+OwAH/62OAH4IgCIpKBvCvXf7a6C9PCfGxqPxM6dztv3p48xsff+crSvBzHqQrn5PLBQ2bZulRuS6I7zxEXUQvjJ21/6crxWmMWwA/BEEQpJcB/I8++sPZa38uxHmxdUbh3Jmd619Lnzv51ltv6R71c3axHKgr89PPAHyU4jxU0J3y2ovXicF6IUdUp0MQBEGQEfzpc2/sPv++EOfFs/cUXn7v6aWvffjuG2+99dbAgPv7/vgPrmlkMtGr3C7LBZ15+HE4226vvXidDvBDEARBUckA/pHzfyDEeTH7LZ3v9P2R/Zx/cnLSHZr96ppApnaXbDqQy7Zpo++7F68zAPwQBEFQVKLAv7S09PF3vmK0/Zx/aWmpJW6rnOnqdnwJSCfYZLnSdTW6EnWt4sQhwuoWB7riulLGd/yWKoM6lAD4IQiCIEoU+Eul0vDw8NmzZ98mdfbsWXyPbyCFi2qAH4IgCNKLAr8QolQqLS0tTZLCV/YGFcAPQRAERSUrn1879tax8c/yRi2AH4IgCIpK1uaLJZcB/qgF8EMQBEFRyVpZnjj202MD/FHL/sX+EA1BEARBGlnjD264/KMPB2AYhmEY7kpb736Ug3vEWy+fwDAMwz1u6/u/2oB7xLHfbTAMw3DsBvh7yLHfbTAMw3DsBvh7yLHfbXACPTJ8SQgRezMSa4wPxrmbbI8zwN9D9nqLWJZlzGDnMeaEE+sunnA5t6UxD3N8mnHkg+aPTnlqZC/Y6zgrh04eYd3wKr9MXRnceSBn6zgD/D1nr7eI7lOByaubDPCHNT7ORbAyrOsUFs0+xtk17ehIrKS1ccrSlVJeuA41wN9z9nR/WNJ+xdOHBO4U9wj4dbdlcPDL9z8NG4Df3zhvSUsr4xpLeaD8UU4kInT0JQP4e878m4NeHesWwj4+FRd+3Rf7J6HH3U3g5zN4i7FB9zo+RECiPR1NkRDtCfxGBstj6wP8rttGdyN1lgH+njPzzmDuSFxnfXwYLvy6z3bsH4ZedjeB32X6nmTewGG949fV2NEUCdHMJyv0ABKLKv5SzHhBO/qSAfw9Zx93CUF95ceP/5FoUh/sj9e9CX7lJK7MzweS8ZTyM9LRFAnRXnf8rmlHR3odrf3t+LvgkgH8PWevtwi9HJY/e/yPhIv6YH+M7lbwEzckvREMOD7EIkBZe0dTJER7fccv/6gcfN1twL8xnAfyTNhxBvh7zvybQ3lz6z5v9MpatpL6YH9c7jXw06T3AX55fSwvlznbzR63j1/uYw6sj0T6Inb05TsGf3v/DaDAajSOXW+Iel3U6+LoSNSOGrWjxmHt2AeHjYPDRvWwUTloVA4a5WqjXG2Uqo1SpVGs1Ivl+n65vleq75Xqr4r1V8X67n59Z7++s1cv7NW3X9W3Xx3ld4+2do+2do5e7hxtFo42C0cvto9y20e5/NFGvra+VXu+VXv+svZss7a2WVt7UVt9UVvJ1VZyteWNw6cbh0/XD5fWDxefHz55fvjk2eHCs8P5tcP5tcO51cPZ1YPZlYPsykF2+WBm+WD66cH004OppYPHSwePFw8mFw8mn1QnnlQnFqqPFqoP56sP56vj89XxuerYXHVsrpqZrWZmqw+ytiujM5XRmcp929OVkenKyHQlXPDrpir5vtd9GDr6U9GD7krw07tAXc7g40M/ZiBWBrDvHb/uQHnsCkUnduUUB/B3CfhHprjs93R/MLdKXfap6EF3H/iZs7zxlNfxCXG10YPmg9+V4hp2OUVZ1rgIk4+7Y4oD+JMI/rHZama2nJkp3Z8qjs4U708X70+VRmfKNPjTPPZ7uj+MWxP58xb7PQ37cDeBn7gPlTcw577lP+on0nWYYbahF+wJ/JxxM05ZdHy66tiHK+A4A/xJAf/4XDUzU7qbyQ8MrV25+fTy9cUrN5eu3Fz8/PqT/lvLN9MvRqaK96fLkYEf7hF3E/gxPp1rjHOU4wzwJwX8men9gaFnnw/MDY0szy9uPc+9yhdKm1vFpZXtdGb108vTnw0s3nmwPTJVAvhhGIZh3wb4EwH+zMz+wNDqpWuzswsvy5VD1yDU64313Ksbg08u9M/eur+VflwamS67wf/LDdrCsuBQvPZ0HIZhuHMN8McP/kcL5Tv3c5cG5uYXt+r1hm4odl9Vbg4tfnp1bnBsN/24NDJV9gr+7OMhOKAxjDAMd7otIcSpN9On3kzreGOfJTI4s/EzMPMT4LczGMFvZ7PBbx8nDfyjU68u33gyNrleO6o7R8D6049dY/IyX/zs6kz/nbV7j/bSj0tewZ8ZuQYHNIYRhuFOtxr8Tdg3T7lSlMx2ptMxdeCXa3GaAL8zmxL8LicK/PfG81duLmxuFV3Ut91MaTREvdEYm3j+ydX5Ww8Kw5PFtL3pZ4O/0qr3338/l8vqfPr06QokSR7G7hCxBIcgqMv0GvwEiYUG6jSnlVt8ZVlly+gdv7IuecffPJXYHf/EQmVw9MXg8NLB4dHrq/Il8p3sbzRE7ai+ur77ydXstfTmvYd7w/amPwD4iTsD7FcK4IcgqNNlKVFNU1y3m9edFYy1gi5no3F8oAR/8x2/Evx2YrHcsulP2jv+iYXK7fsvRjJrry9J60a/+WO90Tio1V/ki58PzPUP5QbHXrUV/ALsVwnghyCo02V4x0/v+OVsxFnj6kGX38a/E/zNdAL8x9T/8pf7kgz+WyO5dGZVcW1a3/Ef1RvVw6PN7eLl6/OX727cyex8MVlsK/j7+vq++93vWl2KOn8C+CEI6nSxwB/ijp9I0VXqAr/umYHTynf8zUf9p5L0jn9ioXL7fu72F0vyb/bJz/lL1drzzb3PB+YuDa7fflAIC/wzC0+bicVisVKplEqli4OPcrlsX1+fE/z2t1a54jS/RYtGC3MB4TWOXDuzPfwmOQXwt0+WZfgjo+aVJQrq8igzyN+g56m1/gqGIrlS3y2JtyMcyW1TtlbZeM5NRdcVeyllQWMRQ0DjU3fmZl0ZgQglR+YQnf+o31mqueO3/5yvmZ4Q8A+Nb10amN0ulF5fFekdf73RqB4e7ZUP55/mP7062383d/tB4V5I4M/n87VarVar7e/v53/6J/bxf/zfdSHE2NiYJYFW9yPBUT6J6WyuDPIxsz2+BfC3ScYZsHlWOXvKp4hocn5dcGObfZTyGtxTBt8tSSbyheqaKq+y8v7h3FS6e4a+fyIrpSwYAvgFD+ShgJ9O0UUjfrmPfsfv/OU+lxO1409P7n56dfbh4416XTQa6nf8B7X6fvlwa7d8b3Tlt/2z/UO5EHf8uVyuXC7v7+9v/vCNF/0/3t/fL5fLP/7goggP/MazFRW5jXHo2gF+vnjTRXvFBL+c6A/8On54bXCbkAnw22KCX0hdYF4d3zD2lM6vi9OLdoFf5j3xYEAuxTnFzCw/6vcEfvsdf/NRf3O7nyjwZ7LFa4Nrn12dXnm2Uzuq2643GrZrR/XKwdFe+XB7rzo9v3nx6syFa0v99zZDfMe/urq6u7ubz+dXV1fz+fyzZ88KhcK//OQjoQG/cl/eTLFaJWfQyQf4de1RtlCOQ5RqHruyAfztE2ffo9w2yfshT1soZimiSa6wciOJ6pSllO1RhpXzGDur64uulK5fEcvZHjlRmJBptUpXSq6LWYozOD7uOlcviDZ7knbHb4u54zcuC/jgd6UH+a1++Qt8Egj+ySfV8bnSFw93Pr22cHlgdmm1UD6olaq18kHNPtgvH+7sV/O7lemFl/035n5zafbirWfX0vkQf6t/cXFxY2PjzR/8z9ff/a9/fP/DjY2Nf/v5xb9P/UyQO37LC9SNeZRhjXHoNvgLZUnsdwrgb584M6DzWCaBcUJXFtdFYDaYaBjdSE/tD6sU3ReiihiRL/R9NB57hTGnCJHH6z3MjBYb+Imz9DpAF4co5Uy0n/M3T9F/x9+088/5Tjne8dtO1G/1Tz6pPpwvj07t3R7dvPD7uc+uZh9N5za2ioW9amG/ur1X3dopr268ykyuXx7I/vbS7Cc3Vvrv5m6Mbof4d/zZbPb58+ffPPPB33zvP7955oO//cF/r6ys/PW3/1XwwM+hPiebc2XNjBMi+CvSwwNlcYC/ffIxabp2Y7GAn050tVBZirlw0cWhwe8qxeyLsc1RisBe8L77Y2o04Bfkg64givNyctQb39VfGZ8tpSd3b97f/HTgyW8uT125OTeYXh7OrA5nVgfTy/035n93aeY3V+YvXF+5fHfjWjp/Z2wnlG/uswd5eXn5dzdG/ukn/9t0Pp//i394TzDAz6Q+P6cnWsvH9FKAEwrgj0v+Jk3BBieRPyD4XQdGfhvjyNmYbfaHMV2QJICfHkDjFfe04gk4XMYI/krJGQD+bgD/xJPqw7nyg5niFxO7N0e3+u8+u3jj6ScDixeuLf7u98f+5PrKxVvP+gc3rg2/vJ0p3JvYd31X//d+uUGbAP/fvffTP/vGt12+eHNYSOBvrj0rrRt0Z7pScgZdZmc2OY8yDidFV5cyONEjgL9Nag64M8WYx5lO51EGkS+3jwa7ornao+uaLkVO1OVx5dd1hO6aszjRO87gyBk4KUYp28DvJt1y5hjSpYjafZRSBqH77kMAfyLAP7FQHZ8rj04Xhyd272TyN0ZeXr2Xu3ov1z+00T+00T+Uu3I3d21488ZIfnBs597Efnqq5PpneYOAn5C84+8m+egawB+ZAk5tEEehUATqOCX9evcO+B8tVMdmyw+ypfTkq3sPd+5ktm+PFW6N5m+N5m89KNx6sD04Vhga3x1+XExPlUamy27w/2KDNsDfVHOy81EW4I9GQBEEtU9J/3T1FPgfzlfH5yqZbHl0pjQyVUw/3h+e2Bue2PtiYn94spieKo48Lt2fLt+fqdyfqQD8sQjghyCo0wXwJwv8D+er4/PVsdlKZraSma2MzpRHZ8qjM5XRmUrzICzwn+YJ4HcK4IcgqNMF8CcR/ONz1bG56thcNTNbzcxWH2RtVwKCHw7FcTO6LYr7gw5BUHSysssVuNMthDCC/3u/2AgChhMnTsT+X6h9insigiAoOgH83WDhHfwpj4oLSE2B/W1V3BMRBEHRCeDvBgtf4B/kKZVKWZYF7na34p6IID9y/m2360DOo8vgPCWLjua7LiheAfzdYOEX/JaVMtoGf1xAagorj7Yq7okI8qPg4FdinojmSlQGp4tDSRDA3w0W4YFftH56Lcty7viVf/5u/LN43380D0WmeKehcGXclTYhZ9zgGuM4D1wB6dpd+NSlMOMQrNXldOURqtEg0nUdkRvjaVTpzhIpdNfo4HRdsZdSFjQWMQSMHVpwcAu/4Ge+4Lf0X39LnNJlI0Rkw46/rQo4jyRHnuZf4oBfkZKpRByCi57iEO1XRnC1Vo5jHA26Yc5SxOLAq5jjo+uXcQB1naKvYGSllAUBfriSDQB+zi3iescfF/ihtirgPJIoMcFP0JFZCwF+TvM44OeHUnJdF1wHaWULiWjOH4lRDSIm+HWtZd4MxmhEKU46vy5OLwB+uJI9Bv+60c6JXgZ/KmWlUoq1pLzjt+UiB81sZSlXii6yLez426qA80iixJnrldzyNJ8qWStz14hV5aTPbIyMWx0haKg7O6Jss66I7qwuJYiIliurk0dG2S9lI32U4lwyH3edfJPo2uxJAH83WIQBfsuyhFCwX7fjt3h7fV0pf3GgNingPJIoMedfHXE91eJjXuaA3xiHXkwQLaQ5x6mXDqJcPYQAKo97Zf4F9bde0eVhXnriWJkf4IcVFoHBn0rZ1D9+o59KWY5TKSWbleTWSYd5W3I2Wdjxt1UB55FEKXbw03H44De2hwa/MgidwUhrI/j5jeeLwB69bOIsO/wxNRrwC00v2gX+U2+mmYlEuifTQeSzp95MKxP5QbzWmGSLkHb8g4OD9p3mTKd3/Bzqy6WUB/xoUOgKOI8kSvxNHhOcRBAd/oOAnz/Fy6GUYOaA30elxvRQwE+H5a9pOPG9Xn2vEfyVkjO0Bfw283RYtYnrOnYm+jBRoxyfrsh5Skn6ZiKnuoD96izwC+nzb8u542+uPZ3HTjWzubjiyuBKtKTHADKZsONvqwLOI8mRvD0iMsg3sKeK5P/ya3fN+84UfntcRVztcbVTPiU0H3ldva5xM5byNJ46KS+QcQzlNtPBibqMpYjafZRSBqH77kNa8NOkVB77AySBamVwGee0afC39TFGpOD/+brRzone92/1Q12pIJMIFL1kYsnpSqjowGZpJNdlbIMzQzi9hcKWG/x89IayOTZyXQdsGtV0L4yLA051ibLwBX6+rAQ8gcfKo62KeyKCICg6tYBfiUPjFtkTI10A1mGbeJWgW2TQD/nlxtMLCPp5QNIsvINft8DXCdztbsU9EUEQFJ20O37jRl8HZmW68akAjVjiiT0H/MyUngJ/xwkrj7Yq7okIgqDo5Bn8HMr6sG41oNzxE/mNp3QLER+LleRYtB/8Nnfj/S/UPsU9EUEQFJ3U4Dc+6g8dkMb9t/GNA+cUp0Y6TjItfIH/fVJ9fX2xEAiKRXFPRBAERSfDL/cpd/ZEHt/mP3inqcxP5+TsFPYLv+DP5bI69/X1nT59OhYIQdEr7okIgqDo5BP8kT3qNx44M8uPB4j4wR8MJMfCL/iJO6Ovr08IAfb3iCKabyAISoAU4Cfef8v4b/ejfhfRlWsCHemVLyx0NRKN79Z3/Ebwn/5SsaAIilJRTTgQBMUvfFd/N1gEAD/xmv/EiROZTMYJfvtP+1xxmn/yx8eM1/xQuxX3RBSm6O+NUX41je47aoK3hBPTdwOCN1jXfcvj19K1VcrmyU1SttDYbDkIp7NRllIWNBYxBIwdWnBwi2Dgt6w+MfbHltXn8okTJ4T0tN+SvnZXd4oWwJ8oBZxHkiNP8y9xEGJ7+BnaAVcfo+EvTpskj49yxJTX3Xgz0MudJJRSFgT44Uo2DPArDfD3jgLOI4kSE3U6fsTSmPbJx2j4i9MmMcEvpBb66JcnGHtK59fF6QXAD1eygcGvvDNyuawO/LZc0WiQWw7RcaBYFHAeSZQ4c708k4azkXKIH5nYrTbnfdeBXEqu2lPtcik5G6fZ7ZOz48pW0ci0WqUrJdfFLMUZDR93nasXRJs9CeDvBgsh3vn5utGuub4V/Fbq9b0uBAl+10GFsX2XMyjjQHEp4DySKDHnX+U8HnBKJXjpiQpyKQ6MjWgk6jWihVNX+8SBvb8LylnicEpxyjKXLHI6wA8rLAKDP2VZ4vjzLywhLMsD+JnktmcuIg4UowLOI4lSjOAX+le5nnDCpFGI4Dcm6narEYCfwB7RU2YL/TE1GvAL/YMcTiOplsQOLTi4RQg7fvF6x5+yrJTFBL9XbPteN0BtVcB5JFHio444CKsNxsjMVUICwU+HDUt0S4w99dR9fqd8INx3KTkDwA9XsoHBr/vyPhn8zbVnpfW1vTO9onmwryxbkZ4EQLEo4DySHBk3ec4M8g3stS5dcFf85jHRHnlyV8YheqrrBdE1TmTdEPkbNK9S1h5K3+VSup7SpYjafZRSBqH77kMAfzdYCPHOh+tGu+Z6G/wnSAl8eV9vKMgkAiVE7WYw1DVSgN/4dbxev6s/YAZP1fHrMv4jPcp04psKY/x2PxEA/EYB/L2gNs8zUNsVzf4b6g6Zwc+BXPB/6c4Y3Pgt/Uwqc1Bt7B3ze/6TD/4xhgD+XlDcExEEQdGpBfwEYvm45eOcNt0qJrNp8IfYnk4E/2m2YkERFKXinoggCIpO6n+kx4U3DtKIbTGNRuPyggAzsy4+1H20tnPBD0FNxT0RQRAUnQyP+pU/umCsA60Oll6fChgXBzKGvT4PkCtVrgzodQPAD3Wu4p6IIAiKTtxf7uPgmUAp8yyx4w8L/MReP6tZytBt6FDwpzwqLiBB0SjuiQiCoOikftTvdWsbBPw0rZundO3x8Rhf1xHd2W4F/yBPqVTKwt/Zd7vinoggP3L+bbfrQM6jy+A8JYuO5rsuKF75+eU+mpftAD8zGvM5vzIbwXviUX+ng9+yv6WPNMDfC4p7IoL8KDj4lZgnorkSlcHp4lAS5PNRv3O3bUQy8806M52T07iG4CxTemHHL2NetH56Lctygt+eHVzMaM4afMx4zQ+1W/FOQ+HKuCttQs64wTXGcR64AtK1u/CpS2HGIViry+nKI1SjQaTrOiI3xtOo0p0lUuiu0cHpumIvpSxoLGIIyAQkB4deod48q1xGcKDLz+bateuqkztI/CoAc92TZPAzX/Bb+n+IjzhFi5nZU0zItwLOI8mRp/mXOOBXpGQqEYfgoqc4RPuVEVytleMYR4NumLMUsTjwKub46PplHEBdp+grGFkpZcG2g98JOR2ziXfqNKpppvKJTjwDcD6NJ6rTFffX5g4CP+cWAfh7QQHnkUSJCX6CjsxaCPBzmscBPz+Ukuu64DpIK1tIRHP+SIxqEDHBr2st82YwRiNKcdL5dXF60Rbwwx1nEQb4UykrlVKsJWXw23KRw1L9q30EtpVxXCm6uqDQFXAeSZQ4c72SW57mUyVrZe4asaqc9JmNkXGrIwQNdWdHlG3WFdGd1aUEEdFyZXXyyCj7pWykj1KcS+bjrpNvEl2bPQng7waLMMBvWZYQCvbrdvzKxIoX8BsP6AhQiAo4jyRKzPlXR1xPtfiYlzngN8ZxMYPGuS6Dv3rpIMrVQwig8rhX5l9QJcI9tYdfltNyZX6AH1ZYBAZ/KmVT//iNfiplOU4ZwC+zuflpJ0ijw7yrIMAfjQLOI4lS7OCn4/DBb2wPDX5lEDqDkdZG8PMbzxeBPXrZxFl2+GNqNOAXml6EDH7XG3ElY+J9mQ0rLULa8Q8ODtp3mjOdBr8SzMQpXRzlgTEIFJYCziOJEn+TxwQnEUSH/yDg50/xciglmDng91GpMT0U8NNh+WsaTnyvV99rBH+l5AwxgJ8+BcdiIcQ759eNdk70yl/uU66O5T/na3JdVjNbxcRvVxFXoiU9BggNcZBKAeeR5EjeHhEZ5BvYU0Xyf/m1u+Z9Zwq/Pa4irva42imfEpqPvK5e17gZS3kaT52UF8g4hnKb6eBEXcZSRO0+SimD0H33IYC/GyxCAr9S+AKfXlCQSQSKXjKx5HQlVHRgszSS6zK2wZkhnN5CYUv7lb1yIp0HjtHCF/j5Avi7XnFPRBAERScF+LOte/pQvsMHbquFd/DrFvg6xQUkKBrFPRFBEBSdWODXfakOwJ8QC+/ghyCn4p6IIAiKTh52/HIKwJ8QCyHeOf/c6LjhAiVXcU9EEARFJzP4s6qv0cU7/kRZAPxQMMU9EUEQFJ3+H21Qi86+UzlzAAAAAElFTkSuQmCC)

　　H2数据库就一个jar文件，这个Jar文件里面包含了使用JDBC方式连接H2数据库时使用的驱动类，将"h2-1.4.183.jar"加入到【H2DBTest】项目中，如下图所示：

![](data:image/png;base64,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)

## 二、启动H2数据库

　　既然是要将H2数据库作为我们Web应用的一部分嵌入进来，那么我们就要在Web应用中启动H2数据库的服务，这样我们才能够连接到H2数据库，因此我们可以编写一个专门用于启动H2数据库服务的监听器(Listener)，监听器示例代码如下：

```
1 package me.gacl.web.listener;
 2 
 3 import java.sql.SQLException;
 4 import javax.servlet.ServletContextEvent;
 5 import javax.servlet.ServletContextListener;
 6 import org.h2.tools.Server;
 7 
 8 /**
 9 * @ClassName: H2DBServerStartListener
10 * @Description: 用于启动H2数据库服务的监听器，在应用系统初始化时就启动H2数据库的服务
11 * @author: 孤傲苍狼
12 * @date: 2014-12-20 下午11:43:39
13 *
14 */ 
15 public class H2DBServerStartListener implements ServletContextListener {
16 
17     //H2数据库服务器启动实例
18     private Server server;
19     /* 
20      * Web应用初始化时启动H2数据库
21      */
22     public void contextInitialized(ServletContextEvent sce) {
23         try {  
24             System.out.println("正在启动h2数据库...");
25             //使用org.h2.tools.Server这个类创建一个H2数据库的服务并启动服务，由于没有指定任何参数，那么H2数据库启动时默认占用的端口就是8082
26             server = Server.createTcpServer().start(); 
27             System.out.println("h2数据库启动成功...");
28         } catch (SQLException e) {  
29             System.out.println("启动h2数据库出错：" + e.toString());  
30             e.printStackTrace();  
31             throw new RuntimeException(e);  
32         }  
33     }
34 
35     /* 
36      * Web应用销毁时停止H2数据库
37      */
38     public void contextDestroyed(ServletContextEvent sce) {
39         if (this.server != null) {
40             // 停止H2数据库
41             this.server.stop();
42             this.server = null;
43         }
44     }
45 }
```

　　监听器写好之后，我们在Web.xml文件中注册这个监听器，另外，因为我们要将H2数据库嵌入到我们的Web应用当中，为了能够方便访问H2数据库提供的Console，我们可以在Web.xml文件中配置用于访问H2数据库Console的Servlet。

Web.xml文件的配置如下：

```
1 <!-- 使用监听器启动和停止数据库 -->
 2       <listener>
 3         <listener-class>me.gacl.web.listener.H2DBServerStartListener</listener-class>
 4     </listener>
 5     
 6     <!-- 使用H2控制台的Servlet H2控制台是一个独立的应用程序，包括它自己的Web服务器，但它可以作为一个servlet作为-->
 7     <servlet>
 8         <servlet-name>H2Console</servlet-name>
 9         <servlet-class>org.h2.server.web.WebServlet</servlet-class>
10          <init-param>
11             <param-name>webAllowOthers</param-name>
12             <param-value></param-value>
13         </init-param>
14         <init-param>
15             <param-name>trace</param-name>
16             <param-value></param-value>
17         </init-param>
18         <load-on-startup>1</load-on-startup>
19     </servlet>
20     <!-- 映射H2控制台的访问路径 -->
21     <servlet-mapping>
22         <servlet-name>H2Console</servlet-name>
23         <url-pattern>/console/*</url-pattern>
24     </servlet-mapping>
```

　　配置好Listener和访问Console的Servlet之后，我们就可以把H2数据库当作是我们Web应用中的一部分来使用了。

　　将Web应用部署到Tomcat服务器，当启动Tomcat服务器时，在控制台就可以看到H2数据库启动成功的消息，如下图所示：

![](https://images0.cnblogs.com/blog/289233/201412/282038550137279.png)

　　为了进一步验证H2数据库是否真的是通过监听器正常启动了，我们可以访问一下H2数据库的Console，输入访问地址："http://localhost:8080/H2DBTest/console/"进行访问，如下图所示：

![](https://images0.cnblogs.com/blog/289233/201412/282046307788839.png)

　　能够看到H2数据库Console的登录页面，说明了H2数据库已经正常启动了。

## 三、向H2数据库注册自定义的数据库函数

　　H2作为一个数据库，和其他类型的数据库一样，会自带有一些数据库函数给我们使用，但是H2数据库提供的数据库函数有限，无法满足我们开发中的需求，幸运的是，H2数据库支持自定义数据库函数的，因此我们可以根据开发中的实际应用场景编写满足我们需求的数据库函数。

　　下面就来说一下如何实现H2数据库的自定义函数

　　在MySQL数据库中有一个UUID函数是用来生成UUID的，执行"SELECT UUID()"就可以看到UUID函数生成的UUID，如下图所示：

![](https://images0.cnblogs.com/blog/289233/201412/272251323587503.png)

　　而默认情况下，H2数据库是没有提供有UUID这个函数给我们使用的，如下图所示：

![](https://images0.cnblogs.com/blog/289233/201412/282317024663809.png)

　　那么我们现在就来实现一个UUID函数，然后注册到H2数据库当中，这样H2数据库就支持UUID函数了，具体做法分为两个步骤：

     　　(1) 使用Java实现自定义函数的方法。

　　　  (2) 将Java的自定义函数注册到H2数据库中。

　　首先我们来实现这个UUID函数，在java中，生成一个UUID的方法是使用java.util.UUID这个类里面的一个randomUUID()方法生成的，封装成一个uuid方法，代码如下：

```
1 package h2db.function.ext;
 2 
 3 import java.util.UUID;
 4 
 5 /**
 6 * @ClassName: H2DBFunctionExt
 7 * @Description: 针对H2数据库函数的扩展
 8 * @author: 孤傲苍狼
 9 * @date: 2014-12-20 下午11:20:34
10 *
11 */ 
12 public class H2DBFunctionExt {
13 
14     /**
15     * 用法：SELECT uuid();
16     * H2数据库注册uuid函数：CREATE ALIAS uuid FOR "h2db.function.ext.H2DBFunctionExt.uuid";
17     * @Method: uuid
18     * @Description: 实现MySQL数据库的uuid函数，用于生成UUID
19     * @Anthor:孤傲苍狼
20     *
21     * @return
22     */ 
23     public static String uuid(){
24         return UUID.randomUUID().toString();
25     }
26 }
```

　　这样，我们的uuid函数就算是编写好了，需要注意的是，类和方法必须是公共(Public)的,且方法需为静态(static)的，如方法中使用了Connection对象需将其关闭。

　　接下来我们要将其注册到H2数据库中，须执行"CREATE ALIAS"语句，SQL语法如下：

```
1 CREATE ALIAS [IF NOT EXISTS] newFunctionAliasName [DETERMINISTIC] FOR classAndMethodName
```

　　其中[]括起来的部分是可选的，本例须执行的语句为：  `CREATE ALIAS UUID FOR "h2db.function.ext.H2DBFunctionExt.uuid"` ，执行结果如下图所示：

![](https://images0.cnblogs.com/blog/289233/201412/282334007783508.png)

　　这样H2数据库中就多了一个UUID函数可以使用了，我们再次执行"SELECT UUID()"语句就可以被H2数据库正常解析了，执行结果如下图所示：

![](https://images0.cnblogs.com/blog/289233/201412/282338172635818.png)

　　以上就是针对H2数据库函数的一个扩展，我们向H2数据库新增加了一个UUID函数用于生成uuid。因此当H2数据库提供的函数不满足我们开发中的实际需求时，就可以使用这种方式来扩展H2数据库的函数了。接下来演示一下一次性向H2数据库扩展多个函数，我们编写一个H2DBFunctionExt类，在类中编写针对H2数据库的扩展函数，代码如下：

```
1 package h2db.function.ext;
 2 
 3 import java.net.InetAddress;
 4 import java.net.UnknownHostException;
 5 import java.text.ParseException;
 6 import java.text.SimpleDateFormat;
 7 import java.util.Date;
 8 import java.util.UUID;
 9 
10 /**
11 * @ClassName: H2DBFunctionExt
12 * @Description: 针对H2数据库函数的扩展
13 * @author: 孤傲苍狼
14 * @date: 2014-12-20 下午11:20:34
15 *
16 */ 
17 public class H2DBFunctionExt {
18 
19     /**
20     * 用法：SELECT uuid();
21     * H2数据库注册uuid函数：CREATE ALIAS IF NOT EXISTS uuid FOR "h2db.function.ext.H2DBFunctionExt.uuid";
22     * @Method: uuid
23     * @Description: 实现MySQL数据库的uuid函数，用于生成UUID
24     * @Anthor:孤傲苍狼
25     *
26     * @return
27     */ 
28     public static String uuid(){
29         return UUID.randomUUID().toString();
30     }
31 
32     /**
33     * H2数据库注册currentTime函数：CREATE ALIAS IF NOT EXISTS currentTime FOR "h2db.function.ext.H2DBFunctionExt.now";
34     * @Method: now
35     * @Description: 实现MySQL数据库的now()函数，用于生成当前系统时间
36     * @Anthor:孤傲苍狼
37     *
38     * @return
39     */ 
40     public static String now(){
41         return new Date().toLocaleString();
42     }
43     
44     /**
45      * H2数据库注册IP函数：CREATE ALIAS IF NOT EXISTS IP FOR "h2db.function.ext.H2DBFunctionExt.getIp";
46     * @Method: getIp
47     * @Description: 
48     * @Anthor:孤傲苍狼
49     *
50     * @return
51     */ 
52     public static String getIp(){
53         try {
54             InetAddress addr = InetAddress.getLocalHost();
55             //获得本机IP
56             return addr.getHostAddress();
57         } catch (UnknownHostException e) {
58             e.printStackTrace();
59             return "未知的IP地址";
60         } 
61     }
62     
63     /**
64     * H2数据库注册date_format函数：CREATE ALIAS IF NOT EXISTS date_format FOR "h2db.function.ext.H2DBFunctionExt.date_format";
65     * @Method: date_format
66     * @Description: 实现MySQL数据库的date_format()函数，用于格式化日期
67     * @Anthor:孤傲苍狼
68     * @param date
69     * @param pattern
70     * @return
71     */ 
72     public static String date_format(String date,String pattern){
73         if (date != null) {
74             SimpleDateFormat sdf = new SimpleDateFormat(pattern);
75             try {
76                 Date temp = sdf.parse(date);
77                 return sdf.format(temp);
78             } catch (ParseException e) {
79                 e.printStackTrace();
80             }
81         }
82         return "";
83     }
84 }
```

　　为了实现批量注册H2数据库的扩展函数，我们可以编写一个Servlet，专门用于注册扩展函数，代码如下：

```
1 package me.gacl.sys.init;
 2 
 3 
 4 import java.sql.Connection;
 5 import java.sql.Statement;
 6 
 7 import javax.servlet.ServletException;
 8 import javax.servlet.http.HttpServlet;
 9 
10 import me.gacl.util.JdbcUtil;
11 
12 /**
13 * @ClassName: RegisterH2ExtFuncServlet
14 * @Description:注册H2数据库的扩展函数
15 * @author: 孤傲苍狼
16 * @date: 2014-12-20 下午11:47:03
17 *
18 */ 
19 public class RegisterH2ExtFuncServlet extends HttpServlet {
20 
21     /**
22     * @Field: serialVersionUID
23     */ 
24     private static final long serialVersionUID = 4379248469825545593L;
25 
26     public void init() throws ServletException {
27         //1、注册uuid函数的SQL语句
28         String sql1 = "CREATE ALIAS IF NOT EXISTS uuid FOR \"h2db.function.ext.H2DBFunctionExt.uuid\"";
29         //2、注册currentTime函数的SQL语句
30         String sql2 = "CREATE ALIAS IF NOT EXISTS currentTime FOR \"h2db.function.ext.H2DBFunctionExt.now\"";
31         //3、注册IP函数的SQL语句
32         String sql3 = "CREATE ALIAS IF NOT EXISTS IP FOR \"h2db.function.ext.H2DBFunctionExt.getIp\"";
33         //4、注册date_format函数的SQL语句
34         String sql4 = "CREATE ALIAS IF NOT EXISTS date_format FOR \"h2db.function.ext.H2DBFunctionExt.date_format\"";
35         Connection connection = null;
36         Statement stmt = null;
37         try {
38             //获取数据库连接
39             connection = JdbcUtil.getConnection();
40             //获取Statement对象
41             stmt = connection.createStatement();
42             //添加要执行的SQL
43             stmt.addBatch(sql1);
44             stmt.addBatch(sql2);
45             stmt.addBatch(sql3);
46             stmt.addBatch(sql4);
47             //批量执行上述的4条SQL
48             stmt.executeBatch();
49             System.out.println("H2数据库扩展函数注册成功！");
50             stmt.clearBatch();
51         } catch (Exception e) {
52             System.out.println("H2数据库扩展函数注册失败！");
53             e.printStackTrace();
54         }finally{
55             try {
56                 stmt.close();
57                 connection.close();
58             } catch (Exception e2) {
59                 e2.printStackTrace();
60             }
61         }
62     }
63 }
```

　　在Web.xml中注册RegisterH2ExtFuncServlet

```
1 <servlet>
 2     <description>注册H2数据库的扩展函数</description>
 3     <servlet-name>RegisterH2DBExtFunction</servlet-name>
 4     <servlet-class>me.gacl.sys.init.RegisterH2ExtFuncServlet</servlet-class>
 5     <!-- 
 6     1、load-on-startup元素标记容器是否在启动的时候就加载这个servlet(实例化并调用其init()方法)。
 7     2、它的值必须是一个整数，表示servlet应该被载入的顺序
 8     3、当值为0或者大于0时，表示容器在应用启动时就加载并初始化这个servlet；
 9     4、当值小于0或者没有指定时，则表示容器在该servlet被选择时才会去加载。
10     5、正数的值越小，该servlet的优先级越高，应用启动时就越先加载。
11     6、当值相同时，容器就会自己选择顺序来加载。
12     所以，<load-on-startup>x</load-on-startup>，中x的取值1，2，3，4，5代表的是优先级，而非启动延迟时间。
13      -->
14      <load-on-startup>1</load-on-startup>
15 </servlet>
```

　　RegisterH2ExtFuncServlet要批量执行SQL语句，因此需要连接上H2数据库才能够执行，工具类JdbcUtil提供了获取数据库连接的方法，JdbcUtil的代码如下：

```
1 /**
 2  * 
 3  */
 4 package me.gacl.util;
 5 
 6 import java.io.InputStream;
 7 import java.sql.Connection;
 8 import java.util.Properties;
 9 import org.h2.jdbcx.JdbcConnectionPool;
10 
11 public class JdbcUtil {
12 
13     /**
14      * H2数据库自带的连接池
15      */
16     private static JdbcConnectionPool cp = null;
17     
18     static{
19         try {
20             //加载src目录下的h2config.properties
21             InputStream in = JdbcUtil.class.getClassLoader().getResourceAsStream("h2config.properties");
22             Properties prop = new Properties();
23             prop.load(in);
24             //创建数据库连接池
25             cp = JdbcConnectionPool.create(prop.getProperty("JDBC_URL"), prop.getProperty("USER"), prop.getProperty("PASSWORD"));
26         } catch (Exception e) {
27             System.out.println("连接池初始化异常");
28             e.printStackTrace();
29         }
30     }
31     
32     /**
33     * @Method: getConnection
34     * @Description:获取数据库连接
35     * @Anthor:孤傲苍狼
36     * @return
37     * @throws Exception
38     */ 
39     public static Connection getConnection() throws Exception{
40         return cp.getConnection();
41     }
42 
43     public static JdbcConnectionPool getCp() {
44         return cp;
45     }
46 }
```

　　h2config.properties的配置信息如下：

```
JDBC_URL=jdbc:h2:tcp://localhost/~/h2db
USER=gacl
PASSWORD=123
```

　　当web应用启动时，就会执行RegisterH2ExtFuncServlet这个Servlet中的init方法，init方法内部的处理就是通过JdbcUtil工具类获取一个H2的数据库连接，然后创建Statement对象，再由Statement对象批量执行SQL向H2数据库注册扩展函数。

　　RegisterH2ExtFuncServlet执行的过程中如果没有出现任何错误，那就说明所有的针对H2数据库的扩展函数都注册成功了，我们可以到H2的Console去验证一下上述的4个扩展函数，如下图所示：

![](https://images0.cnblogs.com/blog/289233/201412/290041385286375.png)

　　关于在Web应用中嵌入使用H2数据库，以及针对H2数据库函数的扩展的内容就讲解这么多了。









