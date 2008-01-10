/**
 * \file rle.c
 * \brief RLE Encoding utilities
 * 
 * This file handles compression and decompression of text, aimed at reducing
 * the size of maps transferred via network.
 */

/* RLE Compression. */


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
   int i, k, j;
   i = 0;
   k = 0;

   if ( ( src == NULL ) || ( dest == NULL ) )
     {
        perror ( "NULL arguments - rle_decode" );
        exit(0);
     }


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
     }
}




