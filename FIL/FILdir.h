#pragma once
// ---------------------------------------------------------------------------
// Copyright (C) For Ever Eliot Muir .  All Rights Reserved
//
// Module: FILdir
//
// Description:
//
// File utilities
//
// Author: Eliot Muir 
// Date:   Wednesday 01 July 2026 - 07:40PM
// ---------------------------------------------------------------------------

#include "COL/COLbase.h"

#define FIL_DIR_SEPARATOR "/"

COLstring FILdirCurrent();

bool FILpathSimplify(COLstring* pPath);

class COLfile{
public:
   bool IsDir;
};
   
bool FILdirList(const COLstring& Dir, COLdictSorted<COLstring, COLfile>* pList); 
