#include "Application.hpp"

/**
 * @brief Main function.
 * @return An integer indicating whether the program ended successfully or not.
 * A value of 0 indicates the program ended succesfully, while a non-zero value indicates
 * something wrong happened during execution.
 */
int main()
{
	{
		Application application;
		application.Run();
	}

	return 0;
}
