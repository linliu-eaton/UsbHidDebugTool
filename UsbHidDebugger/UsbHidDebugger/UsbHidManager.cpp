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
**  \file UsbHidManager.cpp
**  \author Ln
**
********************************************************************************
**
**  \brief This file handle all infos of USB HID
**
********************************************************************************
**
*/

#include "pch.h"
#include "./logger/Logger.hpp"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include "./logger/Logger.hpp"
#include "UsbHidManager.h"


namespace {
    using namespace usb::hid;

    bool descendSort(const UsbDevInfo& d1, const UsbDevInfo& d2)
    {
        return d1.Vid > d2.Vid;
    }
}


namespace usb::hid
{
    void UsbHidManager::FindOneTargetHidDev(const UsbDevInfo& dev, DevMatchStrategy strategy)
    {
        switch (strategy)
        {
        case DevMatchStrategy::VID_PID:
            for (int i=0; i < static_cast<int>(m_hidDevList.size()); i++)
            {
                if (dev.Vid == m_hidDevList[i].Vid && dev.Pid == m_hidDevList[i].Pid)
                {
                    m_curDevIdx = i;
                    log_info("FindOneTargetHidDev, VID_PID, Vid=" << dev.Vid << ", Pid=" << dev.Pid << ", m_curDevIdx=" << m_curDevIdx);
                    return;
                }
            }
            break;
        case DevMatchStrategy::VID_ONLY:
            for (int i = 0; i < static_cast<int>(m_hidDevList.size()); i++)
            {
                if (dev.Vid == m_hidDevList[i].Vid)
                {
                    m_curDevIdx = i;
                    log_info("FindOneTargetHidDev, VID_ONLY, Vid=" << dev.Vid << ", Pid=" << dev.Pid << ", m_curDevIdx=" << m_curDevIdx);
                    return;
                }
            }
            break;
        case DevMatchStrategy::VID_PID_VER:
            for (int i = 0; i < static_cast<int>(m_hidDevList.size()); i++)
            {
                if (dev.Vid == m_hidDevList[i].Vid && dev.Pid == m_hidDevList[i].Pid && dev.Ver == m_hidDevList[i].Ver)
                {
                    m_curDevIdx = i;
                    log_info("FindOneTargetHidDev, VID_PID_VER, Vid=" << dev.Vid << ", Pid=" << dev.Pid << ", Ver=" << dev.Ver << ", m_curDevIdx=" << m_curDevIdx);
                    return;
                }
            }
            break;
        case DevMatchStrategy::VID_PID_SN:
            for (int i = 0; i < static_cast<int>(m_hidDevList.size()); i++)
            {
                if (dev.Vid == m_hidDevList[i].Vid && dev.Pid == m_hidDevList[i].Pid && dev.serialNumStr == m_hidDevList[i].serialNumStr)
                {
                    m_curDevIdx = i;
                    log_info("FindOneTargetHidDev, VID_PID_SN, Vid=" << dev.Vid << ", Pid=" << dev.Pid << ", SN=" << dev.serialNumStr << ", m_curDevIdx=" << m_curDevIdx);
                    return;
                }
            }
            break;
        case DevMatchStrategy::VID_PID_VER_SN:
            for (int i = 0; i < static_cast<int>(m_hidDevList.size()); i++)
            {
                if (dev.Vid == m_hidDevList[i].Vid && dev.Pid == m_hidDevList[i].Pid 
                    && dev.Ver == m_hidDevList[i].Ver && dev.serialNumStr == m_hidDevList[i].serialNumStr)
                {
                    m_curDevIdx = i;
                    log_info("FindOneTargetHidDev, VID_PID_VER_SN, Vid=" << dev.Vid << ", Pid=" << dev.Pid << ", Ver=" << dev.Ver << ", SN=" << dev.serialNumStr << ", m_curDevIdx=" << m_curDevIdx);
                    return;
                }
            }
            break;
        default:
            break;

        m_curDevIdx = W_UNSET;
        return;
        }
    }
    
