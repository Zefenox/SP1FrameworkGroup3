#ifndef _GAME_H
#define _GAME_H
#include "Framework\timer.h"
#include <string>

extern CStopWatch g_swTimer;
extern bool g_bQuitGame;
class Console;
extern Console g_Console;
class Bullet;
extern Bullet* bulletArray[100];
// struct to store keyboard events
// a small subset of KEY_EVENT_RECORD
struct SKeyEvent
{
    bool keyDown;
    bool keyReleased;
};

// struct to store mouse events
// a small subset of MOUSE_EVENT_RECORD
struct SMouseEvent
{
    COORD mousePosition;
    DWORD buttonState;
    DWORD eventFlags;
};

// Enumeration to store the control keys that your game will have
enum EKEYS
{
    K_W, //K_UP
    K_S, //K_DOWN
    K_A, //K_LEFT
    K_D, //K_RIGHT
    K_Q,
    K_T,
    K_Y,
    K_U,
    K_H,
    K_1,
    K_2, 
    K_3,
    K_4,
    K_5,
    K_L,
    K_ESCAPE,
    K_SPACE,
    K_UP,
    K_DOWN,
    K_COUNT,
    
};

// Enumeration for the different screen states
enum EGAMESTATES
{
    S_SPLASHSCREEN,
    S_STARTSCREEN,
    S_HELP,
    S_GAME,
    S_PAUSESCREEN,
    S_LOSS,
    S_VICTORY,
    S_COUNT
};

struct SGameChar
{
    COORD m_cLocation;
    bool  m_bActive;
    int   hp;
};

void init        ( void );      // initialize your variables, allocate memory, etc
void gameInit    ( void );      // initialize game variables
void getInput    ( void );      // get input from player
void update      ( double dt ); // update the game and the state of the game
void render      ( void );      // renders the current state of the game to the console
void shutdown    ( void );      // do clean up, free memory

void updateSplashScreen();    // waits for time to pass in splash screen
void updateStart();         // updates start menu
void updateHelp();
void updateGame();          // gameplay logic
void updatePause();
void updateLoss();
void updateVictory();
void cheatInput();
void inventoryInput();
void startInput();
void pauseInput();
void lossInput();
void moveCharacter();       // moves the character, collision detection, physics, etc
void processUserInput();    // checks if you should change states or do something else with the game, e.g. pause, exit
void playerInteractions();
void renderInteractions();
void clearScreen();         // clears the current screen and draw from scratch 
void renderTitle(int x,int y);
void renderSplashScreen();  // renders the splash screen
void renderIntroText(int x, int y);
void renderEndText(int x, int y);
void renderStart();
void renderGame();          // renders the game stuff
void renderPauseScreen();   // renders the pause screen
void renderLoss(); // renders the loss screen
void renderVictory(); // renders the victory screen
void renderMap(); // renders the map to the buffer first

void loadmap();
void loadStartmap();
void loadLosescreen();
void renderStartmap();

void shootInput();
void shoot();
void bulletInteraction();
void renderBullets();

void renderCharacter();     // renders the character into the buffer
void renderHelp();
void renderStartOptions();
void renderPauseBase();
void renderPauseOptions();
void renderLossOptions();
void renderGUI();
void renderFramerate();     // renders debug information, frame rate, elapsed time, etc
void renderToScreen();      // dump the contents of the buffer to the screen, one frame worth of game
void renderInputEvents();   // renders the status of input events

//Enemy functions:
void bossMovement(SGameChar BArr[9]); // Boss movement
bool bossAttackSeq(); // Boss attack
char bossSearchPlayer(SGameChar BArr[9]);
char bossProj();
void bossDeath();
//phantom movements (Hardcoded for now)
void phantomMovement();
void phantomMovement2();
void phantomMovement3();
void phantomMovement4();
void phantomMovement5();
char phantomSearchPlayer();
char phantomSearchPlayer2();
char phantomSearchPlayer3();
char phantomSearchPlayer4();
char phantomSearchPlayer5();
void phantomFireProj();         //Phantom fires a proj randomly
void phantomFireProj2();
void phantomFireProj3();
void phantomFireProj4();
void phantomFireProj5();
void projReachPlayer();
//stalker functions
void checkerX1(int i, int n);
void checkerY1(int i, int n);
void checkerX2(int i, int n);
void checkerY2(int i, int n);
void stalkerMovement(SGameChar EArr[12]); // Stalker movement, collision detection
char stalkerSearchPlayer();   // enemy lookout for player to stalk
void stalkerChasePlayer();    // enemy chase player function once searched
void stalkerReachPlayer(); // enemy corners player
// Enemy spawn functions
void renderEnemies(SGameChar EArr[12], int charnum, WORD Colour); // renders enemies into the buffer (Sherryan)
void renderBossBullet();
void renderBoss();
char renderProj();
char renderProj2();
char renderProj3();
char renderProj4();
char renderProj5();
void setStalkerCoords(SGameChar EArr[12]); // random generates enemies with different x and y values
void bossBodyCoord(SGameChar BArr[9], int x, int y); //generates boss body coords that are side - by - side.

// keyboard and mouse input event managers
void keyboardHandler(const KEY_EVENT_RECORD& keyboardEvent);  // define this function for the console to call when there are keyboard events
void mouseHandler(const MOUSE_EVENT_RECORD& mouseEvent);      // define this function for the console to call when there are mouse events

void startKBHandler(const KEY_EVENT_RECORD& keyboardEvent); // handles keyboard events for start screen
void startMouseHandler(const MOUSE_EVENT_RECORD& mouseEvent);

void helpKBHandler(const KEY_EVENT_RECORD& keyboardEvent); // handles keyboard events for start screen
void helpMouseHandler(const MOUSE_EVENT_RECORD& mouseEvent);

void gameplayKBHandler(const KEY_EVENT_RECORD& keyboardEvent);   // handles keyboard events for gameplay 
void gameplayMouseHandler(const MOUSE_EVENT_RECORD& mouseEvent); // handles mouse events for gameplay 

void pauseKBHandler(const KEY_EVENT_RECORD& keyboardEvent); // handles keyboard events for pausescreen
void pauseMouseHandler(const MOUSE_EVENT_RECORD& mouseEvent);

void lossKBHandler(const KEY_EVENT_RECORD& keyboardEvent); // handles keyboard events for loss
void lossMouseHandler(const MOUSE_EVENT_RECORD& mouseEvent);

void victoryKBHandler(const KEY_EVENT_RECORD& keyboardEvent); // handles keyboard events for victory
void victoryMouseHandler(const MOUSE_EVENT_RECORD& mouseEvent);
#endif // _GAME_H
