#include <math.h>
#include <stdlib.h>

#include "clip_sz.hpp"


static void DetectCode( int x, int y, BarSpec& clipper, int* code )

 {
   memset( code, 0, sizeof(int)*4 );

   if( x<=clipper.x1 && y>=clipper.y1 ) { code[0]=1; code[3]=1; }
   else if( x>=clipper.x1 && x<=clipper.x2 && y>=clipper.y1 ) code[0]=1;
   else if( x>=clipper.x2 && y>=clipper.y1 ) { code[0]=1; code[2]=1; }
   else if( x>=clipper.x2 && y<=clipper.y1 && y>=clipper.y2 ) code[2]=1;
   else if( x>=clipper.x2 && y<=clipper.y2 ) { code[1]=1; code[2]=1; }
   else if( x>=clipper.x1 && x<=clipper.x2 && y<=clipper.y2 ) code[1]=1;
   else if( x<=clipper.x1 && y<=clipper.y2 ) { code[1]=1; code[3]=1; }
   else if( x<=clipper.x1 && y<=clipper.y1 && y>=clipper.y2 ) code[3]=1;
 }


int ClipSozKoe( Line& pline, BarSpec& clipper )

 {
   int fl=1;
   double tang;
   int flWid;
   Line p=pline;
   int t1[4], t2[4], clip[4];
   int tstZero[4]={ 0, 0, 0, 0 };
   int swp, s2;


   clip[0]=clipper.x1; clip[1]=clipper.x2;
   clip[2]=clipper.y2; clip[3]=clipper.y1;

   if( p.x1==p.x2 ) fl=-1;
   else
    {
      tang= double(p.y2-p.y1)/double(p.x2-p.x1);
      if( !tang ) fl=0;
    }

   for( int i=1, j=3, j2=0; i<=4; i++, j--, j2++ )
    {

      DetectCode( p.x1, p.y1, clipper, t1 );
      DetectCode( p.x2, p.y2, clipper, t2 );
      //AskWid( p, clipper, flWid );

      if( !memcmp(tstZero, t1, sizeof(int)*4) && !memcmp(tstZero, t2, sizeof(int)*4) ) flWid=1;
      else
       {
         for( int i=0, summ=0; i<4; i++ )
           summ+=t1[i]&t2[i];
         if( summ ) flWid=-1;
         else flWid=0;
       }

      switch( flWid )
       {
         case -1: 
		   return -1;

         case 1:
//MY_OUT:
          //(*line)( int(p.x1), int(p.y1), int(p.x2), int(p.y2), color );
          return 1;

         default:
          break;
       }

      if( t1[j]==t2[j] ) continue;

      if( !t1[j] )
       {
         swp=p.x1; p.x1=p.x2; p.x2=swp;
         swp=p.y1; p.y1=p.y2; p.y2=swp;
       }

      if( fl==-1 )
        p.y1=clip[j2];
      else
       {
         if( i<=2 )
          {
            p.y1= tang*double(clip[j2]-p.x1)+p.y1;
            p.x1=clip[j2];
          }
         else if( fl )
          {
            p.x1= (1.0/tang)*double(clip[j2]-p.y1)+double(p.x1);
            p.y1=clip[j2];
          }
       }
    }

   //goto MY_OUT;
   return 1;

 }
