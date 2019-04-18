# recyclerview每次刷新数据后都会定位在第一条 - weixin_33985507的博客 - CSDN博客
2018年02月27日 10:09:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：118
找了半天才发现自己蠢的不行
刷新数据不能每次都new adapter  并且不能 每次都setadapter
if (mHomeBean.getBallList().size() > 0) {
if (page == 1 && !isRefresh) {
//第一次需要new出Adapter
adapter = new LivingHomeRecyclerAdapter(activity, mLivingList);
RecyclerView.LayoutManager mLayoutManager = new LinearLayoutManager(activity);
mRecyclerView.setLayoutManager(mLayoutManager);
mRecyclerView.setAdapter(adapter);
}
//                            mLivingList.addAll(mHomeBean.getBallList());
List<LivingHomeBean.BallListBean> list = new ArrayList<>();
list = mHomeBean.getBallList();
adapter.setList(list);
`                    }`
