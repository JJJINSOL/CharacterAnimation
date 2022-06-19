#include "World.h"
#include "ObjectMgr.h"
World* World::m_pWorld = nullptr;

bool World::Load(std::wstring saveFile)
{
	return true;
}
bool World::Init()
{
	return true;
}
bool World::Frame()
{
	for (auto obj : m_UIObj)
	{
		Object2D* pObj = obj.get();
		if (pObj != nullptr)
		{
			pObj->Frame();
		}
	}
	for (auto obj : m_NpcObj)
	{
		Object2D* pObj = obj.get();
		if (pObj != nullptr)
		{
			pObj->Frame();
		}
	}
	return true;
}
bool World::Render()
{
	for (auto obj : m_UIObj)
	{
		Object2D* pObj = obj.get();
		if (pObj != nullptr)
		{
			pObj->Render();
		}
	}
	for (auto obj : m_NpcObj)
	{
		Object2D* pObj = obj.get();
		if (pObj->m_bDead == false)
		{
			pObj->Render();
		}
	}
	return true;
}
bool World::Release()
{
	for (auto obj : m_UIObj)
	{
		if (obj != nullptr)
		{
			obj->Release();
			obj = nullptr;
		}

	}
	for (auto obj : m_ItemObj)
	{
		if (obj != nullptr)
		{
			obj->Release();
			obj = nullptr;
		}
	}
	for (auto obj : m_NpcObj)
	{
		if (obj != nullptr)
		{
			obj->Release();
			obj = nullptr;
		}
	}
	for (auto obj : m_MapObj)
	{
		if (obj != nullptr)
		{
			obj->Release();
			obj = nullptr;
		}
	}
	m_ItemObj.clear();
	m_UIObj.clear();
	m_NpcObj.clear();
	m_MapObj.clear();
	return true;
}
World::World()
{

}

World::~World()
{
	Release();
}