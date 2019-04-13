
# Winform仿制QQ微信聊天窗口气泡 - 致林 - 博客园






# [Winform仿制QQ微信聊天窗口气泡](https://www.cnblogs.com/bincoding/p/7877568.html)
因为公司业务原因，不能上传原始项目，这是简化版本。
临时设计的窗体和气泡样式，有需要可以重新设计。效果如下：
![](https://images2017.cnblogs.com/blog/771778/201711/771778-20171122101509555-839454153.png)
主要原理：一个TextBlock + 一个三角形
项目结构：
-- Form1 窗体类
-- Item 控件类（气泡）
Form1前端代码：
![](https://images.cnblogs.com/OutliningIndicators/ContractedBlock.gif)![](https://images.cnblogs.com/OutliningIndicators/ExpandedBlockStart.gif)\#regionWindows 窗体设计器生成的代码///<summary>///设计器支持所需的方法 - 不要///使用代码编辑器修改此方法的内容。///</summary>privatevoidInitializeComponent()
        {this.panel1 =newSystem.Windows.Forms.Panel();this.textBox1 =newSystem.Windows.Forms.TextBox();this.button1 =newSystem.Windows.Forms.Button();this.SuspendLayout();////panel1//this.panel1.AutoScroll =true;this.panel1.Location =newSystem.Drawing.Point(0,0);this.panel1.Name ="panel1";this.panel1.Size =newSystem.Drawing.Size(377,404);this.panel1.TabIndex =0;////textBox1//this.textBox1.Location =newSystem.Drawing.Point(0,406);this.textBox1.Multiline =true;this.textBox1.Name ="textBox1";this.textBox1.Size =newSystem.Drawing.Size(377,65);this.textBox1.TabIndex =1;////button1//this.button1.Location =newSystem.Drawing.Point(302,477);this.button1.Name ="button1";this.button1.Size =newSystem.Drawing.Size(75,23);this.button1.TabIndex =2;this.button1.Text ="Send";this.button1.UseVisualStyleBackColor =true;this.button1.Click +=newSystem.EventHandler(this.button1_Click);////Form1//this.AutoScaleDimensions =newSystem.Drawing.SizeF(6F, 12F);this.AutoScaleMode =System.Windows.Forms.AutoScaleMode.Font;this.ClientSize =newSystem.Drawing.Size(380,504);this.Controls.Add(this.button1);this.Controls.Add(this.textBox1);this.Controls.Add(this.panel1);this.Name ="Form1";this.Text ="Form1";this.ResumeLayout(false);this.PerformLayout();

        }\#endregionprivateSystem.Windows.Forms.Panel panel1;privateSystem.Windows.Forms.TextBox textBox1;privateSystem.Windows.Forms.Button button1;
View Code
Form类后台代码：
![](https://images.cnblogs.com/OutliningIndicators/ContractedBlock.gif)![](https://images.cnblogs.com/OutliningIndicators/ExpandedBlockStart.gif)///<summary>///当前消息气泡起始位置///</summary>privateinttop =0;///<summary>///当前消息气泡高度///</summary>privateintheight =0;privatevoidbutton1_Click(objectsender, EventArgs e)
        {
            AddSendMessage(textBox1.Text);
            AddReceiveMessage(textBox1.Text);
        }///<summary>///显示接收消息///</summary>///<param name="model"></param>privatevoidAddReceiveMessage(stringcontent)
        {
            Item item=newItem();
            item.messageType=WindowsFormsApplication2.Item.MessageType.receive;
            item.SetWeChatContent(content);//计算高度item.Top = top +height;
            top=item.Top;
            height=item.HEIGHT;//滚动条移动最上方，重新计算气泡在panel的位置panel1.AutoScrollPosition =newPoint(0,0);
            panel1.Controls.Add(item);
        }//<summary>///更新界面，显示发送消息///</summary>privatevoidAddSendMessage(stringcontent)
        {
            Item item=newItem();
            item.messageType=WindowsFormsApplication2.Item.MessageType.send;
            item.SetWeChatContent(content);
            item.Top= top +height;
            item.Left=370-item.WIDTH;
top=item.Top;
            height=item.HEIGHT;
            panel1.AutoScrollPosition=newPoint(0,0);
            panel1.Controls.Add(item);
        }
View Code
Item类前端代码：
![](https://images.cnblogs.com/OutliningIndicators/ContractedBlock.gif)![](https://images.cnblogs.com/OutliningIndicators/ExpandedBlockStart.gif)\#region组件设计器生成的代码///<summary>///设计器支持所需的方法 - 不要///使用代码编辑器修改此方法的内容。///</summary>privatevoidInitializeComponent()
        {this.panel1 =newSystem.Windows.Forms.Panel();this.lblContent =newSystem.Windows.Forms.Label();this.panel1.SuspendLayout();this.SuspendLayout();////panel1//this.panel1.AutoSize =true;this.panel1.BackColor =System.Drawing.Color.LightGray;this.panel1.Controls.Add(this.lblContent);this.panel1.Location =newSystem.Drawing.Point(20,10);this.panel1.MaximumSize =newSystem.Drawing.Size(370,400);this.panel1.Name ="panel1";this.panel1.Padding =newSystem.Windows.Forms.Padding(10,10,5,10);this.panel1.Size =newSystem.Drawing.Size(26,36);this.panel1.TabIndex =0;////lblContent//this.lblContent.AutoSize =true;this.lblContent.Font =newSystem.Drawing.Font("宋体", 16F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Pixel);this.lblContent.ForeColor =System.Drawing.Color.White;this.lblContent.ImeMode =System.Windows.Forms.ImeMode.NoControl;this.lblContent.Location =newSystem.Drawing.Point(5,10);this.lblContent.Margin =newSystem.Windows.Forms.Padding(0);this.lblContent.MaximumSize =newSystem.Drawing.Size(280,1000);this.lblContent.Name ="lblContent";this.lblContent.Size =newSystem.Drawing.Size(16,16);this.lblContent.TabIndex =5;this.lblContent.Text ="";this.lblContent.Visible =false;////Item//this.AutoScaleDimensions =newSystem.Drawing.SizeF(6F, 12F);this.AutoScaleMode =System.Windows.Forms.AutoScaleMode.Font;this.AutoSize =true;this.Controls.Add(this.panel1);this.Name ="Item";this.Padding =newSystem.Windows.Forms.Padding(20,10,10,5);this.Size =newSystem.Drawing.Size(59,54);this.panel1.ResumeLayout(false);this.panel1.PerformLayout();this.ResumeLayout(false);this.PerformLayout();
}\#endregionprivateSystem.Windows.Forms.Panel panel1;privateSystem.Windows.Forms.Label lblContent;
View Code
Item 类后台代码：
![](https://images.cnblogs.com/OutliningIndicators/ContractedBlock.gif)![](https://images.cnblogs.com/OutliningIndicators/ExpandedBlockStart.gif)///<summary>///本窗体总高度///</summary>publicintHEIGHT =40;///<summary>///本窗体总宽度///</summary>publicintWIDTH =45;///<summary>///消息类型///</summary>publicMessageType messageType;publicItem()
        {///设置控件样式SetStyle(
                    ControlStyles.AllPaintingInWmPaint|//不闪烁ControlStyles.OptimizedDoubleBuffer//支持双缓存,true);
            InitializeComponent();this.Paint +=Item_Paint;
        }\#region界面重绘///<summary>///绘制气泡左上角小箭头///</summary>///<param name="sender"></param>///<param name="e"></param>privatevoidItem_Paint(objectsender, PaintEventArgs e)
        {//自己发送的消息箭头在右上角if(messageType ==MessageType.send)
            {
Color color=System.Drawing.Color.LightGray;
                panel1.BackColor=color;
                Brush brushes=newSolidBrush(color);
                Point[] point=newPoint[3];
                point[0] =newPoint(WIDTH -5,10);
                point[1] =newPoint(WIDTH -15,10);
                point[2] =newPoint(WIDTH -15,20);
                e.Graphics.FillPolygon(brushes, point);
            }else{
Color color=System.Drawing.Color.LightGray;
                Brush brushes=newSolidBrush(color);
                Point[] point=newPoint[3];
                point[0] =newPoint(10,10);
                point[1] =newPoint(20,10);
                point[2] =newPoint(20,20);
                e.Graphics.FillPolygon(brushes, point);
            }
        }\#endregion\#region功能操作///<summary>///设置气泡内容///</summary>///<param name="type">消息类型</param>///<param name="content">消息内容</param>publicvoidSetWeChatContent(stringcontent)
        {
lblContent.Text=content;
            lblContent.Visible=true;
            HEIGHT+=lblContent.Height;
            WIDTH+=lblContent.Width;
        }\#endregion///<summary>///内部类///</summary>classMessageItem
        {publicstringRESPATH {get;set; }publicstringRESTYPE {get;set; }
        }///<summary>///消息类型///</summary>publicenumMessageType
        {
            send,
            receive
        }
View Code
项目中的一些坑：
1. panel控件出现滚动条后，添加控件时需要重新计算相对位置，不然每个气泡间的间距会变大。比较简单的解决方法：每次添加控件前将滚动条移到最上方，添加完控件后再将滚动条移到最下方。
2. 设置双缓冲和不闪烁
3. 计算气泡位置和绘制小箭头，这个不难但是需要时间，不知道为什么按设计稿设置位置一直出错，对winform理解不够，wpf可能会更自由一点
Github:
[https://github.com/haibincoder/WinformBubble](https://github.com/haibincoder/WinformBubble)





