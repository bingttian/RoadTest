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
��ʼ������
*/
const int column=13;			//��
int row=0;
int column_c=column;
const int cx_standard=-200;		//���ݱ�׼
const int yb_standard=200;		//ӵ����׼
const int cz_standard=20;		//���ޱ�׼
int **data_final;
int **data_show;			//������һ��������������ʾ��ÿ������֮�����5������������
int m_distance=0;				//�������������OpenGL��ͼ·���λ��
int distance_c=0;				//������OpenGL.cpp����

int Pic_Size=80;				//���û���·��ľ��루������ʾ�ĳ��ȣ�
int ScrollToSmall=5;			//ӳ�䵽����������С������Ĭ�ϵ����ֵ��65535������10w����ʾ

CRect rect(20,75, 370, 425);	//һ�����κ�ɫ����

//"��һ��"��λ�ñ��
int index_cx=0;
int index_yb=0;
int index_cz=0;

bool show_flag=false;//"��һ��"OpenGl�ĺ�����ʾ��������

int show_line=0;//"��һ��"�ĺ���λ��

BOOL display_flag=false;

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
	m_cx = 0;		//����������Edit����ʾ�ı���
	m_cz = 0;
	m_yb = 0;
	//  m_pos = 0;
	outputPath = _T("");
	m_show_area = 0;
	m_show_deep = 0;
	m_show_length = 0;
	m_show_weith = 0;
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
	DDX_Text(pDX, IDC_SHOW_area, m_show_area);
	DDX_Text(pDX, IDC_SHOW_deep, m_show_deep);
	DDX_Text(pDX, IDC_SHOW_length, m_show_length);
	DDX_Text(pDX, IDC_SHOW_weith, m_show_weith);
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
	ON_BN_CLICKED(IDC_EIXT, &CFastDlg::OnBnClickedEixt)
	ON_BN_CLICKED(IDC_NEXT_CX, &CFastDlg::OnBnClickedNextCx)
	ON_BN_CLICKED(IDC_NEXT_YB, &CFastDlg::OnBnClickedNextYb)
	ON_BN_CLICKED(IDC_NEXT_CZ, &CFastDlg::OnBnClickedNextCz)
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
	
	

	// TODO: Add extra initialization here
	m_pDisplay->Create( NULL,   //CWnd default
						NULL,   //has no name
						WS_CHILD|WS_CLIPSIBLINGS|WS_CLIPCHILDREN|WS_VISIBLE,
						rect,
						this,   //this is the parent
						0);     //this should really be a different number... check resource.h

	//��ӹ�����
	/*m_ScrollBar1.SetScrollRange(0,row-80);
	m_ScrollBar1.SetScrollPos(row-80);*/

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

