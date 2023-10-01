//Classes,Objects,Polymorphism,Inheritance,Abstract classes,Virtual function
//Operator overloading,Vector,File handling

#include <iostream>
#include <conio.h>
#include <windows.h>
#include <ctime>
#include <vector>
#include <fstream> // Added for file handling
using namespace std;

// Constants for screen dimensions and game play settings
const int SCREEN_WIDTH = 90;   //This constant represents the width of the entire game screen
const int SCREEN_HEIGHT = 26;  //This constant represents the height of the entire game screen
const int WIN_WIDTH = 70;      //This constant represents the width of the playable area of the game screen
const int MENU_WIDTH = 20;     //This constant represents the width of the menu
const int GAP_SIZE = 7;        //This constant represents the gap size or spacing between elements in the game
const int ENEMY_DIF = 30;      // Increase this value to slow down the enemy

const int MAX_BULLETS = 20;

HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
COORD CursorPosition;

int enemyX[3];
int enemyY[3];

// Function to set the cursor position
void gotoxy(int x, int y) {
    CursorPosition.X = x;
    CursorPosition.Y = y;
    SetConsoleCursorPosition(console, CursorPosition);
}

// Base class for game entities
class Entity {
public:
    int x, y;
    int width, height;
    char symbol;

    Entity(int x, int y, int width, int height, char symbol)
        : x(x), y(y), width(width), height(height), symbol(symbol) {}

    // Virtual functions to draw and erase the entity
    virtual void draw() const {
        gotoxy(x, y);
        cout << symbol;
    }

    virtual void erase() const {
        gotoxy(x, y);
        cout << ' ';
    }

    virtual void move() {}
};

// Class representing enemy aliens
class Alien : public Entity {
private:
    bool active;

public:
    int timer;

    Alien()
        : Entity(0, 0, 5, 4, 'M'), active(false), timer(0) {}

    // Constructor to initialize 'x' and 'y'
    Alien(int startX, int startY)
        : Entity(startX, startY, 5, 4, 'M'), active(true), timer(0) {}

    // Method to draw the alien
  void drawAlien() {
    if (isActive()) {
        gotoxy(x, y);
        cout << "  /\\  ";
        gotoxy(x, y + 1);
        cout << " |**| ";

    }
}



    // Method to erase the alien
    void erase() {
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                gotoxy(x + j, y + i);
                cout << ' ';
            }
        }
    }

    bool isActive() const {
        return active;
    }

    void setActive(bool value) {
        active = value;
    }

    // Method to spawn an alien
    void spawn() {
        x = 3 + rand() % (WIN_WIDTH - 10);
        y = 4;
        active = true;
        timer = 0;
    }

    // Method to update the alien's timer
    void updateTimer() {
        timer++;
        if (timer >= ENEMY_DIF) {
            erase();  // Erase the alien
            spawn(); // Respawn the alien
        }
    }

    // Method to move the alien downward
    void move() override {
        if (active && y < SCREEN_HEIGHT - 5) {
            erase();
            y++;
            draw();
            updateTimer(); // Update the timer
        }
        else {
            active = false;
        }
    }
};

// Class representing bullets
class Bullet : public Entity {
private:
    bool active;

public:
    Bullet()
        : Entity(0, 0, 1, 1, '|'), active(false) {}

    bool isActive() const {
        return active;
    }

    void setActive(bool value) {  //This  function allows  to set the active status of the bullet
        active = value;
    }

  // Method to shoot a bullet from the left side of the spaceship
void shootLeft(int x, int y) {
    if (!active) {
        this->x = x + 0; // Adjust the x-coordinate to align with the "|" sign
        this->y = y + 2; // Adjust the y-coordinate to align with the "|" sign
        active = true;
    }
}

// Method to shoot a bullet from the right side of the spaceship
void shootRight(int x, int y) {
    if (!active) {
        this->x = x + 2; // Adjust the x-coordinate to align with the "|" sign
        this->y = y + 2; // Adjust the y-coordinate to align with the "|" sign
        active = true;
    }
}


    // Method to move the bullet upward
    void move() override {
        if (active) {
            erase();
            if (y > 0) {
                y--;
                draw();
            } else {
                active = false;
            }
        }
    }

      // Method to check if the bullet hits an alien
    bool hitsAlien(const Alien& alien) const {
        if (active && alien.isActive() &&
            x >= alien.x && x <= alien.x + alien.width - 1 &&
            y >= alien.y && y <= alien.y + alien.height - 1) {
            return true;
        }
        return false;}
};

