
// UsbHidDebuggerDlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "./logger/Logger.hpp"
#include "UsbHidDebugger.h"
#include "UsbHidDebuggerDlg.h"
#include "afxdialogex.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CUsbHidDebuggerDlg dialog



CUsbHidDebuggerDlg::CUsbHidDebuggerDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_USBHIDDEBUGGER_DIALOG, pParent)
    , m_editLog(_T(""))
    , m_Edit_ShowVid(_T(""))
    , m_Edit_ShowPid(_T(""))
    , m_Edit_ShowVer(_T(""))
    , m_Edit_ShowManu(_T(""))
    , m_Edit_ShowProd(_T(""))
    , m_Edit_ShowSN(_T(""))
    , m_editValueCmd(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CUsbHidDebuggerDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_COMBO_SELECTDEV, m_comboSelectDev);
    DDX_Control(pDX, IDC_BUTTON_OPENDEV, m_BtnOpenDev);
    DDX_Control(pDX, IDC_BUTTON_CLOSEDEV, m_BtnCloseDev);
    DDX_Text(pDX, IDC_EDIT_SHOWVID, m_Edit_ShowVid);
    DDX_Text(pDX, IDC_EDIT_SHOWPID, m_Edit_ShowPid);
    DDX_Text(pDX, IDC_EDIT_SHOWVER, m_Edit_ShowVer);
    DDX_Text(pDX, IDC_EDIT_SHOWMANU, m_Edit_ShowManu);
    DDX_Text(pDX, IDC_EDIT_SHOWPROD, m_Edit_ShowProd);
    DDX_Text(pDX, IDC_EDIT_SHOWSN, m_Edit_ShowSN);
    DDX_Control(pDX, IDC_EDIT_CMD, m_editCtrlCmd);
    DDX_Text(pDX, IDC_EDIT_CMD, m_editValueCmd);
    DDX_Control(pDX, IDC_BUTTON_SENDMSG, m_BtnSendCmd);
    DDX_Control(pDX, IDC_BUTTON_STOPSEND, m_BtnStopSend);
    DDX_Control(pDX, IDC_CHECK_AUTO_R, m_chkCtrlAutoR);
    DDX_Control(pDX, IDC_CHECK_CMDFWV, m_chkCmdFwv);
    DDX_Control(pDX, IDC_CHECK_CMDUDESC, m_chkCmdUdesc);
    DDX_Control(pDX, IDC_CHECK_CMDUAS, m_chkCmdUas);
    DDX_Control(pDX, IDC_CHECK_CMDBL, m_chkCmdBl);
    DDX_Control(pDX, IDC_CHECK_CMDQ1, m_chkCmdQ1);
    DDX_Control(pDX, IDC_CHECK_CMDPSNR, m_chkCmdPsnr);
    DDX_Control(pDX, IDC_CHECK_CMDCF, m_chkCmdCf);
    DDX_Control(pDX, IDC_CHECK_CMDSI, m_chkCmdSi);
    DDX_Control(pDX, IDC_CHECK_CMDBRR, m_chkCmdBrr);
    DDX_Control(pDX, IDC_CHECK_CMDLF, m_chkCmdLf);
    DDX_Control(pDX, IDC_BUTTON_ALLCMD, m_btnCmdAll);
    DDX_Control(pDX, IDC_CHECK1_AUTOLOOP, m_chkAutoLoop);
    DDX_Control(pDX, IDC_EDIT_LOG, m_editCtrlLog);
    DDX_Control(pDX, IDC_EDIT_INTERVAL, m_editCtrlCmdIntervals);
}