//��ȡtxt��ʽ��ԭʼ����
void CFastDlg::OnBnClickedGetfile()
{
	distance_c=0;
	index_cx=0;
	index_cz=0;
	index_yb=0;
	for (int i=0;i<row;i++)
	{
		delete [] data_show[i];
	}
	delete data_show;

	//delete data_final���ͷ��ڴ档
	for (int i=0;i<row;i++)
	{
		delete [] data_final[i];
	}
	delete data_final;
	
	CFileDialog fDlg(TRUE, _TEXT("txt"), NULL, 4|2,  
		_TEXT("�ı��ļ�(*.txt,*.ini,*.RD)|*.txt;*.ini;*.RD|ȫ���ļ�(*.*)|*.*||"));  

	// ���� DoModal() �������򿪶Ի���  
	if (fDlg.DoModal() == IDOK)  
	{  
		// ���ñ༭��1������Ϊѡ���ļ���·��  
		// GetPathName() --> ����ѡ���ļ�������·��  
		SetDlgItemText(IDC_FilePath,fDlg.GetPathName());  

		//------------------------------------------------------------------------------  
		// CFile�������⣺http://baike.baidu.com/view/994710.htm  
		//------------------------------------------------------------------------------  
		CStdioFile fFile(fDlg.GetPathName(),CFile::modeRead);  

		CString strFile ;  
		CString data;

		// ��ȡ���ļ�����  
		// ��һ����������ȡ���ݵĻ�������  
		// �ڶ�����������ȡ���ݵĳ���  
		//fFile.Read(strFile.GetBufferSetLength(fFile.GetLength()),fFile.GetLength());  

		int i=0;
		row=0;
		//����ȡһ��
		while(fFile.ReadString(strFile))
		{
			if(i==1){
				data+=strFile;
				data+=",";
				row++;
			}
			i++;
			i=i%3;
		}

		//SetDlgItemText(IDC_File,data);
		// �ر��ļ����  
		fFile.Close();  


		//short int data_split[column*row];
		 int *data_split=new int[row*column];
		int pos=0;
		//��һ��һ�ж�ȡ�������ö��ŷָ��һ��һά������
		//ÿƴ��һ��CString�ͻ�����newһ��CString�Ķ������Էָ�ʱ�������ַ���ƴ��
		for(int i=0;i<column*row;i++)							//��������̫��O(��_��)O
		{  
			data_split[i]=_ttoi(data.Tokenize(_T(","),pos));
		}			

		data_final=new  int *[row];
		for (int i=0;i<row;i++)
		{
			data_final[i]=new  int [column];
		}
		//��һά�����ɿɴ���Ķ�ά����
		for (int j=0;j<column;j++){
			for(int i=0;i<row;i++){
				data_final[i][j]=data_split[column*i+j];
			}
		}

		//��֤data_final
		/*CString str="";
		CString temp;
		for(int i=0;i<row;i++)
		{
		for(int j=0;j<column;j++) 
		{
		temp.Format(_T("%-4d  "),data_final[i][j]);
		str=str+temp;
		}
		str+="\r\n";
		}
		SetDlgItemText(IDC_EDIT1,str);*/

	}
}


void CFastDlg::OnEnChangeFilepath()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialog::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}

//���������ṹ�����ڴ�Ŵ����ĳ���ӵ����������
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
// Parameter: int data[row][column]	ԭʼ����
// Parameter: int flag[row][column]	�������
// Parameter: int i	
// Parameter: int j
// Parameter: int index				���ֵ
//************************************
//����ͨ���㷨�������ͬʱ��¼�³�������Ȼ��߸߶ȣ����
void mark_cx( int **data, int **flag,int i,int j,int index){
	//����Ǻ�
	flag[i][j] = index;
	//��¼i��j����ֵ������㳤��
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
	//�����
	if(deep>data[i][j]){
		deep=data[i][j];
	}
	//�����
	area++;
	//int len = row;
	//�ݹ��㷨
	if (j + 1 < column && data[i][j + 1] <= cx_standard && flag[i][j + 1] == 0) {
		mark_cx(data, flag, i, j + 1, index);
	}
	if (i + 1 < row && j + 1 < column && data[i + 1][j + 1] <= cx_standard && flag[i + 1][j + 1] == 0) {
		mark_cx(data, flag, i + 1, j + 1, index);
	}
	if (i + 1 < row && data[i + 1][j] <= cx_standard && flag[i + 1][j] == 0) {
		mark_cx(data, flag, i + 1, j, index);
	}
	if (i + 1 < row && j - 1 >= 0 && data[i + 1][j - 1] <= cx_standard && flag[i + 1][j - 1] == 0) {
		mark_cx(data, flag, i + 1, j - 1, index);
	}
	if (j - 1 >= 0 && data[i][j - 1] <= cx_standard && flag[i][j - 1] == 0) {
		mark_cx(data, flag, i, j - 1, index);
	}
	if ( i- 1>= 0 && j-1>0 && data[i-1][j-1] <= cx_standard && flag[i-1][j-1] == 0){
		mark_cx(data, flag, i-1, j-1, index);
	}
	if (i-1>=0 && data[i-1][j]<= cx_standard && flag[i-1][j]==0){
		mark_cx(data, flag, i-1, j, index);
	}
	if (i-1>=0 && j+1<column && data[i-1][j+1]<= cx_standard && flag[i-1][j+1]==0){
		mark_cx(data, flag, i-1, j+1, index);
	}
}