// Class representing the player's spaceship
class Spaceship : public Entity {
private:
    char spaceship[3][4] = {
        "/ \\",
        "|=|",
        "=O="
    };
    int score;

public:
    Spaceship(int startX, int startY)
        : Entity(startX, startY, 4, 3, ' '), score(0) {}

    // Method to draw the spaceship
    void draw() {
        for (int i = 0; i < 3; i++) {
            gotoxy(x, y + i);
            cout << spaceship[i];
        }
    }

    // Method to erase the spaceship
    void erase() {
        for (int i = 0; i < 3; i++) {
            gotoxy(x, y + i);
            cout << "    "; // Clear 4 characters to erase the spaceship
        }
    }

    // Method to move the spaceship left
    void moveLeft() {
        if (x > 2)
            x -= 2;
    }

    // Method to move the spaceship right
    void moveRight() {
        if (x < WIN_WIDTH - 4)
            x += 2;
    }

    // Overload the << operator to display the current score
    friend ostream& operator<<(ostream& os, const Spaceship& spaceship) {
        os << "Score: " << spaceship.score;
        return os;
    }

    // Method to increase the score
    void increaseScore() {
        score++;
    }

    // Method to get the current score
    int getScore() const {
        return score;
    }

    // Method to shoot bullets from both sides of the spaceship
   void shoot(Bullet bullets[], int maxBullets) {
    int bulletsShot = 0;
    for (int i = 0; i < maxBullets && bulletsShot < 2; i++) {
        if (!bullets[i].isActive()) {
            bullets[i].shootLeft(x, y);
            bullets[i + 1].shootRight(x, y);
            bulletsShot += 2;
        }
    }
}

};





// Function to draw the game border
void drawBorder() {
    for (int i = 0; i < SCREEN_WIDTH; i++) {
        gotoxy(i, SCREEN_HEIGHT);
        cout << "-";
    }

    for (int i = 0; i < SCREEN_HEIGHT; i++) {
        gotoxy(0, i);
        cout << "|";
        gotoxy(SCREEN_WIDTH, i);
        cout << " |";
    }

    for (int i = 0; i < SCREEN_HEIGHT; i++) {
        gotoxy(WIN_WIDTH, i);
        cout << "|";
    }
}

// Function to display the game over screen
void gameover() {
    system("cls");
    cout << endl;
    cout << "\t\t--------------------------" << endl;
    cout << "\t\t-------- Game Over -------" << endl;
    cout << "\t\t--------------------------" << endl << endl;
    cout << "\t\tPress any key to go back to menu.";
    getch();
}

// Function to set cursor visibility and size
void setcursor(bool visible, DWORD size) {
    if (size == 0) size = 20;
    CONSOLE_CURSOR_INFO lpCursor;
    lpCursor.bVisible = visible;
    lpCursor.dwSize = size;
    SetConsoleCursorInfo(console, &lpCursor);
}

// Function to update and display the player's score
void updateScore(int score) {
    gotoxy(WIN_WIDTH + 7, 5);
    cout << "Score: " << score << endl;
}

// Function to save the player's highest score to a file
void saveScore(int score) {
    ofstream outFile("score.txt");
    if (outFile.is_open()) {
        outFile << score;
        outFile.close();
    }
}

// Function to load the player's highest score from a file
int loadScore() {
    int score = 0;
    ifstream inFile("score.txt");
    if (inFile.is_open()) {
        inFile >> score;
        inFile.close();
    }
    return score;
}

// Function to display game instructions
void instructions() {
    system("cls");
    cout << "Instructions";
    cout << "\n----------------";
    cout << "\n Press A to move left";
    cout << "\n Press D to move right";
    cout << "\n Press Spacebar to shoot the alien";
    cout << "\n\nPress any key to go back to the menu";
    getch();
}


