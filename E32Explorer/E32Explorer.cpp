#include <iostream>
#include <chrono>
#include <thread>
#include "E32Image.h"
#include "TRomImage.h"
#include "Loader/E32ImageLoader.h"
#include "Loader/TRomImageLoader.h"
#include "Loader/TRomLoader.h"
#include "Gui/GuiE32Image.h"
#include "Gui/GuiTRomImage.h"
#include "Gui/GuiTRom.h"

std::string extract_filename(const std::string& filepath)
{
	auto pos = filepath.rfind("\\");
	if (pos == std::string::npos)
		pos = -1;
	return std::string(filepath.begin() + pos + 1, filepath.end());
}

int main(int argc, char *argv[])
{
	if (argc > 1) {
		E32Image e32;
		TRomImage tromImage;
		TRom trom;
		

		//First try with E32Image
		bool worked = E32ImageLoader::load(argv[1], e32);

		Gui* gui = nullptr;

		if (worked) {
			gui = new GuiE32Image(e32, extract_filename(std::string(argv[1])));
		}
		else {
			e32.data.clear();
			//next try: TRomImage
			worked = TRomImageLoader::load(argv[1], tromImage);

			if (worked) {
				gui = new GuiTRomImage(tromImage, extract_filename(std::string(argv[1])));		
			}

			else {
				tromImage.data.clear();
				//next try TRom
				worked = TRomLoader::parse(argv[1], trom);

				if (!worked) {
					//not a know format
					return -1;
				}
				
				gui = new GuiTRom(trom, extract_filename(std::string(argv[1])));
			}

		}

		bool running = true;

		const int FRAMES_PER_SECOND = 25;
		const uint64_t SKIP_TICKS = 1000 / FRAMES_PER_SECOND;
		auto clock = std::chrono::high_resolution_clock::now().time_since_epoch();
		auto next_game_tick = std::chrono::duration_cast<std::chrono::milliseconds>(clock).count();
		uint64_t sleep_time = 0;

		while (running) {
			running =gui->render();

			next_game_tick += SKIP_TICKS;

			clock = std::chrono::high_resolution_clock::now().time_since_epoch();
			auto tickCount = std::chrono::duration_cast<std::chrono::milliseconds>(clock).count();
			sleep_time = next_game_tick - tickCount;

			if (sleep_time > 0) {
				std::this_thread::sleep_for(std::chrono::milliseconds(sleep_time));
			}
		}

		delete gui;
	}
	else
	{
		printf("Usage: %s <E32ImageFile>\r\n\r\n", argv[0]);
                puts("Press any key to exit.\r\n");
		getchar();
	}

	return 0;

}
