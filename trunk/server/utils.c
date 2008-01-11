/**
 * \file rle.c
 * \brief RLE Encoding utilities
 * 
 * This file handles compression and decompression of text, aimed at reducing
 * the size of maps transferred via network.
 */

/* RLE Compression. */


#include <stdio.h>

/**
 * RLE Encoder
 * Encodes at most 'max' bytes into dest, including the terminating zero,
 * and returns 1 if it did not fit.
 * @param src source buffer
 * @param dest destination buffer
 * @param max destination buffer's size
*/
int rle_encode( char *src, char *dest, int max )
{
   unsigned char *s1, *s2, *d;
   
   s1 = s2 = (unsigned char *) src;
   d = (unsigned char *) dest;
   
   while ( *s2 && max > 1 )
     {
        while ( *(s2++) == *s1 )
          {
             if ( s2 - s1 == 255 )
               {
                  *(d++) = s2 - s1;
                  *(d++) = *s1;
                  s1 = s2;
               }
          }
        *(d++) = s2 - s1;
        *(d++) = *s1;
        s1 = s2;
	
	max--;
     }
   
   if ( max )
     *d = 0;
   
   return 0;
}


/**
 * RLE Decoder (incomplete)
 * Decode from src into dest.
 * @param src source buffer
 * @param dest destination buffer
 * @return number of bytes written
 */
void rle_decode( unsigned char *src, unsigned char *dest )
{
/*   int i, k, j;
   i = 0;
   k = 0;

   while ( i < strlen( src ) )
     {
        for ( j = 0; j < src[i]; j++ )
          {
             dest[k] = src[i+1];
             k++;
          }
        i+=2;
        if ( src[i] == '\0' )
          break;
     }*/
}



void Bresenham_old(short x1, short y1, short x2, short y2,
	       void (*delegate)( short y, short x, void *p ), void *p )
{

   short slope, dx, dy, incE, incNE, d, x, y;
   
   //y1 = -y1;
   //y2 = -y2;
   
   // Reverse lines where x1 > x2
   if (x1 > x2)
     {
	Bresenham_old(x2, y2, x1, y1, delegate, p);
	return;
     }
   dx = x2 - x1;
   dy = y2 - y1;
   // Adjust y-increment for negatively sloped lines
   if (dy < 0)
     {
	slope = -1;
	dy = -dy;
     }
   else
     {
	slope = 1;
     }
   // Bresenham constants
   incE = 2 * dy;
   incNE = 2 * dy - 2 * dx;
   d = 2 * dy - dx;
   y = y1;
   // Blit
   for (x = x1; x <= x2; x++)
     {
	delegate( y, x, p );
	if (d <= 0)
	  {
	     d += incE;
	  }
	else
	  {
	     d += incNE;
	     y += slope;
	  }
     }
}


void Bresenham(short x0, short y0, short x1, short y1,
	       void (*delegate)( short y, short x, void *p ), void *p )
{
#define SWAP(a, b)	(temp) = (a), (a) = (b), (b) = (temp)
#define ABS(a)		((a) < 0 ? -(a) : (a))
   
   int deltax, deltay, error, ystep, y, x;
   
   int steep = ABS( y1 - y0 ) > ABS( x1 - x0 );
   int temp, inverse = 0;
   
   if ( steep ) 
     {
	SWAP( x0, y0 );
	SWAP( x1, y1 );
     }
   
   printf( "Steep: %d, x0>x1: %d\n", steep, x0 > x1 );
   
   if ( x0 > x1 )
     {
//	SWAP( x0, x1 );
//	SWAP( y0, y1 );
	inverse = 1;
     }
   
   deltax = x1 - x0;
   deltay = ABS ( y1 - y0 );
   error = ( -deltax ) / 2;
   y = y0;
   if ( y0 < y1 )
     ystep = 1;
   else
     ystep = -1;
   
   for ( x = x0; x <= x1; x++ )
     {
	if ( steep )
	  {
	     if ( !inverse )
	       delegate( x, y, p );
	     else
	       delegate( -x, -y, p );
	  }
	else
	  {
	     if ( !inverse )
	       delegate( y, x, p );
	     else
	       delegate( -y, -x, p );
	  }
	
	error = error + deltay;
	
	if ( error >= 0 )
	  {
	     y = y + ystep;
	     error = error - deltax;
	  }
     }
}
