# Echart Stacked Line Chart简单代码 - 专注于数据挖掘算法研究和应用 - CSDN博客





2018年03月30日 08:54:04[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：393








```java
option = {
    title: {
        text: 'RMSE on LFM and SkillLFM'
    },
    tooltip: {
        trigger: 'axis'
    },
    legend: {
        data:['LFM_Funk-SVD','LFM_Bias-SVD','SkillLFM_SVD++','SkillLFM_Bias_SVD']
    },
    grid: {
        left: '3%',
        right: '4%',
        bottom: '3%',
        containLabel: true
    },
    toolbox: {
        feature: {
            saveAsImage: {}
        }
    },
    xAxis: {
        type: 'category',
        data: ['3','5','8','10'],
        name: 'k'
    },
    yAxis: {
        type: 'value',
        name: 'RMSE',
        min: 0.1,
        max: 0.2
    },
    series: [
        {
            name:'LFM_Funk-SVD',
            type:'line',
            data:[0.126224, 0.142596, 0.160834, 0.169922],
            itemStyle : { normal: {label : {show: true}}}
        },
        {
            name:'LFM_Bias-SVD',
            type:'line',
            data:[0.187339, 0.168075, 0.152611, 0.148874],
            itemStyle : { normal: {label : {show: true}}}
        },
        {
            name:'SkillLFM_SVD++',
            type:'line',
            data:[0.115101, 0.125955, 0.140908, 0.147782],
            itemStyle : { normal: {label : {show: true}}}
        },
        {
            name:'SkillLFM_Bias_SVD',
            type:'line',
            data:[0.114685, 0.117486, 0.120649, 0.121932],
            itemStyle : { normal: {label : {show: true}}}
        },
    ]
};
```

效果图：

![](https://img-blog.csdn.net/20180330085358602)




