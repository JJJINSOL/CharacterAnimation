#pragma once
#include "Std.h"
#include "fmod.hpp"
#include "fmod_errors.h"
#pragma comment(lib,"fmod_vc.lib")
class Sound
{
public:
	int				m_iIndex;
	std::wstring	m_csName;
	FMOD::System*	m_pSystem = nullptr;
	FMOD::Sound*	m_pSound = nullptr;
	FMOD::Channel*	m_pChannel = nullptr;
	float			m_fVolume = 0.5f;
	std::wstring	m_szMsg;
	TCHAR			m_szBuffer[256];
public:
	void	Set(FMOD::System* pSystem, std::wstring name, int iIndex);
	bool	Init();
	bool	Frame();
	bool	Render();
	bool	Release();
public:
	void    Play(bool bLoop = false);
	void    PlayEffect();
	void    Stop();
	void    Paused();
	void    VolumeUp(float fVolume = 0.1f);
	void    VolumeDown(float fVolume = 0.1f);
public:
	Sound();
	virtual ~Sound();
};
//=============================================================
class SoundMgr : public Singleton<SoundMgr>
{
	int		m_iIndex;
public:
	friend class Singleton<SoundMgr>;
	FMOD::System* m_pSystem = nullptr;
	std::map<std::wstring, std::shared_ptr<Sound> > m_list;
public:
	Sound*  Load(std::string filename);
	Sound*  GetPtr(std::wstring key);
	bool	Init();
	bool	Frame();
	bool	Render();
	bool	Release();
private:
	SoundMgr();
public:
	~SoundMgr();
};

#define I_Sound SoundMgr::Get()