#include "raylib.h"
#include "external/stb_image.h"
#include <string>
#include <fstream>

static unsigned short maskBit(unsigned short value, int begin, int end)
{
	unsigned short mask = (1 << (end - begin)) - 1;
	return (value >> begin) & mask;
}

int main()
{
	InitWindow(800, 640, "raysan5's decoding software diy ⚠️");
	SetTargetFPS(60);
	int currentScreen = 0;
	int menuSelection = 0;
	int selectedChannel = 0;
	int selectedBit = 0;
	long long startingPixel = 0;
	int wordSize = 0;
	int key = 0;
	bool fileAlreadyDropped = false;
	FilePathList paths{};
	int width, height, bpp;
	uint8_t* selectedImage;
	std::string textInput = "";
	int wordCounter = 0;
	std::string currentWord = "";
	std::string message = "";
	while (!WindowShouldClose())
	{
		BeginDrawing();
			ClearBackground(RAYWHITE);
			DrawText("raysan5's decoding suite", 10, 10, 30, BLACK);
			switch (currentScreen)
			{
			case 0:
				DrawText("Select Channel & bit to remember", 10, 50, 30, BLACK);
				switch (menuSelection)
				{
				case 0:
					DrawRectangleGradientV(5, 90, 70, 50, RAYWHITE, GOLD);
					break;
				case 1:
					DrawRectangleGradientV(100, 90, 110, 50, RAYWHITE, GOLD);
					break;
				default:
					DrawRectangleGradientV(240, 90, 90, 50, RAYWHITE, GOLD);
					break;
				}
				DrawText("RED", 10, 100, 30, RED);
				DrawText("GREEN", 105, 100, 30, GREEN);
				DrawText("BLUE", 245, 100, 30, BLUE);
				if (IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_D))
				{
					menuSelection++;
					if (menuSelection > 2)
						menuSelection = 0;
				}
				else if (IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_A))
				{
					menuSelection--;
					if (menuSelection < 0)
						menuSelection = 2;
				}
				else if (IsKeyPressed(KEY_ENTER))
				{
					currentScreen++;
					selectedChannel = menuSelection;
					menuSelection = 0;
				}
				break;
			case 1:
				DrawText("Select Channel & bit to remember", 10, 50, 30, BLACK);
				switch (selectedChannel)
				{
				case 0:
					DrawRectangleGradientV(5, 90, 70, 50, RAYWHITE, GOLD);
					break;
				case 1:
					DrawRectangleGradientV(100, 90, 110, 50, RAYWHITE, GOLD);
					break;
				default:
					DrawRectangleGradientV(240, 90, 90, 50, RAYWHITE, GOLD);
					break;
				}
				DrawText("RED", 10, 100, 30, RED);
				DrawText("GREEN", 105, 100, 30, GREEN);
				DrawText("BLUE", 245, 100, 30, BLUE);
				switch (menuSelection)
				{
				case 0:
					DrawRectangleGradientV(10, 190, 15, 50, RAYWHITE, RED);
					break;
				case 1:
					DrawRectangleGradientV(28, 190, 15, 50, RAYWHITE, RED);
					break;
				case 2:
					DrawRectangleGradientV(28+18, 190, 15, 50, RAYWHITE, RED);
					break;
				case 3:
					DrawRectangleGradientV(28+(18*2), 190, 15, 50, RAYWHITE, RED);
					break;
				case 4:
					DrawRectangleGradientV(28+(18*3), 190, 15, 50, RAYWHITE, RED);
					break;
				case 5:
					DrawRectangleGradientV(28+(18*4), 190, 15, 50, RAYWHITE, RED);
					break;
				case 6:
					DrawRectangleGradientV(28+(18*5), 190, 15, 50, RAYWHITE, RED);
					break;
				default:
					DrawRectangleGradientV(28+(18*6), 190, 15, 50, RAYWHITE, RED);
					break;
				}
				DrawText("00000000", 10, 200, 30, BLACK);
				if (IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_D))
				{
					menuSelection++;
					if (menuSelection > 7)
						menuSelection = 0;
				}
				else if (IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_A))
				{
					menuSelection--;
					if (menuSelection < 0)
						menuSelection = 7;
				}
				else if (IsKeyPressed(KEY_ENTER))
				{
					currentScreen++;
					selectedBit = menuSelection;
					menuSelection = 0;
				}
				break;
			case 2:
				DrawText("Type Starting Pixel to remember", 10, 50, 30, BLACK);
				// Set the window's cursor to the I-Beam
				SetMouseCursor(MOUSE_CURSOR_IBEAM);

				// Get char pressed (unicode character) on the queue
				key = GetCharPressed();

				// Check if more characters have been pressed on the same frame
				while (key > 0)
				{
					// NOTE: Only allow keys in range [32..125]
					if ((key >= 32) && (key <= 125) && (textInput.size() < 7))
					{
						textInput = textInput.substr(0, menuSelection) + (char)key + textInput.substr(menuSelection);
						menuSelection++;
					}

					key = GetCharPressed();  // Check next character in the queue
				}

				if (IsKeyPressed(KEY_BACKSPACE))
				{
					if (menuSelection > 0)
					{
						textInput = textInput.substr(0, menuSelection - 1) + textInput.substr(menuSelection);
						menuSelection--;
					}
				}
				else if (IsKeyPressed(KEY_LEFT))
				{
					if (menuSelection > 0) menuSelection--;
				}
				else if (IsKeyPressed(KEY_RIGHT))
				{
					if (menuSelection < 8 && menuSelection < textInput.size()) menuSelection++;
				}
				else if (IsKeyPressed(KEY_ENTER))
				{
					startingPixel = std::stoi(textInput);
					textInput = "";
					currentScreen++;
					menuSelection = 0;
				}
				DrawRectangle(10, 100, 650, 50, BLACK);
				DrawText(textInput.c_str(), 20, 100, 30, WHITE);
				break;
			case 3:
				DrawText("Type Word Size to remember", 10, 50, 30, BLACK);
				// Set the window's cursor to the I-Beam
				SetMouseCursor(MOUSE_CURSOR_IBEAM);

				// Get char pressed (unicode character) on the queue
				key = GetCharPressed();

				// Check if more characters have been pressed on the same frame
				while (key > 0)
				{
					// NOTE: Only allow keys in range [32..125]
					if ((key >= 32) && (key <= 125) && (textInput.size() < 7))
					{
						textInput = textInput.substr(0, menuSelection) + (char)key + textInput.substr(menuSelection);
						menuSelection++;
					}

					key = GetCharPressed();  // Check next character in the queue
				}

				if (IsKeyPressed(KEY_BACKSPACE))
				{
					if (menuSelection > 0)
					{
						textInput = textInput.substr(0, menuSelection - 1) + textInput.substr(menuSelection);
						menuSelection--;
					}
				}
				else if (IsKeyPressed(KEY_LEFT))
				{
					if (menuSelection > 0) menuSelection--;
				}
				else if (IsKeyPressed(KEY_RIGHT))
				{
					if (menuSelection < 8 && menuSelection < textInput.size()) menuSelection++;
				}
				else if (IsKeyPressed(KEY_ENTER))
				{
					wordSize = std::stoi(textInput);
					textInput = "";
					currentScreen++;
					menuSelection = 0;
				}
				DrawRectangle(10, 100, 650, 50, BLACK);
				DrawText(textInput.c_str(), 20, 100, 30, WHITE);
				break;
			case 4:
				DrawText("Drop Image file to encode/decode", 10, 50, 30, BLACK);
				if (IsFileDropped() && !fileAlreadyDropped)
				{
					paths = LoadDroppedFiles();
					menuSelection = 1;
					fileAlreadyDropped = true;
				}
				if (menuSelection > 0)
				{
					DrawRectangleGradientV(10, 95, 110, 50, RAYWHITE, GOLD);
					DrawText("Decode?", 10, 100, 30, BLACK);
					if (IsKeyPressed(KEY_ENTER))
					{
						currentScreen++;
						menuSelection = 0;
					}
				}
				break;
			case 5:
				selectedImage = stbi_load(paths.paths[0], &width, &height, &bpp, 3);
				// loop through every pixel and read each character
				for (int i = startingPixel; i < width*height*3; i += 3)
				{
					wordCounter++;
					if (wordCounter == wordSize - 1)
					{
						wordCounter = 0;
						switch (selectedChannel)
						{
						case 0:
							if ((char)maskBit(selectedImage[i], selectedBit, 8) == '\x1')
							{
								currentWord += '1';
							}
							else
							{
								currentWord += '0';
							}
							break;
						case 1:
							if ((char)maskBit(selectedImage[i + 1], selectedBit, 8) == '\x1')
							{
								currentWord += '1';
							}
							else
							{
								currentWord += '0';
							}
							break;
						default:
							if ((char)maskBit(selectedImage[i + 2], selectedBit, 8) == '\x1')
							{
								currentWord += '1';
							}
							else
							{
								currentWord += '0';
							}
							break;
						}
					}
				}
				stbi_image_free(selectedImage);
				currentScreen++;
				break;
			case 6:
				for (int i = 0; i < currentWord.size(); i += 8)
				{
					message += static_cast<char>(std::stoi(currentWord.substr(i,8), nullptr, 2));
				}
				DrawText(TextFormat("SECRET MESSAGE = %s", message), 10, 100, 50, BLACK);
				break;
			}
		EndDrawing();
	}
	return 0;
}
