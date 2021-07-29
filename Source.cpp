#include <iostream>
#include <Windows.h>
#include <vector>


const int screenWidth = 120; // Console Screen Size X (columns)
const int screenHeight = 40; // Console Screen Size Y (rows)


int count_live_cells(int* cell, int x, int y)
{
	int sum = 0;

	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			int x_pos = x - 1 + i;
			int y_pos = y - 1 + j;

			// Prevents inclusion of centre cell in list of adjacent cells
			if (i == 1 && j == 1) { continue; }

			if (x_pos < 0 || x_pos >= screenWidth || y_pos < 0 || y_pos >= screenHeight) { continue; }

			sum += cell[x_pos + screenWidth * y_pos];
		}
	}

	return sum;
}



int main()
{
	// Time
	int time = 500; // approx miliseconds between frames

	// Create cells
	int* cells = new int[screenWidth * screenHeight]; // Question: When to delete this memory
	
	// Initialize cells
	for (int x = 0; x < screenWidth; x++)
	{
		for (int y = 0; y < screenHeight; y++)
		{
			cells[x + screenWidth * y] = 0;
		}
	}

	// Set cells
	cells[10 + 10 * screenWidth] = 1;
	cells[11 + 11 * screenWidth] = 1;
	cells[12 + 10 * screenWidth] = 1;
	cells[12 + 9 * screenWidth] = 1;
	cells[12 + 11 * screenWidth] = 1;


	// Create screen buffer
	wchar_t* screen = new wchar_t[screenWidth * screenHeight];
	HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	SetConsoleActiveScreenBuffer(hConsole);
	DWORD dwBytesWritten = 0;

	//-------------------------
	// Draw initial frame

	// Set cells to frame
	for (int x = 0; x < screenWidth; x++)
	{
		for (int y = 0; y < screenHeight; y++)
		{
			char toDraw = (cells[x + screenWidth * y]) ? '#' : ' ';
			screen[x + screenWidth * y] = toDraw;
		}
	}

	// Display frame
	screen[screenWidth * screenHeight - 1] = '\0';
	WriteConsoleOutputCharacter(hConsole, screen, screenWidth * screenHeight, { 0,0 }, &dwBytesWritten);

	// Controls loop timing
	Sleep(time);
	//-------------------------

	// Game loop
	while (true)
	{
		// Update cells (this is soooo bad)
		int* temp_cells = new int[screenWidth * screenHeight];

		for (int i = 0; i < screenWidth * screenHeight; i++)
		{
			temp_cells[i] = cells[i];
		}

		for (int x = 0; x < screenWidth; x++)
		{
			for (int y = 0; y < screenHeight; y++)
			{
				int live = count_live_cells(cells, x, y);
				switch (live)
				{
				case 2:
					// If there are 2 live neighbours, alive cells stay alive, and dead cells stay dead; so nothing happens.
					break;

				case 3:
					// If there are 3 live neighbours, the cell becomes alive or stays alive
					temp_cells[x + screenWidth * y] = 1;
					break;

				default:
					// If there aren't 2 or 3 live neighbours, the cell dies or stays dead
					temp_cells[x + screenWidth * y] = 0;
				}
			}
		}


		for (int i = 0; i < screenWidth * screenHeight; i++)
		{
			cells[i] = temp_cells[i];
		}

		delete[] temp_cells; // Deletes the temporary cell array after cells have been updated.

		// -----------------------
		// Draw cells to frame
		// -----------------------

		// Set cells to frame
		for (int x = 0; x < screenWidth; x++)
		{
			for (int y = 0; y < screenHeight; y++)
			{
				char toDraw = (cells[x + screenWidth * y]) ? '#' : ' ';
				screen[x + screenWidth * y] = toDraw;
			}
		}

		// Display frame
		screen[screenWidth * screenHeight - 1] = '\0';
		WriteConsoleOutputCharacter(hConsole, screen, screenWidth * screenHeight, { 0,0 }, &dwBytesWritten);

		// Controls loop timing
		Sleep(time);
	}

	return 0;
}
