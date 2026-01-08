// HidTestShutProto.cpp : implementation file
//

#include "pch.h"
#include <sstream>
#include <optional>
#include <map>
#include "UsbHidTestTool.h"
#include "HidTestShutProto.h"
#include "afxdialogex.h"

#include "utils.h"
#include "./logger/Logger.hpp"


// CHidTestShutProto dialog

IMPLEMENT_DYNAMIC(CHidTestShutProto, CDialogEx)

CHidTestShutProto::CHidTestShutProto(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_SHUT, pParent)
    , m_editLogShut(_T(""))
{

}

CHidTestShutProto::~CHidTestShutProto()
{
}

void CHidTestShutProto::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_EDIT_LOG_SHUT, m_editCtrlLogShut);
    DDX_Text(pDX, IDC_EDIT_LOG_SHUT, m_editLogShut);
    DDX_Control(pDX, IDC_EDIT_VID_INPUT_SHUT, m_editInputVidShut);
    DDX_Control(pDX, IDC_EDIT_PID_INPUT_SHUT, m_editInputPidShut);
    DDX_Control(pDX, IDC_BUTTON_OPEN_DEV_SHUT, m_btnOpenDevShut);
    DDX_Control(pDX, IDC_BUTTON_CLOSE_DEV_SHUT, m_btnCloseDevShut);
    DDX_Control(pDX, IDC_COMBO_MSG_TYPE1, m_combRptType1);
    DDX_Control(pDX, IDC_COMBO_MSG_TYPE2, m_combRptType2);
    DDX_Control(pDX, IDC_COMBO_MSG_TYPE3, m_combRptType3);
    DDX_Control(pDX, IDC_COMBO_MSG_TYPE4, m_combRptType4);
    DDX_Control(pDX, IDC_COMBO_MSG_TYPE5, m_combRptType5);
    DDX_Control(pDX, IDC_COMBO_MSG_TYPE6, m_combRptType6);
    DDX_Control(pDX, IDC_EDIT_RPTID1, m_editCtrlRptID1);
    DDX_Control(pDX, IDC_EDIT_RPTID2, m_editCtrlRptID2);
    DDX_Control(pDX, IDC_EDIT_RPTID3, m_editCtrlRptID3);
    DDX_Control(pDX, IDC_EDIT_RPTID4, m_editCtrlRptID4);
    DDX_Control(pDX, IDC_EDIT_RPTID5, m_editCtrlRptID5);
    DDX_Control(pDX, IDC_EDIT_RPTID6, m_editCtrlRptID6);
    DDX_Control(pDX, IDC_EDIT_DATATOSEND1, m_editCtrlRptData1);
    DDX_Control(pDX, IDC_EDIT_DATATOSEND2, m_editCtrlRptData2);
    DDX_Control(pDX, IDC_EDIT_DATATOSEND3, m_editCtrlRptData3);
    DDX_Control(pDX, IDC_EDIT_DATATOSEND4, m_editCtrlRptData4);
    DDX_Control(pDX, IDC_EDIT_DATATOSEND5, m_editCtrlRptData5);
    DDX_Control(pDX, IDC_EDIT_DATATOSEND6, m_editCtrlRptData6);
    DDX_Control(pDX, IDC_CHECK_LOOP_SHUT, m_chkAutoLoopShut);
    DDX_Control(pDX, IDC_EDIT_INTERVAL_MS_SHUT, m_editCtrlIntvShut);
    DDX_Control(pDX, IDC_EDIT_TIMEOUT_MS_SHUT, m_editCtrlTmoutShut);
    DDX_Control(pDX, IDC_BUTTON_SEND_SHUT, m_btnSendShut);
    DDX_Control(pDX, IDC_BUTTON_STOP_SHUT, m_btnStopShut);
}