BEGIN_MESSAGE_MAP(CUsbHidDebuggerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
    ON_BN_CLICKED(IDC_BUTTON_CLRLOG, &CUsbHidDebuggerDlg::OnBnClickedButtonClrlog)
    ON_BN_CLICKED(IDC_BUTTON_OPENDEV, &CUsbHidDebuggerDlg::OnBnClickedButtonOpendev)
    ON_BN_CLICKED(IDC_BUTTON_CLOSEDEV, &CUsbHidDebuggerDlg::OnBnClickedButtonClosedev)
    ON_BN_CLICKED(IDC_BUTTON_SENDMSG, &CUsbHidDebuggerDlg::OnBnClickedButtonSendmsg)
    ON_BN_CLICKED(IDC_BUTTON_STOPSEND, &CUsbHidDebuggerDlg::OnBnClickedButtonStopsend)
    ON_BN_CLICKED(IDC_BUTTON_ALLCMD, &CUsbHidDebuggerDlg::OnBnClickedButtonAllcmd)
    ON_WM_TIMER()
END_MESSAGE_MAP()


// CUsbHidDebuggerDlg message handlers
void CUsbHidDebuggerDlg::showLog(CString str)
{
    m_editCtrlLog.LineScroll(m_editCtrlLog.GetLineCount());

    const size_t  maxLogSize{65535};
    if (m_editLog.GetLength() >= maxLogSize)
    {
        m_editLog = _T("");
    } 

    CString s = str + _T("\r\n");
    m_editLog += s;
    m_editCtrlLog.SetWindowText(m_editLog);
    m_editCtrlLog.LineScroll(m_editCtrlLog.GetLineCount());
}

#define SHOW_LOG(fmt, ...)   do{                                      \
	                              CString cs;                         \
	                              cs.Format(_T(fmt), __VA_ARGS__);    \
	                              showLog(cs);                        \
                            }while(0)

void CUsbHidDebuggerDlg::BuildDevDroplist()
{
    m_hidManager.RefreshDevlist();
	size_t devCount = m_hidManager.GetDevlistCount();
	for(size_t i=0; i < devCount; i++)
	{
		auto dev = m_hidManager.GetCurDevInfo(i);
		if (!dev.has_value()) {
			continue;
		}

		CString cstr = _T("Vid:0x") + m_hidManager.toHexStr(dev.value().Vid) + _T(", ") + _T("Pid:0x") + m_hidManager.toHexStr(dev.value().Pid) + _T(", ") + _T("Ver:") + m_hidManager.toHexStr(dev.value().Ver);
		m_comboSelectDev.AddString(cstr);
		SHOW_LOG("Device found: %s", cstr);
	}
	SHOW_LOG("Device list refresh OK, total %d devices found.", devCount);
    log_info("Device list refresh OK, total " << devCount << " devices found.");
}

BOOL CUsbHidDebuggerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
    SHOW_LOG("initializing......");
    log_info("initializing......");
    BuildDevDroplist();
	m_BtnOpenDev.EnableWindow(1);
	m_BtnCloseDev.EnableWindow(0);
	m_editCtrlCmd.EnableWindow(0);
	m_BtnSendCmd.EnableWindow(0);
	m_BtnStopSend.EnableWindow(0);
	m_chkCtrlAutoR.EnableWindow(0);
    m_chkAutoLoop.EnableWindow(0);
    m_editCtrlCmdIntervals.EnableWindow(0);
    DisableAllCmdChkbox();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CUsbHidDebuggerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CUsbHidDebuggerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CUsbHidDebuggerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CUsbHidDebuggerDlg::OnBnClickedButtonClrlog()
{
    // TODO: Add your control notification handler code here
    m_editLog = _T("");
    m_editCtrlLog.SetWindowText(m_editLog);
}

void CUsbHidDebuggerDlg::ShowDevDetailInfo()
{
    m_Edit_ShowVid = _T("0x") + m_hidManager.GetVidStrHex();
    m_Edit_ShowPid = _T("0x") + m_hidManager.GetPidStrHex();
    m_Edit_ShowVer = _T("0x") + m_hidManager.GetVerStrHex();
    auto dev = m_hidManager.GetCurDevInfo();
    unsigned short inRptSz{ 0 };
    unsigned short OutRptSz{ 0 };
    if (dev.has_value())
    {
        m_Edit_ShowManu = dev.value().manufactureStr;
        m_Edit_ShowProd = dev.value().productStr;
        m_Edit_ShowSN = dev.value().serialNumStr;
        inRptSz = dev.value().InRptBytesLen;
        OutRptSz = dev.value().OutRptBytesLen;
    }
    UpdateData(FALSE);

	SHOW_LOG("\r\nSelected device:\r\nVid:%s\r\nPid:%s\r\nVer:%s\r\nManufacture:%s\r\nProduct:%s\r\nSerialNumber:%s\r\nInputReportSize:%d\r\nOutputReportSize:%d\r\n", 
		      m_Edit_ShowVid,
              m_Edit_ShowPid,
              m_Edit_ShowVer,
              m_Edit_ShowManu,
			  m_Edit_ShowProd,
		      m_Edit_ShowSN,
              inRptSz,
              OutRptSz);
	log_info("Selected device: Vid=" << m_Edit_ShowVid << ", Pid= " << m_Edit_ShowPid << ", Ver=" << m_Edit_ShowVer << ", Manufacture:" << m_Edit_ShowManu \
              << ", Product:" << m_Edit_ShowProd << ", SerialNumber:" << m_Edit_ShowSN << ", InputReportSize=" << inRptSz << ", OutputReportSize=" << OutRptSz); 
}

