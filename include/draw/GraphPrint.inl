
void CGraph::PrintBarSeries(HDDC hDC)
{
  int barWidth;
  int dataPlotSize;   //used to plot rects of data
  int barL, barT, barR, barB;
  int tickXLocation, tickYLocation;
  int seriesSpace;
  int barHeight;  //for scalability



  if (graphAlignment) {
    //determine width of barchart data blocks
    seriesSpace = xAxisWidth / ngraphSeries;
    barWidth = (int)((seriesSpace * .6) / seriesSize);
    dataPlotSize = seriesSize * barWidth;



    for (int x = 1; x <= ngraphSeries; x++) {


      tickXLocation = xApexPoint + ((x * seriesSpace) - (seriesSpace / 2));

      for (int s = 0; s < seriesSize; s++) {
        if (data[x * seriesSize + s] > -1) {
          double dataScale = 0.00;
          dataScale = ((yAxisHeight - 100) * 1.00) / (maxTick - minTick);
          barHeight = (int)(((data[x * seriesSize + s] - minTick) * 1.00) * dataScale);

          if (((data[x * seriesSize + s] - minTick) > 0) && (barHeight < 1)) {
            barHeight = (int)dataScale;
          }

          barL = tickXLocation - (dataPlotSize / 2) + (s * barWidth);

          if (barHeight > yAxisHeight) {
            barT = yApexPoint + yAxisHeight;
          }
          else {
            barT = yApexPoint + barHeight;
          }

          barR = barL + barWidth;
          barB = yApexPoint;

          draw_rect(hDC, barL, barT, barR, barB, graphLegend[s].color);

        }
      }
    }
  }
  else {
    //determine width of barchart data blocks
    seriesSpace = yAxisHeight / ngraphSeries;
    barHeight = (int)((seriesSpace * .6) / seriesSize);
    dataPlotSize = seriesSize * barHeight;



    for (int y = 0; y < ngraphSeries; y++) {


      tickYLocation = yApexPoint + ((y * seriesSpace) - (seriesSpace / 2));

      for (int s = 0; s < seriesSize; s++) {
        if (data[x * seriesSize + s] > -1) {
          double dataScale = 0.00;
          dataScale = ((xAxisWidth - 100) * 1.00) / (maxTick - minTick);

          barWidth = (int)(((data[x * seriesSize + s] - minTick) * 1.00) * dataScale);

          if (((data[x * seriesSize + s] - minTick) > 0) && (barWidth < 1)) {
            barWidth = (int)dataScale;
          }

          barL = xApexPoint;
          barT = tickYLocation - (dataPlotSize / 2) + (s * barHeight);
          barB = barT + barHeight;

          if (barWidth > xAxisWidth) {
            barR = xApexPoint + xAxisWidth;
          }
          else {
            barR = xApexPoint + barWidth;
          }

          draw_rect(hDC, barL, barT, barR, barB, graphLegend[s].color);

        }
      }
    }
  }
}

void CGraph::PrintLineSeries(HDDC hDC)
{
  int barWidth;
  int dataPlotSize;   //used to plot rects of data
  int tickXLocation, tickYLocation;
  int seriesSpace;
  int barHeight;  //for scalability



  int lastXLoc, lastYLoc;

  if (graphAlignment) {
    for (int s = 0; s < seriesSize; s++) {
      //determine width of barchart data blocks
      seriesSpace = xAxisWidth / ngraphSeries;
      barWidth = (int)((seriesSpace * .6) / seriesSize);
      dataPlotSize = seriesSize * barWidth;



      for (int x = 1; x <= ngraphSeries; x++) {


        if (data[x * seriesSize + s] > -1) {
          tickXLocation = xApexPoint + ((x * seriesSpace) - (seriesSpace / 2));

          barHeight = 0;
          double dataScale = 0.00;

          if ((maxTick - minTick) != yAxisHeight) {
            dataScale = ((yAxisHeight * 1.00) / ((maxTick - minTick) * 1.00)) * tickSpace;
          }
          else {
            dataScale = tickSpace * 15.00;
          }

          int axisHeight = (int)(dataScale * numTicks);
          double barPercent = 0.00;
          barPercent = ((data[x * seriesSize + s] - minTick) * 1.00) / maxTick;
          barHeight = (int)(barPercent * axisHeight);

          if (((data[x * seriesSize + s] - minTick) > 0) && (barHeight < 1)) {
            barHeight = (int)dataScale;
          }

          int yDataLocation = yApexPoint + barHeight;

          //now have x and y location of center of ellipse
          COLORREF barColor;
          barColor = graphLegend[s].color;



          //draw line back to last data member
          if (x > 1) {

            COLORREF linePen(PS_SOLID, 20, barColor);
            _clr = (linePen);
            TMoveTo(hDC, lastXLoc, lastYLoc);
            _LineTo(hDC, tickXLocation, yDataLocation);

          }

          //now draw ellipse...
          hDC->Ellipse(tickXLocation + 60, yDataLocation + 60,
              tickXLocation - 60, yDataLocation - 60);
          lastXLoc = tickXLocation;
          lastYLoc = yDataLocation;

        }
      }
    }
  }
  else {
    for (int s = 0; s < seriesSize; s++) {
      //determine width of barchart data blocks
      seriesSpace = yAxisHeight / ngraphSeries;
      barHeight = (int)((seriesSpace * .6) / seriesSize);
      dataPlotSize = seriesSize * barHeight;



      for (int x = 1; x <= ngraphSeries; x++) {


        if (data[x * seriesSize + s] > -1) {
          tickYLocation = yApexPoint + ((x * seriesSpace) - (seriesSpace / 2));

          barWidth = 0;
          double dataScale = 0.00;

          dataScale = ((xAxisWidth - 100) * 1.00) / (maxTick - minTick);

          int axisWidth = (int)(dataScale * (maxTick - minTick));
          double barPercent = 0.00;
          barPercent = ((data[x * seriesSize + s] - minTick) * 1.00) / ((maxTick - minTick) * 1.00);
          barWidth = (int)(barPercent * axisWidth);

          if (((data[x * seriesSize + s] - minTick) > 0) && (barHeight < 1)) {
            barWidth = (int)dataScale;
          }

          int xDataLocation = xApexPoint + barWidth;

          //now have x and y location of center of ellipse
          COLORREF barColor;
          barColor = graphLegend[s].color;



          //draw line back to last data member
          if (x > 1) {

            COLORREF linePen(PS_SOLID, 20, barColor);
            _clr = (linePen);
            TMoveTo(hDC, lastXLoc, lastYLoc);
            _LineTo(hDC, xDataLocation, tickYLocation);

          }

          //now draw ellipse...
          hDC->Ellipse(xDataLocation + 60, tickYLocation + 60,
              xDataLocation - 60, tickYLocation - 60);
          lastYLoc = tickYLocation;
          lastXLoc = xDataLocation;

        }
      }
    }
  }
}

void CGraph::PrintPieSeries(HDDC hDC)
{
  int seriesSpace;



  double dataSum = 0.00;  //for storing cumulative sum
  int lastXLocation, lastYLocation;
  int newXLocation, newYLocation;
  double percent = 0.00;
  int degrees;
  double totalSum = 0.00;
  int deltaXY;
  int radius;

  double labelData = 0.00;
  int labelXLocation, labelYLocation;
  double labelPercent = 0.00;
  int labelDegrees;
  int labelDeltaXY;
  int labelLineXStart, labelLineYStart;
  int labelLineXEnd, labelLineYEnd;
  int maxLabelWidth;
  int maxLabelHeight;
  TEXTMETRIC tm;

  lastXLocation = 0;
  lastYLocation = 0;

  tm = GetCurFont()->tm;
  maxLabelWidth = tm.tmMaxCharWidth + 50;
  maxLabelHeight = tm.tmHeight + 30;

  //pie labels are stored in a list and drawn after entire pie
  //is completed.
  CObList* pieLabels;
  CGraphPieLabel* pieLabel;

  //determine width of pie display area
  if (xAxisWidth > yAxisHeight) {
    seriesSpace = yAxisHeight / ngraphSeries;
  }
  else {
    seriesSpace = xAxisWidth / ngraphSeries;
  }

  seriesSpace -= (3 * maxLabelWidth);  //allows text like 25%  (3 chars)

  //to plot a pie plus labels inside of series space, use the following :
  //(3 * radius) + (2 * label width) = series space
  //so, 3*radius = series space - (2 * label width)
  // 1 radius = (series space - (2 * label width)) / 3
  radius = seriesSpace / 3;  //pie needs 2 radius, + 1 extra for line to labels = 3 for my divisor

  int centerYPie = pGraphT - (yAxisHeight / 2) - 500;


  for (int x = 1; x <= ngraphSeries; x++) {
    pieLabels = new CObList();


    totalSum = 0;

    for (int s = 0; s < seriesSize; s++) {
      totalSum += data[x * seriesSize + s];
    }

    int pieLeft, pieRight;

    if (ngraphSeries == 1) {
      pieLeft = xApexPoint + (xAxisWidth / 2) - radius;
    }
    else {
      pieLeft = xApexPoint + 15 + ((x * 2 - 1) * ((xAxisWidth / ngraphSeries) / 2)) - radius;
    }

    pieRight = pieLeft + (2 * radius);
    RECT pieRect(pieLeft,
        centerYPie + radius,
        pieRight,
        centerYPie - radius);
    int centerXPie = pieLeft + radius;

    int pieFont;
    pieFont = font_create("Arial", xTickFontSize * 20, 0);
    int pOldFont = TSelectFont(pieFont);

    //plot series label
    TTextOut2(hDC, centerXPie - ((STRLEN(graphSeries[x]) / 2) * ((xTickFontSize / 2) * 20)),
        centerYPie - radius - maxLabelHeight - 600, graphSeries[x]);
    TSelectFont(pOldFont);

    lastXLocation = pieLeft;
    lastYLocation = centerYPie;

    dataSum = 0;

    for (s = 0; s < seriesSize; s++) {
      if (data[x * seriesSize + s] > 0) {
        int seriesDataValue;
        seriesDataValue = data[x * seriesSize + s];
        dataSum += seriesDataValue;
        percent = (dataSum * 100) / totalSum;
        degrees = (int)((360 * percent) / 100);

        labelData = data[x * seriesSize + s] / 2;
        labelPercent = ((dataSum - labelData) / totalSum) * 100;
        labelDegrees = (int)((360 * labelPercent) / 100);

        //degress / 90 will never exceed 4.
        //this can tell us the quadrant of destination
        deltaXY = (degrees * radius) / 90;
        labelDeltaXY = (labelDegrees * radius) / 90;
        //deltaXY is change from start point of pie 0
        //this value is total change.  so if radius is 300
        //and degrees is 270, delta is 300.  The change
        //would move both x and y 300 pixels.  For x, 100
        //to right is center, another 100 to right edge,
        //100 back to center.  For y, 100 to bottom, 100
        //back to center, 100 to top. thus gives 270 degree
        //rotation.

        //determine destination quadrant...
        //and set newXLocation and newYLocation values...
        int quadrant = degrees / 90;  //truncates decimal
        int labelQuadrant = labelDegrees / 90;

        switch (quadrant) {
          //in the computations below, the remarked line is
          //the original computation.  The line above it, for
          //actual use, is the simplified line, and gives the
          //exact same result
        case 0 :
          newXLocation = pieLeft + deltaXY;
          newYLocation = centerYPie - deltaXY;
          break;

        case 1 :
          newXLocation = pieLeft + deltaXY;
          newYLocation = centerYPie - (2 * radius) + deltaXY;
          break;

        case 2 :
          newXLocation = pieLeft + (4 * radius) - deltaXY;
          newYLocation = centerYPie - (2 * radius) + deltaXY;
          break;

        case 3 :
          newXLocation = pieLeft + (4 * radius) - deltaXY;
          newYLocation = centerYPie + (4 * radius) - deltaXY;
          break;

        case 4 :
          newXLocation = pieLeft;
          newYLocation = centerYPie + 1;
          break;
        }

        switch (labelQuadrant) {
          //after getting X & Y location for label, we take
          //1/2 the delta between x y locations and center pie
        case 0 :
          labelXLocation = pieLeft + labelDeltaXY;
          labelYLocation = centerYPie - labelDeltaXY;
          labelLineXStart = labelXLocation + ((centerXPie - labelXLocation) / 2);
          labelLineYStart = labelYLocation - ((labelYLocation - centerYPie) / 2);
          labelLineXEnd = labelXLocation - (radius / 2);
          labelLineYEnd = labelYLocation - (radius / 2);
          break;

        case 1 :
          labelXLocation = pieLeft + labelDeltaXY;
          labelYLocation = centerYPie - (2 * radius) + labelDeltaXY;
          labelLineXStart = labelXLocation - ((labelXLocation - centerXPie) / 2);
          labelLineYStart = labelYLocation - ((labelYLocation - centerYPie) / 2);
          labelLineXEnd = labelXLocation + (radius / 2);
          labelLineYEnd = labelYLocation - (radius / 2);
          break;

        case 2 :
          labelXLocation = pieLeft + (4 * radius) - labelDeltaXY;
          labelYLocation = centerYPie - (2 * radius) + labelDeltaXY;
          labelLineXStart = labelXLocation - ((labelXLocation - centerXPie) / 2);
          labelLineYStart = labelYLocation - ((labelYLocation - centerYPie) / 2);
          labelLineXEnd = labelXLocation + (radius / 2);
          labelLineYEnd = labelYLocation + (radius / 2);
          break;

        case 3 :
          labelXLocation = pieLeft + (4 * radius) - labelDeltaXY;
          labelYLocation = centerYPie + (4 * radius) - labelDeltaXY;
          labelLineXStart = labelXLocation + ((centerXPie - labelXLocation) / 2);
          labelLineYStart = labelYLocation - ((labelYLocation - centerYPie) / 2);
          labelLineXEnd = labelXLocation - (radius / 2);
          labelLineYEnd = labelYLocation + (radius / 2);
          break;
          //there should never be a half point ending on 4, so leave it out
        }

        pieLabel = new CGraphPieLabel();

        pieLabel->lineXStart = labelLineXStart;
        pieLabel->lineYStart = labelLineYStart;
        pieLabel->lineXEnd = labelLineXEnd;
        pieLabel->lineYEnd = labelLineYEnd;

        switch (labelQuadrant) {
        case 0 : //label to left of line
          pieLabel->topLeftX = labelLineXEnd - maxLabelWidth;
          pieLabel->topLeftY = labelLineYEnd;
          break;

        case 1 : //label to right of line
          pieLabel->topLeftX = labelLineXEnd + 50;
          pieLabel->topLeftY = labelLineYEnd;
          break;

        case 2 : //label to right of line
          pieLabel->topLeftX = labelLineXEnd + 50;
          pieLabel->topLeftY = labelLineYEnd + maxLabelHeight;
          break;

        case 3 : //label to left of line
          pieLabel->topLeftX = labelLineXEnd - maxLabelWidth;
          pieLabel->topLeftY = labelLineYEnd + maxLabelHeight;
          break;
        }

        pieLabel->labelQuadrant = labelQuadrant;
        int roundPercent;
        roundPercent = (int)((seriesDataValue * 100) / totalSum);
        pieLabel->pieLabel.Format("%d%%", roundPercent);
        pieLabels->AddTail(pieLabel);

        if (s == 0) {
          lastYLocation -= 1;
        }

        POINT p1(lastXLocation, lastYLocation);
        POINT p2(newXLocation, newYLocation);
        COLORREF barColor;
        barColor = graphLegend[s].color;


        COLORREF piePen(barColor);


        _clr = (piePen);
        hDC->Pie(pieRect, p1, p2);



        lastXLocation = newXLocation;
        lastYLocation = newYLocation;
      }
    }

    //draw lines and labels for pies slices
    POSITION labelPos;

    COLORREF lineBrush(BLACK);

    pOldBrush = TSelectFont(lineBrush);
    POSITION mainLinePos, checkLinePos;
    mainLinePos = pieLabels->GetHeadPosition();
    int numLinesDrawn = 0;
    CGraphPieLabel* currentLabel;
    CGraphPieLabel* tmpLabel;

    while (mainLinePos != NULL) {
      currentLabel = (CGraphPieLabel*)pieLabels->GetNext(mainLinePos);
      int r = 0;
      checkLinePos = pieLabels->GetHeadPosition();

      while (r < numLinesDrawn) {
        //check if any overlap in label areas
        tmpLabel = (CGraphPieLabel*)pieLabels->GetAt(checkLinePos);

        if ((currentLabel->topLeftX > tmpLabel->topLeftX) &&
            (currentLabel->topLeftX < (tmpLabel->topLeftX + maxLabelWidth)) &&
            (currentLabel->topLeftY < tmpLabel->topLeftY) &&
            (currentLabel->topLeftY > (tmpLabel->topLeftY - maxLabelHeight))) {
          //OVERLAP !!!
          //move current label top left position up or down
          //depending on its quadrant
          if (currentLabel->labelQuadrant < 2) {
            //move label down to tmplabel topleft + height
            currentLabel->topLeftY = tmpLabel->topLeftY - maxLabelHeight;
            currentLabel->lineYEnd = tmpLabel->lineYEnd - maxLabelHeight;
          }
          else {
            //move label up to tmpLabel topleft - height
            currentLabel->topLeftY = tmpLabel->topLeftY + maxLabelHeight;
            currentLabel->lineYEnd = tmpLabel->lineYEnd + maxLabelHeight;
          }

          //reset r value to 0 so it starts over, just in
          //case we moved the label and it overlaps another
          r = 0;
          checkLinePos = pieLabels->GetHeadPosition();
        }
        else {
          r++;
          pieLabels->GetNext(checkLinePos);
        }

      }

      //draw the line and label
      TMoveTo(hDC, currentLabel->lineXStart, currentLabel->lineYStart);
      _LineTo(hDC, currentLabel->lineXEnd, currentLabel->lineYEnd);

      //write the label
      TTextOut2(hDC, currentLabel->topLeftX, currentLabel->topLeftY,
          currentLabel->pieLabel);
      numLinesDrawn++;
    }



    //now done, remove everything inside the label list
    labelPos = pieLabels->GetHeadPosition();

    while (labelPos != NULL) {
      pieLabel = (CGraphPieLabel*)pieLabels->GetNext(labelPos);
      delete pieLabel;
    }

    delete pieLabels;
  }
}


