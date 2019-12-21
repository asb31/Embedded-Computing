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
int		         i,j,b,t,rleon=0,IM=0;
unsigned char    A,B;
int    size;
unsigned char runcout=1;
unsigned char  ab,C=1,D=0;
unsigned char a=0;




	
cpt=fopen(argv[2],"w");



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
	/* read image data from file */
  
   
     fread(image,1,size,fpt); 
  
 
   t=0;
   A=image[0];
   
      
     for (i=0; i<size; i++)
     {
          
      B=image[i+1];    
    
      if (image[i]==image[i+1] || runcout >> 1)
       {
          t=0; 
          if (rleon >> 0)
           {
            
             
              fwrite(&a,1,1,cpt);
            rleon=0;
           }     
          if (A == B)
           {
            if (runcout !=255)
             {
              runcout++;
             }
            else       
             {
              
                fwrite(&runcout,1,1,cpt);
                fwrite(&A,1,1,cpt);
                if (A==0)
                 {
                  printf("Akshay is testing here ");
                 
                   //fwrite(&C,1,1,cpt);
                   //fwrite(&D,1,1,cpt);
                  
                   //fwrite(&D,1,1,cpt);
                 }
             
              A=B;
              runcout=1;
             } 
           }
          if (A!=B)
           {
            
            
                fwrite(&runcout,1,1,cpt);
                fwrite(&A,1,1,cpt);
                 
            A=B;
            runcout=1;
            } 
        }    
          //CODE END FOR RLE ON
   
      else   // CODE START RLE OFF
      
       {
        rleon++;
        if (t==0)
         {
          
                unsigned char pt = 0;
                fwrite(&pt,1,1,cpt);
                fwrite(&A,1,1,cpt);
                if (A==0)
                 {
                 
                   fwrite(&C,1,1,cpt);
                   fwrite(&D,1,1,cpt);
                   fwrite(&D,1,1,cpt);
                 }
          
          A=B;
         }
        if (t!=0)
         {
          
           
                 if (image[i]==0)
                 {
                   fwrite(&D,1,1,cpt); 
                 
                   fwrite(&C,1,1,cpt);
                   fwrite(&D,1,1,cpt);
                   fwrite(&D,1,1,cpt);
                  }
                  else
                   {
                    fwrite(&image[i],1,1,cpt);
                   }
                   
               
          A=B;
         }
         t++; 
       }                      //CODE END FOR RLE OFF
           
      
  }
 }





// Code writtem below is for decompressor//
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
