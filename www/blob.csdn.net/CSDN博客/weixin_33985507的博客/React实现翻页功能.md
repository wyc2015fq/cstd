# React实现翻页功能 - weixin_33985507的博客 - CSDN博客
2017年10月27日 15:33:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：6
由于项目中遇到简单的类似于轮播图中，左右翻页功能，首先明白需求是这样的：
一个页面最多展示6条从后台返回的数据，如果少于6条，一页全部展示，多于6条，做分页处理。
整体思路：
1.首先，声明一个空的数组arr，用于保存截取的数据；
2.其次，声明一个常量NUM，用于保存一页展示的数据条数，便于日后改需求；
3.然后在constructor里声明一个标志位flag，便于把每一次翻页的信息记录下来。（注意：这里的flag相当于一个全局变量，可以在constructor里通过this.flag = 0来声明，也可以在class外声明一个全局变量）
4.最后就是实现分页功能的原理：
```
①如果是向左翻页，首先判断flag标志位是否为0，如果过标志位为0，则表示已经是第一页，不能继续向左翻页，把flag置为  
           0，否则，每点击  一次，让flag-1；
       ②如果是向右翻页，则需要做两步判断：
              a.首先判断是否是NUM的整数倍，如果是NUM 的整数倍（arr.length % NUM == 0），表示每页都有NUM条数据，此时
                 继续判断是否已经翻到最后一页（this.flag >= arr.length/NUM-1），如果已经翻页到最后一页，把flag设置为
                 arr.length/NUM-1，不能继续向右翻页，否 则，每点击一次让flag+1；
              b.其次判断不是NUM的整数倍，最后一页不能展示NUM条数据，此时判断是否已经翻页到最后一页，如果已经翻页到最
                  后一页，把flag标志位设置为Math.floor(arr.length/NUM)，否则，让flag+1
```
```
class Lists extends React.Component {
    constructor(porps){
        super(porps);
        this.state = {
            datas:[]
        };
        //设置翻页标志位全局变量
        this.flag = 0;
        this.getPageData = this.getPageData.bind(this);
    }
    componentDidMount(){
        this.getPageData();
    }
    //页面加载时，获取前几条数据
    getPageData(){
        var lists = this.props.tools.slice(0,6);
        this.setState({
            datas:lists
        })
        console.log("lists------>",lists);
    }
    //左右翻页功能
    changePage(num){
        var newArr = [];
        const NUM = 6;
        /*var flag = 0;*/
        var arr = this.props.tools;
        //如果是向左滑动，首先判断标志位flag是否小于等于0
        if(num == -1){
            //如果标志位为0，则表示不能向左滑动，把flag标志位置位0
            if(this.flag <= 0){
                this.flag = 0;
                newArr = arr.slice(this.flag*NUM,this.flag*NUM+NUM);
            }else{
                this.flag = this.flag - 1;
                newArr = arr.slice(this.flag*NUM,this.flag*NUM+NUM);
            }
        }else{//如果向右滑动，则需要判断上限
            //首先判断是不是NUM的整数倍
            //如果是NUM的整数倍
            /*debugger;*/
            if(arr.length % NUM == 0){
                //如果flag大于等于arr.length/NUM-1，把flag置为arr.length/NUM-1
                if(this.flag >= arr.length/NUM-1){
                    flag = arr.length/NUM-1;
                    newArr = arr.slice(this.flag*NUM,this.flag*NUM+NUM);
                }else{//如果flag小于arr.length/NUM-1
                    this.flag = this.flag + 1;
                    newArr = arr.slice(this.flag*NUM,this.flag*NUM+NUM);
                }
            }else{//如果不是NUM的整数倍
                //如果flag大于等于Math.floor(arr.length/NUM),把flag置为Math.floor(arr.length/NUM)
                if(this.flag >= Math.floor(arr.length/NUM)){
                    this.flag = Math.floor(arr.length/NUM);
                    newArr = arr.slice(this.flag*NUM,this.flag*NUM+NUM);
                }else{//如果flag小于Math.floor(arr.length/NUM)
                    this.flag = this.flag + 1;
                    newArr = arr.slice(this.flag*NUM,this.flag*NUM+NUM);
                }
            }
        }
        console.log(newArr);
        this.setState({
            datas:newArr
        })
    }
    render() {
        return (
            <div className={ style.container }>
                <div className={ style.left } onClick={()=> this.changePage(-1)}>
                    ![](/static/img/tools/arrow_left.png)
                </div>
                <div className={ style.middle }>
                    {
                        this.state.datas.map( (value,index) => {
                            return(
                                <div className={ style.content } key={ index }>
                                    <Analyze value = { value }></Analyze>
                                </div>
                            )
                        })
                    }
                </div>
                <div className={ style.right } onClick={()=> this.changePage(1)}>
                    ![](/static/img/tools/arrow_right.png)
                </div>
            </div>
        )
    }
}
export default Lists;
```
还有一点需要注意的是：由于我 把从后台获取到的数据保存到了state里面，当页面首次加载的时候，this.state = { datas:[ ] }，datas为空数组;不会显示数据，后来得到了改进，在componentDidMount的时候，加载几条数据，用于第一页显示。
```
componentDidMount(){
        this.getPageData();
    }
    //页面加载时，获取前几条数据
    getPageData(){
        var lists = this.props.tools.slice(0,6);
        this.setState({
            datas:lists
        })
        console.log("lists------>",lists);
    }
```
