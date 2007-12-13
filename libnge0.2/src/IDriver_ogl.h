#ifndef _IDRIVER_OGL_H_
#define _IDRIVER_OGL_H_
#include <gl/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glaux.h>
#include "math_compatible.h"
#include "datatype.h"
#include "SDL.h"
/**
 *  图象显示接口，有2个实现分别对应2个平台。此处不用派生为了避免虚函数的开销。
 */
class IVideoDriver
{
public:
      IVideoDriver();
      /**
       * 显示开始函数，所有的显示函数都必须在BeginScene()和EndScene()之间。
       * 作用是清屏和一些初始化工作。默认是用黑色0xff000000清屏。可以用
       * SetClearColor()改变默认清屏颜色。
       * @param flag int,是否清屏的标致，默认为true。
       * @return 无返回值. 
       */
	void BeginScene(int flag = 1);
      
      /**
       * 显示结束函数，所有的显示函数都必须在BeginScene()和EndScene()之间。
       * 作用是提交显示并刷新显示。
       * @return 无返回值. 
       */
	void EndScene();
      
      /**
       * 2d图象显示函数，提供了旋转，缩放，混色功能。用于显示2d图片。
       * @param texture CTexture*, 由图象加载函数LoadTexture()得到的CTexture*.
       * @param sx      float,x(left)   需要显示图片部分的x坐标.默认是原图（下同）。这4个参数是
       *                为了只显示图片上的某一部分而设置的。
       * @param sy      float,y(top)    需要显示图片部分的y坐标.
       * @param sw      float,w(right)  需要显示图片部分的宽.
       * @param sh      float,h(bottom) 需要显示图片部分的高.
       * @param dx      float,将显示在屏幕的x坐标.
       * @param dy      float,将显示在屏幕的y坐标.
       * @param xscale  float,x（水平）方向上缩放比例.默认为参数1.0，不做缩放。
       * @param yscale  float,y（垂直）方向上缩放比例.默认为参数1.0，不做缩放。
       * @param angle   float,旋转角度正代表逆时针，负代表顺时针方向,旋转中心由CTexture的rcentrex和rcentrey指定，
       *		绕旋转中心旋转这个角度，默认旋转中心是图片的中心，可以通过设置rcentrex和rcentrey
       *		来改变这个旋转中心。默认参数是0。
       * @param color   int,混色的颜色，用来做图片的alpha混合。用宏RGBA(r,g,b,a)来生成这个颜色
       *                如果RGBA(r,g,b,a)中的a值为0那么图片为透明.默认参数是显示原图。
       *                例如要将图片半透明显示可以设置RGBA(0xff,0xff,0xff,0x7f)。显示原图就是RGBA(0xff,0xff,0xff,0xff)。
       * @return 无返回值.
       * @see 用RenderQuadFast()来快速显示原图，如果不需要缩放和旋转的话,会有一定的速度提升。
       */
	void RenderQuad(CTexture* texture,float sx,float sy,float sw,float sh,float dx,float dy,
				float xscale = 1.0,float yscale = 1.0,float angle = 0.0,int color=0xffffffff);
      /**
       * 2d图象显示函数，提供了混色功能。用于显示2d图片。
       * 这个是RenderQuad()的简化版.
       * @param texture CTexture*, 由图象加载函数LoadTexture()得到的CTexture*.
       * @param dx      float,将显示在屏幕的x坐标.
       * @param dy      float,将显示在屏幕的y坐标.
       * @see 如果需要旋转，缩放，混色采用RenderQuad().
       * @return 无返回值. 
       */		
	void RenderQuadFast(CTexture* texture,float dx,float dy);
      
      /**
       * 设置清屏的颜色值，用宏RGBA(r,g,b,a)生成这个颜色值。
       * @param color int,清屏的颜色值。
       */
       void SetClearColor(int color);
      
      /**
       * 获取清屏的颜色值。
       * @return 颜色值. 
       */
       int GetClearColor(); 
      
      /**
       * 调试函数，初始化fps记数器
       * @return 无返回值. 
       */
       void FpsInit() ;
      
      /**
       * 调试函数，用于显示当前的fps，需要先用FpsInit()初始化，不然会得到错误结果.
       * @return 无返回值. 
       */
       void ShowFps() ; 
       
