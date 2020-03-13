/**
 * (C) 2010 Tapio Vierros
 * (C) 2020 Sergei Akhmatdinov
 *
 * Most of the code unchanged from RLUtil.
 */

#include "../rogueutil.h"
#include <algorithm>
#include <cstdlib>
#include <cstdio>
#include <ctime>

static inline void
waitkey()
{
        rogueutil::anykey("Press any key to continue...\n");
}

char
chargen()
{
	return rand() % (('~' - '!') + 1) + '!'; // I am really sorry for this
}

int
main()
{
	rogueutil::saveDefaultColor();

	std::cout << "Welcome to the Rogueutil test program." << std::endl;
	waitkey();

	std::cout << "\nTest 1: Colors" << std::endl;
	for (int i = 0; i < 16; i++) {
		rogueutil::setColor(i);
		std::cout << i << " ";
	}
	rogueutil::resetColor();
	std::cout << std::endl << "You should see numbers 0-15 in different colors." << std::endl;
	waitkey();

	std::cout << "\nTest 2: Background colors\n";
	for (int i = 0; i < 8; i++) {
		rogueutil::setBackgroundColor(i);
		std::cout << i;
		rogueutil::setBackgroundColor(0);
		std::cout << ' ';
	}
	rogueutil::resetColor();
	std::cout << "\nYou should see numbers 0-7 in different-colored backgrounds.\n";
	waitkey();

	rogueutil::cls();
	std::cout << "Test 3: Clear screen" << std::endl;
	std::cout << "The colors should now be gone." << std::endl;
	waitkey();

	std::cout << "\nTest 4: Cursor hiding" << std::endl;
	rogueutil::hidecursor();
	std::cout << "The cursor should now be invisible." << std::endl;
	waitkey();

	std::cout << "\nTest 5: Cursor showing" << std::endl;
	rogueutil::showcursor();
	std::cout << "The cursor should now be visible again." << std::endl;
	waitkey();

	rogueutil::cls();
	std::cout << "Test 6: Cursor positioning" << std::endl;
	rogueutil::locate(16,6); std::cout << "(16,6)";
	rogueutil::locate(4,3); std::cout << "(4,3)";
	rogueutil::locate(8,8); std::cout << "(8,8)";
	rogueutil::locate(-1,-10); std::cout << "(-1,-10)";
	std::cout << std::endl << "You should see three coordinates in their specified locations." << std::endl;
	waitkey();

	// Test 7
	{	int x = 7; int y = 7; unsigned int cnt = 0;
		while (true) {
			rogueutil::cls();
			std::cout << "Test 7: Blocking keyboard input" << std::endl;
			std::cout << "You should be able to move the '@' character with WASD keys." << std::endl;
			std::cout << "Hit Space to continue to the next test." << std::endl;
			std::cout << "Turn count: " << cnt << std::endl;
			rogueutil::locate(x,y); std::cout << '@'; // Output player
			char k = getch(); // Get character
			if (k == 'a') --x;
			else if (k == 'd') ++x;
			else if (k == 'w') --y;
			else if (k == 's') ++y;
			else if (k == ' ') break;
			cnt++;
		}
	}

	// Test 8
	{	int x = 7; int y = 7; unsigned int cnt = 0;
		rogueutil::hidecursor();
		rogueutil::cls();
		std::cout << "Test 8: Non-blocking keyboard input" << std::endl;
		std::cout << "You should be able to move the '@' character with WASD keys." << std::endl;
		std::cout << "Hit Space to continue to the next test." << std::endl;
		std::cout << "Turn count: " << cnt << std::endl;
		rogueutil::locate(x,y); std::cout << '@' << std::endl; // Output player
		while (true) {
			rogueutil::locate(1,4); std::cout << "Turn count: " << cnt;
			if (kbhit()) {
				char k = getch(); // Get character
				rogueutil::locate(x,y); std::cout << " "; // Erase player
				if (k == 'a') --x;
				else if (k == 'd') ++x;
				else if (k == 'w') --y;
				else if (k == 's') ++y;
				else if (k == ' ') break;
				rogueutil::locate(x,y); std::cout << '@'; // Output player
			}
			cnt++;
			std::cout.flush();
		}
		rogueutil::showcursor();
	}

	// Test 9
	{	int x = 7; int y = 7;
		rogueutil::cls();
		rogueutil::CursorHider curs;
		std::cout << "Test 9: Arrow keys" << std::endl;
		std::cout << "You should be able to move the '@' character with arrow keys." << std::endl;
		std::cout << "Hit Escape to continue to the next test." << std::endl;
		gotoxy(x,y); std::cout << '@' << std::endl; // Output player
		while (true) {
			if (kbhit()) {
				int k = rogueutil::getkey(); // Get character
				gotoxy(x,y); std::cout << " "; // Erase player
				if (k == rogueutil::KEY_LEFT) --x;
				else if (k == rogueutil::KEY_RIGHT) ++x;
				else if (k == rogueutil::KEY_UP) --y;
				else if (k == rogueutil::KEY_DOWN) ++y;
				else if (k == rogueutil::KEY_ESCAPE) break;
				gotoxy(x,y); std::cout << '@'; // Output player
				std::cout.flush();
			}
		}
	}

	rogueutil::cls();
	std::cout << "Test 10: Delay" << std::endl;
	std::cout << "Next numbers should appear rapidly after each other (but not instantly)." << std::endl;
	rogueutil::msleep(500);
	for (float t = 0; t <= 2; t += 0.25f) {
		std::cout << t << "s" << std::endl;
		rogueutil::msleep(250);
	}
	waitkey();

	rogueutil::cls();
	std::cout << "Test 11: Terminal Dimensions" << std::endl;
	std::cout << "You should see the size in character rows and columns of your terminal window." << std::endl;
	std::cout << rogueutil::trows() << " Rows" << std::endl;
	std::cout << rogueutil::tcols() << " Columns" << std::endl;
	waitkey();

	rogueutil::cls();
	std::cout << "Test 12: Non-advancing character setting" << std::endl;
	std::cout << "Next ASCII characters should appear rapidly after each other in the same position." << std::endl;
	{
		rogueutil::CursorHider hider;
		for(char c = '!'; c <= '~'; ++c) {
			rogueutil::setChar(c);
			std::cout.flush();
			rogueutil::msleep(75);
		}
	}
	waitkey();

	std::srand(std::time(0));
	rogueutil::cls();
	std::cout << "Test 13: Non-advancing string setting" << std::endl;
	std::cout << "Next random character strings should appear rapidly after each other starting in the same position." << std::endl;
	{
		const rogueutil::CursorHider hider;
		const unsigned int maxlen = rogueutil::tcols() / 2;
		std::string buf(maxlen, '\0');
		for(unsigned int l = 1; l < maxlen; ++l) {
			std::generate(buf.begin(), buf.begin() + l, chargen);
			rogueutil::setString(buf.c_str());
			std::cout.flush();
			rogueutil::msleep(75);
		}
	}
	std::cout << std::endl;
	waitkey();

	rogueutil::cls();
	std::cout << "Test 14: Terminal window title" << std::endl;
	{
		std::string title(14, '\0');
		std::generate(title.begin(), title.end(), chargen);
		rogueutil::setConsoleTitle(title);
		std::cout << "Your terminal window should now be named \"" << title << "\"." << std::endl;
	}
        waitkey();

        std::cout << "Test 15: Getting the username." << std::endl;
        std::cout << rogueutil::getUsername() << std::endl;
        std::cout << "You should see your username above." << std::endl;
        waitkey();

	std::cout << "All tests done. Bye!" << std::endl;
	return 0;
}