BEGIN_MESSAGE_MAP(CHidTestShutProto, CDialogEx)
    ON_BN_CLICKED(IDC_BUTTON_CLRLOG_SHUT, &CHidTestShutProto::OnBnClickedButtonClrlogShut)
    ON_BN_CLICKED(IDC_BUTTON_OPEN_DEV_SHUT, &CHidTestShutProto::OnBnClickedButtonOpenDevShut)
    ON_BN_CLICKED(IDC_BUTTON_CLOSE_DEV_SHUT, &CHidTestShutProto::OnBnClickedButtonCloseDevShut)
    ON_BN_CLICKED(IDC_BUTTON_SEND_SHUT, &CHidTestShutProto::OnBnClickedButtonSendShut)
    ON_BN_CLICKED(IDC_BUTTON_STOP_SHUT, &CHidTestShutProto::OnBnClickedButtonStopShut)
    ON_CBN_SELCHANGE(IDC_COMBO_MSG_TYPE1, &CHidTestShutProto::OnCbnSelchangeComboMsgType1)
    ON_CBN_SELCHANGE(IDC_COMBO_MSG_TYPE2, &CHidTestShutProto::OnCbnSelchangeComboMsgType2)
    ON_CBN_SELCHANGE(IDC_COMBO_MSG_TYPE3, &CHidTestShutProto::OnCbnSelchangeComboMsgType3)
    ON_CBN_SELCHANGE(IDC_COMBO_MSG_TYPE4, &CHidTestShutProto::OnCbnSelchangeComboMsgType4)
    ON_CBN_SELCHANGE(IDC_COMBO_MSG_TYPE5, &CHidTestShutProto::OnCbnSelchangeComboMsgType5)
    ON_CBN_SELCHANGE(IDC_COMBO_MSG_TYPE6, &CHidTestShutProto::OnCbnSelchangeComboMsgType6)
    ON_WM_TIMER()
END_MESSAGE_MAP()


// CHidTestShutProto message handlers
void CHidTestShutProto::ShowLog(CString str)
{
    utils::ShowLog(m_editCtrlLogShut, m_editLogShut, str);
}

#define SHOW_LOG(fmt, ...)   do{                                      \
	                              CString cs;                         \
	                              cs.Format(_T(fmt), __VA_ARGS__);    \
	                              ShowLog(cs);                        \
                            }while(0)

void CHidTestShutProto::EnableReportInfoTab()
{
    m_combRptType1.EnableWindow(TRUE);
    m_editCtrlRptID1.EnableWindow(TRUE);
    m_editCtrlRptData1.EnableWindow(TRUE);

    m_combRptType2.EnableWindow(TRUE);
    m_editCtrlRptID2.EnableWindow(TRUE);
    m_editCtrlRptData2.EnableWindow(TRUE);

    m_combRptType3.EnableWindow(TRUE);
    m_editCtrlRptID3.EnableWindow(TRUE);
    m_editCtrlRptData3.EnableWindow(TRUE);

    m_combRptType4.EnableWindow(TRUE);
    m_editCtrlRptID4.EnableWindow(TRUE);
    m_editCtrlRptData4.EnableWindow(TRUE);

    m_combRptType5.EnableWindow(TRUE);
    m_editCtrlRptID5.EnableWindow(TRUE);
    m_editCtrlRptData5.EnableWindow(TRUE);

    m_combRptType6.EnableWindow(TRUE);
    m_editCtrlRptID6.EnableWindow(TRUE);
    m_editCtrlRptData6.EnableWindow(TRUE);
}

void CHidTestShutProto::DisableReportInfoTab()
{
    m_combRptType1.EnableWindow(FALSE);
    m_editCtrlRptID1.EnableWindow(FALSE);
    m_editCtrlRptData1.EnableWindow(FALSE);

    m_combRptType2.EnableWindow(FALSE);
    m_editCtrlRptID2.EnableWindow(FALSE);
    m_editCtrlRptData2.EnableWindow(FALSE);

    m_combRptType3.EnableWindow(FALSE);
    m_editCtrlRptID3.EnableWindow(FALSE);
    m_editCtrlRptData3.EnableWindow(FALSE);

    m_combRptType4.EnableWindow(FALSE);
    m_editCtrlRptID4.EnableWindow(FALSE);
    m_editCtrlRptData4.EnableWindow(FALSE);

    m_combRptType5.EnableWindow(FALSE);
    m_editCtrlRptID5.EnableWindow(FALSE);
    m_editCtrlRptData5.EnableWindow(FALSE);

    m_combRptType6.EnableWindow(FALSE);
    m_editCtrlRptID6.EnableWindow(FALSE);
    m_editCtrlRptData6.EnableWindow(FALSE);
}

