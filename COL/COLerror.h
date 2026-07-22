#pragma once
//-------------------------------------------------------
// Copyright (C) 2021 Eliot Muir.  All Rights Reserved
//
// COLerror
// 
// Basic cross platform function to translating operating
// system error codes into the description given by the operating system.
//-------------------------------------------------------

class COLstring;

// This returns the error code from the operating system translation table.  May be quite incomprehensible :-)
COLstring COLerrorMessage(int ErrorCode);

