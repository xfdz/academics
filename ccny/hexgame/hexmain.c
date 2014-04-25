#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>
#include <X11/Xatom.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define BOARDSIZE 11
char gameboard[BOARDSIZE][BOARDSIZE];
double radius;
/* gameboard conventions here: '0' empty, 'A' user, 'B' program */

Display *display_ptr;
Screen *screen_ptr;
int screen_num;
char *display_name = NULL;
unsigned int display_width, display_height;

Window win;
int border_width;
unsigned int win_width, win_height;
int win_x, win_y;

XWMHints *wm_hints;
XClassHint *class_hints;
XSizeHints *size_hints;
XTextProperty win_name, icon_name;
char *win_name_string = "Example Window";
char *icon_name_string = "Icon for Example Window";

XEvent report;

GC gc, gc_yellow, gc_red, gc_grey;
unsigned long valuemask = 0;
XGCValues gc_values, gc_yellow_values, gc_red_values, gc_grey_values;
Colormap color_map;
XColor tmp_color1, tmp_color2;


void initgameboard();
void drawgameboard();
void makeanswermove();

int main(int argc, char **argv)
{
  initgameboard();
  /* opening display: basic connection to X Server */
  if( (display_ptr = XOpenDisplay(display_name)) == NULL )
    { printf("Could not open display. \n"); exit(-1);}
  printf("Connected to X server  %s\n", XDisplayName(display_name) );
  screen_num = DefaultScreen( display_ptr );
  screen_ptr = DefaultScreenOfDisplay( display_ptr );
  color_map  = XDefaultColormap( display_ptr, screen_num );
  display_width  = DisplayWidth( display_ptr, screen_num );
  display_height = DisplayHeight( display_ptr, screen_num );

  printf("Width %d, Height %d, Screen Number %d\n", 
           display_width, display_height, screen_num);
  /* creating the game window */
  border_width = 10;
  win_x = 0; win_y = 0;
  win_width = display_width/2;
  win_height = (int) (win_width / 1.7);
  
  win= XCreateSimpleWindow( display_ptr, RootWindow( display_ptr, screen_num),
                            win_x, win_y, win_width, win_height, border_width,
                            BlackPixel(display_ptr, screen_num),
                            WhitePixel(display_ptr, screen_num) );
  /* now try to put it on screen, this needs cooperation of window manager */
  size_hints = XAllocSizeHints();
  wm_hints = XAllocWMHints();
  class_hints = XAllocClassHint();
  if( size_hints == NULL || wm_hints == NULL || class_hints == NULL )
    { printf("Error allocating memory for hints. \n"); exit(-1);}

  size_hints -> flags = PPosition | PSize | PMinSize  ;
  size_hints -> min_width = 60;
  size_hints -> min_height = 60;

  XStringListToTextProperty( &win_name_string,1,&win_name);
  XStringListToTextProperty( &icon_name_string,1,&icon_name);
  
  wm_hints -> flags = StateHint | InputHint ;
  wm_hints -> initial_state = NormalState;
  wm_hints -> input = False;

  class_hints -> res_name = "hex_example";
  class_hints -> res_class = "games";

  XSetWMProperties( display_ptr, win, &win_name, &icon_name, argv, argc,
                    size_hints, wm_hints, class_hints );

  /* what events do we want to receive */
  XSelectInput( display_ptr, win, 
            ExposureMask | StructureNotifyMask | ButtonPressMask );
  
  /* finally: put window on screen */
  XMapWindow( display_ptr, win );

  XFlush(display_ptr);

  /* create graphics context, so that we may draw in this window */
  gc = XCreateGC( display_ptr, win, valuemask, &gc_values);
  XSetForeground( display_ptr, gc, BlackPixel( display_ptr, screen_num ) );
  XSetLineAttributes( display_ptr, gc, 4, LineSolid, CapRound, JoinRound);
  /* and three other graphics contexts, to draw in yellow and red and grey*/
  gc_yellow = XCreateGC( display_ptr, win, valuemask, &gc_yellow_values);
  XSetLineAttributes(display_ptr, gc_yellow, 6, LineSolid,CapRound, JoinRound);
  if( XAllocNamedColor( display_ptr, color_map, "yellow", 
			&tmp_color1, &tmp_color2 ) == 0 )
    {printf("failed to get color yellow\n"); exit(-1);} 
  else
    XSetForeground( display_ptr, gc_yellow, tmp_color1.pixel );
  gc_red = XCreateGC( display_ptr, win, valuemask, &gc_red_values);
  XSetLineAttributes( display_ptr, gc_red, 6, LineSolid, CapRound, JoinRound);
  if( XAllocNamedColor( display_ptr, color_map, "red", 
			&tmp_color1, &tmp_color2 ) == 0 )
    {printf("failed to get color red\n"); exit(-1);} 
  else
    XSetForeground( display_ptr, gc_red, tmp_color1.pixel );
  gc_grey = XCreateGC( display_ptr, win, valuemask, &gc_grey_values);
  if( XAllocNamedColor( display_ptr, color_map, "light grey", 
			&tmp_color1, &tmp_color2 ) == 0 )
    {printf("failed to get color grey\n"); exit(-1);} 
  else
    XSetForeground( display_ptr, gc_grey, tmp_color1.pixel );

  /* and now it starts: the event loop */
  while(1)
    { XNextEvent( display_ptr, &report );
      switch( report.type )
	{
	case Expose:
          /* draw the game board */
          drawgameboard();
          break;
        case ConfigureNotify:
          win_width = report.xconfigure.width;
          win_height = report.xconfigure.height;
          radius = (double) win_height / (4.0*BOARDSIZE + 2.0);
          if( radius > ((double) win_width / (8*sqrt(0.75)*(BOARDSIZE-1) +6)))
	    radius = ((double) win_width / (8*sqrt(0.75)*(BOARDSIZE-1) +6));
          break;
        case ButtonPress:
          {  int i, j;
             int x, y;
             for( i = 0; i < BOARDSIZE; i++)
               for( j = 0; j < BOARDSIZE; j++)
		 { /* find the field where button was pressed */
  	            x = 3*radius + 4*radius*(i+j)*sqrt(0.75); 
                    y = (2*BOARDSIZE +1 -2*(i-j) )*radius;
                    if( ( (report.xbutton.x - x)*(report.xbutton.x - x) 
                         + (report.xbutton.y - y)*(report.xbutton.y - y) 
			  < radius*radius )
                        && (gameboard[i][j] == '0'))
		      { gameboard[i][j] = 'A';
		        drawgameboard();
                        makeanswermove();
                      }
                  }
          }
          break;
        default: 
          break;
	}

    }
  exit(0);
}