       /**
        * 画直线的函数，传入参数是4个float值。
        * @param x1 float,直线第一个点的x坐标。
        * @param y1 float,直线第一个点的y坐标。
        * @param x2 float,直线第二个点的x坐标。
        * @param y2 float,直线第二个点的y坐标。
        * @param color int,直线的颜色，用宏RGBA生成这个颜色。
        * @return 无返回值。
        */
       void DrawLine(float x1, float y1, float x2, float y2, int color) ;
       
       /**
        * 画直线的函数，传入参数是2个CPointf坐标值。
        * @param p1 CPointf,直线第一个点的坐标。
        * @param p2 Cpointf,直线第二个点的坐标。
        * @param color int,直线的颜色，用宏RGBA生成这个颜色。
        * @return 无返回值。
        */
       void DrawLine(CPointf p1, CPointf p2, int color) ;
      
       /**
        * 画矩形线框的函数，传入参数是4个float值。
        * @param x float,画到屏幕上的x坐标。
        * @param y float,画到屏幕上的y坐标。
        * @param width float,矩形线框的宽。
        * @param height float,矩形线框的高。
        * @param color int,线框颜色。
        * @return 无返回值。
        */
       void  DrawRect(float x, float y, float width, float height,int color) ;
       
       /**
        * 画矩形线框的函数，传入参数是CRectf值。
        * @param rect CRectf,矩形线框的参数。
        * @param color int,线框颜色。
        * @return 无返回值。
        */
       void DrawRect(CRectf rect,int color) ;
       
       /**
        * 画实心矩形的函数，传入参数是4个float值。
        * @param x float,画到屏幕上的x坐标。
        * @param y float,画到屏幕上的y坐标。
        * @param width float,矩形的宽。
        * @param height float,矩形的高。
        * @param color int,实心矩形颜色。
        * @return 无返回值。
        */
       void  FillRect(float x, float y, float width, float height,int color) ;
       
       /**
        * 画实心矩形的函数，传入参数是CRectf值。
        * @param rect CRectf,矩形的参数。
        * @param color int,实心矩形颜色。
        * @return 无返回值。
        */
       void FillRect(CRectf rect,int color) ;
       
	   /**
        * 画圆形线框的函数。
        * @param x float,圆心的顶点的x坐标。
		* @param y float,圆心的顶点的y坐标。
		* @param radius float ,圆的半径。
        * @param color int,圆形线框的颜色。
        * @return 无返回值。
        */
	   void DrawCircle(float x, float y, float radius, int color);
	   /**
        * 画实心圆形的函数。
        * @param x float,圆心的顶点的x坐标。
		* @param y float,圆心的顶点的y坐标。
		* @param radius float ,圆的半径。
        * @param color int,实心圆形的颜色。
        * @return 无返回值。
        */
	   void FillCircle(float x, float y, float radius, int color);
	   /**
        * 画椭圆线框的函数。
        * @param x float,圆心的顶点的x坐标。
		* @param y float,圆心的顶点的y坐标。
		* @param xradius float ,椭圆x的半径。
		* @param yradius float ,椭圆y的半径。
        * @param color int,椭圆线框的颜色。
        * @return 无返回值。
        */
	   void DrawEllipse(float x,float y ,float xradius,   float yradius , int color) ;
	   /**
        * 画实心椭圆的函数。
        * @param x float,圆心的顶点的x坐标。
		* @param y float,圆心的顶点的y坐标。
		* @param xradius float ,椭圆x的半径。
		* @param yradius float ,椭圆y的半径。
        * @param color int,实心椭圆的颜色。
        * @return 无返回值。
        */
	   void FillEllipse(float x,float y ,float xradius,   float yradius , int color) ;
	   
	   void FillPie(float x, float y, float radius,float theta, int color);
	   /**
        * 画实心凸多边形的函数。
        * @param array CPointf*,凸多边形的顶点数组。
        * @param color int,凸多边形的颜色。
        * @return 无返回值。
        */
	   void FillPolygon(CPointf* array, int count, int color);
	   /**
        * 画凸多边形线框的函数。
        * @param CPointf* array,凸多边形的顶点数组。
        * @param color int,凸多边形线框的颜色。
        * @return 无返回值。
        */
	   void DrawPolygon(CPointf* array, int count, int color);
	   void FillRoundRect(float x, float y, float w, float h, float radius, int color);
private:
	int m_colorint;
	color4f m_clrcolor;
	float m_sintable[360];
	float m_costable[360];
	int m_update;
	int m_frame;
	int m_t0;
};


#endif
