#include "GuSystem.h"

static unsigned int __attribute__((aligned(16))) list[262144];

#define BUF_WIDTH (512)
#define SCR_WIDTH (480)
#define SCR_HEIGHT (272)

#define PSP_SLICE_F 0.03125f
#define PSP_PI_DIV_180 0.0174532925

struct Vertex
{
	float u, v;
	unsigned int color;
	float x,y,z;
};


static void swizzle_fast(u8* out, const u8* in, unsigned int width, unsigned int height)
{
   unsigned int blockx, blocky;
   unsigned int j;
 
   unsigned int width_blocks = (width / 16);
   unsigned int height_blocks = (height / 8);
 
   unsigned int src_pitch = (width-16)/4;
   unsigned int src_row = width * 8;
 
   const u8* ysrc = in;
   u32* dst = (u32*)out;
 
   for (blocky = 0; blocky < height_blocks; ++blocky)
   {
      const u8* xsrc = ysrc;
      for (blockx = 0; blockx < width_blocks; ++blockx)
      {
         const u32* src = (u32*)xsrc;
         for (j = 0; j < 8; ++j)
         {
            *(dst++) = *(src++);
            *(dst++) = *(src++);
            *(dst++) = *(src++);
            *(dst++) = *(src++);
            src += src_pitch;
         }
         xsrc += 16;
     }
     ysrc += src_row;
   }
}

CGuSystem::CGuSystem()
{
	InitGu();
	m_transmatrix.x=0.0;
	m_transmatrix.y=0.0;
	m_transmatrix.z=0.0;
	
}

void CGuSystem::BeginScene()
{
	sceGuStart(GU_DIRECT,list);
	sceGuClearColor(0);
	sceGuClear(GU_COLOR_BUFFER_BIT);
	sceGumMatrixMode(GU_PROJECTION);
	sceGumLoadIdentity();
	sceGumOrtho(0, 480, 272, 0, -1, 1);
}

void CGuSystem::EndScene()
{
	sceGuFinish();
	sceGuSync(0,0);
	#ifdef RELEASE
	m_drawbuf = sceGuSwapBuffers();
	#endif

	
}

static inline int RoundUpToPowerOfTwo(int x)
{
	int b = x;
	int n;
	for (n = 0; b != 0; n++) b >>= 1;
	b = 1 << n;
	if (b == 2 * x) b >>= 1;
	return b;
}
#ifdef RELEASE
void CGuSystem::FpsInit()
{
}
#else
void CGuSystem::FpsInit()
{
	
	m_currms = 1.0;
	m_fcount = 0;
	sceRtcGetCurrentTick(&m_lasttick);
	m_tickres = sceRtcGetTickResolution();
}
#endif

#include <malloc.h> 
int __freemem() 
{ 
 void *ptrs[480]; 
 int mem, x, i; 
 for (x = 0; x < 480; x++) 
 { 
    void *ptr = malloc(51200); 
    if (!ptr) break; 
  
    ptrs[x] = ptr; 
 } 
 mem = x * 51200; 
 for (i = 0; i < x; i++) 
  free(ptrs[i]); 

 return mem; 
}

#ifdef RELEASE
void CGuSystem::ShowFps()
{
}
#else
void CGuSystem::ShowFps()
{
	static int tick = 10000;
	static int freemem = 0;
	++m_fcount;
	sceRtcGetCurrentTick(&m_currtick);
	//if ((m_currtick-m_lasttick) >= m_tickres)
	{ 
		pspDebugScreenSetOffset((int)m_drawbuf);
		pspDebugScreenSetXY(0,0);
		float curr_fps = 1.0f / m_currms;
		if(tick>curr_fps*10){
			tick = 0;
			freemem = __freemem();
		}
		tick++;
		pspDebugScreenPrintf("fps: %d.%03d \tfreemem: %d byte(%.2fM)",(int)curr_fps,(int)((curr_fps-(int)curr_fps) * 1000.0f),freemem,freemem*1.0/(1024*1024) );
		double time_span = ((m_currtick-m_lasttick)) / (float)m_tickres;
		m_currms = time_span / m_fcount;
		m_fcount = 0;
		sceRtcGetCurrentTick(&m_lasttick);
	}
	m_drawbuf = sceGuSwapBuffers();
}
#endif


