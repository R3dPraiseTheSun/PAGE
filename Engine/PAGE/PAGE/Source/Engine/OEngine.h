#pragma once

class PAGE_API OEngine;
namespace Engine {

	enum EngineMode : INT {
		NONE,
		DEBUG,
		RELEASE,
		EDITOR,
		SERVER
	};

	extern OEngine g_PAGEEngine;

	VOID PAGE_API SetMode(EngineMode mode);
	EngineMode PAGE_API GetMode();

	std::wstring PAGE_API EngineModeToString();

}

using namespace Engine;
class PAGE_API OEngine {
	
public:
	OEngine();
	~OEngine();

private:
	EngineMode m_EngineMode;

public:
	EngineMode GetEngineMode();
	VOID SetEngineMode(EngineMode mode);


};