void CHidTestShutProto::ShowDevDetailInfo()
{
    CString showVid = _T("0x") + m_hidManager.GetVidStrHex();
    CEdit* editShowVid = (CEdit*)GetDlgItem(IDC_EDIT_SHOWVID_SHUT);
    editShowVid->SetWindowText(showVid);

    CString showPid = _T("0x") + m_hidManager.GetPidStrHex();
    CEdit* editShowPid = (CEdit*)GetDlgItem(IDC_EDIT_SHOWPID_SHUT);
    editShowPid->SetWindowText(showPid);

    CString showVer = _T("0x") + m_hidManager.GetVerStrHex();
    CEdit* editShowVer = (CEdit*)GetDlgItem(IDC_EDIT_SHOWVER_SHUT);
    editShowVer->SetWindowText(showVer);

    CString showManu = m_hidManager.GetManufactureStr();
    CEdit* editShowManu = (CEdit*)GetDlgItem(IDC_EDIT_SHOWMANU_SHUT);
    editShowManu->SetWindowText(showManu);

    CString showProd = m_hidManager.GetProductStr();
    CEdit* editShowProd = (CEdit*)GetDlgItem(IDC_EDIT_SHOWPROD_SHUT);
    editShowProd->SetWindowText(showProd);

    CString showSN = m_hidManager.GetSerialNum();
    CEdit* editShowSN = (CEdit*)GetDlgItem(IDC_EDIT_SHOWSN_SHUT);
    editShowSN->SetWindowText(showSN);
}

void CHidTestShutProto::ClearDevDetailInfo()
{
    CEdit* editShowVid = (CEdit*)GetDlgItem(IDC_EDIT_SHOWVID_SHUT);
    editShowVid->SetWindowText(_T(""));

    CEdit* editShowPid = (CEdit*)GetDlgItem(IDC_EDIT_SHOWPID_SHUT);
    editShowPid->SetWindowText(_T(""));

    CEdit* editShowVer = (CEdit*)GetDlgItem(IDC_EDIT_SHOWVER_SHUT);
    editShowVer->SetWindowText(_T(""));

    CEdit* editShowManu = (CEdit*)GetDlgItem(IDC_EDIT_SHOWMANU_SHUT);
    editShowManu->SetWindowText(_T(""));

    CEdit* editShowProd = (CEdit*)GetDlgItem(IDC_EDIT_SHOWPROD_SHUT);
    editShowProd->SetWindowText(_T(""));

    CEdit* editShowSN = (CEdit*)GetDlgItem(IDC_EDIT_SHOWSN_SHUT);
    editShowSN->SetWindowText(_T(""));
}

void CHidTestShutProto::SetElemStatusWhenInit()
{
    m_editInputVidShut.EnableWindow(1);
    m_editInputPidShut.EnableWindow(1);

    m_btnOpenDevShut.EnableWindow(1);
    m_btnCloseDevShut.EnableWindow(0);

    ClearDevDetailInfo();

    DisableReportInfoTab();

    m_btnSendShut.EnableWindow(0);
    m_btnStopShut.EnableWindow(0);

    m_chkAutoLoopShut.EnableWindow(0);
    m_editCtrlIntvShut.EnableWindow(0);
    m_editCtrlTmoutShut.EnableWindow(0);
}

void CHidTestShutProto::SetElemStatusWhenOpened()
{
    m_editInputVidShut.EnableWindow(0);
    m_editInputPidShut.EnableWindow(0);

    m_btnOpenDevShut.EnableWindow(0);
    m_btnCloseDevShut.EnableWindow(1);

    EnableReportInfoTab();

    m_btnSendShut.EnableWindow(1);
    m_btnStopShut.EnableWindow(0);

    m_chkAutoLoopShut.EnableWindow(1);
    m_editCtrlIntvShut.EnableWindow(1);
    m_editCtrlTmoutShut.EnableWindow(1);
}

