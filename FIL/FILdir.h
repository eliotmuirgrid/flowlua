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

class BASstring;

#define FIL_DIR_SEPARATOR "/"

BASstring FILdirCurrent();
bool FILpathSimplify(BASstring* pPath);

