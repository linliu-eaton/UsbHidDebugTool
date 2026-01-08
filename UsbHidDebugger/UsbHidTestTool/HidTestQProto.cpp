// HidTestQProto.cpp : implementation file
//

#include "pch.h"
#include "UsbHidTestTool.h"
#include "HidTestQProto.h"
#include "afxdialogex.h"

#include "utils.h"
#include "./logger/Logger.hpp"


// CHidTestQProto dialog

IMPLEMENT_DYNAMIC(CHidTestQProto, CDialogEx)

CHidTestQProto::CHidTestQProto(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_QPROTO, pParent)
    , m_editLogQ(_T(""))
{

}

CHidTestQProto::~CHidTestQProto()
{
}

void CHidTestQProto::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_EDIT_LOG_Q, m_editCtrlLogQ);
    DDX_Text(pDX, IDC_EDIT_LOG_Q, m_editLogQ);
    DDX_Control(pDX, IDC_EDIT_VID_INPUT_Q, m_editInputVidQ);
    DDX_Control(pDX, IDC_EDIT_PID_INPUT_Q, m_editInputPidQ);
    DDX_Control(pDX, IDC_BUTTON_OPEN_DEV_Q, m_btnOpenDevQ);
    DDX_Control(pDX, IDC_BUTTON_CLOSE_DEV_Q, m_btnCloseDevQ);
    DDX_Control(pDX, IDC_EDIT_QCMD, m_editCtrlCmdQ);
    DDX_Control(pDX, IDC_BUTTON_SENDCMD_Q, m_BtnSendCmdQ);
    DDX_Control(pDX, IDC_BUTTON_STOPSEND_Q, m_BtnStopCmdQ);
    DDX_Control(pDX, IDC_CHECK_AUTO_R, m_chkCtrlAutoR);
    DDX_Control(pDX, IDC_CHECK_AUTO_LOOP, m_chkAutoLoop);
    DDX_Control(pDX, IDC_EDIT_INTERVAL_MS_Q, m_editCtrlCmdIntvQ);
    DDX_Control(pDX, IDC_EDIT_TIMEOUT_MS_Q, m_editCtrlTmoutQ);
    DDX_Control(pDX, IDC_CHECK_QCMD_FWV, m_chkCmdFwv);
    DDX_Control(pDX, IDC_CHECK_QCMD_UDESC, m_chkCmdUdesc);
    DDX_Control(pDX, IDC_CHECK_QCMD_UAS, m_chkCmdUas);
    DDX_Control(pDX, IDC_CHECK_QCMD_BI, m_chkCmdBl);
    DDX_Control(pDX, IDC_CHECK_QCMD_Q1, m_chkCmdQ1);
    DDX_Control(pDX, IDC_CHECK_QCMD_PSNR, m_chkCmdPsnr);
    DDX_Control(pDX, IDC_CHECK_QCMD_CF, m_chkCmdCf);
    DDX_Control(pDX, IDC_CHECK_QCMD_SI, m_chkCmdSi);
    DDX_Control(pDX, IDC_CHECK_QCMD_BRR, m_chkCmdBrr);
    DDX_Control(pDX, IDC_CHECK_QCMD_Lf, m_chkCmdLf);
    DDX_Control(pDX, IDC_BUTTON_ALL_QCMD, m_btnCmdAll);
}


BEGIN_MESSAGE_MAP(CHidTestQProto, CDialogEx)
    ON_BN_CLICKED(IDC_BUTTON_CLRLOG_Q, &CHidTestQProto::OnBnClickedButtonClrlogQ)
    ON_BN_CLICKED(IDC_BUTTON_OPEN_DEV_Q, &CHidTestQProto::OnBnClickedButtonOpenDevQ)
    ON_BN_CLICKED(IDC_BUTTON_CLOSE_DEV_Q, &CHidTestQProto::OnBnClickedButtonCloseDevQ)
    ON_BN_CLICKED(IDC_BUTTON_SENDCMD_Q, &CHidTestQProto::OnBnClickedButtonSendcmdQ)
    ON_BN_CLICKED(IDC_BUTTON_STOPSEND_Q, &CHidTestQProto::OnBnClickedButtonStopsendQ)
    ON_BN_CLICKED(IDC_BUTTON_ALL_QCMD, &CHidTestQProto::OnBnClickedButtonAllQcmd)
    ON_WM_TIMER()