void CUsbHidDebuggerDlg::UnshowDevDetailInfo()
{
    m_Edit_ShowVid  = _T("");
    m_Edit_ShowPid  = _T("");
    m_Edit_ShowVer  = _T("");
    m_Edit_ShowManu = _T("");
    m_Edit_ShowProd = _T("");
    m_Edit_ShowSN   = _T("");
    UpdateData(FALSE);
}

void CUsbHidDebuggerDlg::OnBnClickedButtonOpendev()
{
    // TODO: Add your control notification handler code here
    int devIdx = m_comboSelectDev.GetCurSel();
	SHOW_LOG("Selected Combo box index: %d of totoal %d devices.", devIdx, m_hidManager.GetDevlistCount());
    log_info("Selected Combo box index: " << devIdx << " of totoal " << m_hidManager.GetDevlistCount() << " devices.");
    if (CB_ERR == devIdx) {
        (void)MessageBox(TEXT("Please select a HID device!"), TEXT("Waring"), MB_YESNO | MB_ICONQUESTION);
        log_info("No HID device selected.");
        return;
    }

    if (devIdx < 0 || devIdx >= m_hidManager.GetDevlistCount())
    {
        (void)MessageBox(TEXT("Invalid selected HID device!"), TEXT("Waring"), MB_YESNO | MB_ICONQUESTION);
        log_info("Invalid HID device, index=" << devIdx);
        return;
    }

    m_hidManager.SetTargetDev(devIdx);
    if (!m_hidManager.OpenTargetDev())
    {
        (void)MessageBox(TEXT("Open device failed!"), TEXT("Waring"), MB_YESNO | MB_ICONQUESTION);
        log_info("Open device failed!");
        return;
    }
    log_info("Open HID device OK!");

    ShowDevDetailInfo();

    m_BtnOpenDev.EnableWindow(0);
    m_BtnCloseDev.EnableWindow(1);
	m_editCtrlCmd.EnableWindow(1);
	m_BtnSendCmd.EnableWindow(1);
	m_BtnStopSend.EnableWindow(0);
	m_chkCtrlAutoR.EnableWindow(1);
    m_chkAutoLoop.EnableWindow(1);
    m_editCtrlCmdIntervals.EnableWindow(1);
    EnableAllCmdChkbox();
}


void CUsbHidDebuggerDlg::OnBnClickedButtonClosedev()
{
    // TODO: Add your control notification handler code here
    m_isStopCmd = true;

    m_hidManager.CloseCurDev();
    UnshowDevDetailInfo();

    m_BtnOpenDev.EnableWindow(1);
    m_BtnCloseDev.EnableWindow(0);
	m_editCtrlCmd.EnableWindow(0);
	m_BtnSendCmd.EnableWindow(0);
	m_BtnStopSend.EnableWindow(0);
	m_chkCtrlAutoR.EnableWindow(0);
    m_chkAutoLoop.EnableWindow(0);
    m_editCtrlCmdIntervals.EnableWindow(0);
    DisableAllCmdChkbox();
}


void CUsbHidDebuggerDlg::OnBnClickedButtonSendmsg()
{
    // TODO: Add your control notification handler code here
    m_isStopCmd = false;

    GetCmdLists();
    for (auto& cmd : m_CmdLists)
    {
        log_info("cmd: " << cmd);
    }

    m_editCtrlCmd.EnableWindow(0);
    m_BtnSendCmd.EnableWindow(0);
    m_BtnStopSend.EnableWindow(1);
	m_chkCtrlAutoR.EnableWindow(0);
    m_chkAutoLoop.EnableWindow(0);
    m_editCtrlCmdIntervals.EnableWindow(0);
    DisableAllCmdChkbox();

    CString csint;
    m_editCtrlCmdIntervals.GetWindowText(csint);
    int interval = _ttoi(csint);
    interval = interval > 0 ? interval : 500;  // 500ms as default

    SetTimer(TIMER_HID_RX, interval, NULL);

    m_cmdIndex = 0;
    std::string cmd = m_CmdLists[m_cmdIndex];
    m_hidManager.SendCmds(cmd);

    CString cstr(cmd.c_str());
    SHOW_LOG("Command: %s", cstr);

}


