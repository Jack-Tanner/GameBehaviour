#include "Application.h"

#include "Debug.h"

int main( int argc, char* argv[] )
{
	Application app;

	if ( app.Initialise() )
		app.Run();

	app.Shutdown();

	return 0;
}