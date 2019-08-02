#pragma once

#include <string>

class Module {
	public:
		std::string name;
		virtual void Load() = 0;
		virtual void Unload() = 0;
	private:

	protected:

};

