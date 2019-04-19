# ae专题图 - 毛小亮 - 博客园
# [ae专题图](https://www.cnblogs.com/xianerwonder/p/4291942.html)
点密度图、分层设色图/等级图、单值图、柱状图、饼状图的实现代码 C#
 private void 点密度图ToolStripMenuItem_Click(object sender, EventArgs e)
        {
            //获取当前图层 ，并把它设置成IGeoFeatureLayer的实例 
            IMap pMap = axMapControl1.Map;
            ILayer pLayer = pMap.get_Layer(0) as IFeatureLayer;
            IFeatureLayer pFeatureLayer = pLayer as IFeatureLayer;
            IGeoFeatureLayer pGeoFeatureLayer = pLayer as IGeoFeatureLayer;
            //获取图层上的feature
            IFeatureClass pFeatureClass = pFeatureLayer.FeatureClass;
            IFeatureCursor pFeatureCursor = pFeatureClass.Search(null, false);
            IFeature pFeature = pFeatureCursor.NextFeature();
            ///////////////////////
            ///////////////////////////////////////////////////////////////////
            //定义点密度图渲染组件
            IDotDensityRenderer DotDensityRenderer = new DotDensityRendererClass();
            //定义点密度图渲染组件对象的渲染字段对象
            IRendererFields flds = (IRendererFields)DotDensityRenderer;
            flds.AddField("FID", "FID");
            //flds.AddField("Shape", "Shape");
            //定义点密度图渲染得符号对象
            IDotDensityFillSymbol ddSym = new DotDensityFillSymbolClass();
            IRgbColor BackColor = new RgbColorClass();
            BackColor.Red = 234;
            BackColor.Blue = 128;
            BackColor.Green = 220;
            IRgbColor SymbolColor = new RgbColorClass();
            SymbolColor.Red = 234;
            SymbolColor.Blue = 128;
            SymbolColor.Green = 220;
            ////点密度图渲染背景颜色
            //ddSym.BackgroundColor = BackColor;
            ddSym.DotSize =8;
            ddSym.FixedPlacement = true;
            //ddSym.Color = SymbolColor;
            ILineSymbol pLineSymbol=new CartographicLineSymbolClass();
            ddSym.Outline = pLineSymbol;
            //定义符号数组 
            ISymbolArray symArray = (ISymbolArray)ddSym;
            //添加点密度图渲染的点符号到符号数组中去
            ISimpleMarkerSymbol pMarkerSymbol = new SimpleMarkerSymbolClass();
            pMarkerSymbol.Style. = esriSimpleMarkerStyle.esriSMSCircle;
            pMarkerSymbol.Size = 2;
            pMarkerSymbol.Color = SymbolColor;;
            symArray.AddSymbol(pMarkerSymbol as ISymbol );
            //设置点密度图渲染的点符号
                    //DotDensityRenderer.DotDensitySymbol =symArray;
               DotDensityRenderer.DotDensitySymbol = ddSym;
            //确定一个点代表多少值
                    DotDensityRenderer .DotValue=0.2;
            //点密度渲染采用的颜色模式
                    DotDensityRenderer.ColorScheme = "Custom";
            //创建点密度图渲染图例
                    DotDensityRenderer.CreateLegend();
            //设置符号大小是否固定
                    DotDensityRenderer.MaintainSize = true;
            //将点密度图渲染对象与渲染图层挂钩
                    pGeoFeatureLayer.Renderer = (IFeatureRenderer)DotDensityRenderer;
            //刷新地图和TOOCotrol
                    IActiveView pActiveView = axMapControl1.Map as IActiveView;
                    pActiveView.Refresh();
                    axTOCControl1.Update();
        }
