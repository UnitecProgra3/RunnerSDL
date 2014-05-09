#include "Enemigo.h"

Enemigo::Enemigo(int x,int y,SDL_Surface*surface)
{
    this->x=x;
    this->y=y;
    this->surface=surface;
}

Enemigo::~Enemigo()
{
    //dtor
}

void Enemigo::avanzar()
{
    x-=8;
    if(x<-surface->w)
        x=700;
}

void Enemigo::imprimir(SDL_Surface*screen)
{
    //Holds offsets
    SDL_Rect offset;

    //Get offsets
    offset.x = this->x;
    offset.y = this->y;

    //Blit
    SDL_BlitSurface( this->surface, NULL, screen, &offset );
}
