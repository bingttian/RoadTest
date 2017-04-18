// fastDlg.cpp : implementation file
//

#include "stdafx.h"
#include "RoadTest.h"
#include "RoadTestDlg.h"
#include "afxdialogex.h"
#include "string"
#include "afx.h"  
#include <vector>
#include <gl/gl.h>
#include <gl/glu.h>

using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/*
初始化数据
*/
const int column=13;			//宽
const int row=6651;				//长
int column_c=column;
//const int row=101376;
const int cx_standard=-20;		//沉陷标准
const int yb_standard=20;		//拥包标准
const int cz_standard=20;		//车辙标准
short int data_final[row][column]={0};
int m_distance=0;				//距离变量，控制OpenGL绘图路面的位置
int distance_c=0;				//用来给OpenGL.cpp传参

int Pic_Size=80;				//设置绘制路面的距离（正在显示的长度）

/////////////////////////////////////////////////////////////////////////////
// CFastDlg dialog

CFastDlg::CFastDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFastDlg::IDD, pParent)

{
	//{{AFX_DATA_INIT(CFastDlg)
	// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_pDisplay = new COpenGL;
	m_cx = 0;		//三个用于在Edit上显示的变量
	m_cz = 0;
	m_yb = 0;
	//  m_pos = 0;
	outputPath = _T("");
}

void CFastDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFastDlg)
	// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
	DDX_Text(pDX, IDC_CX, m_cx);
	DDX_Text(pDX, IDC_CZ, m_cz);
	DDX_Text(pDX, IDC_YB, m_yb);
	DDX_Control(pDX, IDC_SCROLLBAR1, m_ScrollBar1);
	//  DDX_Text(pDX, IDC_POSITION, m_pos);
	DDX_Control(pDX, IDC_POSITION, m_pos);
}

BEGIN_MESSAGE_MAP(CFastDlg, CDialog)
	//{{AFX_MSG_MAP(CFastDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_GetFile, &CFastDlg::OnBnClickedGetfile)
	ON_EN_CHANGE(IDC_FilePath, &CFastDlg::OnEnChangeFilepath)
	ON_BN_CLICKED(IDC_Calculate, &CFastDlg::OnBnClickedCalculate)
	ON_BN_CLICKED(IDC_Color, &CFastDlg::OnBnClickedColor)
	ON_WM_SIZE()
	ON_WM_VSCROLL()
	ON_WM_ERASEBKGND()
	ON_BN_CLICKED(IDC_start, &CFastDlg::OnBnClickedstart)
	ON_BN_CLICKED(IDC_stop, &CFastDlg::OnBnClickedstop)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_SAVE, &CFastDlg::OnBnClickedSave)
	ON_BN_CLICKED(IDC_SelectPath, &CFastDlg::OnBnClickedSelectpath)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFastDlg message handlers

BOOL CFastDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	CRect rect(20,40, 370, 390);	//一个方形黑色背景

	// TODO: Add extra initialization here
	m_pDisplay->Create( NULL,   //CWnd default
						NULL,   //has no name
						WS_CHILD|WS_CLIPSIBLINGS|WS_CLIPCHILDREN|WS_VISIBLE,
						rect,
						this,   //this is the parent
						0);     //this should really be a different number... check resource.h

	//添加滚动条
	m_ScrollBar1.SetScrollRange(0,row-80);
	m_ScrollBar1.SetScrollPos(row-80);
	

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CFastDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

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
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CFastDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

CFastDlg::~CFastDlg()
{
	if(m_pDisplay)
	{
		delete m_pDisplay;
	}
}