private void 分层设色ToolStripMenuItem_Click(object sender, EventArgs e)
        {
            //获取当前图层 ，并把它设置成IGeoFeatureLayer的实例 
            IMap pMap = axMapControl1.Map;
            ILayer pLayer = pMap.get_Layer(0) as IFeatureLayer;
            IFeatureLayer pFeatureLayer = pLayer as IFeatureLayer;
            IGeoFeatureLayer pGeoFeatureLayer = pLayer as IGeoFeatureLayer;
            //获取图层上的feature
            IFeatureClass pFeatureClass = pFeatureLayer.FeatureClass;
            IFeatureCursor pFeatureCursor = pFeatureClass.Search(null, false);
            IFeature pFeature = pFeatureCursor.NextFeature();
            //
            IFeatureRenderer PR=pGeoFeatureLayer.Renderer;
            //JoinData("县级区域", "DZGB", "sectioncode");   //join外部表
           // int DC ;  
            int  desiredClasses = 5;
            string fieldName = "AREA";
            int classesCount;
            double[] classes;
            string strOutput = "";
            bool ok;
            object dataFrequency;
            object dataValues;
            ITable pTable ;
            //IClassify pClassify;
            EqualIntervalClass pClassify;
            //IBasicHistogram pTableHistogram = new BasicTableHistogramClass();
            //IHistogram pTableHistogram = new BasicTableHistogramClass();
            ITableHistogram pTableHistogram = new BasicTableHistogramClass() as ITableHistogram;
            IBasicHistogram pHistogram;
            IClassBreaksRenderer pClassBreaksRenderer;
            IHsvColor pFromColor;
            IHsvColor pToColor;
            IAlgorithmicColorRamp pAlgorithmicColorRamp;
            IEnumColors pEnumColors;
            IColor pColor;
            ISimpleFillSymbol pSimpleFillSymbol;
            pLayer = (IFeatureLayer)axMapControl1.get_Layer(0);
            pGeoFeatureLayer = (IGeoFeatureLayer)pLayer;
            pTable = (ITable)pGeoFeatureLayer;
            pHistogram = (IBasicHistogram)pTableHistogram;
            // Get values and frequencies for the field
            pTableHistogram.Field = fieldName;
            pTableHistogram.Table = pTable;
            pHistogram.GetHistogram(out dataValues, out dataFrequency);
            // Put values and frequencies into an Equal Interval Classify Object
            pClassify = new EqualIntervalClass();
            //pClassify = new NaturalBreaksClass();
            pClassify.SetHistogramData(dataValues, dataFrequency);
            pClassify.Classify(dataValues, dataFrequency, ref desiredClasses);
            //pClassify.Classify(ref desiredClasses);
            classes = (double[])pClassify.ClassBreaks;
            classesCount = classes.Length;
            // Initialise a new Class Breaks renderer
            // Supply the number of Class Breaks and the field to perform. the class breaks on
            pClassBreaksRenderer = new ClassBreaksRendererClass();
            pClassBreaksRenderer.Field = fieldName;
            pClassBreaksRenderer.BreakCount = classesCount;
            pClassBreaksRenderer.SortClassesAscending = true;
            // Use algorithmic color ramp to generate an range of colors between YELLOW to RED
            // Initial color: YELLOW
            pFromColor = new HsvColorClass();
            pFromColor.Hue = 60;
            pFromColor.Saturation = 100;
            pFromColor.Value = 96;
            // Final color: RED
            pToColor = new HsvColorClass();
            pToColor.Hue = 0;
            pToColor.Saturation = 100;
            pToColor.Value = 96;
            // Set up HSV Color ramp to span from YELLOW to RED
            pAlgorithmicColorRamp = new AlgorithmicColorRampClass();
            pAlgorithmicColorRamp.Algorithm = esriColorRampAlgorithm.esriHSVAlgorithm;
            pAlgorithmicColorRamp.FromColor = pFromColor;
            pAlgorithmicColorRamp.ToColor = pToColor;
            pAlgorithmicColorRamp.Size = classesCount;
            pAlgorithmicColorRamp.CreateRamp(out ok);
            pEnumColors = pAlgorithmicColorRamp.Colors;
            for (int index = 0; index < classesCount - 1; index++)
            {
                pColor = pEnumColors.Next();
                pSimpleFillSymbol = new SimpleFillSymbolClass();
                pSimpleFillSymbol.Color = pColor;
                pSimpleFillSymbol.Style. = esriSimpleFillStyle.esriSFSSolid;
                pClassBreaksRenderer.set_Symbol(index, (ISymbol)pSimpleFillSymbol);
                pClassBreaksRenderer.set_Break(index, classes[index + 1]);
                // Store each break value for user output
                strOutput += "-" + classes[index + 1] + "/n";
            }
            pGeoFeatureLayer.Renderer = (IFeatureRenderer)pClassBreaksRenderer;
            //this.axMapControl1.Refresh();
           /////////////////////////////////////////////////////////////////////////////////////////
            //////////////////////////////////////////////////////////////////////////////////////////
            //get the custom property from which is supposed to be the layer to be saved
            object customProperty = null;
            //IMapControl3 mapControl = null;
            customProperty = axMapControl1.CustomProperty;
            //ask the user to set a name for the new layer file
            SaveFileDialog saveFileDialog = new SaveFileDialog();
            saveFileDialog.Filter = "Layer File|*.lyr|All Files|*.*";
            saveFileDialog.Title = "生成专题图";
            saveFileDialog.RestoreDirectory = true;
            saveFileDialog.FileName = System.IO.Path.Combine(saveFileDialog.InitialDirectory, pGeoFeatureLayer.Name + ".lyr");
            //get the layer name from the user
            DialogResult dr = saveFileDialog.ShowDialog();
            if (saveFileDialog.FileName != "" && dr == DialogResult.OK)
            {
                if (System.IO.File.Exists(saveFileDialog.FileName))
                {
                    //try to delete the existing file
                    System.IO.File.Delete(saveFileDialog.FileName);
                }
                //create a new LayerFile instance
                ILayerFile layerFile = new LayerFileClass();
                //create a new layer file
                layerFile.New(saveFileDialog.FileName);
                //attach the layer file with the actual layer
                layerFile.ReplaceContents((ILayer)pGeoFeatureLayer);
                //save the layer file
                layerFile.Save();
                //ask the user whether he'd like to add the layer to the map
                if (DialogResult.Yes == MessageBox.Show("Would you like to add the layer to the map?", "Message", MessageBoxButtons.YesNo, MessageBoxIcon.Question))
                {
                    axMapControl1.**AddLayerFromFile**(saveFileDialog.FileName, 0);
                }
            }
        }
    private void 单值图ToolStripMenuItem1_Click(object sender, EventArgs e)
        {
            //获取当前图层 ，并把它设置成IGeoFeatureLayer的实例 
            IMap pMap = axMapControl1.Map;
            ILayer pLayer = pMap.get_Layer(0) as IFeatureLayer;
            IFeatureLayer pFeatureLayer = pLayer as IFeatureLayer;
            IGeoFeatureLayer pGeoFeatureLayer = pLayer as IGeoFeatureLayer;
            //获取图层上的feature
            IFeatureClass pFeatureClass = pFeatureLayer.FeatureClass;
            IFeatureCursor pFeatureCursor = pFeatureClass.Search(null, false);          
            //定义单值图渲染组件    
            IUniqueValueRenderer pUniqueValueRenderer = new UniqueValueRendererClass();
            //设置渲染字段对象         
            pUniqueValueRenderer.FieldCount = 1;
            pUniqueValueRenderer.set_Field(0, "ID");            
            //创建填充符号
            ISimpleFillSymbol PFillSymbol = new SimpleFillSymbolClass();
            pUniqueValueRenderer.DefaultSymbol = (ISymbol)PFillSymbol;
            pUniqueValueRenderer.UseDefaultSymbol = false;    
           //QI the table from the geoFeatureLayer and get the field number of
            ITable pTable;
            int fieldNumber;
            pTable = pGeoFeatureLayer as ITable ;
            fieldNumber = pTable.FindField("ID");
            if (fieldNumber == -1)
            {
                MessageBox.Show("Can't find field called ", "Message", MessageBoxButtons.OK, MessageBoxIcon.Information);
            }
            //创建并设置随机色谱
            IRandomColorRamp pColorRamp = new RandomColorRampClass();
            pColorRamp.StartHue = 0;
            pColorRamp.MinValue = 99;
            pColorRamp.MinSaturation = 15;
            pColorRamp.EndHue = 360;
            pColorRamp.MaxValue = 100;
            pColorRamp.MaxSaturation = 30;
            pColorRamp.Size = 100;
            //pColorRamp.Size = pUniqueValueRenderer.ValueCount;
            bool k = true;
            pColorRamp.CreateRamp(out ok);
            IEnumColors pEnumRamp;
            pEnumRamp = pColorRamp.Colors;        
          //为每个值设置一个符号
          int n = pFeatureClass.FeatureCount(null);              
          for (int i = 0; i < n; i++)
          {
              IFeature pFeature = pFeatureCursor.NextFeature();
              IClone pSourceClone = PFillSymbol as IClone;
              ISimpleFillSymbol pSimpleFillSymbol = pSourceClone.Clone() as ISimpleFillSymbol ;
              string pFeatureValue = pFeature.get_Value(pFeature.Fields.FindField("ID")).ToString();
              pUniqueValueRenderer.AddValue(pFeatureValue, "烈度", (ISymbol)pSimpleFillSymbol);
          }
          //为每个符号设置颜色
          for (int i = 0; i <= pUniqueValueRenderer.ValueCount - 1; i++)
          {
              string xv = pUniqueValueRenderer.get_Value(i);
              if (xv != "")
              {
                  ISimpleFillSymbol pNextSymbol = (ISimpleFillSymbol)pUniqueValueRenderer.get_Symbol(xv);
                  pNextSymbol.Color = pEnumRamp.Next();
                  pUniqueValueRenderer.set_Symbol(xv, (ISymbol)pNextSymbol);
              }
          }
          //将单值图渲染对象与渲染图层挂钩
          pGeoFeatureLayer.Renderer = (IFeatureRenderer)pUniqueValueRenderer;
          pGeoFeatureLayer.DisplayField = "ID";
          //刷新地图和TOOCotrol
          IActiveView pActiveView = axMapControl1.Map as IActiveView;
          pActiveView.Refresh();
          axTOCControl1.Update();
        }
