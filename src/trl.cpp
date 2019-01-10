#include <iostream>
#include "video.hpp"

int main() {
	video.init(200,200);
	while(1) {
		video.update();
	}
	return 0;
}
