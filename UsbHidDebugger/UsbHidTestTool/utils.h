#pragma once

#include "pch.h"


namespace utils
{
    constexpr int BUFF_SZIE = 255;

    constexpr int INVALID_VALUE = -1;
    constexpr int TIMER_CHK_RSP_INNER = 100;       // timer
    constexpr int TIMER_CMD_INTERV = 300;          // timer
    constexpr int CHK_RSP_INNER_TIMEOUT_Q = 5; // ms

    constexpr int TIMER_START_SEND_SHUT = 500;       // timer
    constexpr int START_SEND_TMOUT_SHUT = 5;  // ms
    constexpr int TIMER_CMD_INTERVAL_SHUT = 600;       // timer


    void ShowLog(CEdit& editCtrlLog, CString& editLog, CString& str);
    int CstringHex2Int(CString& cstr);

    std::string Int2HexStr(unsigned int data);
    std::string ByteArray2HexString(const uint8_t* arr, size_t arrLen);
}

