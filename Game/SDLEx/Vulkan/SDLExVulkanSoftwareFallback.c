#include "SDLExVulkan.h"
SoftwareFallbackState software_fallback_state;

#define Renderer software_fallback_state.SoftwareRenderer

SDL_BlendMode CurrentBlendMode;

int sdlex_is_software_fallback_enabled() {
	return software_fallback_state.IsEnabled;
}

void sdlex_enable_fallback_software(SDL_Window * targetWindow) {
	SDL_Log("Error Initializing Vulkan, Enabling Software Renderer Fallback...\n");
	software_fallback_state.IsEnabled = 1;
	software_fallback_state.TargetWindow = targetWindow;
	software_fallback_state.TargetSurface = SDL_GetWindowSurface(targetWindow);
	software_fallback_state.SoftwareRenderer = SDL_CreateRenderer(software_fallback_state.TargetWindow, -1, 0);
}

void sdlex_software_clear() {
	SDL_SetRenderDrawColor(Renderer, 0u, 0u, 0u, 0xffu);
	SDL_RenderClear(Renderer);
}

void sdlex_software_set_blend_mode(enum BlendMode mode) {
	switch (mode)
	{
	case SDLEX_BLEND_MODE_ALPHABLEND:
		CurrentBlendMode = SDL_BLENDMODE_BLEND;
		break;
	case SDLEX_BLEND_MODE_ADDITIVE:
		CurrentBlendMode = SDL_BLENDMODE_ADD;
		break;
	}
}

void sdlex_software_flush() {
	return;
}

void sdlex_software_endframe() {
	SDL_RenderPresent(software_fallback_state.SoftwareRenderer);
	// SDL_UpdateWindowSurface(software_fallback_state.TargetWindow);
}

int sdlex_software_render_texture(SDL_Rect target) {
	SDL_SetTextureColorMod(software_fallback_state.ActiveTexture, 0xffu, 0xffu, 0xffu);
	SDL_SetTextureAlphaMod(software_fallback_state.ActiveTexture, 0xffu);
	SDL_SetTextureBlendMode(software_fallback_state.ActiveTexture, CurrentBlendMode);
	return SDL_RenderCopy(
		Renderer,
		software_fallback_state.ActiveTexture,
		NULL,
		&target
	);
}

int sdlex_software_render_texture_region_ex(Vector2 position, Vector2 origin, float rotation, Vector2 scale, Vector4 color, SDL_Rect * sourceRegion) {
	SDL_Rect dst;
	SDL_Rect frame;
	SDL_Point origin_p;
	origin_p.x = (int)origin.X;
	origin_p.y = (int)origin.Y;
	frame = sourceRegion != NULL ? *sourceRegion : texture_frame(software_fallback_state.ActiveTexture);
	Vector2 leftBottom = vector2_sub(position, origin);
	Vector2 rightUpper = vector2_adds(leftBottom, (float)frame.w, (float)frame.h);
	leftBottom = vector2_sub(leftBottom, position);
	rightUpper = vector2_sub(rightUpper, position);
	leftBottom.X *= scale.X;
	leftBottom.Y *= scale.Y;
	rightUpper.X *= scale.X;
	rightUpper.Y *= scale.Y;
	leftBottom = vector2_add(leftBottom, position);
	rightUpper = vector2_add(rightUpper, position);
	dst.x = leftBottom.X;
	dst.y = leftBottom.Y;
	dst.w = rightUpper.X - leftBottom.X;
	dst.h = rightUpper.Y - leftBottom.Y;
	SDL_RenderSetClipRect(Renderer, &software_fallback_state.TargetSurface->clip_rect);
	SDL_SetTextureColorMod(software_fallback_state.ActiveTexture, (Uint8)(color.X * 255), (Uint8)(color.Y * 255), (Uint8)(color.Z * 255));
	SDL_SetTextureAlphaMod(software_fallback_state.ActiveTexture, (Uint8)(color.W * 255));
	SDL_SetTextureBlendMode(software_fallback_state.ActiveTexture, CurrentBlendMode);
	return SDL_RenderCopyEx(
		Renderer,
		software_fallback_state.ActiveTexture,
		sourceRegion,
		&dst,
		rotation,
		&origin_p,
		SDL_FLIP_NONE
	);
}