void CGuSystem::RenderQuad(CTexture* tex,float sx,float sy,float sw,float sh,float dx,float dy,float xscale /* = 1.0 */,float yscale /* = 1.0 */,float angle /* = 1.0 */,int color)
{
	//sceGumMatrixMode(GU_PROJECTION);
	//sceGumLoadIdentity();
	//sceGumOrtho(0, 480, 272, 0, -1, 1);
	
	//sdk bug: if-else destroy the matrix,
        //so I should push the current matrix :( .
        //I hope it will be fixed later.
        //sceGumPushMatrix();
        //prevent div 0
	static int lastid = 0;
	if((xscale==0)||(yscale==0)){
		xscale=1.0;
		yscale=1.0;
	} 
	
	if(sw==0){
	  	sw=tex->w*xscale;
	  	sh=tex->h*yscale;
	}
	else{
		sw = sw*xscale;
        	sh = sh*yscale;
        }
        
	float su = sw/tex->texw;
        float sv = sh/tex->texh;
        float swf = tex->texw;
       sceGumMatrixMode(GU_VIEW);
	sceGumLoadIdentity();
 
	sceGumMatrixMode(GU_MODEL);
	sceGumLoadIdentity();
	
	
	
	
        m_transmatrix.x = dx;
	 m_transmatrix.y = dy;
        sceGumTranslate(&m_transmatrix);
        ScePspFVector3 tmp;
        tmp.x = tex->rcentrex*xscale;
        tmp.y = tex->rcentrey*yscale;
        tmp.z = 0.0;
        sceGumTranslate(&tmp);
        sceGumRotateZ(angle*PSP_PI_DIV_180); //angle * (GU_PI/180.0f)
        tmp.x = -tex->rcentrex*xscale;
        tmp.y = -tex->rcentrey*yscale;
        tmp.z = 0.0; 
        sceGumTranslate(&tmp);
        
        
       
 	sceGuTexMode(tex->mode,0,0,tex->swizzle);
 	sceKernelDcacheWritebackAll();
 	sceGuTexOffset(sx/tex->texw,sy/tex->texh);
 	sceGuTexScale(1.0/xscale,1.0/yscale);
	if(tex->texid != lastid){
 		sceGuTexImage(0,tex->texw,tex->texh,tex->texw,tex->data);
 		sceKernelDcacheWritebackAll();
		lastid = tex->texid;
	}
	sceGuTexFunc(GU_TFX_MODULATE,GU_TCC_RGBA);
	sceGuTexFilter(GU_LINEAR,GU_LINEAR);
	
	
	for(float ustart=0,step = 0;ustart<su;ustart+=PSP_SLICE_F,step+=swf*PSP_SLICE_F){
		struct Vertex* vertices = (struct Vertex*)sceGuGetMemory(4 * sizeof(struct Vertex));
		double width = (ustart+PSP_SLICE_F ) < su ? (PSP_SLICE_F) : su-ustart;
				
		vertices[0].u = ustart; 
		vertices[0].v = 0;
		vertices[0].color = color;
		vertices[0].x = ustart*swf; 
		vertices[0].y = 0; 
		vertices[0].z = 0.0f;
		vertices[1].u = ustart+width; 
		vertices[1].v = 0;
		vertices[1].color = color;
		vertices[1].x = step+width*swf; 
		vertices[1].y = 0; 
		vertices[1].z = 0.0f;
		vertices[2].u = ustart; 
		vertices[2].v = sv;
		vertices[2].color = color;
		vertices[2].x = ustart*swf; 
		vertices[2].y = sh; 
		vertices[2].z = 0.0f;
		vertices[3].u = ustart+width; 
		vertices[3].v = sv;
		vertices[3].color = color;
		vertices[3].x = (step + width*swf); 
		vertices[3].y = sh; 
		vertices[3].z = 0.0f;
		sceGumDrawArray(GU_TRIANGLE_STRIP,GU_TEXTURE_32BITF|GU_COLOR_8888|GU_VERTEX_32BITF|GU_TRANSFORM_3D,1*4,0,vertices);
	}
}

void CGuSystem::InitGu(void)
{
	pspDebugScreenInit();
	// Setup GU
 	m_drawbuf = getStaticVramBuffer(BUF_WIDTH,SCR_HEIGHT,GU_PSM_8888);
	m_displaybuf = getStaticVramBuffer(BUF_WIDTH,SCR_HEIGHT,GU_PSM_8888);
	m_zbuf = getStaticVramBuffer(BUF_WIDTH,SCR_HEIGHT,GU_PSM_4444);
 
	sceGuInit();
 
	sceGuStart(GU_DIRECT,list);
	sceGuDrawBuffer(GU_PSM_8888,m_drawbuf,BUF_WIDTH);
	sceGuDispBuffer(SCR_WIDTH,SCR_HEIGHT,m_displaybuf,BUF_WIDTH);
	sceGuDepthBuffer(m_zbuf,BUF_WIDTH);
	sceGuOffset(2048 - (SCR_WIDTH/2),2048 - (SCR_HEIGHT/2));
	sceGuViewport(2048,2048,SCR_WIDTH,SCR_HEIGHT);
	//sceGuDepthRange(65535,0);
	sceGuEnable(GU_SCISSOR_TEST);
	sceGuScissor(0,0,SCR_WIDTH,SCR_HEIGHT);
	sceGuFrontFace(GU_CW);
	//sceGuEnable(GU_CULL_FACE);
	//sceGuDepthMask(0xffff);
	sceGuDisable(GU_DEPTH_TEST);
	sceGuClear(GU_COLOR_BUFFER_BIT);
	sceGuEnable(GU_BLEND);
	sceGuBlendFunc(GU_ADD, GU_SRC_ALPHA, GU_ONE_MINUS_SRC_ALPHA, 0, 0);
	sceGuShadeModel(GU_SMOOTH);
	sceGuEnable(GU_TEXTURE_2D);
	sceGuFinish();
	sceGuSync(0,0);
 
	sceDisplayWaitVblankStart();
	sceGuDisplay(1);
	// finish
}