
#if 0

typedef struct test_Approximation_t {
} test_Approximation_t;

static LRESULT test_Approximation_proc(ctrl_t* sender, ctrl_t* c, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  test_Approximation_t* s = (test_Approximation_t*)c->x;
  //IRECT rc = c->rc;
  int x = LOWORD(lParam), y = HIWORD(lParam);

  switch (uMsg) {
  case WM_CTRLINIT:
    if (s) {
      return 0;
    }

    break;

  case WM_COMMAND:
    break;

  case WM_GETMINMAXINFO:
    if (1) {
      MINMAXINFO* mminfo = (PMINMAXINFO)lParam;
      c->sub->ctrlproc(c, c->sub, uMsg, wParam, lParam);
      mminfo->ptMinTrackSize.x = 400;
      mminfo->ptMinTrackSize.y = 300;
      //printf("WM_MOUSEMOVE\n");
    }

    break;

  case WM_TIMER:
    if (s) {
      //force_redraw(c);
    }

    break;

  case WM_PAINT:
    if (c->recalcsize) {
      ctrl_setrect(c->sub, c->rc);
    }
    if (c->redraw) {
      img_t* im = (img_t*)wParam;
      int i;
      int n_drawn = 0;
      IRECT rc = c->rc;
      IRECT rcclip = c->rcclip;
      iRectOff0(&rcclip);
      iRectOff0(&rc);
      //s->lay1->c->isshow = 0;
      imdraw_fill(im, rgb(1, 1, 1));
      c->redraw = 0;
    }

    break;
  }

  return 0;
}


