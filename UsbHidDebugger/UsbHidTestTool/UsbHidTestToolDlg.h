
// UsbHidTestToolDlg.h : header file
//

#pragma once

#include "HidTestQProto.h"
#include "HidTestShutProto.h"


// CUsbHidTestToolDlg dialog
class CUsbHidTestToolDlg : public CDialogEx
{
// Construction
public:
	CUsbHidTestToolDlg(CWnd* pParent = nullptr);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_USBHIDTESTTOOL_DIALOG };
#endif

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
    afx_msg void OnTcnSelchangeTabMain(NMHDR *pNMHDR, LRESULT *pResult);

	void Main_RegisterForDeviceNotifications();
	LRESULT Main_OnUsbHidDevChange(WPARAM wParam, LPARAM lParam); // device plug out detect

public:
    CTabCtrl m_tab_main;

    CHidTestQProto m_tab_qcmd;
    CHidTestShutProto m_tab_shut;
};
