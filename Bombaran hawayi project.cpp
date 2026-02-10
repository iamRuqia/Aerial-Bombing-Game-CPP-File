#include <iostream>
#include <conio.h>
#include <windows.h>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <string>

using namespace std;

const int WIDTH = 60;
const int HEIGHT = 20;

HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
CHAR_INFO consoleBuffer[WIDTH * HEIGHT];
COORD bufferSize = { WIDTH, HEIGHT };
COORD bufferCoord = { 0, 0 };
SMALL_RECT writeRegion = { 0, 0, WIDTH - 1, HEIGHT - 1 };

void gotoxy(int x, int y) {
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(hConsole, coord);
}

void hideCursor() {
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(hConsole, &cursorInfo);
    cursorInfo.bVisible = false;
    SetConsoleCursorInfo(hConsole, &cursorInfo);
}

class GameObject {
public:
    int x, y;
    GameObject(int x, int y) : x(x), y(y) {}
};

class Airplane : public GameObject {
public:
    Airplane(int x, int y) : GameObject(x, y) {}
};

class Bomb : public GameObject {
public:
    bool active;
    Bomb(int x, int y) : GameObject(x, y), active(false) {}
};

class Building : public GameObject {
public:
    int floors;
    bool destroyed;
    Building(int x, int y, int floors) : GameObject(x, y), floors(floors), destroyed(false) {}
};

void drawGame(const Airplane& plane, const Bomb& bomb, const vector<Building>& buildings, int score, const string& message = "") {
    for (int i = 0; i < WIDTH * HEIGHT; i++) {
        consoleBuffer[i].Char.UnicodeChar = ' ';
        consoleBuffer[i].Attributes = 7;
    }

    if (plane.y >= 0 && plane.y < HEIGHT && plane.x >= 0 && plane.x < WIDTH) {
        int index = plane.y * WIDTH + plane.x;
        consoleBuffer[index].Char.UnicodeChar = 'A';
        consoleBuffer[index].Attributes = FOREGROUND_GREEN | FOREGROUND_INTENSITY;
    }

    for (int dy = -1; dy <= 1; dy++) {
        for (int dx = -1; dx <= 1; dx++) {
            int x = plane.x + dx;
            int y = plane.y + 1 + dy;
            if (x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT) {
                int index = y * WIDTH + x;
                if (dx == -1) {
                    consoleBuffer[index].Char.UnicodeChar = '/';
                } else if (dx == 0) {
                    consoleBuffer[index].Char.UnicodeChar = '|';
                } else if (dx == 1) {
                    consoleBuffer[index].Char.UnicodeChar = '\\';
                }
                consoleBuffer[index].Attributes = FOREGROUND_GREEN | FOREGROUND_INTENSITY;
            }
        }
    }

    if (plane.y + 2 >= 0 && plane.y + 2 < HEIGHT) {
        if (plane.x - 2 >= 0 && plane.x - 2 < WIDTH) {
            int index = (plane.y + 2) * WIDTH + (plane.x - 2);
            consoleBuffer[index].Char.UnicodeChar = 'o';
            consoleBuffer[index].Attributes = FOREGROUND_RED | FOREGROUND_INTENSITY;
        }
        if (plane.x + 2 >= 0 && plane.x + 2 < WIDTH) {
            int index = (plane.y + 2) * WIDTH + (plane.x + 2);
            consoleBuffer[index].Char.UnicodeChar = 'o';
            consoleBuffer[index].Attributes = FOREGROUND_RED | FOREGROUND_INTENSITY;
        }
    }

    if (bomb.active && bomb.y >= 0 && bomb.y < HEIGHT && bomb.x >= 0 && bomb.x < WIDTH) {
        int index = bomb.y * WIDTH + bomb.x;
        consoleBuffer[index].Char.UnicodeChar = '*';
        consoleBuffer[index].Attributes = FOREGROUND_RED | FOREGROUND_INTENSITY;
    }

    for (const auto& building : buildings) {
        if (!building.destroyed) {
            for (int y = HEIGHT - building.floors; y < HEIGHT; y++) {
                if (y >= 0 && y < HEIGHT && building.x >= 0 && building.x < WIDTH) {
                    int index = y * WIDTH + building.x;
                    consoleBuffer[index].Char.UnicodeChar = '#';
                    consoleBuffer[index].Attributes = FOREGROUND_BLUE | FOREGROUND_INTENSITY;
                }
            }
        }
    }

    for (int x = 0; x < WIDTH; x++) {
        int index = (HEIGHT - 1) * WIDTH + x;
        consoleBuffer[index].Char.UnicodeChar = '_';
        consoleBuffer[index].Attributes = FOREGROUND_GREEN;
    }

    WriteConsoleOutput(hConsole, consoleBuffer, bufferSize, bufferCoord, &writeRegion);

    gotoxy(0, HEIGHT + 1);
    cout << "Imtiyaz: " << score << "  ";
    cout << "Ertfae Hawapayma: " << HEIGHT - plane.y << "  ";
    cout << "Dastorat: Space=Partab bomb, Q=Khoroj, Up/Down=Tanzim surat";
    
    if (!message.empty()) {
        gotoxy(0, HEIGHT + 2);
        cout << "=== " << message << " ===          ";
    }
}

