# jquery控件 jtable 使用范例 - z69183787的专栏 - CSDN博客
2013年11月28日 15:56:32[OkidoGreen](https://me.csdn.net/z69183787)阅读数：9438
```java
function initJtable(){
		$('#content').jtable({
            title: '资产列表',
            paging: true, //Enable paging
            pageSize: 10, //Set page size (default: 10)
           	sorting: true, //Enable sorting
            messages: {
            	pagingInfo:'显示 {0}-{1}条   总共{2}条',
            	gotoPageLabel: '跳转到',
            	pageSizeChangeLabel: '每页显示',
            	loadingMessage: '数据加载中...',
            	editRecord: '详细',
            	noDataAvailable: '没有数据！'
            },
            actions: {
                listAction: '<%=basePath %>asset/inquery.action'
            },
            fields: {
                id: {
                    key: true,
                    create: false,
                    edit: false,
                    list: false
                },
                assetCode:{
					title:'资产编码',
					width:'6%'
                },
                name: {
                    title: '资产名称',
                    width: '10%'
                },
                mainType:{                        	
					title:'大类',
					width:'10%',
					display: function (data) {
						if(data.record.mainType){
							return data.record.mainType.name;
						}
				        return ;
				    }
                },
                subType:{
					title:'中类',
					width:'10%',
					display: function (data) {
						if(data.record.subType){
							return data.record.subType.name;
						}
				        return ;
				    }
                },
                detailType:{
					title:'小类',
					width:'10%',
					display: function (data) {
						if(data.record.detailType){
							return data.record.detailType.name;
						}
				        return ;
				    }
                },
                'assetOwnerInf.line':{
                	title:'线路',
					width:'10%',
					display:function(data){
						if(data.record.assetOwnerInf.line){
							return data.record.assetOwnerInf.line.name;
						}
						return ;
                	}
                },
                'assetOwnerInf.station':{
                	title:'车站',
					width:'10%',
					display:function(data){
						if(data.record.assetOwnerInf.station){
							return data.record.assetOwnerInf.station.name;
						}
						return ;
                	}
                },
                useDate:{
					title:'使用时间',
					width:'10%',
					display:function(data){
					if(data.record.useDate!=null && data.record.useDate.length>10 ){
						return data.record.useDate.substring(0,10);
					}
					return ;
            	}
                },
                options:{
                	title:'操作',
                	width:'5%',
                	sorting: false,
					display: function (data) {
						var btn = "<a href='<%=basePath%>asset/showView.action?id="+data.record.id+"' target='_blank')>查看</a>";
				        return btn;
				    }
                }
            }
        });
		$('#content').jtable('load');
	}
```
```
<div id="content" class="fl w100p" style="width: 100%;">
          </div>
```
增加查询参数：重新load即可
```java
$('#content').jtable('load',{
			name_like_filter:assetName,
			assetCode_like_filter:assetCode,
			'mainType.id_equal_filter':mainType,
			'subType.id_equal_filter':subType,
			'detailType.id_equal_filter':detailType,
			'assetOwnerInf.line.id_equal_filter':lineId,
			'assetOwnerInf.useOrganization.id_equal_filter':useOrgId,
			'assetOwnerInf.ownerOrganization.id_equal_filter':ownerOrgId,
			'assetOwnerInf.department.id_equal_filter':departmentId,
			 verifyState_equal_filter: verifyState,
			'state.state_equal_filter':assetState,
			'project.id_equal_filter':projectId
		});
```
```java
{
    "Result": "OK",
    "Records": [
        {
            "approvalScrapDate": "",
            "assetCode": "23561560205110801001",
            "assetOwnerInf": {
                "beginUseDate": "2006-12-31 00:00:00",
                "createDate": "2013-11-26 17:01:42",
                "department": {
                    "id": "56",
                    "name": "上海地铁第二运营有限公司",
                    "remarks": ""
                },
                "id": "8a81aba3429394f0014293a488a91507",
                "line": {
                    "code": "02",
                    "id": "2",
                    "name": "轨道交通二号线",
                    "projects": [
                        {
                            "approvalDate": "",
                            "cityAllowance": "",
                            "completedFinalPrice": 10762964547.53,
                            "contractPerson": "",
                            "contractPhone": "",
                            "createDate": "2013-02-18 15:37:26",
                            "dispatchNo": "",
                            "id": "8a818e903ce01617013cec3c632202ec",
                            "inputOperationDate": "",
                            "keyword": "",
                            "line": null,
                            "moneySourceType": "",
                            "operateDate": "2013-09-22 11:03:11",
                            "overhaulContent": "",
                            "primaryDesignBudget": "1048387.000000",
                            "primaryDesignDate": "1997-01-20",
                            "primaryDesignMoney": "",
                            "primaryDesignNo": "沪建建规（97）第0057号",
                            "primaryDesignRemark": "",
                            "professionalType": "",
                            "projectAdddept": "集团",
                            "projectAdddeptId": "2500",
                            "projectAddperson": "朱沪生",
                            "projectAddpersonId": "2511",
                            "projectAddtimeDate": "",
                            "projectAttachId": "",
                            "projectBudgetAll": "1048387.000000",
                            "projectCheckunit": "",
                            "projectCompany": "上海申通地铁集团有限公司",
                            "projectCompanyId": "2500",
                            "projectCreateunit": "",
                            "projectDestoryDate": "",
                            "projectEndtimePlanDate": "",
                            "projectEstimateAll": "",
                            "projectExecuteunit": "",
                            "projectExplain": "",
                            "projectFeasibilityBudget": "1007000",
                            "projectFeasibilityDate": "1995-03-01",
                            "projectFeasibilityNo": "计投资【1995】210号",
                            "projectFeasibilityRemark": "",
                            "projectMoneysource": "",
                            "projectName": "上海地铁二号线一期工程",
                            "projectNo": "1996-2号线-001",
                            "projectStarttimePlanDate": "",
                            "projectState": "",
                            "projectType": "1",
                            "projectType2": "",
                            "remark": "",
                            "removed": "0",
                            "shortName": "二号线",
                            "substituteConstruction": ""
                        },
                        {
                            "approvalDate": "",
                            "cityAllowance": "",
                            "completedFinalPrice": 2433043036.07,
                            "contractPerson": "",
                            "contractPhone": "",
                            "createDate": "2013-02-18 15:39:00",
                            "dispatchNo": "",
                            "id": "8a818e903ce01617013cec3dd44602ed",
                            "inputOperationDate": "2006-12-30 00:00:00",
                            "keyword": "",
                            "line": null,
                            "moneySourceType": "",
                            "operateDate": "2013-02-19 08:49:25",
                            "overhaulContent": "",
                            "primaryDesignBudget": "",
                            "primaryDesignDate": "",
                            "primaryDesignMoney": "",
                            "primaryDesignNo": "",
                            "primaryDesignRemark": "",
                            "professionalType": "",
                            "projectAdddept": "长宁线项目公司",
                            "projectAdddeptId": "2521",
                            "projectAddperson": "严厚群",
                            "projectAddpersonId": "3910",
                            "projectAddtimeDate": "",
                            "projectAttachId": "",
                            "projectBudgetAll": "455900.000000",
                            "projectCheckunit": "",
                            "projectCompany": "上海轨道交通长宁线发展有限公司",
                            "projectCompanyId": "2521",
                            "projectCreateunit": "",
                            "projectDestoryDate": "",
                            "projectEndtimePlanDate": "",
                            "projectEstimateAll": "",
                            "projectExecuteunit": "",
                            "projectExplain": "",
                            "projectFeasibilityBudget": "",
                            "projectFeasibilityDate": "",
                            "projectFeasibilityNo": "沪计城（2001）562号",
                            "projectFeasibilityRemark": "",
                            "projectMoneysource": "",
                            "projectName": "地铁二号线西延伸工程",
                            "projectNo": "2001-2号线西延伸-001",
                            "projectStarttimePlanDate": "",
                            "projectState": "",
                            "projectType": "1",
                            "projectType2": "",
                            "remark": "",
                            "removed": "0",
                            "shortName": "二号线西延伸",
                            "substituteConstruction": ""
                        },
                        {
                            "approvalDate": "",
                            "cityAllowance": "",
                            "completedFinalPrice": 0,
                            "contractPerson": "",
                            "contractPhone": "",
                            "createDate": "2013-02-18 15:40:36",
                            "dispatchNo": "",
                            "id": "8a818e903ce01617013cec3f498002ee",
                            "inputOperationDate": "",
                            "keyword": "",
                            "line": null,
                            "moneySourceType": "",
                            "operateDate": "2013-02-19 08:49:44",
                            "overhaulContent": "",
                            "primaryDesignBudget": "",
                            "primaryDesignDate": "",
                            "primaryDesignMoney": "",
                            "primaryDesignNo": "",
                            "primaryDesignRemark": "",
                            "professionalType": "",
                            "projectAdddept": "长宁线项目公司",
                            "projectAdddeptId": "2521",
                            "projectAddperson": "严厚群",
                            "projectAddpersonId": "3910",
                            "projectAddtimeDate": "",
                            "projectAttachId": "",
                            "projectBudgetAll": "193700.000000",
                            "projectCheckunit": "",
                            "projectCompany": "上海轨道交通长宁线发展有限公司",
                            "projectCompanyId": "2521",
                            "projectCreateunit": "",
                            "projectDestoryDate": "",
                            "projectEndtimePlanDate": "",
                            "projectEstimateAll": "",
                            "projectExecuteunit": "",
                            "projectExplain": "",
                            "projectFeasibilityBudget": "",
                            "projectFeasibilityDate": "",
                            "projectFeasibilityNo": "沪发改城（2008）59号",
                            "projectFeasibilityRemark": "",
                            "projectMoneysource": "",
                            "projectName": "轨道交通2号线西延伸淞虹路站-诸光路站工程",
                            "projectNo": "2008-2号线西西延伸-001",
                            "projectStarttimePlanDate": "",
                            "projectState": "",
                            "projectType": "1",
                            "projectType2": "",
                            "remark": "",
                            "removed": "0",
                            "shortName": "二号线西西延伸",
                            "substituteConstruction": ""
                        },
                        {
                            "approvalDate": "",
                            "cityAllowance": "",
                            "completedFinalPrice": 0,
                            "contractPerson": "",
                            "contractPhone": "",
                            "createDate": "2013-01-29 12:30:40",
                            "dispatchNo": "",
                            "id": "8a818e903b84d1e1013c84923607395b",
                            "inputOperationDate": "2010-04-08 00:00:00",
                            "keyword": "",
                            "line": null,
                            "moneySourceType": "",
                            "operateDate": "2013-02-19 09:20:06",
                            "overhaulContent": "",
                            "primaryDesignBudget": "",
                            "primaryDesignDate": "",
                            "primaryDesignMoney": "",
                            "primaryDesignNo": "",
                            "primaryDesignRemark": "",
                            "professionalType": "",
                            "projectAdddept": "二号线东延伸项目公司",
                            "projectAdddeptId": "2522",
                            "projectAddperson": "刘纯洁",
                            "projectAddpersonId": "3922",
                            "projectAddtimeDate": "",
                            "projectAttachId": "",
                            "projectBudgetAll": "934100.000000",
                            "projectCheckunit": "",
                            "projectCompany": "上海轨道交通二号线东延伸发展有限公司",
                            "projectCompanyId": "2522",
                            "projectCreateunit": "",
                            "projectDestoryDate": "",
                            "projectEndtimePlanDate": "",
                            "projectEstimateAll": "",
                            "projectExecuteunit": "",
                            "projectExplain": "",
                            "projectFeasibilityBudget": "",
                            "projectFeasibilityDate": "",
                            "projectFeasibilityNo": "发改投资（2007）1314号",
                            "projectFeasibilityRemark": "",
                            "projectMoneysource": {
                                "moneySource": [
                                    {
                                        "unitId": "24",
                                        "unitName": "上海轨道交通二号线东延伸发展有限公司",
                                        "lineName": "2号线",
                                        "money": "110.3200"
                                    }
                                ]
                            },
                            "projectName": "上海市轨道交通2号线东延伸段工程",
                            "projectNo": "2007-2号线东延伸-002",
                            "projectStarttimePlanDate": "",
                            "projectState": "",
                            "projectType": "1",
                            "projectType2": "",
                            "remark": "工可批准文号 发改投资（2007）1314号\r\n初步设计批准文号 沪建交（0）号 ",
                            "removed": "0",
                            "shortName": "二号线东延伸",
                            "substituteConstruction": ""
                        }
                    ],
                    "remarks": "",
                    "shortName": "2号线",
                    "stations": [
                        {
                            "code": "10",
                            "id": "45",
                            "line": null,
                            "name": "静安寺站",
                            "remarks": ""
                        },
                        {
                            "code": "28",
                            "id": "63",
                            "line": null,
                            "name": "远东大道站",
                            "remarks": ""
                        },
                        {
                            "code": "97",
                            "id": "69",
                            "line": null,
                            "name": "川沙基地",
                            "remarks": ""
                        },
                        {
                            "code": "05",
                            "id": "40",
                            "line": null,
                            "name": "北新泾站",
                            "remarks": ""
                        },
                        {
                            "code": "14",
                            "id": "49",
                            "line": null,
                            "name": "陆家嘴站",
                            "remarks": ""
                        },
                        {
                            "code": "12",
                            "id": "47",
                            "line": null,
                            "name": "人民广场站",
                            "remarks": ""
                        },
                        {
                            "code": "19",
                            "id": "54",
                            "line": null,
                            "name": "龙阳路站",
                            "remarks": ""
                        },
                        {
                            "code": "02",
                            "id": "37",
                            "line": null,
                            "name": "虹桥火车站站",
                            "remarks": ""
                        },
                        {
                            "code": "15",
                            "id": "50",
                            "line": null,
                            "name": "东昌路站",
                            "remarks": ""
                        },
                        {
                            "code": "99",
                            "id": "71",
                            "line": null,
                            "name": "龙阳路基地",
                            "remarks": ""
                        },
                        {
                            "code": "11",
                            "id": "46",
                            "line": null,
                            "name": "南京西路站",
                            "remarks": ""
                        },
                        {
                            "code": "18",
                            "id": "53",
                            "line": null,
                            "name": "世纪公园站",
                            "remarks": ""
                        },
                        {
                            "code": "20",
                            "id": "55",
                            "line": null,
                            "name": "张江高科站",
                            "remarks": ""
                        },
                        {
                            "code": "27",
                            "id": "62",
                            "line": null,
                            "name": "凌空路站",
                            "remarks": ""
                        },
                        {
                            "code": "06",
                            "id": "41",
                            "line": null,
                            "name": "威宁路站",
                            "remarks": ""
                        },
                        {
                            "code": "03",
                            "id": "38",
                            "line": null,
                            "name": "虹桥2号航站楼站",
                            "remarks": ""
                        },
                        {
                            "code": "22",
                            "id": "57",
                            "line": null,
                            "name": "广兰路站",
                            "remarks": ""
                        },
                        {
                            "code": "00",
                            "id": "72",
                            "line": null,
                            "name": "新闸路控制中心",
                            "remarks": ""
                        },
                        {
                            "code": "30",
                            "id": "65",
                            "line": null,
                            "name": "浦东国际机场站",
                            "remarks": ""
                        },
                        {
                            "code": "07",
                            "id": "42",
                            "line": null,
                            "name": "娄山关路站",
                            "remarks": ""
                        },
                        {
                            "code": "04",
                            "id": "39",
                            "line": null,
                            "name": "淞虹路站",
                            "remarks": ""
                        },
                        {
                            "code": "23",
                            "id": "58",
                            "line": null,
                            "name": "唐镇站",
                            "remarks": ""
                        },
                        {
                            "code": "09",
                            "id": "44",
                            "line": null,
                            "name": "江苏路站",
                            "remarks": ""
                        },
                        {
                            "code": "17",
                            "id": "52",
                            "line": null,
                            "name": "上海科技馆站",
                            "remarks": ""
                        },
                        {
                            "code": "25",
                            "id": "60",
                            "line": null,
                            "name": "华夏东路站",
                            "remarks": ""
                        },
                        {
                            "code": "01",
                            "id": "36",
                            "line": null,
                            "name": "徐泾东站",
                            "remarks": ""
                        },
                        {
                            "code": "32",
                            "id": "67",
                            "line": null,
                            "name": "浦东火车站站",
                            "remarks": ""
                        },
                        {
                            "code": "21",
                            "id": "56",
                            "line": null,
                            "name": "金科路站",
                            "remarks": ""
                        },
                        {
                            "code": "98",
                            "id": "70",
                            "line": null,
                            "name": "北翟路基地",
                            "remarks": ""
                        },
                        {
                            "code": "29",
                            "id": "64",
                            "line": null,
                            "name": "海天三路站",
                            "remarks": ""
                        },
                        {
                            "code": "08",
                            "id": "43",
                            "line": null,
                            "name": "中山公园站",
                            "remarks": ""
                        },
                        {
                            "code": "24",
                            "id": "59",
                            "line": null,
                            "name": "创新中路站",
                            "remarks": ""
                        },
                        {
                            "code": "77",
                            "id": "68",
                            "line": null,
                            "name": "全线",
                            "remarks": ""
                        },
                        {
                            "code": "31",
                            "id": "66",
                            "line": null,
                            "name": "飞翱路站",
                            "remarks": ""
                        },
                        {
                            "code": "16",
                            "id": "51",
                            "line": null,
                            "name": "世纪大道站",
                            "remarks": ""
                        },
                        {
                            "code": "26",
                            "id": "61",
                            "line": null,
                            "name": "川沙站",
                            "remarks": ""
                        },
                        {
                            "code": "13",
                            "id": "48",
                            "line": null,
                            "name": "南京东路站",
                            "remarks": ""
                        }
                    ]
                },
                "owner": "",
                "ownerOrganization": {
                    "id": "23",
                    "name": "上海轨道交通长宁线发展有限公司",
                    "order": "13",
                    "remarks": ""
                },
                "purchaseDate": "",
                "receiveDate": "2006-12-31 00:00:00",
                "remarks": "",
                "station": {
                    "code": "05",
                    "id": "40",
                    "line": {
                        "code": "02",
                        "id": "2",
                        "name": "轨道交通二号线",
                        "projects": [
                            {
                                "approvalDate": "",
                                "cityAllowance": "",
                                "completedFinalPrice": 10762964547.53,
                                "contractPerson": "",
                                "contractPhone": "",
                                "createDate": "2013-02-18 15:37:26",
                                "dispatchNo": "",
                                "id": "8a818e903ce01617013cec3c632202ec",
                                "inputOperationDate": "",
                                "keyword": "",
                                "line": null,
                                "moneySourceType": "",
                                "operateDate": "2013-09-22 11:03:11",
                                "overhaulContent": "",
                                "primaryDesignBudget": "1048387.000000",
                                "primaryDesignDate": "1997-01-20",
                                "primaryDesignMoney": "",
                                "primaryDesignNo": "沪建建规（97）第0057号",
                                "primaryDesignRemark": "",
                                "professionalType": "",
                                "projectAdddept": "集团",
                                "projectAdddeptId": "2500",
                                "projectAddperson": "朱沪生",
                                "projectAddpersonId": "2511",
                                "projectAddtimeDate": "",
                                "projectAttachId": "",
                                "projectBudgetAll": "1048387.000000",
                                "projectCheckunit": "",
                                "projectCompany": "上海申通地铁集团有限公司",
                                "projectCompanyId": "2500",
                                "projectCreateunit": "",
                                "projectDestoryDate": "",
                                "projectEndtimePlanDate": "",
                                "projectEstimateAll": "",
                                "projectExecuteunit": "",
                                "projectExplain": "",
                                "projectFeasibilityBudget": "1007000",
                                "projectFeasibilityDate": "1995-03-01",
                                "projectFeasibilityNo": "计投资【1995】210号",
                                "projectFeasibilityRemark": "",
                                "projectMoneysource": "",
                                "projectName": "上海地铁二号线一期工程",
                                "projectNo": "1996-2号线-001",
                                "projectStarttimePlanDate": "",
                                "projectState": "",
                                "projectType": "1",
                                "projectType2": "",
                                "remark": "",
                                "removed": "0",
                                "shortName": "二号线",
                                "substituteConstruction": ""
                            },
                            {
                                "approvalDate": "",
                                "cityAllowance": "",
                                "completedFinalPrice": 2433043036.07,
                                "contractPerson": "",
                                "contractPhone": "",
                                "createDate": "2013-02-18 15:39:00",
                                "dispatchNo": "",
                                "id": "8a818e903ce01617013cec3dd44602ed",
                                "inputOperationDate": "2006-12-30 00:00:00",
                                "keyword": "",
                                "line": null,
                                "moneySourceType": "",
                                "operateDate": "2013-02-19 08:49:25",
                                "overhaulContent": "",
                                "primaryDesignBudget": "",
                                "primaryDesignDate": "",
                                "primaryDesignMoney": "",
                                "primaryDesignNo": "",
                                "primaryDesignRemark": "",
                                "professionalType": "",
                                "projectAdddept": "长宁线项目公司",
                                "projectAdddeptId": "2521",
                                "projectAddperson": "严厚群",
                                "projectAddpersonId": "3910",
                                "projectAddtimeDate": "",
                                "projectAttachId": "",
                                "projectBudgetAll": "455900.000000",
                                "projectCheckunit": "",
                                "projectCompany": "上海轨道交通长宁线发展有限公司",
                                "projectCompanyId": "2521",
                                "projectCreateunit": "",
                                "projectDestoryDate": "",
                                "projectEndtimePlanDate": "",
                                "projectEstimateAll": "",
                                "projectExecuteunit": "",
                                "projectExplain": "",
                                "projectFeasibilityBudget": "",
                                "projectFeasibilityDate": "",
                                "projectFeasibilityNo": "沪计城（2001）562号",
                                "projectFeasibilityRemark": "",
                                "projectMoneysource": "",
                                "projectName": "地铁二号线西延伸工程",
                                "projectNo": "2001-2号线西延伸-001",
                                "projectStarttimePlanDate": "",
                                "projectState": "",
                                "projectType": "1",
                                "projectType2": "",
                                "remark": "",
                                "removed": "0",
                                "shortName": "二号线西延伸",
                                "substituteConstruction": ""
                            },
                            {
                                "approvalDate": "",
                                "cityAllowance": "",
                                "completedFinalPrice": 0,
                                "contractPerson": "",
                                "contractPhone": "",
                                "createDate": "2013-02-18 15:40:36",
                                "dispatchNo": "",
                                "id": "8a818e903ce01617013cec3f498002ee",
                                "inputOperationDate": "",
                                "keyword": "",
                                "line": null,
                                "moneySourceType": "",
                                "operateDate": "2013-02-19 08:49:44",
                                "overhaulContent": "",
                                "primaryDesignBudget": "",
                                "primaryDesignDate": "",
                                "primaryDesignMoney": "",
                                "primaryDesignNo": "",
                                "primaryDesignRemark": "",
                                "professionalType": "",
                                "projectAdddept": "长宁线项目公司",
                                "projectAdddeptId": "2521",
                                "projectAddperson": "严厚群",
                                "projectAddpersonId": "3910",
                                "projectAddtimeDate": "",
                                "projectAttachId": "",
                                "projectBudgetAll": "193700.000000",
                                "projectCheckunit": "",
                                "projectCompany": "上海轨道交通长宁线发展有限公司",
                                "projectCompanyId": "2521",
                                "projectCreateunit": "",
                                "projectDestoryDate": "",
                                "projectEndtimePlanDate": "",
                                "projectEstimateAll": "",
                                "projectExecuteunit": "",
                                "projectExplain": "",
                                "projectFeasibilityBudget": "",
                                "projectFeasibilityDate": "",
                                "projectFeasibilityNo": "沪发改城（2008）59号",
                                "projectFeasibilityRemark": "",
                                "projectMoneysource": "",
                                "projectName": "轨道交通2号线西延伸淞虹路站-诸光路站工程",
                                "projectNo": "2008-2号线西西延伸-001",
                                "projectStarttimePlanDate": "",
                                "projectState": "",
                                "projectType": "1",
                                "projectType2": "",
                                "remark": "",
                                "removed": "0",
                                "shortName": "二号线西西延伸",
                                "substituteConstruction": ""
                            },
                            {
                                "approvalDate": "",
                                "cityAllowance": "",
                                "completedFinalPrice": 0,
                                "contractPerson": "",
                                "contractPhone": "",
                                "createDate": "2013-01-29 12:30:40",
                                "dispatchNo": "",
                                "id": "8a818e903b84d1e1013c84923607395b",
                                "inputOperationDate": "2010-04-08 00:00:00",
                                "keyword": "",
                                "line": null,
                                "moneySourceType": "",
                                "operateDate": "2013-02-19 09:20:06",
                                "overhaulContent": "",
                                "primaryDesignBudget": "",
                                "primaryDesignDate": "",
                                "primaryDesignMoney": "",
                                "primaryDesignNo": "",
                                "primaryDesignRemark": "",
                                "professionalType": "",
                                "projectAdddept": "二号线东延伸项目公司",
                                "projectAdddeptId": "2522",
                                "projectAddperson": "刘纯洁",
                                "projectAddpersonId": "3922",
                                "projectAddtimeDate": "",
                                "projectAttachId": "",
                                "projectBudgetAll": "934100.000000",
                                "projectCheckunit": "",
                                "projectCompany": "上海轨道交通二号线东延伸发展有限公司",
                                "projectCompanyId": "2522",
                                "projectCreateunit": "",
                                "projectDestoryDate": "",
                                "projectEndtimePlanDate": "",
                                "projectEstimateAll": "",
                                "projectExecuteunit": "",
                                "projectExplain": "",
                                "projectFeasibilityBudget": "",
                                "projectFeasibilityDate": "",
                                "projectFeasibilityNo": "发改投资（2007）1314号",
                                "projectFeasibilityRemark": "",
                                "projectMoneysource": {
                                    "moneySource": [
                                        {
                                            "unitId": "24",
                                            "unitName": "上海轨道交通二号线东延伸发展有限公司",
                                            "lineName": "2号线",
                                            "money": "110.3200"
                                        }
                                    ]
                                },
                                "projectName": "上海市轨道交通2号线东延伸段工程",
                                "projectNo": "2007-2号线东延伸-002",
                                "projectStarttimePlanDate": "",
                                "projectState": "",
                                "projectType": "1",
                                "projectType2": "",
                                "remark": "工可批准文号 发改投资（2007）1314号\r\n初步设计批准文号 沪建交（0）号 ",
                                "removed": "0",
                                "shortName": "二号线东延伸",
                                "substituteConstruction": ""
                            }
                        ],
                        "remarks": "",
                        "shortName": "2号线",
                        "stations": [
                            {
                                "code": "10",
                                "id": "45",
                                "line": null,
                                "name": "静安寺站",
                                "remarks": ""
                            },
                            {
                                "code": "28",
                                "id": "63",
                                "line": null,
                                "name": "远东大道站",
                                "remarks": ""
                            },
                            {
                                "code": "97",
                                "id": "69",
                                "line": null,
                                "name": "川沙基地",
                                "remarks": ""
                            },
                            null,
                            {
                                "code": "14",
                                "id": "49",
                                "line": null,
                                "name": "陆家嘴站",
                                "remarks": ""
                            },
                            {
                                "code": "12",
                                "id": "47",
                                "line": null,
                                "name": "人民广场站",
                                "remarks": ""
                            },
                            {
                                "code": "19",
                                "id": "54",
                                "line": null,
                                "name": "龙阳路站",
                                "remarks": ""
                            },
                            {
                                "code": "02",
                                "id": "37",
                                "line": null,
                                "name": "虹桥火车站站",
                                "remarks": ""
                            },
                            {
                                "code": "15",
                                "id": "50",
                                "line": null,
                                "name": "东昌路站",
                                "remarks": ""
                            },
                            {
                                "code": "99",
                                "id": "71",
                                "line": null,
                                "name": "龙阳路基地",
                                "remarks": ""
                            },
                            {
                                "code": "11",
                                "id": "46",
                                "line": null,
                                "name": "南京西路站",
                                "remarks": ""
                            },
                            {
                                "code": "18",
                                "id": "53",
                                "line": null,
                                "name": "世纪公园站",
                                "remarks": ""
                            },
                            {
                                "code": "20",
                                "id": "55",
                                "line": null,
                                "name": "张江高科站",
                                "remarks": ""
                            },
                            {
                                "code": "27",
                                "id": "62",
                                "line": null,
                                "name": "凌空路站",
                                "remarks": ""
                            },
                            {
                                "code": "06",
                                "id": "41",
                                "line": null,
                                "name": "威宁路站",
                                "remarks": ""
                            },
                            {
                                "code": "03",
                                "id": "38",
                                "line": null,
                                "name": "虹桥2号航站楼站",
                                "remarks": ""
                            },
                            {
                                "code": "22",
                                "id": "57",
                                "line": null,
                                "name": "广兰路站",
                                "remarks": ""
                            },
                            {
                                "code": "00",
                                "id": "72",
                                "line": null,
                                "name": "新闸路控制中心",
                                "remarks": ""
                            },
                            {
                                "code": "30",
                                "id": "65",
                                "line": null,
                                "name": "浦东国际机场站",
                                "remarks": ""
                            },
                            {
                                "code": "07",
                                "id": "42",
                                "line": null,
                                "name": "娄山关路站",
                                "remarks": ""
                            },
                            {
                                "code": "04",
                                "id": "39",
                                "line": null,
                                "name": "淞虹路站",
                                "remarks": ""
                            },
                            {
                                "code": "23",
                                "id": "58",
                                "line": null,
                                "name": "唐镇站",
                                "remarks": ""
                            },
                            {
                                "code": "09",
                                "id": "44",
                                "line": null,
                                "name": "江苏路站",
                                "remarks": ""
                            },
                            {
                                "code": "17",
                                "id": "52",
                                "line": null,
                                "name": "上海科技馆站",
                                "remarks": ""
                            },
                            {
                                "code": "25",
                                "id": "60",
                                "line": null,
                                "name": "华夏东路站",
                                "remarks": ""
                            },
                            {
                                "code": "01",
                                "id": "36",
                                "line": null,
                                "name": "徐泾东站",
                                "remarks": ""
                            },
                            {
                                "code": "32",
                                "id": "67",
                                "line": null,
                                "name": "浦东火车站站",
                                "remarks": ""
                            },
                            {
                                "code": "21",
                                "id": "56",
                                "line": null,
                                "name": "金科路站",
                                "remarks": ""
                            },
                            {
                                "code": "98",
                                "id": "70",
                                "line": null,
                                "name": "北翟路基地",
                                "remarks": ""
                            },
                            {
                                "code": "29",
                                "id": "64",
                                "line": null,
                                "name": "海天三路站",
                                "remarks": ""
                            },
                            {
                                "code": "08",
                                "id": "43",
                                "line": null,
                                "name": "中山公园站",
                                "remarks": ""
                            },
                            {
                                "code": "24",
                                "id": "59",
                                "line": null,
                                "name": "创新中路站",
                                "remarks": ""
                            },
                            {
                                "code": "77",
                                "id": "68",
                                "line": null,
                                "name": "全线",
                                "remarks": ""
                            },
                            {
                                "code": "31",
                                "id": "66",
                                "line": null,
                                "name": "飞翱路站",
                                "remarks": ""
                            },
                            {
                                "code": "16",
                                "id": "51",
                                "line": null,
                                "name": "世纪大道站",
                                "remarks": ""
                            },
                            {
                                "code": "26",
                                "id": "61",
                                "line": null,
                                "name": "川沙站",
                                "remarks": ""
                            },
                            {
                                "code": "13",
                                "id": "48",
                                "line": null,
                                "name": "南京东路站",
                                "remarks": ""
                            }
                        ]
                    },
                    "name": "北新泾站",
                    "remarks": ""
                },
                "stopUseDate": "",
                "updateDate": "",
                "useOrganization": {
                    "id": "56",
                    "name": "上海地铁第二运营有限公司",
                    "order": "08",
                    "remarks": ""
                },
                "user": ""
            },
            "assetPic": "",
            "completeTransAssetType": "初始",
            "contract": null,
            "contractNo": "",
            "contractPrice": 32442,
            "count": 1,
            "createDate": "2013-11-26 17:01:42",
            "dataList": "有",
            "depreciationMethod": "直线法",
            "detailInstallSite": "北新泾站",
            "detailType": {
                "code": "01",
                "id": "110801",
                "name": "双电源配电箱",
                "parent": {
                    "code": "08",
                    "id": "1108",
                    "name": "票务电源设备",
                    "parent": {
                        "code": "11",
                        "id": "11",
                        "name": "自动售检票系统",
                        "parent": {
                            "code": "0",
                            "id": "0",
                            "name": "根节点",
                            "parent": null
                        }
                    }
                }
            },
            "equipmentList": "",
            "expectancyLife": 10,
            "factoryPrice": 32442,
            "id": "8a81aba3429394f0014293a488a91509",
            "madeDate": "2006-09-12 00:00:00",
            "mainType": {
                "code": "11",
                "id": "11",
                "name": "自动售检票系统",
                "parent": {
                    "code": "0",
                    "id": "0",
                    "name": "根节点",
                    "parent": null
                }
            },
            "manufactureCountry": "中国",
            "manufacturer": null,
            "name": "双电源自切配电箱",
            "operateProjectAsset": "",
            "operateProjectAssetDate": "",
            "originalValue": 49038.92,
            "overhaulFinalPrice": 0,
            "overhaulRate": "",
            "project": {
                "approvalDate": "",
                "cityAllowance": "",
                "completedFinalPrice": 2433043036.07,
                "contractPerson": "",
                "contractPhone": "",
                "createDate": "2013-02-18 15:39:00",
                "dispatchNo": "",
                "id": "8a818e903ce01617013cec3dd44602ed",
                "inputOperationDate": "2006-12-30 00:00:00",
                "keyword": "",
                "line": {
                    "code": "02",
                    "id": "2",
                    "name": "轨道交通二号线",
                    "projects": [
                        {
                            "approvalDate": "",
                            "cityAllowance": "",
                            "completedFinalPrice": 10762964547.53,
                            "contractPerson": "",
                            "contractPhone": "",
                            "createDate": "2013-02-18 15:37:26",
                            "dispatchNo": "",
                            "id": "8a818e903ce01617013cec3c632202ec",
                            "inputOperationDate": "",
                            "keyword": "",
                            "line": null,
                            "moneySourceType": "",
                            "operateDate": "2013-09-22 11:03:11",
                            "overhaulContent": "",
                            "primaryDesignBudget": "1048387.000000",
                            "primaryDesignDate": "1997-01-20",
                            "primaryDesignMoney": "",
                            "primaryDesignNo": "沪建建规（97）第0057号",
                            "primaryDesignRemark": "",
                            "professionalType": "",
                            "projectAdddept": "集团",
                            "projectAdddeptId": "2500",
                            "projectAddperson": "朱沪生",
                            "projectAddpersonId": "2511",
                            "projectAddtimeDate": "",
                            "projectAttachId": "",
                            "projectBudgetAll": "1048387.000000",
                            "projectCheckunit": "",
                            "projectCompany": "上海申通地铁集团有限公司",
                            "projectCompanyId": "2500",
                            "projectCreateunit": "",
                            "projectDestoryDate": "",
                            "projectEndtimePlanDate": "",
                            "projectEstimateAll": "",
                            "projectExecuteunit": "",
                            "projectExplain": "",
                            "projectFeasibilityBudget": "1007000",
                            "projectFeasibilityDate": "1995-03-01",
                            "projectFeasibilityNo": "计投资【1995】210号",
                            "projectFeasibilityRemark": "",
                            "projectMoneysource": "",
                            "projectName": "上海地铁二号线一期工程",
                            "projectNo": "1996-2号线-001",
                            "projectStarttimePlanDate": "",
                            "projectState": "",
                            "projectType": "1",
                            "projectType2": "",
                            "remark": "",
                            "removed": "0",
                            "shortName": "二号线",
                            "substituteConstruction": ""
                        },
                        null,
                        {
                            "approvalDate": "",
                            "cityAllowance": "",
                            "completedFinalPrice": 0,
                            "contractPerson": "",
                            "contractPhone": "",
                            "createDate": "2013-02-18 15:40:36",
                            "dispatchNo": "",
                            "id": "8a818e903ce01617013cec3f498002ee",
                            "inputOperationDate": "",
                            "keyword": "",
                            "line": null,
                            "moneySourceType": "",
                            "operateDate": "2013-02-19 08:49:44",
                            "overhaulContent": "",
                            "primaryDesignBudget": "",
                            "primaryDesignDate": "",
                            "primaryDesignMoney": "",
                            "primaryDesignNo": "",
                            "primaryDesignRemark": "",
                            "professionalType": "",
                            "projectAdddept": "长宁线项目公司",
                            "projectAdddeptId": "2521",
                            "projectAddperson": "严厚群",
                            "projectAddpersonId": "3910",
                            "projectAddtimeDate": "",
                            "projectAttachId": "",
                            "projectBudgetAll": "193700.000000",
                            "projectCheckunit": "",
                            "projectCompany": "上海轨道交通长宁线发展有限公司",
                            "projectCompanyId": "2521",
                            "projectCreateunit": "",
                            "projectDestoryDate": "",
                            "projectEndtimePlanDate": "",
                            "projectEstimateAll": "",
                            "projectExecuteunit": "",
                            "projectExplain": "",
                            "projectFeasibilityBudget": "",
                            "projectFeasibilityDate": "",
                            "projectFeasibilityNo": "沪发改城（2008）59号",
                            "projectFeasibilityRemark": "",
                            "projectMoneysource": "",
                            "projectName": "轨道交通2号线西延伸淞虹路站-诸光路站工程",
                            "projectNo": "2008-2号线西西延伸-001",
                            "projectStarttimePlanDate": "",
                            "projectState": "",
                            "projectType": "1",
                            "projectType2": "",
                            "remark": "",
                            "removed": "0",
                            "shortName": "二号线西西延伸",
                            "substituteConstruction": ""
                        },
                        {
                            "approvalDate": "",
                            "cityAllowance": "",
                            "completedFinalPrice": 0,
                            "contractPerson": "",
                            "contractPhone": "",
                            "createDate": "2013-01-29 12:30:40",
                            "dispatchNo": "",
                            "id": "8a818e903b84d1e1013c84923607395b",
                            "inputOperationDate": "2010-04-08 00:00:00",
                            "keyword": "",
                            "line": null,
                            "moneySourceType": "",
                            "operateDate": "2013-02-19 09:20:06",
                            "overhaulContent": "",
                            "primaryDesignBudget": "",
                            "primaryDesignDate": "",
                            "primaryDesignMoney": "",
                            "primaryDesignNo": "",
                            "primaryDesignRemark": "",
                            "professionalType": "",
                            "projectAdddept": "二号线东延伸项目公司",
                            "projectAdddeptId": "2522",
                            "projectAddperson": "刘纯洁",
                            "projectAddpersonId": "3922",
                            "projectAddtimeDate": "",
                            "projectAttachId": "",
                            "projectBudgetAll": "934100.000000",
                            "projectCheckunit": "",
                            "projectCompany": "上海轨道交通二号线东延伸发展有限公司",
                            "projectCompanyId": "2522",
                            "projectCreateunit": "",
                            "projectDestoryDate": "",
                            "projectEndtimePlanDate": "",
                            "projectEstimateAll": "",
                            "projectExecuteunit": "",
                            "projectExplain": "",
                            "projectFeasibilityBudget": "",
                            "projectFeasibilityDate": "",
                            "projectFeasibilityNo": "发改投资（2007）1314号",
                            "projectFeasibilityRemark": "",
                            "projectMoneysource": {
                                "moneySource": [
                                    {
                                        "unitId": "24",
                                        "unitName": "上海轨道交通二号线东延伸发展有限公司",
                                        "lineName": "2号线",
                                        "money": "110.3200"
                                    }
                                ]
                            },
                            "projectName": "上海市轨道交通2号线东延伸段工程",
                            "projectNo": "2007-2号线东延伸-002",
                            "projectStarttimePlanDate": "",
                            "projectState": "",
                            "projectType": "1",
                            "projectType2": "",
                            "remark": "工可批准文号 发改投资（2007）1314号\r\n初步设计批准文号 沪建交（0）号 ",
                            "removed": "0",
                            "shortName": "二号线东延伸",
                            "substituteConstruction": ""
                        }
                    ],
                    "remarks": "",
                    "shortName": "2号线",
                    "stations": [
                        {
                            "code": "10",
                            "id": "45",
                            "line": null,
                            "name": "静安寺站",
                            "remarks": ""
                        },
                        {
                            "code": "28",
                            "id": "63",
                            "line": null,
                            "name": "远东大道站",
                            "remarks": ""
                        },
                        {
                            "code": "97",
                            "id": "69",
                            "line": null,
                            "name": "川沙基地",
                            "remarks": ""
                        },
                        {
                            "code": "05",
                            "id": "40",
                            "line": null,
                            "name": "北新泾站",
                            "remarks": ""
                        },
                        {
                            "code": "14",
                            "id": "49",
                            "line": null,
                            "name": "陆家嘴站",
                            "remarks": ""
                        },
                        {
                            "code": "12",
                            "id": "47",
                            "line": null,
                            "name": "人民广场站",
                            "remarks": ""
                        },
                        {
                            "code": "19",
                            "id": "54",
                            "line": null,
                            "name": "龙阳路站",
                            "remarks": ""
                        },
                        {
                            "code": "02",
                            "id": "37",
                            "line": null,
                            "name": "虹桥火车站站",
                            "remarks": ""
                        },
                        {
                            "code": "15",
                            "id": "50",
                            "line": null,
                            "name": "东昌路站",
                            "remarks": ""
                        },
                        {
                            "code": "99",
                            "id": "71",
                            "line": null,
                            "name": "龙阳路基地",
                            "remarks": ""
                        },
                        {
                            "code": "11",
                            "id": "46",
                            "line": null,
                            "name": "南京西路站",
                            "remarks": ""
                        },
                        {
                            "code": "18",
                            "id": "53",
                            "line": null,
                            "name": "世纪公园站",
                            "remarks": ""
                        },
                        {
                            "code": "20",
                            "id": "55",
                            "line": null,
                            "name": "张江高科站",
                            "remarks": ""
                        },
                        {
                            "code": "27",
                            "id": "62",
                            "line": null,
                            "name": "凌空路站",
                            "remarks": ""
                        },
                        {
                            "code": "06",
                            "id": "41",
                            "line": null,
                            "name": "威宁路站",
                            "remarks": ""
                        },
                        {
                            "code": "03",
                            "id": "38",
                            "line": null,
                            "name": "虹桥2号航站楼站",
                            "remarks": ""
                        },
                        {
                            "code": "22",
                            "id": "57",
                            "line": null,
                            "name": "广兰路站",
                            "remarks": ""
                        },
                        {
                            "code": "00",
                            "id": "72",
                            "line": null,
                            "name": "新闸路控制中心",
                            "remarks": ""
                        },
                        {
                            "code": "30",
                            "id": "65",
                            "line": null,
                            "name": "浦东国际机场站",
                            "remarks": ""
                        },
                        {
                            "code": "07",
                            "id": "42",
                            "line": null,
                            "name": "娄山关路站",
                            "remarks": ""
                        },
                        {
                            "code": "04",
                            "id": "39",
                            "line": null,
                            "name": "淞虹路站",
                            "remarks": ""
                        },
                        {
                            "code": "23",
                            "id": "58",
                            "line": null,
                            "name": "唐镇站",
                            "remarks": ""
                        },
                        {
                            "code": "09",
                            "id": "44",
                            "line": null,
                            "name": "江苏路站",
                            "remarks": ""
                        },
                        {
                            "code": "17",
                            "id": "52",
                            "line": null,
                            "name": "上海科技馆站",
                            "remarks": ""
                        },
                        {
                            "code": "25",
                            "id": "60",
                            "line": null,
                            "name": "华夏东路站",
                            "remarks": ""
                        },
                        {
                            "code": "01",
                            "id": "36",
                            "line": null,
                            "name": "徐泾东站",
                            "remarks": ""
                        },
                        {
                            "code": "32",
                            "id": "67",
                            "line": null,
                            "name": "浦东火车站站",
                            "remarks": ""
                        },
                        {
                            "code": "21",
                            "id": "56",
                            "line": null,
                            "name": "金科路站",
                            "remarks": ""
                        },
                        {
                            "code": "98",
                            "id": "70",
                            "line": null,
                            "name": "北翟路基地",
                            "remarks": ""
                        },
                        {
                            "code": "29",
                            "id": "64",
                            "line": null,
                            "name": "海天三路站",
                            "remarks": ""
                        },
                        {
                            "code": "08",
                            "id": "43",
                            "line": null,
                            "name": "中山公园站",
                            "remarks": ""
                        },
                        {
                            "code": "24",
                            "id": "59",
                            "line": null,
                            "name": "创新中路站",
                            "remarks": ""
                        },
                        {
                            "code": "77",
                            "id": "68",
                            "line": null,
                            "name": "全线",
                            "remarks": ""
                        },
                        {
                            "code": "31",
                            "id": "66",
                            "line": null,
                            "name": "飞翱路站",
                            "remarks": ""
                        },
                        {
                            "code": "16",
                            "id": "51",
                            "line": null,
                            "name": "世纪大道站",
                            "remarks": ""
                        },
                        {
                            "code": "26",
                            "id": "61",
                            "line": null,
                            "name": "川沙站",
                            "remarks": ""
                        },
                        {
                            "code": "13",
                            "id": "48",
                            "line": null,
                            "name": "南京东路站",
                            "remarks": ""
                        }
                    ]
                },
                "moneySourceType": "",
                "operateDate": "2013-02-19 08:49:25",
                "overhaulContent": "",
                "primaryDesignBudget": "",
                "primaryDesignDate": "",
                "primaryDesignMoney": "",
                "primaryDesignNo": "",
                "primaryDesignRemark": "",
                "professionalType": "",
                "projectAdddept": "长宁线项目公司",
                "projectAdddeptId": "2521",
                "projectAddperson": "严厚群",
                "projectAddpersonId": "3910",
                "projectAddtimeDate": "",
                "projectAttachId": "",
                "projectBudgetAll": "455900.000000",
                "projectCheckunit": "",
                "projectCompany": "上海轨道交通长宁线发展有限公司",
                "projectCompanyId": "2521",
                "projectCreateunit": "",
                "projectDestoryDate": "",
                "projectEndtimePlanDate": "",
                "projectEstimateAll": "",
                "projectExecuteunit": "",
                "projectExplain": "",
                "projectFeasibilityBudget": "",
                "projectFeasibilityDate": "",
                "projectFeasibilityNo": "沪计城（2001）562号",
                "projectFeasibilityRemark": "",
                "projectMoneysource": "",
                "projectName": "地铁二号线西延伸工程",
                "projectNo": "2001-2号线西延伸-001",
                "projectStarttimePlanDate": "",
                "projectState": "",
                "projectType": "1",
                "projectType2": "",
                "remark": "",
                "removed": "0",
                "shortName": "二号线西延伸",
                "substituteConstruction": ""
            },
            "projectAppDocNo": "沪计城（2001）562号",
            "projectNo": "",
            "remarks": "",
            "state": {
                "assetId": "",
                "createDate": "2013-11-26 17:01:42",
                "id": "8a81aba3429394f0014293a488a91508",
                "nameplateSite": "",
                "remarks": "",
                "state": "1",
                "updateDate": "",
                "useState": "使用"
            },
            "subType": {
                "code": "08",
                "id": "1108",
                "name": "票务电源设备",
                "parent": {
                    "code": "11",
                    "id": "11",
                    "name": "自动售检票系统",
                    "parent": {
                        "code": "0",
                        "id": "0",
                        "name": "根节点",
                        "parent": null
                    }
                }
            },
            "supplier": null,
            "type": "DKS-ZQ",
            "unit": {
                "conversion": "",
                "id": "8a81aba34202905c0142029412d20092",
                "name": "套",
                "seniorUnit": ""
            },
            "updateDate": "",
            "useDate": "2006-09-12 00:00:00",
            "useLife": 10,
            "verifyState": "2",
            "warrantyPeriod": "2008-12-31 00:00:00"
        }
    ],
    "TotalRecordCount": 1
}
```