void CGraph::PrintScatterSeries(HDDC hDC)
{
  //rightXTick and topYTick contain the outer bounds of the axis ticks.
  //So, if maxTick is 100 on both axis lines, then 100, 100 would be top
  //right.  We will use the bounds to see spacing from apex points and then
  //scale so any point can be plotted inside

  double yAxisScale, xAxisScale;
  int tickXLocation, tickYLocation;

  //multiply each value by 1.00 to force result into a double value, and therefore
  //make it more accurate in it's plot location.
  yAxisScale = ((p_topYTick - yApexPoint) / (maxTick * 1.00));
  xAxisScale = ((p_rightXTick - xApexPoint) / (maxTick * 1.00));

  //points will now plot as ((value * scale) + apex point)
  COLORREF barColor;
  barColor = BLACK;






  for (pos != NULL; graphSeries->GetNext(pos)) {
    tmpSeries = (CGraphSeries*)graphSeries->GetAt(pos);

    //multiply each value by 1.00 to force result into a double value, and therefore
    //make it more accurate in it's plot location.
    tickXLocation = (int)((tmpSeries->GetXDataValue() * 1.00) * xAxisScale) + xApexPoint;
    tickYLocation = yApexPoint + (int)((tmpSeries->GetYDataValue() * 1.00) * yAxisScale);

    //draw ellipse...
    hDC->Ellipse(tickXLocation - 60, tickYLocation - 60,
        tickXLocation + 60, tickYLocation + 60);

  }


}

void CGraph::PrintBoxWhiskerSeries(HDDC hDC)
{
  int upperQuartile;
  int lowerQuartile;
  int median;
  int highValue;
  int lowValue;

  COLORREF barColor;
  barColor = BLACK;




  int barWidth;
  int barL, barT, barR, barB;
  int tickXLocation, tickYLocation;
  int seriesSpace;
  int barHeight;


  CUIntArray dataArray;
  CUIntArray sortedArray;

  for (int x = 1; x <= ngraphSeries; x++) {
    dataArray.RemoveAll();
    sortedArray.RemoveAll();

    CObList* sortedData;
    sortedData = new CObList();

    seriesSize = tmpSeries->GetDataCount();

    int dataValue;

    for (int s = 0; s < seriesSize; s++) {
      dataValue = 0;

      if (data[x * seriesSize + s] > -1) {
        dataValue = data[x * seriesSize + s];
      }

      dataArray.Add(dataValue);
    }

    //sort the array
    dataValue = dataArray.GetAt(0);
    sortedArray.Add(dataValue);

    for (int d = 1; d < dataArray.GetSize(); d++) {
      dataValue = dataArray.GetAt(d);
      BOOL placed = FALSE;

      for (int s = 0; s < sortedArray.GetSize(); s++) {
        int tmpI = sortedArray.GetAt(s);

        if (dataValue <= tmpI) {
          sortedArray.InsertAt(s, dataValue);
          placed = TRUE;
          break;
        }
      }

      if (!placed) { //add at end
        sortedArray.Add(dataValue);
      }
    }

    //sortedArray now contains the sorted list of all data in this
    //series.  From here, we derive the other box-whisker data

    int medialElement;
    div_t div_result;
    div_result = div(sortedArray.GetSize(), 2);

    if (div_result.rem == 0) { //even number of elements
      //size is not 0 base, so below, I subtract 1 to 0 base it.
      medialElement = (sortedArray.GetSize() / 2) - 1;
      median = sortedArray.GetAt(medialElement) +
          ((sortedArray.GetAt(medialElement + 1) -
              sortedArray.GetAt(medialElement)) / 2);
      highValue = sortedArray.GetAt(sortedArray.GetSize() - 1);
      lowValue = sortedArray.GetAt(0);
      div_t div2Result;
      div2Result = div(medialElement + 1, 2);

      if (div2Result.rem == 0) { //even again
        upperQuartile = sortedArray.GetAt(medialElement + 1 + (medialElement / 2)) +
            (sortedArray.GetAt(medialElement + (medialElement / 2) + 2) -
                sortedArray.GetAt(medialElement + (medialElement / 2))) / 2;
        lowerQuartile = sortedArray.GetAt(medialElement / 2) +
            ((sortedArray.GetAt((medialElement / 2) + 1) -
                sortedArray.GetAt(medialElement / 2)) / 2);
      }
      else { //odd
        upperQuartile = sortedArray.GetAt(medialElement + 1 + (medialElement / 2));
        lowerQuartile = sortedArray.GetAt(medialElement / 2);
      }
    }
    else { //odd number of elements
      //size is not 0 base, so below, I subtract 1 to 0 base it.
      medialElement = sortedArray.GetSize() / 2;
      median = sortedArray.GetAt(medialElement);
      highValue = sortedArray.GetAt(sortedArray.GetSize() - 1);
      lowValue = sortedArray.GetAt(0);
      div_t div2Result;
      div2Result = div(medialElement, 2);

      if (div2Result.rem == 0) { //even
        upperQuartile = sortedArray.GetAt(medialElement + (medialElement / 2)) +
            (sortedArray.GetAt(medialElement + (medialElement / 2) + 1) -
                sortedArray.GetAt(medialElement + (medialElement / 2))) / 2;
        lowerQuartile = sortedArray.GetAt((medialElement - 1) / 2) +
            ((sortedArray.GetAt((medialElement + 1) / 2) -
                sortedArray.GetAt((medialElement - 1) / 2)) / 2);
      }
      else { //odd
        upperQuartile = sortedArray.GetAt(medialElement + 1 + (medialElement / 2));
        lowerQuartile = sortedArray.GetAt(medialElement / 2);
      }
    }

    //data has been computed for median, high, low, and interquartile range
    //now we can draw the series

    if (graphAlignment) {
      seriesSpace = xAxisWidth / ngraphSeries;

      barWidth = (int)(seriesSpace * .8);

      double tickScale = 0.00;
      tickScale = (yAxisHeight - 100) / numTicks;  //this must be consistent with PrintAxis()

      tickXLocation = xApexPoint + ((x * seriesSpace) - (seriesSpace / 2));

      barL = tickXLocation - (barWidth / 2);
      barR = barL + barWidth;
      double dataScale = 0.00;
      dataScale = ((yAxisHeight - 60) * 1.00) / (maxTick - minTick);

      //top cross bar (max value)
      barHeight = (int)((highValue * 1.00) * tickScale);
      barT = yApexPoint + barHeight;
      barB = barT - 60;
      draw_rect(hDC, barL, barT, barR, barB, barColor);

      //bottom cross bar (min value)
      barHeight = (int)((lowValue * 1.00) * tickScale);
      barT = yApexPoint + barHeight;
      barB = barT + 60;
      draw_rect(hDC, barL, barT, barR, barB, barColor);

      //vertical line (whisker)
      barHeight = (int)((highValue * 1.00) * tickScale);
      barT = yApexPoint + barHeight;
      draw_rect(hDC, tickXLocation - 40, barT, tickXLocation + 40, barB, barColor);

      //box (interquartile range, from upper quartile to lower quartile)
      barHeight = (int)(upperQuartile * tickScale);
      barT = yApexPoint + barHeight;
      barHeight = (int)(lowerQuartile * tickScale);
      barB = yApexPoint + barHeight;
      draw_rect(hDC, barL, barT, barR, barB, barColor);

      //draw median line (in RED)

      COLORREF linePen(RED);
      _clr = (linePen);
      barHeight = (int)((median * 1.00) * tickScale);
      TMoveTo(hDC, barL, yApexPoint + barHeight);
      _LineTo(hDC, barR, yApexPoint + barHeight);

    }
    else {
      seriesSpace = yAxisHeight / ngraphSeries;

      barHeight = (int)(seriesSpace * .8);

      double tickScale = 0.00;
      tickScale = (xAxisWidth - 200) / numTicks;  //same as PrintAxis()

      tickYLocation = yApexPoint + ((x * seriesSpace) - (seriesSpace / 2));

      barT = tickYLocation + (barHeight / 2);
      barB = barT - barHeight;
      double dataScale = 0.00;
      dataScale = ((xAxisWidth - 60) * 1.00) / (maxTick - minTick);

      //top cross bar (max value)
      barWidth = (int)((highValue * 1.00) * tickScale);
      barR = xApexPoint + barWidth;
      barL = barR - 60;
      draw_rect(hDC, barL, barT, barR, barB, barColor);

      //bottom cross bar (min value)
      barWidth = (int)((lowValue * 1.00) * tickScale);
      barR = xApexPoint + barWidth;
      barL = barR + 60;
      draw_rect(hDC, barL, barT, barR, barB, barColor);

      //vertical line (whisker)
      barWidth = (int)((highValue * 1.00) * tickScale);
      barR = xApexPoint + barWidth;
      draw_rect(hDC, barL, tickYLocation + 40, barR, tickYLocation - 40, barColor);

      //box (interquartile range, from upper quartile to lower quartile)
      barWidth = (int)(upperQuartile * tickScale);
      barL = xApexPoint + barWidth;
      barWidth = (int)(lowerQuartile * tickScale);
      barR = xApexPoint + barWidth;
      draw_rect(hDC, barL, barT, barR, barB, barColor);

      //draw median line (in RED)

      COLORREF linePen(RED);
      _clr = (linePen);
      barWidth = (int)((median * 1.00) * tickScale);
      TMoveTo(hDC, xApexPoint + barWidth, barT);
      _LineTo(hDC, xApexPoint + barWidth, barB);

    }
  }


}

