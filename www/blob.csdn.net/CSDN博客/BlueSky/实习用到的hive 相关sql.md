# 实习用到的hive 相关sql - BlueSky - CSDN博客
2016年07月21日 19:24:36[ASIA_kobe](https://me.csdn.net/ASIA_kobe)阅读数：447
a.dt as `日期`,--日期
get_json_object(val_val,'$.ad_id')  as ad_id, --获取json内容
case order_from 
                when 1 then 'app'
                when 2 then 'pc'
            end as order_from,--订单来源（判断）
if(e.branch_name is not null,e.branch_name,'非品牌') as `品牌名称`,a.ad_inlet as `广告入口`,--if判断语句
case 
when (city_name in('北京','广州','深圳','杭州','西安','武汉','成都','天津') and ad_suc_ord_total_amt>0 and ad_suc_ord_total_amt<1) then '0-1'
when (city_name in('北京','广州','深圳','杭州','西安','武汉','成都','天津','南京','上海','厦门','福州') and ad_suc_ord_total_amt>=1 and ad_suc_ord_total_amt<1.5) then '1-1' end as price_ctrl
```
select
	a.prom_date as `推广日期`,--推广日期
	a.city as `城市名称`,--城市名称
	a.price_ctrl as `价格区间`,
	a.poi_id as `商家ID`,--商家id
	a.poi_name as `商家名称`,--商家名称
	b.category_l1 as `一级品类名称`,--一级品类名称
	b.category_l2 as `二级品类名称`,--二级品类名称
	b.category_l3 as `三级品类名称`,--三级品类名称
	e.brand_id  as `品牌ID`,
	if(e.branch_name is not null,e.branch_name,'非品牌') as `品牌名称`,
	a.ad_inlet as `广告入口`,
	a.ad_site_order as `广告排名`,
	a.ad_orig_price_expend as`广告原价消耗`,
	a.ad_actl_price_expend as `广告实际消耗`,
	a.orig_price_ad_chnl as `外卖实际支付交易额(广告)`
from
	(
		select
			dt as prom_date,--推广日期
			city_name as city,--城市名称
			poi_id,--商家id
			poi_name,--商家名称
			ad_entry as ad_inlet,--广告入口
			ad_pos as ad_site_order,--广告排名
			ad_suc_ord_original_amt as ad_orig_price_expend,--广告原价消耗
			ad_suc_ord_total_amt as ad_actl_price_expend,--广告实际消耗
			ad_wm_sub_total_amt as orig_price_ad_chnl,--外卖实际支付交易金额
			case 
			when (city_name in('北京','广州','深圳','杭州','西安','武汉','成都','天津','南京','上海','厦门','福州') and ad_suc_ord_total_amt>0 and ad_suc_ord_total_amt<1) then '0-1'
			when (city_name in('北京','广州','深圳','杭州','西安','武汉','成都','天津','南京','上海','厦门','福州') and ad_suc_ord_total_amt>=1 and ad_suc_ord_total_amt<1.5) then '1-1'
			when (city_name in('北京','广州','深圳','杭州','西安','武汉','成都','天津','南京','上海','厦门','福州') and ad_suc_ord_total_amt>=1.5 and ad_suc_ord_total_amt<10.5) then '2-10'
			when (city_name in('北京','广州','深圳','杭州','西安','武汉','成都','天津','南京','上海','厦门','福州') and ad_suc_ord_total_amt>=10.5 and ad_suc_ord_total_amt<30.5) then '11-30'
			when (city_name in('北京','广州','深圳','杭州','西安','武汉','成都','天津','南京','上海','厦门','福州') and ad_suc_ord_total_amt>=30.5 and ad_suc_ord_total_amt<50.5) then '31-50'
			when (city_name in('北京','广州','深圳','杭州','西安','武汉','成都','天津','南京','上海','厦门','福州') and ad_suc_ord_total_amt>=50.5 and ad_suc_ord_total_amt<80.5) then '51-80'
			when (city_name in('北京','广州','深圳','杭州','西安','武汉','成都','天津','南京','上海','厦门','福州') and ad_suc_ord_total_amt>=80.5 and ad_suc_ord_total_amt<120.5) then '81-120'
			when (city_name in('北京','广州','深圳','杭州','西安','武汉','成都','天津','南京','上海','厦门','福州') and ad_suc_ord_total_amt>=120.5 and ad_suc_ord_total_amt<200.5) then '121-200'
			when (city_name in('北京','广州','深圳','杭州','西安','武汉','成都','天津','南京','上海','厦门','福州') and ad_suc_ord_total_amt>=200.5) then '201-'
			
			when (city_name in('郑州','昆明','重庆','常州','无锡','温州') and ad_suc_ord_total_amt>0 and ad_suc_ord_total_amt<1) then '0-1'
			when (city_name in('郑州','昆明','重庆','常州','无锡','温州') and ad_suc_ord_total_amt=1 and ad_suc_ord_total_amt<1.5) then '1-1'
			when (city_name in('郑州','昆明','重庆','常州','无锡','温州') and ad_suc_ord_total_amt>=1.5 and ad_suc_ord_total_amt<5.5) then '2-5'
			when (city_name in('郑州','昆明','重庆','常州','无锡','温州') and ad_suc_ord_total_amt>=5.5 and ad_suc_ord_total_amt<10.5) then '6-10'
			when (city_name in('郑州','昆明','重庆','常州','无锡','温州') and ad_suc_ord_total_amt>=10.5 and ad_suc_ord_total_amt<20.5) then '11-20'
			when (city_name in('郑州','昆明','重庆','常州','无锡','温州') and ad_suc_ord_total_amt>=20.5 and ad_suc_ord_total_amt<50.5) then '21-50'
			when (city_name in('郑州','昆明','重庆','常州','无锡','温州') and ad_suc_ord_total_amt>=50.5 and ad_suc_ord_total_amt<80.5) then '51-80'
			when (city_name in('郑州','昆明','重庆','常州','无锡','温州') and ad_suc_ord_total_amt>=80.5 ) then '81-'
			when (city_name in('绍兴','镇江') and ad_suc_ord_total_amt>0 and ad_suc_ord_total_amt<1) then '0-1'
			when (city_name in('绍兴','镇江') and ad_suc_ord_total_amt=1 and ad_suc_ord_total_amt<1.5) then '1-1'
			when (city_name in('绍兴','镇江') and ad_suc_ord_total_amt>=1.5 and ad_suc_ord_total_amt<5.5) then '2-5'
			when (city_name in('绍兴','镇江') and ad_suc_ord_total_amt>=5.5 and ad_suc_ord_total_amt<10.5) then '6-10'
			when (city_name in('绍兴','镇江') and ad_suc_ord_total_amt>=10.5 and ad_suc_ord_total_amt<20.5) then '11-20'
			when (city_name in('绍兴','镇江') and ad_suc_ord_total_amt>=20.5 and ad_suc_ord_total_amt<30.5) then '21-30'
			when (city_name in('绍兴','镇江') and ad_suc_ord_total_amt>=30.5 and ad_suc_ord_total_amt<40.5) then '31-40'
			when (city_name in('绍兴','镇江') and ad_suc_ord_total_amt>=40.5) then '41-'
			end as price_ctrl
		from
			mart_waimai.topic_dt__wm_ad_poieffect_adpos_entry_poi_ctype
		where 
			city_id<>'-1' and ctype='-1'  and poi_id<>'-1' and dt between 20160701 and 20160711
	)a
left outer join
(
	select
		dt,
  		poi_id,--商家id
		first_tag_name as category_l1,--一级品类名称
		second_tag_name as category_l2,--二级品类名称
		third_tag_name as category_l3--三级品类名称
	from
		mart_waimai.dim_wm_poi_tag_his
	where 
		dt between 20160701 and 20160711
)b
on 
	a.prom_date=b.dt and a.poi_id = b.poi_id
left outer join
(	select
		c.poi_id as poi_id,--商家id
		c.brand_id as brand_id,--商家名称
		d.branch_name as branch_name,--品牌名称
		c.dt as dt
	from
		(	--商家名称和商家城市名称
			select
				poi_id,--商家id
				poi_name,--商家名称
				brand_id,--品牌id
				dt
			from 
				mart_waimai.dim_poi_level_his
			where 
				dt between 20160701 and 20160711
		)c
		left outer join
		(	--品牌ID和品牌名称
			select
				brand_id,
				branch_name,
				dt
			from 
				mart_waimai.dim_ka_attribute_his
			where 
				dt between 20160701 and 20160711
		)d
		on 
			c.dt=d.dt and c.brand_id=d.brand_id
)e
on 
	a.prom_date=e.dt and a.poi_id = e.poi_id
limit 3000;
```