//������һ����һ���Ǵ��ڣ�һ����С��
void mark_yb(int **data, int** flag,int i,int j,int index){
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
	if (j + 1 < column && data[i][j + 1] >= yb_standard && flag[i][j + 1] == 0) {
		mark_yb(data, flag, i, j + 1, index);
	}
	if (i + 1 < row && j + 1 < column && data[i + 1][j + 1] >= yb_standard && flag[i + 1][j + 1] == 0) {
		mark_yb(data, flag, i + 1, j + 1, index);
	}
	if (i + 1 < row && data[i + 1][j] >= yb_standard && flag[i + 1][j] == 0) {
		mark_yb(data, flag, i + 1, j, index);
	}
	if (i + 1 < row && j - 1 >= 0 && data[i + 1][j - 1] >= yb_standard && flag[i + 1][j - 1] == 0) {
		mark_yb(data, flag, i + 1, j - 1, index);
	}
	if (j - 1 >= 0 && data[i][j - 1] >= yb_standard && flag[i][j - 1] == 0) {
		mark_yb(data, flag, i, j - 1, index);
	}
	if ( i- 1>= 0 && j-1>0 && data[i-1][j-1] >= yb_standard && flag[i-1][j-1] == 0){
		mark_yb(data, flag, i-1, j-1, index);
	}
	if (i-1>=0 && data[i-1][j]>= yb_standard && flag[i-1][j]==0){
		mark_yb(data, flag, i-1, j, index);
	}
	if (i-1>=0 && j+1<column && data[i-1][j+1]>= yb_standard && flag[i-1][j+1]==0){
		mark_yb(data, flag, i-1, j+1, index);
	}
}
//*************************************