void CHidTestShutProto::SetElemStatusWhenSending()
{
    m_editInputVidShut.EnableWindow(0);
    m_editInputPidShut.EnableWindow(0);

    m_btnOpenDevShut.EnableWindow(0);
    m_btnCloseDevShut.EnableWindow(1);

    DisableReportInfoTab();

    m_btnSendShut.EnableWindow(0);
    m_btnStopShut.EnableWindow(1);

    m_chkAutoLoopShut.EnableWindow(0);
    m_editCtrlIntvShut.EnableWindow(0);
    m_editCtrlTmoutShut.EnableWindow(0);
}

void CHidTestShutProto::SetElemStatusWhenStopped()
{
    m_editInputVidShut.EnableWindow(0);
    m_editInputPidShut.EnableWindow(0);

    m_btnOpenDevShut.EnableWindow(0);
    m_btnCloseDevShut.EnableWindow(1);

    EnableReportInfoTab();

    m_btnSendShut.EnableWindow(1);
    m_btnStopShut.EnableWindow(0);

    m_chkAutoLoopShut.EnableWindow(1);
    m_editCtrlIntvShut.EnableWindow(1);
    m_editCtrlTmoutShut.EnableWindow(1);
}

void CHidTestShutProto::OnBnClickedButtonClrlogShut()
{
    // TODO: Add your control notification handler code here
    m_editLogShut = _T("");
    m_editCtrlLogShut.SetWindowText(m_editLogShut);
}


void CHidTestShutProto::OnBnClickedButtonOpenDevShut()
{
    // TODO: Add your control notification handler code here
    CString editVidStr;
    m_editInputVidShut.GetWindowText(editVidStr);
    SHOW_LOG("VID=%s", editVidStr);
    int vid = utils::CstringHex2Int(editVidStr);
    log_info("VID:" << vid);
    if (vid < 0)
    {
        (void)MessageBox(_T("Invalid VID!"), _T("Waring"), MB_OK);
        return;
    }

    CString editPidStr;
    m_editInputPidShut.GetWindowText(editPidStr);
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
    m_VidShut = vid;
    m_PidShut = pid;

    ShowDevDetailInfo();

    SetElemStatusWhenOpened();
}

std::optional<ShutCmdInfo> CHidTestShutProto::GetMsgInfo(const CComboBox& combType, const CEdit& editRptId, const CEdit& editRptData)
{
    ShutCmdInfo cmdInfo;

    // type
    int type = combType.GetCurSel();
    switch (type)
    {
    case 0:
        cmdInfo.type = SHUT_CMD_TYPE::CMD_SET_RPT;
        break;
    case 1:
        cmdInfo.type = SHUT_CMD_TYPE::CMD_GET_RPT;
        break;
    case 2:
        cmdInfo.type = SHUT_CMD_TYPE::CMD_GET_DESC;
        break;
    case 3:
        cmdInfo.type = SHUT_CMD_TYPE::CMD_SET_FEAT;
        break;
    case 4:
        cmdInfo.type = SHUT_CMD_TYPE::CMD_GET_FEAT;
        break;
    default:
        return std::nullopt;
    }

    // report id
    CString reportIdStr;
    editRptId.GetWindowText(reportIdStr);
    reportIdStr.Trim();
    if (reportIdStr.IsEmpty())   // empty
    {
        return std::nullopt;
    }
    uint8_t reportId = utils::CstringHex2Int(reportIdStr);
    cmdInfo.msg[0] = reportId;
    cmdInfo.msgLen = 1;

    // only set_report and set_feature need data context
    if (cmdInfo.type != SHUT_CMD_TYPE::CMD_SET_RPT && cmdInfo.type != SHUT_CMD_TYPE::CMD_SET_FEAT) 
    {
        return cmdInfo;
    }

    // report data
    CString reportDataStr;
    editRptData.GetWindowText(reportDataStr);
    reportDataStr.Trim();
    if (reportDataStr.IsEmpty())   // empty
    {
        return std::nullopt;
    }
    CT2CA pszConvertedAnsiString(reportDataStr);
    std::string msgData(pszConvertedAnsiString);

    std::istringstream iss(msgData);
    std::string data;
    size_t index = cmdInfo.msgLen; // msg[0] is report ID
    while (iss >> data) {
        try {
            CString csData;
            csData = data.c_str();
            uint8_t num = utils::CstringHex2Int(csData);
            index = index < sizeof(cmdInfo.msg) ? index : sizeof(cmdInfo.msg) - 1;
            cmdInfo.msg[index] = num;
            index++;
            cmdInfo.msgLen = index;
        }
        catch (const std::exception&)
        {
            return std::nullopt;
        }
    }

    return cmdInfo;
}

