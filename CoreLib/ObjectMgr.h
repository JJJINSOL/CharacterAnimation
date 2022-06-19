#pragma once
#include "Object2D.h"
#include "TFbxImporter.h"
//function을 사용하면 virtual 기능이 작동
//                                      <반환값(인자들)>
using CollisionFunction = std::function<void(BaseObject*, DWORD)>;//충돌처리
using SelectFunction = std::function<void(BaseObject*, DWORD)>;//마우스가 오브젝트 선택하는거
//오브젝트 충돌처리
class ObjectMgr : public BaseMgr<TFbxImporter, ObjectMgr>//객체 생성은 부모에서
{
	friend class Singleton<TextureMgr>;
private:
	int	m_iExcueteCollisionID;
	int	m_iExcueteSelectID;
	std::map<int, BaseObject*> m_ObjectList;
	std::map<int, BaseObject*> m_SelectList;
public://부모가 자식을 만들어서 쓸수 잇도록 friend로 해줌
	friend Singleton< ObjectMgr>;

	typedef std::map<int, CollisionFunction>::iterator FuncionIterator;
	std::map<int, CollisionFunction> m_fnCollisionExecute;//함수가등록/고유한 오브젝트의 인덱스에 해당하는 함수
	typedef std::map<int, SelectFunction>::iterator FuncionIterator;
	std::map<int, SelectFunction> m_fnSelectExecute;

	//                        자기 자신          반환될 함수
	void  AddCollisionExecute(BaseObject* owner, CollisionFunction func);//이거 할때마다 m_iExcueteCollisionID가 +1
	void  DeleteCollisionExecute(BaseObject* owner);

	void  AddSelectExecute(BaseObject* owner, CollisionFunction func);
	void  DeleteSelectExecute(BaseObject* owner);

	bool  Init();
	bool  Frame();
	bool  Release();
	void  CallRecursive(BaseObject* pSrcObj, DWORD dwState);

private://외부에서는 절대로 객체 생성 불가능
	ObjectMgr();
public:
	virtual ~ObjectMgr();

};
#define I_ObjectMgr ObjectMgr::Get()//그래서 객체 생성하고 싶으면 이거 써야댐