# PartitionMotionSearch()函数 - maopig的专栏 - CSDN博客
2011年06月08日 23:41:00[maopig](https://me.csdn.net/maopig)阅读数：1024
![](http://hi.csdn.net/attachment/201106/8/0_1307547665Iucm.gif)
encode_one_macroblock()函数中的运动估计分为两大块，对于宏块级的三种模式，分块后直接对patition依次调用PartitionMotionSearch()函数；而对于亚宏块级的（含8x8, 8x4,4x8,4x4）模式，首先将宏块拆分为4个8×8子宏块，针对每个8×8子宏块调用PartitionMotionSearch()函数。
void 
PartitionMotionSearch (int    blocktype,  //块模式 
                   int    block8x8,  //当前partition在宏块的序号 
                   double lambda)   // λ 
{ 
static int  bx0[5][4] = {{0,0,0,0}, {0,0,0,0}, {0,0,0,0}, {0,2,0,0}, {0,2,0,2}}; 
static int  by0[5][4] = {{0,0,0,0}, {0,0,0,0}, {0,2,0,0}, {0,0,0,0}, {0,0,2,2}}; //参见注释[1] 
int   **ref_array, ***mv_array; //参考帧列表和运动向量列表 
int   parttype  = (blocktype<4?blocktype:4); 
int   step_h0   = (input->blc_size[ parttype][0]>>2); 
int   step_v0   = (input->blc_size[ parttype][1]>>2); 
//partition的尺寸，用于决定block的边界，对非P8×8模式无意义。 
int   step_h    = (input->blc_size[blocktype][0]>>2); //子块的尺寸 
int   step_v    = (input->blc_size[blocktype][1]>>2); 
int   v, h; // BlockMotionSearch()函数要处理的子块在当前宏块中的相对块坐标 
//以上尺寸全部以4×4block为单位
numlists=bslice?2:1; 
for (list=0; list 
{ 
for (ref=0; ref < listXsize[list+list_offset]; ref++) 
{ 
ref_array = enc_picture->ref_idx[list]; 
    mv_array  = enc_picture->mv[list]; 
 //遍历partition中的每一个 
for (v=by0[parttype][block8x8]; v        { 
          pic_block_y = img->block_y + v;// 当前子块在图像中的块坐标=当前宏块的块坐标+当前子块在宏块中的相对块坐标 
          for (h=bx0[parttype][block8x8]; h          { 
            pic_block_x = img->block_x + h; // 当前子块在图像中的块坐标=当前宏块的块坐标+当前子块在宏块中的相对块坐标 
            mcost = BlockMotionSearch (ref, list, h<<2, v<<2, blocktype, search_range, lambda); //对当前子块作运动向量搜索 
            motion_cost[blocktype][list][ref][block8x8] += mcost;  //保存代价值 
          for (j=0; j 
             for (i=0; i 
             { 
         mv_array [pic_block_x+i][pic_block_y+j][0] = img->all_mv[h][v][list][ref][blocktype][0]; 
         mv_array [pic_block_x+i][pic_block_y+j][1] = img->all_mv[h][v][list][ref][blocktype][1]; 
//以4×4block为单位保存运动向量 
         ref_array [pic_block_x+i][pic_block_y+j]    = ref; 
//保存参考帧序号 
             } 
          } 
       } 
    } 
  } 
} 
[1] static int  bx0[5][4] = {{0,0,0,0}, {0,0,0,0}, {0,0,0,0}, {0,2,0,0}, {0,2,0,2}}; 
    static int  by0[5][4] = {{0,0,0,0}, {0,0,0,0}, {0,2,0,0}, {0,0,0,0}, {0,0,2,2}}; 
 这里的bx0, by0两个数组分别对应了SKIP模式，16×16，16×8，8×16，P8×8这四种模式的横坐标和纵坐标。举两个例子 
如图所示的16×16宏块，首先划分为4个8×8子块（因为PartitionMotionSearch()函数处理的最小块的尺寸为8×8），以4×4block为单位设定坐标，图上已标出4个8×8子块左上角的块坐标。SKIP模式实际上并不牵涉到这个函数，因此坐标全部置零；16×16模式只有第一个坐标起作用，后三个置零；16×8只有前两个有意义，标出两个partition的左上角坐标，如图标出了(0,0),(0,2)，对照bx0, by0可以看到相应坐标值；最多子块情况为4个8×8，即最后一组坐标。