void CHidTestShutProto::GetMsgLists()
{
    m_msgLists.clear();

    std::optional<ShutCmdInfo> info1 = GetMsgInfo(m_combRptType1, m_editCtrlRptID1, m_editCtrlRptData1);
    if (info1.has_value()) {
        m_msgLists.push_back(info1.value());
    }

    std::optional<ShutCmdInfo> info2 = GetMsgInfo(m_combRptType2, m_editCtrlRptID2, m_editCtrlRptData2);
    if (info2.has_value()) {
        m_msgLists.push_back(info2.value());
    }

    std::optional<ShutCmdInfo> info3 = GetMsgInfo(m_combRptType3, m_editCtrlRptID3, m_editCtrlRptData3);
    if (info3.has_value()) {
        m_msgLists.push_back(info3.value());
    }

    std::optional<ShutCmdInfo> info4 = GetMsgInfo(m_combRptType4, m_editCtrlRptID4, m_editCtrlRptData4);
    if (info4.has_value()) {
        m_msgLists.push_back(info4.value());
    }

    std::optional<ShutCmdInfo> info5 = GetMsgInfo(m_combRptType5, m_editCtrlRptID5, m_editCtrlRptData5);
    if (info5.has_value()) {
        m_msgLists.push_back(info5.value());
    }

    std::optional<ShutCmdInfo> info6 = GetMsgInfo(m_combRptType6, m_editCtrlRptID6, m_editCtrlRptData6);
    if (info6.has_value()) {
        m_msgLists.push_back(info6.value());
    }
}


void CHidTestShutProto::OnBnClickedButtonCloseDevShut()
{
    // TODO: Add your control notification handler code here
    m_cmdIndex = 0;
    m_loopCounter = 0;

    KillTimer(utils::TIMER_START_SEND_SHUT);
    KillTimer(utils::TIMER_CMD_INTERVAL_SHUT);

    m_hidManager.CloseHidDev();

    SetElemStatusWhenInit();
}

void CHidTestShutProto::PrintMsgLists()
{
    std::map<SHUT_CMD_TYPE, std::string> cmdTypeStr = {
        { SHUT_CMD_TYPE::CMD_SET_RPT,  "SR" },
        { SHUT_CMD_TYPE::CMD_GET_RPT,  "GR" },
        { SHUT_CMD_TYPE::CMD_GET_DESC, "GD" },
        { SHUT_CMD_TYPE::CMD_SET_FEAT, "SF" },
        { SHUT_CMD_TYPE::CMD_GET_FEAT, "GF" }
    };
    std::map<SHUT_CMD_TYPE, CString> cmdTypeCstr = {
        { SHUT_CMD_TYPE::CMD_SET_RPT,  _T("SR") },
        { SHUT_CMD_TYPE::CMD_GET_RPT,  _T("GR") },
        { SHUT_CMD_TYPE::CMD_GET_DESC, _T("GD") },
        { SHUT_CMD_TYPE::CMD_SET_FEAT, _T("SF") },
        { SHUT_CMD_TYPE::CMD_GET_FEAT, _T("GF") }
    };

    SHOW_LOG("Send List(total %d): ", m_msgLists.size());
    log_info("Send List" << "(total " << m_msgLists.size() << "): ");
    int msgCnt = 1;
    for (auto& item : m_msgLists)
    {
        std::string sendStr = utils::ByteArray2HexString(item.msg, item.msgLen);
        CString csStr(sendStr.c_str());
        SHOW_LOG("%d.%s: [%s]", msgCnt, cmdTypeCstr[item.type], csStr);
        log_info(msgCnt << "." <<cmdTypeStr[item.type] << ": [" << sendStr << "]");
        msgCnt++;
    }
}