void CUsbHidDebuggerDlg::OnBnClickedButtonStopsend()
{
    // TODO: Add your control notification handler code here
    m_isStopCmd = true;

    m_editCtrlCmd.EnableWindow(1);
    m_BtnSendCmd.EnableWindow(1);
    m_BtnStopSend.EnableWindow(0);
	m_chkCtrlAutoR.EnableWindow(1);
    m_chkAutoLoop.EnableWindow(1);
    m_editCtrlCmdIntervals.EnableWindow(1);
    EnableAllCmdChkbox();
}


void CUsbHidDebuggerDlg::OnBnClickedButtonAllcmd()
{
    // TODO: Add your control notification handler code here
    CString strCurrTxt;
    m_btnCmdAll.GetWindowText(strCurrTxt);
    if (strCurrTxt == "Select All")
    {
        m_chkCmdFwv.SetCheck(TRUE);
        m_chkCmdUdesc.SetCheck(TRUE);
        m_chkCmdUas.SetCheck(TRUE);
        m_chkCmdBl.SetCheck(TRUE);
        m_chkCmdQ1.SetCheck(TRUE);
        m_chkCmdPsnr.SetCheck(TRUE);
        m_chkCmdCf.SetCheck(TRUE);
        m_chkCmdSi.SetCheck(TRUE);
        m_chkCmdBrr.SetCheck(TRUE);
        m_chkCmdLf.SetCheck(TRUE);
        m_btnCmdAll.SetWindowText(L"Clear All");
    }
    else
    {
        m_chkCmdFwv.SetCheck(FALSE);
        m_chkCmdUdesc.SetCheck(FALSE);
        m_chkCmdUas.SetCheck(FALSE);
        m_chkCmdBl.SetCheck(FALSE);
        m_chkCmdQ1.SetCheck(FALSE);
        m_chkCmdPsnr.SetCheck(FALSE);
        m_chkCmdCf.SetCheck(FALSE);
        m_chkCmdSi.SetCheck(FALSE);
        m_chkCmdBrr.SetCheck(FALSE);
        m_chkCmdLf.SetCheck(FALSE);
        m_btnCmdAll.SetWindowText(L"Select All");
    }
}

void CUsbHidDebuggerDlg::EnableAllCmdChkbox()
{
    m_chkCmdFwv.EnableWindow(1);
    m_chkCmdUdesc.EnableWindow(1);
    m_chkCmdUas.EnableWindow(1);
    m_chkCmdBl.EnableWindow(1);
    m_chkCmdQ1.EnableWindow(1);
    m_chkCmdPsnr.EnableWindow(1);
    m_chkCmdCf.EnableWindow(1);
    m_chkCmdSi.EnableWindow(1);
    m_chkCmdBrr.EnableWindow(1);
    m_chkCmdLf.EnableWindow(1);
    m_btnCmdAll.EnableWindow(1);
}

void CUsbHidDebuggerDlg::DisableAllCmdChkbox()
{
    m_chkCmdFwv.EnableWindow(0);
    m_chkCmdUdesc.EnableWindow(0);
    m_chkCmdUas.EnableWindow(0);
    m_chkCmdBl.EnableWindow(0);
    m_chkCmdQ1.EnableWindow(0);
    m_chkCmdPsnr.EnableWindow(0);
    m_chkCmdCf.EnableWindow(0);
    m_chkCmdSi.EnableWindow(0);
    m_chkCmdBrr.EnableWindow(0);
    m_chkCmdLf.EnableWindow(0);
    m_btnCmdAll.EnableWindow(0);
}

void CUsbHidDebuggerDlg::GetChkboxCmdLists()
{
    if (m_chkCmdFwv.GetCheck())     { m_CmdLists.push_back("FWV\r"); }
    if (m_chkCmdUdesc.GetCheck())   { m_CmdLists.push_back("UDESC\r"); }
    if (m_chkCmdUas.GetCheck())     { m_CmdLists.push_back("UAS\r"); }
    if (m_chkCmdBl.GetCheck())      { m_CmdLists.push_back("Bl\r"); }
    if (m_chkCmdQ1.GetCheck())      { m_CmdLists.push_back("Q1\r"); }
    if (m_chkCmdPsnr.GetCheck())    { m_CmdLists.push_back("PSNR\r"); }
    if (m_chkCmdCf.GetCheck())      { m_CmdLists.push_back("CF\r"); }
    if (m_chkCmdSi.GetCheck())      { m_CmdLists.push_back("Si ?\r"); }
    if (m_chkCmdBrr.GetCheck())     { m_CmdLists.push_back("BRR\r"); }
    if (m_chkCmdLf.GetCheck())      { m_CmdLists.push_back("Lf\r"); }
}