void CGraph::PrintStackedBarSeries(HDDC hDC)
{
  int barWidth;
  int barL, barT, barR, barB;
  int tickXLocation, tickYLocation;
  int seriesSpace;
  int barHeight;  //for scalability



  if (graphAlignment) {
    //determine width of barchart data blocks
    seriesSpace = xAxisWidth / ngraphSeries;
    barWidth = (int)(seriesSpace * .6);



    double dataScale = 0.00;
    dataScale = ((yAxisHeight - 100) * 1.00) / (maxTick - minTick);

    for (int x = 1; x <= ngraphSeries; x++) {


      tickXLocation = xApexPoint + ((x * seriesSpace) - (seriesSpace / 2));

      barT = yApexPoint;  //this is a running total;

      //it starts at yApexPoint so barB can
      //start there, then moves up by barHeight
      for (int s = 0; s < seriesSize; s++) {
        if (data[x * seriesSize + s] > -1) {
          barHeight = (int)(((data[x * seriesSize + s] - minTick) * 1.00) * dataScale);

          if (((data[x * seriesSize + s] - minTick) > 0) && (barHeight < 1)) {
            barHeight = (int)dataScale;
          }

          barB = barT;
          barL = tickXLocation - (barWidth / 2);

          if (barHeight > yAxisHeight) {
            barT = barB + yAxisHeight;
          }
          else {
            barT = barB + barHeight;
          }

          barR = barL + barWidth;

          COLORREF barColor;
          barColor = graphLegend[s].color;



          draw_rect(hDC, barL, barT, barR, barB, barColor);

        }
      }
    }
  }
  else {
    //determine width of barchart data blocks
    seriesSpace = yAxisHeight / ngraphSeries;
    barHeight = (int)(seriesSpace * .6);



    double dataScale = 0.00;
    dataScale = ((xAxisWidth - 100) * 1.00) / (maxTick - minTick);

    for (int y = 0; y < ngraphSeries; y++) {


      tickYLocation = yApexPoint + ((y * seriesSpace) - (seriesSpace / 2));

      barR = xApexPoint;  //this is a running total;

      //it starts at xApexPoint so barL can
      //start there, then moves right by barWidth
      for (int s = 0; s < seriesSize; s++) {
        if (data[x * seriesSize + s] > -1) {

          barWidth = (int)(((data[x * seriesSize + s] - minTick) * 1.00) * dataScale);

          if (((data[x * seriesSize + s] - minTick) > 0) && (barWidth < 1)) {
            barWidth = (int)dataScale;
          }

          barL = barR;
          barT = tickYLocation + (barHeight / 2);
          barB = barT - barHeight;

          if (barWidth > xAxisWidth) {
            barR = barL + xAxisWidth;
          }
          else {
            barR = barL + barWidth;
          }

          COLORREF barColor;
          barColor = graphLegend[s].color;



          draw_rect(hDC, barL, barT, barR, barB, barColor);

        }
      }
    }
  }
}

void CGraph::PrintXYLineSeries(HDDC hDC)
{
  //same as Line graph, but without dots at the values
  int barWidth;
  int dataPlotSize;   //used to plot rects of data
  int tickXLocation, tickYLocation;
  int seriesSpace;
  int barHeight;  //for scalability



  int lastXLoc, lastYLoc;

  if (graphAlignment) {
    for (int s = 0; s < seriesSize; s++) {
      //determine width of barchart data blocks
      seriesSpace = xAxisWidth / ngraphSeries;
      barWidth = (int)((seriesSpace * .6) / seriesSize);
      dataPlotSize = seriesSize * barWidth;



      for (int x = 1; x <= ngraphSeries; x++) {


        if (data[x * seriesSize + s] > -1) {
          tickXLocation = xApexPoint + ((x * seriesSpace) - (seriesSpace / 2));

          barHeight = 0;
          double dataScale = 0.00;

          if ((maxTick - minTick) != yAxisHeight) {
            dataScale = ((yAxisHeight * 1.00) / ((maxTick - minTick) * 1.00)) * tickSpace;
          }
          else {
            dataScale = tickSpace * 15.00;
          }

          int axisHeight = (int)(dataScale * numTicks);
          double barPercent = 0.00;
          barPercent = ((data[x * seriesSize + s] - minTick) * 1.00) / maxTick;
          barHeight = (int)(barPercent * axisHeight);

          if (((data[x * seriesSize + s] - minTick) > 0) && (barHeight < 1)) {
            barHeight = (int)dataScale;
          }

          int yDataLocation = yApexPoint + barHeight;

          //now have x and y location of center of ellipse
          COLORREF barColor;
          barColor = graphLegend[s].color;



          //draw line back to last data member
          if (x > 1) {

            COLORREF linePen(PS_SOLID, 40, barColor);
            _clr = (linePen);
            TMoveTo(hDC, lastXLoc, lastYLoc);
            _LineTo(hDC, tickXLocation, yDataLocation);

          }

          lastXLoc = tickXLocation;
          lastYLoc = yDataLocation;

        }
      }
    }
  }
  else {
    for (int s = 0; s < seriesSize; s++) {
      //determine width of barchart data blocks
      seriesSpace = yAxisHeight / ngraphSeries;
      barHeight = (int)((seriesSpace * .6) / seriesSize);
      dataPlotSize = seriesSize * barHeight;



      for (int x = 1; x <= ngraphSeries; x++) {


        if (data[x * seriesSize + s] > -1) {
          tickYLocation = yApexPoint + ((x * seriesSpace) - (seriesSpace / 2));

          barWidth = 0;
          double dataScale = 0.00;

          dataScale = ((xAxisWidth - 100) * 1.00) / (maxTick - minTick);

          int axisWidth = (int)(dataScale * (maxTick - minTick));
          double barPercent = 0.00;
          barPercent = ((data[x * seriesSize + s] - minTick) * 1.00) / ((maxTick - minTick) * 1.00);
          barWidth = (int)(barPercent * axisWidth);

          if (((data[x * seriesSize + s] - minTick) > 0) && (barHeight < 1)) {
            barWidth = (int)dataScale;
          }

          int xDataLocation = xApexPoint + barWidth;

          //now have x and y location of center of ellipse
          COLORREF barColor;
          barColor = graphLegend[s].color;



          //draw line back to last data member
          if (x > 1) {

            COLORREF linePen(PS_SOLID, 40, barColor);
            _clr = (linePen);
            TMoveTo(hDC, lastXLoc, lastYLoc);
            _LineTo(hDC, xDataLocation, tickYLocation);

          }

          //now draw ellipse...
          lastYLoc = tickYLocation;
          lastXLoc = xDataLocation;

        }
      }
    }
  }
}

//void CGraph::PrintRadarSeries(HDDC hDC)
//{
//}

void CGraph::Print3DBarSeries(HDDC hDC)
{
  int barWidth;
  int dataPlotSize;   //used to plot rects of data
  int barL, barT, barR, barB;
  int tickXLocation, tickYLocation;
  int seriesSpace;
  int barHeight;


  int blue;
  int red;
  int green;

  if (graphAlignment) {
    //determine width of barchart data blocks
    seriesSpace = (xAxisWidth - 100) / ngraphSeries;
    barWidth = (int)((seriesSpace * .8) / seriesSize);
    dataPlotSize = seriesSize * barWidth;



    for (int x = 1; x <= ngraphSeries; x++) {


      tickXLocation = xApexPoint + ((x * seriesSpace) - (seriesSpace / 2));

      for (int s = 0; s < seriesSize; s++) {
        if (data[x * seriesSize + s] > -1) {
          double dataScale = 0.00;
          dataScale = (yAxisHeight * 1.00) / (maxTick - minTick);
          int dScale;
          dScale = (int)dataScale;
          barHeight = (int)(((data[x * seriesSize + s] - minTick) * 1.00) * dScale);

          if (((data[x * seriesSize + s] - minTick) > 0) && (barHeight < 1)) {
            barHeight = 1 * dScale;
          }

          barL = tickXLocation - (dataPlotSize / 2) + (s * barWidth);

          if (barHeight > yAxisHeight) {
            barT = yApexPoint + yAxisHeight;
          }
          else {
            barT = yApexPoint + barHeight;
          }

          barR = barL + barWidth;
          barB = yApexPoint;

          COLORREF barColor;
          barColor = graphLegend[s].color;

          COLORREF rectPen(barColor);

          _clr = (rectPen);


          draw_rect(hDC, barL, barT, barR, barB, barColor);

          //now, we do the side
          //side is darker than front, so subtract
          //from color to make closer to black
          red = GetRV(barColor);
          green = GetGV(barColor);
          blue = GetBV(barColor);
          int sideRed = red - 35;
          int sideGreen = green - 35;
          int sideBlue = blue - 35;

          if (sideRed < 0) {
            sideRed = 0;
          }

          if (sideGreen < 0) {
            sideGreen = 0;
          }

          if (sideBlue < 0) {
            sideBlue = 0;
          }

          COLORREF sideColor;
          sideColor = _RGB(sideRed, sideGreen, sideBlue);
          COLORREF sideBrush(sideColor);
          COLORREF sidePen(sideColor);
          TSelectFont(sideBrush);
          TSelectFont(sidePen);
          POINT sidePolyArray[4];
          POINT sp1(barR, barT);
          POINT sp2(barR, barB);
          POINT sp3(barR + depth, barB + depth);
          POINT sp4(barR + depth, barT + depth);
          sidePolyArray[0] = sp1;
          sidePolyArray[1] = sp2;
          sidePolyArray[2] = sp3;
          sidePolyArray[3] = sp4;

          hDC->Polygon(sidePolyArray, 4);

          //finally, the top
          int topRed = red + 35;
          int topGreen = green + 35;
          int topBlue = blue + 35;

          if (topRed > 255) {
            topRed = 255;
          }

          if (topGreen > 255) {
            topGreen = 255;
          }

          if (topBlue > 255) {
            topBlue = 255;
          }

          COLORREF topColor;
          topColor = _RGB(topRed, topGreen, topBlue);
          COLORREF topBrush(topColor);
          COLORREF topPen(topColor);
          TSelectFont(topBrush);
          TSelectFont(topPen);
          POINT topPolyArray[4];
          POINT tp1(barL, barT);
          POINT tp2(barR, barT);
          POINT tp3(barR + depth, barT + depth);
          POINT tp4(barL + depth, barT + depth);
          topPolyArray[0] = tp1;
          topPolyArray[1] = tp2;
          topPolyArray[2] = tp3;
          topPolyArray[3] = tp4;

          hDC->Polygon(topPolyArray, 4);



        }
      }
    }
  }
  else {
    //determine width of barchart data blocks
    seriesSpace = yAxisHeight / ngraphSeries;
    barHeight = (int)((seriesSpace * .8) / seriesSize);
    dataPlotSize = seriesSize * barHeight;



    for (int y = 0; y < ngraphSeries; y++) {


      tickYLocation = yApexPoint + ((y * seriesSpace) - (seriesSpace / 2));

      for (int s = 0; s < seriesSize; s++) {
        if (data[x * seriesSize + s] > -1) {
          double dataScale = 0.00;
          dataScale = ((xAxisWidth - 200) * 1.00) / (maxTick - minTick);
          barWidth = (int)(((data[x * seriesSize + s] - minTick) * 1.00) * dataScale);

          if (((data[x * seriesSize + s] - minTick) > 0) && (barWidth < 1)) {
            barWidth = (int)dataScale;
          }

          barL = xApexPoint;
          barB = tickYLocation - (dataPlotSize / 2) + (s * barHeight);
          barT = barB + barHeight;

          if (barWidth > xAxisWidth) {
            barR = xApexPoint + xAxisWidth;
          }
          else {
            barR = xApexPoint + barWidth;
          }

          COLORREF barColor;
          barColor = graphLegend[s].color;



          COLORREF rectPen(barColor);

          _clr = (rectPen);
          draw_rect(hDC, barL, barT, barR, barB, barColor);

          //now, we do the side
          //side is darker than front, so subtract
          //from color to make closer to black
          red = GetRV(barColor);
          green = GetGV(barColor);
          blue = GetBV(barColor);
          int sideRed = red - 35;
          int sideGreen = green - 35;
          int sideBlue = blue - 35;

          if (sideRed < 0) {
            sideRed = 0;
          }

          if (sideGreen < 0) {
            sideGreen = 0;
          }

          if (sideBlue < 0) {
            sideBlue = 0;
          }

          COLORREF sideColor;
          sideColor = _RGB(sideRed, sideGreen, sideBlue);
          COLORREF sideBrush(sideColor);
          COLORREF sidePen(sideColor);
          TSelectFont(sideBrush);
          TSelectFont(sidePen);
          POINT sidePolyArray[4];
          POINT sp1(barL, barT);
          POINT sp2(barR, barT);
          //I'm subtracting below to shorten the width of the
          //bar, since it was showing farther right than actual value
          POINT sp3(barR + depth - (seriesSize - s), barT + depth);
          POINT sp4(barL + depth - (seriesSize - s), barT + depth);
          sidePolyArray[0] = sp1;
          sidePolyArray[1] = sp2;
          sidePolyArray[2] = sp3;
          sidePolyArray[3] = sp4;

          hDC->Polygon(sidePolyArray, 4);

          //finally, the top
          int topRed = red + 35;
          int topGreen = green + 35;
          int topBlue = blue + 35;

          if (topRed > 255) {
            topRed = 255;
          }

          if (topGreen > 255) {
            topGreen = 255;
          }

          if (topBlue > 255) {
            topBlue = 255;
          }

          COLORREF topColor;
          topColor = _RGB(topRed, topGreen, topBlue);
          COLORREF topBrush(topColor);
          COLORREF topPen(topColor);
          TSelectFont(topBrush);
          TSelectFont(topPen);
          POINT topPolyArray[4];
          POINT tp1(barR, barT);
          POINT tp2(barR, barB);
          //I'm subtracting below to shorten the width of the
          //bar, since it was showing farther right than actual value
          POINT tp3(barR + depth - (seriesSize - s), barB + depth);
          POINT tp4(barR + depth - (seriesSize - s), barT + depth);
          topPolyArray[0] = tp1;
          topPolyArray[1] = tp2;
          topPolyArray[2] = tp3;
          topPolyArray[3] = tp4;

          hDC->Polygon(topPolyArray, 4);



        }
      }
    }
  }
}