//读取txt格式的原始数据
void CFastDlg::OnBnClickedGetfile()
{
	
	CFileDialog fDlg(TRUE, _TEXT("txt"), NULL, 4|2,  
		_TEXT("文本文件(*.txt,*.ini,*.log)|*.txt;*.ini;*.log|全部文件(*.*)|*.*||"));  

	// 调用 DoModal() 方法来打开对话框  
	if (fDlg.DoModal() == IDOK)  
	{  
		// 设置编辑框1的内容为选择文件的路径  
		// GetPathName() --> 返回选定文件的完整路径  
		SetDlgItemText(IDC_FilePath,fDlg.GetPathName());  

		//------------------------------------------------------------------------------  
		// CFile类具体详解：http://baike.baidu.com/view/994710.htm  
		//------------------------------------------------------------------------------  
		CStdioFile fFile(fDlg.GetPathName(),CFile::modeRead);  

		CString strFile ;  
		CString data;

		// 读取打开文件内容  
		// 第一个参数：读取内容的缓冲区域  
		// 第二个参数：读取内容的长度  
		//fFile.Read(strFile.GetBufferSetLength(fFile.GetLength()),fFile.GetLength());  

		int i=0;
		//三行取一行
		while(fFile.ReadString(strFile))
		{
			if(i==1){
				data+=strFile;
				data+=",";
			}
			i++;
			i=i%3;
		}

		//SetDlgItemText(IDC_File,data);
		// 关闭文件句柄  
		fFile.Close();  

		short int data_split[column*row];
		int pos=0;
		//把一行一行读取的内容用逗号分割成一个一维的数组
		//每拼接一次CString就会重新new一个CString的对象，所以分割时不能用字符串拼接
		for(int i=0;i<column*row;i++)							//这里曾经太慢O(∩_∩)O
		{  
			data_split[i]=_ttoi(data.Tokenize(_T(","),pos));
		}			

		//把一维数组变成可处理的二维数组
		for (int j=0;j<column;j++){
			for(int i=0;i<row;i++){
				data_final[i][j]=data_split[column*i+j];
			}
		}
	}
}


void CFastDlg::OnEnChangeFilepath()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}

//定义三个结构体用于存放处理后的沉陷拥包车辙数据
struct chenxian{
	int index;
	int i;
	int j;
	int width;
	int length;
	int area;
	int deep;
};
struct yongbao{
	int index;
	int i;
	int j;
	int width;
	int length;
	int area;
	int deep;
};
struct chezhe{
	int index;
	int i;
	int j;
	int width;
	int length;
	int area;
	int deep;
};
vector<chenxian> cx_vector;
vector<yongbao> yb_vector;
vector<chezhe> cz_vector;

int area=0;
int i_min,i_max,j_min,j_max;
int deep=0;
//************************************
// Method:    mark
// FullName:  mark
// Access:    public 
// Returns:   void
// Qualifier:
// Parameter: int data[row][column]	原始数据
// Parameter: int flag[row][column]	标记数据
// Parameter: int i	
// Parameter: int j
// Parameter: int index				标记值
//************************************
//八连通域算法，计算的同时记录下长，宽，深度或者高度，面积
void mark_cx(short int data[row][column],short int flag[row][column],int i,int j,int index){
	//给标记号
	flag[i][j] = index;
	//记录i、j的最值，相减算长宽
	if(i<i_min){
		i_min=i;
	}
	if(i>i_max){
		i_max=i;
	}
	if(j<j_min){
		j_min=j;
	}
	if(j>j_max){
		j_max=j;
	}
	//记深度
	if(deep>data[i][j]){
		deep=data[i][j];
	}
	//记面积
	area++;
	//int len = row;
	//递归算法
	if (j + 1 < column && data[i][j + 1] < cx_standard && flag[i][j + 1] == 0) {
		mark_cx(data, flag, i, j + 1, index);
	}
	if (i + 1 < row && j + 1 < column && data[i + 1][j + 1] < cx_standard && flag[i + 1][j + 1] == 0) {
		mark_cx(data, flag, i + 1, j + 1, index);
	}
	if (i + 1 < row && data[i + 1][j] < cx_standard && flag[i + 1][j] == 0) {
		mark_cx(data, flag, i + 1, j, index);
	}
	if (i + 1 < row && j - 1 >= 0 && data[i + 1][j - 1] < cx_standard && flag[i + 1][j - 1] == 0) {
		mark_cx(data, flag, i + 1, j - 1, index);
	}
	if (j - 1 >= 0 && data[i][j - 1] < cx_standard && flag[i][j - 1] == 0) {
		mark_cx(data, flag, i, j - 1, index);
	}
	if ( i- 1>= 0 && j-1>0 && data[i-1][j-1] < cx_standard && flag[i-1][j-1] == 0){
		mark_cx(data, flag, i-1, j-1, index);
	}
	if (i-1>=0 && data[i-1][j]<cx_standard && flag[i-1][j]==0){
		mark_cx(data, flag, i-1, j, index);
	}
	if (i-1>=0 && j+1<column && data[i-1][j+1]<cx_standard && flag[i-1][j+1]==0){
		mark_cx(data, flag, i-1, j+1, index);
	}
}