void showMenu() {
    system("cls");
    cout << "============= bomBbaran Hwayi =============" << endl;
    cout << "\nBa bazie bobaran hawayi khosh amadid!" << endl;
    cout << "\nQawanin bazi:" << endl;
    cout << "1. Sakhtman hara (#) nabod konid, ba hawapaymay khud." << endl;
    cout << "2. Az barkhurd ba sakhtman ha khudDari nmayid" << endl;
    cout << "3. Ertfae hawapayma dar har marhala kam mishawad" << endl;
    cout << "\nBaray shoro dkmae Enter ra fishar dhid...";
    cout << "\nBaray khoroj az bazi dkmae Q ra fishar dehid...";
    cin.get();
}

int main() {
    SetConsoleOutputCP(65001);
    hideCursor();
    
    srand(time(0));
    showMenu();
    
    SMALL_RECT windowSize = { 0, 0, WIDTH - 1, HEIGHT + 5 };
    SetConsoleWindowInfo(hConsole, TRUE, &windowSize);
    
    Airplane plane(5, 5);
    Bomb bomb(0, 0);
    vector<Building> buildings;
    int score = 0;
    int speed = 1;
    int altitudeLoss = 0;
    
    for (int i = 0; i < 5; i++) {
        int x = 10 + rand() % (WIDTH - 20);
        int floors = 2 + rand() % 5;
        buildings.push_back(Building(x, HEIGHT - 1, floors));
    }
    
    bool gameOver = false;
    while (!gameOver) {
        plane.x += speed;
        if (plane.x >= WIDTH) {
            plane.x = 0;
            plane.y++;
            altitudeLoss++;
            
            if (plane.y >= HEIGHT - 1) {
                gameOver = true;
                gotoxy(0, HEIGHT + 3);
                cout << "\nHawapayma soqot kard, Khatm bazi." << endl;
                break;
            }
        }
        
        if (bomb.active) {
            bomb.y++;
            
            bool hitBuilding = false;
            for (auto& building : buildings) {
                if (!building.destroyed && bomb.x == building.x && bomb.y >= HEIGHT - building.floors) {
                    building.destroyed = true;
                    bomb.active = false;
                    score += building.floors * 10;
                    cout << "\a";
                    drawGame(plane, bomb, buildings, score, "Hadaf nabod shud! (+" + to_string(building.floors * 10) + " imtiyaz)");
                    hitBuilding = true;
                    Sleep(1500);
                    break;
                }
            }
            
            if (!hitBuilding && bomb.y >= HEIGHT) {
                bomb.active = false;
                score -= 5;
                cout << "\a";
                drawGame(plane, bomb, buildings, score, "Khata! Bomb ba hadaf nakhord (-5 Imtiyaz)");
                Sleep(1500);
            }
        }
        
        for (const auto& building : buildings) {
            if (!building.destroyed) {
                if (plane.x == building.x && plane.y >= HEIGHT - building.floors) {
                    gameOver = true;
                    gotoxy(0, HEIGHT + 3);
                    cout << "\nHawapayma ba sakhtman barkhurd kard! Khatm bazi." << endl;
                    break;
                }
                if ((plane.x - 1 == building.x || plane.x + 1 == building.x) && plane.y + 1 >= HEIGHT - building.floors) {
                    gameOver = true;
                    gotoxy(0, HEIGHT + 3);
                    cout << "\nHawapayma ba sakhtman barkhurd kard! Khatm bazi." << endl;
                    break;
                }
            }
        }
        
        drawGame(plane, bomb, buildings, score);
        
        if (_kbhit()) {
            int ch = _getch();
            
            if (ch == 0 || ch == 224) { 
                ch = _getch();
                switch(ch) {
                    case 72: speed = min(speed + 1, 3); break; 
                    case 80: speed = max(speed - 1, 1); break; 
                }
            }
            else {
                switch(tolower(ch)) {
                    case ' ':
                        if (!bomb.active) {
                            bomb.x = plane.x;
                            bomb.y = plane.y + 2;
                            bomb.active = true;
                        }
                        break;
                    case 'q':
                        gameOver = true;
                        gotoxy(0, HEIGHT + 3);
                        cout << "\nBazi ra tark kardid. Imtiyaz nehayi: " << score << endl;
                        Sleep(2000);
                        return 0;
                }
            }
        }
        
        Sleep(100);
    }
    
    gotoxy(0, HEIGHT + 4);
    cout << "\nImtiyaz nehayi shuma: " << score << endl;
    cout << "Thanks for playing!" << endl;
    
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(hConsole, &cursorInfo);
    cursorInfo.bVisible = true;
    SetConsoleCursorInfo(hConsole, &cursorInfo);
    
    return 0;
}