void CGraph::Print3DLineSeries(HDDC hDC)
{
  int barWidth;
  int dataPlotSize;   //used to plot rects of data
  int tickXLocation, tickYLocation;
  int seriesSpace;
  int barHeight;


  int thick;
  int blue;
  int red;
  int green;



  thick = depth / 5;

  int lastXLoc, lastYLoc;

  if (graphAlignment) {
    for (int s = 0; s < seriesSize; s++) {
      //determine width of barchart data blocks
      seriesSpace = (xAxisWidth - 100) / ngraphSeries;


      int yDataLocation;

      for (int x = 1; x <= ngraphSeries; x++) {
        tickXLocation = xApexPoint + ((x * seriesSpace) - (seriesSpace / 2));


        if (data[x * seriesSize + s] > -1) {
          double dataScale = 0.00;
          dataScale = (yAxisHeight * 1.00) / (maxTick - minTick);
          int dScale;
          dScale = (int)dataScale;
          barHeight = (int)(((data[x * seriesSize + s] - minTick) * 1.00) * dScale);

          if (((data[x * seriesSize + s] - minTick) > 0) && (barHeight < 1)) {
            barHeight = 1 * dScale;
          }

          yDataLocation = yApexPoint + barHeight;
          //now have x and y location of center of ellipse
          COLORREF barColor;
          barColor = graphLegend[s].color;
          red = GetRV(barColor);
          green = GetGV(barColor);
          blue = GetBV(barColor);


          //draw line back to last data member
          if (x > 1) {
            COLORREF linePen(barColor);
            _clr = (linePen);
            //front side of line
            POINT sidePolyArray[4];
            POINT sp1(lastXLoc, lastYLoc);
            POINT sp2(lastXLoc, lastYLoc - thick);
            POINT sp3(tickXLocation, yDataLocation - thick);
            POINT sp4(tickXLocation, yDataLocation);
            sidePolyArray[0] = sp1;
            sidePolyArray[1] = sp2;
            sidePolyArray[2] = sp3;
            sidePolyArray[3] = sp4;
            hDC->Polygon(sidePolyArray, 4);

            //top part of line, lighter than front
            int topRed = red + 35;
            int topGreen = green + 35;
            int topBlue = blue + 35;

            if (topRed > 255) {
              topRed = 255;
            }

            if (topGreen > 255) {
              topGreen = 255;
            }

            if (topBlue > 255) {
              topBlue = 255;
            }

            COLORREF topColor;
            topColor = _RGB(topRed, topGreen, topBlue);

            COLORREF topPen(topColor);
            COLORREF topBrush(topColor);
            TSelectFont(topPen);
            TSelectFont(topBrush);
            POINT topPolyArray[4];
            POINT tp1(lastXLoc, lastYLoc);
            POINT tp2(lastXLoc + depth, lastYLoc + depth);
            POINT tp3(tickXLocation + depth, yDataLocation + depth);
            POINT tp4(tickXLocation, yDataLocation);
            topPolyArray[0] = tp1;
            topPolyArray[1] = tp2;
            topPolyArray[2] = tp3;
            topPolyArray[3] = tp4;
            hDC->Polygon(topPolyArray, 4);



          }

          lastXLoc = tickXLocation;
          lastYLoc = yDataLocation;
        }
      }

      if (x > 1) {
        //end point of line
        int endRed = red - 35;
        int endGreen = green - 35;
        int endBlue = blue - 35;

        if (endRed < 0) {
          endRed = 0;
        }

        if (endGreen < 0) {
          endGreen = 0;
        }

        if (endBlue < 0) {
          endBlue = 0;
        }

        COLORREF endColor;
        endColor = _RGB(endRed, endGreen, endBlue);

        COLORREF endPen(endColor);
        COLORREF endBrush(endColor);
        TSelectFont(endPen);
        TSelectFont(endBrush);
        POINT endPolyArray[4];
        POINT ep1(tickXLocation, yDataLocation);
        POINT ep2(tickXLocation + depth, yDataLocation + depth);
        POINT ep3(tickXLocation + depth, yDataLocation + depth - thick);
        POINT ep4(tickXLocation, yDataLocation + thick);
        endPolyArray[0] = ep1;
        endPolyArray[1] = ep2;
        endPolyArray[2] = ep3;
        endPolyArray[3] = ep4;
        hDC->Polygon(endPolyArray, 4);



      }
    }
  }
  else {
    for (int s = 0; s < seriesSize; s++) {
      //determine width of barchart data blocks
      seriesSpace = yAxisHeight / ngraphSeries;
      barHeight = (int)((seriesSpace * .6) / seriesSize);
      dataPlotSize = seriesSize * barHeight;


      int xDataLocation;

      for (int x = 1; x <= ngraphSeries; x++) {
        tickYLocation = yApexPoint + ((x * seriesSpace) - (seriesSpace / 2));


        if (data[x * seriesSize + s] > -1) {
          double dataScale = 0.00;
          dataScale = ((xAxisWidth - 200) * 1.00) / (maxTick - minTick);
          barWidth = (int)(((data[x * seriesSize + s] - minTick) * 1.00) * dataScale);

          if (((data[x * seriesSize + s] - minTick) > 0) && (barWidth < 1)) {
            barWidth = (int)dataScale;
          }

          xDataLocation = xApexPoint + barWidth;

          //now have x and y location of center of ellipse
          COLORREF barColor;
          barColor = graphLegend[s].color;
          red = GetRV(barColor);
          green = GetGV(barColor);
          blue = GetBV(barColor);


          //draw line back to last data member
          if (x > 1) {
            COLORREF linePen(barColor);
            _clr = (linePen);
            //front side of line
            POINT sidePolyArray[4];
            POINT sp1(lastXLoc, lastYLoc);
            POINT sp2(lastXLoc - thick, lastYLoc);
            POINT sp3(xDataLocation - thick, tickYLocation);
            POINT sp4(xDataLocation, tickYLocation);
            sidePolyArray[0] = sp1;
            sidePolyArray[1] = sp2;
            sidePolyArray[2] = sp3;
            sidePolyArray[3] = sp4;
            hDC->Polygon(sidePolyArray, 4);

            //top part of line, lighter than front
            int topRed = red + 35;
            int topGreen = green + 35;
            int topBlue = blue + 35;

            if (topRed > 255) {
              topRed = 255;
            }

            if (topGreen > 255) {
              topGreen = 255;
            }

            if (topBlue > 255) {
              topBlue = 255;
            }

            COLORREF topColor;
            topColor = _RGB(topRed, topGreen, topBlue);

            COLORREF topPen(topColor);
            COLORREF topBrush(topColor);
            TSelectFont(topPen);
            TSelectFont(topBrush);
            POINT topPolyArray[4];
            POINT tp1(lastXLoc, lastYLoc);
            POINT tp2(lastXLoc + depth, lastYLoc + thick);
            POINT tp3(xDataLocation + depth, tickYLocation + thick);
            POINT tp4(xDataLocation, tickYLocation);
            topPolyArray[0] = tp1;
            topPolyArray[1] = tp2;
            topPolyArray[2] = tp3;
            topPolyArray[3] = tp4;
            hDC->Polygon(topPolyArray, 4);



          }

          lastYLoc = tickYLocation;
          lastXLoc = xDataLocation;
        }
      }

      if (x > 1) {
        //end point of line
        int endRed = red - 35;
        int endGreen = green - 35;
        int endBlue = blue - 35;

        if (endRed < 0) {
          endRed = 0;
        }

        if (endGreen < 0) {
          endGreen = 0;
        }

        if (endBlue < 0) {
          endBlue = 0;
        }

        COLORREF endColor;
        endColor = _RGB(endRed, endGreen, endBlue);

        COLORREF endPen(endColor);
        COLORREF endBrush(endColor);
        TSelectFont(endPen);
        TSelectFont(endBrush);
        POINT endPolyArray[4];
        POINT ep1(xDataLocation, tickYLocation);
        POINT ep2(xDataLocation + depth, tickYLocation + thick);
        POINT ep3(xDataLocation + depth - thick, tickYLocation + thick);
        POINT ep4(xDataLocation - thick, tickYLocation);
        endPolyArray[0] = ep1;
        endPolyArray[1] = ep2;
        endPolyArray[2] = ep3;
        endPolyArray[3] = ep4;
        hDC->Polygon(endPolyArray, 4);



      }
    }
  }
}