void CUsbHidDebuggerDlg::GetEditCtrlCmdLists()
{
    CString editCtrlCmdStr;
    m_editCtrlCmd.GetWindowText(editCtrlCmdStr);
    editCtrlCmdStr.Trim();

    if (editCtrlCmdStr.IsEmpty())   // empty
    {
        return;
    }

	while (0 == editCtrlCmdStr.Find(_T("\r\n")))
	{
		editCtrlCmdStr.Delete(0, CString{_T("\r\n")}.GetLength());
    }

    BOOL isAutoR = m_chkCtrlAutoR.GetCheck();
    log_info("Auto add \\r: " << isAutoR);

    if (editCtrlCmdStr.Find(_T("\r\n"), 0) < 0)    // single line
    {
        CT2CA pszConvertedAnsiString(editCtrlCmdStr);
        std::string cmd(pszConvertedAnsiString);
        if (isAutoR) {
            cmd += "\r";
        }
        m_CmdLists.push_back(cmd);
        return;
    }

    editCtrlCmdStr += _T("\r\n");
    int idx = 0;
    while (idx = editCtrlCmdStr.Find(_T("\r\n"), 0) >= 0)
    {
        CString cstr = editCtrlCmdStr.Left(idx+1);
        if (cstr.IsEmpty())
        {
            continue;
        }
        CT2CA pszConvertedAnsiString(cstr);
        std::string cmd(pszConvertedAnsiString);
        if (isAutoR) {
            cmd += "\r";
        }
        m_CmdLists.push_back(cmd);

        editCtrlCmdStr.Delete(0, idx + 1 + CString{_T("\r\n")}.GetLength());
    }
}

void CUsbHidDebuggerDlg::GetCmdLists()
{
    m_CmdLists.clear();
    GetChkboxCmdLists();
    if (!m_CmdLists.empty())
    {
        return;
    }
    GetEditCtrlCmdLists();
}

void CUsbHidDebuggerDlg::OnTimer(UINT_PTR nIDEvent)
{
    // TODO: Add your message handler code here and/or call default
    switch(nIDEvent) 
    {
    //Wait boot device attach timer
    case TIMER_HID_RX:
        {
            // recv last response
            char msgRsp[255] = {0};
            m_hidManager.RecvRsp(&(msgRsp[0]), sizeof(msgRsp));
            if (msgRsp[1]) {
                std::string cmd = &msgRsp[1];
                CString cstr(cmd.c_str());
                SHOW_LOG("Response: %s", cstr);
            } else {
                SHOW_LOG("Response: ---");
            }

            if (m_isStopCmd) {
                m_cmdIndex = 0;
                KillTimer(TIMER_HID_RX);
                break;
            }

            // send next command
            m_cmdIndex++;
            if (m_cmdIndex < m_CmdLists.size()) {
                std::string cmd = m_CmdLists[m_cmdIndex];
                m_hidManager.SendCmds(cmd);
                CString cstr(cmd.c_str());
                SHOW_LOG("Command: %s", cstr);
            } else {
                if (m_chkAutoLoop.GetCheck())
                {
                    m_cmdIndex = 0;
                    std::string cmd = m_CmdLists[m_cmdIndex];
                    m_hidManager.SendCmds(cmd);
                    CString cstr(cmd.c_str());
                    SHOW_LOG("Command: %s", cstr);
                } else {
                    m_cmdIndex = 0;
                    KillTimer(TIMER_HID_RX);
                    //
                    m_editCtrlCmd.EnableWindow(1);
                    m_BtnSendCmd.EnableWindow(1);
                    m_BtnStopSend.EnableWindow(0);
                    m_chkCtrlAutoR.EnableWindow(1);
                    m_chkAutoLoop.EnableWindow(1);
                    m_editCtrlCmdIntervals.EnableWindow(1);
                    EnableAllCmdChkbox();
                }
            }
        }

        break;
    default:
        break;
    }

    CDialogEx::OnTimer(nIDEvent);
}