    CString UsbHidManager::toHexStr(unsigned short val)
    {
        try
        {
            std::stringstream ss;
            ss << std::hex << std::setw(4) << std::setfill('0') << val;
            std::string str =  ss.str();
            CString cstr(str.c_str());
            return cstr.MakeUpper();
        } catch (const std::exception&) {
        }
        return _T("Unknown");

    }

    CString UsbHidManager::GetVidStrHex()
    {
        try
        {
            unsigned short vid = m_hidDevList.at(m_curDevIdx).Vid;
            return toHexStr(vid);
        }
        catch (const std::exception&) {
            // easylog
        }
        return _T("Unknown");
    }

    CString UsbHidManager::GetPidStrHex()
    {
        try
        {
            unsigned short pid = m_hidDevList.at(m_curDevIdx).Pid;
            return toHexStr(pid);
        }
        catch (const std::exception&) {
            // easylog
        }
        return _T("Unknown");
    }

    CString UsbHidManager::GetVerStrHex()
    {
        try
        {
            unsigned short ver = m_hidDevList.at(m_curDevIdx).Ver;
            return toHexStr(ver);
        }
        catch (const std::exception&) {
            // easylog
        }
        return _T("Unknown");
    }

    std::optional<UsbDevInfo> UsbHidManager::GetCurDevInfo(size_t index)
    {
        if(index >= m_hidDevList.size())
        {
            return std::nullopt;
        }

        return m_hidDevList[index];
    }

    std::optional<UsbDevInfo> UsbHidManager::GetCurDevInfo()
    {
        return GetCurDevInfo(m_curDevIdx);
    }