//��������
void CFastDlg::OnBnClickedCalculate()
{
	cx_vector.clear();
	yb_vector.clear();
	cz_vector.clear();
	int index_cx=1;
	int index_yb=1;
	int index_cz=1;
	static int **flag_cx;
	static  int **flag_yb;
	flag_cx=new int *[row];
	for (int i=0;i<row;i++)
	{
		flag_cx[i]=new  int [column];
	}
	flag_yb=new  int *[row];
	for (int i=0;i<row;i++)
	{
		flag_yb[i]=new  int [column];
	}
	for (int i=0;i<row;i++)
	{
		for (int j=0;j<column;j++)
		{
			flag_cx[i][j]=0;
			flag_yb[i][j]=0;
		}
	}
	for (int i = 0; i < row; i++) {
		for (int j = 0; j < column; j++) {
			chenxian c;
			yongbao y;
			chezhe z;
			if (data_final[i][j] < cx_standard && flag_cx[i][j] == 0) {
				//��ʼ��
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

			//������һ��
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

	for (int i=0;i<row;i++)
	{
		delete [] flag_cx[i];
	}
	delete flag_cx;
	for (int i=0;i<row;i++)
	{
		delete [] flag_yb[i];
	}
	delete flag_yb;

	m_cx=cx_vector.size();
	m_yb=yb_vector.size();
	m_cz=cz_vector.size();
	UpdateData(false);
}

void Chazhi(int **data)
{
	data_show=new int *[row];
	for (int i=0;i<row;i++)
	{
		data_show[i]=new int [column*6-5];
	}
	for(int i=0;i<row;i++)
	{
		for(int j=0;j<column-1;j++)
		{
			for(int k=0;k<6;k++)	//����֮�����5����
			{	
				data_show[i][j*6+k]=(data[i][j+1]-data[i][j])/6*k+data[i][j];
				int temp=data_show[i][j*6+k];
			}
		}
		data_show[i][column*6-6]=data[i][column];
	}
}

//ˢ��һ�£����µ���OnPaint��������Ķ�ά������ʾ����
void CFastDlg::OnBnClickedColor()
{
	Chazhi(data_final);//���ԵĲ�ֵ
	display_flag=true;
	//ˢ�¹��������ı�rowֵ
	m_ScrollBar1.SetScrollRange(0,(row-Pic_Size)/ScrollToSmall);	//���ù�������Χ
	m_ScrollBar1.SetScrollPos((row-Pic_Size)/ScrollToSmall);		//���ù�������ʼλ��

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

//������
void CFastDlg::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	//int iNowPos;
	int pos = m_ScrollBar1.GetScrollPos();    // ��ȡ��������ǰλ��  
	char sPos[10];
	switch(nSBCode)
	{
	case SB_THUMBTRACK:	//���϶�������ʱ
			pos=nPos;
			m_distance=pos;
			itoa((row-Pic_Size-m_distance*ScrollToSmall),sPos,10);
			show_line-=(row-Pic_Size-m_distance*ScrollToSmall);
			m_pos.SetSel(0,-1);
			m_pos.ReplaceSel(sPos);
			//Invalidate();
			break;
	case  SB_LINEDOWN:	
			pos+=1;
			if(pos>(row-Pic_Size)/ScrollToSmall){
				pos=(row-Pic_Size)/ScrollToSmall;
			}else if(show_line<row/ScrollToSmall)
				show_line+=ScrollToSmall;
			m_distance=pos;
			itoa((row-Pic_Size-m_distance*ScrollToSmall),sPos,10);
			m_pos.SetSel(0,-1);
			m_pos.ReplaceSel(sPos);
			//Invalidate();
			break;
	case SB_LINEUP:
			pos-=1;
			show_line-=ScrollToSmall;
			if(pos<0){
				pos=0;
			}
			m_distance=pos;
			itoa((row-Pic_Size-m_distance*ScrollToSmall),sPos,10);
			m_pos.SetSel(0,-1);
			m_pos.ReplaceSel(sPos);
			//Invalidate();
			break;
	case SB_PAGEDOWN:
			if(show_line<row/ScrollToSmall)
				show_line+=ScrollToSmall*4;
			pos+=4;
			m_distance=pos;
			itoa((row-Pic_Size-m_distance*ScrollToSmall),sPos,10);
			m_pos.SetSel(0,-1);
			m_pos.ReplaceSel(sPos);
			//Invalidate();
			break;
	case SB_PAGEUP:
			show_line-=ScrollToSmall*4;
			pos-=4;
			m_distance=pos;
			itoa((row-Pic_Size-m_distance*ScrollToSmall),sPos,10);
			m_pos.SetSel(0,-1);
			m_pos.ReplaceSel(sPos);
			//Invalidate();
			break;
	}
	//Ϊ���ò�������ʵ��·����ʾ�߼����ù��������������ƶ�����������������ʱ��ʾλ��Ϊ0
	if(m_distance<=0)
	{
		distance_c =row-Pic_Size-1;
	}else{
	distance_c = row-Pic_Size-m_distance*ScrollToSmall;
	}
	m_ScrollBar1.SetScrollPos(pos);
	//ֻˢ��rect���򣬽����˸����÷���
	InvalidateRect(&rect, FALSE); 
	CDialog::OnVScroll(nSBCode, nPos, pScrollBar);
}


BOOL CFastDlg::OnEraseBkgnd(CDC* pDC)
{
	//Invalidate(FALSE);
	//return TRUE;
	return CDialog::OnEraseBkgnd(pDC);
}

//��ʼ���Σ�����OnTimer
void CFastDlg::OnBnClickedstart()
{	
	if(display_flag)
	{
		SetTimer(1,100,NULL);
	}	
}

//ֹͣ����
void CFastDlg::OnBnClickedstop()
{
	KillTimer(1);
}

//��ʱ��
void CFastDlg::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent==1)
	{
		char sPos[10];
		int pos = m_ScrollBar1.GetScrollPos();    // ��ȡ��������ǰλ��  
		int speed=5;
		if (distance_c+speed>=row-Pic_Size)
		{
			KillTimer(1);
		}else{
			distance_c+=speed;
			show_line-=speed;
		}
		itoa((row-Pic_Size-pos*ScrollToSmall),sPos,10);
		m_pos.SetSel(0,-1);
		m_pos.ReplaceSel(sPos);
		m_ScrollBar1.SetScrollPos(pos-speed);
		//Invalidate();
		InvalidateRect(&rect, FALSE); 
	}

	CDialog::OnTimer(nIDEvent);
}

//�ѽ��д��CString�������TXT
void CFastDlg::OnBnClickedSave()
{
	CString str;  //��ʾƥ����
	str="���      i         j         ��        ���      ��         ��";
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
	CString FileName=outputPath+"\\"+"����.txt";
	file.Open(FileName,CFile::modeCreate|CFile::modeWrite);
	file.SeekToBegin();
	file.Write(str,str.GetLength());//CString m_data
	file.Flush();
	file.Close();


	str="";  //��ʾƥ����
	str="���      i         j         ��        ���      ��         ��";
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
	FileName=outputPath+"\\"+"ӵ��.txt";
	file.Open(FileName,CFile::modeCreate|CFile::modeWrite);
	file.SeekToBegin();
	file.Write(str,str.GetLength());//CString m_data
	file.Flush();
	file.Close();

	str="";  //��ʾƥ����
	str="���      i         j         ��        ���      ��         ��";
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
	FileName=outputPath+"\\"+"����.txt";
	file.Open(FileName,CFile::modeCreate|CFile::modeWrite);
	file.SeekToBegin();
	file.Write(str,str.GetLength());//CString m_data
	file.Flush();
	file.Close();

	MessageBox("д��ɹ���");
	
}

//ѡ�񱣴�·��
void CFastDlg::OnBnClickedSelectpath()
{
	CString ReturnPach,folder;
	TCHAR szPath[_MAX_PATH];
	BROWSEINFO bi;
	bi.hwndOwner=NULL;
	bi.pidlRoot=NULL;
	bi.lpszTitle=_T("��ѡ��һ���ļ���");
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


void CFastDlg::OnBnClickedEixt()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDialog::OnOK();
}


void CFastDlg::OnBnClickedNextCx()
{
	char sPos[10];
	if(index_cx<cx_vector.size()){
		show_line=0;
		distance_c=cx_vector[index_cx].i;
		show_flag=true;
		m_show_length=cx_vector[index_cx].length;
		m_show_weith=cx_vector[index_cx].width;
		m_show_deep=cx_vector[index_cx].deep;
		m_show_area=cx_vector[index_cx].area;
		int pos = m_ScrollBar1.GetScrollPos();    // ��ȡ��������ǰλ��
		itoa((cx_vector[index_cx].i),sPos,10);
		m_pos.SetSel(0,-1);
		m_pos.ReplaceSel(sPos);
		m_ScrollBar1.SetScrollPos(((row-Pic_Size-distance_c)/ScrollToSmall));
		UpdateData(false);
		InvalidateRect(&rect, FALSE); 
		//show_cx=false;
		index_cx++;
	}
}


void CFastDlg::OnBnClickedNextYb()
{
	char sPos[10];
	if(index_yb<yb_vector.size()){
		show_line=0;
		distance_c=yb_vector[index_yb].i;
		show_flag=true;
		m_show_length=yb_vector[index_yb].length;
		m_show_weith=yb_vector[index_yb].width;
		m_show_deep=yb_vector[index_yb].deep;
		m_show_area=yb_vector[index_yb].area;
		int pos = m_ScrollBar1.GetScrollPos();    // ��ȡ��������ǰλ��
		itoa((yb_vector[index_yb].i),sPos,10);
		m_pos.SetSel(0,-1);
		m_pos.ReplaceSel(sPos);
		m_ScrollBar1.SetScrollPos(((row-Pic_Size-distance_c)/ScrollToSmall));
		UpdateData(false);
		InvalidateRect(&rect, FALSE); 
		//show_yb=false;
		index_yb++;
	}
}


void CFastDlg::OnBnClickedNextCz()
{
	char sPos[10];
	if(index_cz<cz_vector.size()){
		show_line=0;
		distance_c=cz_vector[index_cz].i;
		show_flag=true;
		m_show_length=cz_vector[index_cz].length;
		m_show_weith=cz_vector[index_cz].width;
		m_show_deep=cz_vector[index_cz].deep;
		m_show_area=cz_vector[index_cz].area;
		int pos = m_ScrollBar1.GetScrollPos();    // ��ȡ��������ǰλ��	
		m_ScrollBar1.SetScrollPos(((row-Pic_Size-distance_c)/ScrollToSmall));
		itoa((cz_vector[index_cz].i),sPos,10);
		m_pos.SetSel(0,-1);
		m_pos.ReplaceSel(sPos);
		UpdateData(false);
		InvalidateRect(&rect, FALSE); 
		//show_cz=false;
		index_cz++;
	}
}
