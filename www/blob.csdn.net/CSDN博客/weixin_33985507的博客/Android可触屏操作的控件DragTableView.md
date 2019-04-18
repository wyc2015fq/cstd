# Android可触屏操作的控件DragTableView - weixin_33985507的博客 - CSDN博客
2016年09月27日 16:49:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：8
> 
先上效果图
![2113387-705288f2c2690e09.gif](https://upload-images.jianshu.io/upload_images/2113387-705288f2c2690e09.gif)
drag_table.gif
> 
下面是github地址，jcenter因为升级了AS打包好像出了点问题，又掉坑里了T.T
[https://github.com/Linyuzai/Demo4TableView](https://link.jianshu.com?t=https://github.com/Linyuzai/Demo4TableView)
> 
可以用手拖动item进行位置的交换，在RecyclerView的基础上改进的，然后简单介绍一下，因为是RecyclerView肯定要Adapter
```
int getRowCount();//行数
int getColCount();//列数
int getTableItemViewType(int row, int col);//item的类型
VH onCreateTableViewHolder(ViewGroup parent, int viewType);
void onBindTableViewHolder(VH holder, int row, int col);
boolean isDraggable(int row, int col);//所在行列能不能拖动
```
> 
然后是两个监听器
```
public interface OnTableItemClickListener {    
    void onTableItemClick(View view, int row, int col);
}
public interface OnTableItemDragListener {    
    //手指按下某个item就会触发
    void onFromItemSelected(View view, int row, int col);    
    //移动时返回当前松手将会交换的item
    void onItemDragged(View toView, int toViewRow, int toViewCol);    
    //手指松开时，被交换的item
    void onToItemSelected(View view, int row, int col);
}
```
> 
下面是一些方法
```
public boolean isDraggable();//能否拖动
public void setDraggable(boolean draggable);//设置能否拖动
public TableAdapter getAdapter();
public void setAdapter(TableAdapter adapter);
public OnTableItemDragListener getOnTableItemDragListener();
public void setOnTableItemDragListener(OnTableItemDragListener dragListener);
public OnTableItemClickListener getOnTableItemClickListener();
public void setOnTableItemClickListener(OnTableItemClickListener clickListener);
public TableView getTableView();//得到里面的RecyclerView，有什么需求可以自己实现
public void setItemAnimator(RecyclerView.ItemAnimator itemAnimator);
public void addItemDecoration(RecyclerView.ItemDecoration itemDecoration);//
public void addItemDecoration(RecyclerView.ItemDecoration itemDecoration, int index);//
public void moveItem(int fromRow, int fromCol, int toRow, int toCol);//非触屏交换item
public void reset(boolean animation);//重置，true有动画效果，像上面的效果那样
```
> 
其实之前有试过用ViewDragHelper，但是。。。一堆的bug，于是放弃了，也许是我没写好，回头再去恶补一下。
我还是先把jcenter搞定，坑爹的玩意儿（无爱脸）
