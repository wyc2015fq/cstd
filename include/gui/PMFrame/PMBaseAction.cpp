#include "StdAfx.h"
#include "PMBaseAction.h"

#define MAX_POSITION_COUNT 100
PMBaseAction::PMBaseAction(void):mIndex(0)
{
}

PMBaseAction::~PMBaseAction(void)
{
	
}

PMVoid PMBaseAction::AddExcute( TJSON *pAction )
{
	ClearCommandList(mRedoList);
	Excute(*pAction);
}

PMVoid PMBaseAction::ClearCommandList( TJSONS &cmdList )
{
	for (TJSONS::iterator it = cmdList.begin(); it != cmdList.end(); it++)
	{
		TJSON::DeleteJSON(*it);
	}
	cmdList.clear();
}

PMVoid PMBaseAction::UnDo()
{
	if(mUndoList.size()>0)
	{
		TJSON *jnAction = mUndoList.back();
		mUndoList.pop_back();
		UnExcute(*jnAction);
	}
}

PMVoid PMBaseAction::ReDo()
{
	if(mRedoList.size()>0)
	{
		TJSON *jnAction = mRedoList.back();
		mRedoList.pop_back();		
		Excute(*jnAction);
	}
}

TJSON * PMBaseAction::CreateCommand( PMInt type )
{
	TJSON *jnCmd = new TJSON();
	(*jnCmd)[0]=type;
	return jnCmd;
}

PMVoid PMBaseAction::Destroy()
{
	ClearCommandList(mRedoList);
	ClearCommandList(mUndoList);
	ClearCommandList(mPositionList);
}

PMBool PMBaseAction::ExcuteForward( IntRect &OutObj )
{
	if( mIndex+1<(PMInt)mPositionList.size() )
	{
		mIndex++;
		TJSON *jnAction = mPositionList[mIndex];	
		OutObj = jnAction->GetRect();
		return true;
	}
	return false;	
}

PMBool PMBaseAction::ExcuteBackward( IntRect &OutObj )
{
	if(mPositionList.size()>0 && mIndex-1>=0)
	{
		mIndex--;
		TJSON *jnAction = mPositionList[mIndex];
		OutObj = jnAction->GetRect();
		return true;
	}
	return false;
}

PMVoid PMBaseAction::AddPosition( IntRect &InObj )
{
	//1、简单处理先前的数据
	if(mIndex<(PMInt)mPositionList.size()-1)//需要清理后面的数据
	{
		TJSONS::iterator it    = mPositionList.begin() + mIndex + 1;
		TJSONS::iterator itEnd = mPositionList.end();
		for(;it!=itEnd;it++)
		{
			TJSON::DeleteJSON(*it);
		}
		mPositionList.erase(mPositionList.begin()+mIndex+1,itEnd);
	}else if(mPositionList.size()>MAX_POSITION_COUNT)
	{
		TJSON::DeleteJSON(mPositionList.front());
		mPositionList.erase(mPositionList.begin());
		if(mIndex>0)mIndex--;
	}
	if(mPositionList.size()>0)
	{
		TJSON *jnAction = mPositionList[mIndex];
		if(jnAction->At(1).ToInt()==InObj.top)//行相同不需要重新存储
		{
			jnAction->At(0) = InObj.left;
			jnAction->At(2) = InObj.right;
			jnAction->At(3) = InObj.bottom;
			return;
		}
	}
	TJSON *jnPosition = new TJSON;
	jnPosition->Push(InObj);
	mPositionList.push_back(jnPosition);
	mIndex = mPositionList.size() - 1;
}