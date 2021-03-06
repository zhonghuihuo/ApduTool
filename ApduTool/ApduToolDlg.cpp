
// ApduToolDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "ApduTool.h"
#include "ApduToolDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
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


// CApduToolDlg 对话框



CApduToolDlg::CApduToolDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_APDUTOOL_DIALOG, pParent)
	, mStrCapdu(_T(""))
	, mStrRapdu(_T(""))
	, mStrInfo(_T(""))
	, mStrBase64(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CApduToolDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_C_APDU, mStrCapdu);
	DDX_Text(pDX, IDC_EDIT_R_APDU, mStrRapdu);
	DDX_Text(pDX, IDC_EDIT_INFO, mStrInfo);
	DDX_Text(pDX, IDC_EDIT_BASE64, mStrBase64);
}

BEGIN_MESSAGE_MAP(CApduToolDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_CLEAR, &CApduToolDlg::OnBnClickedButtonClear)
	ON_EN_CHANGE(IDC_EDIT_C_APDU, &CApduToolDlg::OnChangeEditCApdu)
	ON_EN_CHANGE(IDC_EDIT_BASE64, &CApduToolDlg::OnChangeEditBase64)
END_MESSAGE_MAP()


// CApduToolDlg 消息处理程序

BOOL CApduToolDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
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

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CApduToolDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CApduToolDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CApduToolDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CApduToolDlg::OnBnClickedButtonClear()
{
	// TODO: 在此添加控件通知处理程序代码
	mStrCapdu = "";
	mStrRapdu = "";

	this->UpdateData(FALSE);
}

// C-APDU 80E2910013BF2D105C0E5A4F9F70909192939495B6B7B89900
// BF2D8181A07FE37D5A0A981099090021436590204F10A0000005591010FFFFFFFF89000011009F70010191084F454D2D47534D41920642424B5F3031950102B62930278002041081216F656D67736D612D6C70612D6A736F6E2E64656D6F2E67656D616C746F2E636F6DB71980030001108108BAE00000000000008208FFFFFFFFFFFFFFFF9000
unsigned char GetByteDataFromHex(CString strHexByte) {
	unsigned char _c1 = strHexByte[0];
	unsigned char _c2 = strHexByte[1];

	unsigned char cdata = 0;

	if ((_c1 >= '0') && (_c1 <= '9')) {
		cdata += ((_c1 - '0')&0xFF);
	}
	else if ((_c1 >= 'A') && (_c1 <= 'F')) {
		cdata += ((_c1 - 'A' + 10)&0xFF);
	}
	else if ((_c1 >= 'a') && (_c1 <= 'f')) {
		cdata += ((_c1 - 'a' + 10)&0xFF);
	}

	cdata <<= 4;

	if ((_c2 >= '0') && (_c2 <= '9')) {
		cdata += ((_c2 - '0')&0xFF);
	}
	else if ((_c2 >= 'A') && (_c2 <= 'F')) {
		cdata += ((_c2 - 'A' + 10)&0xFF);
	}
	else if ((_c2 >= 'a') && (_c2 <= 'f')) {
		cdata += ((_c2 - 'a' + 10)&0xFF);
	}

	return cdata;
}