void CHidTestShutProto::OnBnClickedButtonSendShut()
{
    // TODO: Add your control notification handler code here
    GetMsgLists();
    PrintMsgLists();

    if (m_msgLists.size() <= 0) {
        (void)MessageBox(_T("No command to send!"), _T("Waring"), MB_OK);
        return;
    }

    SetElemStatusWhenSending();

    CString csint;
    m_editCtrlIntvShut.GetWindowText(csint);
    int interval = _ttoi(csint);
    m_msgIntvShut = interval > 0 ? interval : DEFAULT_CMD_INTERV_MS;
    SHOW_LOG("Send cmd interval: %d", m_msgIntvShut);
    log_info("Send cmd interval: " << m_msgIntvShut);
    CString feedInterv;
    feedInterv.Format(_T("%d"), m_msgIntvShut);
    m_editCtrlIntvShut.SetWindowText(feedInterv);

    CString cstmout;
    m_editCtrlTmoutShut.GetWindowText(cstmout);
    int tmout = _ttoi(cstmout);
    m_msgTmoutShut = tmout > DEFAULT_CMD_RECV_TIMEOUT_MS ? tmout : DEFAULT_CMD_RECV_TIMEOUT_MS;
    SHOW_LOG("Recv cmd timeout: %d", m_msgTmoutShut);
    log_info("Recv cmd timeout: " << m_msgTmoutShut);
    CString feedTmout;
    feedTmout.Format(_T("%d"), m_msgTmoutShut);
    m_editCtrlTmoutShut.SetWindowText(feedTmout);

    m_cmdIndex = 0;
    m_loopCounter = 0;
    SHOW_LOG("-------------------loop %d -------------------", m_loopCounter);
    log_info("-------------------loop " << m_loopCounter << " -------------------");
    SetTimer(utils::TIMER_START_SEND_SHUT, utils::START_SEND_TMOUT_SHUT, NULL);
}

