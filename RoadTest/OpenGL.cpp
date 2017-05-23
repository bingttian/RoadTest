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
	
extern int column_c;			//宽
extern  int **data_final;
extern  int **data_show;		//重声明一个数组用来做显示，每两个点之间加了5个点用来过度
//extern int m_distance;
extern int distance_c;
extern int Pic_Size;
extern BOOL display_flag;

//"下一个"OpenGl的横线显示布尔变量
extern bool show_flag;
extern int show_line;

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
			glClear(GL_COLOR_BUFFER_BIT);  
			glMatrixMode(GL_PROJECTION);
			//glMatrixMode(GL_MODELVIEW);
			glClear (GL_COLOR_BUFFER_BIT);  
			glLoadIdentity();//加载单位矩阵  
			glRotatef(55, 1.0, 0.0, 0.0); //rotate about the x axis   
			glRotatef(-180, 0.0, 1.0, 0.0); //rotate about the y axis  

			int w=73;  
			int h=80;  

			float scalar_x=45;
			float scalar_y=50;

			float imageCenterX=w*0.5;  
			float imageCenterY=h*0.5;  
			float x,y,z; 
			int ii=distance_c;

			if(display_flag)		//点完彩色路面再显示
			{
				glBegin(GL_QUADS);		//正方形显示
				for (int i=0;i<Pic_Size;i++){   	
					for (int j=0;j<column_c*6-5;j++){  
						choose_color(data_show[i+distance_c][j]);
					
						//设置坐标
						x=((float)j-imageCenterX)/scalar_x;
						y=((float)i-imageCenterY)/scalar_y; 
						z=(float)data_show[i+distance_c][j]/10000;
						glVertex3f(x,y,z);

						choose_color(data_show[i+1+distance_c][j]);
					
						x=((float)j-imageCenterX)/scalar_x;
						y=((float)(i+1)-imageCenterY)/scalar_y; 
						z=(float)data_show[i+1+distance_c][j]/10000;
						glVertex3f(x,y,z);
					
						choose_color(data_show[i+1+distance_c][j+1]);

						x=((float)(j+1)-imageCenterX)/scalar_x;
						y=((float)(i+1)-imageCenterY)/scalar_y; 
						z=(float)data_show[i+1+distance_c][j+1]/10000;
						glVertex3f(x,y,z);
			
						choose_color(data_show[i+distance_c][j+1]);

						x=((float)(j+1)-imageCenterX)/scalar_x;
						y=((float)i-imageCenterY)/scalar_y; 
						z=(float)data_show[i+distance_c][j+1]/10000;
						glVertex3f(x,y,z);
					}  
				}
				glEnd();
				if(show_flag){
					glBegin(GL_LINES);
					glColor3f(1.0f, 0.0f, 0.0f);
					float yy=((float)show_line-imageCenterY)/scalar_y;
					glVertex3f((-imageCenterX)/scalar_x,yy,0);
					glVertex3f((81-imageCenterX)/scalar_x,yy,0);
				}
				glEnd();
			}
			glFlush();  
			SwapBuffers(hdc);
			}
         
    }  
	

	wglMakeCurrent(NULL, NULL) ; 
	::ReleaseDC (m_hWnd, hdc) ; 
	wglDeleteContext(hglrc); 
	
}

//颜色索引
void COpenGL::choose_color(int dis)
{
	if(dis<=-800){
		glColor3f(0.0f, 0.0f, 1.0f);	//1
	}
	else if(dis>-800&&dis<=-700){
		glColor3f(0.118f,0.565f,1.0f);	//2
	}
	else if(dis>-700&&dis<=-600){
		glColor3f(0.0f,0.749f,1.0f);	//3
	}
	else if(dis>-600&&dis<=-500){
		glColor3f(0.251f,0.878f,0.816f);	//4
	}
	else if(dis>-500&&dis<=-400){
		glColor3f(0.498f,1.0f,0.831f);	//5
	}
	else if(dis>-400&&dis<=-300){
		glColor3f(0.0f,0.804f,0.0f);	//6.......
	}
	else if(dis>-300&&dis<=-200){
		glColor3f(0.0f,0.902f,0.0f);	//7
	}
	else if(dis>-200&&dis<=-130){
		glColor3f(0.0f,1.0f,0.0f);	//8
	}
	else if(dis>-130&&dis<=-60){
		glColor3f(0.341f,1.0f,0.09f);	//9
	}
	else if(dis>-60&&dis<=-20){
		glColor3f(0.678f,1.0f,0.184f);	//10
	}
	else if(dis>-20&&dis<=-10){
		glColor3f(0.839f,1.0f,0.09f);	//11
	}
	else if(dis>-10&&dis<=-5){
		glColor3f(1.0f,1.0f,0.0f);	//12
	}
	else if(dis>-5&&dis<=0){
		glColor3f(1.0f,0.922f,0.0f);	//13  中间
	}
	else if(dis>0&&dis<=5){
		glColor3f(1.0f,0.843f,0.0f);	//14  
	}
	else if(dis>5&&dis<=10){
		glColor3f(0.929f,0.745f,0.063f);	//15  
	}
	else if(dis>10&&dis<=20){
		glColor3f(0.855f,0.647f,0.125f);	//16
	}
	else if(dis>20&&dis<=60){
		glColor3f(0.929f,0.647f,0.063f);	//17
	}
	else if(dis>60&&dis<=130){
		glColor3f(1.0f,0.647f,0.0f);	//18
	}
	else if(dis>130&&dis<=199){
		glColor3f(1.0f,0.647f,0.0f);	//19
	}
	else if(dis>199&&dis<=300){
		glColor3f(1.0f,0.549f,0.0f);	//20
	}
	else if(dis>300&&dis<=400){
		glColor3f(1.0f,0.388f,0.278f);	//21
	}
	else if(dis>400&&dis<=500){
		glColor3f(1.0f,0.118f,0.0f);	//22
	}
	else if(dis>500&&dis<=600){
		glColor3f(1.0f,0.078f,0.576f);	//23
	}
	else if(dis>600&&dis<=700){
		glColor3f(0.78f,0.082f,0.522f);	//24
	}
	else if(dis>700&&dis<=800){
		glColor3f(0.729f,0.333f,0.827f);	//25
	}
	else if(dis>800){
		glColor3f(0.647f,0.165f,0.165f);	//26
	}
}
