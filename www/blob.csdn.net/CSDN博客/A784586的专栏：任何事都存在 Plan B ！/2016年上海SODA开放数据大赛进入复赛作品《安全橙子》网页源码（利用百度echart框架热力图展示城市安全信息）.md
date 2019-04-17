# 2016年上海SODA开放数据大赛进入复赛作品《安全橙子》网页源码（利用百度echart框架热力图展示城市安全信息） - A784586的专栏：任何事都存在 Plan B ！ - CSDN博客





2017年03月11日 13:37:33[QuJack](https://me.csdn.net/A784586)阅读数：1826








2016年上海SODA开放数据大赛进入复赛作品《安全橙子》：




效果：




![](https://img-blog.csdn.net/20170311133419779?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvQTc4NDU4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)![](https://img-blog.csdn.net/20170311133434617?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvQTc4NDU4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)








网页展示源码：



```
<!DOCTYPE html>
<html>
<head>
<meta charset="utf-8">
<script src="./jquery.min.js"></script>
<style>
*{margin: 0;padding: 0;box-sizing: border-box;}
.js-tab{width: 100%;}
.js-tab li.js-click{float: left;width:50%;height: 50px;border: #000 solid 1px;color: #000;list-style: none;cursor: pointer;;color: #fff;background-color: #101734;}
.js-tab li.js-click2{float: right;width:50%;height: 50px;border: #000 solid 1px;color: #000;background-color:#5E5E5E;list-style: none;cursor: pointer;}
.js-div{border: #000 solid 1px;width: 100%;height: 970px;}
.display{display: none;}


	
	.leftdiv{
			position:absolute;
			z-index:20;
			bottom:0%;
			right:88%;
		   background:#4A4752;
		}
		
	.container{
		position:absolute;
			z-index:20;
			bottom:0%;
			right:0%;
			
		}
		
	.leftdiv2{
			position:absolute;
			z-index:20;
			bottom:0%;
			right:88%;
		   background:#4A4752;<!--url('./img/xing2.jpg');-->
		}
		
	.container2{
		position:absolute;
			z-index:20;
			top:0%;
			bottom:0%;
			left:100%;
		}
		
#logo1{
				position:relative;
				width:100%;
				height:70px;
				z-index:100;
				top:0px;
				left:0px;
				text-align:left;
			}
		#logo1 img{
				width:50px;
				height:61px;
			}
			
			
#logo2{
				position:relative;
				width:100%;
				height:70px;
				z-index:100;
				top:0px;
				left:0px;
				text-align:left;
			}
		#logo2 img{
				width:50px;
				height:61px;
			}
			
#logo3{
				position:relative;
				width:100%;
				height:70px;
				z-index:100;
				top:0px;
				left:0px;
				text-align:left;
			}
		#logo3 img{
				width:50px;
				height:61px;
			}
			
#logo12{
				position:relative;
				width:100%;
				height:70px;
				z-index:100;
				top:0px;
				left:0px;
				text-align:left;
			}
		#logo12 img{
				width:50px;
				height:61px;
			}
			
			
#logo22{
				position:relative;
				width:100%;
				height:70px;
				z-index:100;
				top:0px;
				left:0px;
				text-align:left;
			}
		#logo22 img{
				width:50px;
				height:61px;
			}
			
#logo32{
				position:relative;
				width:100%;
				height:70px;
				z-index:100;
				top:0px;
				left:0px;
				text-align:left;
			}
		#logo32 img{
				width:50px;
				height:61px;
			}
</style>
</head>
<body>
<ul class="js-tab clearfix" id="js-tab">
    <li class="js-click"> <p align="left" style="font-family:Microsoft YaHei;color:#4ED3D3;font-size:28px; margin: 0;line-height:42px;">  城市公共安全综合态势图</p></li>
    <li class="js-click2"> <p align="right" style="font-family:Microsoft YaHei;color:#4ED3D3;font-size:28px; margin: 0;line-height:42px"><input size="45" style="font-family:Microsoft YaHei;width:180px;height:30px;" type="text" class="content" id="inputcontent" name="q" autocomplete="off" value="上海站" placeholder="输入感兴趣位置..."> <button style="font-family:Microsoft YaHei;width:100px;height: 30px;background:#1169EE;color:#FFFFFF;border:0;border-radius:20px ;cursor:pointer;font-size:14px;margin-right:15px;" type="submit" class="search-button"  onclick="searchByStationName();"><span style="font-family:Microsoft YaHei;" class="hide-text">搜索</span><span class="sprite-global-icon-magnifier-dark"></span></button>     城市公共安全信息检索系统  </p></li>
</ul>



<div class="js-div" id="js-div">
		
		<div class="option1">
		
			<div class="leftdiv" style="height:95%;width:12%" >	
			
				 <div style="font-family:Microsoft YaHei;width:100%;height:40%;float:left">
						<div class="logo1" id="logo1" style="background:#3399cc;"><img style="vertical-align: middle;" src="img/logo1.png"/><span align="center" style="font-family:Microsoft YaHei;color:#FFFFFF;font-size:25px; margin: 0;line-height:28px;vertical-align: middle;"> 社会治安</span></div>
						 <p align="center">
						 </br>
						<input style="font-family:Microsoft YaHei;width:150px;height: 40px;background:#BE0E31;color:#FFFFFF;border:0;font-size:14px;margin-right:15px;" type="button" value="派出所出警" class="btn" id="chujing"/></br></br>
						<input style="font-family:Microsoft YaHei;width:150px;height: 40px;background:#BE0E31;color:#FFFFFF;border:0;font-size:14px;margin-right:15px;" type="button" value="交通事故" class="btn" id="jiaotong"/></br></br>
						<input style="font-family:Microsoft YaHei;width:150px;height: 40px;background:#BE0E31;color:#FFFFFF;border:0;font-size:14px;margin-right:15px;" type="button" value="企业不良记录" class="btn" id="qiyebuliang"/></br></br>
						<input style="font-family:Microsoft YaHei;width:150px;height: 40px;background:#BE0E31;color:#FFFFFF;border:0;font-size:14px;margin-right:15px;" type="button" value="乱晾晒车乱停" class="btn" id="luanting"/></br></br>
						</p>
				 </div>
				 
				 <div style="width:100%;height:25%;float:left">
						<div class="logo2" id="logo2" style="font-family:Microsoft YaHei;background:#39383F;"><img style="vertical-align: middle;" src="img/logo2.png"/><span align="center" style="color:#FFFFFF;font-size:25px; margin: 0;line-height:28px;vertical-align: middle;"> 食品安全</span></div>
						 <p align="center">
						 </br>
						<input style="font-family:Microsoft YaHei;width:150px;height: 40px;background:#15B8B8;color:#FFFFFF;border:0;font-size:14px;margin-right:15px;" type="button" value="餐饮处罚" class="btn2" id="canyin"/></br></br>
						<input style="font-family:Microsoft YaHei;width:150px;height: 40px;background:#15B8B8;color:#FFFFFF;border:0;font-size:14px;margin-right:15px;" type="button" value="摆摊占道" class="btn2" id="baitan"/></br></br>
						</p>
				 </div>
				
				 
				 <div style="width:100%;height:35%;float:left">
				        <div class="logo3" id="logo3" style="font-family:Microsoft YaHei;background:#39383F;"><img style="vertical-align: middle;" src="img/logo3.png"/><span align="center" style="font-family:Microsoft YaHei;color:#FFFFFF;font-size:25px; margin: 0;line-height:28px;vertical-align: middle;"> 环境卫生</span></div>
						 <p align="center">
						 </br>
						<input style="font-family:Microsoft YaHei;width:150px;height: 40px;background:#0C2DAF;color:#FFFFFF;border:0;font-size:14px;margin-right:15px;" type="button" value="重污染" class="btn3" id="wuran"/></br></br>
						<input style="font-family:Microsoft YaHei;width:150px;height: 40px;background:#0C2DAF;color:#FFFFFF;border:0;font-size:14px;margin-right:15px;" type="button" value="暴露垃圾乱涂写" class="btn3" id="baolvlaji"/></br></br>
						<input style="font-family:Microsoft YaHei;width:150px;height: 40px;background:#0C2DAF;color:#FFFFFF;border:0;font-size:14px;margin-right:15px;" type="button" value="建筑工地污染" class="btn3" id="gongdi"/></br></br>
						</p>
				 </div>
				 
				 
			</div>
		
			<div class="container" id="container" style="height:95%; width:88%">

			</div>

			
		</div>

		<div class="option2" style="display: none;">
			<div class="leftdiv2" id="leftdiv2" style="font-family:Microsoft YaHei;height:95%;width:12%" >	
			 <div style="font-family:Microsoft YaHei;width:100%;height:40%;float:left">
						<div class="logo12" id="logo12" style="font-family:Microsoft YaHei;background:#39383F;"><img style="vertical-align: middle;" src="img/logo1.png"/><span align="center" style="color:#FFFFFF;font-size:25px; margin: 0;line-height:28px;vertical-align: middle;"> 社会治安</span></div>
						 <p align="center">
						 </br>
						<input style="font-family:Microsoft YaHei;width:150px;height: 40px;background:#BE0E31;color:#FFFFFF;border:0;font-size:14px;margin-right:15px;" type="button" value="派出所出警" class="btn4" id="chujing2"/><input type="checkbox" name="category" value="0" checked="true" /></br></br>
						<input style="font-family:Microsoft YaHei;width:150px;height: 40px;background:#BE0E31;color:#FFFFFF;border:0;font-size:14px;margin-right:15px;" type="button" value="交通事故" class="btn4" id="jiaotong2"/><input type="checkbox" name="category" value="1" checked="true" /></br></br>
						<input style="font-family:Microsoft YaHei;width:150px;height: 40px;background:#BE0E31;color:#FFFFFF;border:0;font-size:14px;margin-right:15px;" type="button" value="企业不良记录" class="btn4" id="qiyebuliang2"/><input type="checkbox" name="category" value="2" /></br></br>
						<input style="font-family:Microsoft YaHei;width:150px;height: 40px;background:#BE0E31;color:#FFFFFF;border:0;font-size:14px;margin-right:15px;" type="button" value="乱晾晒车乱停" class="btn4" id="luanting2"/><input type="checkbox" name="category" value="3" /></br></br>
						</p>
				 </div>
				 
				 <div style="font-family:Microsoft YaHei;width:100%;height:25%;float:left">
						<div class="logo22" id="logo22" style="font-family:Microsoft YaHei;background:#39383F;"><img style="vertical-align: middle;" src="img/logo2.png"/><span align="center" style="font-family:Microsoft YaHei;color:#FFFFFF;font-size:25px; margin: 0;line-height:28px;vertical-align: middle;"> 食品安全</span></div>
						 <p align="center">
						 </br>
						<input style="font-family:Microsoft YaHei;width:150px;height: 40px;background:#15B8B8;color:#FFFFFF;border:0;font-size:14px;margin-right:15px;" type="button" value="餐饮处罚" class="btn5" id="canyin2"/><input type="checkbox" name="category" value="4" /></br></br>
						<input style="font-family:Microsoft YaHei;width:150px;height: 40px;background:#15B8B8;color:#FFFFFF;border:0;font-size:14px;margin-right:15px;" type="button" value="摆摊占道" class="btn5" id="baitan2"/><input type="checkbox" name="category" value="5" /></br></br>
						</p>
				 </div>
				
				 
				 <div style="font-family:Microsoft YaHei;width:100%;height:35%;float:left">
				        <div class="logo32" id="logo32" style="font-family:Microsoft YaHei;background:#39383F;"><img style="vertical-align: middle;" src="img/logo3.png"/><span align="center" style="color:#FFFFFF;font-size:25px; margin: 0;line-height:28px;vertical-align: middle;"> 环境卫生</span></div>
						 <p align="center">
						 </br>
						<input style="font-family:Microsoft YaHei;width:150px;height: 40px;background:#0C2DAF;color:#FFFFFF;border:0;font-size:14px;margin-right:15px;" type="button" value="重污染" class="btn6" id="wuran2"/><input type="checkbox" name="category" value="6" /></br></br>
						<input style="font-family:Microsoft YaHei;width:150px;height: 40px;background:#0C2DAF;color:#FFFFFF;border:0;font-size:14px;margin-right:15px;" type="button" value="暴露垃圾乱涂写" class="btn6" id="baolvlaji2"/><input type="checkbox" name="category" value="7" /></br></br>
						<input style="font-family:Microsoft YaHei;width:150px;height: 40px;background:#0C2DAF;color:#FFFFFF;border:0;font-size:14px;margin-right:15px;" type="button" value="建筑工地污染" class="btn6" id="gongdi2"/><input type="checkbox" name="category" value="8" /></br></br>
						
						<input style="font-family:Microsoft YaHei;width:50px;height: 20px;background:#AA5577;color:#FFFFFF;border:0;font-size:14px;margin-right:15px;" type="button" value="全选" class="btn7" id="selectAll"/> 
						<input style="font-family:Microsoft YaHei;width:50px;height: 20px;background:#AA5577;color:#FFFFFF;border:0;font-size:14px;margin-right:15px;" type="button"  value="全不选" class="btn7" id="unSelect"/>   
						<input style="font-family:Microsoft YaHei;width:50px;height: 20px;background:#AA5577;color:#FFFFFF;border:0;font-size:14px;margin-right:15px;" type="button"  value="反选" class="btn7" id="reverse"/>
						
						</p>
						
						
				 </div>
				 
			<div class="container2" id="container2" style="height:100%; width:800%">
				
			</div>
			
			
		</div>

</div>





<!-- 引入 js -->
      <script type="text/javascript" src="http://apps.bdimg.com/libs/jquery/2.1.4/jquery.min.js"></script>
	  
       <script type="text/javascript" src="http://echarts.baidu.com/gallery/vendors/echarts/echarts-all-3.js"></script>
       <script type="text/javascript" src="http://echarts.baidu.com/gallery/vendors/echarts/extension/dataTool.min.js"></script>
       <script type="text/javascript" src="http://echarts.baidu.com/gallery/vendors/echarts/map/js/china.js"></script>
       <script type="text/javascript" src="http://echarts.baidu.com/gallery/vendors/echarts/map/js/world.js"></script>
       <script type="text/javascript" src="http://api.map.baidu.com/api?v=2.0&ak=iYwyAuGoB5PMQUewkUH30cmgxY05VMrw">//ZUONbpqGBsYGXNIYHicvbAbM</script>
       <script type="text/javascript" src="http://echarts.baidu.com/gallery/vendors/echarts/extension/bmap.min.js"></script>
<!-- End -->	   
	   
<script type="text/javascript">	
/////////////////////////////////////
//option2 jquery监控选择按钮
$(function () { 
	//全选   
    $("#selectAll").click(function () { 
         $("#leftdiv2 :checkbox,#all").prop("checked", true);   
    });   
    //全不选 
    $("#unSelect").click(function () {   
         $("#leftdiv2 :checkbox,#all").prop("checked", false);   
    });   
    //反选  
    $("#reverse").click(function () {  
         $("#leftdiv2 :checkbox").each(function () {   
              $(this).prop("checked", !$(this).prop("checked"));   
         }); 
         //allchk(); 
    }); 
})
//searchByStationName();
option2();
function option2() {
//初始化地图
 var map = new BMap.Map("container2");
    map.centerAndZoom("上海", 12);
    map.enableScrollWheelZoom();    //启用滚轮放大缩小，默认禁用
    map.enableContinuousZoom();    //启用地图惯性拖拽，默认禁用
    map.addControl(new BMap.NavigationControl());  //添加默认缩放平移控件
    map.addControl(new BMap.OverviewMapControl()); //添加默认缩略地图控件
    map.addControl(new BMap.OverviewMapControl({ isOpen: true, anchor: BMAP_ANCHOR_BOTTOM_RIGHT }));   //右下角，打开
    var localSearch = new BMap.LocalSearch(map);
    localSearch.enableAutoViewport(); //允许自动调节窗体大小
	map.setMapStyle({style:'light'});//midnight bluish
}

function searchByStationName() {
	//初始化地图
 var map = new BMap.Map("container2");
    map.centerAndZoom("上海", 12);
    map.enableScrollWheelZoom();    //启用滚轮放大缩小，默认禁用
    map.enableContinuousZoom();    //启用地图惯性拖拽，默认禁用
    map.addControl(new BMap.NavigationControl());  //添加默认缩放平移控件
    map.addControl(new BMap.OverviewMapControl()); //添加默认缩略地图控件
    map.addControl(new BMap.OverviewMapControl({ isOpen: true, anchor: BMAP_ANCHOR_BOTTOM_RIGHT }));   //右下角，打开
    var localSearch = new BMap.LocalSearch(map);
    localSearch.enableAutoViewport(); //允许自动调节窗体大小
	map.setMapStyle({style:'light'});//midnight bluish
	
    map.clearOverlays();//清空原来的标注
    var keyword = document.getElementById("inputcontent").value;
    localSearch.setSearchCompleteCallback(function (searchResult) {
        var poi = searchResult.getPoi(0);
        //document.getElementById("result_").value = poi.point.lng + "," + poi.point.lat;
///////////////////////////////得到复选框内容////////////////////////////////////////
		var arr = new Array();                
        var items = document.getElementsByName("category");                 
        for (i = 0; i < items.length; i++) {                    
            if (items[i].checked) {                        
                arr.push(items[i].value);                    
            }                
        }                 
        //alert("选择的个数为：" + arr[1]);            
  
////////////////////////////////////////////////////////////////
		
		Find("chaxun",poi.point.lng,poi.point.lat,arr,map);
		
        map.centerAndZoom(poi.point, 14);
		
		var myIcon = new BMap.Icon("./img/marker01.png",//"http://api.map.baidu.com/img/markers.png",  
                        new BMap.Size(23, 45)//25, {  
                          //  offset: new BMap.Size(10,25),  
                          //  imageOffset: new BMap.Size(0, 0 - index * 25)  
                        //}
						);  
						
        var marker = new BMap.Marker(new BMap.Point(poi.point.lng, poi.point.lat));  // 创建标注，为要查询的地方对应的经纬度
        map.addOverlay(marker);
        var content = "查询位置："+document.getElementById("inputcontent").value + "<br/><br/>经度：" + poi.point.lng + "<br/>纬度：" + poi.point.lat;
        var infoWindow = new BMap.InfoWindow("<p style='font-size:14px;'>" + content + "</p>");
        marker.addEventListener("click", function () { this.openInfoWindow(infoWindow); });
        // marker.setAnimation(BMAP_ANIMATION_BOUNCE); //跳动的动画
    });
    localSearch.search(keyword);
} 
/////////////////////////////

function Find(val,lng,lat,arr,map) {
var url='data/'+val+'.json';


$.get(url, function (data) {
//alert(data.length);
for(var k=0; k < arr.length; k++)
{
	//alert(arr[k]);
	j=arr[k];

	for (var i = 0; i < data[j].length; i++)
	{ 
					//在地图上绘制点  
									
		if(data[j][i].coord[0]>=lng-0.01 && data[j][i].coord[0]<=lng+0.01 && data[j][i].coord[1]>=lat-0.01 && data[j][i].coord[1]<=lat+0.01){
		
                        var p0 = data[j][i].coord[0];  //points[i][0];
                        var p1 = data[j][i].coord[1];  //points[i][1];
                        var maker = addMarker(new window.BMap.Point(p0, p1), j); 
                        addInfoWindow(maker, data[j][i].type, data[j][i].elevation );  
						}						
   }  
}        
   
				// 添加标注  
                function addMarker(point, index) {  
                    var myIcon = new BMap.Icon("http://api.map.baidu.com/img/markers.png",  
                       new BMap.Size(23, 25),{  
                            offset: new BMap.Size(10, 25),  
                            imageOffset: new BMap.Size(0, 0 - index * 25)  
                        }
						);  
                    var marker = new BMap.Marker(point, { icon: myIcon });  
                    map.addOverlay(marker);  
                    return marker;  
                } 				
  
            // 添加信息窗口  
            function addInfoWindow(marker,type,elevation) {  
                    //pop弹窗标题  
                    var title = '<div style="font-weight:bold;color:#CE5521;font-size:14px">' + type + '</div>'; // 
                    //pop弹窗信息  
                    var html = [];  
                    html.push('<table cellspacing="0" style="table-layout:fixed;width:100%;font:12px arial,simsun,sans-serif"><tbody>');  
                    html.push('<tr>');  
                    html.push('<td style="vertical-align:top;line-height:16px;width:38px;white-space:nowrap;word-break:keep-all">频率:</td>');  
                    html.push('<td style="vertical-align:top;line-height:16px">' + elevation + ' </td>'); // 
                    html.push('</tr>');  
                    html.push('</tbody></table>');  
                    var infoWindow = new BMap.InfoWindow(html.join(""), { title: title, width: 200 });  
  
                    var openInfoWinFun = function () {  
                        marker.openInfoWindow(infoWindow);  
                    };  
                    marker.addEventListener("click", openInfoWinFun);  
                    return openInfoWinFun;  
                  
		//var marker = new BMap.Marker(new BMap.Point(data[0][i].coord[0],data[0][i].coord[1]));  // 创建标注，为要查询的地方对应的经纬度
        //map.addOverlay(marker);
		//alert(data[0][i].type);
		//var content =data[0][i].type+"<br/>" + "案件数量：" + data[0][i].elevation;
        //var infoWindow = new BMap.InfoWindow("<p style='font-size:14px;'>" + content + "</p>");
        //marker.addEventListener("click", function () { this.openInfoWindow(infoWindow); });
		}
		})
		}
////////////////////////////////////////





//option1刷新浏览器默认数据设置
	var val="shehuizhian_juhe"
	Time(val);

function Time(val) {
var dom = document.getElementById("container");
var myChart = echarts.init(dom);

var app = {};
option = null;
app.title = '热力图与标注';

var url='data/'+val+'.json';

//针对不同数据，设置符号大小
var maxpoints=0;
var pointsize=0;
var zoomsize=0;
var lonlat=[121.476213,31.137046];
var beishu=1;
if(val=="shehuizhian_juhe"){
	 maxpoints=2;
	 pointsize=5;
	 zoomsize=12;
	 var beishu=2;
}else if(val=="shipinanquan"){
	 maxpoints=2;
	 pointsize=5;
	 zoomsize=12;
	 beishu=0.5;
}else if(val=="huanjingweisheng1"){
	 maxpoints=2;
	 pointsize=5;
	 zoomsize=12;
	 beishu=1;
}else if(val=="gongdi"){
	 maxpoints=2;
	 pointsize=5;
	 zoomsize=12;
	 beishu=0.5;
}else if(val=="wuranyuan"){
	 maxpoints=2;
	 pointsize=5;
	 zoomsize=10;
	 beishu=0.5;
}else if(val=="wg_huanjing"){
	 maxpoints=2;
	 pointsize=2;
	 zoomsize=13;
	 beishu=1;
	 lonlat=[121.605569,31.245444];
}else if(val=="wg_shipin"){
	 maxpoints=2;
	 pointsize=2;
	 zoomsize=13;
	 beishu=1;
	 lonlat=[121.605569,31.245444];
}else if(val=="wg_shehui"){
	 maxpoints=2;
	 pointsize=2;
	 zoomsize=13;
	 beishu=1;
	 lonlat=[121.605569,31.245444];
}

//alert(url);
$.get(url, function (data) {
//alert(data);
	//[[{"coord":[120.14322240845,30.236064370321],"elevation":21},{}],[]]
	//重复来数据的时候清空points
	//var points=[[{"coord":[120.14322240845,30.236064370321],"elevation":21}]];
	
    var points = [].concat.apply([], data.map(function (track) {
        return track.map(function (seg) {
            return seg.coord.concat([seg.elevation*beishu]);
        });
    }));
	
	//var points=[[121.485807,31.223243,1,"title","address 地址"],[121.418146,31.323953,2,"abc","上海大学"],[121.595795,31.255323,3,"abc","杨高中路"],[121.220375,31.10505,4,"abc","欢乐谷"],[121.458785,31.236953,5,"abc","上海植物园"],[121.442113,31.230284,6,"abc","公园"],[121.447323,31.13149,7,"abc","aaaaaaa"],[121.482824,31.237046,8,"abc","市中心"]];
	//alert(points);
    myChart.setOption(
	option = {

        animation: false,
        bmap: {
			// 百度地图中心经纬度
            center: lonlat,
			// 百度地图缩放
            zoom: zoomsize,
			 // 是否开启拖拽缩放，可以只设置 'scale' 或者 'move'
            roam: true
			// 百度地图的自定义样式，见 http://developer.baidu.com/map/jsdevelop-11.htm
           // mapStyle: {}
        },
        visualMap: {
            show: false,
            top: 'top',
            min: 0,
            max: maxpoints,//20  4
            seriesIndex: 0,
            calculable: true,
            inRange: {
                color: ['blue', 'green', 'yellow', 'red']
            }
        },
        series: [{
            type: 'heatmap',
			// 使用百度地图坐标系
           // 数据格式跟在 geo 坐标系上一样，每一项都是 [经度，纬度，数值大小，其它维度...]
            coordinateSystem: 'bmap',
            data: points,
            pointSize: pointsize,//5  15
            blurSize: 15
        }]
	
	});
	
	
	
	
	
    if (!app.inNode) {
        // 添加百度地图插件
        var bmap = myChart.getModel().getComponent('bmap').getBMap();
        bmap.addControl(new BMap.MapTypeControl());
		bmap.setMapStyle({style:'light'});      
    } 
});



if (option && typeof option === "object") {
    myChart.setOption(option, true);
}

}
//////////////////////上面是无个性化时间轴公共模块////////////////////////////////

////////////////////////////下面是企业不良记录模块//////////////////////////
function Timeqiye(val) {
var dom = document.getElementById("container");
var myChart = echarts.init(dom);

var app = {};
option = null;
app.title = '热力图与标注';
var url='data/'+val+'.json';

//针对不同数据，设置符号大小
var maxpoints=0;
var pointsize=0;
if(val=="qiyebuliang"){
	 maxpoints=3;
	 pointsize=3;
}
//alert(url);
//alert(maxpoints);
//alert(pointsize);
$.get(url, function (data) {

	//[[{"coord":[120.14322240845,30.236064370321],"elevation":21},{}],[]]
	
	//重复来数据的时候清空points
	var points=new Array(4);
	
	for (var i = 0; i < data.length; i++) { 
		var dt=data[i]
		 points[i] = [].concat.apply([], dt.map(function (track){
			return track.map(function (seg) {
				return seg.coord.concat([1]);
        });
    }));
		//alert(i);
		//alert(points[i]);
	}
	
	//alert(points);
	//var points=[[121.485807,31.223243,1,"title","address 地址"],[121.418146,31.323953,2,"abc","上海大学"],[121.595795,31.255323,3,"abc","杨高中路"],[121.220375,31.10505,4,"abc","欢乐谷"],[121.458785,31.236953,5,"abc","上海植物园"],[121.442113,31.230284,6,"abc","公园"],[121.447323,31.13149,7,"abc","aaaaaaa"],[121.482824,31.237046,8,"abc","市中心"]];
	//alert(points);
    myChart.setOption(option = {
	
	baseOption: {
		timeline: {
			type: "number",
			label: {
				formatter: function(s) {
					return s;
				},
				textStyle: {
					color: '#fff',
					fontSize: 5
				}
			},
			data: [{
                    value: '01',
                    tooltip: {
                        formatter: '{b} 2016年01月'
                    },
                    symbol: 'diamond',
                    symbolSize: 20
                },{
                    value: '02',
                    tooltip: {
                        formatter: '{b} 2016年02月'
                    },
                    symbol: 'diamond',
                    symbolSize: 20
                },{
                    value: '03',
                    tooltip: {
                        formatter: '{b} 2016年03月'
                    },
                    symbol: 'diamond',
                    symbolSize: 20
                },{
                    value: '04',
                    tooltip: {
                        formatter: '{b} 2016年04月'
                    },
                    symbol: 'diamond',
                    symbolSize: 20
                }],
			autoPlay: true,
			playInterval: 1000
		},
		
        animation: false,
		tooltip: {},
        bmap: {
			// 百度地图中心经纬度
            center: [121.476213,31.137046],
			// 百度地图缩放
            zoom: 11,
			 // 是否开启拖拽缩放，可以只设置 'scale' 或者 'move'
            roam: true
			// 百度地图的自定义样式，见 http://developer.baidu.com/map/jsdevelop-11.htm
           // mapStyle: {}
        },
        visualMap: {
            show: false,
            top: 'top',
            min: 0,
            max: maxpoints,//20  4
            seriesIndex: 0,
            calculable: true,
            inRange: {
                color: ['blue', 'green', 'yellow', 'red']
            }
        },
        series: [{
            type: 'heatmap',
			// 使用百度地图坐标系
           // 数据格式跟在 geo 坐标系上一样，每一项都是 [经度，纬度，数值大小，其它维度...]
            coordinateSystem: 'bmap',
            //data: points,
           // pointSize: pointsize,//5  15
            //blurSize: 25
        }],
    },
	//配对时间轴变化初始化数据
	options:[{     
        series: [{
            data: points[0],
			pointSize: 4,//5  15
            blurSize: 15
        }]
	},
	{
        series: [{
            data: points[1],
			pointSize: 4,//5  15
            blurSize: 15
        }]
	},
	{
        series: [{
            data: points[2],
			pointSize: 4,//5  15
            blurSize: 15
        }]
	},
	{
        series: [{
            data: points[3],
			pointSize: 4,//5  15
            blurSize: 15
        }]
	}
		]
	});
	
	
    if (!app.inNode) {
        // 添加百度地图插件
        var bmap = myChart.getModel().getComponent('bmap').getBMap();
        bmap.addControl(new BMap.MapTypeControl());
		bmap.setMapStyle({style:'light'});
		
	var markerArr = [    
                  //  { title: "名称：人民广场", point: "121.484297,31.238929", address: "上海市人民广场", tel: "12306" },  
                   // { title: "名称：上海物资大厦", point: "121.488106,31.245536", address: "上海物资大厦 ", tel: "18500000000" },  
                   // { title: "名称：上海绍兴公园", point: "121.472296,31.216202", address: "上海市博物馆", tel: "18500000000" },  
                   // { title: "名称：黄埔中心医院", point: "121.494861,31.239856", address: "上海市黄埔中心医院", tel: "18500000000" }  
				   
		{
            "chufa": "已要求上海石化督促芳烃部及时整改。", 
            "company": "中国石化上海石油化工股份有限公司", 
            "coord": [
                121.487899, 
                31.249162
            ]
        }, 
        {
            "chufa": "责令整改", 
            "company": "上海赛科石油化工有限责任公司", 
            "coord": [
                121.477429, 
                30.804188
            ]
        }, 
        {
            "chufa": "已要求上海石化尽快落实91号令的相关要求。", 
            "company": "中国石化上海石油化工股份有限公司", 
            "coord": [
                121.445528, 
                30.790746
            ]
        }, 
        {
            "chufa": "对第1项立即整改，对第5项限期上报。", 
            "company": "拜耳材料科技（中国）有限公司", 
            "coord": [
                121.486823, 
                30.801649
            ]
        }, 
        {
            "chufa": "要求企业整改。", 
            "company": "巴斯夫化工有限公司", 
            "coord": [
                121.470768, 
                30.803213
            ]
        }, 
        {
            "chufa": "责令限期改正。", 
            "company": "巴斯夫化工有限公司", 
            "coord": [
                121.443542, 
                30.809948
            ]
        }, 
        {
            "chufa": "已要求上海石化督促芳烃部及时整改。", 
            "company": "中国石化上海石油化工股份有限公司", 
            "coord": [
                121.338397, 
                30.718223
            ]
        }, 
        {
            "chufa": "责令限期改正。", 
            "company": "巴斯夫化工有限公司", 
            "coord": [
                121.470768, 
                30.803213
            ]
        }, 
        {
            "chufa": "立即整改", 
            "company": "上海帕卡濑精有限公司", 
            "coord": [
                121.309003, 
                31.297768
            ]
        }, 
        {
            "chufa": "要求当场整改。", 
            "company": "中国石化上海石油化工股份有限公司", 
            "coord": [
                121.487899, 
                31.249162
            ]
        }, 
        {
            "chufa": "要求企业在6月30日前完成整改。", 
            "company": "中国石化上海石油化工股份有限公司", 
            "coord": [
                121.338397, 
                30.718223
            ]
        }, 
        {
            "chufa": "要求企业立即补办试生产备案手续，对紧急切断和安全联锁问题制定整改计划，其他问题立即改正。", 
            "company": "中国石化上海石油化工股份有限公司", 
            "coord": [
                121.487899, 
                31.249162
            ]
        }, 
        {
            "chufa": "立即整改", 
            "company": "宣伟（上海）涂料有限公司", 
            "coord": [
                121.27326, 
                31.420333
            ]
        }, 
        {
            "chufa": "立即整改", 
            "company": "上海新光化工有限公司", 
            "coord": [
                121.291003, 
                31.48653
            ]
        }, 
        {
            "chufa": "要求企业整改。", 
            "company": "巴斯夫化工有限公司", 
            "coord": [
                121.443542, 
                30.809948
            ]
        }, 
        {
            "chufa": "由金山区安全监管局报区政府协调解决。金山区安全监管局称预计8月底前完成整治。", 
            "company": "中国石化上海石油化工股份有限公司", 
            "coord": [
                121.338397, 
                30.718223
            ]
        }, 
        {
            "chufa": "责令立即整改和限期整改。", 
            "company": "中国石化上海石油化工股份有限公司", 
            "coord": [
                121.445528, 
                30.790746
            ]
        }, 
        {
            "chufa": "行政处罚：1.警告并处罚款人民币伍仟元。2. 罚款人民币陆千元。", 
            "company": "上海盛瀛化工有限公司", 
            "coord": [
                121.487899, 
                31.249162
            ]
        }, 
        {
            "chufa": "责令限期整改。", 
            "company": "巴斯夫化工有限公司", 
            "coord": [
                121.443542, 
                30.809948
            ]
        }, 
        {
            "chufa": "责令限期整改。", 
            "company": "圣莱科特化工（上海）有限公司", 
            "coord": [
                121.269476, 
                31.031609
            ]
        }, 
        {
            "chufa": "责令限期改正，罚款5万元。", 
            "company": "上海美兴化工股份有限公司", 
            "coord": [
                121.077855, 
                30.872092
            ]
        }, 
        {
            "chufa": "责令停止现场土建作业", 
            "company": "明尼苏达矿业制造特殊材料（上海）有限公司", 
            "coord": [
                121.479685, 
                30.808412
            ]
        }, 
        {
            "chufa": "立即整改", 
            "company": "上海东士石油化工有限公司", 
            "coord": [
                121.690195, 
                31.311714
            ]
        }, 
        {
            "chufa": "由青浦区安监局督促整改。", 
            "company": "上海罗门哈斯化工有限公司", 
            "coord": [
                121.142192, 
                31.180201
            ]
        }, 
        {
            "chufa": "要求当场整改。", 
            "company": "中国石化上海石油化工股份有限公司", 
            "coord": [
                121.338397, 
                30.718223
            ]
        }, 
        {
            "chufa": "由浦东新区安监局责令改正。", 
            "company": "上海鑫联石油有限公司", 
            "coord": [
                121.713106, 
                31.220148
            ]
        }, 
        {
            "chufa": "立即整改", 
            "company": "上海浦航石油有限公司", 
            "coord": [
                121.675534, 
                31.273315
            ]
        }, 
        {
            "chufa": "立即整改", 
            "company": "上海安易捷化工物流有限公司", 
            "coord": [
                121.271349, 
                31.484772
            ]
        }, 
        {
            "chufa": "责令整改", 
            "company": "上海赛科石油化工有限责任公司", 
            "coord": [
                121.434364, 
                30.78822
            ]
        }, 
        {
            "chufa": "要求该公司尽快针对事故开展全员教育培训，尽快完善施工作业监护制度，尽快上报风险抵押金缴纳情况、隐患排查治理分级管理情况、职业卫生现状评价开展情况、安全生产费用提取和使用情况。", 
            "company": "拜耳材料科技（中国）有限公司", 
            "coord": [
                121.486823, 
                30.801649
            ]
        }, 
        {
            "chufa": "由闵行区安监局责令限期整改。", 
            "company": "中国石油化工股份有限公司上海石油储运配送分公司", 
            "coord": [
                121.384508, 
                31.020049
            ]
        }, 
        {
            "chufa": "责令限期改正。", 
            "company": "上海赛科石油化工有限责任公司", 
            "coord": [
                121.434364, 
                30.78822
            ]
        }, 
        {
            "chufa": "责令限期改正。", 
            "company": "上海亨斯迈聚氨酯有限公司", 
            "coord": [
                121.467754, 
                30.791279
            ]
        }, 
        {
            "chufa": "由松江区安监局责令限期整改。", 
            "company": "王佳胶带（上海松江）有限公司", 
            "coord": [
                121.222457, 
                31.000938
            ]
        }, 
        {
            "chufa": "由嘉定区安监局责令限期整改。", 
            "company": "爱普香料集团股份有限公司", 
            "coord": [
                121.340547, 
                31.442618
            ]
        }, 
        {
            "chufa": "责令限期整改。", 
            "company": "巴斯夫化工有限公司", 
            "coord": [
                121.470768, 
                30.803213
            ]
        }, 
        {
            "chufa": "1、2项问题责令限期整改，第3项问题要求企业加强管理，并走访中石化工物流有限公司调研。", 
            "company": "优月仓储（上海）有限公司", 
            "coord": [
                121.475276, 
                30.7941
            ]
        }, 
        {
            "chufa": "要求企业对第3项问题立即整改。", 
            "company": "中国石化上海石油化工股份有限公司", 
            "coord": [
                121.338397, 
                30.718223
            ]
        }, 
        {
            "chufa": "要求集团公司立即整改。", 
            "company": "中国石化上海石油化工股份有限公司", 
            "coord": [
                121.445528, 
                30.790746
            ]
        }, 
        {
            "chufa": "已督促企业对险种进行调整。", 
            "company": "上海亨斯迈聚氨酯有限公司", 
            "coord": [
                121.467754, 
                30.791279
            ]
        }, 
        {
            "chufa": "立即整改", 
            "company": "中国石油化工股份有限公司上海高桥分公司", 
            "coord": [
                121.321363, 
                31.026699
            ]
        }, 
        {
            "chufa": "已要求企业限期改正。", 
            "company": "中国石化上海石油化工股份有限公司", 
            "coord": [
                121.487899, 
                31.249162
            ]
        }, 
        {
            "chufa": "已要求企业在竣工验收前完成整改。", 
            "company": "上海金力泰化工股份有限公司", 
            "coord": [
                121.456173, 
                30.826173
            ]
        }, 
        {
            "chufa": "立即整改", 
            "company": "中国石油化工股份有限公司上海高桥分公司", 
            "coord": [
                121.574005, 
                31.327927
            ]
        }, 
        {
            "chufa": "要求限期整改。", 
            "company": "中国石化上海石油化工股份有限公司", 
            "coord": [
                121.445528, 
                30.790746
            ]
        }, 
        {
            "chufa": "由金山区安全监管局报区政府协调解决。金山区安全监管局称预计8月底前完成整治。", 
            "company": "中国石化上海石油化工股份有限公司", 
            "coord": [
                121.445528, 
                30.790746
            ]
        }, 
        {
            "chufa": "立即整改", 
            "company": "上海高纯气体有限公司", 
            "coord": [
                121.570286, 
                31.349582
            ]
        }, 
        {
            "chufa": "立即整改", 
            "company": "上海神乐物流仓储有限公司", 
            "coord": [
                121.660245, 
                31.250407
            ]
        }, 
        {
            "chufa": "行政处罚：1. 罚款人民币壹万壹仟元；2.罚款人民币壹万元；3.警告并处罚款人民币壹万元。", 
            "company": "上海申宇医药化工有限公司", 
            "coord": [
                121.487899, 
                31.249162
            ]
        }, 
        {
            "chufa": "已要求企业限期整改。", 
            "company": "中国石化上海石油化工股份有限公司", 
            "coord": [
                121.445528, 
                30.790746
            ]
        }, 
        {
            "chufa": "已要求企业限期整改。", 
            "company": "中国石化上海石油化工股份有限公司", 
            "coord": [
                121.338397, 
                30.718223
            ]
        }, 
        {
            "chufa": "立即整改", 
            "company": "上海氯碱化工股份有限公司华胜化工厂", 
            "coord": [
                121.460579, 
                30.789108
            ]
        }, 
        {
            "chufa": "已要求企业限期整改。", 
            "company": "中国石化上海石油化工股份有限公司", 
            "coord": [
                121.487899, 
                31.249162
            ]
        }, 
        {
            "chufa": "因《上海市安全生产事故隐患排查治理办法》自2013年1月1日起刚刚实施，已提醒企业责令改正，未开具法律文书。", 
            "company": "上海巴斯夫聚氨酯有限公司", 
            "coord": [
                121.452735, 
                30.842648
            ]
        }, 
        {
            "chufa": "要求企业限期完成整改。", 
            "company": "中国石化上海石油化工股份有限公司", 
            "coord": [
                121.487899, 
                31.249162
            ]
        }, 
        {
            "chufa": "要求按照上海石化编制的隐患排查治理分级管理细则，加强隐患排查治理工作。", 
            "company": "中国石化上海石油化工股份有限公司", 
            "coord": [
                121.445528, 
                30.790746
            ]
        }, 
        {
            "chufa": "立即整改", 
            "company": "上海东士石油化工有限公司", 
            "coord": [
                121.408648, 
                30.884387
            ]
        }, 
        {
            "chufa": "要求企业限期完成整改。", 
            "company": "中国石化上海石油化工股份有限公司", 
            "coord": [
                121.338397, 
                30.718223
            ]
        }, 
        {
            "chufa": "由青浦区安监局督促整改。", 
            "company": "上海富臣化工有限公司", 
            "coord": [
                121.144544, 
                31.180214
            ]
        }, 
        {
            "chufa": "没收违法所得10307.69元，罚款50000元。", 
            "company": "宝美施化工（上海）有限公司", 
            "coord": [
                121.321933, 
                31.06623
            ]
        }, 
        {
            "chufa": "立即整改", 
            "company": "上海试四赫维化工有限公司", 
            "coord": [
                121.484509, 
                31.373935
            ]
        }, 
        {
            "chufa": "已要求企业限期改正。", 
            "company": "中国石化上海石油化工股份有限公司", 
            "coord": [
                121.445528, 
                30.790746
            ]
        }, 
        {
            "chufa": "已要求企业限期改正。", 
            "company": "中国石化上海石油化工股份有限公司", 
            "coord": [
                121.338397, 
                30.718223
            ]
        }, 
        {
            "chufa": "责令立即停止使用乙醇储罐。", 
            "company": "小川香料（上海）有限公司", 
            "coord": [
                121.287993, 
                31.045848
            ]
        }, 
        {
            "chufa": "要求按照上海石化编制的隐患排查治理分级管理细则，加强隐患排查治理工作。", 
            "company": "中国石化上海石油化工股份有限公司", 
            "coord": [
                121.487899, 
                31.249162
            ]
        }, 
        {
            "chufa": "责令整改", 
            "company": "上海赛科石油化工有限责任公司", 
            "coord": [
                121.443542, 
                30.809948
            ]
        }, 
        {
            "chufa": "责令限期整改。", 
            "company": "上海富乐达氟碳材料有限公司", 
            "coord": [
                121.365117, 
                30.824097
            ]
        }, 
        {
            "chufa": "由嘉定区安监局责令限期整改。", 
            "company": "上海祥乐田村电化工业有限公司", 
            "coord": [
                121.296978, 
                31.301223
            ]
        }, 
        {
            "chufa": "立即整改", 
            "company": "上海浦东国际机场航空油料有限责任公司", 
            "coord": [
                121.783664, 
                31.175304
            ]
        }, 
        {
            "chufa": "要求集团公司立即整改。", 
            "company": "中国石化上海石油化工股份有限公司", 
            "coord": [
                121.487899, 
                31.249162
            ]
        }, 
        {
            "chufa": "要求企业在6月30日前完成整改。", 
            "company": "中国石化上海石油化工股份有限公司", 
            "coord": [
                121.445528, 
                30.790746
            ]
        }, 
        {
            "chufa": "罚款人民币五万元整。", 
            "company": "中石化中海船舶燃料供应有限公司上海物资分公司", 
            "coord": [
                121.530326, 
                31.331645
            ]
        }, 
        {
            "chufa": "责令整改", 
            "company": "赢创德固赛特种化学（上海）有限公司", 
            "coord": [
                121.523995, 
                31.054744
            ]
        }, 
        {
            "chufa": "罚款人民币贰拾万元整。", 
            "company": "上海盛瀛化工有限公司", 
            "coord": [
                121.487899, 
                31.249162
            ]
        }, 
        {
            "chufa": "要求集团公司立即整改。", 
            "company": "中国石化上海石油化工股份有限公司", 
            "coord": [
                121.338397, 
                30.718223
            ]
        }, 
        {
            "chufa": "要求企业立即补办试生产备案手续，对紧急切断和安全联锁问题制定整改计划，其他问题立即改正。", 
            "company": "中国石化上海石油化工股份有限公司", 
            "coord": [
                121.445528, 
                30.790746
            ]
        }, 
        {
            "chufa": "罚款人民币贰万元整。", 
            "company": "上海品尚金属表面处理材料有限公司", 
            "coord": [
                121.403849, 
                31.374778
            ]
        }, 
        {
            "chufa": "要求当场整改。", 
            "company": "中国石化上海石油化工股份有限公司", 
            "coord": [
                121.445528, 
                30.790746
            ]
        }, 
        {
            "chufa": "责令立即整改和限期整改。", 
            "company": "中国石化上海石油化工股份有限公司", 
            "coord": [
                121.338397, 
                30.718223
            ]
        }, 
        {
            "chufa": "由浦东新区安全监管局责令改正。", 
            "company": "上海东士石油化工有限公司", 
            "coord": [
                121.690195, 
                31.311714
            ]
        }, 
        {
            "chufa": "要求企业限期整改。", 
            "company": "上海巴斯夫聚氨酯有限公司", 
            "coord": [
                121.452735, 
                30.842648
            ]
        }, 
        {
            "chufa": "要求企业对第3项问题立即整改。", 
            "company": "中国石化上海石油化工股份有限公司", 
            "coord": [
                121.445528, 
                30.790746
            ]
        }, 
        {
            "chufa": "我局执法人员会跟踪隐患治理情况", 
            "company": "上海氯碱化工股份有限公司华胜化工厂", 
            "coord": [
                121.460579, 
                30.789108
            ]
        }, 
        {
            "chufa": "责令立即改正。", 
            "company": "上海申港乙炔气厂", 
            "coord": [
                121.233346, 
                30.938464
            ]
        }, 
        {
            "chufa": "由松江区安监局督促整改。", 
            "company": "三晟化工（上海）有限公司", 
            "coord": [
                121.32947, 
                31.049214
            ]
        }, 
        {
            "chufa": "已要求上海石化尽快落实91号令的相关要求。", 
            "company": "中国石化上海石油化工股份有限公司", 
            "coord": [
                121.487899, 
                31.249162
            ]
        }, 
        {
            "chufa": "要求企业限期完成整改。", 
            "company": "中国石化上海石油化工股份有限公司", 
            "coord": [
                121.445528, 
                30.790746
            ]
        }, 
        {
            "chufa": "已督促企业对险种进行调整。", 
            "company": "上海巴斯夫聚氨酯有限公司", 
            "coord": [
                121.452735, 
                30.842648
            ]
        }, 
        {
            "chufa": "要求企业立即整改。", 
            "company": "上海巴斯夫聚氨酯有限公司", 
            "coord": [
                121.452735, 
                30.842648
            ]
        }, 
        {
            "chufa": "罚款人民币贰万元整。", 
            "company": "上海一汽四环物流服务有限公司", 
            "coord": [
                121.163695, 
                31.38893
            ]
        }, 
        {
            "chufa": "由金山区安全监管局报区政府协调解决。金山区安全监管局称预计8月底前完成整治。", 
            "company": "中国石化上海石油化工股份有限公司", 
            "coord": [
                121.487899, 
                31.249162
            ]
        }, 
        {
            "chufa": "责令限期改正。", 
            "company": "上海赛科石油化工有限责任公司", 
            "coord": [
                121.443542, 
                30.809948
            ]
        }, 
        {
            "chufa": "已要求上海石化督促芳烃部及时整改。", 
            "company": "中国石化上海石油化工股份有限公司", 
            "coord": [
                121.445528, 
                30.790746
            ]
        }, 
        {
            "chufa": "责令立即改正。", 
            "company": "宝美施化工（上海）有限公司", 
            "coord": [
                121.321933, 
                31.06623
            ]
        }, 
        {
            "chufa": "1-4项限期整改，第5项已列入公司2013年计划。", 
            "company": "洛德化学（上海）有限公司", 
            "coord": [
                121.474934, 
                30.811774
            ]
        }, 
        {
            "chufa": "要求企业在12月大修时完成整改。", 
            "company": "上海孚宝港务有限公司", 
            "coord": [
                121.520996, 
                31.054641
            ]
        }, 
        {
            "chufa": "立即整改", 
            "company": "上海宝钢化工有限公司", 
            "coord": [
                121.478008, 
                31.416156
            ]
        }, 
        {
            "chufa": "已要求上海石化尽快落实91号令的相关要求。", 
            "company": "中国石化上海石油化工股份有限公司", 
            "coord": [
                121.338397, 
                30.718223
            ]
        }, 
        {
            "chufa": "要求企业对第3项问题立即整改。", 
            "company": "中国石化上海石油化工股份有限公司", 
            "coord": [
                121.487899, 
                31.249162
            ]
        }, 
        {
            "chufa": "要求企业限期整改。", 
            "company": "中国石化上海石油化工股份有限公司", 
            "coord": [
                121.487899, 
                31.249162
            ]
        }, 
        {
            "chufa": "要求企业限期整改。", 
            "company": "中国石化上海石油化工股份有限公司", 
            "coord": [
                121.338397, 
                30.718223
            ]
        }, 
        {
            "chufa": "要求企业限期整改。", 
            "company": "上海赛科石油化工有限责任公司", 
            "coord": [
                121.443542, 
                30.809948
            ]
        }, 
        {
            "chufa": "要求限期整改。", 
            "company": "中国石化上海石油化工股份有限公司", 
            "coord": [
                121.487899, 
                31.249162
            ]
        }, 
        {
            "chufa": "责令限期改正。", 
            "company": "上海化学工业区浦江特种气体有限公司", 
            "coord": [
                121.472934, 
                30.820752
            ]
        }, 
        {
            "chufa": "立即整改", 
            "company": "上海华荣达石油化工仓储有限公司", 
            "coord": [
                121.541123, 
                31.344081
            ]
        }, 
        {
            "chufa": "要求企业限期整改。", 
            "company": "拜耳材料科技（中国）有限公司", 
            "coord": [
                121.486823, 
                30.801649
            ]
        }, 
        {
            "chufa": "要求企业限期整改。", 
            "company": "上海赛科石油化工有限责任公司", 
            "coord": [
                121.477429, 
                30.804188
            ]
        }, 
        {
            "chufa": "要求按照上海石化编制的隐患排查治理分级管理细则，加强隐患排查治理工作。", 
            "company": "中国石化上海石油化工股份有限公司", 
            "coord": [
                121.338397, 
                30.718223
            ]
        }, 
        {
            "chufa": "由闵行区安监局责令限期整改。", 
            "company": "中国石油化工股份有限公司上海石油储运配送分公司", 
            "coord": [
                121.321363, 
                31.026699
            ]
        }, 
        {
            "chufa": "要求企业在6月30日前完成整改。", 
            "company": "中国石化上海石油化工股份有限公司", 
            "coord": [
                121.487899, 
                31.249162
            ]
        }, 
        {
            "chufa": "立即整改", 
            "company": "上海神乐物流配送有限公司", 
            "coord": [
                121.660245, 
                31.250407
            ]
        }, 
        {
            "chufa": "因《上海市安全生产事故隐患排查治理办法》于2013年1月1日起刚刚实施，已要求企业限期改正，未开具法律文书。", 
            "company": "巴斯夫化工有限公司", 
            "coord": [
                121.443542, 
                30.809948
            ]
        }, 
        {
            "chufa": "立即整改", 
            "company": "中国石油化工股份有限公司上海高桥分公司", 
            "coord": [
                121.568588, 
                31.338025
            ]
        }, 
        {
            "chufa": "要求企业限期整改。", 
            "company": "上海赛科石油化工有限责任公司", 
            "coord": [
                121.434364, 
                30.78822
            ]
        }, 
        {
            "chufa": "要求企业限期整改。", 
            "company": "中国石化上海石油化工股份有限公司", 
            "coord": [
                121.445528, 
                30.790746
            ]
        }, 
        {
            "chufa": "罚款人民币贰拾万元整。", 
            "company": "上海盛瀛化工有限公司", 
            "coord": [
                121.290005, 
                30.740801
            ]
        }, 
        {
            "chufa": "罚款人民币伍万元整。", 
            "company": "上海唯稳实业有限公司", 
            "coord": [
                121.248408, 
                30.835081
            ]
        }, 
        {
            "chufa": "要求限期整改。", 
            "company": "中国石化上海石油化工股份有限公司", 
            "coord": [
                121.338397, 
                30.718223
            ]
        }, 
        {
            "chufa": "由浦东新区安全监管局责令改正。", 
            "company": "上海东士石油化工有限公司", 
            "coord": [
                121.408648, 
                30.884387
            ]
        }, 
        {
            "chufa": "要求企业立即补办试生产备案手续，对紧急切断和安全联锁问题制定整改计划，其他问题立即改正。", 
            "company": "中国石化上海石油化工股份有限公司", 
            "coord": [
                121.338397, 
                30.718223
            ]
        }, 
        {
            "chufa": "责令限期改正。", 
            "company": "拜耳材料科技（中国）有限公司", 
            "coord": [
                121.486823, 
                30.801649
            ]
        }, 
        {
            "chufa": "立即整改", 
            "company": "上海宝氢气体工业有限公司", 
            "coord": [
                121.462651, 
                31.416264
            ]
        }, 
        {
            "chufa": "责令限期改正。", 
            "company": "上海赛科石油化工有限责任公司", 
            "coord": [
                121.477429, 
                30.804188
            ]
        }, 
        {
            "chufa": "责令立即整改和限期整改。", 
            "company": "中国石化上海石油化工股份有限公司", 
            "coord": [
                121.487899, 
                31.249162
            ]
        }, 
        {
            "chufa": "罚款人民币壹佰贰拾贰万元整。", 
            "company": "上海翁牌冷藏实业有限公司", 
            "coord": [
                121.361595, 
                31.315392
            ]
        }, 
        {
            "chufa": "因《上海市安全生产事故隐患排查治理办法》于2013年1月1日起刚刚实施，已要求企业限期改正，未开具法律文书。", 
            "company": "巴斯夫化工有限公司", 
            "coord": [
                121.470768, 
                30.803213
            ]
        }, 
        {
            "chufa": "行政处罚：1.警告并处罚款人民币伍仟元。2. 罚款人民币陆千元。", 
            "company": "上海盛瀛化工有限公司", 
            "coord": [
                121.290005, 
                30.740801
            ]
        }
    ]; 
		    //在地图上绘制点    
                    for (var i = 0; i < markerArr.length; i++) {  
                        var p0 = markerArr[i].coord[0];  //points[i][0];
                        var p1 = markerArr[i].coord[1];  //points[i][1];
                        var maker = addMarker(new window.BMap.Point(p0, p1)); 
							//addInfoWindow(maker,points[i][3],points[i][4],i);
                        addInfoWindow(maker, markerArr[i], i);   
                    }  
                
				// 添加标注  
                function addMarker(point) {  
                    var marker = new BMap.Marker(point);  
                    bmap.addOverlay(marker);  
                    return marker;  
                }  
  
            // 添加信息窗口  
            function addInfoWindow(marker,poi) {  
                    //pop弹窗标题  
                    var title = '<div style="font-weight:bold;color:#CE5521;font-size:14px">' + poi.company + '</div>'; // 
                    //pop弹窗信息  
                    var html = [];  
                    html.push('<table cellspacing="0" style="table-layout:fixed;width:100%;font:12px arial,simsun,sans-serif"><tbody>');  
                    html.push('<tr>');  
                    html.push('<td style="vertical-align:top;line-height:16px;width:38px;white-space:nowrap;word-break:keep-all">处罚:</td>');  
                    html.push('<td style="vertical-align:top;line-height:16px">' + poi.chufa + ' </td>'); // 
                    html.push('</tr>');  
                    html.push('</tbody></table>');  
                    var infoWindow = new BMap.InfoWindow(html.join(""), { title: title, width: 200 });  
  
                    var openInfoWinFun = function () {  
                        marker.openInfoWindow(infoWindow);  
                    };  
                    marker.addEventListener("click", openInfoWinFun);  
                    return openInfoWinFun;  
                }  
    } 
});



if (option && typeof option === "object") {
    myChart.setOption(option, true);
}

}

///////////////////交通模块////////////////////////////
function Timejiaotong(val) {
var dom = document.getElementById("container");
var myChart = echarts.init(dom);

var app = {};
option = null;
app.title = '热力图与标注';
var url='data/'+val+'.json';

//针对不同数据，设置符号大小
var maxpoints=0;
var pointsize=0;
if(val=="jiaotong"){
	 maxpoints=2;
	 pointsize=3;
}
//alert(url);
//alert(maxpoints);
//alert(pointsize);
$.get(url, function (data) {

	//[[{"coord":[120.14322240845,30.236064370321],"elevation":21},{}],[]]
	
	//重复来数据的时候清空points
	var points=new Array(10);
	
	for (var i = 0; i < data.length; i++) { 
		var dt=data[i]
		 points[i] = [].concat.apply([], dt.map(function (track){
			return track.map(function (seg) {
				return seg.coord.concat([1]);
        });
    }));
	   //alert(points[i]);
	}
	
	//alert(points);
	//var points=[[121.485807,31.223243,1,"title","address 地址"],[121.418146,31.323953,2,"abc","上海大学"],[121.595795,31.255323,3,"abc","杨高中路"],[121.220375,31.10505,4,"abc","欢乐谷"],[121.458785,31.236953,5,"abc","上海植物园"],[121.442113,31.230284,6,"abc","公园"],[121.447323,31.13149,7,"abc","aaaaaaa"],[121.482824,31.237046,8,"abc","市中心"]];
	//alert(points);
    myChart.setOption(option = {
	
	baseOption: {
		timeline: {
			type: "number",
			label: {
				formatter: function(s) {
					return s;
				},
				textStyle: {
					color: '#fff',
					fontSize: 5
				}
			},
			data: [{
                    value: '01',
                    tooltip: {
                        formatter: '{b} 2015年07月'
                    },
                    symbol: 'diamond',
                    symbolSize: 20
                },{
                    value: '02',
                    tooltip: {
                        formatter: '{b} 2015年08月'
                    },
                    symbol: 'diamond',
                    symbolSize: 20
                },{
                    value: '03',
                    tooltip: {
                        formatter: '{b} 2015年09月'
                    },
                    symbol: 'diamond',
                    symbolSize: 20
                },{
                    value: '04',
                    tooltip: {
                        formatter: '{b} 2015年10月'
                    },
                    symbol: 'diamond',
                    symbolSize: 20
                },{
                    value: '05',
                    tooltip: {
                        formatter: '{b} 2015年11月'
                    },
                    symbol: 'diamond',
                    symbolSize: 20
                },{
                    value: '06',
                    tooltip: {
                        formatter: '{b} 2015年12月'
                    },
                    symbol: 'diamond',
                    symbolSize: 20
                },{
                    value: '07',
                    tooltip: {
                        formatter: '{b} 2016年01月'
                    },
                    symbol: 'diamond',
                    symbolSize: 20
                },{
                    value: '08',
                    tooltip: {
                        formatter: '{b} 2016年02月'
                    },
                    symbol: 'diamond',
                    symbolSize: 20
                },{
                    value: '09',
                    tooltip: {
                        formatter: '{b} 2016年03月'
                    },
                    symbol: 'diamond',
                    symbolSize: 20
                },{
                    value: '10',
                    tooltip: {
                        formatter: '{b} 2016年04月'
                    },
                    symbol: 'diamond',
                    symbolSize: 20
                }],
			autoPlay: true,
			playInterval: 1000
		},
		
        animation: false,
		tooltip: {},
        bmap: {
			// 百度地图中心经纬度
            center: [121.476213,31.237046],
			// 百度地图缩放
            zoom: 13,
			 // 是否开启拖拽缩放，可以只设置 'scale' 或者 'move'
            roam: true
			// 百度地图的自定义样式，见 http://developer.baidu.com/map/jsdevelop-11.htm
           // mapStyle: {}
        },
        visualMap: {
            show: false,
            top: 'top',
            min: 0,
            max: maxpoints,//20  4
            seriesIndex: 0,
            calculable: true,
            inRange: {
                color: ['blue', 'green', 'yellow', 'red']
            }
        },
        series: [{
            type: 'heatmap',
			// 使用百度地图坐标系
           // 数据格式跟在 geo 坐标系上一样，每一项都是 [经度，纬度，数值大小，其它维度...]
            coordinateSystem: 'bmap',
            //data: points,
           // pointSize: pointsize,//5  15
            //blurSize: 25
        }],
    },
	//配对时间轴变化初始化数据
	options:[{     
        series: [{
            data: points[0],
			pointSize: 15,//5  15
            blurSize: 15
        }]
	},
	{
        series: [{
            data: points[1],
			pointSize: 15,//5  15
            blurSize: 15
        }]
	},
	{
        series: [{
            data: points[2],
			pointSize: 15,//5  15
            blurSize: 15
        }]
	},
	{
        series: [{
            data: points[3],
			pointSize: 15,//5  15
            blurSize: 15
        }]
	},{     
        series: [{
            data: points[4],
			pointSize: 15,//5  15
            blurSize: 15
        }]
	},
	{
        series: [{
            data: points[5],
			pointSize: 15,//5  15
            blurSize: 15
        }]
	},
	{
        series: [{
            data: points[6],
			pointSize: 15,//5  15
            blurSize: 15
        }]
	},
	{
        series: [{
            data: points[7],
			pointSize: 15,//5  15
            blurSize: 15
        }]
	},
	{
        series: [{
            data: points[8],
			pointSize: 15,//5  15
            blurSize: 15
        }]
	},
	{
        series: [{
            data: points[9],
			pointSize: 15,//5  15
            blurSize: 15
        }]
	}
		]
	});
	
	
	
	
	
    if (!app.inNode) {
        // 添加百度地图插件
        var bmap = myChart.getModel().getComponent('bmap').getBMap();
        bmap.addControl(new BMap.MapTypeControl());
		bmap.setMapStyle({style:'light'}); 
    } 
});



if (option && typeof option === "object") {
    myChart.setOption(option, true);
}

}


////////////////出警模块///////////////////////
function Timechujing(val) {
var dom = document.getElementById("container");
var myChart = echarts.init(dom);

var app = {};
option = null;
app.title = '热力图与标注';
var url='data/'+val+'.json';

//针对不同数据，设置符号大小
var maxpoints=2;
var pointsize=0;
if(val=="chujing"){
	 maxpoints=2;
	 pointsize=3;
}
//alert(url);
//alert(maxpoints);
//alert(pointsize);
$.get(url, function (data) {

	//[[{"coord":[120.14322240845,30.236064370321],"elevation":21},{}],[]]
	
	//重复来数据的时候清空points
	var points=new Array(9);
	
	for (var i = 0; i < data.length; i++) { 
		var dt=data[i]
		 points[i] = [].concat.apply([], dt.map(function (track){
			return track.map(function (seg) {
				return seg.coord.concat([seg.elevation]);
        });
    }));
	 // alert(points[i]);
	}
	
	//alert(points);
	//var points=[[121.485807,31.223243,1,"title","address 地址"],[121.418146,31.323953,2,"abc","上海大学"],[121.595795,31.255323,3,"abc","杨高中路"],[121.220375,31.10505,4,"abc","欢乐谷"],[121.458785,31.236953,5,"abc","上海植物园"],[121.442113,31.230284,6,"abc","公园"],[121.447323,31.13149,7,"abc","aaaaaaa"],[121.482824,31.237046,8,"abc","市中心"]];
	//alert(points);
    myChart.setOption(option = {
	
	baseOption: {
		timeline: {
			type: "number",
			label: {
				formatter: function(s) {
					return s;
				},
				textStyle: {
					color: '#fff',
					fontSize: 5
				}
			},
			data: [{
                    value: '201601',
                    tooltip: {
                        formatter: '{b} 2016年01月'
                    },
                    symbol: 'diamond',
                    symbolSize: 20
                },{
                    value: '201602',
                    tooltip: {
                        formatter: '{b} 2016年02月'
                    },
                    symbol: 'diamond',
                    symbolSize: 20
                },{
                    value: '201603',
                    tooltip: {
                        formatter: '{b} 2016年03月'
                    },
                    symbol: 'diamond',
                    symbolSize: 20
                },{
                    value: '201604',
                    tooltip: {
                        formatter: '{b} 2016年04月'
                    },
                    symbol: 'diamond',
                    symbolSize: 20
                }],
			autoPlay: true,
			playInterval: 1000
		},
		
        animation: false,
		tooltip: {},
        bmap: {
			// 百度地图中心经纬度
            center: [121.476213,31.237046],
			// 百度地图缩放
            zoom: 13,
			 // 是否开启拖拽缩放，可以只设置 'scale' 或者 'move'
            roam: true
			// 百度地图的自定义样式，见 http://developer.baidu.com/map/jsdevelop-11.htm
           // mapStyle: {}
        },
        visualMap: {
            show: false,
            top: 'top',
            min: 0,
            max: 3,//20  4
            seriesIndex: 0,
            calculable: true,
            inRange: {
                color: ['blue', 'green', 'yellow', 'red']
            }
        },
        series: [{
            type: 'heatmap',
			// 使用百度地图坐标系
           // 数据格式跟在 geo 坐标系上一样，每一项都是 [经度，纬度，数值大小，其它维度...]
            coordinateSystem: 'bmap',
            //data: points,
           // pointSize: pointsize,//5  15
            //blurSize: 25
        }],
    },
	//配对时间轴变化初始化数据
	options:[{     
        series: [{
            data: points[5],
			pointSize: 2,//5  15
            blurSize: 15
        }]
	},
	{
        series: [{
            data: points[6],
			pointSize: 2,//5  15
            blurSize: 15
        }]
	},
	{
        series: [{
            data: points[7],
			pointSize: 2,//5  15
            blurSize: 15
        }]
	},
	{
        series: [{
            data: points[8],
			pointSize: 2,//5  15
            blurSize: 15
        }]
	}
		]
	});
	
	
	
	
	
    if (!app.inNode) {
        // 添加百度地图插件
        var bmap = myChart.getModel().getComponent('bmap').getBMap();
        bmap.addControl(new BMap.MapTypeControl());
		bmap.setMapStyle({style:'light'});
    } 
});



if (option && typeof option === "object") {
    myChart.setOption(option, true);
}

}
///////////////餐饮////////////////////
function Timecanyin(val) {
var dom = document.getElementById("container");
var myChart = echarts.init(dom);

var app = {};
option = null;
app.title = '热力图与标注';

var url='data/'+val+'.json';

//针对不同数据，设置符号大小
var maxpoints=0;
var pointsize=0;
if(val=="canyin"){
	 maxpoints=2;
	 pointsize=3;
}
//alert(url);
//alert(maxpoints);
//alert(pointsize);
$.get(url, function (data) {

	//[[{"coord":[120.14322240845,30.236064370321],"elevation":21},{}],[]]
	
	//重复来数据的时候清空points
	var points=new Array(10);
	
	for (var i = 0; i < data.length; i++) { 
		var dt=data[i]
		 points[i] = [].concat.apply([], dt.map(function (track){
			return track.map(function (seg) {
				return seg.coord.concat([(seg.elevation-1)*2/4]);
        });
    }));
	   //alert(points[i]);
	}
	
	//alert(points);
	//var points=[[121.485807,31.223243,1,"title","address 地址"],[121.418146,31.323953,2,"abc","上海大学"],[121.595795,31.255323,3,"abc","杨高中路"],[121.220375,31.10505,4,"abc","欢乐谷"],[121.458785,31.236953,5,"abc","上海植物园"],[121.442113,31.230284,6,"abc","公园"],[121.447323,31.13149,7,"abc","aaaaaaa"],[121.482824,31.237046,8,"abc","市中心"]];
	//alert(points);
    myChart.setOption(option = {
	
	baseOption: {
		timeline: {
			type: "number",
			label: {
				formatter: function(s) {
					return s;
				},
				textStyle: {
					color: '#fff',
					fontSize: 5
				}
			},
			data: [{
                    value: '01',
                    tooltip: {
                        formatter: '{b} 2015年07月'
                    },
                    symbol: 'diamond',
                    symbolSize: 20
                },{
                    value: '02',
                    tooltip: {
                        formatter: '{b} 2015年08月'
                    },
                    symbol: 'diamond',
                    symbolSize: 20
                },{
                    value: '03',
                    tooltip: {
                        formatter: '{b} 2015年09月'
                    },
                    symbol: 'diamond',
                    symbolSize: 20
                },{
                    value: '04',
                    tooltip: {
                        formatter: '{b} 2015年10月'
                    },
                    symbol: 'diamond',
                    symbolSize: 20
                },{
                    value: '05',
                    tooltip: {
                        formatter: '{b} 2015年11月'
                    },
                    symbol: 'diamond',
                    symbolSize: 20
                },{
                    value: '06',
                    tooltip: {
                        formatter: '{b} 2015年12月'
                    },
                    symbol: 'diamond',
                    symbolSize: 20
                },{
                    value: '07',
                    tooltip: {
                        formatter: '{b} 2016年01月'
                    },
                    symbol: 'diamond',
                    symbolSize: 20
                },{
                    value: '08',
                    tooltip: {
                        formatter: '{b} 2016年02月'
                    },
                    symbol: 'diamond',
                    symbolSize: 20
                },{
                    value: '09',
                    tooltip: {
                        formatter: '{b} 2016年03月'
                    },
                    symbol: 'diamond',
                    symbolSize: 20
                },{
                    value: '10',
                    tooltip: {
                        formatter: '{b} 2016年04月'
                    },
                    symbol: 'diamond',
                    symbolSize: 20
                }],
				
			autoPlay: true,
			playInterval: 2000
		},
		
        animation: false,
		tooltip: {},
        bmap: {
			// 百度地图中心经纬度
            center: [121.476213,31.237046],
			// 百度地图缩放
            zoom: 11,
			 // 是否开启拖拽缩放，可以只设置 'scale' 或者 'move'
            roam: true
			// 百度地图的自定义样式，见 http://developer.baidu.com/map/jsdevelop-11.htm
           // mapStyle: {}
        },
        visualMap: {
            show: false,
            top: 'top',
            min: 0,
            max: maxpoints,//20  4
            seriesIndex: 0,
            calculable: true,
            inRange: {
                color: ['blue', 'green', 'yellow', 'red']
            }
        },
        series: [{
            type: 'heatmap',
			// 使用百度地图坐标系
           // 数据格式跟在 geo 坐标系上一样，每一项都是 [经度，纬度，数值大小，其它维度...]
            coordinateSystem: 'bmap',
            //data: points,
           // pointSize: pointsize,//5  15
            //blurSize: 25
        }],
    },
	//配对时间轴变化初始化数据
	options:[{     
        series: [{
            data: points[0],
			pointSize: 8,//5  15
            blurSize: 15
        }]
	},
	{
        series: [{
            data: points[1],
			pointSize: 8,//5  15
            blurSize: 15
        }]
	},
	{
        series: [{
            data: points[2],
			pointSize: 8,//5  15
            blurSize: 15
        }]
	},
	{
        series: [{
            data: points[3],
			pointSize: 8,//5  15
            blurSize: 15
        }]
	},{     
        series: [{
            data: points[4],
			pointSize: 8,//5  15
            blurSize: 15
        }]
	},
	{
        series: [{
            data: points[5],
			pointSize: 8,//5  15
            blurSize: 15
        }]
	},
	{
        series: [{
            data: points[6],
			pointSize: 8,//5  15
            blurSize: 15
        }]
	},
	{
        series: [{
            data: points[7],
			pointSize: 8,//5  15
            blurSize: 15
        }]
	},
	{
        series: [{
            data: points[8],
			pointSize: 8,//5  15
            blurSize: 15
        }]
	},
	{
        series: [{
            data: points[9],
			pointSize: 8,//5  15
            blurSize: 15
        }]
	}
		]
	});
	
	
	
	
	
    if (!app.inNode) {
        // 添加百度地图插件
        var bmap = myChart.getModel().getComponent('bmap').getBMap();
        bmap.addControl(new BMap.MapTypeControl());
		bmap.setMapStyle({style:'light'});	 
    } 
});



if (option && typeof option === "object") {
    myChart.setOption(option, true);
}

}
/////////////////////////////////
$('.js-click').click(function(){
	$(this).css("background",'#101734');
	$(".js-click2").css("background",'#5E5E5E');

	$(".option1").show().siblings().hide()
});

$('.js-click2').click(function(){
	$(this).css("background",'#101734');
	$(".js-click").css("background",'#5E5E5E');
	$(".option2").show().siblings().hide()
})

$('.logo1').click(function(){
	$(".logo1").css("background",'#3399cc');
	$(".logo2").css("background",'#39383F');	
	$(".logo3").css("background",'#39383F');
    Time("shehuizhian_juhe");//响应选择社会治安
})
$('.logo2').click(function(){
	$(".logo1").css("background",'#39383F');
	$(".logo2").css("background",'#3399cc');	
	$(".logo3").css("background",'#39383F');
	Time("shipinanquan");//响应选择食品安全
})
$('.logo3').click(function(){
	$(".logo1").css("background",'#39383F');
	$(".logo2").css("background",'#39383F');	
	$(".logo3").css("background",'#3399cc');
    Time("huanjingweisheng1");//响应选择环境卫生	
})

$(".btn").eq(0).click(function(){
	$(".logo1").css("background",'#3399cc');
	$(".logo2").css("background",'#39383F');	
	$(".logo3").css("background",'#39383F');
		Timechujing("chujing");
	})
$(".btn").eq(1).click(function(){
	$(".logo1").css("background",'#3399cc');
	$(".logo2").css("background",'#39383F');	
	$(".logo3").css("background",'#39383F');
		Timejiaotong("jiaotong");
	})
$(".btn").eq(2).click(function(){
	$(".logo1").css("background",'#3399cc');
	$(".logo2").css("background",'#39383F');	
	$(".logo3").css("background",'#39383F');
		Timeqiye("qiyebuliang");
	})
$(".btn").eq(3).click(function(){
	$(".logo1").css("background",'#3399cc');
	$(".logo2").css("background",'#39383F');	
	$(".logo3").css("background",'#39383F');
		Time("wg_shehui");
	})
	
$(".btn2").eq(0).click(function(){
	$(".logo1").css("background",'#39383F');
	$(".logo2").css("background",'#3399cc');	
	$(".logo3").css("background",'#39383F');
		Timecanyin("canyin");
	})
$(".btn2").eq(1).click(function(){
	$(".logo1").css("background",'#39383F');
	$(".logo2").css("background",'#3399cc');	
	$(".logo3").css("background",'#39383F');
		Time("wg_shipin");
	})

$(".btn3").eq(0).click(function(){
	$(".logo1").css("background",'#39383F');
	$(".logo2").css("background",'#39383F');	
	$(".logo3").css("background",'#3399cc');
		Time("wuranyuan");
	})
$(".btn3").eq(1).click(function(){
	$(".logo1").css("background",'#39383F');
	$(".logo2").css("background",'#39383F');	
	$(".logo3").css("background",'#3399cc');
		Time("wg_huanjing");
	})
$(".btn3").eq(2).click(function(){
	$(".logo1").css("background",'#39383F');
	$(".logo2").css("background",'#39383F');	
	$(".logo3").css("background",'#3399cc');
		Time("gongdi");
	})
	

</script>
</body>
</html>
```




