void CApduToolDlg::OnChangeEditCApdu()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码

	this->UpdateData(TRUE);

	//mStrCapdu
	mStrCapdu = mStrCapdu.MakeUpper();

	CString strCmd = mStrCapdu;

	CString strCLA;
	unsigned char cCLA = 0;
	CString strINS;
	unsigned char cINS = 0;
	CString strP1;
	unsigned char cP1 = 0;
	CString strP2;
	unsigned char cP2 = 0;
	CString strLc;
	unsigned char cLc = 0;
	CString strData;

	CString strLe;
	unsigned char cLe = 0;

	if (strCmd.GetLength() >= 2) {
		strCLA = strCmd.Left(2);
		strCmd = strCmd.Right(strCmd.GetLength() - 2);

		cCLA = GetByteDataFromHex(strCLA);
	}

	if (strCmd.GetLength() >= 2) {
		strINS = strCmd.Left(2);
		strCmd = strCmd.Right(strCmd.GetLength() - 2);

		cINS = GetByteDataFromHex(strINS);
	}

	if (strCmd.GetLength() >= 2) {
		strP1 = strCmd.Left(2);
		strCmd = strCmd.Right(strCmd.GetLength() - 2);

		cP1 = GetByteDataFromHex(strP1);
	}

	if (strCmd.GetLength() >= 2) {
		strP2 = strCmd.Left(2);
		strCmd = strCmd.Right(strCmd.GetLength() - 2);

		cP2 = GetByteDataFromHex(strP2);
	}

	if (strCmd.GetLength() >= 2) {
		strLc = strCmd.Left(2);
		strCmd = strCmd.Right(strCmd.GetLength() - 2);

		cLc = GetByteDataFromHex(strLc);
	}

	if (strCmd.GetLength() >= (cLc * 2)) {
		strData = strCmd.Left(cLc * 2);
		strCmd = strCmd.Right(strCmd.GetLength() - (cLc * 2));
	}

	if (strCmd.GetLength() >= 2) {
		strLe = strCmd.Left(2);
		strCmd = strCmd.Right(strCmd.GetLength() - 2);

		cLe = GetByteDataFromHex(strLe);
	}

	//mStrInfo = mStrCapdu;
	mStrInfo.Empty();

	mStrInfo += (CString("命令类别\t:") + strCLA + CString("\r\n"));
	mStrInfo += (CString("指令代码\t:") + strINS + CString("\r\n"));
	mStrInfo += (CString("指令参数\t:") + strP1 + CString("\r\n"));
	mStrInfo += (CString("指令参数\t:") + strP2 + CString("\r\n"));
	mStrInfo += (CString("数据长度\t:") + strLc + CString("\r\n"));
	mStrInfo += (CString("数据内容\t:") + strData + CString("\r\n"));
	mStrInfo += (CString("响应长度\t:") + strLe + CString("\r\n"));

	if (strCmd.GetLength() > 0) {
		mStrInfo += (CString("剩余内容\t:") + strCmd + CString("\r\n"));
	}
	


	this->UpdateData(FALSE);
}

const char listBase64[64] = {
	'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z',
	'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z',
	'0','1','2','3','4','5','6','7','8','9','+','/'
};

int GetBase64Code(char _c) {
	for (int i = 0; i < 64; i++) {
		if (listBase64[i] == _c) {
			return i;
		}
	}
	return -1;
}




void CApduToolDlg::OnChangeEditBase64()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	this->UpdateData(TRUE);

	mStrInfo.Empty();

	unsigned int nBase64Length = mStrBase64.GetLength();

	CString strOldString = mStrBase64;

	unsigned int nDataOffset = 0;
	unsigned int nDataLength = (((nBase64Length / 4) * 3) + (((nBase64Length % 4) > 0) ? 3 : 0));
	unsigned char * pBuf = (unsigned char *)malloc(nDataLength);

	while (strOldString.GetLength() > 0) {
		int nNeedCpLen = strOldString.GetLength() > 4 ? 4 : strOldString.GetLength();
		CString str4Char = strOldString.Left(nNeedCpLen);
		strOldString = strOldString.Right(strOldString.GetLength() - nNeedCpLen);

		unsigned char cData[4] = { 0 };
		unsigned long nData = 0;
		memcpy(cData, str4Char.GetBuffer(), str4Char.GetLength());

		for (unsigned int i = 0; i < 4; i++) {
			int idata = GetBase64Code(cData[i] & 0xFF);
			nData <<= 6;
			if (idata < 0) {
				AfxMessageBox("有字符找不到");
			}
			else {
				nData |= (idata & 0x3F);
			}
		}

		// 小端模式，直接拷贝
		//memcpy(&pBuf[nDataOffset], ((unsigned char *)&nData), 1);
		//nDataOffset += 3;

		pBuf[nDataOffset++] = (((unsigned char *)&nData)[2] & 0xFF);
		pBuf[nDataOffset++] = (((unsigned char *)&nData)[1] & 0xFF);
		pBuf[nDataOffset++] = (((unsigned char *)&nData)[0] & 0xFF);

	}

	for (unsigned int i = 0; i < nDataLength; i += 16 ) {

		CString strHex;
		CString strString;

		for (unsigned int j = i; ((j < nDataLength) && (j < (i + 16))); j++) {
			CString strChar;
			strChar.Format("%02x ", pBuf[j]);
			strHex += strChar;

			if ((pBuf[j] >= ' ') && (pBuf[j] <= '~')) {
				CString strChar;
				strChar.Format("%c", pBuf[j]);
				strString += strChar;
			}
			else {
				strString += CString(".");
			}
		}

		CString strLine;
		strLine.Format("%06x: %-52s %s", i, strHex, strString);
		mStrInfo += (strLine + CString("\r\n"));
	}

	this->UpdateData(FALSE);
}
