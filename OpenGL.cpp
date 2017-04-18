// OpenGL.cpp : implementation file
//

#include "stdafx.h"
#include "RoadTest.h"
#include "OpenGL.h"
#include <gl/gl.h>
#include <gl/glu.h>
//#include "value.h "
#include "string"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COpenGL
	
//const int row=6651;				//长
extern int column_c;		//宽
extern int row_c;
extern short int data_final[101376][13];
//extern int m_distance;
extern int distance_c;
extern int Pic_Size;

COpenGL::COpenGL()
{
}

COpenGL::~COpenGL()
{
}


BEGIN_MESSAGE_MAP(COpenGL, CWnd)
	//{{AFX_MSG_MAP(COpenGL)
	ON_WM_CREATE()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


//****************************************************************************************************************************
int MySetPixelFormat(HDC hdc)
{
	PIXELFORMATDESCRIPTOR pfd = { 
	    sizeof(PIXELFORMATDESCRIPTOR),    // size of this pfd 
	    1,                                // version number 
	    PFD_DRAW_TO_WINDOW |              // support window 
	    PFD_SUPPORT_OPENGL |              // support OpenGL 
	    PFD_DOUBLEBUFFER,                 // double buffered 
	    PFD_TYPE_RGBA,                    // RGBA type 
	    24,                               // 24-bit color depth 
	    0, 0, 0, 0, 0, 0,                 // color bits ignored 
	    0,                                // no alpha buffer 
	    0,                                // shift bit ignored 
	    0,                                // no accumulation buffer 
	    0, 0, 0, 0,                       // accum bits ignored 
	    32,                               // 32-bit z-buffer     
	    0,                                // no stencil buffer 
	    0,                                // no auxiliary buffer 
	    PFD_MAIN_PLANE,                   // main layer 
	    0,                                // reserved 
	    0, 0, 0                           // layer masks ignored 
	}; 
	
	int  iPixelFormat; 
 
	// get the device context's best, available pixel format match 
	if((iPixelFormat = ChoosePixelFormat(hdc, &pfd)) == 0)
	{
		MessageBox(NULL, "ChoosePixelFormat Failed", NULL, MB_OK);
		return 0;
	}
	 
	// make that match the device context's current pixel format 
	if(SetPixelFormat(hdc, iPixelFormat, &pfd) == FALSE)
	{
		MessageBox(NULL, "SetPixelFormat Failed", NULL, MB_OK);
		return 0;
	}

	return 1;
}

/////////////////////////////////////////////////////////////////////////////
// COpenGL message handlers

int COpenGL::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	MySetPixelFormat(::GetDC(m_hWnd));

	return 0;
}

void COpenGL::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	HDC hdc = ::GetDC(m_hWnd);
	HGLRC hglrc;

	glClearColor(0,0,0,0);
	glColor3f(1, 1, 1);

	if (hglrc = wglCreateContext(hdc))
	{ 
        // try to make it the thread's current rendering context 
        if(wglMakeCurrent(hdc, hglrc))
		{
			glViewport(-15,-100,380,550);

			glMatrixMode(GL_PROJECTION);
			glClear (GL_COLOR_BUFFER_BIT);  
			glLoadIdentity();//加载单位矩阵  
			glRotatef(55, 1.0, 0.0, 0.0); //rotate about the x axis   
			glRotatef(-180, 0.0, 1.0, 0.0); //rotate about the y axis  

			int w=13;  
			int h=80;  

			float scalar_x=8;
			float scalar_y=50;

			float imageCenterX=w*0.5;  
			float imageCenterY=h*0.5;  
			float x,y,z;  
			
			glBegin(GL_QUADS);		//正方形显示
			for (int i=0;i<Pic_Size;i++){   
				for (int j=0;j<column_c;j++){  
					// color interpolation			//红 绿 蓝
					//设置颜色
					//1
					if(data_final[i+distance_c][j]<-35){
						glColor3f(0.0, 1, 0.0);}
					else if(data_final[i+distance_c][j]<0&&data_final[i+distance_c][j]>=-35){
						glColor3f(0.1,(float)(-data_final[i+distance_c][j])/35+0.1, 0.1);}
					else if(data_final[i+distance_c][j]>=0&&data_final[i+distance_c][j]<=35){
						glColor3f((float)data_final[i+distance_c][j]/35+0.1, 0.1, 0.1);}
					else{
						glColor3f(1.0, 0.0, 0.0);}
					
					//设置坐标
					x=((float)j-imageCenterX)/scalar_x;
					y=((float)i-imageCenterY)/scalar_y; 
					z=(float)data_final[i+distance_c][j]/1000;
					glVertex3f(x,y,z);

					//2
					if(data_final[i+1+distance_c][j]<-35){
						glColor3f(0.0, 1, 0.0);}
					else if(data_final[i+1+distance_c][j]<0&&data_final[i+1+distance_c][j]>=-35){
						glColor3f(0.1,(float)(-data_final[i+1+distance_c][j])/35+0.1, 0.1);}
					else if(data_final[i+1+distance_c][j]>=0&&data_final[i+1+distance_c][j]<=35){
						glColor3f((float)data_final[i+1+distance_c][j]/35+0.1, 0.1, 0.1);}
					else{
						glColor3f(1.0, 0.0, 0.0);}
					
					x=((float)j-imageCenterX)/scalar_x;
					y=((float)(i+1)-imageCenterY)/scalar_y; 
					z=(float)data_final[i+1+distance_c][j]/1000;
					glVertex3f(x,y,z);
					
					//3
					if(data_final[i+1+distance_c][j+1]<-35){
						glColor3f(0.0, 1, 0.0);}
					else if(data_final[i+1+distance_c][j+1]<0&&data_final[i+1+distance_c][j+1]>=-35){
						glColor3f(0.1,(float)(-data_final[i+1+distance_c][j+1])/35+0.1, 0.1);}
					else if(data_final[i+1+distance_c][j+1]>=0&&data_final[i+1+distance_c][j+1]<=35){
						glColor3f((float)data_final[i+1+distance_c][j+1]/35+0.1, 0.1, 0.1);}
					else{
						glColor3f(1.0, 0.0, 0.0);}

					x=((float)(j+1)-imageCenterX)/scalar_x;
					y=((float)(i+1)-imageCenterY)/scalar_y; 
					z=(float)data_final[i+1+distance_c][j+1]/1000;
					glVertex3f(x,y,z);
					
					//4
					if(data_final[i+distance_c][j+1]<-35){
						glColor3f(0.0, 1, 0.0);}
					else if(data_final[i+distance_c][j+1]<0&&data_final[i+distance_c][j+1]>=-35){
						glColor3f(0.1,(float)(-data_final[i+distance_c][j+1])/35+0.1, 0.1);}
					else if(data_final[i+distance_c][j+1]>=0&&data_final[i+distance_c][j+1]<=35){
						glColor3f((float)data_final[i+distance_c][j+1]/35+0.1, 0.1, 0.1);}
					else{
						glColor3f(1.0, 0.0, 0.0);}

					x=((float)(j+1)-imageCenterX)/scalar_x;
					y=((float)i-imageCenterY)/scalar_y; 
					z=(float)data_final[i+distance_c][j+1]/1000;
					glVertex3f(x,y,z);
				}  
			}  
			glEnd();  
			glFlush();  
			SwapBuffers(hdc);
        } 
    }  

	wglMakeCurrent(NULL, NULL) ; 
	::ReleaseDC (m_hWnd, hdc) ; 
	wglDeleteContext(hglrc); 

	// Do not call CWnd::OnPaint() for painting messages
}
