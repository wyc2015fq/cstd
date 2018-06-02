#pragma once

#include "CppSQLite3.h"

class CFaceSqlite
{
public:
	CFaceSqlite();
	virtual ~CFaceSqlite();

public:
	CppSQLite3DB facedb;
};

