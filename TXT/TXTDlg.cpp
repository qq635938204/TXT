

// TXTDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "TXT.h"
#include "TXTDlg.h"
#include "afxdialogex.h"
#include <tchar.h>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <list>
using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

HANDLE __HEVENT_EXIT__ = NULL;
long __lValue__ = 1;
// CTXTDlg 对话框



CTXTDlg::CTXTDlg(CWnd* pParent /*=NULL*/)
: CDialogEx(CTXTDlg::IDD, pParent)
, m_csPath(_T(""))
, m_csFileName(_T(""))
, m_csMainPath(_T(""))
, m_csOutPutFilename(_T(""))
, m_hMain(NULL)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CTXTDlg::~CTXTDlg()
{
	if (__HEVENT_EXIT__)
	{
		CloseHandle(__HEVENT_EXIT__);
		__HEVENT_EXIT__ = NULL;
	}
}

void CTXTDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_MFCEDITBROWSE1, m_csPath);
	DDX_Text(pDX, IDC_EDIT1, m_csFileName);
	DDX_Control(pDX, IDC_EDIT1, m_output);
	DDX_Control(pDX, IDC_BUTTON_START, m_start);
	DDX_Control(pDX, IDC_BUTTON_TEST, m_Test);
}

BEGIN_MESSAGE_MAP(CTXTDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_START, &CTXTDlg::OnBnClickedButtonStart)
	ON_COMMAND(IDCLOSE, &CTXTDlg::OnIdclose)
	ON_BN_CLICKED(IDC_BUTTON_TEST, &CTXTDlg::OnBnClickedButtonTest)
END_MESSAGE_MAP()


// CTXTDlg 消息处理程序
void FindNext(const CString& csSrc, CString& csNext)
{
	if (csSrc.IsEmpty())
		return;
	long lPos = -1;
	CString csNextName, csFront, csBack;
	long lNum = 0;
	TCHAR szNum[11] = { _T("0123456789") };
	lPos = csSrc.FindOneOf(szNum);
	if (lPos >= 0)
	{
		csFront = csSrc.Left(lPos);
		long lPos2 = csSrc.Find(_T(".txt"));
		if (lPos2 < 0)
			lPos2 = csSrc.Find(_T(".TXT"));
		if (lPos2>lPos)
		{
			long lTemp = 0;
			lTemp = csSrc.Find(_T(".jpg"));
			if (lTemp >= 0)
				lPos2 = lTemp;
			else
			{
				lTemp = csSrc.Find(_T(".JPG"));
				if (lTemp >= 0)
					lPos2 = lTemp;
			}
			csBack = csSrc.Right(csSrc.GetLength() - lPos2);
			csNextName = csSrc.Mid(lPos, lPos2 - lPos);
			lNum = _ttol(csNextName);
			lNum += 1;
			csNext.Format(_T("%s%d%s"), csFront, lNum, csBack);
		}
	}
}

BOOL FindFileExtensions(const CString& csName)
{
	long lPos = csName.ReverseFind('.');
	if (lPos>0)
	{
		CString csTemp;
		csTemp = csName.Right(csName.GetLength() - lPos - 1);
		csTemp.MakeLower();
		if (csTemp.CompareNoCase(_T("txt")) == 0 || csTemp.CompareNoCase(_T("eml")) == 0)
			return TRUE;
	}
	return FALSE;
}

DWORD WINAPI _AfxMain(LPVOID lpParam)
{
	try
	{
		CTXTDlg *pDlg = (CTXTDlg*)lpParam;
		long lStatus = 0;
		if (pDlg)
		{
			CString csStartPath = pDlg->m_csPath, csNext, csTempFileName;
			long lTemp = 0, lPos = 0, lCount = 0;
			while (lCount < 5)
			{
				if (WaitForSingleObject(__HEVENT_EXIT__, 10L) == WAIT_OBJECT_0)
				{
					lStatus = -1;
					break;
				}
				long lReturnValue = 0;
				lReturnValue = pDlg->FileProcessing(csStartPath);
				if (lReturnValue < 0)
				{
					if (lReturnValue == -2)
					{
						lPos = csStartPath.ReverseFind('\\');
						csTempFileName = csStartPath.Right(csStartPath.GetLength() - lPos - 1);
						FindNext(csTempFileName, csNext);
						csStartPath = pDlg->m_csMainPath + csNext;
						lCount++;
					}
				}
				else
				{
					lPos = csStartPath.ReverseFind('\\');
					csTempFileName = csStartPath.Right(csStartPath.GetLength() - lPos - 1);
					FindNext(csTempFileName, csNext);
					csStartPath = pDlg->m_csMainPath + csNext;
				}
			}
			pDlg->m_start.SetWindowText(_T("开始"));
			pDlg->m_start.EnableWindow();
			if (lStatus >= 0)
				AfxMessageBox(_T("处理完成！"));
		}
		else AfxMessageBox(_T("处理失败！"));
	}
	catch (...)
	{
	}
	return 0;
}

BOOL CTXTDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO:  在此添加额外的初始化代码
	if (!AfxSocketInit())
		return FALSE;
	__HEVENT_EXIT__ = CreateEvent(NULL, TRUE, FALSE, NULL);
	m_output.SetWindowText(_T("output.txt"));

#ifndef DEBUG
	m_Test.ShowWindow(SW_HIDE);
