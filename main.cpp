#include <windows.h>
#include <iostream>
#include <conio.h>
#include <fstream>
using namespace std;

void setColor(int color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

void hideCursor() {
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO info;
    info.dwSize = 100;
    info.bVisible = FALSE;
    SetConsoleCursorInfo(h, &info);
}

void gotoxy(int x, int y) {
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD pos;
    pos.X = x;
    pos.Y = y;
    SetConsoleCursorPosition(h, pos);
}

bool gameOver;

int score, highScore;
int dinoY;
int jump = 0;
int speed = 60;
int frameCount = 0;

int obstacle1X, obstacle2X;

// Load/Save
void loadHighScore() {
    ifstream file("highscore.txt");
    if (file.is_open()) file >> highScore;
    else highScore = 0;
    file.close();
}

void saveHighScore() {
    ofstream file("highscore.txt");
    file << highScore;
    file.close();
}

void setup() {
    gameOver = false;
    score = 0;
    dinoY = 0;
    obstacle1X = 60;
    obstacle2X = 90;
    frameCount = 0;
    speed = 60;
}

// 💥 FLASH EFFECT
void flashScreen() {
    for (int i = 0; i < 2; i++) {
        setColor(15);
        system("cls");
        Sleep(60);
        setColor(0);
        system("cls");
        Sleep(60);
    }
}

// DRAW
void draw() {
    gotoxy(0, 0);

    bool isNight = ((score / 5) % 2 == 1);

    // Clear screen
    setColor(7);
    for (int i = 0; i < 20; i++) {
        gotoxy(0, i);
        cout << string(100, ' ');
    }

    // 🌙 NIGHT
    if (isNight) {
        setColor(15);
        gotoxy(10, 2); cout << (rand() % 2 ? "*" : ".");
        gotoxy(20, 3); cout << "*";

        setColor(7);
        gotoxy(70, 2); cout << " _";
        gotoxy(70, 3); cout << " ) \\";
        gotoxy(70, 4); cout << "  ) |";
        gotoxy(70, 5); cout << " )_/";
    }

    // ☀️ DAY
    else {
        setColor(14);
        gotoxy(70, 2); cout << " \\|/ ";
        gotoxy(70, 3); cout << "--O--";
        gotoxy(70, 4); cout << " /|\\ ";

        setColor(11);
        gotoxy(10, 2); cout << "   ____   ";
        gotoxy(10, 3); cout << " _(    )_ ";
        gotoxy(10, 4); cout << "(________)";

        gotoxy(35, 3); cout << "   ____   ";
        gotoxy(35, 4); cout << " _(    )_ ";
        gotoxy(35, 5); cout << "(________)";
    }

    int dinoX = 10;
    int groundY = 12;

    // Ground
    setColor(8);
    gotoxy(0, groundY);
    for (int i = 0; i < 100; i++) cout << "_";

    int frame = frameCount % 4;

    // 🦖 DINO (alive vs dead)
    if (gameOver) setColor(8);
    else setColor(15);

    gotoxy(dinoX, groundY - dinoY - 3); cout << "   __";

    gotoxy(dinoX, groundY - dinoY - 2);
    if (gameOver)
        cout << " _/ x\\_>";
    else
        cout << " _/ o\\_>";

    gotoxy(dinoX, groundY - dinoY - 1); cout << "|_>  _|";

    gotoxy(dinoX, groundY - dinoY);
    if (dinoY > 0) {
        cout << "  | |";
    } else {
        if (frame == 0) cout << "  / \\";
        else if (frame == 1) cout << "  | |";
        else if (frame == 2) cout << "  \\ /";
        else cout << "  | |";
    }

    // 🌵 CACTUS
    setColor(10);
    gotoxy(obstacle1X, groundY - 2); cout << " | ";
    gotoxy(obstacle1X, groundY - 1); cout << "| |";
    gotoxy(obstacle1X, groundY);     cout << " | ";

    gotoxy(obstacle2X, groundY - 2); cout << " | ";
    gotoxy(obstacle2X, groundY - 1); cout << "| |";
    gotoxy(obstacle2X, groundY);     cout << " | ";

    // Score
    setColor(15);
    gotoxy(0, groundY + 2);
    cout << "                                                                                ";
    gotoxy(2, groundY + 2);
    cout << "Score: " << score << "   High Score: " << highScore;
}

// INPUT
void input() {
    if (_kbhit()) {
        char ch = _getch();
        if (ch == ' ' && dinoY == 0)
            jump = 6;
    }
}

// LOGIC
void logic() {
    frameCount++;

    if (!gameOver) {
        if (jump > 0) {
            dinoY += 2;
            jump--;
        } else if (dinoY > 0) {
            dinoY -= 2;
            if (dinoY < 0) dinoY = 0;
        }

        obstacle1X -= 2;
        obstacle2X -= 2;

        if (obstacle1X < 0) {
            obstacle1X = 80 + rand() % 20;
            score++;
        }

        if (obstacle2X < 0) {
            obstacle2X = 100 + rand() % 20;
            score++;
        }

        if (score % 3 == 0 && speed > 20)
            speed -= 2;

        if (score > highScore)
            highScore = score;

        // 💥 COLLISION
        if ((obstacle1X >= 8 && obstacle1X <= 15 && dinoY < 3) ||
            (obstacle2X >= 8 && obstacle2X <= 15 && dinoY < 3)) {
            gameOver = true;
            flashScreen();
        }
    }
}

// MENU
void menu() {
    system("cls");
    cout << "====== DINO GAME ======\n";
    cout << "1. Start Game\n";
    cout << "2. Exit\n";
    cout << "Choose: ";

    int choice;
    cin >> choice;

    if (choice != 1)
        exit(0);
}

// MAIN
int main() {
    hideCursor();
    loadHighScore();
    menu();
    setup();

    while (!gameOver) {
        draw();
        input();
        logic();
        Sleep(speed);
    }

    draw(); // show dead dino

    saveHighScore();

    gotoxy(0, 15);
    cout << "\nGAME OVER!\nFinal Score: " << score << endl;

    system("pause");
    return 0;
}

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
    return main();
}