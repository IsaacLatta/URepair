#include "app.h"

int main() {
	App app;
	if(!app.init()) {
		ERROR("App", "init failed, exiting ...");
		return -1;
	}
	app.run();
}
