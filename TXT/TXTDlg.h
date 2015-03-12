
// TXTDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"


// CTXTDlg �Ի���
class CTXTDlg : public CDialogEx
{
// ����
public:
	CTXTDlg(CWnd* pParent = NULL);	// ��׼���캯��
	~CTXTDlg();

// �Ի�������
	enum { IDD = IDD_TXT_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
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
