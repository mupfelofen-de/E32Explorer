#include <iostream>
#include "E32Image.h"
#include "Loader.h"
#include "Gui\GuiE32Image.h"
#include "Gui\GuiTRomImage.h"

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
		//First try with E32Image
		E32Image file;
		bool worked = loader::load(argv[1], file);

		Gui* gui = nullptr;

		if (worked) {
			gui = new GuiE32Image(file, extract_filename(std::string(argv[1])));
		}
		else {
			//next try: TRomImage
			TRomImage file;
			bool worked = loader::load(argv[1], file);

			if (!worked) {
				//not a know format
				return -1;
			}

			gui = new GuiTRomImage(file, extract_filename(std::string(argv[1])));
		}

		bool running = true;;

		const int FRAMES_PER_SECOND = 25;
		const int SKIP_TICKS = 1000 / FRAMES_PER_SECOND;
		ULONGLONG next_game_tick = GetTickCount64();
		int sleep_time = 0;

		while (running) {
			running =gui->render();

			next_game_tick += SKIP_TICKS;
			sleep_time = next_game_tick - GetTickCount();
			if (sleep_time >= 0) {
				Sleep(sleep_time);
			}
			else {
				// Shit, we are running behind!
			}
		}

		delete gui;
	}

	return 0;

}