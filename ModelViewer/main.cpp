#include <iostream>
#include <memory>
#include "MessageHandler.h"
#include "WindowsWindow.h"

int main() {
	constexpr int width=800, height=600;
	bool quitRequest = 0;
	WindowsWindow window;

	std::shared_ptr<MessageHandler> msgHandler = std::make_shared<MessageHandler>(quitRequest);

	window.Init(msgHandler, width, height);
	window.Show();

	while (!quitRequest) {
		window.PumpMessage();
	}

	return 0;
}