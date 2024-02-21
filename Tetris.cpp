#include <iostream>  
#include <windows.h> 
#include <conio.h>   
#include <ctime>     
#include <fstream>  
#include <iomanip>   
#include <algorithm> 

using namespace std;

/*KEYS*/
#define KB_UpArrow 72
#define KB_DownArrow 80
#define KB_LeftArrow 75
#define KB_RightArrow 77
#define KB_SPACE 32
#define KB_ESC 27
#define KB_r 114
#define KB_h 104

/*CONSOLE TEXT COLOR*/
#define RESET_COLOR "\033[0m"
#define BLACK_COLOR "\033[30m"
#define RED_COLOR "\033[31m"
#define GREEN_COLOR "\033[32m"
#define YELLOW_COLOR "\033[33m"
#define BLUE_COLOR "\033[34m"
#define PURPLE_COLOR "\033[35m"
#define CYAN_COLOR "\033[36m"
#define WHITE_COLOR "\033[37m"

/*CONSOLE TEXT STYLE*/
#define BOLDON_STYLE "\033[1m"
#define BOLDOFF_STYLE "\033[22m"

struct Element
{
    int number;
    bool isMoveable;
};

struct Tetrimino
{
    int size;
    int **block;
    int positionX;
    int positionY;
};

struct Game
{
    string playerName;
    int mode;
    int boardLength;
    int boardWidth;
    int theme;
    int score;
    int level;
    int lineCount;
    bool haveLost = false;
    clock_t startTime = clock();
    clock_t endTime = clock();
    int time = 0;
    Tetrimino upcomingTetriminos[5];
    Element **board;
};

struct Option
{
    int number;
    string title;
};

/*MENU*/
void displayMenu(); // displays the menu

/*GAMEPLAY FLOW*/
// Initialize a new game
void generateNewGame();      // calls "getInfo" and creats a new board and sends data to "playGame"
void setQuickInfo(Game &);   // gets data required for running a saving the game from the player
void getCustomInfo(Game &);  // sets data required for running the game quickly
void displayThemes();        // displays the available themes to the player
void initializeGame(Game &); // initiate new games
void playGame(Game &);       // starts the main game sequence (the control panel of the game)
int setLevel(Game &);        // sets the level of the player
bool checkIfLost(Game);      // check if the game is over
// Shapes
void insertTetrimino(Game);     // inserts the tetrimino into the board
Tetrimino generateTetrimino();  // chooses a random tetrimino from below
Tetrimino generateTetriminoI(); // tetrimino I
Tetrimino generateTetriminoO(); // tetrimino O
Tetrimino generateTetriminoL(); // tetrimino L
Tetrimino generateTetriminoJ(); // tetrimino J
Tetrimino generateTetriminoS(); // tetrimino S
Tetrimino generateTetriminoZ(); // tetrimino Z
Tetrimino generateTetriminoT(); // tetrimino T
void removeTetrimino(Game);     // removes tetrimino from previous location
// Movements
void makeMove(Game &);                           // get a move command from the player
int setDelayTime(Game);                          // sets the delay time of the game
void displayControls(Game);                      // displays the controls under the upcoming box
void displayGameInfo(Game);                      // displays information on the left side of the board
void displayUpcomingTetriminos(Game);            // displays upcoming tetriminoss
void displayBoardTable(Game);                    // prints out the board table
bool checkDown(Game);                            // to check down-move possibility
void moveDown(Game &);                           // move the tetrimino down
bool checkLeft(Game);                            // to check lefr-move possibility
void moveLeft(Game &);                           // move the tetrimino left
bool checkRight(Game);                           // to check right-move possibility
void moveRight(Game &);                          // move the tetrimino right
bool checkRotation(Game);                        // to check rotation possibility
void rotateTetriminoClockwise(Tetrimino);        // rotates the tetrimino clockwise
void rotateTetriminoCounterClockwise(Tetrimino); // rotates the tetrimino counter-clockwise
void holdTetrimino(Game &);                      // holds the current tetrimino for the player
void fixElements(Game);                          // copies game board into game board
void checkIfRowIsFull(Game &);                   // check for popping a full row
void popRow(Game &, int, int);                   // delete full row and count the score and shift upper rows down
// Game ending
void deallocate(Game);      // deallocates game board memory
void deallocate(Tetrimino); // deallocates tetromino memories
void endGame(Game &);       // shows the final score of the player
// Pause menu
void displayPauseMenu(Game); // display pause menu mid-game
void restartGame(Game &);    // restarts the game
void saveGame(Game);         // saves the game in the file

/*GAME LOADING*/
void displayLoadMenu();         // displays the load-game menu
void loadGame();                // gets an input for selecting a file
void initializeLoadedGame(int); // initiates the selected game to load from the file

/*LEADER BOARD*/
void displayLeaderboard();                  // displays data from the game-data.txt file and sorts them
void fetchData(int);                        // reads data from the file
int sortHelper(const Game &, const Game &); // sorts the data from the file

/*HOW TO PLAY*/
void displayHowToPlay(); // show the game rules to the player

/*CREDITS*/
void displayAboutUs(bool); // displays the credits

/*EXIT GAME*/
void exitGame(); // closes the game permanently

/*UI COMPONENTS*/
string applyTheme(int, int);                        // applies the selected theme onto the game board
string chooseColor(int);                            // determines the color of an object
void displayTitle(string, string, int space = 42);  // displays a custom outline for titles in a menu
void displayOptions(Option[], int, int space = 20); // displays a custom outline for options in a menu
void displayInput(string, int);                     // displays a custom outline for inputs in a menu

/*TERMINAL*/
void printCharacterByCharacter(string, int, int, int); // print lines slowly
void setCursor(int x = 0, int y = 0);                  // control terminal cursor
void showConsoleCursor(bool);                          // makes the cursor not show in terminal
int generateRandomNumber(int);                         // generates a random tetrimino number

