// fastDlg.h : header file
//

#if !defined(AFX_FASTDLG_H__A55CD8E3_982F_4F90_B2AE_D1F59D8AE029__INCLUDED_)
#define AFX_FASTDLG_H__A55CD8E3_982F_4F90_B2AE_D1F59D8AE029__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "opengl.h"


/////////////////////////////////////////////////////////////////////////////
// CFastDlg dialog

class CFastDlg : public CDialog
{
// Construction
public:
	~CFastDlg(void);
	CFastDlg(CWnd* pParent = NULL);	// standard constructor
	COpenGL *m_pDisplay;

// Dialog Data
	//{{AFX_DATA(CFastDlg)
	enum { IDD = IDD_FAST_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFastDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CFastDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedGetfile();
	afx_msg void OnEnChangeFilepath();
	afx_msg void OnBnClickedCalculate();
	afx_msg void OnEnChangeEdit2();
	afx_msg void OnEnChangeEdit3();
	afx_msg void OnEnChangeEdit4();
	int m_cx;
	int m_cz;
	int m_yb;
	afx_msg void OnBnClickedColor();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	CScrollBar m_ScrollBar1;
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	// 彩色路面显示的位置
//	int m_distance;
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
//	int m_pos;
	CEdit m_pos;
	afx_msg void OnBnClickedstart();
	afx_msg void OnBnClickedstop();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedSave();
	CString outputPath;
	afx_msg void OnBnClickedSelectpath();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FASTDLG_H__A55CD8E3_982F_4F90_B2AE_D1F59D8AE029__INCLUDED_)
