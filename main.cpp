#include <memory>

#include "System/System.hpp"


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow)
{
	std::unique_ptr<System> system;
	bool result;


	// Create the system object.
	system = std::make_unique<System>();

	// Initialize and run the system object.
	result = system->Initialize();
	if (result)
	{
		system->Run();
	}

	// Shutdown and release the system object.
	system->Shutdown();
	system.reset();
	system = nullptr;

	return 0;
}