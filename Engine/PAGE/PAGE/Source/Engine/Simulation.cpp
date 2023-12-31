#include "page.h"
#include "Simulation.h"
#include "Engine/SplashScreen.h"

namespace PAGE {

	Simulation::Simulation()
		:Win32::Window(L"MainApplication", NULL)
	{
	}

	Simulation::~Simulation()
	{
	}

	VOID Simulation::PreInitialize()
	{

		Logger::PrintDebugSeperator();
		Logger::PrintLog(L"Application Starting...\n");
		Logger::PrintLog(L"Game Name: %s\n", PerGameSettings::GameName());
		Logger::PrintLog(L"Short Name: %s\n", PerGameSettings::ShortName());
		Logger::PrintLog(L"Boot Time: %s\n", Time::GetDateTimeString().c_str());
		Logger::PrintLog(L"Engine Mode: %s\n", Engine::EngineModeToString().c_str());

		Logger::PrintDebugSeperator();

		SplashScreen::Open();
	
		std::this_thread::sleep_for(std::chrono::seconds(3));

		Win32::Window::RegisterNewClass();
		Win32::Window::Initialize();

		SplashScreen::Close();

	}

	LRESULT Simulation::MessageHandler(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
	{

		switch (message)
		{

		}

		return Window::MessageHandler(hwnd, message, wParam, lParam);
	}
}