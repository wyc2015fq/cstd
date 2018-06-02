
#include "GraphSeries.h"
#include "GraphDataSet.inl"

/////////////////////////////////////////////////////////////////////////////
// CGraphSeries

CGraphSeries::CGraphSeries()
{
	dataValues = new CObList();
}

CGraphSeries::~CGraphSeries()
{
	POSITION pos;
	CGraphDataSet *dataSet;
	pos = dataValues->GetHeadPosition();
	while(pos != NULL)
	{
		dataSet = (CGraphDataSet*)dataValues->GetAt(pos);
		dataValues->RemoveAt(pos);
		delete dataSet;
	}
	delete dataValues;
}


/////////////////////////////////////////////////////////////////////////////
// CGraphSeries message handlers

void CGraphSeries::SetLabel(char* label)
{
	seriesLabel = label;
}

void CGraphSeries::SetData(int xValue, int yValue)
{
	CGraphDataSet *dataSet = new CGraphDataSet();
	dataSet->SetXPosition(xValue);
	dataSet->SetYValue(yValue);
	dataValues->AddTail(dataSet);
}

char* CGraphSeries::GetLabel()
{
	return seriesLabel;
}

int CGraphSeries::GetData(int xValue)
{
	POSITION pos;
	pos = dataValues->GetHeadPosition();
	int retVal = 0;
	while(pos != NULL)
	{
		CGraphDataSet *dataSet;
		dataSet = (CGraphDataSet*)dataValues->GetAt(pos);
		if(dataSet->GetXData() == xValue)
		{
			retVal = dataSet->GetYData();
			return retVal;
		}
		dataValues->GetNext(pos);
	}
	return 0;
}

int CGraphSeries::GetDataCount()
{
	return dataValues->GetCount();
}

int CGraphSeries::GetXDataValue()
{
	POSITION pos;
	pos = dataValues->GetHeadPosition();
	CGraphDataSet *retVal;
	retVal = (CGraphDataSet*)dataValues->GetAt(pos);

	return retVal->GetXData();
}

int CGraphSeries::GetYDataValue()
{
	POSITION pos;
	pos = dataValues->GetHeadPosition();
	CGraphDataSet *retVal;
	retVal = (CGraphDataSet*)dataValues->GetAt(pos);

	return retVal->GetYData();
}
