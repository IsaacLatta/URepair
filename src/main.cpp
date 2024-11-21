#include "App.h"
#include <sqlite3.h>
#include <iostream>

int main() {
	App app;
	if(!app.init()) {
		ERROR("App", "init failed, exiting ...");
		return -1;
	}
	app.run();
	return 0;
}
