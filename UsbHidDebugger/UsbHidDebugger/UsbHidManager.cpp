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
    CString toHexStr(unsigned short val)
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
        return _T("");
    }
}

namespace usb::hid
{
    bool UsbHidManager::OpenHidDev(unsigned short vid, unsigned short pid)
    {
        log_info("Open Hid Device...");

        GUID hidGuid;
        HidD_GetHidGuid(&hidGuid);
        HANDLE hDevInfo = SetupDiGetClassDevs(&hidGuid, NULL, NULL, DIGCF_PRESENT | DIGCF_INTERFACEDEVICE);
        SP_INTERFACE_DEVICE_DATA devInfoData;
        devInfoData.cbSize = sizeof(devInfoData);

        int memberInedx = 0;
        while (1) {
            int result = SetupDiEnumDeviceInterfaces(hDevInfo, 0, &hidGuid, memberInedx, &devInfoData);
            memberInedx++;
            if (!result) {
                if (ERROR_NO_MORE_ITEMS == GetLastError()) {
                    log_info("No device found. Vid=" <<  vid << ", Pid=" << pid);
                    SetupDiDestroyDeviceInfoList(hDevInfo);
                    return false;
                }
                continue;
            }

            ULONG length = 0;
            (void)SetupDiGetDeviceInterfaceDetail(hDevInfo, &devInfoData, NULL, 0, &length, NULL);
            PSP_DEVICE_INTERFACE_DETAIL_DATA detailData = (PSP_DEVICE_INTERFACE_DETAIL_DATA)malloc(length);
            if (detailData == NULL) {
                SetupDiDestroyDeviceInfoList(hDevInfo);
                return false;
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
                DeviceHandle = nullptr;
                continue;
            }

            HIDD_ATTRIBUTES Attributes;
            Attributes.Size = sizeof(Attributes);
            (void)HidD_GetAttributes(DeviceHandle, &Attributes);
            if (Attributes.VendorID == vid && Attributes.ProductID == pid) {
                m_hidDev.Vid = Attributes.VendorID;
                m_hidDev.Pid = Attributes.ProductID;
                m_hidDev.Ver = Attributes.VersionNumber;

                WCHAR manufactures[255];
                memset(manufactures, 0, sizeof(manufactures));
                (void)HidD_GetManufacturerString(DeviceHandle, (PVOID)manufactures, sizeof(manufactures));
                m_hidDev.manufactureStr = manufactures;
                WCHAR products[255];
                memset(products, 0, sizeof(products));
                (void)HidD_GetProductString(DeviceHandle, (PVOID)products, sizeof(products));
                m_hidDev.productStr = products;
                WCHAR serialN[255];
                memset(serialN, 0, sizeof(serialN));
                (void)HidD_GetSerialNumberString(DeviceHandle, (PVOID)serialN, sizeof(serialN));
                m_hidDev.serialNumStr = serialN;

                PHIDP_PREPARSED_DATA	PreparsedData;
                HIDP_CAPS               Capabilities;
                HidD_GetPreparsedData(DeviceHandle, &PreparsedData);
                HidP_GetCaps(PreparsedData, &Capabilities);
                HidD_FreePreparsedData(PreparsedData);
                m_hidDev.InRptBytesLen = Capabilities.InputReportByteLength;
                m_hidDev.OutRptBytesLen = Capabilities.OutputReportByteLength;
                m_devHandle = DeviceHandle;

                free(detailData);
                SetupDiDestroyDeviceInfoList(hDevInfo);
                log_info("Device found. Vid=" <<  vid << ", Pid=" << pid);
                return true;
            }
            free(detailData);
            CloseHandle(DeviceHandle);
            DeviceHandle = nullptr;
        }
        return false;
    }

    CString UsbHidManager::GetVidStrHex()
    {
        try
        {
            unsigned short vid = m_hidDev.Vid;
            return toHexStr(vid);
        }
        catch (const std::exception&) {
            log_info("Exception when get Vid str hex.");
        }
        return _T("");
    }

    CString UsbHidManager::GetPidStrHex()
    {
        try
        {
            unsigned short pid = m_hidDev.Pid;
            return toHexStr(pid);
        }
        catch (const std::exception&) {
            log_info("Exception when get Pid str hex.");
        }
        return _T("");
    }

    CString UsbHidManager::GetVerStrHex()
    {
        try
        {
            unsigned short ver = m_hidDev.Ver;
            return toHexStr(ver);
        }
        catch (const std::exception&) {
            log_info("Exception when get Ver str hex.");
        }
        return _T("");
    }

    CString UsbHidManager::GetManufactureStr()
    {
        return m_hidDev.manufactureStr;
    }

    CString UsbHidManager::GetProductStr()
    {
        return m_hidDev.productStr;
    }

    CString UsbHidManager::GetSerialNum()
    {
        return m_hidDev.serialNumStr;
    }

    unsigned short UsbHidManager::InReportSize()
    {
        return m_hidDev.InRptBytesLen;
    }

    unsigned short UsbHidManager::OutReportSize()
    {
        return m_hidDev.OutRptBytesLen;
    }

    int UsbHidManager::SendCmds(std::string& cmd)
    {
        log_info("SendCmds: cmd:" << cmd);
    
        unsigned char array[255] = {0};
        memset(&(array[0]), 0, sizeof(array));
        memcpy_s(&(array[1]), sizeof(array) - 2, cmd.c_str(), cmd.length());

        int ret = HidD_SetOutputReport(m_devHandle, array, m_hidDev.OutRptBytesLen);
        return ret;
    }

    int UsbHidManager::RecvRsp(char* resultBuff, const int bufLen)
    {
        if (!resultBuff || bufLen <= 0) {
            return FALSE;
        }

        DWORD bytesRead{ 0 };
        int ret = ReadFile(m_devHandle, resultBuff, m_hidDev.InRptBytesLen, &bytesRead, NULL);

        log_info("RecvRsp: rsp:" << std::string{resultBuff + 1});
        if (ret) {
            return bytesRead;
        }
        
        return FALSE;
    }

} // namespace