//跟上面一样，一个是大于，一个是小于
void mark_yb(short int data[row][column],short int flag[row][column],int i,int j,int index){
	flag[i][j] = index;
	if(i<i_min){
		i_min=i;
	}
	if(i>i_max){
		i_max=i;
	}
	if(j<j_min){
		j_min=j;
	}
	if(j>j_max){
		j_max=j;
	}
	if(deep<data[i][j]){
		deep=data[i][j];
	}
	area++;
	//int len = row;
	if (j + 1 < column && data[i][j + 1] >yb_standard && flag[i][j + 1] == 0) {
		mark_yb(data, flag, i, j + 1, index);
	}
	if (i + 1 < row && j + 1 < column && data[i + 1][j + 1] >yb_standard && flag[i + 1][j + 1] == 0) {
		mark_yb(data, flag, i + 1, j + 1, index);
	}
	if (i + 1 < row && data[i + 1][j] >yb_standard && flag[i + 1][j] == 0) {
		mark_yb(data, flag, i + 1, j, index);
	}
	if (i + 1 < row && j - 1 >= 0 && data[i + 1][j - 1] >yb_standard && flag[i + 1][j - 1] == 0) {
		mark_yb(data, flag, i + 1, j - 1, index);
	}
	if (j - 1 >= 0 && data[i][j - 1] >yb_standard && flag[i][j - 1] == 0) {
		mark_yb(data, flag, i, j - 1, index);
	}
	if ( i- 1>= 0 && j-1>0 && data[i-1][j-1] >yb_standard && flag[i-1][j-1] == 0){
		mark_yb(data, flag, i-1, j-1, index);
	}
	if (i-1>=0 && data[i-1][j]>yb_standard && flag[i-1][j]==0){
		mark_yb(data, flag, i-1, j, index);
	}
	if (i-1>=0 && j+1<column && data[i-1][j+1]>yb_standard && flag[i-1][j+1]==0){
		mark_yb(data, flag, i-1, j+1, index);
	}
}

//计算数据
void CFastDlg::OnBnClickedCalculate()
{
	int index_cx=1;
	int index_yb=1;
	int index_cz=1;
	static short int flag_cx[row][column]={0};
	static short int flag_yb[row][column]={0};
	for (int i = 0; i < row; i++) {
		for (int j = 0; j < column; j++) {
			chenxian c;
			yongbao y;
			chezhe z;
			if (data_final[i][j] < cx_standard && flag_cx[i][j] == 0) {
				//初始化
				c.i=i;
				c.j=j;
				z.i=i;
				z.j=j;
				area=0;
				i_min=i,i_max=i,j_min=j,j_max=j;
				deep=0;
				
				mark_cx(data_final, flag_cx, i, j, index_cx);

				c.index=index_cx;
				c.area=area;
				c.deep=deep;
				c.length=i_max-i_min+1;
				if(i_max-i_min+1>cz_standard){
					z.index=index_cz;
					index_cz++;
					z.area=area;
					z.deep=deep;
					z.length=i_max-i_min+1;
					z.width=j_max-j_min+1;
					cz_vector.push_back(z);
				}
				c.width=j_max-j_min+1;
				cx_vector.push_back(c);
				index_cx++;
			}

			//跟上面一样
			if(data_final[i][j]>yb_standard && flag_yb[i][j]==0){
				y.i=i;
				y.j=j;
				area=0;
				i_min=i,i_max=i,j_min=j,j_max=j;
				deep=0;
				mark_yb(data_final,flag_yb,i,j,index_yb);
				y.index=index_yb;
				y.area=area;
				y.deep=deep;
				y.length=i_max-i_min+1;
				y.width=j_max-j_min+1;
				yb_vector.push_back(y);
				index_yb++;
			}
		}
	}
	m_cx=cx_vector.size();
	m_yb=yb_vector.size();
	m_cz=cz_vector.size();
	UpdateData(false);
}

