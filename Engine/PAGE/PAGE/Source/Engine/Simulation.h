#pragma once

#include "Platform/Win32/IApplication.h"

namespace PAGE {
	class PAGE_API Simulation : public Win32::IApplication {

	public:
		Simulation();
		~Simulation();

	};
}