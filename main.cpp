#include <iostream>



//The headers
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "SDL/SDL_ttf.h"
#include <string>
#include "Enemigo.h"

//Screen attributes
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 572;
const int SCREEN_BPP = 32;

//The surfaces
SDL_Surface *background = NULL;
SDL_Surface *up = NULL;
SDL_Surface *down = NULL;
SDL_Surface *left = NULL;
SDL_Surface *right = NULL;
SDL_Surface *screen = NULL;

//The event structure
SDL_Event event;

//The font
TTF_Font *font = NULL;

//The color of the font
SDL_Color textColor = { 0, 0, 0 };

SDL_Surface *load_image( std::string filename )
{
    return IMG_Load( filename.c_str());
}

void apply_surface( int x, int y, SDL_Surface* source, SDL_Surface* destination, SDL_Rect* clip = NULL )
{
    //Holds offsets
    SDL_Rect offset;

    //Get offsets
    offset.x = x;
    offset.y = y;

    //Blit
    SDL_BlitSurface( source, clip, destination, &offset );
}

bool init()
{
    //Initialize all SDL subsystems
    if( SDL_Init( SDL_INIT_EVERYTHING ) == -1 )
    {
        return false;
    }

    //Set up the screen
    screen = SDL_SetVideoMode( SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE );

    //If there was an error in setting up the screen
    if( screen == NULL )
    {
        return false;
    }

    //Initialize SDL_ttf
    if( TTF_Init() == -1 )
    {
        return false;
    }

    //Set the window caption
    SDL_WM_SetCaption( "Press an Arrow Key", NULL );

    //If everything initialized fine
    return true;
}

bool load_files()
{
    //Load the background image
    background = load_image("background.png");

    //Open the font
    font = TTF_OpenFont( "lazy.ttf", 72 );

    //If there was a problem in loading the background
    if( background == NULL )
    {
        return false;
    }

    //If there was an error in loading the font
    if( font == NULL )
    {
        return false;
    }

    //If everything loaded fine
    return true;
}

void clean_up()
{
    //Free the surfaces
    SDL_FreeSurface( background );
    SDL_FreeSurface( up );
    SDL_FreeSurface( down );
    SDL_FreeSurface( left );
    SDL_FreeSurface( right );

    //Close the font
    TTF_CloseFont( font );

    //Quit SDL_ttf
    TTF_Quit();

    //Quit SDL
    SDL_Quit();
}

class Punto
{
public:
    int x;
    int y;
    Punto(int x, int y)
    {
        this->x=x;
        this->y=y;
    }
    Punto()
    {
    }
};

class Rectangulo
{
public:
    Punto p;
    int w;
    int h;
    Rectangulo(Punto p,int w,int h)
    {
        this->p=p;
        this->w=w;
        this->h=h;
    }
};

bool puntoDentroDeRect(Punto p, Rectangulo r)
{
    if(p.x>=r.p.x && p.x<=r.p.x+r.w
            && p.y>=r.p.y && p.y<=r.p.y+r.h)
        return true;
    return false;
}

bool colision(int x1, int y1, int w1, int h1,
              int x2, int y2, int w2, int h2)
{
    if(puntoDentroDeRect(Punto(x2,y2),Rectangulo(Punto(x1,y1),w1,h1)))
        return true;
    if(puntoDentroDeRect(Punto(x2+w2,y2),Rectangulo(Punto(x1,y1),w1,h1)))
        return true;
    if(puntoDentroDeRect(Punto(x2,y2+h2),Rectangulo(Punto(x1,y1),w1,h1)))
        return true;
    if(puntoDentroDeRect(Punto(x2+w2,y2+h2),Rectangulo(Punto(x1,y1),w1,h1)))
        return true;
    return false;
}

int main( int argc, char* args[] )
{
    //Quit flag
    bool quit = false;

    //Initialize
    if( init() == false )
    {
        return 2;
    }

    //Load the files
    if( load_files() == false )
    {
        return 3;
    }

    //Render the text
    up = TTF_RenderText_Solid( font, "Up", textColor );
    down = TTF_RenderText_Solid( font, "Down", textColor );
    left = TTF_RenderText_Solid( font, "Left", textColor );
    right = TTF_RenderText_Solid( font, "Right", textColor );

    //While the user hasn't quit
    SDL_Surface* jump ;
    SDL_Surface* personaje[4];
    personaje[0] = load_image("Endless Run/run01.png");
    personaje[1] = load_image("Endless Run/run02.png");
    personaje[2] = load_image("Endless Run/run03.png");
    personaje[3] = load_image("Endless Run/run04.png");
    jump = load_image("Endless Run/jump.png");

    SDL_Surface *enemigo[3];
    enemigo[0] = load_image("Endless Run/volador01.png");
    enemigo[1] = load_image("Endless Run/volador02.png");
    enemigo[2] = load_image("Endless Run/volador03.png");
    int cont =0;
    int frame=0;
    int mover =0;
    int pos_personajex=0;
    int pos_personajey=250;
    int velocidad_y=0;
    int acc_y=2;
    int m=0;
    int posmx=500;
    int posmy=250;
    SDL_Surface * monta = load_image( "mountains.png" );

    Enemigo bomba(800,250,load_image("Endless Run/bomba01.png"));

    while( quit == false )
    {
        //While there's events to handle
        while( SDL_PollEvent( &event ) )
        {
            //If the user has Xed out the window
            if( event.type == SDL_QUIT )
            {
                //Quit the program
                quit = true;
            }
        }


        //Apply the background
        apply_surface( 0, 0, background, screen );
        apply_surface( mover, 0,monta, screen );
        apply_surface(posmx, posmy, enemigo[m],screen);
        mover-=5;
        posmx-=8;
        if(cont%15==0)
        {
            m++;
            if(m>2)
            {
                m=0;
            }
        }


        if(mover<-1726)
        {
            mover=0;
        }

        //Get the keystates
        Uint8 *keystates = SDL_GetKeyState( NULL );
        pos_personajey += velocidad_y;
        velocidad_y +=acc_y;
        if(pos_personajey>=250)
            pos_personajey =250;


        if( keystates[ SDLK_SPACE ] )
        {
            if(pos_personajey==250)
                velocidad_y=-30;
        }


        if(pos_personajey==250)
            apply_surface(pos_personajex,pos_personajey,personaje[frame],screen);
        else
            apply_surface(pos_personajex,pos_personajey,jump,screen);

        if(cont%15==0)
        {
            frame++;
            if(frame>3)
                frame=0;
        }
        cont++;

        bomba.avanzar();
        bomba.imprimir(screen);

        int px=pos_personajex;
        int py=pos_personajey;
        int pw=personaje[frame]->w;
        int ph=personaje[frame]->h;

        int bx=bomba.x;
        int by=bomba.y;
        int bw=bomba.surface->w;
        int bh=bomba.surface->h;

        if(colision(px+pw/4,py+ph/4,pw/2,ph/2,
                    bx+bw/4,by+bh/4,bw/2,bh/2))
            exit(0);

        //Update the screen
        if( SDL_Flip( screen ) == -1 )
        {
            return 1;
        }
    }

    //Clean up
    clean_up();

    return 0;
}
