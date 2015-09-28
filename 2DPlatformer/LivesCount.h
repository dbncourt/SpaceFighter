////////////////////////////////////////////////////////////////////////////////
// Filename: LivesCount.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _LIVES_COUNT_H_
#define _LIVES_COUNT_H_

///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "GameObject.h"

class LivesCount : public GameObject
{
public:
	LivesCount();
	LivesCount(const LivesCount& other);
	~LivesCount();

	virtual bool Initialize(ID3D11Device* device, HWND hwnd, Bitmap::DimensionType screen, bool drawCollider) override;
};
#endif