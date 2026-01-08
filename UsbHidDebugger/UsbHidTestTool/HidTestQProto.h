#pragma once
#include <vector>
#include "UsbHidManager.h"


// CHidTestQProto dialog

class CHidTestQProto : public CDialogEx
{
	DECLARE_DYNAMIC(CHidTestQProto)

public:
	CHidTestQProto(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CHidTestQProto();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_QPROTO };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

    // my added ---------------->>>>>>>>
public:
    void SetElemStatusWhenInit(bool isClrLog = false);    // init or closed
    void SetElemStatusWhenOpened();
    void SetElemStatusWhenSending();
    void SetElemStatusWhenStopped();

    void ShowDevDetailInfo();
    void ClearDevDetailInfo();

    void DialogInit(bool isClrLog = false);

    CEdit m_editCtrlLogQ;
    CString m_editLogQ;
    afx_msg void OnBnClickedButtonClrlogQ();
    void ShowLog(CString str);

    CEdit m_editInputVidQ;
    CEdit m_editInputPidQ;
    unsigned short m_VidQ{ 0 };
    unsigned short m_PidQ{ 0 };

    CButton m_btnOpenDevQ;
    CButton m_btnCloseDevQ;
    afx_msg void OnBnClickedButtonOpenDevQ();
    afx_msg void OnBnClickedButtonCloseDevQ();

    CEdit m_editCtrlCmdQ;
    CButton m_BtnSendCmdQ;
    CButton m_BtnStopCmdQ;
    afx_msg void OnBnClickedButtonSendcmdQ();
    afx_msg void OnBnClickedButtonStopsendQ();

    CButton m_chkCtrlAutoR;
    CButton m_chkAutoLoop;

    CEdit m_editCtrlCmdIntvQ;
    int m_cmdIntvQ;

    CEdit m_editCtrlTmoutQ;
    int m_cmdTmoutQ;

    // pre-defined Q command
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
    afx_msg void OnBnClickedButtonAllQcmd();
    void EnableAllCmdChkbox();
    void DisableAllCmdChkbox();
    void SelectAllCmdChkbox();
    void UnselectAllCmdChkbox();

    usb::hid::UsbHidManager m_hidManager;

    std::vector<std::string> m_CmdLists{};
    unsigned int m_cmdIndex{ 0 };
    unsigned int m_loopCounter{ 0 };
    void GetChkboxCmdLists();
    void GetEditCtrlCmdLists();
    void GetCmdLists();

    afx_msg void OnTimer(UINT_PTR nIDEvent);

    bool IsMyDevice(LPARAM lParam);

};
