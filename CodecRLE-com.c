#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[])
{
Display			*Monitor;
Window			ImageWindow;
GC				ImageGC;
XWindowAttributes		Atts;
XImage			*Picture;

unsigned char		*displaydata;
FILE		      *fpt;
FILE                  *cpt;
unsigned char	      *image;
char	 	     header[80];
int		        ROWS,COLS,BYTES;
int		        i,t,j,rleon=0;
unsigned char    A,B,k;
unsigned char    runcout=1;
unsigned char  ab;
unsigned char  d[2];
int            RLEON=1,size;
char           flag;
//char            ;




	
cpt=fopen(argv[2],"w");
//if (argv[1]=="txt")
//{
 //printf("akshay cool\n");
// cpt=fopen(argv[3],"w");
 fpt=fopen(argv[1],"rb");
 if (fpt == NULL)
  {
  printf("Unable to open %s for reading\n",argv[1]);
  exit(0);
  }
//akshay start here today
  fseek(fpt,0,SEEK_END);
  size = ftell(fpt);
  fseek(fpt,0,SEEK_SET); 
  image=(unsigned char *)calloc(size,sizeof(unsigned char));

  
  if (image == NULL)
  {
  printf("Unable to allocate %d x %d memory\n",COLS,ROWS);
  exit(0);
  }
	
   
     fread(image,1,size,fpt);
  
    
     for (i=0; i<size; i=i+2)
     {
      d[0]=image[i];
      d[1]=image[i+1];
      
       if ((int)d[0] == 0)   //change 
        {
          if (RLEON==1) //STATE OF RLE
           {
            
            RLEON=0;   //TURN RLE OFF
           }
           else
           {
            
            RLEON=1;   // TURN RLE ON
           }        
        }
      
       if (RLEON == 1)
        {
         flag='N';
        
         for (j=0;j<(int)d[0];j++)
          {
            fwrite(&d[1],1,1,cpt);
            
            
          } 
        }
       if (RLEON==0)
         {
          i=i+1;
          
          
         
         
           do{
             
            fwrite(&image[i],1,1,cpt);
            
              i++;
            } while ((int)image[i] != 0);
            
            i=i-1;
            RLEON=1;
          
              
           
             
           

         }
          

        
     }
 
}
