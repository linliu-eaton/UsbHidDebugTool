
// UsbHidDebuggerDlg.h : header file
//

#pragma once

#include <atomic>
#include <vector>
#include "UsbHidManager.h"

#define TIMER_CHK_RSP_INNER     100
#define TIMER_CMD_INTERV        300

#define TIMER_CHK_RSP_INNER_TIMEOUT  5   // ms


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


    void RegisterForDeviceNotifications();

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
    LRESULT OnUsbHidDevChange(WPARAM wParam, LPARAM lParam);

    usb::hid::UsbHidManager m_hidManager;

    CEdit m_editCtrlLog;
    CString m_editLog;
    afx_msg void OnBnClickedButtonClrlog();

    CButton m_BtnOpenDev;
    CButton m_BtnCloseDev;
    afx_msg void OnBnClickedButtonOpendev();
    afx_msg void OnBnClickedButtonClosedev();

    CString m_Edit_ShowVid;
    CString m_Edit_ShowPid;
    CString m_Edit_ShowVer;
    CString m_Edit_ShowManu;
    CString m_Edit_ShowProd;
    CString m_Edit_ShowSN;

    CEdit m_editCtrlCmd;
    CButton m_BtnSendCmd;
    CButton m_BtnStopSend;
    afx_msg void OnBnClickedButtonSendmsg();
    afx_msg void OnBnClickedButtonStopsend();
    CButton m_chkCtrlAutoR;
    CButton m_chkAutoLoop;
    CEdit m_editCtrlCmdIntervals;
    int m_cmdInterval;
    CEdit m_editTimeout;
    int m_cmdTimeout;

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
    unsigned int m_cmdIndex{ 0 };
    unsigned int m_loopCounter{ 0 };
    void GetChkboxCmdLists();
    void GetEditCtrlCmdLists();
    void GetCmdLists();

    afx_msg void OnTimer(UINT_PTR nIDEvent);

    CEdit m_EditInputVid;
    CEdit m_EditInputPid;
    unsigned short m_Vid{0};
    unsigned short m_Pid{0};

    bool m_isDevClosed{true};

};