END_MESSAGE_MAP()

// CHidTestQProto message handlers
#define SHOW_LOG(fmt, ...)   do{                                      \
	                              CString cs;                         \
	                              cs.Format(_T(fmt), __VA_ARGS__);    \
	                              ShowLog(cs);                        \
                            }while(0)

void CHidTestQProto::EnableAllCmdChkbox()
{
    m_chkCmdFwv.EnableWindow(TRUE);
    m_chkCmdUdesc.EnableWindow(TRUE);
    m_chkCmdUas.EnableWindow(TRUE);
    m_chkCmdBl.EnableWindow(TRUE);
    m_chkCmdQ1.EnableWindow(TRUE);
    m_chkCmdPsnr.EnableWindow(TRUE);
    m_chkCmdCf.EnableWindow(TRUE);
    m_chkCmdSi.EnableWindow(TRUE);
    m_chkCmdBrr.EnableWindow(TRUE);
    m_chkCmdLf.EnableWindow(TRUE);
    m_btnCmdAll.EnableWindow(TRUE);
}

void CHidTestQProto::DisableAllCmdChkbox()
{
    m_chkCmdFwv.EnableWindow(FALSE);
    m_chkCmdUdesc.EnableWindow(FALSE);
    m_chkCmdUas.EnableWindow(FALSE);
    m_chkCmdBl.EnableWindow(FALSE);
    m_chkCmdQ1.EnableWindow(FALSE);
    m_chkCmdPsnr.EnableWindow(FALSE);
    m_chkCmdCf.EnableWindow(FALSE);
    m_chkCmdSi.EnableWindow(FALSE);
    m_chkCmdBrr.EnableWindow(FALSE);
    m_chkCmdLf.EnableWindow(FALSE);
    m_btnCmdAll.EnableWindow(FALSE);
}

void CHidTestQProto::SelectAllCmdChkbox()
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
}

void CHidTestQProto::UnselectAllCmdChkbox()
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
}

void CHidTestQProto::ShowDevDetailInfo()
{
    CString showVid = _T("0x") + m_hidManager.GetVidStrHex();
    CEdit* editShowVid = (CEdit*)GetDlgItem(IDC_EDIT_SHOWVID_Q);
    editShowVid->SetWindowText(showVid);

    CString showPid = _T("0x") + m_hidManager.GetPidStrHex();
    CEdit* editShowPid = (CEdit*)GetDlgItem(IDC_EDIT_SHOWPID_Q);
    editShowPid->SetWindowText(showPid);

    CString showVer = _T("0x") + m_hidManager.GetVerStrHex();
    CEdit* editShowVer = (CEdit*)GetDlgItem(IDC_EDIT_SHOWVER_Q);
    editShowVer->SetWindowText(showVer);

    CString showManu = m_hidManager.GetManufactureStr();
    CEdit* editShowManu = (CEdit*)GetDlgItem(IDC_EDIT_SHOWMANU_Q);
    editShowManu->SetWindowText(showManu);

    CString showProd = m_hidManager.GetProductStr();
    CEdit* editShowProd = (CEdit*)GetDlgItem(IDC_EDIT_SHOWPROD_Q);
    editShowProd->SetWindowText(showProd);

    CString showSN = m_hidManager.GetSerialNum();
    CEdit* editShowSN = (CEdit*)GetDlgItem(IDC_EDIT_SHOWSN_Q);
    editShowSN->SetWindowText(showSN);
}

void CHidTestQProto::ClearDevDetailInfo()
{
    CEdit* editShowVid = (CEdit*)GetDlgItem(IDC_EDIT_SHOWVID_Q);
    editShowVid->SetWindowText(_T(""));

    CEdit* editShowPid = (CEdit*)GetDlgItem(IDC_EDIT_SHOWPID_Q);
    editShowPid->SetWindowText(_T(""));

    CEdit* editShowVer = (CEdit*)GetDlgItem(IDC_EDIT_SHOWVER_Q);
    editShowVer->SetWindowText(_T(""));

    CEdit* editShowManu = (CEdit*)GetDlgItem(IDC_EDIT_SHOWMANU_Q);
    editShowManu->SetWindowText(_T(""));

    CEdit* editShowProd = (CEdit*)GetDlgItem(IDC_EDIT_SHOWPROD_Q);
    editShowProd->SetWindowText(_T(""));

    CEdit* editShowSN = (CEdit*)GetDlgItem(IDC_EDIT_SHOWSN_Q);
    editShowSN->SetWindowText(_T(""));
}