//刷新一下，重新调用OnPaint，把载入的二维数组显示出来
void CFastDlg::OnBnClickedColor()
{
	Invalidate();
	
}


void CFastDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);


	glViewport(0,0,350,1000);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, 1, 0.1f, 50.0f);
	//gluOrtho2D(0.0,500.0*aspect,0.0,500.0);
}

//滚动条
void CFastDlg::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	//int iNowPos;
	int pos = m_ScrollBar1.GetScrollPos();    // 获取滚动条当前位置  
	char sPos[10];
	switch(nSBCode)
	{
	case SB_THUMBTRACK:	//当拖动滚动块时
			pos=nPos;
			m_distance=pos;
			itoa(row-Pic_Size-m_distance,sPos,10);
			m_pos.SetSel(0,-1);
			m_pos.ReplaceSel(sPos);
			//Invalidate();
			break;
	case  SB_LINEDOWN:	
			pos+=10;
			if(pos>row-Pic_Size){
				pos=row-Pic_Size;
			}
			m_distance=pos;
			itoa(row-Pic_Size-m_distance,sPos,10);
			m_pos.SetSel(0,-1);
			m_pos.ReplaceSel(sPos);
			//Invalidate();
			break;
	case SB_LINEUP:
			pos-=10;
			if(pos<0){
				pos=0;
			}
			m_distance=pos;
			itoa(row-Pic_Size-m_distance,sPos,10);
			m_pos.SetSel(0,-1);
			m_pos.ReplaceSel(sPos);
			//Invalidate();
			break;
	case SB_PAGEDOWN:
			pos+=200;
			m_distance=pos;
			itoa(row-Pic_Size-m_distance,sPos,10);
			m_pos.SetSel(0,-1);
			m_pos.ReplaceSel(sPos);
			//Invalidate();
			break;
	case SB_PAGEUP:
			pos-=200;
			m_distance=pos;
			itoa(row-Pic_Size-m_distance,sPos,10);
			m_pos.SetSel(0,-1);
			m_pos.ReplaceSel(sPos);
			//Invalidate();
			break;
	}
	//为了让操作符合实际路面显示逻辑，让滚动条从下至上移动，滚动条在最下面时显示位置为0
	distance_c = row-Pic_Size-m_distance;
	m_ScrollBar1.SetScrollPos(pos);
	Invalidate();
	CDialog::OnVScroll(nSBCode, nPos, pScrollBar);
}


BOOL CFastDlg::OnEraseBkgnd(CDC* pDC)
{
	//解决彩色路面漫游，重绘时产生的闪烁问题
	//Invalidate(FALSE);
	return TRUE;
	return CDialog::OnEraseBkgnd(pDC);
}

//开始漫游，调用OnTimer
void CFastDlg::OnBnClickedstart()
{	
	SetTimer(1,100,NULL);
}

//停止漫游
void CFastDlg::OnBnClickedstop()
{
	KillTimer(1);
}

