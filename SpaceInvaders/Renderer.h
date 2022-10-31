#pragma once
#include <SDL.h>
#include <vector>
#include "Vector2.h"
#include "Actor.h"

// Compilation + rapide, si pas d'appel de constructeur
class Window;
struct Rectangle;

class Renderer
{
public:
	enum class Flip
	{
		None = SDL_FLIP_NONE,
		Horizontal = SDL_FLIP_HORIZONTAL,
		Vertical = SDL_FLIP_VERTICAL
	};

	Renderer() = default;
	Renderer(const Renderer&) = delete;
	Renderer& operator = (const Renderer&) = delete;

	bool initialize(Window& window);

	void beginDraw();
	void draw();
	void endDraw();

	void drawRect(const Rectangle& rect);
	void addSprite(class SpriteComponent* sprite);
	void removeSprite(class SpriteComponent* sprite);
	void drawSprites();
	void drawSprite(const Actor& actor, const class Texture& tex, Rectangle srcTect, Vector2 origin, Flip flip) const;

	inline SDL_Renderer* toSDLRenderer() const { return SDLRenderer; }
	void close();

private:
	SDL_Renderer* SDLRenderer{ nullptr };
	std::vector<SpriteComponent*> sprites;
};