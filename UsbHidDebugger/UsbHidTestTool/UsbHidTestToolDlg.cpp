
// UsbHidTestToolDlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "UsbHidTestTool.h"
#include "UsbHidTestToolDlg.h"
#include "afxdialogex.h"

#include "./logger/Logger.hpp"

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


// CUsbHidTestToolDlg dialog



CUsbHidTestToolDlg::CUsbHidTestToolDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_USBHIDTESTTOOL_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CUsbHidTestToolDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_TAB_MAIN, m_tab_main);
}

BEGIN_MESSAGE_MAP(CUsbHidTestToolDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
    ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_MAIN, &CUsbHidTestToolDlg::OnTcnSelchangeTabMain)
	ON_MESSAGE(WM_DEVICECHANGE, Main_OnUsbHidDevChange)
END_MESSAGE_MAP()


// CUsbHidTestToolDlg message handlers

BOOL CUsbHidTestToolDlg::OnInitDialog()
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

    // TODO: Add extra initialization here -------------->>>>
    m_tab_main.InsertItem(0, _T("Q Command"));
    m_tab_main.InsertItem(1, _T("SHUT/Power Device"));
    m_tab_qcmd.Create(IDD_DIALOG_QPROTO, &m_tab_main);
    m_tab_shut.Create(IDD_DIALOG_SHUT, &m_tab_main);

    CRect rs;
    m_tab_main.GetClientRect(&rs);
    rs.top += 25;
    rs.bottom -= 6;
    rs.left += 1;
    rs.right -= 10;
    m_tab_qcmd.MoveWindow(&rs);
    m_tab_shut.MoveWindow(&rs);

    m_tab_qcmd.ShowWindow(SW_SHOW);
    m_tab_shut.ShowWindow(SW_HIDE);
    m_tab_main.SetCurSel(0);
    m_tab_qcmd.DialogInit(true);
    m_tab_shut.DialogInit();
    log_info("show Q protocol when init......");

	Main_RegisterForDeviceNotifications();
	log_info("device change event registered...");

    // TODO: Add extra initialization here --------------<<<<<

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CUsbHidTestToolDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CUsbHidTestToolDlg::OnPaint()
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
HCURSOR CUsbHidTestToolDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CUsbHidTestToolDlg::OnTcnSelchangeTabMain(NMHDR *pNMHDR, LRESULT *pResult)
{
    // TODO: Add your control notification handler code here-------------->>>>>>
    m_tab_qcmd.DialogInit();
    m_tab_shut.DialogInit();

    int curSel = m_tab_main.GetCurSel();
    switch (curSel) {
    case 0:
        m_tab_qcmd.ShowWindow(true);
        m_tab_shut.ShowWindow(false);
        log_info("dialog change to Q protocol......");
        break;

    case 1:
        m_tab_qcmd.ShowWindow(false);
        m_tab_shut.ShowWindow(true);
        log_info("dialog change to SHUT protocol......");
        break;
    default:;
    }
    // TODO: Add your control notification handler code here--------------<<<<<

    *pResult = 0;
}

void CUsbHidTestToolDlg::Main_RegisterForDeviceNotifications()
{
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

LRESULT CUsbHidTestToolDlg::Main_OnUsbHidDevChange(WPARAM wParam, LPARAM lParam)
{
	PDEV_BROADCAST_HDR lpdb = (PDEV_BROADCAST_HDR)lParam;
 
	switch(wParam)
	{
		// Find out if a device has been attached or removed.
		// If yes, see if the name matches the device path name of the device we want to access.
 
	case DBT_DEVICEARRIVAL:		        // device pull in
		return TRUE; 
 
	case DBT_DEVICEREMOVECOMPLETE:		// device pull out
        // Is my device?
        if (m_tab_qcmd.IsMyDevice(lParam) || m_tab_shut.IsMyDevice(lParam)) {
			m_tab_qcmd.OnBnClickedButtonCloseDevQ();
			m_tab_shut.OnBnClickedButtonCloseDevShut();

            m_tab_qcmd.ShowLog(_T("Device has been unplugged!"));
			m_tab_shut.ShowLog(_T("Device has been unplugged!"));
            log_info("Device has been unplugged!");
            return TRUE;
        }
		return TRUE; 
 
	default:
		return TRUE; 
	}


}