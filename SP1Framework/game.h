#ifndef _GAME_H
#define _GAME_H
#include "Framework\timer.h"
#include <string>

extern CStopWatch g_swTimer;
extern bool g_bQuitGame;
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
    K_1,
    K_2, 
    K_3,
    K_4,
    K_5,
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
    S_GAME,
    S_PAUSESCREEN,
    S_COUNT
};

// struct for the game character
struct SGameChar
{
    COORD m_cLocation;
    bool  m_bActive;
};

void init        ( void );      // initialize your variables, allocate memory, etc
void getInput    ( void );      // get input from player
void update      ( double dt ); // update the game and the state of the game
void render      ( void );      // renders the current state of the game to the console
void shutdown    ( void );      // do clean up, free memory

void splashScreenWait();    // waits for time to pass in splash screen
void updateStart();         // updates start menu
void updateGame();          // gameplay logic
void updatePause();
void inventoryInput();
void startInput();
void moveCharacter();       // moves the character, collision detection, physics, etc
void processUserInput();    // checks if you should change states or do something else with the game, e.g. pause, exit
void playerInteractions();
void clearScreen();         // clears the current screen and draw from scratch 
void renderTitle();
void renderSplashScreen();  // renders the splash screen
void renderStart();
void renderGame();          // renders the game stuff
void renderPauseScreen();   // renders the pause screen
void renderMap();           // renders the map to the buffer first
void loadmap();
void bulletLogic();

void renderCharacter();     // renders the character into the buffer
void renderStartOptions();
void renderPauseBase();
void renderPauseOptions();
void renderGUI();
void renderFramerate();     // renders debug information, frame rate, elapsed time, etc
void renderToScreen();      // dump the contents of the buffer to the screen, one frame worth of game
void renderInputEvents();   // renders the status of input events

//Enemy functions:
void bossMovement(SGameChar BArr[15]); // Boss movement
void bossAttackSeq();           // Boss attack
void bossSearchPlayer();
void bossDeath();
void phantomMovement();        // Phantom movement
char phantomSearchPlayer();    // Phantom looks out for player to shoot
void phantomFireProj();         //Phantom fires a proj randomly
void stalkerMovement(SGameChar EArr[2]); // Stalker movement, collision detection
bool stalkerSearchPlayer(SGameChar EArr[2]);   // enemy lookout for player to stalk
void stalkerChasePlayer(SGameChar EArr[2]);    // enemy chase player function once searched
// Enemy spawn functions
void renderEnemies(SGameChar EArr[2]); // renders enemies into the buffer (Sherryan)
void renderBoss(SGameChar BArr[15]);
void renderBossParticles(SGameChar BArr[15]);
void renderProj();
bool coordCheck(std::string arr[100], std::string cmb); // Ensures enemies all different x and y values
void randEnemyCoord(SGameChar EArr[2]); // random generates enemies with different x and y values
void bossBodyCoord(SGameChar BArr[15]); //generates boss body coords that are side - by - side.

// keyboard and mouse input event managers
void keyboardHandler(const KEY_EVENT_RECORD& keyboardEvent);  // define this function for the console to call when there are keyboard events
void mouseHandler(const MOUSE_EVENT_RECORD& mouseEvent);      // define this function for the console to call when there are mouse events

void startKBHandler(const KEY_EVENT_RECORD& keyboardEvent); // handles keyboard events for start screen
void startMouseHandler(const MOUSE_EVENT_RECORD& mouseEvent);

void gameplayKBHandler(const KEY_EVENT_RECORD& keyboardEvent);   // handles keyboard events for gameplay 
void gameplayMouseHandler(const MOUSE_EVENT_RECORD& mouseEvent); // handles mouse events for gameplay 

void pauseKBHandler(const KEY_EVENT_RECORD& keyboardEvent); // handles keyboard events for pausescreen
void pauseMouseHandler(const MOUSE_EVENT_RECORD& mouseEvent);
#endif // _GAME_H