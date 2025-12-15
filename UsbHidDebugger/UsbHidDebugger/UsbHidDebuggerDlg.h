
// UsbHidDebuggerDlg.h : header file
//

#pragma once

#include <atomic>
#include <vector>
#include "UsbHidManager.h"


#define TIMER_HID_RX  100

// CUsbHidDebuggerDlg dialog
class CUsbHidDebuggerDlg : public CDialogEx
{
// Construction
public:
	CUsbHidDebuggerDlg(CWnd* pParent = nullptr);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_USBHIDDEBUGGER_DIALOG };
#endif

    void showLog(CString str);
    void BuildDevDroplist();
    void ShowDevDetailInfo();
    void UnshowDevDetailInfo();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
    usb::hid::UsbHidManager m_hidManager;
    CEdit m_editCtrlLog;
    CString m_editLog;
    CComboBox m_comboSelectDev;
    afx_msg void OnBnClickedButtonClrlog();
    CButton m_BtnOpenDev;
    CButton m_BtnCloseDev;
    afx_msg void OnBnClickedButtonOpendev();

    CString m_Edit_ShowVid;
    CString m_Edit_ShowPid;
    CString m_Edit_ShowVer;
    CString m_Edit_ShowManu;
    CString m_Edit_ShowProd;
    CString m_Edit_ShowSN;
    afx_msg void OnBnClickedButtonClosedev();

    CEdit m_editCtrlCmd;
    CButton m_BtnSendCmd;
    CButton m_BtnStopSend;
    afx_msg void OnBnClickedButtonSendmsg();
    afx_msg void OnBnClickedButtonStopsend();
    CButton m_chkCtrlAutoR;

    CButton m_chkCmdFwv;
    CButton m_chkCmdUdesc;
    CButton m_chkCmdUas;
    CButton m_chkCmdBl;
    CButton m_chkCmdQ1;
    CButton m_chkCmdPsnr;
    CButton m_chkCmdCf;
    CButton m_chkCmdSi;
    CButton m_chkCmdBrr;
    CButton m_chkCmdLf;
    CButton m_btnCmdAll;
    afx_msg void OnBnClickedButtonAllcmd();
    void EnableAllCmdChkbox();
    void DisableAllCmdChkbox();

    std::vector<std::string> m_CmdLists{};
    void GetChkboxCmdLists();
    void GetEditCtrlCmdLists();
    void GetCmdLists();

    CButton m_chkAutoLoop;
    CEdit m_editCtrlCmdIntervals;
    afx_msg void OnTimer(UINT_PTR nIDEvent);

    unsigned int m_cmdIndex{ 0 };
    std::atomic<bool> m_isStopCmd{false};

    CEdit m_EditInputVid;
    CEdit m_EditInputPid;
    unsigned short m_Vid{0};
    unsigned short m_Pid{0};
};
