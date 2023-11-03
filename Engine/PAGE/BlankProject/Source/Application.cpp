#include "pch.h"
#include "Application.h"
#include "Platform/WIN32/WinEntry.h"

ENTRYAPP(Application)

Application::Application()
{
}

Application::~Application()
{
}

VOID Application::SetupPerGameSettings()
{
	/* Set the Per Game Settings */
	PerGameSettings::setGameName(IDS_PERGAMENAME);
	PerGameSettings::setShortName(IDS_SHORTNAME);
	PerGameSettings::setMainIcon(IDI_MAINICON);
}

VOID Application::Initialize()
{
	Logger::PrintDebugSeperator();
	Logger::PrintLog(L"Application Starting...\n");
	Logger::PrintLog(L"Game Name: %s\n", PerGameSettings::GameName());
	Logger::PrintLog(L"Short Name: %s\n", PerGameSettings::ShortName());
	Logger::PrintLog(L"Boot Time: %s\n", Time::GetDateTimeString().c_str());
	Logger::PrintDebugSeperator();

	Logger::StartMTail();
}

VOID Application::Update()
{

}