void CGraph::Print3DPieSeries(HDDC hDC)
{
  double dataSum = 0.00;  //for storing cumulative sum
  double labelData = 0.00;
  int lastXLocation, lastYLocation;
  int newXLocation, newYLocation;
  int labelXLocation, labelYLocation;
  double percent = 0.00;
  double labelPercent = 0.00;
  int degrees;
  int labelDegrees;
  double totalSum = 0.00;
  int radius;


  int seriesSpace;
  int labelLineXStart, labelLineYStart;
  int labelLineXEnd, labelLineYEnd;
  int maxLabelWidth;
  int maxLabelHeight;
  TEXTMETRIC tm;



  COLORREF tmpPen(BLACK);
  COLORREF tmpBrush(WHITE);
  _clr = (tmpPen);
  pOldBrush = TSelectFont(tmpBrush);

  //deltaX and deltaY will be based on distance from x and y
  //axis for the new endpoint of the pie.  These values can
  //then be used to find the true distance between starting
  //line and ending line of pie boundary.
  double deltaX, deltaY;
  double degreeRadians, degreeRadians2;
  double labelDeltaX, labelDeltaY;
  double labelDegreeRadians, labelDegreeRadians2;

  lastXLocation = 0;
  lastYLocation = 0;

  tm = GetCurFont()->tm;
  maxLabelWidth = tm.tmMaxCharWidth + 10;
  maxLabelHeight = tm.tmHeight + 6;

  //pie labels will be stored in a list and drawn after entire pie
  //is completed.
  CObList* pieLabels;
  CGraphPieLabel* pieLabel;

  //determine width of pie display area
  if (xAxisWidth > yAxisHeight) {
    seriesSpace = yAxisHeight / ngraphSeries;
  }
  else {
    seriesSpace = xAxisWidth / ngraphSeries;
  }

  seriesSpace -= (3 * maxLabelWidth);  //allows text like 25%  (3 chars)

  //to plot a pie plus labels inside of series space, use the following :
  //(3 * radius) + (2 * label width) = series space
  //so, 3*radius = series space - (2 * label width)
  // 1 radius = (series space - (2 * label width)) / 3
  radius = seriesSpace / 3;  //pie needs 2 radius, + 1 extra for line to labels = 3 for my divisor
  int depth = (int)(radius * depthRatio);  //for shadow pie

  int centerYPie = pGraphT - (yAxisHeight / 2) - 500 - depth;


  for (int x = 1; x <= ngraphSeries; x++) {


    pieLabels = new CObList();


    totalSum = 0;

    for (int s = 0; s < seriesSize; s++) {
      totalSum += data[x * seriesSize + s];
    }

    int pieLeft, pieRight;

    if (ngraphSeries == 1) {
      pieLeft = xApexPoint + (xAxisWidth / 2) - radius;
    }
    else {
      pieLeft = xApexPoint + 15 + ((x * 2 - 1) * ((xAxisWidth / ngraphSeries) / 2)) - radius;
    }

    pieRight = pieLeft + (2 * radius);
    RECT pieRect(pieLeft,
        centerYPie + radius,
        pieRight,
        centerYPie - radius);
    RECT shadowRect(pieLeft + depth,
        centerYPie + radius - depth,
        pieRight + depth,
        centerYPie - radius - depth);
    int centerXPie = pieLeft + radius;

    //plot series label
    TTextOut2(hDC, centerXPie - ((STRLEN(graphSeries[x]) / 2) * ((xTickFontSize / 2) * 20)),
        centerYPie - radius - maxLabelHeight - 600 - depth, graphSeries[x]);

    int centerShadowXPie;
    int centerShadowYPie;

    //draw shadow pie first
    centerShadowYPie = centerYPie - depth;
    centerShadowXPie = centerXPie + depth;
    lastXLocation = centerShadowXPie - radius;
    lastYLocation = centerShadowYPie;

    int lastTopX = centerXPie - radius;
    int lastTopY = centerYPie;
    int newTopX, newTopY;
    dataSum = 0;
    int lastQuad = 0;
    POINT lastSidePolyArray[4];
    COLORREF lastColor;
    POINT spa1;
    POINT spa2;
    POINT spa3;
    POINT spa4;

    for (s = 0; s < seriesSize; s++) {
      if (data[x * seriesSize + s] > 0) {
        int seriesDataValue;
        seriesDataValue = data[x * seriesSize + s];
        dataSum += seriesDataValue;
        percent = (dataSum / totalSum) * 100;
        degrees = (int)((360 * percent) / 100);

        //determine destination quadrant...
        //and set newXLocation and newYLocation values...
        //degrees / 90 will never exceed 4.
        //this can tell us the quadrant of destination
        int quadrant = degrees / 90;  //truncates decimal

        //using the law of sines to determine the deltas :
        //deltaX = radius * sin(90 - degrees)
        //deltaY = radius * sin(degrees)
        //we convert degrees into radians so sin() function works right
        //note :  in quad 1 and 3, we reverse the angle used to compute
        //      the deltas, since the triangle plots reversed.
        switch (quadrant) {
        case 0 : //this is the base quadrant, so no manipulation needed
          degreeRadians = degrees * (3.14159 / 180);
          degreeRadians2 = (90 - degrees) * (3.14159 / 180);
          deltaX = radius * sin(degreeRadians2);
          deltaY = radius * sin(degreeRadians);
          newXLocation = (int)(centerShadowXPie - deltaX);
          newYLocation = (int)(centerShadowYPie - deltaY);
          newTopX = (int)(centerXPie - deltaX);
          newTopY = (int)(centerYPie - deltaY);
          break;

        case 1 : //bottom right quadrant, subtract 90 from angle
          degreeRadians = (degrees - 90) * (3.14159 / 180);
          degreeRadians2 = (90 - (degrees - 90)) * (3.14159 / 180);
          deltaX = radius * sin(degreeRadians);
          deltaY = radius * sin(degreeRadians2);
          newXLocation = (int)(centerShadowXPie + deltaX);
          newYLocation = (int)(centerShadowYPie - deltaY);
          newTopX = (int)(centerXPie + deltaX);
          newTopY = (int)(centerYPie - deltaY);
          break;

        case 2 : //top right quadrant, subtract 180 from angle
          degreeRadians = (degrees - 180) * (3.14159 / 180);
          degreeRadians2 = (90 - (degrees - 180)) * (3.14159 / 180);
          deltaX = radius * sin(degreeRadians2);
          deltaY = radius * sin(degreeRadians);
          newXLocation = (int)(centerShadowXPie + deltaX);
          newYLocation = (int)(centerShadowYPie + deltaY);
          newTopX = (int)(centerXPie + deltaX);
          newTopY = (int)(centerYPie + deltaY);
          break;

        case 3 : //upper left quadrant, subtract 270 from angle
          degreeRadians = (degrees - 270) * (3.14159 / 180);
          degreeRadians2 = (90 - (degrees - 270)) * (3.14159 / 180);
          deltaX = radius * sin(degreeRadians);
          deltaY = radius * sin(degreeRadians2);
          newXLocation = (int)(centerShadowXPie - deltaX);
          newYLocation = (int)(centerShadowYPie + deltaY);
          newTopX = (int)(centerXPie - deltaX);
          newTopY = (int)(centerYPie + deltaY);
          break;

        case 4 : //straight line to left of center
          deltaX = radius;
          deltaY = 1;
          newXLocation = (int)(centerShadowXPie - deltaX);
          newYLocation = (int)(centerShadowYPie + deltaY);
          newTopX = (int)(centerXPie - deltaX);
          newTopY = (int)(centerYPie + deltaY);
          break;
        }

        if (s == 0) {
          lastYLocation += 1;
          lastTopY += 1;
        }

        COLORREF barColor;
        barColor = graphLegend[s].color;
        int red, green, blue;
        red = GetRV(barColor) - 35;
        green = GetGV(barColor) - 35;
        blue = GetBV(barColor) - 35;

        if (red < 0) {
          red = 0;
        }

        if (green < 0) {
          green = 0;
        }

        if (blue < 0) {
          blue = 0;
        }

        POINT sp1(lastXLocation, lastYLocation);
        POINT sp2(newXLocation, newYLocation);
        COLORREF shadowColor(_RGB(red, green, blue));
        COLORREF shadowPen(shadowColor);
        COLORREF shadowBrush(shadowColor);
        TSelectFont(shadowPen);
        TSelectFont(shadowBrush);

        if ((quadrant < 2) || (lastQuad < 2)) {
          POINT sidePolyArray[4];
          spa1.x = lastXLocation;
          spa1.y = lastYLocation;
          spa2.x = newXLocation;
          spa2.y = newYLocation;
          spa3.x = newTopX;
          spa3.y = newTopY;
          spa4.x = lastTopX;
          spa4.y = lastTopY;
          sidePolyArray[0] = spa1;
          sidePolyArray[1] = spa2;
          sidePolyArray[2] = spa3;
          sidePolyArray[3] = spa4;
          hDC->Polygon(sidePolyArray, 4);
          hDC->Pie(shadowRect, sp1, sp2);
        }
        else {
          POINT sidePolyArray[3];
          spa1.x = newXLocation;
          spa1.y = newYLocation;
          spa2.x = newTopX;
          spa2.y = newTopY;
          spa3.x = lastTopX;
          spa3.y = lastTopY;
          sidePolyArray[0] = spa1;
          sidePolyArray[1] = spa2;
          sidePolyArray[2] = spa3;
          COLORREF oldShadowPen(lastColor);
          TSelectFont(oldShadowPen);
          COLORREF oldShadowBrush(lastColor);
          TSelectFont(oldShadowBrush);
          hDC->Pie(shadowRect, sp1, sp2);
          hDC->Polygon(sidePolyArray, 3);
          TSelectFont(shadowPen);
          TSelectFont(shadowBrush);
        }

        lastXLocation = newXLocation;
        lastYLocation = newYLocation;
        lastTopX = newTopX;
        lastTopY = newTopY;

        if ((quadrant > 1) && (lastQuad < 2)) {
          lastSidePolyArray[0] = spa1;
          lastSidePolyArray[1] = spa2;
          lastSidePolyArray[2] = spa3;
          lastSidePolyArray[3] = spa4;
          lastColor = shadowColor;
        }

        lastQuad = quadrant;


      }
    }

    //draw normal pie
    lastXLocation = pieLeft;
    lastYLocation = centerYPie;

    dataSum = 0;

    for (s = 0; s < seriesSize; s++) {
      if (data[x * seriesSize + s] > 0) {
        int seriesDataValue;
        seriesDataValue = data[x * seriesSize + s];
        labelData = seriesDataValue / 2;
        dataSum += seriesDataValue;
        percent = (dataSum / totalSum) * 100;
        labelPercent = ((dataSum - labelData) / totalSum) * 100;
        degrees = (int)((360 * percent) / 100);
        labelDegrees = (int)((360 * labelPercent) / 100);

        //determine destination quadrant...
        //and set newXLocation and newYLocation values...
        //degress / 90 will never exceed 4.
        //this can tell us the quadrant of destination
        int quadrant = degrees / 90;  //truncates decimal
        int labelQuadrant = labelDegrees / 90;

        //using the law of sines to determine the deltas :
        //deltaX = radius * sin(90 - degrees)
        //deltaY = radius * sin(degrees)
        //we convert degrees into radians so sin() function works right
        //note :  in quad 1 and 3, we reverse the angle used to compute
        //      the deltas, since the triangle plots reversed.
        switch (quadrant) {
        case 0 : //this is the base quadrant, so no manipulation needed
          degreeRadians = degrees * (3.14159 / 180);
          degreeRadians2 = (90 - degrees) * (3.14159 / 180);
          deltaX = radius * sin(degreeRadians2);
          deltaY = radius * sin(degreeRadians);
          newXLocation = (int)(centerXPie - deltaX);
          newYLocation = (int)(centerYPie - deltaY);
          break;

        case 1 : //bottom right quadrant, subtract 90 from angle
          degreeRadians = (degrees - 90) * (3.14159 / 180);
          degreeRadians2 = (90 - (degrees - 90)) * (3.14159 / 180);
          deltaX = radius * sin(degreeRadians);
          deltaY = radius * sin(degreeRadians2);
          newXLocation = (int)(centerXPie + deltaX);
          newYLocation = (int)(centerYPie - deltaY);
          break;

        case 2 : //top right quadrant, subtract 180 from angle
          degreeRadians = (degrees - 180) * (3.14159 / 180);
          degreeRadians2 = (90 - (degrees - 180)) * (3.14159 / 180);
          deltaX = radius * sin(degreeRadians2);
          deltaY = radius * sin(degreeRadians);
          newXLocation = (int)(centerXPie + deltaX);
          newYLocation = (int)(centerYPie + deltaY);
          break;

        case 3 : //upper left quadrant, subtract 270 from angle
          degreeRadians = (degrees - 270) * (3.14159 / 180);
          degreeRadians2 = (90 - (degrees - 270)) * (3.14159 / 180);
          deltaX = radius * sin(degreeRadians);
          deltaY = radius * sin(degreeRadians2);
          newXLocation = (int)(centerXPie - deltaX);
          newYLocation = (int)(centerYPie + deltaY);
          break;

        case 4 : //straight line to left of center
          deltaX = radius;
          deltaY = 1;
          newXLocation = (int)(centerXPie - deltaX);
          newYLocation = (int)(centerYPie + deltaY);
          break;
        }

        switch (labelQuadrant) {
          //after getting X & Y location for label, we take
          //1/2 the delta between x y locations and center pie
        case 0 :
          labelDegreeRadians = labelDegrees * (3.14159 / 180);
          labelDegreeRadians2 = (90 - labelDegrees) * (3.14159 / 180);
          labelDeltaX = radius * sin(labelDegreeRadians2);
          labelDeltaY = radius * sin(labelDegreeRadians);
          labelXLocation = (int)(centerXPie - labelDeltaX);
          labelYLocation = (int)(centerYPie - labelDeltaY);
          labelLineXStart = labelXLocation + ((centerXPie - labelXLocation) / 2);
          labelLineYStart = labelYLocation - ((labelYLocation - centerYPie) / 2);
          labelLineXEnd = labelXLocation - (radius / 2);
          labelLineYEnd = labelYLocation - (radius / 2);
          break;

        case 1 :
          labelDegreeRadians = (labelDegrees - 90) * (3.14159 / 180);
          labelDegreeRadians2 = (90 - (labelDegrees - 90)) * (3.14159 / 180);
          labelDeltaX = radius * sin(labelDegreeRadians);
          labelDeltaY = radius * sin(labelDegreeRadians2);
          labelXLocation = (int)(centerXPie + labelDeltaX);
          labelYLocation = (int)(centerYPie - labelDeltaY);
          labelLineXStart = labelXLocation - ((labelXLocation - centerXPie) / 2);
          labelLineYStart = labelYLocation - ((labelYLocation - centerYPie) / 2);
          labelLineXEnd = labelXLocation + (radius / 2);
          labelLineYEnd = labelYLocation - (radius / 2);
          break;

        case 2 :
          labelDegreeRadians = (labelDegrees - 180) * (3.14159 / 180);
          labelDegreeRadians2 = (90 - (labelDegrees - 180)) * (3.14159 / 180);
          labelDeltaX = radius * sin(labelDegreeRadians2);
          labelDeltaY = radius * sin(labelDegreeRadians);
          labelXLocation = (int)(centerXPie + labelDeltaX);
          labelYLocation = (int)(centerYPie + labelDeltaY);
          labelLineXStart = labelXLocation - ((labelXLocation - centerXPie) / 2);
          labelLineYStart = labelYLocation - ((labelYLocation - centerYPie) / 2);
          labelLineXEnd = labelXLocation + (radius / 2);
          labelLineYEnd = labelYLocation + (radius / 2);
          break;

        case 3 :
          labelDegreeRadians = (labelDegrees - 270) * (3.14159 / 180);
          labelDegreeRadians2 = (90 - (labelDegrees - 270)) * (3.14159 / 180);
          labelDeltaX = radius * sin(labelDegreeRadians);
          labelDeltaY = radius * sin(labelDegreeRadians2);
          labelXLocation = (int)(centerXPie - labelDeltaX);
          labelYLocation = (int)(centerYPie + labelDeltaY);
          labelLineXStart = labelXLocation + ((centerXPie - labelXLocation) / 2);
          labelLineYStart = labelYLocation - ((labelYLocation - centerYPie) / 2);
          labelLineXEnd = labelXLocation - (radius / 2);
          labelLineYEnd = labelYLocation + (radius / 2);
          break;
          //there should never be a half point ending on 4, so leave it out
        }

        pieLabel = new CGraphPieLabel();

        pieLabel->lineXStart = labelLineXStart;
        pieLabel->lineYStart = labelLineYStart;
        pieLabel->lineXEnd = labelLineXEnd;
        pieLabel->lineYEnd = labelLineYEnd;

        switch (labelQuadrant) {
        case 0 : //label to left of line
          pieLabel->topLeftX = labelLineXEnd - maxLabelWidth;
          pieLabel->topLeftY = labelLineYEnd;
          break;

        case 1 : //label to right of line
          pieLabel->topLeftX = labelLineXEnd + 50;
          pieLabel->topLeftY = labelLineYEnd;
          break;

        case 2 : //label to right of line
          pieLabel->topLeftX = labelLineXEnd + 50;
          pieLabel->topLeftY = labelLineYEnd + maxLabelHeight;
          break;

        case 3 : //label to left of line
          pieLabel->topLeftX = labelLineXEnd - maxLabelWidth;
          pieLabel->topLeftY = labelLineYEnd + maxLabelHeight;
          break;
        }

        pieLabel->labelQuadrant = labelQuadrant;
        int roundPercent;
        roundPercent = (int)((seriesDataValue * 100) / totalSum);
        pieLabel->pieLabel.Format("%d%%", roundPercent);
        pieLabels->AddTail(pieLabel);

        if (s == 0) {
          lastYLocation += 1;
        }

        COLORREF barColor;
        barColor = graphLegend[s].color;
        POINT p1(lastXLocation, lastYLocation);
        POINT p2(newXLocation, newYLocation);

        COLORREF piePen(barColor);
        TSelectFont(piePen);
        ;
        hDC->Pie(pieRect, p1, p2);

        lastXLocation = newXLocation;
        lastYLocation = newYLocation;
      }


    }

    //draw lines and labels for pies slices
    POSITION labelPos;

    COLORREF lineBrush(BLACK);
    COLORREF linePen(BLACK);
    TSelectFont(lineBrush);
    TSelectFont(linePen);
    POSITION mainLinePos, checkLinePos;
    mainLinePos = pieLabels->GetHeadPosition();
    int numLinesDrawn = 0;
    CGraphPieLabel* currentLabel;
    CGraphPieLabel* tmpLabel;

    while (mainLinePos != NULL) {
      currentLabel = (CGraphPieLabel*)pieLabels->GetNext(mainLinePos);
      int r = 0;
      checkLinePos = pieLabels->GetHeadPosition();

      while (r < numLinesDrawn) {
        //check if any overlap in label areas
        tmpLabel = (CGraphPieLabel*)pieLabels->GetAt(checkLinePos);

        if ((currentLabel->topLeftX > tmpLabel->topLeftX) &&
            (currentLabel->topLeftX < (tmpLabel->topLeftX - maxLabelWidth)) &&
            (currentLabel->topLeftY > tmpLabel->topLeftY) &&
            (currentLabel->topLeftY < (tmpLabel->topLeftY - maxLabelHeight))) {
          //OVERLAP !!!
          //move current label top left position up or down
          //depending on its quadrant
          if (currentLabel->labelQuadrant < 2) {
            //move label down to tmplabel topleft + height
            currentLabel->topLeftY = tmpLabel->topLeftY - maxLabelHeight;
            currentLabel->lineYEnd = tmpLabel->lineYEnd - maxLabelHeight;
          }
          else {
            //move label up to tmpLabel topleft - height
            currentLabel->topLeftY = tmpLabel->topLeftY + maxLabelHeight;
            currentLabel->lineYEnd = tmpLabel->lineYEnd + maxLabelHeight;
          }

          //reset r value to 0 so it starts over, just in
          //case we moved the label and it overlaps another
          r = 0;
          checkLinePos = pieLabels->GetHeadPosition();
        }
        else {
          r++;
          pieLabels->GetNext(checkLinePos);
        }

      }

      //draw the line and label
      TMoveTo(hDC, currentLabel->lineXStart, currentLabel->lineYStart);
      _LineTo(hDC, currentLabel->lineXEnd, currentLabel->lineYEnd);

      //write the label
      TTextOut2(hDC, currentLabel->topLeftX, currentLabel->topLeftY,
          currentLabel->pieLabel);
      numLinesDrawn++;
    }

    //now done, remove everything inside the label list
    labelPos = pieLabels->GetHeadPosition();

    while (labelPos != NULL) {
      pieLabel = (CGraphPieLabel*)pieLabels->GetNext(labelPos);
      delete pieLabel;
    }

    delete pieLabels;
  }


}

