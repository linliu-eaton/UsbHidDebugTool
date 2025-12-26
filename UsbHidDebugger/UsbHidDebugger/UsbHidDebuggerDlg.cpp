
// UsbHidDebuggerDlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "./logger/Logger.hpp"
#include "UsbHidDebugger.h"
#include "UsbHidDebuggerDlg.h"
#include "afxdialogex.h"
#include <string>
#include <cmath>
#include <chrono>
#include <iomanip>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

namespace {
    #define INVALID_VALUE -1

    std::string getLocalTime()
    {
        const std::string TIME_FORMAT{ "%Y-%m-%d %H:%M:%S" };
        std::stringstream ss;
        auto now = std::chrono::system_clock::now();
        uint64_t ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count()
            - std::chrono::duration_cast<std::chrono::seconds>(now.time_since_epoch()).count() * 1000;

        time_t t = std::chrono::system_clock::to_time_t(now);
        struct tm st;
        localtime_s(&st, &t);
        ss << std::put_time(&st, TIME_FORMAT.c_str());
        ss << "." << ms;
        std::string tmStr = ss.str();
        return tmStr;
    }

    int CstringHex2Int(CString& cstr)
    {
        cstr.MakeUpper();
        cstr.Trim();
        cstr.Trim(_T("\r\n"));
        cstr.TrimLeft(_T("0X"));
        if (cstr.IsEmpty())   // empty
        {
            return INVALID_VALUE;
        }
    
        CT2CA pszConvertedAnsiString(cstr);
        std::string strs(pszConvertedAnsiString);
        std::reverse(strs.begin(), strs.end());
        int vid = 0;
        for (size_t i=0; i<strs.length(); i++) {
            if (strs[i] >= '0' && strs[i] <= '9') {
                vid += (strs[i] - 0x30) * (int)std::pow(16, i);
            } else if ((strs[i] >= 'A' && strs[i] <='F')) {
                vid += (strs[i] - 0x37) * (int)std::pow(16, i);
            } else {
                return INVALID_VALUE;
            }
        }

        return vid;
    }
}


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
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CUsbHidDebuggerDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_BUTTON_OPENDEV, m_BtnOpenDev);
    DDX_Control(pDX, IDC_BUTTON_CLOSEDEV, m_BtnCloseDev);

    DDX_Text(pDX, IDC_EDIT_SHOWVID, m_Edit_ShowVid);
    DDX_Text(pDX, IDC_EDIT_SHOWPID, m_Edit_ShowPid);
    DDX_Text(pDX, IDC_EDIT_SHOWVER, m_Edit_ShowVer);
    DDX_Text(pDX, IDC_EDIT_SHOWMANU, m_Edit_ShowManu);
    DDX_Text(pDX, IDC_EDIT_SHOWPROD, m_Edit_ShowProd);
    DDX_Text(pDX, IDC_EDIT_SHOWSN, m_Edit_ShowSN);

    DDX_Control(pDX, IDC_BUTTON_SENDMSG, m_BtnSendCmd);
    DDX_Control(pDX, IDC_BUTTON_STOPSEND, m_BtnStopSend);

    DDX_Control(pDX, IDC_EDIT_CMD, m_editCtrlCmd);
    DDX_Control(pDX, IDC_CHECK_AUTO_R, m_chkCtrlAutoR);
    DDX_Control(pDX, IDC_CHECK1_AUTOLOOP, m_chkAutoLoop);
    DDX_Control(pDX, IDC_EDIT_INTERVAL, m_editCtrlCmdIntervals);

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

    DDX_Control(pDX, IDC_EDIT_LOG, m_editCtrlLog);

    DDX_Control(pDX, IDC_EDIT__IN_VID, m_EditInputVid);
    DDX_Control(pDX, IDC_EDIT__IN_PID, m_EditInputPid);
    DDX_Control(pDX, IDC_EDIT_TIMEOUT, m_editTimeout);
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
    ON_MESSAGE(WM_DEVICECHANGE, OnUsbHidDevChange)
END_MESSAGE_MAP()


// CUsbHidDebuggerDlg message handlers
void CUsbHidDebuggerDlg::showLog(CString str)
{
    m_editCtrlLog.LineScroll(m_editCtrlLog.GetLineCount());

    const size_t  maxLogSize{20 * 1024 * 1024};
    if (m_editLog.GetLength() >= maxLogSize)
    {
        m_editLog = _T("");
    }

    std::string now = getLocalTime();
    CString nowTime(now.c_str());

    CString s = nowTime + _T(" ") + str + _T("\r\n");
    m_editLog += s;
    m_editCtrlLog.SetWindowText(m_editLog);
    m_editCtrlLog.LineScroll(m_editCtrlLog.GetLineCount());
}

