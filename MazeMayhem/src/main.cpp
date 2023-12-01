#include "../include/MazeMayhem.h"

int APIENTRY WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR pCmdLine, int nShowCmd)
{
	// Create the application
	auto app = std::make_unique<MazeMayhem>();
	if (!app->Create(1280, 720, "MAZE MAYHEM"))
	{
		IApplication::Debug("GAME START FAILED!");
		return 0;
	}

	// Run the app and exit when done
	app->Run();
	return 0;
}