void CHidTestShutProto::OnTimer(UINT_PTR nIDEvent)
{
    // TODO: Add your message handler code here and/or call default
    switch (nIDEvent)
    {
    case utils::TIMER_START_SEND_SHUT:
        KillTimer(utils::TIMER_START_SEND_SHUT);
        // fall through
    case utils::TIMER_CMD_INTERVAL_SHUT:
        KillTimer(utils::TIMER_CMD_INTERVAL_SHUT);
        // fall through
    }

    // begin handle message
    uint8_t arr[utils::BUFF_SZIE];
    memcpy_s(arr, sizeof(arr) * sizeof(uint8_t), m_msgLists[m_cmdIndex].msg, sizeof(m_msgLists[m_cmdIndex].msg) * sizeof(uint8_t));
    SHUT_CMD_TYPE type = m_msgLists[m_cmdIndex].type;
    switch (type)
    {
    case SHUT_CMD_TYPE::CMD_SET_RPT:
        {
            bool ret = m_hidManager.SetReport(arr);

            std::string isSucc = ret ? "T" : "F";
            CString csIsSucc(isSucc.c_str());
            std::string sendStr = utils::ByteArray2HexString(m_msgLists[m_cmdIndex].msg, m_msgLists[m_cmdIndex].msgLen);
            CString csStr(sendStr.c_str());
            SHOW_LOG("SET_RPT[%s]: [%s]", csIsSucc, csStr);
            log_info("SET_RPT[" << isSucc << "]: [" << sendStr << "]");
        }

        break;
    case SHUT_CMD_TYPE::CMD_GET_RPT:
        {
            bool ret = m_hidManager.GetReport(arr, m_msgTmoutShut);

            std::string isSucc = ret ? "T" : "F";
            CString csIsSucc(isSucc.c_str());
            std::string sendStr = utils::ByteArray2HexString(arr, m_hidManager.InReportSize());
            CString csStr(sendStr.c_str());
            SHOW_LOG("GET_RPT[%s]: [%s]", csIsSucc, csStr);
            log_info("GET_RPT[" << csIsSucc << "]: [" << sendStr << "]");
        }

        break;
    case SHUT_CMD_TYPE::CMD_GET_DESC:
        {
            uint8_t strIdx = arr[0];
            memset(arr, 0, sizeof(arr) * sizeof(uint8_t));
            bool ret = m_hidManager.GetStrDescriptor(strIdx, arr, m_msgTmoutShut);

            std::string isSucc = ret ? "T" : "F";
            CString csIsSucc(isSucc.c_str());

            std::string resultStr{""};
            for (auto elem : arr)
            {
                if (elem == 0) {
                    continue;
                }
                std::string str(1, static_cast<char>(elem));
                resultStr += str;
            }
            CString csStr(resultStr.c_str());
            SHOW_LOG("GET_DES[%s]: [%s]", csIsSucc, csStr);
            log_info("GET_DES[" << isSucc << "]: [" << resultStr << "]");
        }

        break;
    case SHUT_CMD_TYPE::CMD_SET_FEAT:
        {
            bool ret = m_hidManager.SetFeature(arr);

            std::string isSucc = ret ? "T" : "F";
            CString csIsSucc(isSucc.c_str());

            std::string sendStr = utils::ByteArray2HexString(m_msgLists[m_cmdIndex].msg, m_msgLists[m_cmdIndex].msgLen);
            CString csStr(sendStr.c_str());
            SHOW_LOG("SET_FEA[%s]: [%s]", csIsSucc, csStr);
            log_info("SET_FEA[" << isSucc << "]: [" << sendStr << "]");
        }

        break;
    case SHUT_CMD_TYPE::CMD_GET_FEAT:
        {
            bool ret = m_hidManager.GetFeature(arr, m_msgTmoutShut);

            std::string isSucc = ret ? "T" : "F";
            CString csIsSucc(isSucc.c_str());

            std::string sendStr = utils::ByteArray2HexString(arr, m_hidManager.FeatureRptSize());
            CString csStr(sendStr.c_str());
            SHOW_LOG("GET_FEA[%s]: [%s]", csIsSucc, csStr);
            log_info("GET_FEA[" << isSucc << "]: [" << sendStr << "]");
        }

        break;
    }

    // send next command
    m_cmdIndex++;

    if (m_cmdIndex < m_msgLists.size()) {
        SetTimer(utils::TIMER_CMD_INTERVAL_SHUT, m_msgIntvShut, NULL);
    }
    else
    {
        if (m_chkAutoLoopShut.GetCheck())
        {
            m_loopCounter++;
            SHOW_LOG("-------------------loop %d -------------------", m_loopCounter);
            log_info("-------------------loop " << m_loopCounter << " -------------------");

            m_cmdIndex = 0;

            SetTimer(utils::TIMER_CMD_INTERVAL_SHUT, m_msgIntvShut, NULL);
        }
        else {
            m_cmdIndex = 0;
            m_loopCounter = 0;
            SetElemStatusWhenStopped();
        }
    }

    CDialogEx::OnTimer(nIDEvent);
}


void CHidTestShutProto::OnBnClickedButtonStopShut()
{
    // TODO: Add your control notification handler code here
    m_cmdIndex = 0;
    m_loopCounter = 0;

    KillTimer(utils::TIMER_START_SEND_SHUT);
    KillTimer(utils::TIMER_CMD_INTERVAL_SHUT);

    SetElemStatusWhenStopped();
}

