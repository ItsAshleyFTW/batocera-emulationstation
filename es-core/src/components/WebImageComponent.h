#pragma once

#include "ImageComponent.h"

class HttpReq;

class WebImageComponent : public ImageComponent
{
public:		
	WebImageComponent(Window* window, double keepInCacheDuration = -1);
	virtual ~WebImageComponent();

	void setImage(std::string path, bool tile = false, MaxSizeInfo maxSize = MaxSizeInfo()) override;	

	void update(int deltaTime) override;
	void render(const Transform4x4f& parentTrans) override;

private:
	HttpReq*	mRequest = nullptr;
	
	std::string mUrlToLoad;
	std::string mLocalFile;

	MaxSizeInfo mMaxSize;

	double		mKeepInCacheDuration;
};