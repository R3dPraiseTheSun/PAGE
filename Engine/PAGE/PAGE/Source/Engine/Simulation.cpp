#include "page.h"
#include "Simulation.h"
#include "Engine/SplashScreen.h"


PAGE::Simulation::Simulation()
{
}

PAGE::Simulation::~Simulation()
{
}

VOID PAGE::Simulation::PreInitialize()
{

	Logger::PrintDebugSeperator();
	Logger::PrintLog(L"Application Starting...\n");
	Logger::PrintLog(L"Game Name: %s\n", PerGameSettings::GameName());
	Logger::PrintLog(L"Short Name: %s\n", PerGameSettings::ShortName());
	Logger::PrintLog(L"Boot Time: %s\n", Time::GetDateTimeString().c_str());
	Logger::PrintLog(L"Engine Mode: %s\n", Engine::EngineModeToString().c_str());

	Logger::PrintDebugSeperator();

    SplashScreen::Open();
}
