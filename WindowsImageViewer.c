#include <windows.h>
#include <StdAfx.h>
#include<stdio.h>

LRESULT CALLBACK EventProcessor (HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
{ return(DefWindowProc(hWnd,uMsg,wParam,lParam)); }

int WINAPI WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,
                   LPSTR     lpCmdLine,int nCmdShow)
{
WNDCLASS		wc;
HWND			WindowHandle;
int			ROWS,COLS,BYTES,i;
unsigned char	*displaydata;
BITMAPINFO		*bm_info;
HDC		 	     hDC;
FILE            *fpt;
unsigned char   *image;
char             header[80];
unsigned char    d[2];
unsigned char   msb0,msb1;
char            IMAGENAME[25];


wc.style=CS_HREDRAW | CS_VREDRAW;
wc.lpfnWndProc=(WNDPROC)EventProcessor;
wc.cbClsExtra=wc.cbWndExtra=0;
wc.hInstance=hInstance;
wc.hIcon=NULL;
wc.lpszMenuName=NULL;
wc.hCursor=LoadCursor(NULL,IDC_ARROW);
wc.hbrBackground=(HBRUSH)(COLOR_WINDOW+1);
wc.lpszClassName=L"Image Window Class";
if (RegisterClass(&wc) == 0)
  exit(0);
/*akshaystarts here*/
 if (MessageBox(NULL,L"DO YOU WANT BRIDGE.PPM?",L"BRIDGE",MB_YESNO) == IDYES)
{
	       strcpy(IMAGENAME,"bridge.ppm");
}
else
{
	      strcpy(IMAGENAME,"retina.ppm");
}

  
fpt=fopen(IMAGENAME,"rb");
  if (fpt == NULL)
    {
	 printf("unable to open image\n");
	 exit(0); 
    }
 i=fscanf(fpt,"%s %d %d %d",header,&COLS,&ROWS,&BYTES);  
if (strcmp(header, "P5") == 0)
    {
       
        image = (unsigned char *)calloc(ROWS*COLS, sizeof(unsigned char));
        
        if (image == NULL)
        {
            printf("Unable to allocate %d x %d memory\n", COLS, ROWS);
            exit(0);
        }
        
       
        fread(image, 1, ROWS*COLS, fpt);
        displaydata = (unsigned char*)calloc(ROWS*COLS, sizeof(unsigned char));


        for (i = 0; i<ROWS*COLS; i++)
        {
          
            displaydata[i] = image[i];
                  
        }
     
        fclose(fpt);
		
}
   
   else	if (strcmp(header, "P6") == 0)
    {
       
        image = (unsigned char *)calloc(ROWS*COLS*3, sizeof(unsigned char));
        
        if (image == NULL)
        {
            printf("Unable to allocate %d x %d memory\n", COLS, ROWS);
            exit(0);
        }
        
       
        fread(image, 1, ROWS*COLS*3, fpt);
        displaydata = (unsigned char*)calloc(ROWS*COLS*3, sizeof(unsigned char));


        for (i = 0; i<ROWS*COLS*3; i=i+3)
        {
			d[0]=248;
            d[1]=248;
            msb0=192;
           d[0] = image[i+1] & d[0];
           msb0 = msb0 & image[i+2];
           msb0 = msb0 >> 5;
           d[0] = d[0] | msb0;
		   d[0] = d[0] >> 1;
           d[1] = d[1] & image[i];
           d[1] = d[1] >> 3;
           msb1 = 56;
           msb1 = msb1 & image[i+2];
           msb1 = msb1 << 2 ;
           d[1] = d[1] | msb1; 
			

   
     
  
     if (i!=0)
      {
       i=(2*i)/3;
    
       }
     displaydata[i]=d[1];
     displaydata[i+1]=d[0];
       
       i=(3*i)/2;
        
            
            



        }

        




        fclose(fpt);

    

    }


/*akshay code ends here*/

WindowHandle=CreateWindow(L"Image Window Class",L"ECE468 Lab1",
						  WS_OVERLAPPEDWINDOW,
						  10,10,COLS,ROWS,
						  NULL,NULL,hInstance,NULL);
if (WindowHandle == NULL)
  {
  MessageBox(NULL,L"No window",L"Try again",MB_OK | MB_APPLMODAL);
  exit(0);
  }
ShowWindow (WindowHandle, SW_SHOWNORMAL);

bm_info=(BITMAPINFO *)calloc(1,sizeof(BITMAPINFO) + 256*sizeof(RGBQUAD));
hDC=GetDC(WindowHandle);
bm_info->bmiHeader.biSize=sizeof(BITMAPINFOHEADER);
	bm_info->bmiHeader.biWidth=COLS;
	bm_info->bmiHeader.biHeight=-ROWS;
	bm_info->bmiHeader.biPlanes=1;
	bm_info->bmiHeader.biCompression=BI_RGB;
	bm_info->bmiHeader.biClrImportant=0;
	bm_info->bmiHeader.biSizeImage=0;
	bm_info->bmiHeader.biClrUsed=0;
	if (strcmp(header, "P6") == 0)
	{
	 bm_info->bmiHeader.biBitCount=16;
	}
	else
	{
     bm_info->bmiHeader.biBitCount=8;
	}
/* ... set up bmiHeader field of bm_info ... */

for (i=0; i<256; i++)	/* colormap */
  {
  bm_info->bmiColors[i].rgbBlue=bm_info->bmiColors[i].rgbGreen=bm_info->bmiColors[i].rgbRed=i;
  bm_info->bmiColors[i].rgbReserved=0;
  } 
SetDIBitsToDevice(hDC,0,0,COLS,ROWS,0,0,
			  0, /* first scan line */
			  ROWS, /* number of scan lines */
			  displaydata,bm_info,DIB_RGB_COLORS);
ReleaseDC(WindowHandle,hDC);
free(bm_info);
MessageBox(NULL,L"Press OK to continue",L"",MB_OK | MB_APPLMODAL);
}
