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
#include <vector>
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
    constexpr unsigned short W_UNSET{ 0xFFFF };
    constexpr int DW_UNSET{ -1 };

    struct UsbDevInfo
    {
        unsigned short Vid{ W_UNSET };
        unsigned short Pid{ W_UNSET };
        unsigned short Ver{ W_UNSET };
        CString manufactureStr{_T("")};
        CString productStr{_T("")};
        CString serialNumStr{_T("")};
        unsigned short InRptBytesLen{0};
        unsigned short OutRptBytesLen{0};
        PSP_DEVICE_INTERFACE_DETAIL_DATA detailData{ nullptr };

        UsbDevInfo() = default;
        virtual ~UsbDevInfo()
        {
            if (detailData) {
                free(detailData);
                detailData = nullptr;
            }
        }

        UsbDevInfo(const UsbDevInfo& other) = default;
        UsbDevInfo& operator=(const UsbDevInfo& other) = default;
        UsbDevInfo(UsbDevInfo&& other) = default;
        UsbDevInfo& operator=(UsbDevInfo&& other) = default;
    };

    enum class DevMatchStrategy
    {
        VID_ONLY = 0,
        VID_PID,
        VID_PID_VER,
        VID_PID_SN,
        VID_PID_VER_SN
    };

    class UsbHidManager
    {
    public:
        explicit UsbHidManager() = default;
        virtual ~UsbHidManager() = default;

        void FindOneTargetHidDev(const UsbDevInfo& dev, DevMatchStrategy strategy = DevMatchStrategy::VID_PID_VER);
        CString GetVidStrHex();
        CString GetPidStrHex();
        CString GetVerStrHex();
        std::optional<UsbDevInfo> GetCurDevInfo(size_t index);
        std::optional<UsbDevInfo> GetCurDevInfo();
        void RefreshDevlist();
        int GetDevlistCount()
        {
            size_t sz = m_hidDevList.size();
            return static_cast<int>(sz);
        }

        CString toHexStr(unsigned short val);

        void SetTargetDev(int idx)
        {
            m_curDevIdx = idx;
        }

        void CloseCurDev();
        bool OpenTargetDev();

        int SendCmds(std::string& cmd);

        int RecvRsp(char* resultBuff, const int bufLen);

    private:
        std::vector<UsbDevInfo> m_hidDevList;
        int m_curDevIdx{ DW_UNSET };

        HANDLE m_devHandle{nullptr};



    }; // class
} // namespace


