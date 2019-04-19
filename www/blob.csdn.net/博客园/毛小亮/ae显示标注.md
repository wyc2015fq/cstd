# ae显示标注 - 毛小亮 - 博客园
# [ae显示标注](https://www.cnblogs.com/xianerwonder/p/4293160.html)
```
//添加标注,比TextElment功能更强大
        public static void ToAddAnnotate(ILayer layer, string fieldName)
        {
            IGeoFeatureLayer pGeoLayer = layer as IGeoFeatureLayer;
            IAnnotateLayerPropertiesCollection IPALPColl = pGeoLayer.AnnotationProperties;
            IPALPColl.Clear();
            IColor pColor = GetRGB_Color.GetColor(0, 0, 0);
            IFontDisp pFont = new StdFont()
            {
                Name = "宋体",
                Bold = true
            } as IFontDisp;
            ITextSymbol pTextSymbol = new TextSymbolClass()
            {
                Color = pColor,
                Font = pFont,
                Size = 12
            };
            //用来控制标注和要素的相对位置关系
            ILineLabelPosition pLineLpos = new LineLabelPositionClass()
            {
                //修改标注的属性
                Parallel = false,  //是否与线平行放置
                Perpendicular = true,//是否与线垂直放置
                InLine = true
            };
            //用来控制标注冲突
            ILineLabelPlacementPriorities pLinePlace = new LineLabelPlacementPrioritiesClass()
            {
                AboveStart = 5, //让above 和start的优先级为5
                BelowAfter = 4
            };
            //用来实现对ILineLabelPosition 和 ILineLabelPlacementPriorities以及更高级属性的控制
            IBasicOverposterLayerProperties pBOLP = new BasicOverposterLayerPropertiesClass()
            {
                FeatureType = esriBasicOverposterFeatureType.esriOverposterPolygon,
                LineLabelPlacementPriorities = pLinePlace,
                LineLabelPosition = pLineLpos
            };
            //创建标注对象
            ILabelEngineLayerProperties pLableEngine = new LabelEngineLayerPropertiesClass()
            {
                Symbol = pTextSymbol,
                BasicOverposterLayerProperties = pBOLP,
                IsExpressionSimple = true,
                Expression = "[" + fieldName + "]"
            };
            //设置标注的参考比例尺
            IAnnotateLayerTransformationProperties pAnnoLyrPros = pLableEngine as IAnnotateLayerTransformationProperties;
            //pAnnoLyrPros.ReferenceScale = 2500000;
            //设置标注可见的最大最小比例尺
            IAnnotateLayerProperties pAnnoPros = pLableEngine as IAnnotateLayerProperties;
            //pAnnoPros.AnnotationMaximumScale = 2500000;
            //pAnnoPros.AnnotationMinimumScale = 25000000;
            //pAnnoPros.WhereClause属性  设置过滤条件
            IPALPColl.Add(pAnnoPros);
            pGeoLayer.DisplayAnnotation = true;
        }
```