private void 等级图ToolStripMenuItem_Click(object sender, EventArgs e)
        {
            //获取当前图层 ，并把它设置成IGeoFeatureLayer的实例 
            IMap pMap = axMapControl1.Map;
            ILayer pLayer = pMap.get_Layer(0) as IFeatureLayer;
            IFeatureLayer pFeatureLayer = pLayer as IFeatureLayer;
            IGeoFeatureLayer pGeoFeatureLayer = pLayer as IGeoFeatureLayer;
            //获取图层上的feature
            IFeatureClass pFeatureClass = pFeatureLayer.FeatureClass;
            IFeatureCursor pFeatureCursor = pFeatureClass.Search(null, false);
            IFeature pFeature = pFeatureCursor.NextFeature();
            //////////////////////////////////////////////////////////////////////
            //定义所需的接口对象和相关变量
            IClassBreaksUIProperties pUIProperties;
            object dataValues;
            object dataFrequency;
            //double[] cb;
            int breakIndex;
            long ClassesCount;
            int numClass;
            numClass = 10;
            double[] Classes;
            //////////////////////////////////////////////////////////////////////
            /* We're going to retrieve frequency data from a population 
              field and then classify this data*/
            ITable pTable;
            pTable = pFeatureClass as ITable;
            IBasicHistogram pBasicHist = new BasicTableHistogramClass();
            ITableHistogram pTableHist;
            pTableHist = (ITableHistogram)pBasicHist;
            //Get values and frequencies for the population field into a table histogram object
            pTableHist.Field = "AREA";
            pTableHist.Table = pTable;
            pBasicHist.GetHistogram(out dataValues, out dataFrequency);
            IClassifyGEN pClassifyGEN = new QuantileClass();
            pClassifyGEN.Classify(dataValues, dataFrequency, ref numClass);
            Classes = (double[])pClassifyGEN.ClassBreaks;
            ClassesCount = long.Parse(Classes.GetUpperBound(0).ToString());
            //Initialise a new class breaks renderer and supply the number of class breaks and the field to perform. the class breaks on.
            IClassBreaksRenderer pClassBreaksRenderer = new ClassBreaksRendererClass();
            pClassBreaksRenderer.Field = "AREA";
            //pClassBreaksRenderer.BreakCount = ClassesCount;
            pClassBreaksRenderer.MinimumBreak = Classes[0];
            pClassBreaksRenderer.SortClassesAscending = true;
            //设置着色对象的分级数目
            pClassBreaksRenderer.BreakCount = int.Parse(ClassesCount.ToString());
            //创建并设置随机色谱
            IAlgorithmicColorRamp pColorRamp = new AlgorithmicColorRampClass();
            pColorRamp.Algorithm = esriColorRampAlgorithm.esriCIELabAlgorithm;
            IEnumColors pEnumColors;
            IRgbColor pColor1 = new RgbColorClass();
            IRgbColor pColor2 = new RgbColorClass();
            pColor1.Red = 255;
            pColor1.Green = 210;
            pColor1.Blue = 210;
            pColor2.Red = 190;
            pColor2.Green = 0;
            pColor2.Blue = 170;
            pColorRamp.FromColor = pColor1;
            pColorRamp.ToColor = pColor2;
            pColorRamp.Size = numClass;
            bool k = true;
            pColorRamp.CreateRamp(out ok);
            pEnumColors = pColorRamp.Colors;
            pEnumColors.Reset();// use this interface to set dialog properties
            pUIProperties = pClassBreaksRenderer as IClassBreaksUIProperties;
            pUIProperties.ColorRamp = "Custom";
            ISimpleFillSymbol pSimpleMarkerSymbol = new SimpleFillSymbolClass();
            IColor pColor;
            int[] colors = new int[numClass];
            // be careful, indices are different for the diff lists    
            for (breakIndex = 0; breakIndex < ClassesCount; breakIndex++)
            {
                pClassBreaksRenderer.set_Label (breakIndex,Classes[breakIndex] + " - " + Classes[breakIndex + 1]);
                pUIProperties.set_LowBreak  (breakIndex ,Classes[breakIndex]);
                pSimpleMarkerSymbol = new SimpleFillSymbolClass();
                pColor = pEnumColors.Next();
                pSimpleMarkerSymbol.Color = pColor;
                colors[breakIndex] = pColor.RGB;
                pClassBreaksRenderer.set_Symbol(breakIndex, (ISymbol)pSimpleMarkerSymbol);
                pClassBreaksRenderer.set_Break(breakIndex, Classes[breakIndex + 1]);
            }   
            //将等级图渲染对象与渲染图层挂钩
            pGeoFeatureLayer.Renderer = (IFeatureRenderer)pClassBreaksRenderer;
            //刷新地图和TOOCotrol
            IActiveView pActiveView = axMapControl1.Map as IActiveView;
            pActiveView.Refresh();
            axTOCControl1.Update();
        }     
    private void 柱状图ToolStripMenuItem_Click(object sender, EventArgs e)
        {
            //获取当前图层 ，并把它设置成IGeoFeatureLayer的实例 
            IMap pMap = axMapControl1.Map;
            ILayer pLayer = pMap.get_Layer(0) as IFeatureLayer;
            IFeatureLayer pFeatureLayer = pLayer as IFeatureLayer;
            IGeoFeatureLayer pGeoFeatureLayer = pLayer as IGeoFeatureLayer;
            //获取图层上的feature
            IFeatureClass pFeatureClass = pFeatureLayer.FeatureClass;
            //定义柱状图渲染组建对象
            IChartRenderer pChartRenderer = new ChartRendererClass();
            //定义渲染字段对象并给字段对象实例化为pChartRenderer
            IRendererFields pRendererFields;
            pRendererFields = (IRendererFields)pChartRenderer;
           //向渲染字段对象中添加字段--- 待补充自定义添加
            pRendererFields.AddField("AREA", "AREA");
            pRendererFields.AddField("ID", "ID");         
           // 通过查找features的所用字段的值，计算出数据字段的最大值，作为设置柱状图的比例大小的依据
            ITable pTable;
            int fieldNumber;
            pTable = pGeoFeatureLayer as ITable;
            // 查找出geoFeatureLayer的属性表中的字段个数
            fieldNumber = pTable.FindField("AREA");
            if (fieldNumber == -1)
            {
                MessageBox.Show("Can't find field called ", "Message", MessageBoxButtons.OK, MessageBoxIcon.Information);
            }
            const int  numFields = 2;// 设置bars的个数 
            int[] fieldIndecies = new int[2];
            //long fieldIndex;
            double maxValue;
            bool firstValue;
            //double[] fieldValue=new double[5];
            double  fieldValue;
            fieldIndecies[0] = pTable.FindField("AREA");
            fieldIndecies[1] = pTable.FindField("ID");
            firstValue = true;
            maxValue = 0;          
             int n = pFeatureClass.FeatureCount(null);
             for (int i = 0; i < numFields; i++)
             {
                 IFeatureCursor pFeatureCursor = pFeatureClass.Search(null, false);
                 for (int j = 0; j < n; j++)
                 {
                     IFeature pFeature = pFeatureCursor.NextFeature();
                     fieldValue = Convert.ToDouble(pFeature.get_Value(fieldIndecies[i]));                
                     if (firstValue)
                     {
                         //给maxValue赋初值
                         maxValue = fieldValue;
                         firstValue = false;
                     }
                     else if (fieldValue > maxValue)
                     {
                         maxValue = fieldValue;
                     }
                 }
             }
            if (maxValue <= 0)
            {
                MessageBox.Show("Failed to calculate the maximum value or maxvalue is 0.", "Message", MessageBoxButtons.OK, MessageBoxIcon.Information);
            }          
            // 定义并设置渲染时用的chart marker symbol
            IBarChartSymbol pBarChartSymbol = new BarChartSymbolClass();
            pBarChartSymbol.Width = 6;
            IChartSymbol pChartSymbol;
            pChartSymbol = pBarChartSymbol as IChartSymbol;
            IMarkerSymbol pMarkerSymbol;
            pMarkerSymbol = (IMarkerSymbol)pBarChartSymbol;
            IFillSymbol pFillSymbol ;
            //设置pChartSymbol的最大值
            pChartSymbol.MaxValue = maxValue;
            // 设置bars的最大高度 
            pMarkerSymbol.Size = 16;
            //下面给每一个bar设置符号
            //定义符号数组
            ISymbolArray  pSymbolArray = (ISymbolArray)pBarChartSymbol;
            //克隆pFillSymbol用于符号操作
            //IClone pSourceClone = pFillSymbol as IClone;
            //ISimpleFillSymbol pSimpleFillSymbol = pSourceClone.Clone() as ISimpleFillSymbol;
            // 向符号数组中添加设置后的符号
            //pSimpleFillSymbol.Color = GetRGBColor(193, 252, 179);
            //pSymbolArray.AddSymbol(pSimpleFillSymbol as ISymbol);
            //pSimpleFillSymbol.Color = GetRGBColor(145, 55, 200);
            //pSymbolArray.AddSymbol(pSimpleFillSymbol as ISymbol);
            //添加第一个符号
            pFillSymbol = new SimpleFillSymbolClass();
            pFillSymbol.Color = GetRGBColor(193, 252, 179) as IColor ;
            pSymbolArray.AddSymbol(pFillSymbol as ISymbol) ;
            //添加第二个符号
            pFillSymbol = new SimpleFillSymbolClass();
            pFillSymbol.Color = GetRGBColor(145, 55, 251)as IColor;
            pSymbolArray.AddSymbol(pFillSymbol as ISymbol);
           // 设置背景符号 
            //pSimpleFillSymbol.Color = GetRGBColor(239, 150, 190);
            //pChartRenderer.BaseSymbol = pSimpleFillSymbol as ISymbol;
           // Disable overpoaster 让符号处于图形中央 
            pChartRenderer.UseOverposter = false;
            //pChartRenderer.ChartSymbol = pSymbolArray as IChartSymbol;
            pChartRenderer.ChartSymbol = pChartSymbol as IChartSymbol;
            //pChartRenderer.Label = "AREA";
            pChartRenderer.CreateLegend();
            //将柱状图渲染对象与渲染图层挂钩
            pGeoFeatureLayer.Renderer = (IFeatureRenderer)pChartRenderer;
            pGeoFeatureLayer.DisplayField = "ID";
            //刷新地图和TOOCotrol
            IActiveView pActiveView = axMapControl1.Map as IActiveView;
            pActiveView.Refresh();
            axTOCControl1.Update();
        }
