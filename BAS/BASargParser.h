#ifndef _BAS_ARG_PARSER_H_
#define _BAS_ARG_PARSER_H_
//-------------------------------------------------------
// Copyright (C) 2021 Eliot Muir.  All Rights Reserved
//
// BASargParser
// 
// Command line argument parser
//-------------------------------------------------------

#include "BASavlTree.h"
#include "BASstring.h"

class BASstream;

class BASargParser{
public:
   BASargParser();
   ~BASargParser();

   bool parse(int argc, const char** argv);

   void addArgFlag(const BASstring& Name, const BASstring& Arg, const BASstring& Description);
   void addFlag(const BASstring& Name, const BASstring& Description);

   bool present(const BASstring& Name) const             { return m_Flags.value(Name).Present;  }
   const BASstring& flagArg(const BASstring& Name) const { return m_Flags.value(Name).ArgValue; }

   void showUsage(BASstream& Stream) const;

private:
   struct LineInfo{
      LineInfo(const BASstring& iDescription, const BASstring& iArg) : HasArgument(true), Description(iDescription), Arg(iArg), Present(false){ }
      LineInfo(const BASstring& iDescription) : HasArgument(false), Description(iDescription), Present(false) {}
      bool HasArgument;
      BASstring Description;
      BASstring Arg;
      bool Present;
      BASstring ArgValue;
   };

   BASstring m_Bin;
   BASavlTree<BASstring, LineInfo> m_Flags;
};

BASstream& operator<<(BASstream& Stream, const BASargParser& Parser);

#endif