#endif

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CTXTDlg::OnPaint()
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
HCURSOR CTXTDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CTXTDlg::OnBnClickedButtonStart()
{
	// TODO:  在此添加控件通知处理程序代码
	DWORD id(0);
	long lPos = 0, lTemp = 0, lValue = 0, lCount = 0;
	CString csTempFileName, csNextFileName;
	UpdateData();
	if (m_csPath.IsEmpty())
	{
		AfxMessageBox(_T("请输入处理文本路径！"));
		return;
	}
	if (FindFileExtensions(m_csPath))
	{
		if (m_csFileName.IsEmpty())
			m_csFileName.Format(_T("output.txt"));
		if (!(m_csFileName.Find(_T(".txt")) >= 0))
			m_csFileName.Append(_T(".txt"));
		UpdateData(FALSE);

		lPos = m_csPath.ReverseFind('\\');
		lPos += 1;
		m_csMainPath = m_csPath.Left(lPos);
		csTempFileName = m_csPath.Right(m_csPath.GetLength() - lPos);
		m_csOutPutFilename = m_csMainPath + m_csFileName;
		if (!(GetFileAttributes(m_csOutPutFilename) == 0xFFFFFFFF))
		{
#ifndef DEBUG
			if (IDOK != AfxMessageBox(_T("文件已存在！是否追加到该文件？"), MB_OKCANCEL))
			{
				m_output.SetFocus();
				return;
			}
#endif
		}
		m_start.EnableWindow(FALSE);
		m_start.SetWindowText(_T("处理中..."));
		m_hMain = CreateThread(NULL, 0, _AfxMain, reinterpret_cast<void*>(this), 0, &id);
	}
	else
		AfxMessageBox(_T("文件格式不支持！"));
}

long CTXTDlg::FileProcessing(const CString& csPath)
{
	long lLen = 0;
	FILE *fp, *pFile;
	char chPath[512] = { "" };
	char *pText = NULL;
#ifdef DEBUG
	CString csDebugInfo;
#endif
	if (csPath.IsEmpty())
		return -1;
	memset(&chPath, 0, sizeof(chPath));
	WideCharToMultiByte(CP_ACP, 0, csPath, -1, chPath, 512, NULL, FALSE);
	if (WaitForSingleObject(__HEVENT_EXIT__, 10L) == WAIT_OBJECT_0)
	{
		return -3;
	}
	if (fopen_s(&fp, chPath, "r") == 0)
	{
		fseek(fp, 0, SEEK_END);
		lLen = ftell(fp);
		fseek(fp, 0, SEEK_SET);
		pText = new char[lLen + 1];
		memset(pText, 0, lLen + 1);
		fread_s(pText, lLen, 1, lLen, fp);
		fclose(fp);
	}
	else
	{
		if (pText)
		{
			delete pText;
			pText = NULL;
		}
		return -2;
	}
	if (WaitForSingleObject(__HEVENT_EXIT__, 10L) == WAIT_OBJECT_0)
	{
		return -3;
	}
	memset(chPath, 0, 512);
	WideCharToMultiByte(CP_ACP, 0, m_csOutPutFilename, -1, chPath, 512, NULL, FALSE);
	if (fopen_s(&pFile, chPath, "a+") == 0)
	{
		fseek(pFile, 0, SEEK_END);
		fwrite(pText, lLen, 1, pFile);
		fwrite("\n", 2, 1, fp);
		fclose(pFile);
	}
	if (pText)
	{
		delete pText;
		pText = NULL;
	}
#ifdef DEBUG
	csDebugInfo.Format(_T("%s处理完成！\n"), csPath);
	OutputDebugString(csDebugInfo);
#endif
	return 0;
}


void CTXTDlg::OnIdclose()
{
	// TODO:  在此添加命令处理程序代码
}


void CTXTDlg::OnCancel()
{
	// TODO:  在此添加专用代码和/或调用基类
	Stop();
	CDialogEx::OnCancel();
}

void CTXTDlg::Stop()
{
	if (__HEVENT_EXIT__)
		SetEvent(__HEVENT_EXIT__);
	if (WaitForSingleObject(m_hMain, /*INFINITE*/10000L) != WAIT_OBJECT_0)
		TerminateThread(m_hMain, 0);
	CloseHandle(m_hMain);
	m_hMain = NULL;
}


void CTXTDlg::OnBnClickedButtonTest()
{
	// TODO:  在此添加控件通知处理程序代码
	UpdateData();
	CStringArray csaText;
	//csaText.SetSize(100000);
	CString csText;
	if (!m_csPath.IsEmpty())
	{
		DWORD dwTime(0);
		dwTime = GetTickCount();
		string text;
		ifstream is;
		char chPath[MAX_PATH] = { 0 };
		WideCharToMultiByte(CP_ACP, 0, m_csPath.GetBuffer(), MAX_PATH, chPath, MAX_PATH, NULL, NULL);
		is.open(chPath,std::ios_base::binary);
		while (getline(is,text))
		{
			csText = text.c_str();
			csaText.Add(csText);
			//OutputDebugStringA(text.c_str());
		}
		dwTime = GetTickCount() - dwTime;
		sprintf_s(chPath, MAX_PATH, "Load Time = [%d sec]\r\n", dwTime / 1000);
		OutputDebugStringA(chPath);
	}
	
}