int main() {
    setcursor(0, 0);
    srand(static_cast<unsigned>(time(nullptr)));

    int highestScore = loadScore(); // Load the highest score

    do {
        system("cls");
        gotoxy(10, 5);
        cout << " -------------------------- ";
        gotoxy(10, 6);
        cout << " |     Space Invaders      | ";
        gotoxy(10, 7);
        cout << " --------------------------";
        gotoxy(10, 9);
        cout << "1. Start Game";
        gotoxy(10, 10);
        cout << "2. Instructions";
        gotoxy(10, 11);
        cout << "3. Quit";
        gotoxy(10, 13);
        cout << "Highest Score: " << highestScore; // Display the highest score
        gotoxy(10, 14);
        cout << "Select option: ";
        char op = getche();

        if (op == '1') {
            system("cls");
            int spaceshipPos = WIN_WIDTH / 2;
            int score = 0;
            Spaceship spaceship(WIN_WIDTH / 2 - 1, 22);
            vector<Bullet> bullets(MAX_BULLETS);
            Alien aliens[3];

            for (int i = 0; i < 3; i++) {
                aliens[i].spawn();
            }

            drawBorder();
            score = 0; // Reset the score to 0

            gotoxy(WIN_WIDTH + 5, 2);
            cout << "Space Invaders";
            gotoxy(WIN_WIDTH + 6, 4);
            cout << "----------";
            gotoxy(WIN_WIDTH + 6, 6);
            cout << "----------";
            gotoxy(WIN_WIDTH + 7, 12);
            cout << "Control ";
            gotoxy(WIN_WIDTH + 7, 13);
            cout << "-------- ";
            gotoxy(WIN_WIDTH + 2, 14);
            cout << " A Key - Move Left";
            gotoxy(WIN_WIDTH + 2, 15);
            cout << " D Key - Move Right";
            gotoxy(WIN_WIDTH + 2, 16);
            cout << " Spacebar = Shoot";

            gotoxy(10, 5);
            cout << "Press any key to start";
            getch();
            gotoxy(10, 5);
            cout << "                      ";

            while (1) {
                if (kbhit()) {
                    char ch = getch();
                    if (ch == 'a' || ch == 'A') {
                        spaceship.erase(); // Erase the previous position
                        spaceship.moveLeft();
                    }
                    if (ch == 'd' || ch == 'D') {
                        spaceship.erase(); // Erase the previous position
                        spaceship.moveRight();
                    }
                    if (ch == ' ') {
                        for (int i = 0; i < MAX_BULLETS; i++) {
                            if (!bullets[i].isActive()) {
                                bullets[i].shootLeft(spaceship.x, spaceship.y);
                                bullets[i + 1].shootRight(spaceship.x, spaceship.y);
                                break; // Shoot only two bullets at a time, one left and one right
                            }
                        }
                    }
                    if (ch == 27) {
                        break;
                    }
                }

                spaceship.draw();

                for (int i = 0; i < 3; i++) {
                    if (aliens[i].isActive()) {
                        aliens[i].erase(); // Erase the previous position of the alien
                        aliens[i].move();
                        aliens[i].drawAlien(); // Draw the respawned alien
                    }
                }

                for (int i = 0; i < MAX_BULLETS; i++) {
                    bullets[i].move();
                }

                for (int i = 0; i < 3; i++) {
                    if (!aliens[i].isActive()) {
                        aliens[i].timer++;
                        if (aliens[i].timer >= ENEMY_DIF) {
                            aliens[i].spawn(); // Respawn the alien
                            aliens[i].drawAlien(); // Draw the respawned alien
                        }
                    }
                }

                for (int i = 0; i < 3; i++) {
                    if (aliens[i].isActive() && aliens[i].y + 3 >= 23 &&
                        aliens[i].x + 4 - spaceship.x >= 0 && aliens[i].x + 4 - spaceship.x < 4) {
                        gameover();
                        if (spaceship.getScore() > highestScore) {
                            highestScore = spaceship.getScore(); // Update the highest score if it's higher
                            saveScore(highestScore); // Save the new highest score
                        }
                        return 0;
                    }
                }

                // Inside the main loop where you draw and update aliens
               // Inside the main loop where you draw and update aliens
for (int i = 0; i < 3; i++) {
    if (aliens[i].isActive()) {
        // Collision detection for smaller spaceship
        int smallerSpaceshipX = aliens[i].x;
        int smallerSpaceshipY = aliens[i].y;

        for (int j = 0; j < MAX_BULLETS; j++) {
            if (bullets[j].hitsAlien(aliens[i])) {
                bullets[j].setActive(false);
                aliens[i].setActive(false);
                aliens[i].erase();
                spaceship.increaseScore(); // Increase the score by 1 per alien hit
                updateScore(spaceship.getScore());
                if (spaceship.getScore() > highestScore) {
                    highestScore = spaceship.getScore();
                    saveScore(highestScore);
                }
                aliens[i].spawn();
            }
        }

        aliens[i].erase();
        aliens[i].move();
        aliens[i].drawAlien();
    }
}
          Sleep(250);
            }
        }
        else if (op == '2') {
            instructions();
        }
        else if (op == '3') {
            break;
        }
    } while (1);

    return 0;
}