void CHidTestShutProto::DialogInit()
{
    m_VidShut = 0;
    m_PidShut = 0;

    m_msgIntvShut = 0;
    m_msgTmoutShut = 0;

    m_msgLists.clear();
    m_cmdIndex = 0;
    m_loopCounter = 0;

    KillTimer(utils::TIMER_START_SEND_SHUT);
    KillTimer(utils::TIMER_CMD_INTERVAL_SHUT);

    m_hidManager.CloseHidDev();

    SetElemStatusWhenInit();
}

bool CHidTestShutProto::IsMyDevice(LPARAM lParam)
{
    return m_hidManager.IsMyDevice(lParam);
}


bool CHidTestShutProto::IsSetRptOrSetFeature(const CComboBox& combType)
{
    SHUT_CMD_TYPE rptType = SHUT_CMD_TYPE::CMD_COUNT;

    int curSel = combType.GetCurSel();
    switch (curSel)
    {
    case 0:
        rptType = SHUT_CMD_TYPE::CMD_SET_RPT;
        break;
    case 1:
        rptType = SHUT_CMD_TYPE::CMD_GET_RPT;
        break;
    case 2:
        rptType = SHUT_CMD_TYPE::CMD_GET_DESC;
        break;
    case 3:
        rptType = SHUT_CMD_TYPE::CMD_SET_FEAT;
        break;
    case 4:
        rptType = SHUT_CMD_TYPE::CMD_GET_FEAT;
        break;
    default:
        break;
    }

    if (rptType == SHUT_CMD_TYPE::CMD_SET_RPT || rptType == SHUT_CMD_TYPE::CMD_SET_FEAT)
    {
        return true;
    }

    return false;
}

void CHidTestShutProto::OnCbnSelchangeComboMsgType1()
{
    // TODO: Add your control notification handler code here
    if (IsSetRptOrSetFeature(m_combRptType1)) {
        m_editCtrlRptData1.ShowWindow(TRUE);
        return;
    }

    m_editCtrlRptData1.SetWindowText(_T(""));
    m_editCtrlRptData1.ShowWindow(FALSE);
}


void CHidTestShutProto::OnCbnSelchangeComboMsgType2()
{
    // TODO: Add your control notification handler code here
    if (IsSetRptOrSetFeature(m_combRptType2)) {
        m_editCtrlRptData2.ShowWindow(TRUE);
        return;
    }

    m_editCtrlRptData2.SetWindowText(_T(""));
    m_editCtrlRptData2.ShowWindow(FALSE);
}


void CHidTestShutProto::OnCbnSelchangeComboMsgType3()
{
    // TODO: Add your control notification handler code here
    if (IsSetRptOrSetFeature(m_combRptType3)) {
        m_editCtrlRptData3.ShowWindow(TRUE);
        return;
    }

    m_editCtrlRptData3.SetWindowText(_T(""));
    m_editCtrlRptData3.ShowWindow(FALSE);
}


void CHidTestShutProto::OnCbnSelchangeComboMsgType4()
{
    // TODO: Add your control notification handler code here
    if (IsSetRptOrSetFeature(m_combRptType4)) {
        m_editCtrlRptData4.ShowWindow(TRUE);
        return;
    }

    m_editCtrlRptData4.SetWindowText(_T(""));
    m_editCtrlRptData4.ShowWindow(FALSE);
}


void CHidTestShutProto::OnCbnSelchangeComboMsgType5()
{
    // TODO: Add your control notification handler code here
    if (IsSetRptOrSetFeature(m_combRptType5)) {
        m_editCtrlRptData5.ShowWindow(TRUE);
        return;
    }

    m_editCtrlRptData5.SetWindowText(_T(""));
    m_editCtrlRptData5.ShowWindow(FALSE);
}


void CHidTestShutProto::OnCbnSelchangeComboMsgType6()
{
    // TODO: Add your control notification handler code here
    if (IsSetRptOrSetFeature(m_combRptType6)) {
        m_editCtrlRptData6.ShowWindow(TRUE);
        return;
    }

    m_editCtrlRptData6.SetWindowText(_T(""));
    m_editCtrlRptData6.ShowWindow(FALSE);
}