/// <summary>
/// Summary description for Form1.
/// </summary>
{
  GroupBox groupBox1;
  ListView dataList;
  Button loadDataButton;
  ColumnHeader xColumnHeader;
  ColumnHeader yColumnHeader;
  OpenFileDialog openFileDialog;
  GroupBox groupBox2;
  Chart chart;
  GroupBox groupBox3;
  TextBox momentumBox;
  Label label6;
  TextBox alphaBox;
  Label label2;
  TextBox learningRateBox;
  Label label1;
  Label label8;
  TextBox iterationsBox;
  Label label10;
  Label label9;
  GroupBox groupBox4;
  TextBox currentErrorBox;
  Label label3;
  TextBox currentIterationBox;
  Label label5;
  Button stopButton;
  Button startButton;
  Label label4;
  TextBox neuronsBox;
  /// <summary>
  /// Required designer variable.
  /// </summary>
   System.ComponentModel.Container components = null;
  
  
  // Constructor
  MainForm( )
  {
    //
    // Required for Windows Form Designer support
    //
    InitializeComponent();
    
    // init chart control
    chart.AddDataSeries( "data", Color.Red, Chart.SeriesType.Dots, 5 );
    chart.AddDataSeries( "solution", Color.Blue, Chart.SeriesType.Line, 1 );
    
    // init controls
    UpdateSettings( );
  }
  
  /// <summary>
  /// Clean up any resources being used.
  /// </summary>
  override void Dispose( bool disposing )
  {
    if( disposing )
    {
      if (components != null) 
      {
        components.Dispose();
      }
    }
    base.Dispose( disposing );
  }
  
#region Windows Form Designer generated code
  /// <summary>
  /// Required method for Designer support - do not modify
  /// the contents of this method with the code editor.
  /// </summary>
   void InitializeComponent()
  {
    this->groupBox1 = new System.Windows.Forms.GroupBox();
    this->dataList = new System.Windows.Forms.ListView();
    this->loadDataButton = new System.Windows.Forms.Button();
    this->xColumnHeader = new System.Windows.Forms.ColumnHeader();
    this->yColumnHeader = new System.Windows.Forms.ColumnHeader();
    this->openFileDialog = new System.Windows.Forms.OpenFileDialog();
    this->groupBox2 = new System.Windows.Forms.GroupBox();
    this->chart = new AForge.Controls.Chart();
    this->groupBox3 = new System.Windows.Forms.GroupBox();
    this->momentumBox = new System.Windows.Forms.TextBox();
    this->label6 = new System.Windows.Forms.Label();
    this->alphaBox = new System.Windows.Forms.TextBox();
    this->label2 = new System.Windows.Forms.Label();
    this->learningRateBox = new System.Windows.Forms.TextBox();
    this->label1 = new System.Windows.Forms.Label();
    this->label8 = new System.Windows.Forms.Label();
    this->iterationsBox = new System.Windows.Forms.TextBox();
    this->label10 = new System.Windows.Forms.Label();
    this->label9 = new System.Windows.Forms.Label();
    this->groupBox4 = new System.Windows.Forms.GroupBox();
    this->currentErrorBox = new System.Windows.Forms.TextBox();
    this->label3 = new System.Windows.Forms.Label();
    this->currentIterationBox = new System.Windows.Forms.TextBox();
    this->label5 = new System.Windows.Forms.Label();
    this->stopButton = new System.Windows.Forms.Button();
    this->startButton = new System.Windows.Forms.Button();
    this->label4 = new System.Windows.Forms.Label();
    this->neuronsBox = new System.Windows.Forms.TextBox();
    this->groupBox1.SuspendLayout();
    this->groupBox2.SuspendLayout();
    this->groupBox3.SuspendLayout();
    this->groupBox4.SuspendLayout();
    this->SuspendLayout();
    // 
    // groupBox1
    // 
    this->groupBox1.Controls.Add(this->dataList);
    this->groupBox1.Controls.Add(this->loadDataButton);
    this->groupBox1.Location = new System.Drawing.Point(10, 10);
    this->groupBox1.Name = "groupBox1";
    this->groupBox1.Size = new System.Drawing.Size(180, 320);
    this->groupBox1.TabIndex = 1;
    this->groupBox1.TabStop = false;
    this->groupBox1.Text = "Data";
    // 
    // dataList
    // 
    this->dataList.Columns.AddRange(new System.Windows.Forms.ColumnHeader* {
      this->xColumnHeader,
        this->yColumnHeader});
      this->dataList.FullRowSelect = true;
      this->dataList.GridLines = true;
      this->dataList.Location = new System.Drawing.Point(10, 20);
      this->dataList.Name = "dataList";
      this->dataList.Size = new System.Drawing.Size(160, 255);
      this->dataList.TabIndex = 0;
      this->dataList.View = System.Windows.Forms.View.Details;
      // 
      // loadDataButton
      // 
      this->loadDataButton.Location = new System.Drawing.Point(10, 285);
      this->loadDataButton.Name = "loadDataButton";
      this->loadDataButton.TabIndex = 1;
      this->loadDataButton.Text = "&Load";
      this->loadDataButton.Click += new System.EventHandler(this->loadDataButton_Click);
      // 
      // xColumnHeader
      // 
      this->xColumnHeader.Text = "X";
      // 
      // yColumnHeader
      // 
      this->yColumnHeader.Text = "Y";
      // 
      // openFileDialog
      // 
      this->openFileDialog.Filter = "CSV (Comma delimited) (*.csv)|*.csv";
      this->openFileDialog.Title = "Select data file";
      // 
      // groupBox2
      // 
      this->groupBox2.Controls.Add(this->chart);
      this->groupBox2.Location = new System.Drawing.Point(200, 10);
      this->groupBox2.Name = "groupBox2";
      this->groupBox2.Size = new System.Drawing.Size(300, 320);
      this->groupBox2.TabIndex = 2;
      this->groupBox2.TabStop = false;
      this->groupBox2.Text = "Function";
      // 
      // chart
      // 
      this->chart.Location = new System.Drawing.Point(10, 20);
      this->chart.Name = "chart";
      this->chart.Size = new System.Drawing.Size(280, 290);
      this->chart.TabIndex = 0;
      // 
      // groupBox3
      // 
      this->groupBox3.Controls.Add(this->neuronsBox);
      this->groupBox3.Controls.Add(this->label4);
      this->groupBox3.Controls.Add(this->momentumBox);
      this->groupBox3.Controls.Add(this->label6);
      this->groupBox3.Controls.Add(this->alphaBox);
      this->groupBox3.Controls.Add(this->label2);
      this->groupBox3.Controls.Add(this->learningRateBox);
      this->groupBox3.Controls.Add(this->label1);
      this->groupBox3.Controls.Add(this->label8);
      this->groupBox3.Controls.Add(this->iterationsBox);
      this->groupBox3.Controls.Add(this->label10);
      this->groupBox3.Controls.Add(this->label9);
      this->groupBox3.Location = new System.Drawing.Point(510, 10);
      this->groupBox3.Name = "groupBox3";
      this->groupBox3.Size = new System.Drawing.Size(195, 195);
      this->groupBox3.TabIndex = 4;
      this->groupBox3.TabStop = false;
      this->groupBox3.Text = "Settings";
      // 
      // momentumBox
      // 
      this->momentumBox.Location = new System.Drawing.Point(125, 45);
      this->momentumBox.Name = "momentumBox";
      this->momentumBox.Size = new System.Drawing.Size(60, 20);
      this->momentumBox.TabIndex = 3;
      this->momentumBox.Text = "";
      // 
      // label6
      // 
      this->label6.Location = new System.Drawing.Point(10, 47);
      this->label6.Name = "label6";
      this->label6.Size = new System.Drawing.Size(82, 17);
      this->label6.TabIndex = 2;
      this->label6.Text = "Momentum:";
      // 
      // alphaBox
      // 
      this->alphaBox.Location = new System.Drawing.Point(125, 70);
      this->alphaBox.Name = "alphaBox";
      this->alphaBox.Size = new System.Drawing.Size(60, 20);
      this->alphaBox.TabIndex = 5;
      this->alphaBox.Text = "";
      // 
      // label2
      // 
      this->label2.Location = new System.Drawing.Point(10, 72);
      this->label2.Name = "label2";
      this->label2.Size = new System.Drawing.Size(120, 15);
      this->label2.TabIndex = 4;
      this->label2.Text = "Sigmoid\'s alpha value:";
      // 
      // learningRateBox
      // 
      this->learningRateBox.Location = new System.Drawing.Point(125, 20);
      this->learningRateBox.Name = "learningRateBox";
      this->learningRateBox.Size = new System.Drawing.Size(60, 20);
      this->learningRateBox.TabIndex = 1;
      this->learningRateBox.Text = "";
      // 
      // label1
      // 
      this->label1.Location = new System.Drawing.Point(10, 22);
      this->label1.Name = "label1";
      this->label1.Size = new System.Drawing.Size(78, 14);
      this->label1.TabIndex = 0;
      this->label1.Text = "Learning rate:";
      // 
      // label8
      // 
      this->label8.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
      this->label8.Location = new System.Drawing.Point(10, 147);
      this->label8.Name = "label8";
      this->label8.Size = new System.Drawing.Size(175, 2);
      this->label8.TabIndex = 22;
      // 
      // iterationsBox
      // 
      this->iterationsBox.Location = new System.Drawing.Point(125, 155);
      this->iterationsBox.Name = "iterationsBox";
      this->iterationsBox.Size = new System.Drawing.Size(60, 20);
      this->iterationsBox.TabIndex = 9;
      this->iterationsBox.Text = "";
      // 
      // label10
      // 
      this->label10.Font = new System.Drawing.Font("Microsoft Sans Serif", 6.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((System.Byte)(0)));
      this->label10.Location = new System.Drawing.Point(126, 175);
      this->label10.Name = "label10";
      this->label10.Size = new System.Drawing.Size(58, 14);
      this->label10.TabIndex = 25;
      this->label10.Text = "( 0 - inifinity )";
      // 
      // label9
      // 
      this->label9.Location = new System.Drawing.Point(10, 157);
      this->label9.Name = "label9";
      this->label9.Size = new System.Drawing.Size(70, 16);
      this->label9.TabIndex = 8;
      this->label9.Text = "Iterations:";
      // 
      // groupBox4
      // 
      this->groupBox4.Controls.Add(this->currentErrorBox);
      this->groupBox4.Controls.Add(this->label3);
      this->groupBox4.Controls.Add(this->currentIterationBox);
      this->groupBox4.Controls.Add(this->label5);
      this->groupBox4.Location = new System.Drawing.Point(510, 210);
      this->groupBox4.Name = "groupBox4";
      this->groupBox4.Size = new System.Drawing.Size(195, 75);
      this->groupBox4.TabIndex = 6;
      this->groupBox4.TabStop = false;
      this->groupBox4.Text = "Current iteration";
      // 
      // currentErrorBox
      // 
      this->currentErrorBox.Location = new System.Drawing.Point(125, 45);
      this->currentErrorBox.Name = "currentErrorBox";
      this->currentErrorBox.ReadOnly = true;
      this->currentErrorBox.Size = new System.Drawing.Size(60, 20);
      this->currentErrorBox.TabIndex = 3;
      this->currentErrorBox.Text = "";
      // 
      // label3
      // 
      this->label3.Location = new System.Drawing.Point(10, 47);
      this->label3.Name = "label3";
      this->label3.Size = new System.Drawing.Size(70, 16);
      this->label3.TabIndex = 2;
      this->label3.Text = "Error:";
      // 
      // currentIterationBox
      // 
      this->currentIterationBox.Location = new System.Drawing.Point(125, 20);
      this->currentIterationBox.Name = "currentIterationBox";
      this->currentIterationBox.ReadOnly = true;
      this->currentIterationBox.Size = new System.Drawing.Size(60, 20);
      this->currentIterationBox.TabIndex = 1;
      this->currentIterationBox.Text = "";
      // 
      // label5
      // 
      this->label5.Location = new System.Drawing.Point(10, 22);
      this->label5.Name = "label5";
      this->label5.Size = new System.Drawing.Size(70, 16);
      this->label5.TabIndex = 0;
      this->label5.Text = "Iteration:";
      // 
      // stopButton
      // 
      this->stopButton.Enabled = false;
      this->stopButton.Location = new System.Drawing.Point(630, 305);
      this->stopButton.Name = "stopButton";
      this->stopButton.TabIndex = 8;
      this->stopButton.Text = "S&top";
      this->stopButton.Click += new System.EventHandler(this->stopButton_Click);
      // 
      // startButton
      // 
      this->startButton.Enabled = false;
      this->startButton.Location = new System.Drawing.Point(540, 305);
      this->startButton.Name = "startButton";
      this->startButton.TabIndex = 7;
      this->startButton.Text = "&Start";
      this->startButton.Click += new System.EventHandler(this->startButton_Click);
      // 
      // label4
      // 
      this->label4.Location = new System.Drawing.Point(10, 97);
      this->label4.Name = "label4";
      this->label4.Size = new System.Drawing.Size(115, 15);
      this->label4.TabIndex = 6;
      this->label4.Text = "Neurons in first layer:";
      // 
      // neuronsBox
      // 
      this->neuronsBox.Location = new System.Drawing.Point(125, 95);
      this->neuronsBox.Name = "neuronsBox";
      this->neuronsBox.Size = new System.Drawing.Size(60, 20);
      this->neuronsBox.TabIndex = 7;
      this->neuronsBox.Text = "";
      // 
      // MainForm
      // 
      this->AutoScaleBaseSize = new System.Drawing.Size(5, 13);
      this->ClientSize = new System.Drawing.Size(714, 338);
      this->Controls.Add(this->stopButton);
      this->Controls.Add(this->startButton);
      this->Controls.Add(this->groupBox4);
      this->Controls.Add(this->groupBox3);
      this->Controls.Add(this->groupBox2);
      this->Controls.Add(this->groupBox1);
      this->FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
      this->MaximizeBox = false;
      this->Name = "MainForm";
      this->Text = "Approximation using Multi-Layer Neural Network";
      this->Closing += new System.ComponentModel.CancelEventHandler(this->MainForm_Closing);
      this->groupBox1.ResumeLayout(false);
      this->groupBox2.ResumeLayout(false);
      this->groupBox3.ResumeLayout(false);
      this->groupBox4.ResumeLayout(false);
      this->ResumeLayout(false);
      
  }
#endregion
  
  /// <summary>
  /// The main entry point for the application.
  /// </summary>
  [STAThread]
    static void Main( ) 
  {
    Application.Run( new MainForm( ) );
  }
  
  // On main form closing
   void MainForm_Closing(object sender, System.ComponentModel.CancelEventArgs e)
  {
    // check if worker thread is running
    if ( ( workerThread != null ) && ( workerThread.IsAlive ) )
    {
      needToStop = true;
      workerThread.Join( );
    }
  }
  
  // Update settings controls
   void UpdateSettings()
  {
    learningRateBox.Text	= learningRate.ToString( );
    momentumBox.Text		= momentum.ToString( );
    alphaBox.Text			= sigmoidAlphaValue.ToString( );
    neuronsBox.Text			= neuronsInFirstLayer.ToString( );
    iterationsBox.Text		= iterations.ToString( );
  }
  
  // Load data
   void loadDataButton_Click(object sender, System.EventArgs e)
  {
    // show file selection dialog
    if ( openFileDialog.ShowDialog( ) == DialogResult.OK )
    {
      StreamReader reader = null;
      // read maximum 50 points
      double[,] tempData = new double[50, 2];
      double minX = double.MaxValue;
      double maxX = double.MinValue;
      
      try
      {
        // open selected file
        reader = File.OpenText( openFileDialog.FileName );
        string	str = null;
        int		i = 0;
        
        // read the data
        while ( ( i < 50 ) && ( ( str = reader.ReadLine( ) ) != null ) )
        {
          string* strs = str.Split( ';' );
          if ( strs.Length == 1 )
            strs = str.Split( ',' );
          // parse X
          tempData[i, 0] = double.Parse( strs[0] );
          tempData[i, 1] = double.Parse( strs[1] );
          
          // search for min value
          if ( tempData[i, 0] < minX )
            minX = tempData[i, 0];
          // search for max value
          if ( tempData[i, 0] > maxX )
            maxX = tempData[i, 0];
          
          i++;
        }
        
        // allocate and set data
        data = new double[i, 2];
        Array.Copy( tempData, 0, data, 0, i * 2 );
      }
      catch (Exception)
      {
        MessageBox.Show( "Failed reading the file", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error );
        return;
      }
      finally
      {
        // close file
        if ( reader != null )
          reader.Close( );
      }
      
      // update list and chart
      UpdateDataListView( );
      chart.RangeX = new DoubleRange( minX, maxX );
      chart.UpdateDataSeries( "data", data );
      chart.UpdateDataSeries( "solution", null );
      // enable "Start" button
      startButton.Enabled = true;
    }
  }
  
  // Update data in list view
   void UpdateDataListView( )
  {
    // remove all current records
    dataList.Items.Clear( );
    // add new records
    for ( int i = 0, n = data.GetLength( 0 ); i < n; i++ )
    {
      dataList.Items.Add( data[i, 0].ToString( ) );
      dataList.Items[i].SubItems.Add( data[i, 1].ToString( ) );
    }
  }
  
  // Enable/disale controls
   void EnableControls( bool enable )
  {
    loadDataButton.Enabled		= enable;
    learningRateBox.Enabled		= enable;
    momentumBox.Enabled			= enable;
    alphaBox.Enabled			= enable;
    neuronsBox.Enabled			= enable;
    iterationsBox.Enabled		= enable;
    
    startButton.Enabled	= enable;
    stopButton.Enabled	= !enable;
  }
  
  
  // On button "Stop"
   void stopButton_Click( object sender, System.EventArgs e )
  {
    // stop worker thread
    needToStop = true;
    workerThread.Join( );
    workerThread = null;
  }
  
}
#endif

