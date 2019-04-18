# jquery weather 天气显示控件 - z69183787的专栏 - CSDN博客
2014年04月03日 14:31:50[OkidoGreen](https://me.csdn.net/z69183787)阅读数：6542
作者官方网站：
[http://lab.julying.com/weather/](http://lab.julying.com/weather/)
原理是使用 调用作者 新浪上的 api 返回天气各 参数 并集成jquery 显示。
关键js：
jquery.weather.build.js
jquery.weather.js
jquery.weather.js  该js中可更改 
//原型
Weather.prototype = {
中的方法，使显示页面自定义。
调用实例：
```java
<!DOCTYPE html>
<html>
<head>
<script type="text/javascript" src="../jquery/jquery-1.7.1.min.js"></script>
<script type="text/javascript">
$(document).ready(function (){ 
});
</script>
<meta charset="utf-8">
<title>天气预报</title>
<style type="text/css">
body{margin: 0px; padding: 0px;background-color:transparent;}
#weatherparentbox {margin: 0px;padding: 0px;color: #FFFFFF;font: 12px/1.5 'Helvetica Neue','Microsoft YaHei',Arial,'Liberation Sans',FreeSans,sans-serif;}
#weatherparentbox dt {
    font-weight: normal;
}
#weatherparentbox dt {
    height: 62px;
    line-height: 60px;
    padding-left: 40px;
}
#weatherparentbox h1 {
    font-size: 36px;
    margin-top: 0px;
}
#weatherparentbox h1 font{
    font-size: 24px;
    margin-top: 0px;
}
#weatherparentbox span {
    display: block;
    font-size: 12px;
}
#weatherparentbox  dd {
cursor:pointer;
position:absolute;
top:63px;
margin:0px;
    line-height: 24px;
    font-size: 14px;
    height: 25px;
    background-color: #000000;
    opacity: 0.7;
    width:100%;
}
#weatherparentbox  dd:HOVER {
top:37px;
height: 50px;
}
#weatherparentbox  dd:HOVER .wday{
width:284px;
}
#weatherparentbox  dd:HOVER .wnight{
width:284px;
float:left;
padding-left: 10px;
}
#weatherparentbox  a {
    color: #FFFFFF;
    text-decoration: none;
}
#weatherparentbox .wday{width:142px; padding-left: 10px;}
#weatherparentbox .wnight{width:142px;padding-right: 10px;}
.clearfix:before, .clearfix:after {
    content: ".";
    display: block;
    font-size: 0;
    height: 0;
    line-height: 0;
    overflow: hidden;
    visibility: hidden;
    width: 0;
}
.clearfix:after {
    clear: both;
}
.clearfix:before, .clearfix:after {
    content: ".";
    display: block;
    font-size: 0;
    height: 0;
    line-height: 0;
    overflow: hidden;
    visibility: hidden;
    width: 0;
}
.fr{float:right;}
.fl{float:left;}
.wdate{position: absolute;top:-16px;right:10px;}
.wcity{position: absolute;top:-2px;right:10px;}
</style>
</head>
<body>
<div id="weatherparentbox">
	<!-- <dt><h1>22℃</h1>
	<span class="wdate">2013-10-11</span>
	<span class="wcity">南昌</span>
	</dt>
	<dd class="clearfix">
		<span class="fl">白天:多云 南风3-4级  夜晚:多云 南风3-4级</span>
		<span class="fr">夜晚:多云 南风3-4级</span>
	</dd> -->
</div>
<!-- 	<dl id="weatherparentbox" class="wether"
		style="position: relative; z-index: 1;">
		<dt>
			<h1>22℃</h1>
		</dt>
		<dd class="clearfix">
			<span class="fl">2013-10-11 多云 南风3-4级</span> <span class="fr"><a
				href="#">南昌<i class="city">12</i></a></span>
		</dd>
		<div class="julying-weather">
			<div class="cloudy sky sky-0" style="left: 60.2083px;">
				<div class="child-0 child">
					<div class="weather-info" style="display: none;">
						<h2>
							上海<span>2013-10-24</span>
						</h2>
						<div class="weather-show">
							<div class="day_weather">
								<span class="weather">白天 : 多云</span>/<span class="temp">21°C</span>
							</div>
							<div class="night_weather">
								<span class="weather">夜晚 : 多云</span>/<span class="temp">13°C</span>
							</div>
						</div>
						<div class="julying-weather-copy">
							<a title="©王子墨" target="_blank"
								href="http://julying.com/lab/weather/">julying</a>
						</div><script defer src="http://jquery-weather.julying.com/v3/jquery.weather.build.min.js?parentbox=body&moveArea=all&zIndex=1&move=1&drag=1&autoDrop=0&styleSize=big&style=default&area=assign&city=%E5%8D%97%E6%98%8C"></script>
					</div>
				</div>
			</div>
		</div>
	</dl> -->
	<script defer src="js/jquery.weather.build.js?parentbox=weatherparentbox&moveArea=limit&zIndex=1&move=1&drag=0&autoDrop=0&styleSize=big&style=default&areas=assign&city=上海"></script>
	
</body>
</html>
```
jquery.weather.build.js 中 include 了 weather.js
更改 weather.js 中的init  方法 即可更改显示样式，取消原来点击图表才显示具体数据的样式，直接显示数据：
```java
init : function(config){
			//天气预报信息，此数据来源于中国气象网
			var weatherInfo = this.opts.weatherData || {},
				//IE、Firefox 旧版本 对于字符串拼接很慢，用数组很快 
				html = [],
				//当前天气子节点数量
				skyState_child_length = ( this.skyState || '' ).length ,
				templeteHtml = '<div><h2>{#city#}<span>{#date#}</span></h2><div class="weather-show"><div><span>{#day#} : {#day_weather#}</span>/<span>{#day_temp#}{#temp#}</span></div><div ><span class="weather">{#night#} : {#night_weather#}</span>/<span class="temp">{#night_temp#}{#temp#}</span></div></div><div class="julying-weather-copy"><a href="http://julying.com/lab/weather/" target="_blank" title="©{#wangzimo#}">julying</a></div></div>',
				i = 0
			;
			for( ; i < skyState_child_length ; i++){
				html.push('<div class="julying-weather"><div class="'+ this.opts.sky +' sky sky-'+i+'"><div class="child-'+i+' child">');			
				if( weatherInfo.city ){
				
				}
				html.push('</div></div></div>');
			};
			
			this.$element.append(templete( templeteHtml, {
						'{#city#}' : weatherInfo.city,
						'{#date#}' : weatherInfo.date,
						'{#day#}' : config.lang.day ,
						'{#day_weather#}' : weatherInfo.day_weather ,
						'{#day_temp#}' : weatherInfo.day_temp,
						'{#temp#}' : config.lang.temp,
						'{#day_wind#}' : weatherInfo.day_wind,
						'{#night_weather#}' : weatherInfo.night_weather,
						'{#night#}' : config.lang.night ,
						'{#night_temp#}' : weatherInfo.night_temp,
						'{#wangzimo#}' : config.lang.wangzimo 
					})  );	
			this.$element.append( html.join('') );	
			html = null ;	
			this.$weatherArea = this.$element.find('div.julying-weather');	
			this.$weather = this.$element.find('.sky');	
			
			this.moveIcon();
			this.creatDrop();
			if( this.opts.drag ){
				this.dragWeather();
			}
			return this ;
		},
```