#define SHOW_LOG(fmt, ...)   do{                                      \
	                              CString cs;                         \
	                              cs.Format(_T(fmt), __VA_ARGS__);    \
	                              showLog(cs);                        \
                            }while(0)

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
    SHOW_LOG("---------------------------------------------------");
    log_info("---------------------------------------------------");
    
    m_EditInputVid.EnableWindow(1);
    m_EditInputPid.EnableWindow(1);
	m_BtnOpenDev.EnableWindow(1);
    
	m_BtnCloseDev.EnableWindow(0);
	m_editCtrlCmd.EnableWindow(0);
	m_BtnSendCmd.EnableWindow(0);
	m_BtnStopSend.EnableWindow(0);
	m_chkCtrlAutoR.EnableWindow(0);
    m_chkAutoLoop.EnableWindow(0);
    m_editCtrlCmdIntervals.EnableWindow(0);
    m_editTimeout.EnableWindow(0);
    DisableAllCmdChkbox();
    m_isDevClosed = true;

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
    m_Edit_ShowManu = m_hidManager.GetManufactureStr();
    m_Edit_ShowProd = m_hidManager.GetProductStr();
    m_Edit_ShowSN = m_hidManager.GetSerialNum();
    UpdateData(FALSE);

	SHOW_LOG("\r\nDevice:\r\nVid:%s\r\nPid:%s\r\nVer:%s\r\nManufacture:%s\r\nProduct:%s\r\nSerialNumber:%s\r\nInputReportSize:%d\r\nOutputReportSize:%d\r\n", 
		      m_Edit_ShowVid,
              m_Edit_ShowPid,
              m_Edit_ShowVer,
              m_Edit_ShowManu,
			  m_Edit_ShowProd,
		      m_Edit_ShowSN,
              m_hidManager.InReportSize() - 1,
              m_hidManager.OutReportSize() - 1);
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

void CUsbHidDebuggerDlg::RegisterForDeviceNotifications()
{
    log_info("register device plugout notification...");
	// Request to receive messages when a device is attached or removed.
	// Also see WM_DEVICECHANGE in BEGIN_MESSAGE_MAP(CUsbhidiocDlg, CDialog).

    GUID hidGuid;
    HidD_GetHidGuid(&hidGuid);

	DEV_BROADCAST_DEVICEINTERFACE DevBroadcastDeviceInterface;

	DevBroadcastDeviceInterface.dbcc_size = sizeof(DevBroadcastDeviceInterface);
	DevBroadcastDeviceInterface.dbcc_devicetype = DBT_DEVTYP_DEVICEINTERFACE;
	DevBroadcastDeviceInterface.dbcc_classguid = hidGuid;

	RegisterDeviceNotification(m_hWnd, &DevBroadcastDeviceInterface, DEVICE_NOTIFY_WINDOW_HANDLE);
}

LRESULT CUsbHidDebuggerDlg::OnUsbHidDevChange(WPARAM wParam, LPARAM lParam)
{
	PDEV_BROADCAST_HDR lpdb = (PDEV_BROADCAST_HDR)lParam;
 
	switch(wParam) 
	{
		// Find out if a device has been attached or removed.
		// If yes, see if the name matches the device path name of the device we want to access.
 
	case DBT_DEVICEARRIVAL:		        // HID pull in
        log_info("device plugin, do nothing...");
		return TRUE; 
 
	case DBT_DEVICEREMOVECOMPLETE:		// HID pull out
        // Is my device?
        if (m_hidManager.IsMyDevice(lParam)) {
            if (m_isDevClosed) {
                return TRUE;
            }
            
            OnBnClickedButtonClosedev();
            SHOW_LOG("Device has been unplugged!");
            log_info("a device plugout, It is my device.");
            return TRUE;
        }
        log_info("a device plugout, not my device.");
		return TRUE; 
 
	default:
		return TRUE; 
	}

	return TRUE;
}

