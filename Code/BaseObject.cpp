#include "BaseObject.h"



BaseObject::BaseObject()
{
	p_object = NULL;
	rect_ = { 0,0,0,0 };
}


BaseObject::~BaseObject()
{
	free();
}

bool BaseObject::loadImg(std::string path, SDL_Renderer * screen)
{
	free();
	SDL_Surface* load_surface = IMG_Load(path.c_str());
	if (load_surface != NULL) {
		SDL_SetColorKey(load_surface, SDL_TRUE, SDL_MapRGB(load_surface->format, COLOR_KEY_R, COLOR_KEY_G, COLOR_KEY_B));
		p_object = SDL_CreateTextureFromSurface(screen, load_surface);
		if (p_object != NULL) {
			rect_.w = load_surface->w;
			rect_.h = load_surface->h;
		}
		SDL_FreeSurface(load_surface);
	}
	return p_object != NULL;
}

void BaseObject::render(SDL_Renderer * des, const SDL_Rect * clip)
{
	SDL_Rect renderquad = { rect_.x, rect_.y, rect_.w, rect_.h };
	SDL_RenderCopy(des, p_object, clip, &renderquad);
}

void BaseObject::free()
{
	if(p_object != NULL) {
		SDL_DestroyTexture(p_object);
		p_object = NULL;
		rect_ = { 0,0,0,0 };
	}
}
