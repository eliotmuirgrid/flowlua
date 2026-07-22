#ifndef _COL_SINK_H_
#define _COL_SINK_H_
//-------------------------------------------------------
// Copyright (C) 2021 Eliot Muir.  All Rights Reserved
//
// COLsink
// 
// Sink virtual interface.
//-------------------------------------------------------

class COLsink{
public:
    COLsink(){}
    virtual ~COLsink() {}

    virtual int write(const char* pData, int Size)=0; 
    virtual void flush()=0;
};

#endif
