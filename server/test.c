#include <stdio.h>

void Bresenham( short x1, short y1, short x2, short y2,
		void (*delegate)( short y, short x, void *p ), void *p );

void test_func( short y, short x, void *p )
{
   printf( "Point %d: %d/%d.\n", *(int*)p, x, y );
   (*(int*)p)++;
}


int main( )
{
   int i = 0;
   
   // Octants:
   
   puts( "Octant 1" );
   Bresenham( 0, 0, 10, 5, test_func, &i );
   
   puts( "Octant 2" );
   Bresenham( 0, 0, 5, 10, test_func, &i );
   
   puts( "Octant 3" );
   Bresenham( 0, 0, -5, 10, test_func, &i );
   
   puts( "Octant 4" );
   Bresenham( 0, 0, -10, 5, test_func, &i );
   
   puts( "Octant 5" );
   Bresenham( 0, 0, -10, -5, test_func, &i );
   
   puts( "Octant 6" );
   Bresenham( 0, 0, -5, -10, test_func, &i );
   
   puts( "Octant 7" );
   Bresenham( 0, 0, 5, -10, test_func, &i );
   
   puts( "Octant 8" );
   Bresenham( 0, 0, 10, -5, test_func, &i );
   
   return 0;
}