void initgameboard()
{  int i, j;
   for( i = 0; i < BOARDSIZE; i++)
     for( j = 0; j < BOARDSIZE; j++)
       gameboard[i][j] = '0';
   /* put two example moves in, take out when answer strategy exists */
   gameboard [2][2] = 'A';
   gameboard [2][3] = 'B';
}



void drawgameboard()
{  int i, j;
   int x, y;
   /* draw the grid */
   for( i = 0; i < BOARDSIZE - 1; i++)
     for( j = 0; j < BOARDSIZE - 1; j++)
       {
	 x = 3*radius + 4*radius*(i+j)*sqrt(0.75); 
         y = (2*BOARDSIZE +1 -2*(i-j) )*radius;
	 XDrawLine(display_ptr, win, gc, x, y,
                   (int) (x + 4*radius*sqrt(0.75)) , (int) (y-2*radius) );
	 XDrawLine(display_ptr, win, gc, x, y,
                   (int) (x + 4*radius*sqrt(0.75)) , (int) (y+2*radius) );
 	 XDrawLine(display_ptr, win, gc, 
		   (int) (x + 4*radius*sqrt(0.75)) , (int) (y-2*radius),
                   (int) (x + 4*radius*sqrt(0.75)) , (int) (y+2*radius) );
 	 XDrawLine(display_ptr, win, gc, 
		   (int) (x + 4*radius*sqrt(0.75)) , (int) (y-2*radius),
                   (int) (x + 8*radius*sqrt(0.75)) , y );
 	 XDrawLine(display_ptr, win, gc, 
		   (int) (x + 4*radius*sqrt(0.75)) , (int) (y+2*radius),
                   (int) (x + 8*radius*sqrt(0.75)) , y );
       }
   /* draw the positions: A yellow, B red, empty gray */
   for( i = 0; i < BOARDSIZE; i++)
     for( j = 0; j < BOARDSIZE; j++)
       {
	 x = 3*radius + 4*radius*(i+j)*sqrt(0.75); 
         y = (2*BOARDSIZE +1 -2*(i-j) )*radius;
         switch( gameboard[i][j] )
	   { case '0':
	       XFillArc( display_ptr, win, gc_grey, (int) x-radius, (int) y-radius,
                       (int) 2*radius, (int) 2*radius, 0, 360*64);
               break;
             case 'A':
	       XFillArc( display_ptr, win, gc_yellow, 
                       (int) x-radius, (int) y-radius,
                       (int) 2*radius, (int) 2*radius, 0, 360*64);
               break;
             case 'B':
	       XFillArc( display_ptr, win, gc_red, 
                       (int) x-radius, (int) y-radius,
                       (int) 2*radius, (int) 2*radius, 0, 360*64);
               break;
	     default:
               printf("bad entry in gameboard: %c at %d, %d.\n",
		      gameboard[i][j], i, j );
	       break;
           }
	   XDrawArc( display_ptr, win, gc, (int) x-radius, (int) y-radius,
                       (int) 2*radius, (int) 2*radius, 0, 360*64);

       }
    /* draw the red pair of sides and the yellow pair of sides */
    XDrawLine(display_ptr, win, gc_red, 
	      (int) 3*radius - 4*radius*sqrt(0.75), 
              (int)(2*BOARDSIZE + 3)*radius,
              (int) 3*radius + 4*radius*(-2+BOARDSIZE)*sqrt(0.75),
              (int) (4*BOARDSIZE +1 )*radius);
    XDrawLine(display_ptr, win, gc_red, 
	      (int) 3*radius + 4*radius*(BOARDSIZE)*sqrt(0.75),  
              (int) radius,
              (int) 3*radius + 4*radius*(2*BOARDSIZE -1)*sqrt(0.75), 
              (int) (2*BOARDSIZE -1 )*radius);
    XDrawLine(display_ptr, win, gc_yellow, 
	      (int) 3*radius - 4*radius*sqrt(0.75),  
              (int) (2*BOARDSIZE -1 )*radius,
              (int) 3*radius + 4*radius*(BOARDSIZE-2)*sqrt(0.75), 
              (int) (2*BOARDSIZE +1 -2*(BOARDSIZE) )*radius);
    XDrawLine(display_ptr, win, gc_yellow, 
	      (int) 3*radius + 4*radius*(BOARDSIZE)*sqrt(0.75), 
              (int) (4*BOARDSIZE +1 )*radius,
              (int) 3*radius + 4*radius*(2*BOARDSIZE -1)*sqrt(0.75), 
              (int) (2*BOARDSIZE +3 )*radius);

}

void makeanswermove()
{
}
