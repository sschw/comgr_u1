/*// Converts only a single color channel
double srgbToLinear(const Uint8& c) {
	return pow(c, 2.2);
}

// Converts only a single color channel
Uint8 linearToSrgb(const double& c) {
	return (Uint8)pow(c, 1 / 2.2);
}

Uint32 getGradientColor(Uint32 start, Uint32 end, float step) {
	float stepI = 1 - step;
	Uint32 color = linearToSrgb(srgbToLinear((start >> 16) & 0xFF) * stepI + srgbToLinear((end >> 16) & 0xFF) * step);
	color = color << 8;
	color = color + linearToSrgb(srgbToLinear((start >> 8) & 0xFF) * stepI + srgbToLinear((end >> 8) & 0xFF) * step);
	color = color << 8;
	color = color + linearToSrgb(srgbToLinear(start & 0xFF) * stepI + srgbToLinear(end & 0xFF) * step);
	return color;
}

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
		Uint32 currentColor = getGradientColor(colorGradientFrom, colorGradientTo, step);
		for (Uint32 j = 0; j < surf->w; j++) {
			pixels[heightOffset + j] = currentColor;
		}
	}

	if (SDL_MUSTLOCK(surf)) {
		SDL_UnlockSurface(surf);
	}
}

int main(int argc, char* args[])
{
	SDL_Surface* b = NULL;
	Uint32 width = 640;
	Uint32 height = 480;


	SDL_Init(SDL_INIT_EVERYTHING);

	b = SDL_SetVideoMode(width, height, 32, SDL_SWSURFACE);
	SDL_WM_SetCaption("COMGR - 1 - Color test", NULL);

	paintGradient(b, 0);

	SDL_Flip(b);

	Uint32 offset = 0;
	bool quit = false;
	SDL_Event evt;

	while (quit == false)
	{
		while (SDL_PollEvent(&evt))
		{
			if (evt.type == SDL_QUIT)
			{
				quit = true;
			}
		}
		if (SDL_GetTicks() % 10 == 0) {
			offset++;
			paintGradient(b, offset);
			SDL_Flip(b);
		}
	}

	SDL_Quit();

	return 0;
}*/