int main()
{
    srand(static_cast<unsigned int>(time(nullptr)));
    showConsoleCursor(false);
    PlaySound(TEXT("Tetris2.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
    displayMenu();

    return 0;
}

/*MENU*/
void displayMenu()
{
    system("cls");

    string logo = "\n\033[31m\u2588\u2588\u2588\u2588\u2588\u2588\u2588\u2588\033[37m\u2557\033[33m\u2588\u2588\u2588\u2588\u2588\u2588\u2588\033[37m\u2557\033[32m\u2588\u2588\u2588\u2588\u2588\u2588\u2588\u2588\033[37m\u2557\033[36m\u2588\u2588\u2588\u2588\u2588\u2588\033[37m\u2557 \033[34m\u2588\u2588\033[37m\u2557\033[35m\u2588\u2588\u2588\u2588\u2588\u2588\u2588\033[37m\u2557\033[35m®\033[37m\n\u255A\u2550\u2550\033[31m\u2588\u2588\033[37m\u2554\u2550\u2550\u255D\033[33m\u2588\u2588\033[37m\u2554\u2550\u2550\u2550\u2550\u255D\u255A\u2550\u2550\033[32m\u2588\u2588\033[37m\u2554\u2550\u2550\u255D\033[36m\u2588\u2588\033[37m\u2554\u2550\u2550\033[36m\u2588\u2588\033[37m\u2557\033[34m\u2588\u2588\033[37m\u2551\033[35m\u2588\u2588\033[37m\u2554\u2550\u2550\u2550\u2550\u255D\n   \033[31m\u2588\u2588\033[37m\u2551   \033[33m\u2588\u2588\u2588\u2588\u2588\033[37m\u2557     \033[32m\u2588\u2588\033[37m\u2551   \033[36m\u2588\u2588\u2588\u2588\u2588\u2588\033[37m\u2554\u255D\033[34m\u2588\u2588\033[37m\u2551\033[35m\u2588\u2588\u2588\u2588\u2588\u2588\u2588\033[37m\u2557\n   \033[31m\u2588\u2588\033[37m\u2551   \033[33m\u2588\u2588\033[37m\u2554\u2550\u2550\u255D     \033[32m\u2588\u2588\033[37m\u2551   \033[36m\u2588\u2588\033[37m\u2554\u2550\u2550\033[36m\u2588\u2588\033[37m\u2557\033[34m\u2588\u2588\033[37m\u2551\u255A\u2550\u2550\u2550\u2550\033[35m\u2588\u2588\033[37m\u2551\n   \033[31m\u2588\u2588\033[37m\u2551   \033[33m\u2588\u2588\u2588\u2588\u2588\u2588\u2588\033[37m\u2557   \033[32m\u2588\u2588\033[37m\u2551   \033[36m\u2588\u2588\033[37m\u2551  \033[36m\u2588\u2588\033[37m\u2551\033[34m\u2588\u2588\033[37m\u2551\033[35m\u2588\u2588\u2588\u2588\u2588\u2588\u2588\033[37m\u2551\n   \u255A\u2550\u255D   \u255A\u2550\u2550\u2550\u2550\u2550\u2550\u255D   \u255A\u2550\u255D   \u255A\u2550\u255D  \u255A\u2550\u255D\u255A\u2550\u255D\u255A\u2550\u2550\u2550\u2550\u2550\u2550\u255D\n\n";
    cout << logo << WHITE_COLOR;

    displayTitle("Main Menu", GREEN_COLOR);

    Option options[6];
    for (int i = 0; i < 6; i++)
        options[i].number = i + 1;
    options[0].title = "New Game";
    options[1].title = "Load Game";
    options[2].title = "LeaderBoard";
    options[3].title = "How To Play";
    options[4].title = "About Us";
    options[5].title = "Exit";

    displayOptions(options, 6, 17);

    char command = getch();

    switch (command)
    {
    case '1':
        generateNewGame();
        break;
    case '2':
        displayLoadMenu();
        break;
    case '3':
        displayLeaderboard();
        break;
    case '4':
        displayHowToPlay();
        break;
    case '5':
        displayAboutUs(false);
        break;
    case '6':
        exitGame();
        break;
    default:
        cout << RED_COLOR << "Invalid command!" << endl
             << "Please enter one of the commands above ..." << WHITE_COLOR;

        Sleep(500);

        displayMenu();
        break;
    }
}

/*GAMEPLAY FLOW*/
// Initialize a new game
void generateNewGame()
{
    system("cls");

    Game game;

    displayTitle("New Game", YELLOW_COLOR);

    Option options[3];
    for (int i = 0; i < 3; i++)
        options[i].number = i + 1;
    options[0].title = "Quick Game";
    options[1].title = "Custom Game";
    options[2].title = "Return to Menu";

    displayOptions(options, 3);

    char command = getch();

    switch (command)
    {
    case '1':
        setQuickInfo(game);
        break;
    case '2':
        getCustomInfo(game);
        break;
    case '3':
        displayMenu();
        break;
    default:
        cout << RED_COLOR << "\nPlease choose one of the above options" << WHITE_COLOR;
        Sleep(500);
        generateNewGame();
        break;
    }

    // allocate board memory
    game.board = new Element *[game.boardLength + 3];
    for (int i = 0; i < game.boardLength + 3; i++)
        game.board[i] = new Element[game.boardWidth + 6];

    initializeGame(game);

    system("cls");
    playGame(game);
}

void setQuickInfo(Game &game)
{
    system("cls");

    displayTitle("Quick Game", YELLOW_COLOR);

    displayInput("Please enter your name :", 20);

    setCursor(27, 5);
    cin >> game.playerName;
    // Validation
    while (game.playerName.length() > 15)
    {
        cout << RED_COLOR << "\nPlease enter a shorter name" << WHITE_COLOR << endl;
        Sleep(500);
        setCursor(0, 3);
        displayInput("Please enter your name :", 20);
        setCursor(27, 5);
        cin >> game.playerName;
    }

    game.boardWidth = 10;
    game.boardLength = 20;
    game.mode = 1;
    game.theme = 1;
}

void getCustomInfo(Game &game)
{
    system("cls");
    displayTitle("Custom Game", YELLOW_COLOR);
    displayInput("Please enter your name :", 20);
    setCursor(27, 5);
    cin >> game.playerName;
    // Validation
    while (game.playerName.length() > 17)
    {
        cout << RED_COLOR << "\nYour name should be less than 18 characters" << WHITE_COLOR << endl;
        Sleep(500);
        setCursor(0, 3);
        displayInput("Please enter your name :", 20);
        setCursor(27, 5);
        cin >> game.playerName;
    }

    displayInput("Enter your desirable board width :", 6);
    setCursor(37, 8);
    cin >> game.boardWidth;

    // Validation
    while (cin.fail())
    {
        cout << RED_COLOR << "\nPlease enter a number" << WHITE_COLOR << endl;
        Sleep(500);
        cin.clear();
        cin.ignore(256, '\n');
        setCursor(0, 6);
        displayInput("Enter your desirable board width :", 6);
        setCursor(37, 8);
        cin >> game.boardWidth;
    }
    while (game.boardWidth < 6 || game.boardWidth > 74)
    {
        cout << RED_COLOR << "\nPlease enter a size between 6 and 74" << WHITE_COLOR << endl;
        Sleep(500);
        setCursor(0, 6);
        displayInput("Enter your desirable board width :", 6);
        setCursor(37, 8);
        cin >> game.boardWidth;
    }

    displayInput("Enter your desirable board length :", 6);
    setCursor(38, 11);
    cin >> game.boardLength;

    // Validation
    while (cin.fail())
    {
        cout << RED_COLOR << "\nPlease enter a number" << WHITE_COLOR << endl;
        Sleep(500);
        cin.clear();
        cin.ignore(256, '\n');
        setCursor(0, 9);
        displayInput("Enter your desirable board length :", 6);
        setCursor(38, 11);
        cin >> game.boardLength;
    }
    while (game.boardLength < 15 || game.boardLength > 45)
    {
        cout << RED_COLOR << "\nPlease enter a size between 15 and 45" << WHITE_COLOR << endl;
        Sleep(500);
        setCursor(0, 9);
        displayInput("Enter your desirable board length :", 6);
        setCursor(38, 11);
        cin >> game.boardLength;
    }

    system("cls");
    displayTitle("Custom Game", YELLOW_COLOR);
    displayInput("Enter your desirable mode", 2);
    cout << "\n\n";

    Option options[2];
    for (int i = 0; i < 2; i++)
        options[i].number = i + 1;
    options[0].title = "Classic";
    options[1].title = "Extreme";
    displayOptions(options, 2);

    game.mode = getch() - 48;
    // Validation
    while (game.mode < 1 || game.mode > 2)
    {
        system("cls");
        displayTitle("Custom Game", YELLOW_COLOR);
        displayInput("Enter your desirable mode", 2);
        cout << "\n\n";
        displayOptions(options, 2);
        cout << RED_COLOR << "Please enter an available mode" << WHITE_COLOR << endl;
        game.mode = getch() - 48;
    }

    system("cls");
    displayTitle("Custom Game", YELLOW_COLOR);
    displayInput("Enter game theme", 2);
    displayThemes();
    game.theme = getch() - 48;
    // Validation
    while (game.theme < 1 || game.theme > 6)
    {
        system("cls");
        displayTitle("Custom Game", YELLOW_COLOR);
        displayInput("Enter game theme", 2);
        displayThemes();
        cout << RED_COLOR << "Please enter an available theme" << WHITE_COLOR << endl;

        game.theme = getch() - 48;
    }
}

void displayThemes()
{
    cout << "\n\n";
    string static themeNames[6] = {"Original", "Stroke", "Developer", "Classic", "Alphabet", "Fruity"};
    int static themesPreview[8][6] =
        {
            {0, 0, 2, 2, 0, 0},
            {0, 0, 2, 2, 0, 0},
            {4, 4, 4, 3, 3, 3},
            {6, 6, 4, 3, 5, 5},
            {0, 6, 6, 5, 5, 0},
            {0, 1, 1, 1, 1, 0},
            {7, 7, 7, 7, 7, 7},
            {0, 7, 0, 0, 7, 0}};

    for (int k = 1; k < 7; k += 3)
    {
        cout << BOLDON_STYLE
             << " ~ " << k << "." << themeNames[k - 1] << " ~";

        if (k + 1 < 7)
            cout << setw(2) << ""
                 << " ~ " << k + 1 << "." << themeNames[k] << " ~" << BOLDOFF_STYLE;
        if (k + 2 < 7)
            cout << setw(2) << ""
                 << " ~ " << k + 2 << "." << themeNames[k + 1] << " ~" << BOLDOFF_STYLE;

        cout << "\n\n";

        for (int i = 0; i < 8; i++)
        {
            cout << "  ";
            for (int t = k; t < k + 3; t++)
            {
                if (t < 7)
                {
                    for (int j = 0; j < 6; j++)
                    {
                        cout << applyTheme(themesPreview[i][j], t) << WHITE_COLOR;
                    }
                    cout << setw(4) << "";
                }
            }
            cout << endl;
        }
        cout << "\n\n";
    }
}

void initializeGame(Game &game)
{
    game.startTime = clock();

    for (int i = 0; i < game.boardLength + 3; i++)
        for (int j = 0; j < game.boardWidth + 6; j++)
        {
            game.board[i][j].number = 0;
            game.board[i][j].isMoveable = true;
        }

    for (int i = 1; i < 5; i++)
    {
        game.upcomingTetriminos[i] = generateTetrimino();
        game.upcomingTetriminos[i].positionX = 0;
        game.upcomingTetriminos[i].positionY = 3 + (game.boardWidth - game.upcomingTetriminos[i].size) / 2;
    }

    game.time = 0;
    game.level = 1;
    game.score = 0;
    game.lineCount = 0;
}

void playGame(Game &game)
{
    for (int i = 0; i < 3; i++)
        game.upcomingTetriminos[i] = game.upcomingTetriminos[i + 1];
    game.upcomingTetriminos[3] = generateTetrimino();
    game.upcomingTetriminos[3].positionX = 0;
    game.upcomingTetriminos[3].positionY = 3 + (game.boardWidth - game.upcomingTetriminos[3].size) / 2;

    game.level = setLevel(game);
    insertTetrimino(game);
    makeMove(game);
    deallocate(game.upcomingTetriminos[0]);
    fixElements(game);
    checkIfRowIsFull(game);

    if (checkIfLost(game))
    {
        game.haveLost = true;
        Sleep(200);
        endGame(game);
    }
    else
        playGame(game);
}

int setLevel(Game &game)
{
    if (game.mode == 1)
        if (game.score > 10000)
            return 5;
        else if (game.score > 7000)
            return 4;
        else if (game.score > 3000)
            return 3;
        else if (game.score > 1000)
            return 2;
        else
            return 1;
    else if (game.mode == 2)
        if (game.score > 10000)
            return 7;
        else if (game.score > 3000)
            return 6;
        else
            return 5;
    else
        return 1;
}

bool checkIfLost(Game game)
{
    for (int j = 3 + (game.boardWidth - 4) / 2; j < 3 + (game.boardWidth + 4) / 2; j++)
        if (game.board[1][j].number != 0)
            return true;

    return false;
}

// Shapes
void insertTetrimino(Game game)
{
    Tetrimino tetrimino = game.upcomingTetriminos[0];

    int tetriminoRow = 0;
    for (int i = tetrimino.positionX; i < tetrimino.positionX + tetrimino.size; i++)
    {
        int tetriminoCol = 0;
        for (int j = tetrimino.positionY; j < tetrimino.positionY + tetrimino.size; j++)
        {
            if (tetrimino.block[tetriminoRow][tetriminoCol] != 0)
                game.board[i][j].number = tetrimino.block[tetriminoRow][tetriminoCol];

            tetriminoCol++;
        }
        tetriminoRow++;
    }
}

Tetrimino generateTetrimino()
{
    Tetrimino tetrimino;
    int tetriminoNumber = generateRandomNumber(7);

    switch (tetriminoNumber)
    {
    case 1:
        tetrimino = generateTetriminoI();
        break;
    case 2:
        tetrimino = generateTetriminoO();
        break;
    case 3:
        tetrimino = generateTetriminoL();
        break;
    case 4:
        tetrimino = generateTetriminoJ();
        break;
    case 5:
        tetrimino = generateTetriminoS();
        break;
    case 6:
        tetrimino = generateTetriminoZ();
        break;
    case 7:
        tetrimino = generateTetriminoT();
        break;
    }

    for (int i = 1; i < generateRandomNumber(4); i++)
        rotateTetriminoClockwise(tetrimino);

    return tetrimino;
}

Tetrimino generateTetriminoI()
{
    Tetrimino tetrimino;

    tetrimino.size = 4;

    // Allocate block memory
    tetrimino.block = new int *[tetrimino.size];
    for (int i = 0; i < tetrimino.size; i++)
        tetrimino.block[i] = new int[tetrimino.size];

    // Initialize block values
    for (int i = 0; i < tetrimino.size; i++)
        for (int j = 0; j < tetrimino.size; j++)
            tetrimino.block[i][j] = 0;

    for (int j = 0; j < tetrimino.size; j++)
        tetrimino.block[1][j] = 1;

    return tetrimino;
}

Tetrimino generateTetriminoO()
{
    Tetrimino tetrimino;

    tetrimino.size = 2;

    // Allocate block memory
    tetrimino.block = new int *[tetrimino.size];
    for (int i = 0; i < tetrimino.size; i++)
        tetrimino.block[i] = new int[tetrimino.size];

    // Initialize block values
    for (int i = 0; i < tetrimino.size; i++)
        for (int j = 0; j < tetrimino.size; j++)
            tetrimino.block[i][j] = 0;

    for (int i = 0; i < tetrimino.size; i++)
        for (int j = 0; j < tetrimino.size; j++)
            tetrimino.block[i][j] = 2;

    return tetrimino;
}

Tetrimino generateTetriminoL()
{
    Tetrimino tetrimino;

    tetrimino.size = 3;

    // Allocate block memory
    tetrimino.block = new int *[tetrimino.size];
    for (int i = 0; i < tetrimino.size; i++)
        tetrimino.block[i] = new int[tetrimino.size];

    // Initialize block values
    for (int i = 0; i < tetrimino.size; i++)
        for (int j = 0; j < 4; j++)
            tetrimino.block[i][j] = 0;

    for (int j = 0; j < tetrimino.size; j++)
        tetrimino.block[1][j] = 3;
    tetrimino.block[0][2] = 3;

    return tetrimino;
}

Tetrimino generateTetriminoJ()
{
    Tetrimino tetrimino;

    tetrimino.size = 3;

    // Allocate block memory
    tetrimino.block = new int *[tetrimino.size];
    for (int i = 0; i < tetrimino.size; i++)
        tetrimino.block[i] = new int[tetrimino.size];

    // Initialize block values
    for (int i = 0; i < tetrimino.size; i++)
        for (int j = 0; j < tetrimino.size; j++)
            tetrimino.block[i][j] = 0;

    for (int j = 0; j < tetrimino.size; j++)
        tetrimino.block[1][j] = 4;
    tetrimino.block[0][0] = 4;

    return tetrimino;
}

Tetrimino generateTetriminoS()
{
    Tetrimino tetrimino;

    tetrimino.size = 3;

    // Allocate block memory
    tetrimino.block = new int *[tetrimino.size];
    for (int i = 0; i < tetrimino.size; i++)
        tetrimino.block[i] = new int[tetrimino.size];

    // Initialize block values
    for (int i = 0; i < tetrimino.size; i++)
        for (int j = 0; j < tetrimino.size; j++)
            tetrimino.block[i][j] = 0;

    tetrimino.block[1][0] = 5;
    tetrimino.block[1][1] = 5;
    tetrimino.block[0][1] = 5;
    tetrimino.block[0][2] = 5;

    return tetrimino;
}

Tetrimino generateTetriminoZ()
{
    Tetrimino tetrimino;

    tetrimino.size = 3;

    // Allocate block memory
    tetrimino.block = new int *[tetrimino.size];
    for (int i = 0; i < tetrimino.size; i++)
        tetrimino.block[i] = new int[tetrimino.size];

    // Initialize block values
    for (int i = 0; i < tetrimino.size; i++)
        for (int j = 0; j < tetrimino.size; j++)
            tetrimino.block[i][j] = 0;

    tetrimino.block[1][2] = 6;
    tetrimino.block[1][1] = 6;
    tetrimino.block[0][1] = 6;
    tetrimino.block[0][0] = 6;

    return tetrimino;
}

Tetrimino generateTetriminoT()
{
    Tetrimino tetrimino;

    tetrimino.size = 3;

    // Allocate block memory
    tetrimino.block = new int *[tetrimino.size];
    for (int i = 0; i < tetrimino.size; i++)
        tetrimino.block[i] = new int[tetrimino.size];

    // Initialize block values
    for (int i = 0; i < tetrimino.size; i++)
        for (int j = 0; j < 4; j++)
            tetrimino.block[i][j] = 0;

    for (int j = 0; j < tetrimino.size; j++)
        tetrimino.block[2][j] = 7;
    tetrimino.block[1][1] = 7;

    return tetrimino;
}

void removeTetrimino(Game game)
{
    Tetrimino tetrimino = game.upcomingTetriminos[0];
    for (int i = tetrimino.positionX; i < tetrimino.positionX + tetrimino.size; i++)
    {
        for (int j = tetrimino.positionY; j < tetrimino.positionY + tetrimino.size; j++)
        {
            if (game.board[i][j].isMoveable == true && game.board[i][j].number != 0)
                game.board[i][j].number = 0;
        }
    }
}

// Movements
void makeMove(Game &game)
{
    int delayTime = setDelayTime(game);
    bool canHold = true;
    int command = 0;
    int commandCounter = 0;
    bool canMove = true;

    while (canMove)
    {
        canMove = checkDown(game);

        if (game.boardLength >= 20)
            displayControls(game);
        displayGameInfo(game);
        displayUpcomingTetriminos(game);
        displayBoardTable(game);

        Sleep(delayTime);
        if (kbhit())
        {
            command = getch();
            if (command == 'R' || command == 'H')
                command += 32;
            FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
            if (command == 224)
                command = getch();
        }
        else
        {
            moveDown(game);
            continue;
        }

        if (command == KB_RightArrow || command == KB_LeftArrow || command == KB_UpArrow || command == KB_r)
            commandCounter++;
        else
            commandCounter = 0;

        if (commandCounter == 4)
        {
            moveDown(game);
            commandCounter = 0;
        }

        switch (command)
        {
        case KB_SPACE:
            while (checkDown(game))
                moveDown(game);
            game.score += 2 * game.upcomingTetriminos[0].size;
            break;
        case KB_LeftArrow:
            moveLeft(game);
            break;
        case KB_RightArrow:
            moveRight(game);
            break;
        case KB_UpArrow:
            if (game.mode == 1 && checkRotation(game))
                rotateTetriminoClockwise(game.upcomingTetriminos[0]);
            break;
        case KB_r:
            if (game.mode == 1 && checkRotation(game))
                rotateTetriminoCounterClockwise(game.upcomingTetriminos[0]);
            break;
        case KB_DownArrow:
            for (int i = 0; i < 3; i++)
                moveDown(game);
            break;
        case KB_h:
            if (canHold)
            {
                holdTetrimino(game);
                canHold = false;
            }
            break;
        case KB_ESC:
            displayPauseMenu(game);
            break;
        default:
            cout << RED_COLOR << "\nInvalid move!" << WHITE_COLOR;
            Sleep(500);
            system("cls");
            makeMove(game);
            break;
        }
    }

    game.score += game.upcomingTetriminos[0].size;
}

int setDelayTime(Game game)
{
    switch (game.level)
    {
    case 1:
        return 500;
        break;
    case 2:
        return 450;
        break;
    case 3:
        return 350;
        break;
    case 4:
        return 250;
        break;
    case 5:
        return 200;
        break;
    case 6:
        return 150;
        break;
    case 7:
        return 100;
        break;
    default:
        return 200;
        break;
    }
}

void displayControls(Game game)
{
    int cursorY = game.boardLength, cursorX = 2 * game.boardWidth + 10 + 20;
    setCursor(cursorX, cursorY);
    cout << " \033[32m⟬ESC ⟭\033[33m⏸\033[37m  \033[32m⟬SPACE ⟭\033[33m⇣\033[37m";
    cursorY--;
    setCursor(cursorX, cursorY);
    cout << "\033[32m⟬⏴ ⟭\033[33m←   \033[32m⟬⏷ ⟭\033[33m↓   \033[32m⟬⏵ ⟭\033[33m→";
    cursorY--;
    setCursor(cursorX, cursorY);
    if (game.mode == 1)
        cout << "\033[32m⟬R ⟭\033[33m↺   \033[32m⟬⏶ ⟭\033[33m↻   \033[32m⟬H ⟭\033[33m☞";
    else
        cout << "        \033[32m⟬H ⟭\033[33m☞\033[37m";
}

void displayGameInfo(Game game)
{
    int cursorY = 0;

    setCursor(4, cursorY);
    cout << WHITE_COLOR << "\u2554\u2550\u2550\u2561\033[1m Hold \033[22m\u255E\u2550\u2550\u2557" << endl;
    cursorY++;
    setCursor(4, cursorY);
    cout << "\u2551            \u2551";
    cursorY++;

    for (int i = 0; i < 4; i++)
    {
        setCursor(4, cursorY);
        cout << "\u2551  ";
        for (int j = 0; j < 4; j++)
        {
            if (i >= game.upcomingTetriminos[4].size || j >= game.upcomingTetriminos[4].size)
                cout << "  ";
            else
                cout << applyTheme(game.upcomingTetriminos[4].block[i][j], game.theme) << WHITE_COLOR;
        }

        setCursor(15, cursorY);
        cout << "  \u2551";
        cursorY++;
    }
    setCursor(4, cursorY);

    cout << "\u255A";
    for (int t = 0; t < 12; t++)
        cout << "\u2550";
    cout << "\u255D";

    cursorY = game.boardLength - 1;

    if (game.boardLength >= 20)
    {
        setCursor(0, cursorY);
        displayTitle("Lines :  " + to_string(game.lineCount), RED_COLOR, 20);
        cursorY -= 3;
    }

    if (game.boardLength >= 17)
    {

        int seconds = static_cast<int>(clock() - game.startTime) / CLOCKS_PER_SEC + game.time;
        setCursor(0, cursorY);
        displayTitle("Time :  " + to_string(seconds / 60) + " : " + to_string(seconds % 60), YELLOW_COLOR, 20);
        cursorY -= 3;
    }

    setCursor(0, cursorY);
    displayTitle("Score :  " + to_string(game.score), GREEN_COLOR, 20);
    cursorY -= 3;

    setCursor(0, cursorY);
    displayTitle("Level :  " + to_string(game.level), BLUE_COLOR, 20);
    cursorY -= 3;

    setCursor(0, cursorY);
    displayTitle(game.playerName, PURPLE_COLOR, 20);
    cursorY -= 3;
}

void displayUpcomingTetriminos(Game game)
{
    int cursorY = 0, cursorX = 2 * game.boardWidth + 10 + 20 + 4;

    setCursor(cursorX, cursorY);
    cout << "\u2554\u2561\033[1m Upcoming \033[22m\u255E\u2557";
    cursorY++;
    setCursor(cursorX, cursorY);
    cout << "\u2551            \u2551";
    cursorY++;

    for (int k = 1; k < 4; k++)
    {
        for (int i = 0; i < 4; i++)
        {
            setCursor(cursorX, cursorY);
            cout << "\u2551  ";
            for (int j = 0; j < 4; j++)
            {
                if (i >= game.upcomingTetriminos[k].size || j >= game.upcomingTetriminos[k].size)
                    cout << applyTheme(0, game.theme);
                else
                    cout << applyTheme(game.upcomingTetriminos[k].block[i][j], game.theme) << WHITE_COLOR;
            }
            setCursor(2 * game.boardWidth + 45, cursorY);
            cout << "  \u2551";
            cursorY++;
        }
        if (k == 3)
            break;
        setCursor(cursorX, cursorY);
        cout << "\u2560";
        for (int t = 0; t < 12; t++)
            cout << "\u2550";
        cout << "\u2563";
        cursorY++;
    }

    setCursor(cursorX, cursorY);
    cout << "\u255A";
    for (int t = 0; t < 12; t++)
        cout << "\u2550";
    cout << "\u255D";
}

void displayBoardTable(Game game)
{
    setCursor(25, 0);
    for (int i = 0; i < (game.boardLength + 2); i++)
    {
        for (int j = 3; j < (game.boardWidth + 5); j++)
        {
            // First Line
            if (i == 0)
            {
                if (j == 3)
                {
                    cout << "\u2554";
                    continue;
                }
                else if (j == (game.boardWidth + 5) - 1)
                {
                    cout << "\u2557";
                    continue;
                }

                cout << "\u2550\u2550";
                continue;
            }

            // Last Line
            if (i == (game.boardLength + 2) - 1)
            {
                if (j == 3)
                {
                    cout << "\u255A";
                    continue;
                }
                else if (j == (game.boardWidth + 5) - 1)
                {
                    cout << "\u255D";
                    continue;
                }

                cout << "\u2550\u2550";
                continue;
            }

            // Mid Lines
            else
            {
                if (j == 3)
                    cout << "\u2551";
                else if (j == (game.boardWidth + 5) - 1)
                    cout << "\u2551";
                else
                {
                    int block = game.board[i - 1][j - 1].number;
                    cout << WHITE_COLOR << applyTheme(block, game.theme) << WHITE_COLOR;
                }
            }
        }
        cout << endl;
        setCursor(25, i + 1);
    }
}

bool checkDown(Game game)
{
    Tetrimino tetrimino = game.upcomingTetriminos[0];
    for (int i = tetrimino.positionX; i < tetrimino.positionX + tetrimino.size; i++)
    {
        for (int j = tetrimino.positionY; j < tetrimino.positionY + tetrimino.size; j++)
        {
            if (game.board[i][j].number != 0 && game.board[i][j].isMoveable == true)
            {
                if (i + 1 == game.boardLength || game.board[i + 1][j].number != 0 && game.board[i + 1][j].isMoveable == false)
                    return false;
            }
        }
    }
    return true;
}

void moveDown(Game &game)
{
    if (checkDown(game))
    {
        removeTetrimino(game);
        game.upcomingTetriminos[0].positionX++;
        insertTetrimino(game);
    }
}

bool checkLeft(Game game)
{
    Tetrimino tetrimino = game.upcomingTetriminos[0];

    for (int i = tetrimino.positionX; i < tetrimino.positionX + tetrimino.size; i++)
    {
        for (int j = tetrimino.positionY; j < tetrimino.positionY + tetrimino.size; j++)
        {
            if (game.board[i][j].number != 0 && game.board[i][j].isMoveable == true)
            {
                if (j == 3 || (game.board[i][j - 1].number != 0 && game.board[i][j - 1].isMoveable == false))
                    return false;
            }
        }
    }
    return true;
}

void moveLeft(Game &game)
{
    if (checkLeft(game))
    {
        removeTetrimino(game);
        game.upcomingTetriminos[0].positionY--;
        insertTetrimino(game);
    }
}

bool checkRight(Game game)
{
    Tetrimino tetrimino = game.upcomingTetriminos[0];

    for (int i = tetrimino.positionX; i < tetrimino.positionX + tetrimino.size; i++)
    {
        for (int j = tetrimino.positionY; j < tetrimino.positionY + tetrimino.size; j++)
        {
            if (game.board[i][j].number != 0 && game.board[i][j].isMoveable == true)
            {
                if (j + 1 == game.boardWidth + 3 || game.board[i][j + 1].number != 0 && game.board[i][j + 1].isMoveable == false)
                    return false;
            }
        }
    }
    return true;
}

void moveRight(Game &game)
{
    if (checkRight(game))
    {
        removeTetrimino(game);
        game.upcomingTetriminos[0].positionY++;
        insertTetrimino(game);
    }
}

bool checkRotation(Game game)
{
    Tetrimino tetrimino = game.upcomingTetriminos[0];
    if (tetrimino.positionY >= 3 && tetrimino.positionY + tetrimino.size < game.boardWidth + 3 && tetrimino.positionX + tetrimino.size < game.boardLength)
    {
        for (int i = tetrimino.positionX; i < tetrimino.positionX + tetrimino.size; i++)
        {
            for (int j = tetrimino.positionY; j < tetrimino.positionY + tetrimino.size; j++)
            {
                if ((game.board[i][j].number != 0 && game.board[i][j].isMoveable == false) || (game.board[i + 1][j].number != 0 && game.board[i + 1][j].isMoveable == false))
                {
                    return false;
                }
            }
        }
        return true;
    }
    return false;
}

void rotateTetriminoClockwise(Tetrimino tetrimino)
{

    for (int i = 0; i < tetrimino.size; i++)
    {
        for (int j = i + 1; j < tetrimino.size; j++)
        {
            swap(tetrimino.block[i][j], tetrimino.block[j][i]);
        }
    }

    for (int i = 0; i < tetrimino.size; i++)
    {
        for (int j = 0; j < tetrimino.size / 2; j++)
        {
            swap(tetrimino.block[i][j], tetrimino.block[i][tetrimino.size - 1 - j]);
        }
    }
}

void rotateTetriminoCounterClockwise(Tetrimino tetrimino)
{

    for (int i = 0; i < tetrimino.size; i++)
    {
        for (int j = i + 1; j < tetrimino.size; j++)
        {
            swap(tetrimino.block[i][j], tetrimino.block[j][i]);
        }
    }

    for (int j = 0; j < tetrimino.size; j++)
    {
        for (int i = 0; i < tetrimino.size / 2; i++)
        {
            swap(tetrimino.block[i][j], tetrimino.block[tetrimino.size - 1 - i][j]);
        }
    }
}

void holdTetrimino(Game &game)
{
    removeTetrimino(game);
    swap(game.upcomingTetriminos[0], game.upcomingTetriminos[4]);
    game.upcomingTetriminos[4].positionX = 0;
    game.upcomingTetriminos[4].positionY = 3 + (game.boardWidth - game.upcomingTetriminos[4].size) / 2;
}

void fixElements(Game game)
{
    for (int i = 0; i < game.boardLength; i++)
    {
        for (int j = 3; j < game.boardWidth + 3; j++)
        {
            if (game.board[i][j].number != 0 && game.board[i][j].isMoveable != false)
                game.board[i][j].isMoveable = false;
        }
    }
}

void checkIfRowIsFull(Game &game)
{
    int static sequentialFullRows = 0;
    for (int i = game.boardLength - 1; i >= 0; i--)
    {
        bool isFull = true;
        for (int j = 3; j < game.boardWidth + 3; j++)
        {
            if (game.board[i][j].number == 0)
            {
                isFull = false;
                break;
            }
        }

        if (isFull)
        {
            sequentialFullRows++;
            popRow(game, i, sequentialFullRows);
            checkIfRowIsFull(game);
        }
        else
            sequentialFullRows = 0;
    }
}

void popRow(Game &game, int fullRow, int sequentialFullRows)
{
    for (int t = 0; t < 5; t++)
    {
        for (int j = 3; j < game.boardWidth + 3; j++)
        {
            if (t != 4)
                if ((t + j) % 3 == 0)
                    game.board[fullRow][j].number = 2;
                else
                    game.board[fullRow][j].number = 6;
            else
            {
                game.board[fullRow][j].number = 0;
                game.board[fullRow][j].isMoveable = true;
            }
        }
        displayBoardTable(game);
        Sleep(5);
    }

    for (int i = fullRow; i > 0; i--)
        for (int j = 3; j < game.boardWidth + 3; j++)
            swap(game.board[i][j], game.board[i - 1][j]);

    game.score += game.boardWidth * 10 * sequentialFullRows * game.level;
    game.lineCount++;
}

// Game ending
void deallocate(Game game)
{
    for (int i = 0; i < game.boardLength + 3; i++)
    {
        delete[] game.board[i];
    }
    delete[] game.board;
}

void deallocate(Tetrimino tetrimino)
{
    for (int i = 0; i < tetrimino.size; i++)
        delete[] tetrimino.block[i];

    delete[] tetrimino.block;
}

void endGame(Game &game)
{
    game.endTime = clock();
    game.time += static_cast<int>(game.endTime - game.startTime) / CLOCKS_PER_SEC;

    saveGame(game);
    system("cls");

    displayTitle("Game Over", RED_COLOR);
    displayTitle("Your final SCORE: " + to_string(game.score), YELLOW_COLOR);
    displayTitle("Time: " + to_string(game.time), YELLOW_COLOR);

    Option options[2];
    for (int i = 0; i < 2; i++)
        options[i].number = i + 1;
    options[0].title = "Restart";
    options[1].title = "Return to Menu";

    displayOptions(options, 2);

    char command = getch();

    switch (command)
    {
    case '1':
        restartGame(game);
        break;
    case '2':
        deallocate(game);
        for (int i = 1; i < 5; i++)
            deallocate(game.upcomingTetriminos[i]);
        displayMenu();
        break;
    default:
        cout << RED_COLOR << "\nPlease choose one of the above options" << WHITE_COLOR;
        Sleep(500);
        endGame(game);
        break;
    }
}

// Pause menu
void displayPauseMenu(Game game)
{
    system("cls");

    displayTitle("Game Paused", PURPLE_COLOR);

    Option options[4];
    for (int i = 0; i < 5; i++)
        options[i].number = i + 1;
    options[0].title = "Resume";
    options[1].title = "Restart";
    options[2].title = "Save";
    options[3].title = "Quit";

    displayOptions(options, 4);

    char command = getch();

    switch (command)
    {
    case '1':
        system("cls");
        break;
    case '2':
        restartGame(game);
        break;
    case '3':
        saveGame(game);
        system("cls");
        break;
    case '4':
        deallocate(game);
        displayMenu();
        break;

    default:
        cout << RED_COLOR << "Invalid command!" << endl
             << "Please enter one of the commands above ..." << WHITE_COLOR;
        Sleep(1000);
        displayPauseMenu(game);
        break;
    }
}

void restartGame(Game &game)
{
    system("cls");
    initializeGame(game);
    game.startTime = clock();
    playGame(game);
}

void saveGame(Game game)
{
    game.endTime = clock();
    game.time += static_cast<int>(game.endTime - game.startTime) / CLOCKS_PER_SEC;

    ofstream file("GameData.txt", ios::app);
    if (file.is_open())
    {
        file << game.playerName << " - " << game.haveLost << " - " << game.score << " - " << game.lineCount << " - " << game.time << " - " << game.mode << " - " << game.boardLength << " - " << game.boardWidth << " - " << game.theme << " - ";
        for (int i = 0; i < game.boardLength + 3; i++)
        {
            for (int j = 0; j < game.boardWidth + 6; j++)
            {
                if (game.board[i][j].isMoveable == false || game.board[i][j].number == 0)
                    file << game.board[i][j].number;
                else
                    file << "0";
            }
        }
        file << "\n";
        file.close();
    }
    else
    {
        cout << endl
             << "Unable to open the file." << endl;
    }
}

/*GAME LOADING*/
void displayLoadMenu()
{
    system("cls");
    displayTitle("Load Game", GREEN_COLOR, 140);
    loadGame();
}

void loadGame()
{
    string line, word;
    ifstream file("GameData.txt");
    int l = 0;
    if (file.is_open())
    {
        while (getline(file, line))
            l++;

        file.clear();
        file.seekg(0);

        Game gamesData[l];

        int i = 0;
        while (getline(file, line))
        {
            int j = 0;
            if (i == 0)
            {
                file.clear();
                file.seekg(0);
            }

            while (file >> word)
            {
                if (word == "-")
                    continue;

                switch (j)
                {
                case 0:
                    gamesData[i].playerName = word;
                    break;
                case 1:
                    gamesData[i].haveLost = (stoi(word) == 1) ? true : false;
                    break;
                case 2:
                    gamesData[i].score = stoi(word);
                    break;
                case 3:
                    gamesData[i].lineCount = stoi(word);
                    break;
                case 4:
                    gamesData[i].time = stoi(word);
                    break;
                case 5:
                    gamesData[i].mode = stoi(word);
                    break;
                case 6:
                    gamesData[i].boardLength = stoi(word);
                    break;
                case 7:
                    gamesData[i].boardWidth = stoi(word);
                    break;
                }
                if (j == 7)
                    break;

                j++;
            }
            i++;
        }

        cout << PURPLE_COLOR
             << setw(8) << left << "Number" << setw(8)
             << "  |  "
             << setw(8) << left << "Name" << setw(8)
             << "  |  "
             << setw(8) << left << "Score" << setw(8)
             << "  |  "
             << setw(8) << left << "Time" << setw(8)
             << "  |  "
             << setw(8) << left << "Lines" << setw(8)
             << "  |  "
             << setw(8) << left << "Mode" << setw(8)
             << "  |  "
             << setw(8) << left << "Board Size (L x W)" << setw(8)
             << "  |  "
             << setw(8) << left << "Resume Capability" << setw(8) << WHITE_COLOR << endl;

        for (int i = 0; i < l; i++)
        {
            string resumeCapability = (gamesData[i].haveLost)
                                          ? "\033[31m✘"
                                          : "\033[32m✔",
                   gameDimensions = to_string(gamesData[i].boardLength) + " x " + to_string(gamesData[i].boardWidth);

            string modeTitle = (gamesData[i].mode == 1) ? "Classic" : "Extreme";

            cout << CYAN_COLOR
                 << setw(8) << left << i + 1 << setw(8) << "  |  "
                 << setw(8) << left << gamesData[i].playerName << setw(8) << "  |  "
                 << setw(8) << left << gamesData[i].score << setw(8) << "  |  "
                 << setw(8) << left << gamesData[i].time << setw(8) << "  |  "
                 << setw(8) << left << gamesData[i].lineCount << setw(8) << "  |  "
                 << setw(8) << left << modeTitle << setw(8) << "  |  "
                 << setw(18) << left << gameDimensions << setw(8) << "  |  "
                 << setw(8) << left << resumeCapability << setw(8) << WHITE_COLOR << endl;
        }

        int recordId;
        displayInput("Select the game you want to load :", 6);
        setCursor(0, l + 8);
        cout << BLUE_COLOR << "Enter 0 to return to menu" << WHITE_COLOR;
        setCursor(37, l + 6);
        cin >> recordId;

        // Validation
        while (cin.fail())
        {
            setCursor(0, l + 8);
            cout << RED_COLOR << "\nPlease enter a number" << WHITE_COLOR << "\n\n";
            Sleep(500);
            cin.clear();
            cin.ignore(256, '\n');
            setCursor(0, l + 4);
            displayInput("Select the game you want to load :", 6);
            setCursor(37, l + 6);
            cin >> recordId;
        }
        while (recordId < 0 || recordId > l)
        {
            setCursor(0, l + 8);
            cout << RED_COLOR << "\nPlease enter an available recordId" << WHITE_COLOR << "\n\n";
            Sleep(500);

            cin.clear();
            cin.ignore(256, '\n');
            setCursor(0, l + 4);
            displayInput("Select the game you want to load :", 6);
            setCursor(3, l + 6);
            cin >> recordId;
        }

        if (recordId == 0)
            displayMenu();

        if (!gamesData[recordId - 1].haveLost)
            initializeLoadedGame(recordId);
        else
        {
            setCursor(0, l + 8);
            cout << RED_COLOR << "\nSelected game can't be resumed" << endl
                 << "Please enter another game to load" << WHITE_COLOR;
            Sleep(500);
            displayLoadMenu();
        }

        file.close();
    }
    else
    {
        cout << endl
             << RED_COLOR << "Unable to open the file." << WHITE_COLOR << endl;
        cout << "Press any key to return to the main menu";
        getch();
        displayMenu();
    }
}

void initializeLoadedGame(int row)
{
    Game game;

    string line, word;
    ifstream file("GameData.txt");
    if (file.is_open())
    {
        while (getline(file, line))
        {
            file.clear();
            file.seekg(0);
            for (int i = 1; i < row; i++)
            {
                string line;
                if (!getline(file, line))
                {
                    cout << "Error: File does not have enough lines." << std::endl;
                }
            }

            int j = 0;
            while (file >> word)
            {
                if (word == "-")
                    continue;

                switch (j)
                {
                case 0:
                    game.playerName = word;
                    break;
                case 1:
                    game.haveLost = stoi(word);
                    break;
                case 2:
                    game.score = stoi(word);
                    break;
                case 3:
                    game.lineCount = stoi(word);
                    break;
                case 4:
                    game.time = stoi(word);
                    break;
                case 5:
                    game.mode = stoi(word);
                    break;
                case 6:
                    game.boardLength = stoi(word);
                    break;
                case 7:
                    game.boardWidth = stoi(word);
                    break;
                case 8:
                    game.theme = stoi(word);
                    break;
                }
                if (j == 8)
                    break;

                j++;
            }
            break;
        }

        // allocate board memory
        game.board = new Element *[game.boardLength + 3];
        for (int i = 0; i < game.boardLength + 3; i++)
            game.board[i] = new Element[game.boardWidth + 6];

        char character;

        int i = 0, j = 0;
        while (file.get(character))
        {

            if (character == '-' || character == ' ')
                continue;

            int element = character - '0';
            game.board[i][j].number = element;
            j++;
            if (j >= game.boardWidth + 6)
            {
                j = 0;
                i++;
                if (i >= game.boardLength + 3)
                    break;
            }
        }
        file.close();
    }
    else
    {
        cout << endl
             << RED_COLOR << "Unable to open the file." << WHITE_COLOR << endl;
    }

    for (int i = 0; i < game.boardLength + 3; i++)
    {
        for (int j = 0; j < game.boardWidth + 6; j++)
        {
            if (game.board[i][j].number != 0)
                game.board[i][j].isMoveable = false;
            else
                game.board[i][j].isMoveable = true;
        }
    }

    for (int i = 1; i < 5; i++)
    {
        game.upcomingTetriminos[i] = generateTetrimino();
        game.upcomingTetriminos[i].positionX = 0;
        game.upcomingTetriminos[i].positionY = 3 + (game.boardWidth - game.upcomingTetriminos[i].size) / 2;
    }

    game.startTime = clock();
    system("cls");
    playGame(game);
}

/*LEADER BOARD*/
void displayLeaderboard()
{
    system("cls");

    displayTitle("Choose LeaderBoard Mode", YELLOW_COLOR);

    Option options[3];
    for (int i = 0; i < 3; i++)
        options[i].number = i + 1;
    options[0].title = "Classic";
    options[1].title = "Extreme";
    options[2].title = "Return to Menu";

    displayOptions(options, 3);

    int command = getch() - 48;

    switch (command)
    {
    case 1:
        system("cls");
        displayTitle("LeaderBoard (Classic)", GREEN_COLOR, 140);
        fetchData(command);
        break;
    case 2:
        system("cls");
        displayTitle("LeaderBoard (Extreme)", GREEN_COLOR, 140);
        fetchData(command);
        break;
    case 3:
        displayMenu();
        break;
    default:
        cout << RED_COLOR << "\nPlease choose one of the above options" << WHITE_COLOR;
        Sleep(500);
        generateNewGame();
        break;
    }

    cout << "\n\nPress any key to leave to leader board menu";
    getch();
    displayLeaderboard();
}

void fetchData(int leaderBoardMode)
{
    string line, word;
    ifstream file("GameData.txt");
    int l = 0;
    if (file.is_open())
    {
        while (getline(file, line))
            l++;

        file.clear();
        file.seekg(0);

        Game gamesData[l];

        int i = 0;
        while (getline(file, line))
        {
            int j = 0;
            if (i == 0)
            {
                file.clear();
                file.seekg(0);
            }

            while (file >> word)
            {
                if (word == "-")
                    continue;

                switch (j)
                {
                case 0:
                    gamesData[i].playerName = word;
                    break;
                case 1:
                    gamesData[i].haveLost = stoi(word);
                    break;
                case 2:
                    gamesData[i].score = stoi(word);
                    break;
                case 3:
                    gamesData[i].lineCount = stoi(word);
                    break;
                case 4:
                    gamesData[i].time = stoi(word);
                    break;
                case 5:
                    gamesData[i].mode = stoi(word);
                    break;
                case 6:
                    gamesData[i].boardLength = stoi(word);
                    break;
                case 7:
                    gamesData[i].boardWidth = stoi(word);
                    break;
                }
                if (j == 7)
                    break;

                j++;
            }

            i++;
        }

        Game *gamesDataLength = gamesData + sizeof(gamesData) / sizeof(gamesData[0]);
        sort(gamesData, gamesDataLength, sortHelper);

        cout << PURPLE_COLOR
             << setw(8) << left << "Rank" << setw(8)
             << "  |  "
             << setw(8) << left << "Name" << setw(8)
             << "  |  "
             << setw(8) << left << "Score" << setw(8)
             << "  |  "
             << setw(8) << left << "Time" << setw(8)
             << "  |  "
             << setw(8) << left << "Lines" << setw(8)
             << "  |  "
             << setw(8) << left << "Board Size (L x W)" << setw(8)
             << "  |  "
             << setw(8) << left << "Resume Capability" << setw(8) << WHITE_COLOR << endl;

        for (int i = 0; i < l; i++)
        {
            string resumeCapability = (gamesData[i].haveLost)
                                          ? "\033[31m✘"
                                          : "\033[32m✔",
                   gameDimensions = to_string(gamesData[i].boardLength) + " x " + to_string(gamesData[i].boardWidth);

            if (gamesData[i].mode != leaderBoardMode)
                continue;

            cout << BLUE_COLOR
                 << setw(8) << left << i + 1 << setw(8) << "  |  "
                 << setw(8) << left << gamesData[i].playerName << setw(8) << "  |  "
                 << setw(8) << left << gamesData[i].score << setw(8) << "  |  "
                 << setw(8) << left << gamesData[i].time << setw(8) << "  |  "
                 << setw(8) << left << gamesData[i].lineCount << setw(8) << "  |  "
                 << setw(18) << left << gameDimensions << setw(8) << "  |  "
                 << setw(8) << left << resumeCapability << setw(8) << WHITE_COLOR << endl;
        }

        file.close();
    }
    else
    {
        cout << endl
             << RED_COLOR << "No Data available" << WHITE_COLOR << endl;
        cout << "Press any key to return to the main menu";
        getch();
        displayMenu();
    }
}

int sortHelper(const Game &game1, const Game &game2)
{
    int result = 0;
    if (game2.score < game1.score)
        result = 1;
    return result;
}

/*HOW TO PLAY*/
void displayHowToPlay()
{
    PlaySound(TEXT("Tetris.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
    system("cls");
    displayTitle("How To Play", GREEN_COLOR, 74);

    string explanation = "The \033[31mTetris®️\033[37m game requires players to strategically \033[32mrotate\033[37m, \033[32mmove\033[37m, and \033[32mdrop\033[37m \na procession of \033[35mTetriminos\033[37m that fall into the rectangular Matrix at \nincreasing speeds. Players attempt to clear as many \033[36mlines\033[37m as possible by \ncompleting horizontal rows of blocks without empty space, but if the \n\033[35mTetriminos\033[37m surpass the \033[34mSkyline\033[37m the game is over! It might sound simple, \nbut strategy and speed can go a long way! \nAre YOU up for the challenge?\n";
    string controls = "\n🖈Controls:``\n  `Move Left \033[32m⟬⏴ ⟭\033[37m\n  `Move Right \033[32m⟬⏵ ⟭\033[37m\n  `Clockwise Rotation \033[32m⟬⏶ ⟭\033[37m\n  `Counterclockwise Rotation \033[32m⟬R ⟭\033[37m\n  `Move Down Slowly \033[32m⟬⏷ ⟭\033[37m\n  `Drop Down \033[32m⟬SPACE ⟭\033[37m\n  `Hold \033[32m⟬H ⟭\033[37m  `Pause Game \033[32m⟬Esc ⟭\033[37m\n";
    string scoring = "\n🖈\033[33mScore\033[37m and \033[36mLevel\033[37m:\n  Each \033[35mTetrimino\033[37m that drops down in the board increases the \033[33mscore\033[37m by the \n  length of the \033[35mTetrimino\033[37m's larger dimension. Whenever you fill a row\n  with \033[35mTetriminos\033[37m your \033[33mscore\033[37m will increase by 10 times of the board width\n  and if you fill multiple rows sequentially, your \033[33mscore\033[37m increment will\n  multipy by  the number of those rows and \033[36mlevel\033[37m.\n  \033[36mLevel\033[37m will increase based on your \033[33mscore\033[37m like this:\n     \033[36mlevel\033[37m 1 :  \033[33mscore\033[37m < 1000\n     \033[36mlevel\033[37m 2 :  1000 <= \033[33mscore\033[37m < 3000\n     \033[36mlevel\033[37m 3 :  3000 <= \033[33mscore\033[37m < 7000\n     \033[36mlevel\033[37m 4 :  7000 <= \033[33mscore\033[37m < 10000\n     \033[36mlevel\033[37m 5 :  10000 <= \033[33mscore\033[37m\n`  The \033[33mscore\033[37m for filling a row is calculated as follows:\n     board width * 10 * sequential full rows * \033[36mlevel\033[37m\n";
    string modes = "\n🖈Game Modes:``\n  `1.Classic:\n    Literally the game Alexey Pajitnov created in the midst of\n    the soviet union. There are no control limits and you can enjoy the\n    original \033[31mTetris®️\033[37m gameplay!\n  `2.Extreme:\n    This is a mode for champions. The starting speed is higher and\n    you don't have the ability to rotate the \033[35mTetriminos\033[37m. Be careful\n    your \033[36mlevel\033[37m starts from 5 in this mode!\n    If you have mastered the classic mode, this is the way to go!\n";
    string quote = "\n\n~ 🗣️\033[1m\033[36m🙶only legends play tetris in terminal🙷\033[37m -Alexey Pajitnov (\033[35m Father\033[37m of \033[31mTetris®️\033[37m)\033[22m\n\n\n\n";
    printCharacterByCharacter(explanation, 50, 200, 500);
    printCharacterByCharacter(controls, 50, 200, 500);
    printCharacterByCharacter(scoring, 50, 200, 500);
    printCharacterByCharacter(modes, 50, 200, 500);
    printCharacterByCharacter(quote, 50, 200, 500);

    Sleep(500);
    cout << "Press any key to return to menu";
    getch();
    PlaySound(TEXT("Tetris2.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
    displayMenu();
}

/*CREDITS*/
void displayAboutUs(bool willEnd)
{
    system("cls");

    PlaySound(TEXT("Credits.wav"), NULL, SND_FILENAME | SND_ASYNC);

    int n = 0;
    int aboutUs[6][47]{
        {0, 0, 1, 0, 0, 2, 0, 0, 3, 4, 5, 0, 0, 1, 7, 2, 4, 2, 0, 0, 7, 2, 4, 3, 2, 4, 0, 0, 6, 1, 7, 6, 0, 0, 1, 7, 2, 4, 5, 0, 0, 3, 2, 4, 6, 1, 3},
        {0, 0, 2, 0, 0, 1, 0, 0, 4, 0, 0, 0, 0, 2, 0, 0, 0, 3, 0, 0, 1, 3, 5, 4, 3, 5, 0, 0, 0, 2, 1, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 4, 0, 0, 0, 0, 4},
        {0, 0, 3, 0, 0, 7, 0, 0, 5, 3, 6, 0, 0, 3, 1, 3, 5, 4, 0, 0, 0, 0, 6, 5, 0, 0, 0, 0, 0, 3, 2, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 5},
        {0, 0, 4, 0, 0, 6, 0, 0, 6, 0, 0, 0, 0, 4, 0, 0, 6, 0, 0, 0, 0, 0, 7, 6, 0, 0, 0, 0, 0, 4, 3, 0, 0, 0, 4, 0, 0, 5, 7, 0, 0, 6, 0, 0, 0, 0, 6},
        {0, 0, 0, 4, 6, 0, 0, 0, 7, 0, 0, 0, 0, 5, 0, 0, 7, 5, 0, 0, 0, 0, 1, 7, 0, 0, 0, 0, 0, 5, 4, 0, 0, 0, 5, 0, 0, 0, 1, 0, 0, 7, 0, 0, 0, 0, 7},
        {0, 0, 0, 5, 5, 0, 0, 0, 1, 2, 7, 0, 0, 6, 0, 0, 1, 6, 0, 0, 0, 0, 2, 1, 0, 0, 0, 0, 7, 6, 5, 7, 0, 0, 6, 1, 3, 6, 2, 0, 0, 1, 3, 5, 7, 2, 1}};

    while (true)
    {
        Sleep(n);
        setCursor();
        for (int i = 0; i < 6; i++)
        {
            cout << endl;
            for (int j = 0; j < 47; j++)
                cout << applyTheme(aboutUs[i][j], 2 + generateRandomNumber(3));
        }
        if (kbhit())
        {
            getch();
            break;
        }
        if (n < 89)
            n += 1;
        else if (!willEnd)
            cout << chooseColor(generateRandomNumber(7)) << "\n\n\n   P r e s s   a n y   k e y";
        else
            break;
    }

    setCursor();
    for (int i = 0; i < 6; i++)
    {
        cout << endl;
        for (int j = 0; j < 47; j++)
            cout << applyTheme(aboutUs[i][j], 4);
    }
    cout << WHITE_COLOR << BOLDON_STYLE << " TEAM\n"
         << BOLDOFF_STYLE;

    cout << "\n\n"
         << setw(30) << "";

    string creators = "\n\033[31mTETRIS~~\033[37m \nA project by~\n\n  -AliThz`\n  -SrgtSajjad`";
    printCharacterByCharacter(creators, 50, 50, 100);

    Sleep(500);
    if (!willEnd)
    {
        cout << "\n\n\n\nPress any key to go to the main menu";
        getch();
        PlaySound(TEXT("Tetris2.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
        displayMenu();
    }
}

/*EXIT GAME*/
void exitGame()
{
    system("cls");
    displayAboutUs(true);
    system("cls");
    displayTitle("Thanks for playing!", chooseColor(generateRandomNumber(7)));
    exit(0);
}

/*UI COMPONENTS*/
string applyTheme(int tetriminoNumber, int theme)
{
    string static themes[8][6] = {
        {"  ",
         "  ",
         "  ",
         "  ",
         "  ",
         "  "},
        {"\033[36m\u2588\u2588",
         "🟫",
         "\033[36m 1",
         "\033[32m[]",
         "\033[36m I",
         "🍍"},
        {"\033[33m\u2588\u2588",
         "🟨",
         "\033[33m 1",
         "\033[32m[]",
         "\033[33m O",
         "🍌"},
        {"\033[37m\u2588\u2588",
         "⬜",
         "\033[37m 1",
         "\033[32m[]",
         "\033[37m L",
         "🥥"},
        {"\033[34m\u2588\u2588",
         "🟦",
         "\033[34m 1",
         "\033[32m[]",
         "\033[34m J",
         "🍈"},
        {"\033[32m\u2588\u2588",
         "🟩",
         "\033[32m 1",
         "\033[32m[]",
         "\033[32m S",
         "🥝"},
        {"\033[31m\u2588\u2588",
         "🟥",
         "\033[31m 1",
         "\033[32m[]",
         "\033[31m Z",
         "🍒"},
        {"\033[35m\u2588\u2588",
         "🟪",
         "\033[35m 1",
         "\033[32m[]",
         "\033[35m T",
         "🍇"}};
    return themes[tetriminoNumber][theme - 1];
}

string chooseColor(int number)
{
    switch (number)
    {
    case 1:
        return CYAN_COLOR;
        break;
    case 2:
        return YELLOW_COLOR;
        break;
    case 3:
        return WHITE_COLOR;
        break;
    case 4:
        return BLUE_COLOR;
        break;
    case 5:
        return GREEN_COLOR;
        break;
    case 6:
        return RED_COLOR;
        break;
    case 7:
        return PURPLE_COLOR;
        break;
    default:
        return WHITE_COLOR;
    }
}

void displayTitle(string title, string color, int space)
{
    int length = title.length();

    if ((space - length) % 2 == 1)
        space++;

    cout << color << BOLDON_STYLE;

    cout << "\u2554";
    for (int i = 0; i < space; i++)
        cout << "\u2550";
    cout << "\u2557 \n";

    cout << "\u2551" << setw(abs(space - length) / 2) << "" << title << setw(abs(space - length) / 2) << "" << right << "\u2551 \n";

    cout << "\u255A";
    for (int i = 0; i < space; i++)
        cout << "\u2550";
    cout << "\u255D ";

    cout << WHITE_COLOR << BOLDOFF_STYLE << endl;
}

void displayOptions(Option options[], int count, int space)
{
    for (int k = 0; k < count; k++)
    {
        cout << setw(10) << "";
        int length = options[k].title.length();

        cout << BLUE_COLOR;
        cout << "\u2554\u2550\u2550\u2550\u2564";
        for (int i = 0; i < space; i++)
            cout << "\u2550";

        cout << "\u2557\n";

        cout << setw(10) << ""
             << "\u2551 " << CYAN_COLOR << options[k].number << BLUE_COLOR << " \u2502 " << CYAN_COLOR << options[k].title << BLUE_COLOR << setw(space - options[k].title.length() - 1) << ""
             << "\u2551\n";

        cout << setw(10) << ""
             << "\u255A\u2550\u2550\u2550\u2567";
        for (int i = 0; i < space; i++)
            cout << "\u2550";
        cout << "\u255D";

        cout << WHITE_COLOR << endl;
    }
}

void displayInput(string label, int space)
{
    cout << endl;

    cout << "\u256D";
    for (int i = 0; i < label.length() + space; i++)
        cout << "\u2500";
    cout << "\u256E" << setw(50) << "";

    cout << "\n\u2502 " << label << setw(space - 1) << ""
         << "\u2502" << setw(50) << "" << endl;

    cout << "\u2570";
    for (int i = 0; i < label.length() + space; i++)
        cout << "\u2500";
    cout << "\u256F" << setw(50) << "";

    cout << setw(50) << "" << WHITE_COLOR;
}

/*TERMINAL*/
void printCharacterByCharacter(string message, int shortSleep, int mediumSleep, int longSleep)
{
    for (int i = 0; i < message.length(); i++)
    {
        if (kbhit())
        {
            getch();
            for (int j = i; j < message.length(); j++)
            {
                if (message[j] != '~' && message[j] != '`')
                    cout << message[j];
                else
                    continue;
            }

            break;
        }

        if (message[i] == '~')
            Sleep(longSleep);
        else if (message[i] == '`')
            Sleep(mediumSleep);
        else
        {
            cout << message[i];
            Sleep(shortSleep);
        }
    }
}

void setCursor(int x, int y)
{
    HANDLE hOut;
    COORD Position;

    hOut = GetStdHandle(STD_OUTPUT_HANDLE);

    Position.X = x;
    Position.Y = y;
    SetConsoleCursorPosition(hOut, Position);
}

void showConsoleCursor(bool flagStatus)
{
    HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(out, &cursorInfo);
    cursorInfo.bVisible = flagStatus;
    SetConsoleCursorInfo(out, &cursorInfo);
}

int generateRandomNumber(int n)
{
    int random = rand() % n + 1;
    return random;
}
