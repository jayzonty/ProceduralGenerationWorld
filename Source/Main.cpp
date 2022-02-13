#include "Application.hpp"
#include "MainScene.hpp"

/// <summary>
/// Main function.
/// </summary>
/// <returns>An integer indicating whether the program ended successfully or not.
/// A value of 0 indicates the program ended succesfully, while a non-zero value indicates
/// something wrong happened during execution.</returns>
int main()
{
	{
		MainScene *mainScene = new MainScene();

		Application application;
		application.Run(mainScene);

		delete mainScene;
	}

	return 0;
}
