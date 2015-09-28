#include "Number.h"

Number::Number() : GameObject()
{
}

Number::Number(const Number& other)
{
}

Number::~Number()
{
}

bool Number::Initialize(ID3D11Device* device, HWND hwnd, Bitmap::DimensionType screen, bool drawCollider)
{
	bool result;

	result = GameObject::Initialize(device, hwnd, screen, L"Numbers.dds", Bitmap::DimensionType{ 170, 17 }, Bitmap::DimensionType{ 17, 17 }, 10, 0, false);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the Number GameObject.", L"Error", MB_OK);
		return false;
	}

	return true;
}