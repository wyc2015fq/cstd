# Hightchart 实现 polar 雷达图 - z69183787的专栏 - CSDN博客
2015年08月22日 21:36:15[OkidoGreen](https://me.csdn.net/z69183787)阅读数：7647
**需要多引入一个highchart-more.js**
**推荐一个在线编辑的网站：http://code.hcharts.cn/demos/hhhhig**
```java
$(function () {
    $('#container').highcharts({
        chart: {
            polar: true,
            type: 'line'
        },
        title: {
            text: 'Budget vs spending',
            x: -80
        },
        pane: {
            size: '80%'
        },
        xAxis: {
            categories: ['Sales', 'Marketing', 'Development', 'Customer Support',
                    'Information Technology', 'Administration'],
            tickmarkPlacement: 'on',
            lineWidth: 0
        },
        yAxis: {
            gridLineInterpolation: 'polygon',
            lineWidth: 0,
            min: 0
        },
        tooltip: {
            shared: true,
            pointFormat: '<span style="color:{series.color}">{series.name}: <b>${point.y:,.0f}</b><br/>'
        },
        legend: {
            align: 'right',
            verticalAlign: 'top',
            y: 70,
            layout: 'vertical'
        },
        series: [{
            name: 'Allocated Budget',
            data: [43000, 19000, 60000, 35000, 17000, 10000],
            pointPlacement: 'on'
        }, {
            name: 'Actual Spending',
            data: [50000, 39000, 42000, 31000, 26000, 14000],
            pointPlacement: 'on'
        }]
    });
});
```
```java
var chart;
    $(document).ready(function () {
        chart = new Highcharts.Chart({
            chart: {
                padding:0,
                margin:0,
                renderTo: 'container',
                polar: true,
                type: 'area',
                //backgroundColor: '#F2F2F2'
                plotBackgroundImage:"http://imgd3.laoyuegou.com/mayday/img/dota-news/bg_gray.jpg",
            },
            title: {
                text: '',
                x: -80
            },
            credits:{
                enabled: false
            },
            exporting: {
                enabled: false
            },
            pane: {
                size: '85%',
                startAngle: 20,
                center: ['50%', '50%']
            },
            plotOptions: {
                area: {
                    fillOpacity: 0.30
                }
            },
            xAxis: {
                categories: ['输出', '生存', '推塔', '团战','发育'],
                tickmarkPlacement: 'on',
                lineWidth: 0,
                gridLineColor: '#D8D8D8',
                labels:{
                    style:{
                        color: '#B3B9C2',//B7F1E2
                    }
                },
                showFirstLabel:true
            },
            yAxis: {
                gridLineInterpolation: 'polygon',
                lineWidth: 0,
                min: 0,
                max: 15,
                gridLineWidth :1,
                gridLineColor: '#B3B9C2',
                plotBands: [{
                    color: '#F2F2F2',
                    borderColor:'#B3B9C2',
                    borderWidth:1,
                    from: 0,
                    to: 5
                }, {
                    color: '#F2F2F2',
                    borderColor:'#B3B9C2',
                    borderWidth:1,
                    from: 5,
                    to: 10
                }, {
                    color: '#F2F2F2',
                    borderColor:'#B3B9C2',
                    borderWidth:1,
                    from: 10,
                    to: 15
                }, {
                    color: '#F2F2F2',
                    borderColor:'#B3B9C2',
                    borderWidth:1,
                    from: 15,
                    to: 20
                }],
                labels:{
                    enabled: false
                }
            },
            tooltip: {
                useHTML: true,
                backgroundColor: '#FFFFFF',
                borderWidth: 0,
                style: {
                    color: 'gray',
                    padding: '10px'
                },
                headerFormat: '',
//                shared: true,
//                pointFormat: '<span style="color:{series.color}">{series.name}: <b>{point.y:,.0f}</b><br/>',
                // ["kdara"]=>    //输出
                // ["hdra"]=>     //团战
                // ["dra"]=>      //生存
                // ["gpmra"]=>    //发育 每分钟金钱
                // ["tdra"]=>     //推塔
                formatter: function(){
                    var s = '';
                    if(this.x == '输出')
                    {
                        return  "<p style='text-align: center;'>" + this.key + ": <span style='color:#f60'>" +  "B" + "</span></p></span>";
                    }
                    else if(this.x == '生存')
                    {
                        return "<p style='text-align: center;'>" +this.key + ": <span style='color:#f60'>" +  "A"+ "</span></p></span>";
                    }
                    else if(this.x == '推塔')
                    {
                        return "<p style='text-align: center;'>" +this.key + ": <span style='color:#f60'>" +  "B-"+ "</span></p></span>";
                    }
                    else if(this.x == '团战')
                    {
                        return "<p style='text-align: center;'>" +this.key + ": <span style='color:#f60'>" +  "B+"+ "</span></p></span>";
                    }
                    else if(this.x == '发育')
                    {
                        return "<p style='text-align: center;'>" +this.key + ": <span style='color:#f60'>" +  "A"+ "</span></p></span>";
                    }
                    return s;
                }
            },
            legend : {
              enabled: false
            },
            series: [{
                data: [{
                    name: '输出',
                    y: 6.1,
                    marker: {
                        symbol: 'url(http://imgd3.laoyuegou.com/static/img/app/dota2/icon_shuchu_dark.png)',
                        width: 13,
                        height: 13
                    }
                },{
                    name: '生存',
                    y: 9.5,
                    marker: {
                        symbol: 'url(http://imgd3.laoyuegou.com/static/img/app/dota2/icon_shengcun_dark.png)',
                        width: 13,
                        height: 13
                    }
                },{
                    name: '推塔',
                    y: 5,
                    marker: {
                        symbol: 'url(http://imgd3.laoyuegou.com/static/img/app/dota2/icon_tuita_dark.png)',
                        width: 13,
                        height: 13
                    }
                },{
                    name: '团战',
                    y: 7,
                    marker: {
                        symbol: 'url(http://imgd3.laoyuegou.com/static/img/app/dota2/icon_tuanzhan_dark.png)',
                        width: 13,
                        height: 13
                    }
                },{
                    name: '发育',
                    y: 9.5,
                    marker: {
                        symbol: 'url(http://imgd3.laoyuegou.com/static/img/app/dota2/icon_fayu_dark.png)',
                        width: 13,
                        height: 13
                    }
                }],
                pointPlacement: 'on',
                color: '#269783'
            } ]
        });
        chart.series[0].points[0].onMouseOver();
    });
```