void CGraph::Print3DStackedBarSeries(HDDC hDC)
{
  int barWidth;
  int barL, barT, barR, barB;
  int tickXLocation, tickYLocation;
  int seriesSpace;
  int barHeight;


  int blue;
  int red;
  int green;

  if (graphAlignment) {
    //determine width of barchart data blocks
    seriesSpace = xAxisWidth  / ngraphSeries;
    barWidth = (int)(seriesSpace * .8);



    double dataScale = 0.00;
    dataScale = (yAxisHeight * 1.00) / (maxTick - minTick);

    for (int x = 1; x <= ngraphSeries; x++) {


      tickXLocation = xApexPoint + ((x * seriesSpace) - (seriesSpace / 2));

      barT = yApexPoint;  //this is a running total;

      //it starts at yApexPoint so barB can
      //start there, then moves up by barHeight
      for (int s = 0; s < seriesSize; s++) {
        if (data[x * seriesSize + s] > -1) {
          barHeight = (int)(((data[x * seriesSize + s] - minTick) * 1.00) * dataScale);

          if (((data[x * seriesSize + s] - minTick) > 0) && (barHeight < 1)) {
            barHeight = (int)dataScale;
          }

          barB = barT;
          barL = tickXLocation - (barWidth / 2);

          if (barHeight > yAxisHeight) {
            barT = barB + yAxisHeight;
          }
          else {
            barT = barB + barHeight;
          }

          barR = barL + barWidth;

          COLORREF barColor;
          barColor = graphLegend[s].color;

          COLORREF rectPen(barColor);

          _clr = (rectPen);


          draw_rect(hDC, barL, barT, barR, barB, barColor);

          //now, we do the side
          //side is darker than front, so subtract
          //from color to make closer to black
          red = GetRV(barColor);
          green = GetGV(barColor);
          blue = GetBV(barColor);
          int sideRed = red - 35;
          int sideGreen = green - 35;
          int sideBlue = blue - 35;

          if (sideRed < 0) {
            sideRed = 0;
          }

          if (sideGreen < 0) {
            sideGreen = 0;
          }

          if (sideBlue < 0) {
            sideBlue = 0;
          }

          COLORREF sideColor;
          sideColor = _RGB(sideRed, sideGreen, sideBlue);
          COLORREF sideBrush(sideColor);
          COLORREF sidePen(sideColor);
          TSelectFont(sideBrush);
          TSelectFont(sidePen);
          POINT sidePolyArray[4];
          POINT sp1(barR, barT);
          POINT sp2(barR, barB);
          POINT sp3(barR + depth, barB + depth);
          POINT sp4(barR + depth, barT + depth);
          sidePolyArray[0] = sp1;
          sidePolyArray[1] = sp2;
          sidePolyArray[2] = sp3;
          sidePolyArray[3] = sp4;

          hDC->Polygon(sidePolyArray, 4);

          //finally, the top
          int topRed = red + 35;
          int topGreen = green + 35;
          int topBlue = blue + 35;

          if (topRed > 255) {
            topRed = 255;
          }

          if (topGreen > 255) {
            topGreen = 255;
          }

          if (topBlue > 255) {
            topBlue = 255;
          }

          COLORREF topColor;
          topColor = _RGB(topRed, topGreen, topBlue);
          COLORREF topBrush(topColor);
          COLORREF topPen(topColor);
          TSelectFont(topBrush);
          TSelectFont(topPen);
          POINT topPolyArray[4];
          POINT tp1(barL, barT);
          POINT tp2(barR, barT);
          POINT tp3(barR + depth, barT + depth);
          POINT tp4(barL + depth, barT + depth);
          topPolyArray[0] = tp1;
          topPolyArray[1] = tp2;
          topPolyArray[2] = tp3;
          topPolyArray[3] = tp4;

          hDC->Polygon(topPolyArray, 4);



        }
      }
    }
  }
  else {
    //determine width of barchart data blocks
    seriesSpace = yAxisHeight / ngraphSeries;
    barHeight = (int)(seriesSpace * .8);



    double dataScale = 0.00;
    dataScale = ((xAxisWidth - 10) * 1.00) / (maxTick - minTick);

    for (int y = 0; y < ngraphSeries; y++) {


      tickYLocation = yApexPoint + ((y * seriesSpace) - (seriesSpace / 2));

      barR = xApexPoint;  //this is a running total;

      //it starts at xApexPoint so barL can
      //start there, then moves right by barWidth
      for (int s = 0; s < seriesSize; s++) {
        if (data[x * seriesSize + s] > -1) {

          barWidth = (int)(((data[x * seriesSize + s] - minTick) * 1.00) * dataScale);

          if (((data[x * seriesSize + s] - minTick) > 0) && (barWidth < 1)) {
            barWidth = (int)dataScale;
          }

          barL = barR;
          barT = tickYLocation + (barHeight / 2);
          barB = barT - barHeight;

          if (barWidth > xAxisWidth) {
            barR = barL + xAxisWidth;
          }
          else {
            barR = barL + barWidth;
          }

          COLORREF barColor;
          barColor = graphLegend[s].color;



          COLORREF rectPen(barColor);

          _clr = (rectPen);
          draw_rect(hDC, barL, barT, barR, barB, barColor);

          //now, we do the side
          //side is darker than front, so subtract
          //from color to make closer to black
          red = GetRV(barColor);
          green = GetGV(barColor);
          blue = GetBV(barColor);
          int sideRed = red - 35;
          int sideGreen = green - 35;
          int sideBlue = blue - 35;

          if (sideRed < 0) {
            sideRed = 0;
          }

          if (sideGreen < 0) {
            sideGreen = 0;
          }

          if (sideBlue < 0) {
            sideBlue = 0;
          }

          COLORREF sideColor;
          sideColor = _RGB(sideRed, sideGreen, sideBlue);
          COLORREF sideBrush(sideColor);
          COLORREF sidePen(sideColor);
          TSelectFont(sideBrush);
          TSelectFont(sidePen);
          POINT sidePolyArray[4];
          POINT sp1(barL, barT);
          POINT sp2(barR, barT);
          POINT sp3(barR + depth, barT + depth);
          POINT sp4(barL + depth, barT + depth);
          sidePolyArray[0] = sp1;
          sidePolyArray[1] = sp2;
          sidePolyArray[2] = sp3;
          sidePolyArray[3] = sp4;

          hDC->Polygon(sidePolyArray, 4);

          //finally, the top
          int topRed = red + 35;
          int topGreen = green + 35;
          int topBlue = blue + 35;

          if (topRed > 255) {
            topRed = 255;
          }

          if (topGreen > 255) {
            topGreen = 255;
          }

          if (topBlue > 255) {
            topBlue = 255;
          }

          COLORREF topColor;
          topColor = _RGB(topRed, topGreen, topBlue);
          COLORREF topBrush(topColor);
          COLORREF topPen(topColor);
          TSelectFont(topBrush);
          TSelectFont(topPen);
          POINT topPolyArray[4];
          POINT tp1(barR, barT);
          POINT tp2(barR, barB);
          POINT tp3(barR + depth, barB + depth);
          POINT tp4(barR + depth, barT + depth);
          topPolyArray[0] = tp1;
          topPolyArray[1] = tp2;
          topPolyArray[2] = tp3;
          topPolyArray[3] = tp4;

          hDC->Polygon(topPolyArray, 4);



        }
      }
    }
  }
}

void CGraph::PrintSeries(HDDC hDC)
{
  switch (graphType) {
  case BAR_GRAPH :
    PrintBarSeries(hDC);
    break;

  case LINE_GRAPH :
    PrintLineSeries(hDC);
    break;

  case PIE_GRAPH :
    PrintPieSeries(hDC);
    break;

  case SCATTER_GRAPH :
    PrintScatterSeries(hDC);
    break;

  case BOX_WHISKER_GRAPH :
    PrintBoxWhiskerSeries(hDC);
    break;

  case STACKED_BAR_GRAPH :
    PrintStackedBarSeries(hDC);
    break;

  case XY_LINE_GRAPH :
    PrintXYLineSeries(hDC);
    break;

    //    case RADAR_GRAPH :
    //         PrintRadarSeries(hDC);
    //         break;
  case BAR_GRAPH_3D :
    Print3DBarSeries(hDC);
    break;

  case LINE_GRAPH_3D :
    Print3DLineSeries(hDC);
    break;

  case STACKED_BAR_GRAPH_3D :
    Print3DStackedBarSeries(hDC);
    break;

  case PIE_GRAPH_3D :
    Print3DPieSeries(hDC);
    break;

  default :
    ASSERT(0 && "No graph type to print");
    break;
  }
}


