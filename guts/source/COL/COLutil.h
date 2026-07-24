#pragma once
//-------------------------------------------------------
// Copyright (C) 2021 Eliot Muir.  All Rights Reserved
//
// COLutil
//
// MAX/MIN macros etc.
//-------------------------------------------------------

template <typename T> constexpr const T& COL_MAX(const T& A, const T& B) { return A > B ? A : B; }
template <typename T> constexpr const T& COL_MIN(const T& A, const T& B) { return A < B ? A : B; }


void COLsleep(int Milliseconds);