//定时器
void CFastDlg::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent==1)
	{
		char sPos[10];
		int pos = m_ScrollBar1.GetScrollPos();    // 获取滚动条当前位置  
		int speed=5;
		distance_c+=speed;
		itoa(distance_c,sPos,10);
		m_pos.SetSel(0,-1);
		m_pos.ReplaceSel(sPos);
		m_ScrollBar1.SetScrollPos(pos-speed);
		Invalidate();
	}

	CDialog::OnTimer(nIDEvent);
}

//把结果写成CString，保存成TXT
void CFastDlg::OnBnClickedSave()
{
	CString str;  //显示匹配结果
	str="编号      i         j         深        面积      长         宽";
	str+="\r\n";
	for(int i=0;i<cx_vector.size();i++){ 
		{
			CString temp;

			temp.Format(_T("%-10d%-10d%-10d%-10d%-10d%-10d%-10d"),
				cx_vector[i].index,cx_vector[i].i,cx_vector[i].j,cx_vector[i].deep,
				cx_vector[i].area,cx_vector[i].length,cx_vector[i].width);
			str=str+temp;
		}			
		str+="\r\n";
	}
	
	CFile file;
	CString FileName=outputPath+"\\"+"沉陷.txt";
	file.Open(FileName,CFile::modeCreate|CFile::modeWrite);
	file.SeekToBegin();
	file.Write(str,str.GetLength());//CString m_data
	file.Flush();
	file.Close();


	str="";  //显示匹配结果
	str="编号      i         j         高        面积      长         宽";
	str+="\r\n";
	for(int i=0;i<yb_vector.size();i++){ 
		{
			CString temp;

			temp.Format(_T("%-10d%-10d%-10d%-10d%-10d%-10d%-10d"),
				yb_vector[i].index,yb_vector[i].i,yb_vector[i].j,yb_vector[i].deep,
				yb_vector[i].area,yb_vector[i].length,yb_vector[i].width);
			str=str+temp;
		}			
		str+="\r\n";
	}
	FileName=outputPath+"\\"+"拥包.txt";
	file.Open(FileName,CFile::modeCreate|CFile::modeWrite);
	file.SeekToBegin();
	file.Write(str,str.GetLength());//CString m_data
	file.Flush();
	file.Close();

	str="";  //显示匹配结果
	str="编号      i         j         深        面积      长         宽";
	str+="\r\n";
	for(int i=0;i<cz_vector.size();i++){ 
		{
			CString temp;

			temp.Format(_T("%-10d%-10d%-10d%-10d%-10d%-10d%-10d"),
				cz_vector[i].index,cz_vector[i].i,cz_vector[i].j,cz_vector[i].deep,
				cz_vector[i].area,cz_vector[i].length,cz_vector[i].width);
			str=str+temp;
		}			
		str+="\r\n";	
	}
	FileName=outputPath+"\\"+"车辙.txt";
	file.Open(FileName,CFile::modeCreate|CFile::modeWrite);
	file.SeekToBegin();
	file.Write(str,str.GetLength());//CString m_data
	file.Flush();
	file.Close();

	MessageBox("写入成功！");
	
}

//选择保存路径
void CFastDlg::OnBnClickedSelectpath()
{
	CString ReturnPach,folder;
	TCHAR szPath[_MAX_PATH];
	BROWSEINFO bi;
	bi.hwndOwner=NULL;
	bi.pidlRoot=NULL;
	bi.lpszTitle=_T("请选择一个文件夹");
	bi.pszDisplayName=szPath;
	bi.ulFlags=BIF_RETURNONLYFSDIRS;
	bi.lpfn=NULL;
	bi.lParam=NULL;
	LPITEMIDLIST pItemIDList=SHBrowseForFolder(&bi);
	if(pItemIDList)
	{
		folder = szPath;
		if(SHGetPathFromIDList(pItemIDList,szPath))
			ReturnPach=szPath;
		SetDlgItemText(IDC_EXPORTPATH,ReturnPach);
		GetDlgItem(IDC_SAVE)->EnableWindow(true);
	}
	else
		ReturnPach="";
	outputPath = ReturnPach;
	UpdateData(false);
}

