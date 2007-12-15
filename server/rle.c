/* RLE Compression. */


/* Encodes at most 'max' bytes into dest, including the terminating zero,
 * and returns 1 if it did not fit. */
int rle_encode( unsigned char *src, unsigned char *dest, int max )
{
   unsigned char c;
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

   c = src[0];
   length = 1;
   i = 0;
   pos = 0;

   while ( c != '\0' )
     {
        i++;
        if ( c == src[i] )
          {
             length++;
             if ( length == 255 )
               {
                  dest[pos] = length;
                  pos++;
                  dest[pos] = c;
                  pos++;
                  c = src[i];
                  length = 1;
               }
          }
        else
          {
             dest[pos] = length;
             pos++;
             dest[pos] = c;
                          pos++;
             c = src[i];
             length = 1;
          }
        if (  pos >= max )
          break;
     }
   dest[pos++] = '\0';
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