void CGraph::PrintAxis(HDDC hDC)
{
  TEXTMETRIC tm;
  COLORREF clrText = (BLACK);
  COLORREF _clr = (clrText);
  int sideFont, axisFont;
  int charWidth;
  int charHeight;
  int tFontSize;
  double tickScale;
  int tickYLocation;
  int seriesSpace;
  int tickXLocation;
  int _x0, _y0;

  switch (graphType) {
  case BAR_GRAPH :
  case LINE_GRAPH :
  case SCATTER_GRAPH :
  case BOX_WHISKER_GRAPH :

    //    case RADAR_GRAPH :
  case STACKED_BAR_GRAPH :
  case XY_LINE_GRAPH :
    //draw y axis
    TMoveTo(hDC, xApexPoint, yApexPoint);
    //unlike drawAxis, the lineto below uses a + in the y
    //coordinate because printer uses negative locations
    //when going down a page, so use a + to move the line
    //upwards...
    _LineTo(hDC, xApexPoint, yApexPoint + yAxisHeight);

    //draw x axis
    TMoveTo(hDC, xApexPoint, yApexPoint);
    _LineTo(hDC, xApexPoint + xAxisWidth, yApexPoint);

    //draw labels
    tFontSize = 16;

    if (maxWidth > maxHeight) {
      while ((STRLEN(axisYLabel) * (tFontSize / 2)) > maxHeight) {
        tFontSize -= 2;
      }
    }
    else {
      while ((STRLEN(axisXLabel) * (tFontSize / 2)) > maxWidth) {
        tFontSize -= 2;
      }
    }

    tFontSize *= 20;
    axisFont = font_create("Arial", tFontSize, 0);
    sideFont = font_create("Arial", tFontSize, 0);
    int pOldFont;
    pOldFont = TSelectFont(sideFont);
    tm = GetCurFont()->tm;
    charWidth = tm.tmAveCharWidth;
    charHeight = tm.tmHeight;

    if (graphAlignment) {
      TTextOut2(hDC, pGraphL + 10, pGraphT - (maxHeight / 2) - ((STRLEN(axisYLabel) / 2) * charHeight), axisYLabel);

      TSelectFont(axisFont);
      tm = GetCurFont()->tm;
      charWidth = tm.tmAveCharWidth;

      TTextOut2(hDC, xApexPoint + (xAxisWidth / 2) - ((STRLEN(axisXLabel) / 2) * charWidth), pGraphB + 50 + charHeight, axisXLabel);

      TSelectFont(pOldFont);

      tickScale = 0.00;
      tickScale = (yAxisHeight - 100) / numTicks;

      //draw y axis ticks
      for (int y = 1; y <= numTicks; y++) {
        tickYLocation = yApexPoint + (int)(y * tickScale);

        //draw tick mark
        TMoveTo(hDC, xApexPoint - 60, tickYLocation);
        _LineTo(hDC, xApexPoint + 60, tickYLocation);

        if (graphHasGridLines) {
          //draw grid lines
          COLORREF gridLineColor;
          gridLineColor = DARK_GREY;
          _clr = (gridLineColor);

          TMoveTo(hDC, xApexPoint, tickYLocation);
          _LineTo(hDC, xApexPoint + xAxisWidth, tickYLocation);

        }

        //draw tick label
        char tickLabel[256];
        _snprintf(tickLabel, 256, "%d", minTick + (y * tickSpace));

        int yFont;
        yFont = font_create("Arial", yTickFontSize * 20, 0);
        pOldFont = TSelectFont(yFont);

        TTextOut2(hDC, xApexPoint - 200 - (STRLEN(tickLabel) * (yTickFontSize / 2) * 20), tickYLocation + 120, tickLabel);

        p_topYTick = tickYLocation;
        TSelectFont(pOldFont);

      }

      //draw X axis tick marks
      if (graphType != SCATTER_GRAPH) {



        for (int x = 1; x <= ngraphSeries; x++) {


          int seriesSpace;
          int tickXLocation;
          seriesSpace = xAxisWidth / ngraphSeries;
          tickXLocation = xApexPoint + ((x * seriesSpace) - (seriesSpace / 2));

          TMoveTo(hDC, tickXLocation, yApexPoint - 60);
          _LineTo(hDC, tickXLocation, yApexPoint + 60);

          //draw tick label
          char* tickLabel = graphSeries[x];

          if (!xAxisAlign) { //horizontal
            TTextOut2(hDC, tickXLocation - ((STRLEN(tickLabel) / 2) * (xTickFontSize * 10)), yApexPoint - 160, tickLabel);
          }
          else {
            int sideFont2;
            sideFont2 = font_create("Arial", xTickFontSize * 20, 0);
            pOldFont = TSelectFont(sideFont2);
            tm = GetCurFont()->tm;
            charWidth = tm.tmAveCharWidth;

            if (xAxisAlign < 180) {
              TTextOut2(hDC, tickXLocation - (xTickFontSize * 10), yApexPoint - (xTickFontSize * 10) - (STRLEN(axisXLabel) * charWidth), tickLabel);
            }
            else {
              TTextOut2(hDC, tickXLocation + (xTickFontSize * 10), yApexPoint - 160, tickLabel);
            }

          }

          p_rightXTick = tickXLocation;
          TSelectFont(pOldFont);
        }
      }
      else {
        //scatter graphs will use the same tick marking on both axis lines
        int seriesSpace;
        seriesSpace = (xAxisWidth - 200) / numTicks;

        for (int x = 1; x <= numTicks; x++) {
          int tickXLocation;
          tickXLocation = xApexPoint + (x * seriesSpace);

          //draw tick mark
          TMoveTo(hDC, tickXLocation, yApexPoint - 60);
          _LineTo(hDC, tickXLocation, yApexPoint + 60);

          //draw tick label
          char tickLabel[256];
          _snprintf(tickLabel, 256, "%d", x * tickSpace);

          int xFont;
          xFont = font_create("Arial", xTickFontSize * 20, 0);
          pOldFont = TSelectFont(xFont);
          tm = GetCurFont()->tm;
          charWidth = tm.tmAveCharWidth;

          TTextOut2(hDC, tickXLocation - (STRLEN(tickLabel) * charWidth / 2), yApexPoint - 160, tickLabel);

          p_rightXTick = tickXLocation;
          TSelectFont(pOldFont);
        }
      }
    }
    else {
      //reverse above stuff and treat as horizontal graph
      TTextOut2(hDC, pGraphL + 10, pGraphT - (maxHeight / 2) - ((STRLEN(axisXLabel) / 2) * charHeight), axisXLabel);

      TSelectFont(axisFont);
      tm = GetCurFont()->tm;
      charWidth = tm.tmAveCharWidth;
      int labelHeight = tm.tmHeight + 50;

      if (STRLEN(axisYLabel) == 0) {
        labelHeight = 0;
      }

      TTextOut2(hDC, xApexPoint + (xAxisWidth / 2) - ((STRLEN(axisYLabel) / 2) * charWidth), pGraphB + 50 + labelHeight, axisYLabel);

      TSelectFont(pOldFont);

      //to allow scalability (height may be less than tickRange)
      double tickScale;
      tickScale = 0.00;

      //draw y axis ticks
      if (graphType != SCATTER_GRAPH) {
        tickScale = (yAxisHeight - 100) / seriesSize;



        for (int y = 0; y < ngraphSeries; y++) {
          int seriesSpace;
          int tickYLocation;
          seriesSpace = (yAxisHeight - 50) / ngraphSeries;
          tickYLocation = yApexPoint + ((y * seriesSpace) - (seriesSpace / 2));

          //draw tick mark
          TMoveTo(hDC, xApexPoint - 60, tickYLocation);
          _LineTo(hDC, xApexPoint + 60, tickYLocation);

          //draw tick label
          char* tickLabel = graphSeries[y];

          int sideFont2;
          sideFont2 = font_create("Arial", yTickFontSize * 20, 0);
          pOldFont = TSelectFont(sideFont2);
          tm = GetCurFont()->tm;

          TTextOut2(hDC, xApexPoint - (STRLEN(tickLabel) * (yTickFontSize * 10)) - 200, tickYLocation + 120, tickLabel);

          p_topYTick = tickYLocation;
          TSelectFont(pOldFont);
        }
      }
      else {
        //scatter graphs will use the same tick marking on both axis lines
        for (int y = 1; y <= numTicks; y++) {
          tickScale = (yAxisHeight - 100) / numTicks;
          int tickYLocation;
          tickYLocation = yApexPoint + (int)(y * tickScale);

          //draw tick mark
          TMoveTo(hDC, xApexPoint - 60, tickYLocation);
          _LineTo(hDC, xApexPoint + 60, tickYLocation);

          //draw tick label
          char tickLabel[256];
          _snprintf(tickLabel, 256, "%d", y * tickSpace);

          int yFont;
          yFont = font_create("Arial", yTickFontSize * 20, 0);
          pOldFont = TSelectFont(yFont);

          TTextOut2(hDC, xApexPoint - (STRLEN(tickLabel) * (yTickFontSize * 10)) - 200, tickYLocation + 120, tickLabel);

          p_topYTick = tickYLocation;
          TSelectFont(pOldFont);
        }
      }

      //draw X axis tick marks
      tickScale = (xAxisWidth - 200) / numTicks;

      for (int x = 1; x <= numTicks; x++) {
        int tickXLocation;
        tickXLocation = xApexPoint + (int)(x * tickScale);

        TMoveTo(hDC, tickXLocation, yApexPoint - 60);
        _LineTo(hDC, tickXLocation, yApexPoint + 60);

        if (graphHasGridLines) {
          //draw grid lines
          COLORREF gridLineColor;
          gridLineColor = DARK_GREY;
          _clr = (gridLineColor);
          TMoveTo(hDC, tickXLocation, yApexPoint);
          _LineTo(hDC, tickXLocation, yApexPoint + yAxisHeight);

        }

        //draw tick label
        char tickLabel[256];
        _snprintf(tickLabel, 256, "%d", x * tickSpace);

        if (!xAxisAlign) { //horizontal
          int xFont;
          xFont = font_create("Arial", xTickFontSize * 20, 0);
          pOldFont = TSelectFont(xFont);
          charWidth = tm.tmAveCharWidth;
          TTextOut2(hDC, tickXLocation - ((STRLEN(tickLabel) * charWidth) / 2), yApexPoint - 160, tickLabel);
          TSelectFont(pOldFont);
        }
        else {
          int sideFont2;
          sideFont2 = font_create("Arial", xTickFontSize * 20, 0);
          pOldFont = TSelectFont(sideFont2);
          tm = GetCurFont()->tm;
          charWidth = tm.tmAveCharWidth;

          if (xAxisAlign < 180) {
            TTextOut2(hDC, tickXLocation - (xTickFontSize * 10), yApexPoint - (xTickFontSize * 10) - (STRLEN(axisXLabel) * charWidth), tickLabel);
          }
          else {
            TTextOut2(hDC, tickXLocation + (xTickFontSize * 10), yApexPoint - 160, tickLabel);
          }

          TSelectFont(pOldFont);
        }

        p_rightXTick = tickXLocation;
      }
    }

    break;

  case BAR_GRAPH_3D :
  case LINE_GRAPH_3D :
  case STACKED_BAR_GRAPH_3D :
    if (graphAlignment) {
      depth = (int)(yAxisHeight * .05);
    }
    else {
      depth = (int)(xAxisWidth * .05);
    }

    xApexPoint += depth;
    yApexPoint += depth;
    xAxisWidth -= depth;
    yAxisHeight -= depth;
    //draw y axis
    TMoveTo(hDC, xApexPoint, yApexPoint);
    _LineTo(hDC, xApexPoint, yApexPoint + yAxisHeight);
    //draw x axis
    TMoveTo(hDC, xApexPoint, yApexPoint);
    _LineTo(hDC, xApexPoint + xAxisWidth, yApexPoint);

    //adjust graph settings for 3D look
    yApexPoint -= depth;
    xApexPoint -= depth;
    //foreground axis
    TMoveTo(hDC, xApexPoint, yApexPoint - (int)(depth * .5));
    _LineTo(hDC, xApexPoint, yApexPoint + yAxisHeight);
    TMoveTo(hDC, xApexPoint - (int)(depth * .5), yApexPoint);
    _LineTo(hDC, xApexPoint + xAxisWidth, yApexPoint);
    //connecting line at apex
    TMoveTo(hDC, xApexPoint, yApexPoint);
    _LineTo(hDC, xApexPoint + depth, yApexPoint + depth);
    //connecting line at height
    TMoveTo(hDC, xApexPoint, yApexPoint + yAxisHeight);
    _LineTo(hDC, xApexPoint + depth, yApexPoint + yAxisHeight + depth);
    //connecting line at width
    TMoveTo(hDC, xApexPoint + xAxisWidth, yApexPoint);
    _LineTo(hDC, xApexPoint + xAxisWidth + depth, yApexPoint + depth);
    //top and right border lines
    TMoveTo(hDC, xApexPoint + depth, yApexPoint + yAxisHeight + depth);
    _LineTo(hDC, xApexPoint + xAxisWidth + depth, yApexPoint + yAxisHeight + depth);
    _LineTo(hDC, xApexPoint + xAxisWidth + depth, yApexPoint + depth);

    //Tick marks ... and labels too
    //draw labels
    tFontSize = 16;

    if (maxWidth > maxHeight) {
      while ((STRLEN(axisYLabel) * (tFontSize / 2)) > maxHeight) {
        tFontSize -= 2;
      }
    }
    else {
      while ((STRLEN(axisXLabel) * (tFontSize / 2)) > maxWidth) {
        tFontSize -= 2;
      }
    }

    axisFont = font_create("Arial", tFontSize * 20, 0);
    sideFont = font_create("Arial", tFontSize * 20, 0);

    pOldFont = TSelectFont(sideFont);
    tm = GetCurFont()->tm;
    charWidth = tm.tmAveCharWidth;
    charHeight = tm.tmHeight;

    if (graphAlignment) {
      TTextOut2(hDC, pGraphL + 10, pGraphT - (maxHeight / 2) - ((STRLEN(axisYLabel) * charWidth) / 2), axisYLabel);

      TSelectFont(axisFont);
      tm = GetCurFont()->tm;
      charWidth = tm.tmAveCharWidth;
      TTextOut2(hDC, xApexPoint + (xAxisWidth / 2) - ((STRLEN(axisXLabel) / 2) * charWidth), pGraphB + 50 + charHeight, axisXLabel);

      TSelectFont(pOldFont);

      //to allow scalability (height may be less than tickRange)
      tickScale = 0.00;
      tickScale = yAxisHeight / numTicks;

      //draw y axis ticks
      for (int y = 1; y <= numTicks; y++) {
        tickYLocation = yApexPoint + (int)(y * tickScale);

        //draw tick mark
        TMoveTo(hDC, xApexPoint - (int)(depth * .5), tickYLocation);
        _LineTo(hDC, xApexPoint, tickYLocation);

        COLORREF gridLineColor;

        if (graphHasGridLines) {
          //draw grid lines
          gridLineColor = DARK_GREY;
          COLORREF gridPen(gridLineColor);
          _clr = (gridPen);
          TMoveTo(hDC, xApexPoint, tickYLocation);
          _LineTo(hDC, xApexPoint + depth, tickYLocation + depth);
          _LineTo(hDC, xApexPoint + xAxisWidth + depth, tickYLocation + depth);

        }

        //draw tick label
        char tickLabel[256];
        _snprintf(tickLabel, 256, "%d", y * tickSpace);

        int yFont;
        yFont = font_create("Arial", yTickFontSize * 20, 0);
        pOldFont = TSelectFont(yFont);

        TTextOut2(hDC, xApexPoint - 200 - (STRLEN(tickLabel) * ((yTickFontSize / 2) * 20)), tickYLocation + 120, tickLabel);
        topYTick = tickYLocation;
        TSelectFont(pOldFont);

        //draw 1/2 tick line (grey)
        int midTickYLocation;
        midTickYLocation = yApexPoint + (int)(y * tickScale * .5) + (int)((y - 1) * tickScale * .5);

        //draw tick mark
        TMoveTo(hDC, xApexPoint - (int)(depth * .5), midTickYLocation);
        _LineTo(hDC, xApexPoint, midTickYLocation);

        if (graphHasGridLines) {
          //draw grid lines
          gridLineColor = LIGHT_GREY;
          COLORREF midGridPen(gridLineColor);
          _clr = (midGridPen);
          TMoveTo(hDC, xApexPoint, midTickYLocation);
          _LineTo(hDC, xApexPoint + depth, midTickYLocation + depth);
          _LineTo(hDC, xApexPoint + xAxisWidth + depth, midTickYLocation + depth);

        }
      }

      //draw X axis tick marks



      for (int x = 1; x <= ngraphSeries; x++) {


        seriesSpace = xAxisWidth / ngraphSeries;
        tickXLocation = xApexPoint + ((x * seriesSpace) - (seriesSpace / 2));

        //unlike normal series, we put ticks here
        //on the outsides of the series
        TMoveTo(hDC, xApexPoint + (x * seriesSpace), yApexPoint - (int)(depth * .5));
        _LineTo(hDC, xApexPoint + (x * seriesSpace), yApexPoint);
        _LineTo(hDC, xApexPoint + (x * seriesSpace) + depth, yApexPoint + depth);

        //draw tick label
        char* tickLabel = graphSeries[x];

        if (!xAxisAlign) { //horizontal
          TTextOut2(hDC, tickXLocation - ((STRLEN(tickLabel) / 2) * (xTickFontSize * 10)), yApexPoint - 160, tickLabel);
        }
        else {
          int sideFont2;
          sideFont2 = font_create("Arial", xTickFontSize * 20, 0);
          pOldFont = TSelectFont(sideFont2);
          tm = GetCurFont()->tm;
          charWidth = tm.tmAveCharWidth;

          if (xAxisAlign < 180) {
            TTextOut2(hDC, tickXLocation - (xTickFontSize * 10), yApexPoint - (xTickFontSize * 10) - (STRLEN(axisXLabel) * charWidth), tickLabel);
          }
          else {
            TTextOut2(hDC, tickXLocation + (xTickFontSize * 10), yApexPoint - 160, tickLabel);
          }

          TSelectFont(pOldFont);
        }

        p_rightXTick = tickXLocation;
      }
    }
    else {
      //reverse above stuff and treat as horizontal graph
      TTextOut2(hDC, pGraphL + 10, pGraphT - (maxHeight / 2) - ((STRLEN(axisXLabel) / 2) * charHeight), axisXLabel);

      TSelectFont(axisFont);
      tm = GetCurFont()->tm;
      charWidth = tm.tmAveCharWidth;
      int labelHeight;
      labelHeight = tm.tmHeight + 50;

      if (STRLEN(axisYLabel) == 0) {
        labelHeight = 0;
      }

      TTextOut2(hDC, xApexPoint + (xAxisWidth / 2) - ((STRLEN(axisXLabel) / 2) * charWidth), pGraphB + 50 + labelHeight, axisYLabel);

      TSelectFont(pOldFont);

      //to allow scalability (height may be less than tickRange)
      tickScale = 0.00;

      //draw y axis ticks
      tickScale = (yAxisHeight - 100) / seriesSize;



      for (int y = 0; y < ngraphSeries; y++) {


        seriesSpace = (yAxisHeight - 50) / ngraphSeries;
        tickYLocation = yApexPoint + (y * seriesSpace);

        //draw tick mark
        //unlike normal series, we put ticks here on the
        //outside of the series
        TMoveTo(hDC, xApexPoint - (int)(depth * .5), tickYLocation);
        _LineTo(hDC, xApexPoint, tickYLocation);

        //draw tick label
        char* tickLabel = graphSeries[y];

        int yFont;
        yFont = font_create("Arial", yTickFontSize * 20, 0);
        pOldFont = TSelectFont(yFont);
        tm = GetCurFont()->tm;

        TTextOut2(hDC, xApexPoint - (STRLEN(tickLabel) * (yTickFontSize * 10)), tickYLocation + 120, tickLabel);

        p_topYTick = tickYLocation;
        TSelectFont(pOldFont);
      }

      //draw X axis tick marks
      tickScale = (xAxisWidth - 200) / numTicks;

      for (int x = 1; x <= numTicks; x++) {
        tickXLocation = xApexPoint + (int)(x * tickScale);

        TMoveTo(hDC, tickXLocation, yApexPoint - (int)(depth * .25));
        _LineTo(hDC, tickXLocation, yApexPoint);

        COLORREF gridLineColor;

        if (graphHasGridLines) {
          //draw grid lines
          gridLineColor = DARK_GREY;
          COLORREF gridPen(gridLineColor);
          _clr = (gridPen);
          TMoveTo(hDC, tickXLocation, yApexPoint);
          _LineTo(hDC, tickXLocation + depth, yApexPoint + depth);
          _LineTo(hDC, tickXLocation + depth, yApexPoint + yAxisHeight + depth);

        }

        //draw tick label
        char tickLabel[256];
        _snprintf(tickLabel, 256, "%d", x * tickSpace);

        if (!xAxisAlign) { //horizontal
          int xFont;
          xFont = font_create("Arial", xTickFontSize * 20, 0);
          pOldFont = TSelectFont(xFont);
          charWidth = tm.tmAveCharWidth;
          TTextOut2(hDC, tickXLocation - ((STRLEN(tickLabel) * charWidth) / 2), yApexPoint - 160, tickLabel);
          TSelectFont(pOldFont);
        }
        else {
          int sideFont2;
          sideFont2 = font_create("Arial", xTickFontSize * 20, 0);
          pOldFont = TSelectFont(sideFont2);
          tm = GetCurFont()->tm;
          charWidth = tm.tmAveCharWidth;

          if (xAxisAlign < 180) {
            TTextOut2(hDC, tickXLocation - (xTickFontSize * 10), yApexPoint - (xTickFontSize * 10) - (STRLEN(axisXLabel) * charWidth), tickLabel);
          }
          else {
            TTextOut2(hDC, tickXLocation + (xTickFontSize * 10), yApexPoint - 160, tickLabel);
          }

          TSelectFont(pOldFont);
        }

        p_rightXTick = tickXLocation;

        //draw 1/2 tick line (grey)
        int midTickXLocation;
        midTickXLocation = xApexPoint + (int)(x * tickScale * .5) + (int)((x - 1) * tickScale * .5);

        //draw tick mark
        TMoveTo(hDC, midTickXLocation, yApexPoint - (int)(depth * .25));
        _LineTo(hDC, midTickXLocation, yApexPoint);

        if (graphHasGridLines) {
          //draw grid lines
          gridLineColor = LIGHT_GREY;
          COLORREF midGridPen(gridLineColor);
          TSelectFont(midGridPen);
          TMoveTo(hDC, midTickXLocation, yApexPoint);
          _LineTo(hDC, midTickXLocation + depth, yApexPoint + depth);
          _LineTo(hDC, midTickXLocation + depth, yApexPoint + yAxisHeight + depth);

        }
      }

    }

    break;
  }
}




