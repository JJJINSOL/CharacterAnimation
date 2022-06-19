#pragma once
#include "Device.h"
#include "DxObject.h"
#include "Input.h"
#include "Timer.h"
#include "WriteFont.h"
#include "Camera.h"
class Core : public Device
{
public:
	Camera*		m_pMainCamera = nullptr;
	Camera		m_DefaultCamera;

	Timer		m_GameTimer;
	WriteFont   m_dxWrite;
	bool		m_bWireFrame = false;

private:
	bool CoreInit();
	bool CoreFrame();
	bool CoreRender();
	bool CoreRelease();
public:
	bool GameRun();

	virtual void CreateResizeDevice(UINT iWidth, UINT iHeight) {};
	virtual void DeleteResizeDevice(UINT iWidth, UINT iHeight) {};
	void		 ResizeDevice(UINT iWidth, UINT iHeight);

	virtual bool Init() { return true; };
	virtual bool Frame() { return true; };
	virtual bool Render() { return true; };
	virtual bool Release() { return true; };

	Core() {};
	virtual ~Core() {};
};