img_t _data[1] = {0};
double	learningRate = 0.1;
double	momentum = 0.0;
double	sigmoidAlphaValue = 2.0;
int		neuronsInFirstLayer = 20;
int		iterations = 1000;
bool	needToStop = false;

#include "cfile.h"
//#include "img/imgio.inl"



CC_INLINE int imload_cvs(const char* name, int isint, img_t* im) {
  vstr_t sv[1] = {0};
  vstr_load(name, sv);
  if (sv->n>0) {
    const char* delims = ",";
    const char* trimstr = " ";
    int i, j, m = STRsplitn(sv->v[0], delims, trimstr);
    imsetsize(im, sv->n, m, isint ? sizeof(int) : sizeof(double), 1);
    for (i=0; i<sv->n; ++i) {
      if (isint) {
        int* p = (int*)(im->tt.data + i*im->s);
        str_t s = sv->v[i];
        for (j=0; j<m; ++j) {
          str_t ss = STRsplit(s, delims, trimstr, &s);
          p[j] = atoi(ss.s);
        }
      } else {
        double* p = (double*)(im->tt.data + i*im->s);
        str_t s = sv->v[i];
        for (j=0; j<m; ++j) {
          str_t ss = STRsplit(s, delims, trimstr, &s);
          p[j] = atof(ss.s);
        }
      }
    }
  }
  //imshowmat_f8(im); cvWaitKey(-1);
  return 0;
}

