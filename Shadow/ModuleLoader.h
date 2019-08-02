#pragma once

#include <vector>
#include <memory>
#include "Module.h"

class ModuleLoader {
	public:
		void RegisterModule();

	private:
		std::vector<std::unique_ptr<Module>> mAvailableModules;
		std::vector<std::string> mLoadedModules;
};