    void UsbHidManager::RefreshDevlist()
    {
        log_info("RefreshDevlist: build device list!");

        m_hidDevList.clear();

        GUID hidGuid;
        HidD_GetHidGuid(&hidGuid);
        HANDLE hDevInfo = SetupDiGetClassDevs(&hidGuid, NULL, NULL, DIGCF_PRESENT | DIGCF_INTERFACEDEVICE);
        SP_INTERFACE_DEVICE_DATA devInfoData;
        devInfoData.cbSize = sizeof(devInfoData);

        int memberInedx = 0;
        while(1)
        {
            int result = SetupDiEnumDeviceInterfaces(hDevInfo, 0, &hidGuid, memberInedx, &devInfoData);
            memberInedx++;
            if(!result) {
                if (ERROR_NO_MORE_ITEMS == GetLastError()) {
                    break;
                }
                continue;
            }

            ULONG length = 0;
            (void)SetupDiGetDeviceInterfaceDetail(hDevInfo, &devInfoData, NULL, 0, &length, NULL);
            PSP_DEVICE_INTERFACE_DETAIL_DATA detailData = (PSP_DEVICE_INTERFACE_DETAIL_DATA)malloc(length);
            if (detailData == NULL) {
                break;
            }
            detailData->cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA);
            ULONG required;
            (void)SetupDiGetDeviceInterfaceDetail(hDevInfo, &devInfoData, detailData, length, &required, NULL);
            HANDLE DeviceHandle = CreateFile(detailData->DevicePath,
                                             GENERIC_READ | GENERIC_WRITE,
                                             FILE_SHARE_READ | FILE_SHARE_WRITE,
                                             (LPSECURITY_ATTRIBUTES)NULL,
                                             OPEN_EXISTING,
                                             0,
                                             NULL);
            if (INVALID_HANDLE_VALUE == DeviceHandle) {
                free(detailData);
                continue;
			}

            UsbDevInfo  devInfo;

            // save detail data
            devInfo.detailData = detailData;

            // get string descriptor
            {
                WCHAR manufactures[255];
                memset(manufactures, 0, sizeof(manufactures));
                (void)HidD_GetManufacturerString(DeviceHandle, (PVOID)manufactures, sizeof(manufactures));
                WCHAR products[255];
                memset(products, 0, sizeof(products));
                (void)HidD_GetProductString(DeviceHandle, (PVOID)products, sizeof(products));
                WCHAR serialN[255];
                memset(serialN, 0, sizeof(serialN));
                (void)HidD_GetSerialNumberString(DeviceHandle, (PVOID)serialN, sizeof(serialN));
                devInfo.manufactureStr = manufactures;
                devInfo.productStr = products;
                devInfo.serialNumStr = serialN;
            }

            // get atrributes
            {
                HIDD_ATTRIBUTES  Attributes;
                Attributes.Size = sizeof(Attributes);
                (void)HidD_GetAttributes(DeviceHandle, &Attributes);
                devInfo.Vid = Attributes.VendorID;
                devInfo.Pid = Attributes.ProductID;
                devInfo.Ver = Attributes.VersionNumber;
            }

            // get report size
            {
                PHIDP_PREPARSED_DATA	PreparsedData;
                HIDP_CAPS               Capabilities;
                HidD_GetPreparsedData(DeviceHandle, &PreparsedData);
                HidP_GetCaps(PreparsedData, &Capabilities);
                HidD_FreePreparsedData(PreparsedData);
                devInfo.InRptBytesLen = Capabilities.InputReportByteLength;
                devInfo.OutRptBytesLen = Capabilities.OutputReportByteLength;
            }

            log_info("Device found: Vid:" << devInfo.Vid << ", Pid:" << devInfo.Pid << ", Ver:" << devInfo.Ver);
            m_hidDevList.push_back(devInfo);

            // free(detailData);  // can't free it here because it is saved to devinfo
            CloseHandle(DeviceHandle);
            DeviceHandle = INVALID_HANDLE_VALUE;
        }
        SetupDiDestroyDeviceInfoList(hDevInfo);
        sort(m_hidDevList.begin(),m_hidDevList.end(), descendSort);
    }

    void UsbHidManager::CloseCurDev()
    {
        if (m_devHandle != nullptr)
        {
            CloseHandle(m_devHandle);
            m_devHandle = nullptr;
        }
    }

    bool UsbHidManager::OpenTargetDev()
    {
        CloseCurDev();  // make sure the earlier handle closed.

        auto dev = GetCurDevInfo();
        if (!dev.has_value())
        {
            return false;
        }

        log_info("OpenTargetDev: DevPath:" << (dev.value().detailData)->DevicePath);
        m_devHandle = CreateFile((dev.value().detailData)->DevicePath,
                                    GENERIC_READ | GENERIC_WRITE,
                                    FILE_SHARE_READ | FILE_SHARE_WRITE,
                                    (LPSECURITY_ATTRIBUTES)NULL,
                                    OPEN_EXISTING,
                                    0,
                                    NULL);
        return m_devHandle == nullptr ? false : true;
    }

    int UsbHidManager::SendCmds(std::string& cmd)
    {
        log_info("SendCmds: cmd:" << cmd);
    
        unsigned char array[255] = {0};
        memset(&(array[0]), 0, sizeof(array));
        memcpy_s(&(array[1]), sizeof(array) - 2, cmd.c_str(), cmd.length());

        int ret = HidD_SetOutputReport(m_devHandle, array, m_hidDevList[m_curDevIdx].OutRptBytesLen);
        return ret;
    }

    int UsbHidManager::RecvRsp(char* resultBuff, const int bufLen)
    {
        if (!resultBuff || bufLen <= 0) {
            return FALSE;
        }

        DWORD bytesRead{ 0 };
        int ret = ReadFile(m_devHandle, resultBuff, m_hidDevList[m_curDevIdx].InRptBytesLen, &bytesRead, NULL);

        log_info("RecvRsp: rsp:" << resultBuff);
        if (ret) {
            return bytesRead;
        }
        
        return FALSE;
    }

} // namespace


