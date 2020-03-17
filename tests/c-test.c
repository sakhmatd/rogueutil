/**
 * @section LICENSE
 * Copyright 2020 Sergei Akhmatdinov
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied *.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/*
 * Adopted from RLUtil's test.cpp.
 */

#include "../rogueutil.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#ifndef _WIN32
/* Borrowed from: http://www.strudel.org.uk/itoa/ */
static char*
itoa(int val, int base)
{
	static char buf[32] = {0};
	int i = 30;

	for(; val && i ; --i, val /= base)
		buf[i] = "0123456789abcdef"[val % base];

        return &buf[i+1];
}
#endif /* _WIN32 */

static inline char
chargen()
{
	return rand() % (('~' - '!') + 1) + '!';
}

static inline void
waitkey(void)
{
        anykey("Press any key to continue...\n");
}

int
main(void)
{
        int i;
        /* Seed the RCG */
        srand(time(NULL));
	saveDefaultColor();

	printf("Welcome to the Rogueutil C test program!\n");
	waitkey();

	printf("\nTest 1: Colors\n");

	for (i = 0; i < 16; i++) {
		setColor(i);
		printf("%d ", i);
	}

	resetColor();
	printf("\nYou should see numbers 0-15 in different colors.\n");
	waitkey();

	printf("\nTest 2: Background colors\n");
	for (i = 0; i < 8; i++) {
		setBackgroundColor(i);
		printf("%d", i);
		setBackgroundColor(BLACK);
		printf(" ");
	}

	resetColor();
	printf("\nYou should see numbers 0-7 in different-colored backgrounds.\n");
	waitkey();

	cls();
	printf("Test 3: Clear screen\n");
	printf("\nThe colors should now be gone.\n");
	waitkey();

	printf("\nTest 4: Cursor hiding\n");
	hidecursor();
	printf("\nThe cursor should now be invisible.\n");
	waitkey();

	printf("\nTest 5: Cursor showing\n");
	showcursor();
	printf("\nThe cursor should now be visible again.\n");
	waitkey();

	cls();
	printf("\nTest 6: Cursor positioning\n");
        printXY(16, 6, "(16, 6)");
        printXY(4, 3, "(4,3)");
        printXY(8, 8, "(8,8)");

	printf("\nYou should see three coordinates in "
               "their specified locations.\n");
	waitkey();

        /* Test 7 */
	{
                int x = 7;
                int y = 7;
                unsigned int cnt = 0;
                hidecursor();

		for(;;) {
			cls();
			printf("\nTest 7: Blocking keyboard input\n");
			printf("You should be able to move the '@' character "
                               "with WASD keys.\n");
			printf("Hit Space to continue to the next test.\n");
			printf("Turn count: %u", cnt);
			printXY(x, y, "@");

                        char k = getch();

                        if (k == 'a') --x;
			else if (k == 'd') ++x;
			else if (k == 'w') --y;
			else if (k == 's') ++y;
			else if (k == ' ') break;
			cnt++;
		}

                showcursor();
	}

        /* Test 8 */
	{
                int x = 7;
                int y = 7;
                unsigned int cnt = 0;
		hidecursor();
		cls();

		printf("\nTest 8: Non-blocking keyboard input\n");
		printf("You should be able to move the '@' character "
                       "with WASD keys.\n");
		printf("Hit Space to continue to the next test.\n");
		printXY(x, y, "@");

		for(;;) {
			printXY(1, 5, "Turn count: ");

                        #ifndef _WIN32
                        	printXY(13, 5, itoa(cnt, 10));
			#else /* _WIN32 */
                                char buf[11];
                                itoa(cnt, buf, 10);
                                printXY(13, 5, buf);
			#endif /* _WIN32 */

			if (kbhit()) {
				char k = getch();
				printXY(x, y, " "); /* Erase player */

				if (k == 'a') --x;
				else if (k == 'd') ++x;
				else if (k == 'w') --y;
				else if (k == 's') ++y;
				else if (k == ' ') break;

                                printXY(x, y, "@");
			}
			cnt++;
			fflush(stdout);
		}

		showcursor();
	}

        /* Test 9 */
	{
                int x = 9;
                int y = 9;

		cls();
		hidecursor();

                printf("\nTest 9: Arrow keys\n");
                printf("You should be able to move the '@' character "
                       "with arrow keys.\n");
                printf("Hit Escape to continue to the next test.\n");

                /* Currently doesn't happen until kbhit() */
                printXY(x, y, "@");

		for(;;) {
			if (kbhit()) {
                                cls();

                                /* Printing the header again after a cls() *
                                 * in the loop helps avoid echoes that     *
                                 * sometimes appear.                       */
                                printf("\nTest 9: Arrow keys\n");
                                printf("You should be able to move the "
                                       "'@' character with arrow keys.\n");
                                printf("Hit Escape to continue to the next "
                                       "test.\n");
				int k = getkey();
				printXY(x, y, " "); /* Erase player */

				if (k == KEY_LEFT) --x;
				else if (k == KEY_RIGHT) ++x;
				else if (k == KEY_UP) --y;
				else if (k == KEY_DOWN) ++y;
				else if (k == KEY_ESCAPE) break;

                                printXY(x, y, "@");
				fflush(stdout);
			}
		}

                showcursor();
	}

	cls();
	printf("\nTest 10: Delay\n");
	printf("Next numbers should appear rapidly after each other "
               "(but not instantly).\n");
	msleep(500);
        float t;
	for (t = 0; t <= 2; t += 0.25f) {
		printf("%.2fs\n", t);
		msleep(250);
	}

        /* Tends to skip waitkey() without another msleep */
        msleep(250);
        waitkey();

	cls();
	printf("\nTest 11: Terminal Dimensions\n");
	printf("You should see the size in character rows and columns "
               "of your terminal window.\n");
        printf("%d Rows\n", trows());
        printf("%d Columns\n", tcols());
	waitkey();

	cls();
	printf("\nTest 12: Non-advancing character setting\n");
	printf("Next ASCII characters should appear rapidly after each other "
               "in the same position.\n");
	{
		hidecursor();

		for(char c = '!'; c <= '~'; ++c) {
			setChar(c);
			fflush(stdout);
			msleep(75);
		}

                showcursor();
	}
	waitkey();

	cls();
	printf("\nTest 13: Non-advancing string setting\n");
	printf("Next random character strings should appear rapidly "
               "after each other starting in the same position.\n");
	{
		hidecursor();

		const int maxlen = tcols() / 2;
                int l;

		char buf[maxlen + 1];

                for(l = 1; l < maxlen; ++l) {
                        for (i = 0; i < l; i++)
                                buf[i] = chargen();

			setString(buf);
			fflush(stdout);
			msleep(75);
		}

                showcursor();
	}
	printf("\n");
	waitkey();

	cls();
	printf("\nTest 14: Terminal window title\n");
	{
                char title[15];

                for (i = 0; i < 14; i++)
                        title[i] = chargen();

		setConsoleTitle(title);
		printf("Your terminal window should now be named %s\n", title);
	}
        waitkey();

        printf("\nTest 15: Getting the username.\n");
        printf("%s\n", getUsername());
        printf("\nYou should see your username above.\n");
        waitkey();

	printf("\nAll tests done. Bye!\n");
	return 0;
}
