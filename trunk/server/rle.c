/* RLE Compression. */


/* Encodes at most 'max' bytes into dest, including the terminating zero,
 * and returns 1 if it did not fit. */
int rle_encode( unsigned char *src, unsigned char *dest, int max )
{
   unsigned char c, *s1, *s2, *d;
   unsigned int length, pos, i;
   if ( ( src == NULL ) || ( dest == NULL ) )
     {
        perror ( "NULL arguments - rle_encode" );
        return -1;
     }
   if ( max == 0 )
     {
        perror ( "invalid max - rle_encode" );
        return -1;
     }


   s1 = s2 = src;
   d = dest;
   length = 0;
   while  ( *s2 != '\0' )
     {
        while ( *s2 == *s1 )
          {
             s2++;
             if ( s2 - s1 == 255 )
               {
                  *d = s2-s1;
                  d++;
                  *d = *s1;
                  d++;
                  s1 = s2;
               }
          }
        *d = s2 - s1;
        d++;
        *d = *s1;
        d++;
        s1 = s2;
        if ( d - dest >= max )
          break;
     }
   *d = 0;
  return 0;
}



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




