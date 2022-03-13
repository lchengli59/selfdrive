/*-------------------------------------------------------------------------*/
/*-
https://github.com/JoanStinson/SteeringBehaviors
Please refer to above for License

Inspired by above steering application
Point click steering

*/
/*-------------------------------------------------------------------------*/

#include "gameapp.h"

using namespace std;

int main(int argc, char ** argv)
{

	gameApp *app = gameApp::Instance();
	// run app
	app->run();

	return 0;
}