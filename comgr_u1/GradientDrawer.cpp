#include "GradientDrawer.h"

void paintGradient(SDL_Surface* surf, Uint32 pixelOffset) {
	Uint32 gradientHeight = 100;
	float stepSize = 1.0f / gradientHeight;
	Uint32 colorGradients[] = { 0xFF0000, 0x00FF00, 0x0000FF };
	Uint32 numberOfGradients = sizeof(colorGradients) / sizeof(Uint32);

	Uint32* pixels = (Uint32 *)surf->pixels;

	if (SDL_MUSTLOCK(surf)) {
		SDL_LockSurface(surf);
	}

	for (Uint32 i = pixelOffset, heightOffset = 0; i < surf->h + pixelOffset; i++, heightOffset += surf->w) {
		Uint32 colorGradientFrom = colorGradients[(i / gradientHeight) % numberOfGradients];
		Uint32 colorGradientTo = colorGradients[((i / gradientHeight) + 1) % numberOfGradients];
		float step = (float)fmod(i*stepSize, 1);
		Uint32 currentColor = Color(colorGradientFrom, colorGradientTo, step);
		for (Uint32 j = 0; j < surf->w; j++) {
			pixels[heightOffset + j] = currentColor;
		}
	}

	if (SDL_MUSTLOCK(surf)) {
		SDL_UnlockSurface(surf);
	}
}
