# Study on Android【五】--自定义ContentProvider的语义 - weixin_33985507的博客 - CSDN博客
2007年12月31日 01:10:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：3
Android做到现在。开始感觉到Android确实还是太年轻。系统本身好不够成熟，相关文档更是少的可怜。在Android的旅途中到处是暗坑陷阱，掉进去摔得半死，还只能靠自己琢磨着爬出来。
想
在大部分工作集中在了定义一套ContentProvider以及上层的显示控件上。一个ContentProvider向外提供的接口十分有限，但就是
这几个东西你要表征出正确（标准就是和系统ContentProvider一致的行为）的语义，还是很费功夫的。至少我就摔了好几个跟头。为了降低后续部
队的伤亡，我努力搬走几个绊脚石、填掉几个坑，希望能有一些作用。
ContentProvider中，最重要的就是query操作。query根
据输入返回一个符合条件的Cursor。这就可能出现以下几种情况：1. 查询成功，包含几个正确的结果；2. 查询失败，没有符合的结果；3.
输入错误， 触发了某个异常；4.
没能查询到结果，但无法确定是输入错误还是查询失败。第一种情况是我们最需要的，当然是需要正确维系的，而最后一种情况在大部分应用中应该不会出现（但在
我的应用中会的*_#），而第二种第三种是比较常见的。
经过我的测试，系统的ContentProvider维持这样的语义：如果是情况2，返回
正常的Cursor，并且，其count为0，相当于empty
cursor；如果是情况3，不抛出任何异常，返回null的Cursor。这样的话明明白白写出来是很好理解的，但由于没有官方的文档说明，在自定义的
时候经常会误用。比如在某些情况下，用null表征查询失败，用抛出异常来描述错误的输入。
返回empty
cursor，如果是通过databasecursor自然会有db帮你维护，但是如果返回ArrayListCursor，MergeCursor或其
他自定义的Cursor，就需要自己维系了。ArrayListCursor可以通过new ArrayListCursor(Columns,
new ArrayList(){})来提供。其中Columns一定不为null。MergeCursor不能以new
MergeCursor(new Cursor[]{})来创建，而需要通过new MergeCursor(new
Cursor[]{aEmptyCursor, ...}来维系（其实很好理解，我呆了...）。自定义的Cursor也一定要提供生成empty
cursor的方式。
如果将ContentProvider作为一个单独的module来理解，不通过异常而是通过null来返回MS是有好处
的。在module的出口吃掉所有异常，虽然不能提供足够的信息（异常信息全部写入日志），但可能会使上层使用更简单。但在Android中，我并没有感
觉到这一点。作为ContentProvider的上层函数，ListActivity.managedQuery、
ListView.setListAdapter等，根本不能处理一个null的Cursor，在ListView中这会触发一个异常。更无语的是，当你
把一个null Cursor设置为manage的后。它不会立即抛异常，而是在OnFreeze等生命周期函数的时候，因无法处理null
Cursor而抛出一个异常。这使得你根本无法在当地catch该异常，换句话，ListActivity的manageCursor根本是个无法使用的函数。你必须用getContext().query()获得Cursor，然后判定该Cursor是否null，在进行startManagingCursor进行绑定。这远不如直接用异常进行错误路径的处理来的统一和方便。
当然，有些东西我们是不能改变的，只能去适应。对于自定义的cursor,
ContentProvider，最重要的，是在无人造错误输入的情况下返回empty
cursor，而不是null。至于使用null响应还是异常响应上，我个人觉得还是和系统同步为好，虽然别扭，但至少统一不容易有歧义。
此外，ContentProvider还有很多细致的语义。比如返回的Cursor需要绑定一个URI，以便自动响应更新。自定义的更新需要支持deleteRow等操作语义等等。总之，我们需要更好的文档或更多经验文档，以便我们更好的爬上巨人的肩膀。
PS：而上层的ListView，更是陷阱重重。首先绑定到ListView的Cursor必须有_id项，否则会有异常抛出。如果做过.net的开发，
这一点是可以想到的，但是，这种问题应该在文档中写明。另外，在ListView中，如果你不绑定一个数据源，你一定不能在layout中添加涉及内容的
属性。比如android:height="wrap_content"，这会在onMeasure的时候抛出异常。