int CGraph::PrintGraph(HDDC hDC)
{
  int    graphTitleFont, textFont;
  TEXTMETRIC  tm;
  char tickLabel[256];
  COLORREF _clr = _RGB(0, 0, 0);

  maxHeight = pGraphT - pGraphB;
  maxWidth = pGraphR - pGraphL;

  //titleFont is 24 point, bold
  graphTitleFont = font_create("Times New Roman", -480, 0);

  //textFont is 12 point
  textFont = font_create("Times New Roman", -240, 0);

  int pOldFont = TSelectFont(graphTitleFont);
  tm = GetCurFont()->tm;

  int charWidth = tm.tmAveCharWidth;
  int charHeight = tm.tmHeight;
  //next line is centered....trust me !!
  TTextOut2(hDC, pGraphL + (maxWidth / 2) - ((STRLEN(graphTitle) / 2) * charWidth), pGraphT - 50, graphTitle);

  TSelectFont(pOldFont);

  if ((graphType == PIE_GRAPH) || (graphType == PIE_GRAPH_3D)) {
    xAxisWidth = pGraphR - pGraphL - 200;
    yAxisHeight = maxHeight - 600 - charHeight;
    xApexPoint = pGraphL + 500;
    yApexPoint = pGraphB + 500;
  }
  else {
    //compute label size for axis alignment
    int sideFont, axisFont;
    int tFontSize;
    tFontSize = 16;

    if (maxWidth > maxHeight) {
      while ((STRLEN(axisYLabel) * (tFontSize / 2)) > maxHeight) {
        tFontSize -= 2;
      }
    }
    else {
      while ((STRLEN(axisXLabel) * (tFontSize / 2)) > maxWidth) {
        tFontSize -= 2;
      }
    }

    //set tFontSize for printer
    tFontSize *= -20;
    axisFont = font_create("Arial", tFontSize, 0);
    sideFont = font_create("Arial", tFontSize, 0);

    int pOldFont = TSelectFont(sideFont);
    tm = GetCurFont()->tm;
    int charWidth = tm.tmAveCharWidth;
    int labelHeight = tm.tmHeight + 50;

    if (graphAlignment) {
      if (STRLEN(axisXLabel) == 0) {
        labelHeight = 0;
      }
    }
    else {
      if (STRLEN(axisYLabel) == 0) {
        labelHeight = 0;
      }
    }

    TSelectFont(pOldFont);

    _snprintf(tickLabel, 256, "%d", maxTick);


    //determine axis specifications
    if (graphAlignment) {
      xApexPoint = pGraphL + (STRLEN(tickLabel) * charWidth) + labelHeight + 100;

      if (!xAxisAlign) { //horizontal
        yApexPoint = pGraphB + 50 + (labelHeight * 2);
      }
      else {
        yApexPoint = pGraphB + labelHeight + (STRLEN(axisXLabel) * labelHeight);
      }

      yAxisHeight = pGraphT - 200 - charHeight/*this is title height*/ - yApexPoint;
      xAxisWidth = (maxWidth - 100) - xApexPoint;

    }
    else {
      xApexPoint = pGraphL + (STRLEN(axisXLabel) * charWidth) + labelHeight;

      yApexPoint = pGraphB + 50 + (labelHeight * 2);
      yAxisHeight = pGraphT - 200 - charHeight/*this is title height*/ - yApexPoint;
      xAxisWidth = (maxWidth - 100) - xApexPoint;
    }
  }

  //draw legend
  if (graphHasLegend) {
    int legendL = PrintLegend(hDC);
    xAxisWidth = legendL - 200 - xApexPoint;
  }

  if (graphType != 2) { //not pie
    //draw axis lines
    PrintAxis(hDC);
  }

  //draw series data and labels
  PrintSeries(hDC);


  //update pGraphB value to pad some space below the graph
  pGraphB -= 90;
  return pGraphB;
}


int CGraph::PrintLegend(HDDC hDC)
{
  int    legendLabelFont, legendTitleFont;
  TEXTMETRIC  tm;

  int legendL, legendT, legendR, legendB;
  int barL, barT, barR, barB;
  COLORREF _clr = BLACK;

  //legendLabelFont is 10 point
  legendLabelFont = font_create("Times New Roman", -200, 0);

  //legendTitleFont is 12 point
  legendTitleFont = font_create("Times New Roman", -240, 0);

  int pOldFont = TSelectFont(legendTitleFont);
  tm = GetCurFont()->tm;
  int legendCharWidth = tm.tmAveCharWidth;
  legendT = (((pGraphB - pGraphT) / 2) + pGraphT) + (((seriesSize + 1) / 2) * 240 + 300);
  legendB = (((pGraphB - pGraphT) / 2) + pGraphT) - (((seriesSize + 1) / 2) * 240 + 300);
  legendR = pGraphR - 100;
  legendL = legendR - ((ngraphLegend * legendCharWidth) + 1000);
  legendWidth = legendR - legendL;

  draw_rect(hDC, legendL, legendT, legendR, legendB, _clr);
  TTextOut2(hDC, legendL + (legendWidth / 2) - 360,
      legendT - 160, "Legend");
  TSelectFont(pOldFont);

  for (int i = 0; i < seriesSize; i++) {
    pOldFont = TSelectFont(legendLabelFont);
    tm = GetCurFont()->tm;

    int charWidth = tm.tmMaxCharWidth;
    int charHeight = tm.tmHeight;

    TTextOut2(hDC, legendL + 100, legendT - 500 - (i * charHeight), graphLegend[i].label);
    TSelectFont(pOldFont);

    //draw bar
    COLORREF barColor;
    barColor = graphLegend[i].color;




    //barT uses a 560 offset below,...why?
    //would be 500, but barB is only using 120 height,
    //when it should be 240 (but 240 puts bars too close
    //together, so I'm setting them to 1/2 size.
    //therefore, I want to adjust my barT by half of that
    //adjustment ( 1/2 of 120 is 60).  This basically centers
    //the bar 1/2 way in text, and makes it 1/2 the height
    //of text
    barL = legendL + 50 + (legendMaxText * charWidth / 2);
    barT = legendT - 560 - (i * charHeight);
    barR = legendR - 100;
    barB = barT - 120;
    draw_rect(hDC, barL, barT, barR, barB, _clr);



  }

  return legendL;
}