void CHidTestQProto::SetElemStatusWhenInit(bool isClrLog)
{
    if (isClrLog) {
        OnBnClickedButtonClrlogQ();
    }

    m_editInputVidQ.EnableWindow(1);
    m_editInputPidQ.EnableWindow(1);

    m_btnOpenDevQ.EnableWindow(1);
    m_btnCloseDevQ.EnableWindow(0);

    ClearDevDetailInfo();

    //m_editCtrlCmdQ.SetWindowText(_T(""));  // clear cmds
    m_editCtrlCmdQ.EnableWindow(0);

    m_BtnSendCmdQ.EnableWindow(0);
    m_BtnStopCmdQ.EnableWindow(0);

    m_chkCtrlAutoR.EnableWindow(0);
    m_chkAutoLoop.EnableWindow(0);
    m_editCtrlCmdIntvQ.EnableWindow(0);
    m_editCtrlTmoutQ.EnableWindow(0);

    // UnselectAllCmdChkbox();  // clear selected cmds
    DisableAllCmdChkbox();
}

void CHidTestQProto::SetElemStatusWhenOpened()
{
    m_editInputVidQ.EnableWindow(0);
    m_editInputPidQ.EnableWindow(0);

    m_btnOpenDevQ.EnableWindow(0);
    m_btnCloseDevQ.EnableWindow(1);

    m_editCtrlCmdQ.EnableWindow(1);

    m_BtnSendCmdQ.EnableWindow(1);
    m_BtnStopCmdQ.EnableWindow(0);

    m_chkCtrlAutoR.EnableWindow(1);
    m_chkAutoLoop.EnableWindow(1);
    m_editCtrlCmdIntvQ.EnableWindow(1);
    m_editCtrlTmoutQ.EnableWindow(1);

    EnableAllCmdChkbox();
}

void CHidTestQProto::SetElemStatusWhenSending()
{
    m_editInputVidQ.EnableWindow(0);
    m_editInputPidQ.EnableWindow(0);

    m_btnOpenDevQ.EnableWindow(0);
    m_btnCloseDevQ.EnableWindow(1);

    m_editCtrlCmdQ.EnableWindow(0);

    m_BtnSendCmdQ.EnableWindow(0);
    m_BtnStopCmdQ.EnableWindow(1);

    m_chkCtrlAutoR.EnableWindow(0);
    m_chkAutoLoop.EnableWindow(0);
    m_editCtrlCmdIntvQ.EnableWindow(0);
    m_editCtrlTmoutQ.EnableWindow(0);

    DisableAllCmdChkbox();
}

void CHidTestQProto::SetElemStatusWhenStopped()
{
    m_editInputVidQ.EnableWindow(0);
    m_editInputPidQ.EnableWindow(0);

    m_btnOpenDevQ.EnableWindow(0);
    m_btnCloseDevQ.EnableWindow(1);

    m_editCtrlCmdQ.EnableWindow(1);

    m_BtnSendCmdQ.EnableWindow(1);
    m_BtnStopCmdQ.EnableWindow(0);

    m_chkCtrlAutoR.EnableWindow(1);
    m_chkAutoLoop.EnableWindow(1);
    m_editCtrlCmdIntvQ.EnableWindow(1);
    m_editCtrlTmoutQ.EnableWindow(1);

    EnableAllCmdChkbox();
}

void CHidTestQProto::OnBnClickedButtonClrlogQ()
{
    // TODO: Add your control notification handler code here
    m_editLogQ = _T("");
    m_editCtrlLogQ.SetWindowText(m_editLogQ);
}

void CHidTestQProto::ShowLog(CString str)
{
    utils::ShowLog(m_editCtrlLogQ, m_editLogQ, str);
}