private void asdToolStripMenuItem_Click(object sender, EventArgs e)
        {
            //获取当前图层 ，并把它设置成IGeoFeatureLayer的实例 
            IMap pMap = axMapControl1.Map;
            ILayer pLayer = pMap.get_Layer(0) as IFeatureLayer;
            IFeatureLayer pFeatureLayer = pLayer as IFeatureLayer;
            IGeoFeatureLayer pGeoFeatureLayer = pLayer as IGeoFeatureLayer;
            //获取图层上的feature
            IFeatureClass pFeatureClass = pFeatureLayer.FeatureClass;
            IFeatureCursor pFeatureCursor = pFeatureClass.Search(null, false);
            IFeature pFeature = pFeatureCursor.NextFeature();
            //////////////////////////////////////////////////////////////////////
            /////////////////////////////////////////////////////////////////////////////////////////
            IBasicHistogram pHistogram;
            ITableHistogram pTableHistogram = new  BasicTableHistogramClass();
            ITable pTable;
            IClassifyGEN pClassifyGEN = new EqualIntervalClass();
            long ClassesCount;
            IClassBreaksRenderer pClassBreaksRenderer = new ClassBreaksRendererClass();
            IAlgorithmicColorRamp pAlgorithmicColorRamp = new AlgorithmicColorRampClass();
            ISimpleMarkerSymbol pSimpleMarkerSymbol = new SimpleMarkerSymbolClass();
            IEnumColors pEnumColors;
            object dataFrequency;
            object dataValue;
            double[] Classes;
            int numClass;
            numClass = 10;
            pTable = (ITable)pGeoFeatureLayer;
            //根据渲染字段得到渲染信息赋值给两个数组。
            pHistogram = (IBasicHistogram)pTableHistogram;
            pTableHistogram.Field = "AREA";
            pTableHistogram.Table = pTable;
            pHistogram.GetHistogram(out dataValue, out dataFrequency);
            //下面是分级方法，用于根据获得的值计算得出符合条件要求的数据
            //根据条件计算出Classes和ClassesCount.
            pClassifyGEN.Classify(dataValue, dataFrequency, ref numClass);
            //返回一个数组，注意这个数组的下标是从1开始的
            Classes = (double[])pClassifyGEN.ClassBreaks;
            ClassesCount = long.Parse(Classes.GetUpperBound(0).ToString());
            pClassBreaksRenderer.Field = "AREA";
            //创建并设置随机色谱
            IAlgorithmicColorRamp pColorRamp = new AlgorithmicColorRampClass();
            pColorRamp.Algorithm = esriColorRampAlgorithm.esriCIELabAlgorithm;
            IRgbColor pColor1 = new RgbColorClass();
            IRgbColor pColor2 = new RgbColorClass();
            pColor1.Red = 255;
            pColor1.Green = 210;
            pColor1.Blue = 210;
            pColor2.Red = 190;
            pColor2.Green = 0;
            pColor2.Blue = 170;
            pColorRamp.FromColor = pColor1;
            pColorRamp.ToColor = pColor2;
            pColorRamp.Size = 20;
            bool k = true;
            pColorRamp.CreateRamp(out ok);
            pEnumColors = pColorRamp.Colors;
            pEnumColors.Reset();// use this interface to set dialog properties
            //设置着色对象的分级数目
            pClassBreaksRenderer.BreakCount = int.Parse(ClassesCount.ToString());
            pClassBreaksRenderer.SortClassesAscending = true;
            //设置颜色条带：
            pAlgorithmicColorRamp.FromColor = pColor1;
            pAlgorithmicColorRamp.ToColor = pColor2;
            pAlgorithmicColorRamp.Size = numClass;
            bool re = false;
            pAlgorithmicColorRamp.CreateRamp(out re);
            pEnumColors = pAlgorithmicColorRamp.Colors;
            pEnumColors.Reset();
             //定义图形边界符号
            ILineSymbol pLineSymbol;
            IColor pColor;
            int[] colors = new int[numClass];
            int breakIndex = 0;
            for (breakIndex = 0; breakIndex < ClassesCount; breakIndex++)
            {
                //pEnumColors.Reset();
                pColor = pEnumColors.Next();
                pSimpleMarkerSymbol.Color = pColor;
                colors[breakIndex] = pColor.RGB;
                pSimpleMarkerSymbol.Style. = esriSimpleMarkerStyle.esriSMSSquare;//.esriSMSCircle;
                pSimpleMarkerSymbol.Size = 10;
                pClassBreaksRenderer.set_Symbol(breakIndex, (ISymbol)pSimpleMarkerSymbol);
                pClassBreaksRenderer.set_Break(breakIndex, Classes[breakIndex + 1]);
                pLineSymbol = new CartographicLineSymbolClass();
                //pSimpleMarkerSymbol.Outline= true ;
               // pEnumColors.Reset();
                //pSimpleMarkerSymbol.OutlineColor = pEnumColors.Next(); ;
            }

