#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
Display			*Monitor;
Window			ImageWindow;
GC				ImageGC;
XWindowAttributes		Atts;
XImage			*Picture;

unsigned char		*displaydata;
FILE		      *fpt;
unsigned char	      *image;
char	 	     header[80];
int		        ROWS,COLS,BYTES;
int		         i,j,b;
unsigned char    d[2];
unsigned char temp,ab,msb0,msb1;




	

fpt=fopen(argv[1],"r");
if (fpt == NULL)
  {
  printf("Unable to open %s for reading\n",argv[1]);
  exit(0);
  }
	
i=fscanf(fpt,"%s %d %d %d",header,&COLS,&ROWS,&BYTES);

if (strcmp(header,"P5") == 0)
 {
  
  image=(unsigned char *)calloc(ROWS*COLS,sizeof(unsigned char));
  
  if (image == NULL)
  {
  printf("Unable to allocate %d x %d memory\n",COLS,ROWS);
  exit(0);
  }
	
  
  fread(image,1,ROWS*COLS,fpt);
  displaydata=(unsigned char*)calloc(ROWS*COLS,sizeof(unsigned char));
  
  
  for (i=0; i<ROWS*COLS; i++)
  {
   d[0]=248;
   d[1]=248;
   
   d[1] = image[i] & d[1];
   
   d[0] = image[i] & d[0];
   d[0] = d[0] >> 5;
   d[0] = d[1] | d[0];
   d[1] = d[0]; 
   d[1] = d[1] >> 3;
   
   ab=28;
   temp = image[i];
   
   temp = temp & ab;
   temp = temp << 3;
   d[1] = d[1] | temp;
   if (i!=0)
   {
   i=i+i;
   } 
   displaydata[i]=d[1];
   displaydata[i+1]=d[0];
   i=i/2;
 
   
   
  }
 
  

    


   fclose(fpt);
 }

else if (strcmp(header,"P6") == 0)
 {
  
  image=(unsigned char *)calloc(ROWS*COLS*3,sizeof(unsigned char));
  printf("image %s",image);
  if (image == NULL)
  {
  
  exit(0);
  }
	
  
  fread(image,1,ROWS*COLS*3,fpt);
  displaydata=(unsigned char*)calloc(ROWS*COLS*3,sizeof(unsigned char));
  for (i=0; i<ROWS*COLS*3; i=i+3)
    {
     
     d[0]=248;
     d[1]=248;
     msb0 = 224;
     d[0] = image[i+1] & d[0];
     msb0 = msb0 & image[i+2]; 
     msb0 = msb0 >> 5;
     d[0] = d[0] | msb0;
     d[1] = d[1] & image[i];
     d[1] = d[1] >> 2; 
     msb1 = 28;
     msb1 = msb1 & image[i+2];
     msb1 = msb1 << 3 ;
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



Monitor=XOpenDisplay(NULL);
if (Monitor == NULL)
  {
  printf("Unable to open graphics display\n");
  exit(0);
  }

ImageWindow=XCreateSimpleWindow(Monitor,RootWindow(Monitor,0),
	50,10,		/* x,y on screen */
	COLS,ROWS,	/* width, height */
	2, 		/* border width */
	BlackPixel(Monitor,0),
	WhitePixel(Monitor,0));

ImageGC=XCreateGC(Monitor,ImageWindow,0,NULL);

XMapWindow(Monitor,ImageWindow);
XFlush(Monitor);
while(1)
  {
  XGetWindowAttributes(Monitor,ImageWindow,&Atts);
  if (Atts.map_state == IsViewable /* 2 */)
    break;
  }

Picture=XCreateImage(Monitor,DefaultVisual(Monitor,0),
		DefaultDepth(Monitor,0),
		ZPixmap,	/* format */
		0,		/* offset */
		displaydata,/* the data */
		COLS,ROWS,	/* size of the image data */
		16,		/* pixel quantum (8, 16 or 32) */
		0);		/* bytes per line (0 causes compute) */

XPutImage(Monitor,ImageWindow,ImageGC,Picture,
		0,0,0,0,	/* src x,y and dest x,y offsets */
		COLS,ROWS);	/* size of the image data */

XFlush(Monitor);
sleep(2);
XCloseDisplay(Monitor);

}
