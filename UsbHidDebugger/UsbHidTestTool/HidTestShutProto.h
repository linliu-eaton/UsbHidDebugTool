#pragma once

#include <vector>
#include "utils.h"
#include "UsbHidManager.h"

enum class SHUT_CMD_TYPE {
    CMD_SET_RPT,
    CMD_GET_RPT,
    CMD_GET_DESC,
    CMD_SET_FEAT,
    CMD_GET_FEAT,
    CMD_COUNT
};

struct ShutCmdInfo {
    SHUT_CMD_TYPE type;
    uint8_t msg[utils::BUFF_SZIE];  // 1st byte must be ReportID, ReportID + data
    size_t msgLen;

    ShutCmdInfo()
    {
        type = SHUT_CMD_TYPE::CMD_COUNT;
        memset(msg, 0, sizeof(msg) * sizeof(uint8_t));
        msgLen = 0;
    }
    virtual ~ShutCmdInfo() = default;

    ShutCmdInfo(const ShutCmdInfo& other) = default;
    ShutCmdInfo& operator=(const ShutCmdInfo& other) = default;
    ShutCmdInfo(ShutCmdInfo&& other) = default;
    ShutCmdInfo& operator=(ShutCmdInfo&& other) = default;

};


// CHidTestShutProto dialog

class CHidTestShutProto : public CDialogEx
{
	DECLARE_DYNAMIC(CHidTestShutProto)

public:
	CHidTestShutProto(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CHidTestShutProto();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_SHUT };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
    CEdit m_editCtrlLogShut;
    CString m_editLogShut;
    afx_msg void OnBnClickedButtonClrlogShut();
    void ShowLog(CString str);

    CEdit m_editInputVidShut;
    CEdit m_editInputPidShut;
    unsigned short m_VidShut{ 0 };
    unsigned short m_PidShut{ 0 };

    CButton m_btnOpenDevShut;
    CButton m_btnCloseDevShut;
    afx_msg void OnBnClickedButtonOpenDevShut();
    afx_msg void OnBnClickedButtonCloseDevShut();

    // different with Q Dialog
    CComboBox m_combRptType1;
    CComboBox m_combRptType2;
    CComboBox m_combRptType3;
    CComboBox m_combRptType4;
    CComboBox m_combRptType5;
    CComboBox m_combRptType6;

    CEdit m_editCtrlRptID1;
    CEdit m_editCtrlRptID2;
    CEdit m_editCtrlRptID3;
    CEdit m_editCtrlRptID4;
    CEdit m_editCtrlRptID5;
    CEdit m_editCtrlRptID6;

    CEdit m_editCtrlRptData1;
    CEdit m_editCtrlRptData2;
    CEdit m_editCtrlRptData3;
    CEdit m_editCtrlRptData4;
    CEdit m_editCtrlRptData5;
    CEdit m_editCtrlRptData6;

    CButton m_chkAutoLoopShut;

    CEdit m_editCtrlIntvShut;
    int m_msgIntvShut{ 0 };
    CEdit m_editCtrlTmoutShut;
    int m_msgTmoutShut{ 0 };

    CButton m_btnSendShut;
    CButton m_btnStopShut;
    afx_msg void OnBnClickedButtonSendShut();
    afx_msg void OnBnClickedButtonStopShut();

    // user defined
    usb::hid::UsbHidManager m_hidManager;
    std::vector<ShutCmdInfo> m_msgLists{};
    unsigned int m_cmdIndex{ 0 };
    unsigned int m_loopCounter{ 0 };

    void EnableReportInfoTab();
    void DisableReportInfoTab();

    void ShowDevDetailInfo();
    void ClearDevDetailInfo();

    void SetElemStatusWhenInit();    // init or closed
    void SetElemStatusWhenOpened();
    void SetElemStatusWhenSending();
    void SetElemStatusWhenStopped();

    void DialogInit();

    std::optional<ShutCmdInfo> CHidTestShutProto::GetMsgInfo(const CComboBox& combType, const CEdit& editRptId, const CEdit& editRptData);
    void GetMsgLists();
    void PrintMsgLists();

    afx_msg void OnTimer(UINT_PTR nIDEvent);
    bool IsMyDevice(LPARAM lParam);

    bool IsSetRptOrSetFeature(const CComboBox& combType);
    afx_msg void OnCbnSelchangeComboMsgType1();
    afx_msg void OnCbnSelchangeComboMsgType2();
    afx_msg void OnCbnSelchangeComboMsgType3();
    afx_msg void OnCbnSelchangeComboMsgType4();
    afx_msg void OnCbnSelchangeComboMsgType5();
    afx_msg void OnCbnSelchangeComboMsgType6();
};