void CHidTestQProto::GetChkboxCmdLists()
{
    if (m_chkCmdFwv.GetCheck()) { m_CmdLists.push_back("FWV\r"); }
    if (m_chkCmdUdesc.GetCheck()) { m_CmdLists.push_back("UDESC\r"); }
    if (m_chkCmdUas.GetCheck()) { m_CmdLists.push_back("UAS\r"); }
    if (m_chkCmdBl.GetCheck()) { m_CmdLists.push_back("Bl\r"); }
    if (m_chkCmdQ1.GetCheck()) { m_CmdLists.push_back("Q1\r"); }
    if (m_chkCmdPsnr.GetCheck()) { m_CmdLists.push_back("PSNR\r"); }
    if (m_chkCmdCf.GetCheck()) { m_CmdLists.push_back("CF\r"); }
    if (m_chkCmdSi.GetCheck()) { m_CmdLists.push_back("Si ?\r"); }
    if (m_chkCmdBrr.GetCheck()) { m_CmdLists.push_back("BRR\r"); }
    if (m_chkCmdLf.GetCheck()) { m_CmdLists.push_back("Lf\r"); }
}

void CHidTestQProto::GetEditCtrlCmdLists()
{
    BOOL isAutoR = m_chkCtrlAutoR.GetCheck();
    log_info("Auto add \\r: " << isAutoR);

    CString editCtrlCmdStr;
    m_editCtrlCmdQ.GetWindowText(editCtrlCmdStr);
    editCtrlCmdStr.Trim();
    editCtrlCmdStr.Trim(_T("\r\n"));

    if (editCtrlCmdStr.IsEmpty())   // empty
    {
        return;
    }

    editCtrlCmdStr += _T("\r\n");
    CT2CA pszConvertedAnsiString(editCtrlCmdStr);
    std::string cmds(pszConvertedAnsiString);

    std::string cmd{ "" };
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
        }
        else {
            cmd.append(std::string{ c });
        }
    }
}

void CHidTestQProto::GetCmdLists()
{
    m_CmdLists.clear();
    GetChkboxCmdLists();
    if (!m_CmdLists.empty())
    {
        return;
    }
    GetEditCtrlCmdLists();
}

void CHidTestQProto::OnBnClickedButtonOpenDevQ()
{
    // TODO: Add your control notification handler code here
    CString editVidStr;
    m_editInputVidQ.GetWindowText(editVidStr);
    SHOW_LOG("VID=%s", editVidStr);
    int vid = utils::CstringHex2Int(editVidStr);
    log_info("VID:" << vid);
    if (vid < 0)
    {
        (void)MessageBox(_T("Invalid VID!"), _T("Waring"), MB_OK);
        return;
    }

    CString editPidStr;
    m_editInputPidQ.GetWindowText(editPidStr);
    SHOW_LOG("PID=%s", editPidStr);
    int pid = utils::CstringHex2Int(editPidStr);
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
    m_VidQ = vid;
    m_PidQ = pid;

    ShowDevDetailInfo();

    SetElemStatusWhenOpened();
}


void CHidTestQProto::OnBnClickedButtonCloseDevQ()
{
    // TODO: Add your control notification handler code here
    m_cmdIndex = 0;
    m_loopCounter = 0;

    KillTimer(utils::TIMER_CHK_RSP_INNER);
    KillTimer(utils::TIMER_CMD_INTERV);

    m_hidManager.CloseHidDev();

    SetElemStatusWhenInit();
}


void CHidTestQProto::OnBnClickedButtonSendcmdQ()
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

    SetElemStatusWhenSending();

    CString csint;
    m_editCtrlCmdIntvQ.GetWindowText(csint);
    int interval = _ttoi(csint);
    m_cmdIntvQ = interval > 0 ? interval : DEFAULT_CMD_INTERV_MS;
    SHOW_LOG("Send cmd interval: %d", m_cmdIntvQ);
    log_info("Send cmd interval: " << m_cmdIntvQ);
    CString feedInterv;
    feedInterv.Format(_T("%d"), m_cmdIntvQ);
    m_editCtrlCmdIntvQ.SetWindowText(feedInterv);

    CString cstmout;
    m_editCtrlTmoutQ.GetWindowText(cstmout);
    int tmout = _ttoi(cstmout);
    m_cmdTmoutQ = tmout > DEFAULT_CMD_RECV_TIMEOUT_MS ? tmout : DEFAULT_CMD_RECV_TIMEOUT_MS;
    SHOW_LOG("Recv cmd timeout: %d", m_cmdTmoutQ);
    log_info("Send cmd interval: " << m_cmdTmoutQ);
    CString feedTmout;
    feedTmout.Format(_T("%d"), m_cmdTmoutQ);
    m_editCtrlTmoutQ.SetWindowText(feedTmout);

    SetTimer(utils::TIMER_CHK_RSP_INNER, utils::CHK_RSP_INNER_TIMEOUT_Q, NULL);

    m_cmdIndex = 0;
    m_loopCounter = 0;
    SHOW_LOG("-------------------loop %d -------------------", m_loopCounter);
    log_info("-------------------loop " << m_loopCounter << " -------------------");
    std::string cmd = m_CmdLists[m_cmdIndex];
    m_hidManager.SendCmd(cmd);

    CString cstr(cmd.c_str());
    SHOW_LOG("Command: %s", cstr);
}


