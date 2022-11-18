// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#include "Texture.h"
//#include <SOIL/SOIL.h>
#include <GL/glew.h>
#include <SDL2/SDL.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
Texture::Texture()
:mTextureID(0)
,mWidth(0)
,mHeight(0)
{
	
}

Texture::~Texture()
{
	
}

bool Texture::Load(const std::string& fileName)
{
	int channels = 0;
	/*
	unsigned char* image = SOIL_load_image(fileName.c_str(),
										   &mWidth, &mHeight, &channels, SOIL_LOAD_AUTO);
	*/
	unsigned char* image = stbi_load(fileName.c_str(), &mWidth, &mHeight, &channels, 0);
	if (image == nullptr)
	{
		//SDL_Log("SOIL failed to load image %s: %s", fileName.c_str(), SOIL_last_result());
		return false;
	}
	
	int format = GL_RGB;
	if (channels == 4)
	{
		format = GL_RGBA;
	}
	
	glGenTextures(1, &mTextureID);
	glBindTexture(GL_TEXTURE_2D, mTextureID);
	
	glTexImage2D(GL_TEXTURE_2D, 0, format, mWidth, mHeight, 0, format,
				 GL_UNSIGNED_BYTE, image);
	
	//SOIL_free_image_data(image);
	stbi_image_free(image);
	
	// Enable linear filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	return true;
}

void Texture::Unload()
{
	glDeleteTextures(1, &mTextureID);
}

void Texture::CreateFromSurface(SDL_Surface* surface)
{
	mWidth = surface->w;
	mHeight = surface->h;
        SDL_Surface * s = SDL_CreateRGBSurface(0, mWidth, mHeight, 32, 0x00ff0000,0x0000ff00,0x000000ff,0xff000000);
        SDL_BlitSurface(surface, NULL, s, NULL);
	
	// Generate a GL texture
	glGenTextures(1, &mTextureID);
	glBindTexture(GL_TEXTURE_2D, mTextureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, mWidth, mHeight, 0, GL_BGRA,
				 GL_UNSIGNED_BYTE, /*surface->pixels */ s->pixels);
	
	// Use linear filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void Texture::SetActive()
{
	glBindTexture(GL_TEXTURE_2D, mTextureID);
}