void CUsbHidDebuggerDlg::OnBnClickedButtonOpendev()
{
    CString editVidStr;
    m_EditInputVid.GetWindowText(editVidStr);
    SHOW_LOG("VID=%s", editVidStr);
    int vid = CstringHex2Int(editVidStr);
    log_info("VID:" << vid);
    if (vid < 0)
    {
        (void)MessageBox(_T("Invalid VID!"), _T("Waring"), MB_OK);
        return;
    }

    CString editPidStr;
    m_EditInputPid.GetWindowText(editPidStr);
    SHOW_LOG("PID=%s", editPidStr);
    int pid = CstringHex2Int(editPidStr);
    log_info("PID:" << pid);
    if (pid < 0)
    {
        (void)MessageBox(_T("Invalid PID!"), _T("Waring"), MB_OK);
        return;
    }

    if (!m_hidManager.OpenHidDev(vid, pid))
    {
        (void)MessageBox(_T("Open device failed!"), _T("Waring"), MB_OK);
        return;
    }
    SHOW_LOG("Open HID device OK!");
    log_info("Open HID device OK!");
    m_Vid = vid;
    m_Pid = pid;

    ShowDevDetailInfo();

    RegisterForDeviceNotifications();  // register hid dev plutout notification
    m_isDevClosed = false;

    m_EditInputVid.EnableWindow(0);
    m_EditInputPid.EnableWindow(0);
    m_BtnOpenDev.EnableWindow(0);
    m_BtnCloseDev.EnableWindow(1);
	m_editCtrlCmd.EnableWindow(1);
	m_BtnSendCmd.EnableWindow(1);
	m_BtnStopSend.EnableWindow(0);
	m_chkCtrlAutoR.EnableWindow(1);
    m_chkAutoLoop.EnableWindow(1);
    m_editCtrlCmdIntervals.EnableWindow(1);
    m_editTimeout.EnableWindow(1);
    EnableAllCmdChkbox();
}


void CUsbHidDebuggerDlg::OnBnClickedButtonClosedev()
{
    // TODO: Add your control notification handler code here
    m_cmdIndex = 0;
    m_loopCounter = 0;
    KillTimer(TIMER_CHK_RSP_INNER);
    KillTimer(TIMER_CMD_INTERV);

    m_hidManager.CloseHidDev();
    UnshowDevDetailInfo();

    m_EditInputVid.EnableWindow(1);
    m_EditInputPid.EnableWindow(1);
    m_BtnOpenDev.EnableWindow(1);
    m_BtnCloseDev.EnableWindow(0);
	m_editCtrlCmd.EnableWindow(0);
	m_BtnSendCmd.EnableWindow(0);
	m_BtnStopSend.EnableWindow(0);
	m_chkCtrlAutoR.EnableWindow(0);
    m_chkAutoLoop.EnableWindow(0);
    m_editCtrlCmdIntervals.EnableWindow(0);
    m_editTimeout.EnableWindow(0);
    DisableAllCmdChkbox();
    m_isDevClosed = true;
}


void CUsbHidDebuggerDlg::OnBnClickedButtonSendmsg()
{
    // TODO: Add your control notification handler code here
    GetCmdLists();
    for (auto& cmd : m_CmdLists)
    {
        log_info("cmd: " << cmd);
    }

    if (m_CmdLists.size() <= 0) {
        (void)MessageBox(_T("No command to send!"), _T("Waring"), MB_OK);
        return;
    }

    m_editCtrlCmd.EnableWindow(0);
    m_BtnSendCmd.EnableWindow(0);
    m_BtnStopSend.EnableWindow(1);
	m_chkCtrlAutoR.EnableWindow(0);
    m_chkAutoLoop.EnableWindow(0);
    m_editCtrlCmdIntervals.EnableWindow(0);
    m_editTimeout.EnableWindow(0);
    DisableAllCmdChkbox();

    CString csint;
    m_editCtrlCmdIntervals.GetWindowText(csint);
    SHOW_LOG("Recv cmd timeout: %s", csint);
    int interval = _ttoi(csint);
    m_cmdInterval = interval > 0 ? interval : DEFAULT_CMD_INTERV_MS;
    log_info("Recv cmd timeout: " << m_cmdInterval);
    CString feedInterv;
    feedInterv.Format(_T("%d"), m_cmdInterval);
    m_editCtrlCmdIntervals.SetWindowText(feedInterv);

    CString cstmout;
    m_editTimeout.GetWindowText(cstmout);
    SHOW_LOG("Send cmd interval: %s", cstmout);
    int tmout = _ttoi(cstmout);
    m_cmdTimeout = tmout > DEFAULT_CMD_RECV_TIMEOUT_MS ? tmout : DEFAULT_CMD_RECV_TIMEOUT_MS;
    log_info("Send cmd interval: " << m_cmdTimeout);
    CString feedTmout;
    feedTmout.Format(_T("%d"), m_cmdTimeout);
    m_editTimeout.SetWindowText(feedTmout);

    SetTimer(TIMER_CHK_RSP_INNER, TIMER_CHK_RSP_INNER_TIMEOUT, NULL);

    m_cmdIndex = 0;
    m_loopCounter = 0;
    SHOW_LOG("-------------------loop %d -------------------", m_loopCounter);
    log_info("-------------------loop " << m_loopCounter << " -------------------");
    std::string cmd = m_CmdLists[m_cmdIndex];
    m_hidManager.SendCmd(cmd);

    CString cstr(cmd.c_str());
    SHOW_LOG("Command: %s", cstr);

}


