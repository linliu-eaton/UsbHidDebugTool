/**
********************************************************************************
**
**  Copyright (C) 2025 Eaton
**
**  This software is confidential and licensed under Eaton Proprietary License
**  (EPL or EULA).
**  This software is not authorized to be used, duplicated or disclosed to
**  anyone without the prior written permission of Eaton.
**  Limitations, restrictions and exclusions of the Eaton applicable standard
**  terms and conditions, such as its EPL and EULA, apply.
**
********************************************************************************
**
**  \file UsbHidManager.h
**  \author Ln
**
********************************************************************************
**
**  \brief This file handle all infos of USB HID
**
********************************************************************************
**
*/
#pragma once

#include <string>
#include <optional>

#ifdef __cplusplus
extern "C" {
#endif
#include "hidsdi.h"
#include "setupapi.h"

#ifdef __cplusplus
}
#endif


namespace usb::hid
{
    #define DEFAULT_CMD_RECV_TIMEOUT_MS  1000
    #define DEFAULT_CMD_INTERV_MS        10

    struct HidDevInfo
    {
        unsigned short Vid{ 0 };
        unsigned short Pid{ 0 };
        unsigned short Ver{ 0 };
        CString manufactureStr{_T("")};
        CString productStr{_T("")};
        CString serialNumStr{_T("")};
        unsigned short InRptBytesLen{ 0 };
        unsigned short OutRptBytesLen{ 0 };

        HidDevInfo() = default;
        virtual ~HidDevInfo() = default;

        HidDevInfo(const HidDevInfo& other) = delete;
        HidDevInfo& operator=(const HidDevInfo& other) = delete;
        HidDevInfo(HidDevInfo&& other) = delete;
        HidDevInfo& operator=(HidDevInfo&& other) = delete;

        void init()
        {
            Vid = 0;
            Pid = 0;
            Ver = 0;
            manufactureStr = _T("");
            productStr = _T("");
            serialNumStr = _T("");
            InRptBytesLen = 0;
            OutRptBytesLen = 0;
        }
    };


    class UsbHidManager
    {
    public:
        explicit UsbHidManager() = default;
        virtual ~UsbHidManager()
        {
            CloseHidDev();
        }

        bool OpenHidDev(unsigned short vid, unsigned short pid);
        void CloseHidDev()
        {
            if (m_devHandle != nullptr)
            {
                CloseHandle(m_devHandle);
                m_devHandle = nullptr;
            } 
        }

        CString GetVidStrHex();
        CString GetPidStrHex();
        CString GetVerStrHex();
        CString GetManufactureStr();
        CString GetProductStr();
        CString GetSerialNum();
        unsigned short InReportSize();
        unsigned short OutReportSize();

        int SendCmd(std::string& cmd);
        int RecvRsp(char* resultBuff, const int bufLen, int timeout);

    private:
        HidDevInfo m_hidDev;
        HANDLE m_devHandle{nullptr};

    }; // class
} // namespace


