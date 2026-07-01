#ifndef _BAS_SINK_H_
#define _BAS_SINK_H_
//-------------------------------------------------------
// Copyright (C) 2021 Eliot Muir.  All Rights Reserved
//
// BASsink
// 
// Sink virtual interface.
//-------------------------------------------------------

class BASsink{
public:
    BASsink(){}
    virtual ~BASsink() {}

    virtual int write(const char* pData, int Size)=0; 
    virtual void flush()=0;
};

#endif
