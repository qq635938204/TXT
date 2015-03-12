
// TXTDlg.h : 头文件
//

#pragma once
#include "afxwin.h"


// CTXTDlg 对话框
class CTXTDlg : public CDialogEx
{
// 构造
public:
	CTXTDlg(CWnd* pParent = NULL);	// 标准构造函数
	~CTXTDlg();

// 对话框数据
	enum { IDD = IDD_TXT_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
private:
	HANDLE m_hMain;
public:
	CString m_csPath;
	CString m_csFileName;
	CString m_csMainPath;
	CString m_csOutPutFilename;
protected:
public:
	afx_msg void OnBnClickedButtonStart();
	long FileProcessing(const CString& csPath);
	CEdit m_output;
	CButton m_start;
	afx_msg void OnIdclose();
	virtual void OnCancel();
	void Stop();
};
