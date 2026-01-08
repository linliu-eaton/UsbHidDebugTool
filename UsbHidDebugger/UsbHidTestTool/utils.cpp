
#include "pch.h"
#include <string>
#include <cmath>
#include <chrono>
#include <iomanip>
#include <sstream>

#include "utils.h"

namespace
{
    std::string getLocalTime()
    {
        const std::string TIME_FORMAT{ "%Y-%m-%d %H:%M:%S" };
        std::stringstream ss;
        auto now = std::chrono::system_clock::now();
        uint64_t ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count()
            - std::chrono::duration_cast<std::chrono::seconds>(now.time_since_epoch()).count() * 1000;

        time_t t = std::chrono::system_clock::to_time_t(now);
        struct tm st;
        localtime_s(&st, &t);
        ss << std::put_time(&st, TIME_FORMAT.c_str());
        ss << "." << ms;
        std::string tmStr = ss.str();
        return tmStr;
    }

}

namespace utils
{
    // Dlg log handler
    void ShowLog(CEdit& editCtrlLog, CString& editLog, CString& str)
    {
        editCtrlLog.LineScroll(editCtrlLog.GetLineCount());

        const size_t  maxLogSize{ 20 * 1024 * 1024 };
        if (editLog.GetLength() >= maxLogSize)
        {
            editLog = _T("");
        }

        std::string now = getLocalTime();
        CString nowTime(now.c_str());

        CString s = nowTime + _T(" ") + str + _T("\r\n");
        editLog += s;
        editCtrlLog.SetWindowText(editLog);
        editCtrlLog.LineScroll(editCtrlLog.GetLineCount());
    }

    int CstringHex2Int(CString& cstr)
    {
        cstr.MakeUpper();
        cstr.Trim();
        cstr.Trim(_T("\r\n"));
        cstr.TrimLeft(_T("0X"));
        if (cstr.IsEmpty())   // empty
        {
            return utils::INVALID_VALUE;
        }

        CT2CA pszConvertedAnsiString(cstr);
        std::string strs(pszConvertedAnsiString);
        std::reverse(strs.begin(), strs.end());
        int intVal = 0;
        for (size_t i = 0; i < strs.length(); i++) {
            if (strs[i] >= '0' && strs[i] <= '9') {
                intVal += (strs[i] - 0x30) * (int)std::pow(16, i);
            }
            else if ((strs[i] >= 'A' && strs[i] <= 'F')) {
                intVal += (strs[i] - 0x37) * (int)std::pow(16, i);
            }
            else {
                return utils::INVALID_VALUE;
            }
        }

        return intVal;
    }

    std::string Int2HexStr(unsigned int data)
    {
        std::stringstream ss;
        ss << std::hex << std::setfill('0');
        ss << std::setw(2) << data;

        return ss.str();
    }

    std::string ByteArray2HexString(const uint8_t* arr, size_t arrLen)
    {
        std::string result{""};

        if (!arr || arrLen == 0) {
            return result;
        }

        for (size_t i = 0; i < arrLen; i++) {
            std::string hexStr = Int2HexStr(arr[i]);
            result += hexStr + " ";
        }
        result.erase(result.find_last_not_of(" ") + 1);

        return result;
    }

}