void CUsbHidDebuggerDlg::OnBnClickedButtonStopsend()
{
    // TODO: Add your control notification handler code here
    m_cmdIndex = 0;
    m_loopCounter = 0;
    KillTimer(TIMER_CHK_RSP_INNER);
    KillTimer(TIMER_CMD_INTERV);

    m_editCtrlCmd.EnableWindow(1);
    m_BtnSendCmd.EnableWindow(1);
    m_BtnStopSend.EnableWindow(0);
	m_chkCtrlAutoR.EnableWindow(1);
    m_chkAutoLoop.EnableWindow(1);
    m_editCtrlCmdIntervals.EnableWindow(1);
    m_editTimeout.EnableWindow(1);
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
    BOOL isAutoR = m_chkCtrlAutoR.GetCheck();
    log_info("Auto add \\r: " << isAutoR);

    CString editCtrlCmdStr;
    m_editCtrlCmd.GetWindowText(editCtrlCmdStr);
    editCtrlCmdStr.Trim();
    editCtrlCmdStr.Trim(_T("\r\n"));

    if (editCtrlCmdStr.IsEmpty())   // empty
    {
        return;
    }

    editCtrlCmdStr += _T("\r\n");
    CT2CA pszConvertedAnsiString(editCtrlCmdStr);
    std::string cmds(pszConvertedAnsiString);
    
    std::string cmd{""};
    for (auto c : cmds)
    {
        if (c == '\r') {
            continue;
        } 
        
        if (c == '\n') {
            if (cmd.empty()) {
                continue;
            }
            if (isAutoR) {
                cmd += "\r";
            }
            m_CmdLists.push_back(cmd);
            cmd = "";
        } else {
            cmd.append(std::string{c});
        }
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
    case TIMER_CHK_RSP_INNER:
        {
            KillTimer(TIMER_CHK_RSP_INNER);

            // recv last response
            char msgRsp[255] = {0};
            m_hidManager.RecvRsp(&(msgRsp[0]), sizeof(msgRsp), m_cmdTimeout);
            if (msgRsp[1]) {
                std::string cmd = &msgRsp[1];
                CString cstr(cmd.c_str());
                SHOW_LOG("Response: %s", cstr);
            } else {
                SHOW_LOG("Response: ---");
            }

            SetTimer(TIMER_CMD_INTERV, m_cmdInterval, NULL);
        }
        break;

    case TIMER_CMD_INTERV:
        {
            KillTimer(TIMER_CMD_INTERV);

            // send next command
            m_cmdIndex++;
            if (m_cmdIndex < m_CmdLists.size()) {
                std::string cmd = m_CmdLists[m_cmdIndex];
                m_hidManager.SendCmd(cmd);
                CString cstr(cmd.c_str());
                SHOW_LOG("Command: %s", cstr);
                SetTimer(TIMER_CHK_RSP_INNER, TIMER_CHK_RSP_INNER_TIMEOUT, NULL);
            } else {
                if (m_chkAutoLoop.GetCheck())
                {
                    m_loopCounter++;
                    SHOW_LOG("-------------------loop %d -------------------", m_loopCounter);
                    log_info("-------------------loop " << m_loopCounter << " -------------------");

                    m_cmdIndex = 0;
                    std::string cmd = m_CmdLists[m_cmdIndex];
                    m_hidManager.SendCmd(cmd);
                    CString cstr(cmd.c_str());
                    SHOW_LOG("Command: %s", cstr);
                    SetTimer(TIMER_CHK_RSP_INNER, TIMER_CHK_RSP_INNER_TIMEOUT, NULL);
                } else {
                    m_cmdIndex = 0;
                    m_loopCounter = 0;

                    m_editCtrlCmd.EnableWindow(1);
                    m_BtnSendCmd.EnableWindow(1);
                    m_BtnStopSend.EnableWindow(0);
                    m_chkCtrlAutoR.EnableWindow(1);
                    m_chkAutoLoop.EnableWindow(1);
                    m_editCtrlCmdIntervals.EnableWindow(1);
                    m_editTimeout.EnableWindow(1);
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