void CHidTestQProto::OnBnClickedButtonStopsendQ()
{
    // TODO: Add your control notification handler code here
    m_cmdIndex = 0;
    m_loopCounter = 0;

    KillTimer(utils::TIMER_CHK_RSP_INNER);
    KillTimer(utils::TIMER_CMD_INTERV);

    SetElemStatusWhenStopped();
}


void CHidTestQProto::OnBnClickedButtonAllQcmd()
{
    // TODO: Add your control notification handler code here
    CString strCurrTxt;
    m_btnCmdAll.GetWindowText(strCurrTxt);
    if (strCurrTxt == "Select All")
    {
        SelectAllCmdChkbox();
        m_btnCmdAll.SetWindowText(L"Clear All");
    }
    else
    {
        UnselectAllCmdChkbox();
        m_btnCmdAll.SetWindowText(L"Select All");
    }
}


void CHidTestQProto::OnTimer(UINT_PTR nIDEvent)
{
    // TODO: Add your message handler code here and/or call default
    switch (nIDEvent)
    {
        //Wait boot device attach timer
    case utils::TIMER_CHK_RSP_INNER:
    {
        KillTimer(utils::TIMER_CHK_RSP_INNER);

        // recv last response
        char msgRsp[utils::BUFF_SZIE] = { 0 };
        m_hidManager.RecvRsp(&(msgRsp[0]), sizeof(msgRsp), m_cmdTmoutQ);
        if (msgRsp[1]) {
            std::string cmd = &msgRsp[1];
            CString cstr(cmd.c_str());
            SHOW_LOG("Response: %s", cstr);
        }
        else {
            SHOW_LOG("Response: ---");
        }

        SetTimer(utils::TIMER_CMD_INTERV, m_cmdIntvQ, NULL);
    }
    break;

    case utils::TIMER_CMD_INTERV:
    {
        KillTimer(utils::TIMER_CMD_INTERV);

        // send next command
        m_cmdIndex++;
        if (m_cmdIndex < m_CmdLists.size()) {
            std::string cmd = m_CmdLists[m_cmdIndex];
            m_hidManager.SendCmd(cmd);
            CString cstr(cmd.c_str());
            SHOW_LOG("Command: %s", cstr);
            SetTimer(utils::TIMER_CHK_RSP_INNER, utils::CHK_RSP_INNER_TIMEOUT_Q, NULL);
        }
        else {
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
                SetTimer(utils::TIMER_CHK_RSP_INNER, utils::CHK_RSP_INNER_TIMEOUT_Q, NULL);
            }
            else {
                m_cmdIndex = 0;
                m_loopCounter = 0;
                SetElemStatusWhenStopped();
            }
        }
    }
    break;

    default:
        break;
    }

    CDialogEx::OnTimer(nIDEvent);
}

void CHidTestQProto::DialogInit(bool isClrLog)
{
    m_VidQ = 0;
    m_PidQ = 0;

    m_cmdIntvQ = 0;
    m_cmdTmoutQ = 0;

    m_CmdLists.clear();
    m_cmdIndex = 0;
    m_loopCounter = 0;
    KillTimer(utils::TIMER_CHK_RSP_INNER);
    KillTimer(utils::TIMER_CMD_INTERV);

    m_hidManager.CloseHidDev();

    SetElemStatusWhenInit(isClrLog);
}

bool CHidTestQProto::IsMyDevice(LPARAM lParam)
{
    return m_hidManager.IsMyDevice(lParam);
}
