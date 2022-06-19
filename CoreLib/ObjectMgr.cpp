#include "ObjectMgr.h"
#include "Input.h"
void ObjectMgr::AddCollisionExecute(BaseObject* owner, CollisionFunction func)
{
	//collision id 등록
	owner->m_iCollisionID = m_iExcueteCollisionID++;
	//오브젝트 리스트 등록->키가 id
	m_ObjectList.insert(std::make_pair(owner->m_iCollisionID, owner));
	//그 키로 함수 등록
	m_fnCollisionExecute.insert(std::make_pair(owner->m_iCollisionID, func));
}
void ObjectMgr::DeleteCollisionExecute(BaseObject* owner)
{
	std::map<int, BaseObject*>::iterator objiter;
	objiter = m_ObjectList.find(owner->m_iCollisionID);
	if (objiter != m_ObjectList.end())
	{
		m_ObjectList.erase(objiter);
	}

	FuncionIterator colliter = m_fnCollisionExecute.find(owner->m_iCollisionID);
	if (colliter != m_fnCollisionExecute.end())
	{
		m_fnCollisionExecute.erase(colliter);
	}
}
void ObjectMgr::AddSelectExecute(BaseObject* owner, CollisionFunction func)
{
	owner->m_iSelectID = m_iExcueteSelectID++;
	m_SelectList.insert(std::make_pair(owner->m_iSelectID, owner));
	m_fnSelectExecute.insert(std::make_pair(owner->m_iSelectID, func));
}
void ObjectMgr::DeleteSelectExecute(BaseObject* owner)
{
	std::map<int, BaseObject*>::iterator objiter;
	objiter = m_SelectList.find(owner->m_iCollisionID);
	if (objiter != m_SelectList.end())
	{
		m_SelectList.erase(objiter);
	}

	FuncionIterator colliter = m_fnSelectExecute.find(owner->m_iCollisionID);
	if (colliter != m_fnSelectExecute.end())
	{
		m_fnSelectExecute.erase(colliter);
	}
}
bool  ObjectMgr::Init()
{
	return true;
}
void  ObjectMgr::CallRecursive(BaseObject* pSrcObj, DWORD dwState)
{
	if (pSrcObj->m_pParent == nullptr)
	{
		return;
	}
	CallRecursive(pSrcObj->m_pParent, dwState);
	pSrcObj->HitSelect(pSrcObj, dwState);
}
bool  ObjectMgr::Frame()
{
	// collision
	//N:N 충돌처리
	for (auto src : m_ObjectList)
	{
		BaseObject* pObjSrc = (BaseObject*)src.second;
		if (pObjSrc->m_dwCollisonType == CollisionType::Ignore) continue;
		DWORD dwState = CollisionType::Overlap;
		for (auto dest : m_ObjectList)
		{
			BaseObject* pObjDest = (BaseObject*)dest.second;
			if (pObjSrc == pObjDest) continue;
			if (Collision::ToRect(pObjSrc->m_rtCollision, pObjDest->m_rtCollision))
			{
				FuncionIterator colliter = m_fnCollisionExecute.find(pObjSrc->m_iCollisionID);
				if (colliter != m_fnCollisionExecute.end())
				{
					CollisionFunction call = colliter->second;
					call(pObjDest, dwState);
				}
			}
		}
	}
	// mouse select
	for (auto src : m_SelectList)
	{
		BaseObject* pObjSrc = (BaseObject*)src.second;
		//if (pObjSrc->m_dwSelectType == SelectType::Select_Ignore) continue;
		DWORD dwState = SelectState::T_DEFAULT;
		if (pObjSrc->m_dwSelectType != SelectType::Select_Ignore &&
			Collision::RectToPoint(pObjSrc->m_rtCollision, (float)g_ptMouse.x, (float)g_ptMouse.y)) 
		{
			DWORD dwKeyState = Input::Get().m_dwMouseState[0];
			pObjSrc->m_dwSelectState = SelectState::T_HOVER;
			if (dwKeyState == KEY_PUSH)
			{
				pObjSrc->m_dwSelectState = SelectState::T_ACTIVE;
			}
			if (dwKeyState == KEY_HOLD)
			{
				pObjSrc->m_dwSelectState = SelectState::T_FOCUS;
			}
			if (dwKeyState == KEY_UP)
			{
				pObjSrc->m_dwSelectState = SelectState::T_SELECTED;
			}

			CallRecursive(pObjSrc, dwState);
			FuncionIterator colliter = m_fnSelectExecute.find(pObjSrc->m_iSelectID);
			if (colliter != m_fnSelectExecute.end())
			{
				CollisionFunction call = colliter->second;
				call(pObjSrc, dwState);
			}
		}
		else
		{
			if (pObjSrc->m_dwSelectState != SelectState::T_DEFAULT)
			{
				pObjSrc->m_dwSelectState = SelectState::T_DEFAULT;
				FuncionIterator colliter = m_fnSelectExecute.find(pObjSrc->m_iSelectID);
				if (colliter != m_fnSelectExecute.end())
				{
					CollisionFunction call = colliter->second;
					call(pObjSrc, dwState);
				}
			}
		}

	}
	return true;
}
bool ObjectMgr::Release()
{
	m_ObjectList.clear();
	m_SelectList.clear();
	return true;
}
ObjectMgr::ObjectMgr()
{
	m_iExcueteCollisionID = 0;
	m_iExcueteSelectID = 0;
};
ObjectMgr::~ObjectMgr()
{
	Release();
};