// On button "Start"
void startButton_Click()
{
  img_t im[1] = {0};
  double* data = NULL;
  _chdir("E:/code/cstd/include/AForge/Neuro/data");
  imload_cvs("sample1.csv", 0, _data);
  data = (double*)_data->tt.data;
  imsetsize(im, 512, 512, 4, 1);
  // get learning rate
  learningRate = MAX( 0.00001, MIN( 1, learningRate ) );
  
  // get momentum
  momentum = MAX( 0, MIN( 0.5, momentum ) );
  
  // get sigmoid's alpha value
  sigmoidAlphaValue = MAX( 0.001, MIN( 50, sigmoidAlphaValue ) );
  
  // get neurons count in first layer
  neuronsInFirstLayer = MAX( 5, MIN( 50, neuronsInFirstLayer ) );
  
  // iterations
  iterations = MAX( 0, iterations );
  
  // update settings controls
  //UpdateSettings( );
  
  // disable all settings controls except "Stop" button
  //EnableControls( false );
  
  // run worker thread
  needToStop = false;
  imdraw_Chart(im, 0, iRECT(0, 0, 512, 512), SeriesType_ConnectedDots, _data->h, _data->w, data, 2);
  imshow(im);
  cvWaitKey(-1);
  
  double	minY = DBL_MAX;
  double	maxY = DBL_MIN;
  double	minX = DBL_MAX;
  double	maxX = DBL_MIN;
  double xFactor, yFactor;
  // number of learning samples
  int samples = _data->h;
  int i;
  // data transformation factor
  
  for ( i = 0; i < samples; ++i ) {
    double v = data[i*2+1];
    // check for max
    maxX = MAX(maxX, v);
    maxY = MAX(maxY, v);
    // check for min
    minX = MIN(minX, v);
    minY = MIN(minY, v);
  }
  xFactor = 2.0 / ( maxX - minX );
  yFactor = 1.7 / ( maxY - minY );

  // prepare learning data
  double* input = MALLOC(double, samples);
  double* output = MALLOC(double, samples);
  
  for ( i = 0; i < samples; i++ )
  {
    // set input
    input[i] = ( data[i*2+0] - minX ) * xFactor - 1.0;
    // set output
    output[i] = ( data[i*2+1] - minY ) * yFactor - 0.85;
  }
  
#if 0
  // create multi-layer neural network
  ActivationNetwork	network = new ActivationNetwork(
    new BipolarSigmoidFunction( sigmoidAlphaValue ),
    1, neuronsInFirstLayer, 1 );
  // create teacher
  BackPropagationLearning teacher = new BackPropagationLearning( network );
  // set learning rate and momentum
  teacher.LearningRate	= learningRate;
  teacher.Momentum		= momentum;
  
  // iterations
  int iteration = 1;
  
  // solution array
  double[,]	solution = new double[50, 2];
  double*	networkInput = new double[1];
  
  // calculate X values to be used with solution function
  for ( int j = 0; j < 50; j++ )
  {
    solution[j, 0] = chart.RangeX.Min + (double) j * chart.RangeX.Length / 49;
  }
  
  // loop
  while ( !needToStop )
  {
    // run epoch of learning procedure
    double error = teacher.RunEpoch( input, output ) / samples;
    
    // calculate solution
    for ( int j = 0; j < 50; j++ )
    {
      networkInput[0] = ( solution[j, 0] - minX ) * xFactor - 1.0;
      solution[j, 1] = ( network.Compute( networkInput )[0] + 0.85 ) / yFactor + minY;
    }
    chart.UpdateDataSeries( "solution", solution );
    // calculate error
    double learningError = 0.0;
    for ( int j = 0, k = data.GetLength( 0 ); j < k; j++ )
    {
      networkInput[0] = input[j][0];
      learningError += Math.Abs( data[j, 1] - ( ( network.Compute( networkInput )[0] + 0.85 ) / yFactor + minY ) );
    }
    
    // set current iteration's info
    currentIterationBox.Text = iteration.ToString( );
    currentErrorBox.Text = learningError.ToString( "F3" );
    
    // increase current iteration
    iteration++;
    
    // check if we need to stop
    if ( ( iterations != 0 ) && ( iteration > iterations ) )
      break;
  }
  
  
  // enable settings controls
  EnableControls( true );
#endif
}
