#include "game.h"
#include "Player.h"
#include "Chest.h"
#include "Bullet.h"
#include "Framework\console.h"
#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>
#include <sstream>
#include <time.h>
#include <stdio.h>
#include <stdlib.h> 

// define WASDQ keys

#define VK_KEY_W    0x57
#define VK_KEY_A    0x41
#define VK_KEY_S    0x53
#define VK_KEY_D    0x44
#define VK_KEY_Q    0x51
#define VK_KEY_L    0x4C
#define VK_KEY_T    0x54
#define VK_KEY_Y    0x59
#define VK_KEY_U    0x55
#define VK_KEY_H    0x48

int bossProjCount;
double  g_dElapsedTime;
double  g_dDeltaTime;
SKeyEvent g_skKeyEvent[K_COUNT];
SMouseEvent g_mouseEvent;


// Set up sample colours, and output shadings
const WORD colors[] = {
    0x1A, 0x2B, 0x3C, 0x4D, 0x5E, 0x6F,
    0xA1, 0xB2, 0xC3, 0xD4, 0xE5, 0xF6
};

COORD c;

// stalkers
SGameChar   s1, s2, s3, s4, s5, s6, s7, s8, s9, s10, s11, s12;
WORD sColor = 0x0A;
int snum = 83;
// phantom

WORD pcolor = 0x5E;
int pnum = 43;
SGameChar stalkers[12] = { s1,s2,s3,s4,s5,s6,s7,s8,s9,s10, s11, s12 };

//phantoms
SGameChar   p1, p2, p3, p4, p5;
// projectile of phantom
SGameChar pro1, pro2, pro3, pro4, pro5;
WORD projColor = 0x4D;
int projnum = 60;
// boss

SGameChar b1, b2, b3, b4, b5, b6, b7, b8, b9;
SGameChar bossParticles[9] = { b1,b2,b3,b4,b5,b6,b7,b8,b9 };
SGameChar bpro, bpro2, bossHp;
int pos[1500];
int holder = 0; 

Player* player = new Player; // player initialisation
Chest* chest[10] = { nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr }; // chests initialisation

EGAMESTATES g_eGameState = S_SPLASHSCREEN; // initial state

// Console object
Console g_Console(300, 64, "ESCAPE THE DUNGEON");
// map
char map[65][300];
// bullet array
Bullet* bulletArray[100] = { nullptr };
// lv 1 Clear bool
bool map1Clear = false;

//timer variable
int dis = 0;
double cap = 1.0;

int endtimer;

//--------------------------------------------------------------
// Purpose  : Initialisation function
//            Initialize variables, allocate memory, load data from file, etc. 
//            This is called once before entering into your main loop
// Input    : void
// Output   : cvoid
//--------------------------------------------------------------
void init(void)
{
    // assigning seed
    srand((unsigned int)time(NULL));
    

    // Set precision for floating point output
    g_dElapsedTime = 0.0;
    // sets the initial state for the game
    g_eGameState = S_SPLASHSCREEN;

    gameInit();

    // sets the width, height and the font name to use in the console
    g_Console.setConsoleFont(0, 12, L"Consolas");

    // remember to set your keyboard handler, so that your functions can be notified of input events
    g_Console.setKeyboardHandler(keyboardHandler);
    g_Console.setMouseHandler(mouseHandler);
}

void gameInit()
{
    g_Console.clearBuffer();                                                                       // coded by: Sze Ting {
    player->setSpawnPoint(11, 11); // set spawn point
    player->setPosition(player->getSpawnPoint()); // spawn the player at his spawn point

    if (!map1Clear)
    {
        player->setLives(3);
        player->setHealth(100);
        player->setMaxHealth(100);
        for (int i = 0; i < 5; i++)
            player->setInventory(i, nullptr); // clear inventory
        player->setActive(true); // set him to be active
        // initialises chests
        // for floor 1
        chest[0] = new Chest; // initialise chest
        chest[0]->setPosition(156, 36); // set its position
        chest[1] = new Chest;
        chest[1]->setPosition(41, 4);
        chest[2] = new Chest;
        chest[2]->setPosition(89, 30);
        chest[3] = new Chest;
        chest[3]->setPosition(141, 10);
        chest[4] = new Chest;
        chest[4]->setPosition(191, 46);
        chest[5] = new Chest;
        chest[5]->setPosition(197, 46);
        chest[6] = new Chest;
        chest[6]->setPosition(203, 46);                                                            //                     }

        //Set Enemy initial state and positions
        p1.m_bActive = true;
        p2.m_bActive = true;
        p3.m_bActive = true;
        p4.m_bActive = true;
        p5.m_bActive = true;
        p1.m_cLocation.X = 150;
        p1.m_cLocation.Y = 30;
        p2.m_cLocation.X = 40;
        p2.m_cLocation.Y = 50;
        p3.m_cLocation.X = 100;
        p3.m_cLocation.Y = 55;
        p4.m_cLocation.X = 100;
        p4.m_cLocation.Y = 25;
        p5.m_cLocation.X = 170;
        p5.m_cLocation.Y = 30;
        setStalkerCoords(stalkers);
    }

    if (map1Clear)                                                                                // coded by: Sze Ting {
    {
        // for floor 2
        chest[0] = new Chest; // initialise chest
        chest[0]->setPosition(35, 19); // set its position
        chest[1] = new Chest;
        chest[1]->setPosition(122, 14);
        chest[2] = new Chest;
        chest[2]->setPosition(140, 24);
        chest[3] = new Chest;
        chest[3]->setPosition(140, 27);
        chest[4] = new Chest;
        chest[4]->setPosition(140, 30);
        chest[5] = new Chest;
        chest[5]->setPosition(140, 33);
        chest[6] = new Chest;
        chest[6]->setPosition(218, 11);
        chest[7] = new Chest;
        chest[7]->setPosition(158, 61);
        chest[8] = new Chest;
        chest[8]->setPosition(102, 46);
        chest[9] = new Chest;
        chest[9]->setPosition(98, 54);                                                             // }

        // Initialise boss bullets
        bossProjCount = 0;
        bpro.m_bActive = true;
        bpro2.m_bActive = true;
        // Initialise boss Health
        bossHp.hp = 240;
        // Initialise boss spawn
        bossBodyCoord(bossParticles, 215, 40);
        // Reinitialise Stalker spawn points
        setStalkerCoords(stalkers);
        // Reinitialise Phantom spawn points
        p1.m_cLocation.X = 180;
        p1.m_cLocation.Y = 30;
        p2.m_cLocation.X = 40;
        p2.m_cLocation.Y = 10;
        p3.m_cLocation.X = 80;
        p3.m_cLocation.Y = 10;
        p4.m_cLocation.X = 100;
        p4.m_cLocation.Y = 50; 
        p5.m_cLocation.X = 170;
        p5.m_cLocation.Y = 40; 
    }
}

//--------------------------------------------------------------
// Purpose  : Reset before exiting the program
//            Do your clean up of memory here
//            This is called once just before the game exits
// Input    : Void
// Output   : void
//--------------------------------------------------------------
void shutdown(void)
{
    // Reset to white text on black background
    colour(FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);

    g_Console.clearBuffer();

    delete player;
    for (int i = 0; i < 10; i++)
        delete chest[i];
}

//--------------------------------------------------------------
// Purpose  : Get all the console input events
//            This function sets up the keyboard and mouse input from the console.
//            We will need to reset all the keyboard status, because no events will be sent if no keys are pressed.
//
//            Remember to set the handlers for keyboard and mouse events.
//            The function prototype of the handler is a function that takes in a const reference to the event struct
//            and returns nothing. 
//            void pfKeyboardHandler(const KEY_EVENT_RECORD&);
//            void pfMouseHandlerconst MOUSE_EVENT_RECORD&);
// Input    : Void
// Output   : void
//--------------------------------------------------------------
void getInput(void)
{
    // resets all the keyboard events
    memset(g_skKeyEvent, 0, K_COUNT * sizeof(*g_skKeyEvent));
    // then call the console to detect input from user
    g_Console.readConsoleInput();
}

//--------------------------------------------------------------
// Purpose  : This is the handler for the keyboard input. Whenever there is a keyboard event, this function will be called.
//            Ideally, you should pass the key event to a specific function to handle the event.
//            This is because in some states, some keys would be disabled. Hence, to reduce your code complexity, 
//            it would be wise to split your keyboard input handlers separately.
//            
//            The KEY_EVENT_RECORD struct has more attributes that you can use, if you are adventurous enough.
//
//            In this case, we are not handling any keyboard event in the Splashscreen state
//            
// Input    : const KEY_EVENT_RECORD& keyboardEvent - reference to a key event struct
// Output   : void
//--------------------------------------------------------------
void keyboardHandler(const KEY_EVENT_RECORD& keyboardEvent)
{
    switch (g_eGameState)
    {
    case S_SPLASHSCREEN: // don't handle anything for the splash screen
        break;
    case S_STARTSCREEN: startKBHandler(keyboardEvent);                                           // coded by: Sze Ting
        break;
    case S_HELP: helpKBHandler(keyboardEvent);
        break;
    case S_GAME: gameplayKBHandler(keyboardEvent); // handle gameplay keyboard event 
        break;
    case S_PAUSESCREEN: pauseKBHandler(keyboardEvent); // handle pause screen keyboard event
        break;
    case S_LOSS: lossKBHandler(keyboardEvent); // handle loss screen keyboard event
        break;
    case S_VICTORY: victoryKBHandler(keyboardEvent); // handle victory screen keyboard event
        break;
    }
}

//--------------------------------------------------------------
// Purpose  : This is the handler for the mouse input. Whenever there is a mouse event, this function will be called.
//            Ideally, you should pass the key event to a specific function to handle the event.
//            This is because in some states, some keys would be disabled. Hence, to reduce your code complexity, 
//            it would be wise to split your keyboard input handlers separately.
//            
//            For the mouse event, if the mouse is not moved, no event will be sent, hence you should not reset the mouse status.
//            However, if the mouse goes out of the window, you would not be able to know either. 
//
//            The MOUSE_EVENT_RECORD struct has more attributes that you can use, if you are adventurous enough.
//
//            In this case, we are not handling any mouse event in the Splashscreen state
//            
// Input    : const MOUSE_EVENT_RECORD& mouseEvent - reference to a mouse event struct
// Output   : void
//--------------------------------------------------------------
void mouseHandler(const MOUSE_EVENT_RECORD& mouseEvent)
{
    switch (g_eGameState)
    {
    case S_SPLASHSCREEN: // don't handle anything for the splash screen
        break;
    case S_STARTSCREEN: startMouseHandler(mouseEvent);                                         // coded by: Sze Ting
        break;
    case S_HELP: helpMouseHandler(mouseEvent);
        break;
    case S_GAME: gameplayMouseHandler(mouseEvent); // handle gameplay mouse event
        break;
    case S_PAUSESCREEN: pauseMouseHandler(mouseEvent); // handle pause screen mouse event
        break;
    case S_LOSS: lossMouseHandler(mouseEvent); // handle pause screen mouse event
        break;
    case S_VICTORY: victoryMouseHandler(mouseEvent); // handle victory screen mouse event
        break;
    }
}

//--------------------------------------------------------------
// Purpose  : This is the keyboard handler in the game state. Whenever there is a keyboard event in the game state, this function will be called.
//            
//            Add more keys to the enum in game.h if you need to detect more keys
//            To get other VK key defines, right click on the VK define (e.g. VK_UP) and choose "Go To Definition" 
//            For Alphanumeric keys, the values are their ascii values (uppercase).
// Input    : const KEY_EVENT_RECORD& keyboardEvent
// Output   : void
//--------------------------------------------------------------
void gameplayKBHandler(const KEY_EVENT_RECORD& keyboardEvent)
{
    // here, we map the key to our enums
    EKEYS key = K_COUNT;
    switch (keyboardEvent.wVirtualKeyCode)
    {
    case VK_SPACE: key = K_SPACE; break; // used gameplay controls
    case VK_KEY_W: key = K_W; break;
    case VK_KEY_S: key = K_S; break;
    case VK_KEY_A: key = K_A; break;
    case VK_KEY_D: key = K_D; break;
    case VK_ESCAPE: key = K_ESCAPE; break;
    case 0x31: key = K_1; break;
    case 0x32: key = K_2; break;
    case 0x33: key = K_3; break;
    case 0x34: key = K_4; break;
    case 0x35: key = K_5; break;
    case VK_KEY_L: key = K_L; break;

    case VK_KEY_T: key = K_T; break; // cheats
    case VK_KEY_Y: key = K_Y; break;
    case VK_KEY_U: key = K_U; break;

    case VK_KEY_H: key = K_H; break; // help
    }
    // a key pressed event would be one with bKeyDown == true
    // a key released event would be one with bKeyDown == false
    // if no key is pressed, no event would be fired.
    // so we are tracking if a key is either pressed, or released
    if (key != K_COUNT)
    {
        g_skKeyEvent[key].keyDown = keyboardEvent.bKeyDown;
        g_skKeyEvent[key].keyReleased = !keyboardEvent.bKeyDown;
    }
}

void startKBHandler(const KEY_EVENT_RECORD& keyboardEvent)
{
    // here, we map the key to our enums
    EKEYS key = K_COUNT;
    switch (keyboardEvent.wVirtualKeyCode)
    {
    case VK_SPACE: key = K_SPACE; break;
    case VK_ESCAPE: key = K_ESCAPE; break;
    }

    if (key != K_COUNT)
    {
        g_skKeyEvent[key].keyDown = keyboardEvent.bKeyDown;
        g_skKeyEvent[key].keyReleased = !keyboardEvent.bKeyDown;
    }
}

void helpKBHandler(const KEY_EVENT_RECORD& keyboardEvent)
{
    // here, we map the key to our enums
    EKEYS key = K_COUNT;
    switch (keyboardEvent.wVirtualKeyCode)
    {
    case VK_KEY_H: key = K_H; break;
    }

    if (key != K_COUNT)
    {
        g_skKeyEvent[key].keyDown = keyboardEvent.bKeyDown;
        g_skKeyEvent[key].keyReleased = !keyboardEvent.bKeyDown;
    }
}

void pauseKBHandler(const KEY_EVENT_RECORD& keyboardEvent)
{
    // here, we map the key to our enums
    EKEYS key = K_COUNT;
    switch (keyboardEvent.wVirtualKeyCode)
    {
    case VK_ESCAPE: key = K_ESCAPE; break; // used pause screen controls
    case VK_KEY_Q: key = K_Q; break;
    }
    // a key pressed event would be one with bKeyDown == true
    // a key released event would be one with bKeyDown == false
    // if no key is pressed, no event would be fired.
    // so we are tracking if a key is either pressed, or released
    if (key != K_COUNT)
    {
        g_skKeyEvent[key].keyDown = keyboardEvent.bKeyDown;
        g_skKeyEvent[key].keyReleased = !keyboardEvent.bKeyDown;
    }
}

void lossKBHandler(const KEY_EVENT_RECORD& keyboardEvent)
{
    EKEYS key = K_COUNT;
    switch (keyboardEvent.wVirtualKeyCode)
    {
    case VK_SPACE: key = K_SPACE; break; // used loss screen controls
    case VK_KEY_Q: key = K_Q; break;
    }

    if (key != K_COUNT)
    {
        g_skKeyEvent[key].keyDown = keyboardEvent.bKeyDown;
        g_skKeyEvent[key].keyReleased = !keyboardEvent.bKeyDown;
    }
}

void victoryKBHandler(const KEY_EVENT_RECORD& keyboardEvent)
{
    EKEYS key = K_COUNT;
    switch (keyboardEvent.wVirtualKeyCode)
    {
    }

    if (key != K_COUNT)
    {
        g_skKeyEvent[key].keyDown = keyboardEvent.bKeyDown;
        g_skKeyEvent[key].keyReleased = !keyboardEvent.bKeyDown;
    }
}

//--------------------------------------------------------------
// Purpose  : This is the mouse handler in the game state. Whenever there is a mouse event in the game state, this function will be called.
//            
//            If mouse clicks are detected, the corresponding bit for that mouse button will be set.
//            mouse wheel, 
//            
// Input    : const KEY_EVENT_RECORD& keyboardEvent
// Output   : void
//--------------------------------------------------------------
void gameplayMouseHandler(const MOUSE_EVENT_RECORD& mouseEvent)
{
    if (mouseEvent.dwEventFlags & MOUSE_MOVED) // update the mouse position if there are no events
    {
        g_mouseEvent.mousePosition = mouseEvent.dwMousePosition;
    }
    g_mouseEvent.buttonState = mouseEvent.dwButtonState;
    g_mouseEvent.eventFlags = mouseEvent.dwEventFlags;
}

//--------------------------------------------------------------
// Purpose  : This is the mouse handler in the start state. Whenever there is a mouse event in the game state, this function will be called.
//            
//            If mouse clicks are detected, the corresponding bit for that mouse button will be set.
//            mouse wheel, 
//            
// Input    : const KEY_EVENT_RECORD& keyboardEvent
// Output   : void
//--------------------------------------------------------------
void startMouseHandler(const MOUSE_EVENT_RECORD& mouseEvent)
{
    if (mouseEvent.dwEventFlags & MOUSE_MOVED) // update the mouse position if there are no events
    {
        g_mouseEvent.mousePosition = mouseEvent.dwMousePosition;
    }
    g_mouseEvent.buttonState = mouseEvent.dwButtonState;
    g_mouseEvent.eventFlags = mouseEvent.dwEventFlags;
}

//--------------------------------------------------------------
// Purpose  : This is the mouse handler in the guide page state. Whenever there is a mouse event in the game state, this function will be called.
//            
//            If mouse clicks are detected, the corresponding bit for that mouse button will be set.
//            mouse wheel, 
//            
// Input    : const KEY_EVENT_RECORD& keyboardEvent
// Output   : void
//--------------------------------------------------------------
void helpMouseHandler(const MOUSE_EVENT_RECORD& mouseEvent)
{
    if (mouseEvent.dwEventFlags & MOUSE_MOVED) // update the mouse position if there are no events
    {
        g_mouseEvent.mousePosition = mouseEvent.dwMousePosition;
    }
    g_mouseEvent.buttonState = mouseEvent.dwButtonState;
    g_mouseEvent.eventFlags = mouseEvent.dwEventFlags;
}

//--------------------------------------------------------------
// Purpose  : This is the mouse handler in the pause page state. Whenever there is a mouse event in the game state, this function will be called.
//            
//            If mouse clicks are detected, the corresponding bit for that mouse button will be set.
//            mouse wheel, 
//            
// Input    : const KEY_EVENT_RECORD& keyboardEvent
// Output   : void
//--------------------------------------------------------------
void pauseMouseHandler(const MOUSE_EVENT_RECORD& mouseEvent)
{
    if (mouseEvent.dwEventFlags & MOUSE_MOVED) // update the mouse position if there are no events
    {
        g_mouseEvent.mousePosition = mouseEvent.dwMousePosition;
    }
    g_mouseEvent.buttonState = mouseEvent.dwButtonState;
    g_mouseEvent.eventFlags = mouseEvent.dwEventFlags;
}

//--------------------------------------------------------------
// Purpose  : This is the mouse handler in the lose page state. Whenever there is a mouse event in the game state, this function will be called.
//            
//            If mouse clicks are detected, the corresponding bit for that mouse button will be set.
//            mouse wheel, 
//            
// Input    : const KEY_EVENT_RECORD& keyboardEvent
// Output   : void
//--------------------------------------------------------------
void lossMouseHandler(const MOUSE_EVENT_RECORD& mouseEvent)
{
    if (mouseEvent.dwEventFlags & MOUSE_MOVED) // update the mouse position if there are no events
    {
        g_mouseEvent.mousePosition = mouseEvent.dwMousePosition;
    }
    g_mouseEvent.buttonState = mouseEvent.dwButtonState;
    g_mouseEvent.eventFlags = mouseEvent.dwEventFlags;
}

//--------------------------------------------------------------
// Purpose  : This is the mouse handler in the win page state. Whenever there is a mouse event in the game state, this function will be called.
//            
//            If mouse clicks are detected, the corresponding bit for that mouse button will be set.
//            mouse wheel, 
//            
// Input    : const KEY_EVENT_RECORD& keyboardEvent
// Output   : void
//--------------------------------------------------------------
void victoryMouseHandler(const MOUSE_EVENT_RECORD& mouseEvent)
{
    if (mouseEvent.dwEventFlags & MOUSE_MOVED) // update the mouse position if there are no events
    {
        g_mouseEvent.mousePosition = mouseEvent.dwMousePosition;
    }
    g_mouseEvent.buttonState = mouseEvent.dwButtonState;
    g_mouseEvent.eventFlags = mouseEvent.dwEventFlags;
}

//--------------------------------------------------------------
// Purpose  : Update function
//            This is the update function
//            double dt - This is the amount of time in seconds since the previous call was made
//
//            Game logic should be done here.
//            Such as collision checks, determining the position of your game characters, status updates, etc
//            If there are any calls to write to the console here, then you are doing it wrong.
//
//            If your game has multiple states, you should determine the current state, and call the relevant function here.
//
// Input    : dt = deltatime
// Output   : void
//--------------------------------------------------------------
void update(double dt)
{
    // get the delta time
    g_dElapsedTime += dt;
    g_dDeltaTime = dt;

    switch (g_eGameState)
    {
    case S_SPLASHSCREEN: updateSplashScreen(); // game logic for the splash screen
        break;
    case S_STARTSCREEN: updateStart();
        break;
    case S_HELP: updateHelp();
        break;
    case S_GAME: updateGame(); // gameplay logic when we are in the game
        break;
    case S_PAUSESCREEN: updatePause();
        break;
    case S_VICTORY: updateVictory();
        break;
    case S_LOSS: updateLoss();
        break;
    }
}

//--------------------------------------------------------------
// Purpose  : Update function
//            This is the update function when the player wins
// Input    : map1Clear, endtimer
// Output   : void
//------------------------------------------------------------
void updateVictory()
{
    map1Clear = false;
    gameInit();
    if (endtimer > 150)
    {
        g_eGameState = S_STARTSCREEN;
        dis = 0;
    }
}

//--------------------------------------------------------------
// Purpose  : Update function
//            This is the update function for the splash screen
// Input    : g_dElapsedTime
// Output   : void
// done by luke
//--------------------------------------------------------------
void updateSplashScreen()    // waits for time to pass in splash screen
{
    /*
    if (g_dElapsedTime > 5.0)
    {
        g_eGameState = S_STARTSCREEN;
    }
    else {
        if (g_dElapsedTime < 2.0)
        {
            renderSplashScreen();
        }


    }
    */
    // wait for 3 seconds to switch to start screen, else do nothing


   /*processUserInput();*/
   /*getInput();*/
   /*if (g_skKeyEvent[K_SPACE].keyReleased)
   {
       g_eGameState = S_GAME;
   }*/

    if (g_dElapsedTime > 10.0)
    {
        g_eGameState = S_STARTSCREEN;
        dis = 0;
    }
}

//--------------------------------------------------------------
// Purpose  : Update function
//            This is the update function when the game starts
// Input    : NA
// Output   : void
//--------------------------------------------------------------
void updateStart()
{
    startInput();
}

//--------------------------------------------------------------
// Purpose  : Update function
//            This is the update function for the help screen
// Input    : g_skKeyEvent[K_H]
// Output   : void
//--------------------------------------------------------------
void updateHelp() 
{
    if (g_skKeyEvent[K_H].keyReleased)
        g_eGameState = S_GAME;
}

//--------------------------------------------------------------
// Purpose  : Update function
//            This is the update function for the game flow
//            The flow of the game is set here
// Input    : player->getActive
// Output   : void
//--------------------------------------------------------------
void updateGame()       // gameplay logic
{
    processUserInput(); // checks if you should change states or do something else with the game, e.g. pause, exit
    cheatInput();        // checks for cheat commands
    inventoryInput();
    moveCharacter();    // moves the character, collision detection, physics, etc
                        // sound can be played here too.
    shootInput();
    stalkerMovement(stalkers); // Movements of enemies
    phantomMovement();
    phantomMovement2();
    phantomMovement3();
    phantomMovement4();
    phantomMovement5();
    bossMovement(bossParticles);
    playerInteractions();
    bulletInteraction();
    // interactions
    player->PlayerUpdate(); // checks for updates to player status

    if (!player->getActive()) // if player is dead
        g_eGameState = S_LOSS;
}

//--------------------------------------------------------------
// Purpose  : Update function
//            This is the update function for the pause screen
// Input    : NA
// Output   : void
//--------------------------------------------------------------
void updatePause()
{
    processUserInput();
    pauseInput();
}

//--------------------------------------------------------------
// Purpose  : Update function
//            This is the update function for the Lose screen
// Input    : NA
// Output   : void
//--------------------------------------------------------------
void updateLoss()
{
    lossInput();
}

//--------------------------------------------------------------
// Purpose  : move/set position function
//            This is the function for moving the player
// Input    : player->getX(); player->getY(); g_skKeyEvent[K_W].keyDown
// Output   : void
// done by luke
//--------------------------------------------------------------
void moveCharacter()
{
    // Updating the location of the character based on the key release
    // providing a beep sound whenver we shift the character
    int x;
    int y;

    x = player->getX();
    y = player->getY();

    if (g_skKeyEvent[K_W].keyDown && player->getY() > 0)
    {
        if ((map[y - 1][x] != '#') &&
            (map[y - 1][x] != '=') &&
            (map[y - 1][x] != '[') &&
            (map[y - 1][x] != ']') &&
            (map[y - 1][x] != ')') &&
            (map[y - 1][x] != '(') &&
            (map[y - 1][x] != '*'))
        {
            //Beep(1440, 30);
            player->setPosition(player->getX(), player->getY() - 1);
            player->setDirection('U');
        }
    }

if (g_skKeyEvent[K_A].keyDown && player->getX() > 0)
{
    if ((map[y][x - 1] != '#') &&
        (map[y][x - 1] != '=') &&
        (map[y][x - 1] != '[') &&
        (map[y][x - 1] != ']') &&
        (map[y][x - 1] != ')') &&
        (map[y][x - 1] != '(') &&
        (map[y][x - 1] != '*'))
    {
        //Beep(1440, 30);
        player->setPosition(player->getX() - 1, player->getY());
        player->setDirection('L');
    }
}
if (g_skKeyEvent[K_S].keyDown && player->getY() < g_Console.getConsoleSize().Y - 1)
{
    if ((map[y + 1][x] != '#') &&
        (map[y + 1][x] != '=') &&
        (map[y + 1][x] != '[') &&
        (map[y + 1][x] != ']') &&
        (map[y + 1][x] != ')') &&
        (map[y + 1][x] != '(') &&
        (map[y + 1][x] != '*'))
    {
        //Beep(1440, 30);
        player->setPosition(player->getX(), player->getY() + 1);
        player->setDirection('D');
    }
}
if (g_skKeyEvent[K_D].keyDown && player->getX() < g_Console.getConsoleSize().X - 1)
{
    if ((map[y][x + 1] != '#') &&
        (map[y][x + 1] != '=') &&
        (map[y][x + 1] != '[') &&
        (map[y][x + 1] != ']') &&
        (map[y][x + 1] != ')') &&
        (map[y][x + 1] != '(') &&
        (map[y][x + 1] != '*'))
    {
        //Beep(1440, 30);
        player->setPosition(player->getX() + 1, player->getY());
        player->setDirection('R');
    }
}

}

//--------------------------------------------------------------
// Purpose  : set function
//            This is the function for setting up cheats
// Input    : g_skKeyEvent[K_T]; g_skKeyEvent[K_Y]; g_skKeyEvent[K_U];
// Output   : void
//--------------------------------------------------------------
void cheatInput()
{
    if (g_skKeyEvent[K_T].keyReleased)
        player->setPosition(g_mouseEvent.mousePosition);
    if (g_skKeyEvent[K_Y].keyDown)
        player->setHealth(player->getHealth() - 10);
    if (g_skKeyEvent[K_U].keyReleased)
    {
        if (player->getMaxHealth() == 9999)
            player->setMaxHealth(100);
        else
            player->setMaxHealth(9999);
        player->setHealth(player->getMaxHealth());
    }
}

//--------------------------------------------------------------
// Purpose  : set function
//            This is the function for using consumables
// Input    : g_skKeyEvent[K_1]; player->getInventory()
// Output   : void
//--------------------------------------------------------------
void inventoryInput()
{
    if (g_skKeyEvent[K_1].keyDown)
    {
        if (player->getInventory(0) != nullptr && player->getInventory(0)->getCanBeConsumed() && player->getActive())
        {
            player->consume(player->getInventory(0));
            player->setInventory(0, nullptr);
        }
    }
    if (g_skKeyEvent[K_2].keyDown)
    {
        if (player->getInventory(1) != nullptr && player->getInventory(1)->getCanBeConsumed() && player->getActive())
        {
            player->consume(player->getInventory(1));
            player->setInventory(1, nullptr);
        }
    }
    if (g_skKeyEvent[K_3].keyDown)
    {
        if (player->getInventory(2) != nullptr && player->getInventory(2)->getCanBeConsumed() && player->getActive())
        {
            player->consume(player->getInventory(2));
            player->setInventory(2, nullptr);
        }
    }
    if (g_skKeyEvent[K_4].keyDown)
    {
        if (player->getInventory(3) != nullptr && player->getInventory(3)->getCanBeConsumed() && player->getActive())
        {
            player->consume(player->getInventory(3));
            player->setInventory(3, nullptr);
        }
    }
    if (g_skKeyEvent[K_5].keyDown)
    {
        if (player->getInventory(4) != nullptr && player->getInventory(4)->getCanBeConsumed() && player->getActive())
        {
            player->consume(player->getInventory(4));
            player->setInventory(4, nullptr);
        }
    }

}

//--------------------------------------------------------------
// Purpose  : set function
//            This is the function for calling out shoot function
// Input    : g_skKeyEvent[K_L]; g_mouseEvent;
// Output   : void
// done by luke
//--------------------------------------------------------------
void shootInput()
{
    if (g_skKeyEvent[K_L].keyReleased)
        shoot();

    if (g_mouseEvent.buttonState == FROM_LEFT_1ST_BUTTON_PRESSED)
        shoot();
}

//--------------------------------------------------------------
// Purpose  : set function
//            This is the function for start screen inputs
// Input    : g_mouseEvent; g_skKeyEvent[K_SPACE];
// Output   : void
//-------------------------------------------------------------
void startInput()
{
    // (100 - 123, 28)
    for (int x = 100; x <= 123; x++)
    {
        if (g_mouseEvent.mousePosition.X == x && g_mouseEvent.mousePosition.Y == 28 && g_mouseEvent.buttonState == FROM_LEFT_1ST_BUTTON_PRESSED)
            g_eGameState = S_GAME;
    }
    // (100 - 120, 31)
    for (int x = 100; x <= 120; x++)
    {
        if (g_mouseEvent.mousePosition.X == x && g_mouseEvent.mousePosition.Y == 31 && g_mouseEvent.buttonState == FROM_LEFT_1ST_BUTTON_PRESSED)
            g_bQuitGame = true;
    }

    if (g_skKeyEvent[K_SPACE].keyDown)
        g_eGameState = S_GAME;
    if (g_skKeyEvent[K_ESCAPE].keyDown)
        g_bQuitGame = true;

}

//--------------------------------------------------------------
// Purpose  : set function
//            This is the function for pause screen inputs
// Input    : g_mouseEvent; g_skKeyEvent[K_Q];
// Output   : void
//--------------------------------------------------------------
void pauseInput()
{
    // (100 - 124, 27)
    for (int x = 100; x <= 124; x++)
    {
        if (g_mouseEvent.mousePosition.X == x && g_mouseEvent.mousePosition.Y == 27 && g_mouseEvent.buttonState == FROM_LEFT_1ST_BUTTON_PRESSED)
            g_eGameState = S_GAME;
    }
    // (100 - 118, 30)
    for (int x = 100; x <= 118; x++)
    {
        if (g_mouseEvent.mousePosition.X == x && g_mouseEvent.mousePosition.Y == 30 && g_mouseEvent.buttonState == FROM_LEFT_1ST_BUTTON_PRESSED)
            g_bQuitGame = true;
    }

    if (g_skKeyEvent[K_Q].keyDown)
        g_bQuitGame = true;
}

//--------------------------------------------------------------
// Purpose  : set function
//            This is the function for losing screen inputs
// Input    : g_mouseEvent; g_skKeyEvent[K_Q];g_skKeyEvent[K_SPACE]
// Output   : void
//--------------------------------------------------------------
void lossInput()
{
    // (30 - 53, 28)
    for (int x = 30; x <= 53; x++)
    {
        if (g_mouseEvent.mousePosition.X == x && g_mouseEvent.mousePosition.Y == 28 && g_mouseEvent.buttonState == FROM_LEFT_1ST_BUTTON_PRESSED)
        {
            gameInit();
            g_eGameState = S_GAME;
        }
    }
    // (30 - 48, 31)
    for (int x = 30; x <= 48; x++)
    {
        if (g_mouseEvent.mousePosition.X == x && g_mouseEvent.mousePosition.Y == 31 && g_mouseEvent.buttonState == FROM_LEFT_1ST_BUTTON_PRESSED)
            g_bQuitGame = true;
    }

    if (g_skKeyEvent[K_Q].keyDown)
        g_bQuitGame = true;
    if (g_skKeyEvent[K_SPACE].keyDown)
    {
        map1Clear = false;
        gameInit();
        g_eGameState = S_GAME;
    }
}

//--------------------------------------------------------------
// Purpose  : set function
//            This is the function for processing user inputs
// Input    : g_eGameState; g_skKeyEvent[K_H];g_skKeyEvent[K_SPACE]
// Output   : void
//--------------------------------------------------------------
void processUserInput()
{
    // quits the game if player hits the escape key
    if (g_skKeyEvent[K_ESCAPE].keyReleased) // toggle menu/pause screen
    {
        if (g_eGameState == S_GAME)
        {
            g_eGameState = S_PAUSESCREEN;
            return;
        }
        if (g_eGameState == S_PAUSESCREEN)
        {
            g_eGameState = S_GAME;
            return;
        }
    }

    if (g_skKeyEvent[K_H].keyReleased)
        g_eGameState = S_HELP;
}

//--------------------------------------------------------------
// Purpose  : Render function is to update the console screen
//            At this point, you should know exactly what to draw onto the screen.
//            Just draw it!
//            To get an idea of the values for colours, look at console.h and the URL listed there
// Input    : void
// Output   : void
//--------------------------------------------------------------
void render()
{
    clearScreen();      // clears the current screen and draw from scratch 
    switch (g_eGameState)
    {
    case S_SPLASHSCREEN: renderSplashScreen();
        break;
    case S_STARTSCREEN: renderStart();
        break;
    case S_HELP: renderHelp();
        break;
    case S_GAME: renderGame();
        break;
    case S_PAUSESCREEN: renderPauseScreen();
        break;
    case S_LOSS: renderLoss();
        break;
    case S_VICTORY: renderVictory();
        break;
    }
    renderFramerate();      // renders debug information, frame rate, elapsed time, etc
    //renderInputEvents();    // renders status of input events
    renderToScreen();       // dump the contents of the buffer to the screen, one frame worth of game
}

//--------------------------------------------------------------
// Purpose  : function is to clear the console buffer 
// Input    : void
// Output   : void
//--------------------------------------------------------------
void clearScreen()
{
    // Clears the buffer with this colour attribute
    g_Console.clearBuffer(0x1F);
}

//--------------------------------------------------------------
// Purpose  : function is to render the game title 
// Input    : void
// Output   : void
// done by luke
//--------------------------------------------------------------
void renderTitle(int x, int y) // function to render title
{
    const WORD colors[] = {
        0x0F, 0x2B, 0x3C, 0x4D, 0x5E, 0x6F,
        0xA1, 0xB2, 0xC3, 0xD4, 0xE5, 0xF6
    };
    COORD c = g_Console.getConsoleSize();
    c.Y = y;
    c.X = x;

    // ESCAPE
    g_Console.writeToBuffer(c, "    //   / /  //   ) )  //   ) )  // | |     //   ) ) //   / / ", colors[0]);
    c.Y += 1;
    g_Console.writeToBuffer(c, "   //____    ((        //        //__| |    //___/ / //____    ", colors[0]);
    c.Y += 1;
    g_Console.writeToBuffer(c, "  / ____       ", colors[0]);
    c.X += 15;
    g_Console.writeToBuffer(c, (char)92, colors[0]);
    c.X += 1;
    g_Console.writeToBuffer(c, (char)92, colors[0]);
    c.X += 1;
    g_Console.writeToBuffer(c, "     //        / ___  |   / ____ / / ____     ", colors[0]);
    c.X -= 17;
    c.Y += 1;
    g_Console.writeToBuffer(c, " //              ) ) //        //    | |  //       //          ", colors[0]);
    c.Y += 1;
    g_Console.writeToBuffer(c, "//____/ / ((___ / / ((____/ / //     | | //       //____/ /    ", colors[0]);


    // THE
    c.Y += 2;
    c.X += 15;
    g_Console.writeToBuffer(c, " /__  ___/ //    / / //   / / ", colors[0]);
    c.Y += 1;
    g_Console.writeToBuffer(c, "   / /    //___ / / //____    ", colors[0]);
    c.Y += 1;
    g_Console.writeToBuffer(c, "  / /    / ___   / / ____     ", colors[0]);
    c.Y += 1;
    g_Console.writeToBuffer(c, " / /    //    / / //          ", colors[0]);
    c.Y += 1;
    g_Console.writeToBuffer(c, "/ /    //    / / //____/ /    ", colors[0]);


    // DUNGEON
    c.Y += 2;
    c.X -= 20;
    g_Console.writeToBuffer(c, "    //    ) ) //   / / /|    / / //   ) )  //   / /  //   ) ) /|    / / ", colors[0]);
    c.Y += 1;
    g_Console.writeToBuffer(c, "   //    / / //   / / //|   / / //        //____    //   / / //|   / /  ", colors[0]);
    c.Y += 1;
    g_Console.writeToBuffer(c, "  //    / / //   / / // |  / / //  ____  / ____    //   / / // |  / /   ", colors[0]);
    c.Y += 1;
    g_Console.writeToBuffer(c, " //    / / //   / / //  | / / //    / / //        //   / / //  | / /    ", colors[0]);
    c.Y += 1;
    g_Console.writeToBuffer(c, "//____/ / ((___/ / //   |/ / ((____/ / //____/ / ((___/ / //   |/ /     ", colors[0]);
    c.Y += 1;
}

//--------------------------------------------------------------
// Purpose  : function is to write buffer to console
// Input    : void
// Output   : void
//--------------------------------------------------------------
void renderToScreen()
{
    // Writes the buffer to the console, hence you will see what you have written
    g_Console.flushBufferToConsole();
}

//--------------------------------------------------------------
// Purpose  : function is to render the splash screen
// Input    : void
// Output   : void
//--------------------------------------------------------------
void renderSplashScreen()  // renders the splash screen
{
    //render's BG
    COORD size = g_Console.getConsoleSize();

    //if (g_dElapsedTime < 4)
    //{
    g_Console.clearBuffer();
    if (dis < 61)
        dis++;
    for (int i = 0; i < size.Y; i++)
    {
        for (int x = 0; x < size.X; x++)
        {
            g_Console.writeToBuffer(x, i, " ", 0x80);
        }
    }
    renderTitle(100, 65 - dis);
    renderIntroText(300 + dis, 30);
    //renderIntroText(100, -dis);
//}
//else 
/*
{
    for (int i = 0; i < size.Y; i++)
    {
        for (int x = 0; x < size.X; x++)
        {
            g_Console.writeToBuffer(x, i, " ", 0x80);
        }
    }
    renderTitle(100, 20);
}
*/

}

//--------------------------------------------------------------
// Purpose  : function is to render the intro text
// Input    : void
// Output   : void
// made by luke
//--------------------------------------------------------------
void renderIntroText(int x, int y)
{
    g_Console.writeToBuffer(x + 5, y + 1, "You feel the stagnant wind flow over your body as you open your eyes, lying down on the stone floor. ", 0x0F);
    g_Console.writeToBuffer(x + 5, y + 2, "As you get up, you try to recall the moments that happened before you lost consciousness. ", 0x0F);
    g_Console.writeToBuffer(x + 5, y + 3, "Enduring the headache as you think hard, a memory appeared before your mind. ", 0x0F);
    g_Console.writeToBuffer(x + 5, y + 4, "Though a fragment of what you’re not yet able to remember,", 0x0F);
    g_Console.writeToBuffer(x + 5, y + 5, "it is something that nudged you into the goal that you have set for yourself: to escape the Dungeon.", 0x0F);
    g_Console.writeToBuffer(x + 5 + 30, y + 7, "What Dungeon?", 0x0F);
    g_Console.writeToBuffer(x + 5, y + 9, "Then it became apparent the place where you woke up in was the same place that you have remembered trying to get out of, ", 0x0F);
    g_Console.writeToBuffer(x + 5, y + 10, "a cold and wet labyrinth with dimly lit torches illuminating its hallways. ", 0x0F);
    g_Console.writeToBuffer(x + 5, y + 11, "There is a weathered steel door right in front of you that was decorated with intricate patterns and a symbol of a nondescript man walking through a doorway,", 0x0F);
    g_Console.writeToBuffer(x + 5, y + 12, "and it was only a stone’s throw away from where you standing. ", 0x0F);
    g_Console.writeToBuffer(x + 5, y + 10, "Seeing this door as an obvious way out, you walk over to the door and tug onto its handle, but the door refuses to open. ", 0x0F);
    g_Console.writeToBuffer(x + 5, y + 11, "Then you press your hands against it, and the same result occurs. ", 0x0F);
    g_Console.writeToBuffer(x + 5, y + 12, "After a moment of trial and error getting through the stubborn door, it refuses to budge. ", 0x0F);
    g_Console.writeToBuffer(x + 5, y + 13, "Frustrated, you decide to turn around and tread carefully in the opposite direction,", 0x0F);
    g_Console.writeToBuffer(x + 5, y + 13, "curious of what this so-called Dungeon has to offer but hoping for another way out of here.", 0x0F);

}

//--------------------------------------------------------------
// Purpose  : function is to render the ending text
// Input    : void
// Output   : void
// Myron
//--------------------------------------------------------------
void renderEndText(int x, int y)
{

    g_Console.writeToBuffer(x, y, "Sweating from both the heat of the underground and the last battle against a body of bodies,", 0x0F);
    g_Console.writeToBuffer(x, y + 1, "you stroll into the deepest unknown and press on forward through the darkness before coming across an old desk with a lone lamp. ", 0x0F);
    g_Console.writeToBuffer(x, y + 2, "The light cast by the lamp brings your attention to the worn but intact book resting on the table surface.", 0x0F);
    g_Console.writeToBuffer(x + 50, y + 3, "On its cover read, “My Diary”.", 0x0F);
    g_Console.writeToBuffer(x, y + 4, "Intrigued by the content which lies within the book, you walk over to the desk and turn the book to its first page:", 0x0F);
    g_Console.writeToBuffer(x, y + 5, "“Dear diary, my best friend ف̵̌̈́ф̶̈́̃#̸̧͂φ̸̓̄$̴̝͐و̴͋̏א̵͙̽ and I have been close friends for a few years, but I can’t believe he forgot my birthday!", 0x0F);
    g_Console.writeToBuffer(x, y + 6, "So to prank him back, I decided to build an elaborate dungeon filled with traps and monsters and all that to scare the pants out of him and teach him a lesson!", 0x0F);
    g_Console.writeToBuffer(x, y + 7, "Once I’m done building this dungeon, I’ll visit П̶̤̒о̶̖̃л̸͓̈ and then invite П̵̹̇о̵͋̋л̶͙̈́ to this place. And then...”", 0x0F);

}

//--------------------------------------------------------------
// Purpose  : function is to render the starting text
// Input    : void
// Output   : void
//--------------------------------------------------------------
void renderStart()
{
    loadStartmap();
    renderStartmap();
    renderTitle(30, 10);
    renderStartOptions();
}

//--------------------------------------------------------------
// Purpose  : function is to render the game contents
// Input    : void
// Output   : void
//--------------------------------------------------------------
void renderGame()
{
    loadmap();
    renderMap();        // renders the map to the buffer first
    renderCharacter();  // renders the character into the buffer
    renderBullets();
    renderInteractions();
    renderGUI();        // renders game user interface

    renderEnemies(stalkers, snum, sColor);    // renders the enemies into the buffer 
    if (map1Clear == true)  // renders boss in 2nd map
    {
        //renderEnemies(stalkers, snum, sColor);
        renderBoss();
    }
}

//--------------------------------------------------------------
// Purpose  : function is to render the pause menu
// Input    : void
// Output   : void
//--------------------------------------------------------------
void renderPauseScreen()
{
    renderGame();
    renderPauseBase();
    renderPauseOptions();
}

//--------------------------------------------------------------
// Purpose  : function is to call the funct rendering losing screen options
// Input    : void
// Output   : void
//--------------------------------------------------------------
void renderLoss()
{
    renderLossOptions();
}

//--------------------------------------------------------------
// Purpose  : function is to render the winning screen
// Input    : void
// Output   : void
// Myron
//--------------------------------------------------------------
void renderVictory()
{
    loadWin();
    COORD size = g_Console.getConsoleSize();

    //if (g_dElapsedTime < 4)
    //{
    g_Console.clearBuffer();
    if (dis < 51)
        dis++;
    renderWin();
    renderEndText(50, 65 - dis);
    endtimer++;
}

//--------------------------------------------------------------
// Purpose  : function is to check through the starting map
// Input    : void
// Output   : void
// made by luke
//--------------------------------------------------------------
void loadStartmap()
{
    std::ifstream startscreen("Startscreen.txt");
    std::string line;
    // Init and store Map
    int y = 0;
    while (getline(startscreen, line)) {
        // Output the text from the file
        for (unsigned i = 0; i < line.length(); ++i)
        {
            map[y][i] = line.at(i);
        }
        y++;
    }
}

//--------------------------------------------------------------
// Purpose  : function is to render the losing screen
// 
// Input    : void
// Output   : void
// Myron
//--------------------------------------------------------------
void loadLosescreen()
{
    std::ifstream losescreen("LoseScreen.txt");
    std::string line;
    // Init and store Map
    int y = 0;
    while (getline(losescreen, line)) {
        // Output the text from the file
        for (unsigned i = 0; i < line.length(); ++i)
        {
            map[y][i] = line.at(i);

        }
        y++;
    }
    for (int y = 0; y < 65; y++)
    {
        for (int x = 0; x < 300; x++)
        {
            if (map[y][x] == '#') //wall, cannot pass
            {
                g_Console.writeToBuffer(x, y, (char)219, 0x80);
            }
            else if (map[y][x] == '=') //wall, cannot pass
            {
                g_Console.writeToBuffer(x, y, (char)178, 0x80);
            }
            else //empty space
            {
                g_Console.writeToBuffer(x, y, ' ', 0x80);
            }
        }
    }

}

//--------------------------------------------------------------
// Purpose  : function is to render the starting map
// Input    : void
// Output   : void
//--------------------------------------------------------------
void renderStartmap()
{
    for (int y = 0; y < 65; y++)
    {
        for (int x = 0; x < 300; x++)
        {
            if (map[y][x] == '=')
            {
                g_Console.writeToBuffer(x, y, (char)186, 0x00);
            }
            else if (map[y][x] == '#')
            {
                g_Console.writeToBuffer(x, y, ' ', 0x80);
            }
            else if (map[y][x] == '+')
            {
                g_Console.writeToBuffer(x, y, ' ', 0x22);
            }
            else if (map[y][x] == '~')
            {
                g_Console.writeToBuffer(x, y, ' ', 0x00);
            }
            else if (map[y][x] == '!')
            {
                g_Console.writeToBuffer(x, y, ' ', 0xCC);
            }
            else if (map[y][x] == '"')
            {
                g_Console.writeToBuffer(x, y, ' ', 0x44);
            }
            else //empty space
            {
                g_Console.writeToBuffer(x, y, ' ', 0x77);
            }
        }
    }
}

void loadWin()
{
    std::ifstream Winscreen("Win.txt");
    std::string line;
    // Init and store Map
    int y = 0;
    while (getline(Winscreen, line)) {
        // Output the text from the file
        for (unsigned i = 0; i < line.length(); ++i)
        {
            map[y][i] = line.at(i);

        }
        y++;
    }
}

void renderWin()
{
    for (int y = 0; y < 65; y++)
    {
        for (int x = 0; x < 300; x++)
        {
            if (map[y][x] == '#')
            {
                g_Console.writeToBuffer(x, y,' ', 0x88);
            }
            else if (map[y][x] == '*')
            {
                g_Console.writeToBuffer(x, y, ' ', 0x77);
            }
            else //empty space
            {
                g_Console.writeToBuffer(x, y, ' ', 0x00);
            }
        }
    }
}

//--------------------------------------------------------------
// Purpose  : function is to render the player attack projectiles
// Input    : void
// Output   : void
// done by luke
//--------------------------------------------------------------
void renderBullets()
{
    for (int i = 0; i < 100; i++)
    {
        if (bulletArray[i] != nullptr)
        {
            if (map[bulletArray[i]->Y][bulletArray[i]->X] != '#')
            {
                bulletArray[i]->print();
            }
        }
    }
}

//--------------------------------------------------------------
// Purpose  : function is to check for which map to render first
// Input    : void
// Output   : void
// done by luke
//--------------------------------------------------------------
void loadmap()
{
    //check whether the map has been cleared to decide whether to load the next map
    if (map1Clear != true)
    {
        //Input stream class to operate on files.
        //Objects of this class maintain a filebuf object as their internal stream buffer, which performs input / output operations on the file they are associated with
        std::ifstream Lv1("MapLv1.txt");
        //make a string var to hold the chars
        std::string line;
        // Init and store Map
        int y = 0;
        while (getline(Lv1, line))
        {
            // Output the text from the file
            for (unsigned i = 0; i < line.length(); ++i)
            {
                map[y][i] = line.at(i);
            }
            y++;
        }
    }
    else
    {
        std::ifstream Lv2("MapLv2.txt");
        std::string line;
        // Init and store Map
        int y = 0;
        while (getline(Lv2, line))
        {
            // Output the text from the file
            for (unsigned i = 0; i < line.length(); ++i)
            {
                map[y][i] = line.at(i);
            }
            y++;
        }
    }
}

//--------------------------------------------------------------
// Purpose  : function is to render the map according to the symbols
// Input    : void
// Output   : void
// Myron
//--------------------------------------------------------------
void renderMap()
{
    //render Map according to the chars inside the text file
    for (int y = 0; y < 65; y++)
    {
        for (int x = 0; x < 300; x++)
        {
            if (map[y][x] == '|') //torch stick, can pass thru
            {
                g_Console.writeToBuffer(x, y, (char)186, 0x80);
            }
            else if (map[y][x] == '#') //wall, cannot pass
            {
                g_Console.writeToBuffer(x, y, (char)219, 0x80);
            }
            else if (map[y][x] == '+') //torch fire, pass thru
            {
                g_Console.writeToBuffer(x, y, (char)178, 0x84);
            }
            else if (map[y][x] == '[') //wall, cannot pass
            {
                g_Console.writeToBuffer(x, y, (char)222, 0x80);
            }
            else if (map[y][x] == ']') //wall, cannot pass
            {
                g_Console.writeToBuffer(x, y, (char)221, 0x80);
            }
            else if (map[y][x] == '(') //trap firing block, cannot pass
            {
                g_Console.writeToBuffer(x, y, (char)204, 0x84);
            }
            else if (map[y][x] == ')') //trap firing block, cannot pass
            {
                g_Console.writeToBuffer(x, y, (char)185, 0x84);
            }
            else if (map[y][x] == '=') //wall, cannot pass
            {
                g_Console.writeToBuffer(x, y, (char)254, 0x80);
            }
            else if (map[y][x] == '%') //chest, can pass and gives item
            {
                g_Console.writeToBuffer(x, y, (char)233, 0x8E);
            }
            else if (map[y][x] == '!') //lava trap, can pass with DoT (2/s)
            {
                g_Console.writeToBuffer(x, y, (char)177, 0x8C);
            }
            else if (map[y][x] == '$') //Dungeon gate, can pass
            {
                g_Console.writeToBuffer(x, y, (char)215, 0x86);
            }
            else if (map[y][x] == '-') //wall, cannot pass
            {
                g_Console.writeToBuffer(x, y, (char)205, 0x84);
            }
            else if (map[y][x] == '`') //wall, cannot pass
            {
                g_Console.writeToBuffer(x, y, (char)254, 0x84);
            }
            else if (map[y][x] == '*') //wall, cannot pass
            {
                g_Console.writeToBuffer(x, y, (char)206, 0x84);
            }
            else if (map[y][x] == '<') //tp gate step, pass thru
            {
                g_Console.writeToBuffer(x, y, (char)243, 0x8B);
            }
            else if (map[y][x] == '>') //tp gate step, pass thru
            {
                g_Console.writeToBuffer(x, y, (char)242, 0x8B);
            }
            else if (map[y][x] == '~') //tp gate, teleports to next room
            {
                g_Console.writeToBuffer(x, y, (char)234, 0x8B);
            }
            else if (map[y][x] == '0') //firetrap's fire, pass with DoT (10/s)
            {
                g_Console.writeToBuffer(x, y, '-', 0x84);
            }
            else if (map[y][x] == '1') //firetrap's fire, pass with DoT (10/s)
            {
                g_Console.writeToBuffer(x, y, '=', 0x84);
            }
            else if (map[y][x] == '2') //firetrap's fire, pass with DoT (10/s)
            {
                g_Console.writeToBuffer(x, y, '#', 0x84);
            }
            else if (map[y][x] == '3') //firetrap's fire, pass with DoT (10/s)
            {
                g_Console.writeToBuffer(x, y, '@', 0x84);
            }
            else if (map[y][x] == '&') //Checkpoint, sets player spawn pos
            {
                g_Console.writeToBuffer(x, y, (char)237, 0x8B);
            }
            else if (map[y][x] == '"') //book, ends game
            {
                g_Console.writeToBuffer(x, y, (char)240, 0x8B);
            }
            else //empty space
            {
                g_Console.writeToBuffer(x, y, ' ', 0x80);
            }
        }
    }
}

//--------------------------------------------------------------
// Purpose  : function is to render the options in starting screen
// Input    : void
// Output   : void
//--------------------------------------------------------------
void renderStartOptions()
{
    COORD c = g_Console.getConsoleSize();
    c.Y = (c.Y / 20);
    c.X = c.X / 3;

    COORD cSTART = { c.X, c.Y + 25 };
    COORD cQUIT = { c.X, c.Y + 28 };

    WORD STARTcolour = 0x0F;
    WORD QUITcolour = 0x0F;
    
    std::string START = "> PRESS SPACE TO START <";
    std::string QUIT = "> PRESS ESC TO QUIT <";

    for (int x = cSTART.X; x < cSTART.X + START.length(); x++)
    {
        if (g_mouseEvent.mousePosition.X == x && g_mouseEvent.mousePosition.Y == cSTART.Y)
            STARTcolour = 0x0c;
    }
    for (int x = cQUIT.X; x < cQUIT.X + QUIT.length(); x++)
    {
        if (g_mouseEvent.mousePosition.X == x && g_mouseEvent.mousePosition.Y == cQUIT.Y)
            QUITcolour = 0x0c;
    }

    g_Console.writeToBuffer(cSTART, START, STARTcolour, START.length());
    g_Console.writeToBuffer(cQUIT, QUIT, QUITcolour, QUIT.length());
}

//--------------------------------------------------------------
// Purpose  : function is to render the base of the pause screen
// Input    : void
// Output   : void
//--------------------------------------------------------------
void renderPauseBase()
{
    COORD c = g_Console.getConsoleSize();
    c.X = c.X / 4;
    c.Y = c.Y / 3;

    for (int i = 0; i < 15; i++)
    {
        for (int i = 0; i < c.X; i++)
        {
            g_Console.writeToBuffer(c.X + i, c.Y, ' ', 0);
        }
        c.Y++;
    }
}

//--------------------------------------------------------------
// Purpose  : function is to render the pause screen options
// Input    : void
// Output   : void
//--------------------------------------------------------------
void renderPauseOptions()
{
    COORD c = g_Console.getConsoleSize();
    c.Y = (c.Y / 25);
    c.X = c.X / 3;

    WORD CONTINUEcolour = 0x0f;
    WORD QUITcolour = 0x0f;

    COORD cCONTINUE = { c.X, c.Y + 25 };
    COORD cQUIT = { c.X, c.Y + 28 };

    std::string CONTINUE = "> PRESS ESC TO CONTINUE <";
    std::string QUIT = "> PRESS Q TO QUIT <";

    for (int x = cCONTINUE.X; x < cCONTINUE.X + CONTINUE.length(); x++)
    {
        if (g_mouseEvent.mousePosition.X == x && g_mouseEvent.mousePosition.Y == cCONTINUE.Y)
            CONTINUEcolour = 0x0c;
    }

    for (int x = cQUIT.X; x < cQUIT.X + QUIT.length(); x++)
    {
        if (g_mouseEvent.mousePosition.X == x && g_mouseEvent.mousePosition.Y == cQUIT.Y)
            QUITcolour = 0x0c;
    }

    g_Console.writeToBuffer(cCONTINUE, CONTINUE, CONTINUEcolour, CONTINUE.length());
    g_Console.writeToBuffer(cQUIT, QUIT, QUITcolour, QUIT.length());
}

//--------------------------------------------------------------
// Purpose  : function is to render the lose screen options
// Input    : void
// Output   : void
//--------------------------------------------------------------
void renderLossOptions()
{
    /*for (int y = 0; y < 65; y++) // renders bg
    {
        for (int x = 0; x < 300; x++)
        {
            g_Console.writeToBuffer(x, y, ' ', 0);
        }
    }*/
    loadLosescreen();
    COORD c = g_Console.getConsoleSize();
    c.Y = (c.Y / 20);
    c.X = c.X / 10;

    WORD RETRYcolour = 0x0f;
    WORD QUITcolour = 0x0f;

    COORD cLOST = { c.X + 80, c.Y + 48 };
    COORD cRETRY = { c.X + 80, c.Y + 51 };
    COORD cQUIT = { c.X + 80, c.Y + 54 };

    std::string LOST = "YOU LOST :(";
    std::string RETRY = "> PRESS SPACE TO RETRY <";
    std::string QUIT = "> PRESS Q TO QUIT <";

    for (int x = cRETRY.X; x < cRETRY.X + RETRY.length(); x++)
    {
        if (g_mouseEvent.mousePosition.X == x && g_mouseEvent.mousePosition.Y == cRETRY.Y)
            RETRYcolour = 0x0c;
    }

    for (int x = cQUIT.X; x < cQUIT.X + QUIT.length(); x++)
    {
        if (g_mouseEvent.mousePosition.X == x && g_mouseEvent.mousePosition.Y == cQUIT.Y)
            QUITcolour = 0x0c;
    }

    g_Console.writeToBuffer(cLOST, LOST, 0x0f, LOST.length());
    g_Console.writeToBuffer(cRETRY, RETRY, RETRYcolour, RETRY.length());
    g_Console.writeToBuffer(cQUIT, QUIT, QUITcolour, QUIT.length());
}

//--------------------------------------------------------------
// Purpose  : function is to render the graphical user interface
// Input    : void
// Output   : void
//--------------------------------------------------------------
void renderGUI() // render game user inferface
{
    std::string currentObjective = "Escape the Dungeon";
    if (player->getX() > 20)
        currentObjective = "Get to the portal";
    if (map1Clear)
        currentObjective = "Kill the boss";

    for (int i = 0; i < 9; i++)
    {
        if (!bossParticles[i].m_bActive)
            currentObjective = "Read the diary";
    }

    std::string objective = "Objective: " + currentObjective;
    std::string lifeBar = "Lives: " + std::to_string(player->getLives());
    std::string healthBar = "Health: " + std::to_string(player->getHealth()) + "/" + std::to_string(player->getMaxHealth());
    std::string inventoryList = "Inventory: ";
    std::string tempStr;

    WORD healthColour = 0xAF;
    WORD healthBarColor = 0xAA;

    if (player->getHealth() <= 75)
    {
        healthColour = 14;
        healthBarColor = 0xEE;
    }
    if (player->getHealth() <= 50)
    {
        healthColour = 12;
        healthBarColor = 0xCC;
    }

    if (player->getHealth() <= 25)
    {
        healthColour = 4;
        healthBarColor = 0x44;
    }

    // yellow = 14, light red = 12, red = 4
    g_Console.writeToBuffer(1, 1, objective, 0x0f, objective.length());
    g_Console.writeToBuffer(1, 2, lifeBar, 0x0f, lifeBar.length());
    g_Console.writeToBuffer(50, 1, healthBar, healthColour, healthBar.length());
    // health bar
    for (int i = 0; i < player->getHealth(); i++)
    {
        g_Console.writeToBuffer(70 + i, 1, ' ', healthBarColor);
    }

    g_Console.writeToBuffer(1, 4, inventoryList, 0x0f, inventoryList.length());

    for (int i = 0; i < 5; i++) // print inventory contents
    {
        if (player->getInventory(i) != nullptr)
            tempStr = std::to_string(i + 1) + ". " + player->getInventory(i)->getName();
        else
            tempStr = std::to_string(i + 1) + ".";

        g_Console.writeToBuffer(1, 5 + i, tempStr, 0x0f, tempStr.length());
    }
}

//--------------------------------------------------------------
// Purpose  : function is to check for player interactions
// Input    : void
// Output   : void
// Tze Ting, Luke, Myron
//--------------------------------------------------------------
void playerInteractions()
{
    for (int i = 0; i < 10; i++) // player interacts with a chest // coded by: Sze Ting

    {
        if (chest[i] != nullptr)
        {
            if (player->getX() == chest[i]->getX() && player->getY() == chest[i]->getY()) // check for same position
            {
                for (int j = 0; j < 5; j++) // loop through inventory to check which is free
                {
                    if (player->getInventory(j) == nullptr)
                    {
                        int randNum = (rand() % 4) + 1; // randomise consumable gift
                        delete chest[i];
                        chest[i] = nullptr;
                        switch (randNum)
                        {
                        case 1:
                        {
                            player->setInventory(j, new HealthPotion);
                            break;
                        }
                        case 2:
                        {
                            player->setInventory(j, new ExtraLife);
                            break;
                        }
                        case 3:
                        {
                            player->setInventory(j, new OddPotion);
                            break;
                        }
                        case 4:
                        {
                            player->setInventory(j, new Cheese);
                            break;
                        }
                        }
                        break; // no need for further check
                    }
                }
            }
        }
    }

    //trap interaction
    if (map[player->getY()][player->getX()] == '!')
    {
        player->setHealth(player->getHealth() - 2);
    }

    if ((map[player->getY()][player->getX()] == '0') ||
        (map[player->getY()][player->getX()] == '1') ||
        (map[player->getY()][player->getX()] == '2') ||
        (map[player->getY()][player->getX()] == '3'))
    {
        player->setHealth(player->getHealth() - 5);
    }
    //Enemy interaction
    stalkerReachPlayer();
    projReachPlayer();

    // checkpoint
    if (map[player->getY()][player->getX()] == '&')
    {
        player->setSpawnPoint(player->getX(), player->getY());
    }

    //portal
    if (map[player->getY()][player->getX()] == '~')
    {
        map1Clear = true;
        g_Console.clearBuffer();
        gameInit();
    }

    //book
    if (map[player->getY()][player->getX()] == '"') // coded by: Sze Ting
    {
        g_eGameState = S_VICTORY;
    }
}

//--------------------------------------------------------------
// Purpose  : function is to render the map after interaction
// Input    : void
// Output   : void
// Luke
//--------------------------------------------------------------
void renderInteractions()
{
    // trap interaction
    if (map[player->getY()][player->getX()] == '!')
    {
        g_Console.writeToBuffer(player->getX() - 5, player->getY() - 1, "Wah Pain Lah!");
    }
    // checkpoint
    if (map[player->getY()][player->getX()] == '&')
    {
        g_Console.writeToBuffer(player->getX() - 10, player->getY() - 1, "YAY Checkpoint!!!");
    }
    // chests
    for (int i = 0; i < 10; i++) // player interacts with a chest
    {
        if (chest[i] != nullptr)
        {
            if (player->getX() - chest[i]->getX() <= 1 && player->getX() - chest[i]->getX() >= -1 &&
                player->getY() - chest[i]->getY() <= 1 && player->getY() - chest[i]->getY() >= -1) // check for close proximity
            {
                g_Console.writeToBuffer(player->getX() - 10, player->getY() - 1, "WOW A CHEST COOL!!!");
            }
        }
    }
}

//--------------------------------------------------------------
// Purpose  : function is to render the player
// Input    : void
// Output   : void
//--------------------------------------------------------------
void renderCharacter()
{
    // Draw the location of the character // was 1 for char
    g_Console.writeToBuffer(player->getPosition(), (char)146, player->getCharColour());
}

//--------------------------------------------------------------
// Purpose  : function is to render the help screen
// Input    : void
// Output   : void
//--------------------------------------------------------------
void renderHelp()
{
    COORD size = g_Console.getConsoleSize();

    for (int i = 0; i < size.Y; i++)
    {
        for (int x = 0; x < size.X; x++)
        {
            g_Console.writeToBuffer(x, i, " ", 0);
        }
    }

    std::string fire = "-=#@";

    g_Console.writeToBuffer(size.X / 4, size.Y / 4+2, (char)146, 0x34); // player
    g_Console.writeToBuffer(size.X / 4 + 5, size.Y / 4 + 2, " = player", 0x0f, 10);

    g_Console.writeToBuffer(size.X / 4, size.Y / 4 + 4, (char)204, 0x84); // trap
    g_Console.writeToBuffer(size.X / 4 + 1, size.Y / 4 + 4, (char)185, 0x84);
    g_Console.writeToBuffer(size.X / 4 + 5, size.Y / 4 + 4, " = trap", 0x0f, 8);

    g_Console.writeToBuffer(size.X / 4, size.Y / 4+6, (char)233, 0x8E); // chest
    g_Console.writeToBuffer(size.X / 4 + 5, size.Y / 4 + 6, " = chest", 0x0f, 9);

    g_Console.writeToBuffer(size.X / 4, size.Y / 4+8, (char)177, 0x8C); // lava
    g_Console.writeToBuffer(size.X / 4 + 5, size.Y / 4 + 8, " = lava", 0x0f, 8);

    g_Console.writeToBuffer(size.X / 4, size.Y / 4+10, (char)215, 0x86); // dungeon gate
    g_Console.writeToBuffer(size.X / 4 + 5, size.Y / 4 + 10, " = dungeon gate", 0x0f, 16);

    g_Console.writeToBuffer(size.X / 4, size.Y / 4 + 12, (char)243, 0x8b); // teleporter
    g_Console.writeToBuffer(size.X / 4 + 1, size.Y / 4 + 12, (char)234, 0x8b);
    g_Console.writeToBuffer(size.X / 4 + 2, size.Y / 4 + 12, (char)242, 0x8b);
    g_Console.writeToBuffer(size.X / 4 + 5, size.Y / 4 + 12, " = teleporter", 0x0f, 14);

    g_Console.writeToBuffer(size.X / 4, size.Y / 4 + 14, fire, 0x84, fire.length()); // fire
    g_Console.writeToBuffer(size.X / 4 + 5, size.Y / 4 + 14, " = fire", 0x0f, 8);

    g_Console.writeToBuffer(size.X / 4, size.Y / 4 + 16, (char)237, 0x8b); // checkpoint
    g_Console.writeToBuffer(size.X / 4 + 5, size.Y / 4 + 16, " = checkpoint", 0x0f, 14);

    g_Console.writeToBuffer(size.X / 4, size.Y / 4 + 18, (char)243, 0x8b);  //book
    g_Console.writeToBuffer(size.X / 4 + 1, size.Y / 4 + 18, (char)240, 0x8b);
    g_Console.writeToBuffer(size.X / 4 + 2, size.Y / 4 + 18, (char)242, 0x8b);
    g_Console.writeToBuffer(size.X / 4 + 5, size.Y / 4 + 18, " = book", 0x0f, 8);


}

//--------------------------------------------------------------
// Purpose  : function is to render the enemies
// Input    : void
// Output   : void
//-------------------------------------------------------------
void renderEnemies(SGameChar EArr[12], int charnum, WORD Colour)
{

    for (int i = 0; i < 12; i++)
    {
        if (EArr[i].m_bActive == true)
        {
            g_Console.writeToBuffer(EArr[i].m_cLocation, (char)charnum, Colour);
        }
    }

    if (p1.m_bActive == true)
        g_Console.writeToBuffer(p1.m_cLocation, (char)pnum, pcolor);

    if (p2.m_bActive == true)
        g_Console.writeToBuffer(p2.m_cLocation, (char)pnum, pcolor);

    if (p3.m_bActive == true)
        g_Console.writeToBuffer(p3.m_cLocation, (char)pnum, pcolor);

    if (p4.m_bActive == true)
        g_Console.writeToBuffer(p4.m_cLocation, (char)pnum, pcolor);

    if (p5.m_bActive == true)
        g_Console.writeToBuffer(p5.m_cLocation, (char)pnum, pcolor);

    if (pro1.m_bActive == true)
        g_Console.writeToBuffer(pro1.m_cLocation, (char)projnum, projColor);

    if (pro2.m_bActive == true)
        g_Console.writeToBuffer(pro2.m_cLocation, (char)projnum, projColor);

    if (pro3.m_bActive == true)
        g_Console.writeToBuffer(pro3.m_cLocation, (char)projnum, projColor);

    if (pro4.m_bActive == true)
        g_Console.writeToBuffer(pro4.m_cLocation, (char)projnum, projColor);

    if (pro5.m_bActive == true)
        g_Console.writeToBuffer(pro5.m_cLocation, (char)projnum, projColor);

    if (bpro.m_bActive == true)
        g_Console.writeToBuffer(bpro.m_cLocation, (char)projnum, projColor);

    if (bpro2.m_bActive == true)
        g_Console.writeToBuffer(bpro2.m_cLocation, (char)projnum, projColor);

}

//--------------------------------------------------------------
// Purpose  : function is to render the boss
// Input    : void
// Output   : void
//--------------------------------------------------------------
void renderBoss()
{
    WORD bossColor = 0x5E;
    WORD bossCorner = 0x4D;
    for (int i = 0; i < 9; i++)
    {
        if (bossParticles[i].m_bActive == true)
            if (i == 0 || i == 2 || i == 6 || i == 8)
            {
                // sets the colours for the 4 corners of boss to red
                g_Console.writeToBuffer(bossParticles[i].m_cLocation, (char)43, bossCorner);
                switch (i)
                {
                    // switch case to change corner colours once boss hp decreases to a certain amount
                case 0:
                    if (bossHp.hp <= 180)
                        g_Console.writeToBuffer(bossParticles[0].m_cLocation, (char)43, bossColor);
                    break;
                case 2:
                    if (bossHp.hp <= 120)
                        g_Console.writeToBuffer(bossParticles[2].m_cLocation, (char)43, bossColor);
                    break;
                case 6:
                    if (bossHp.hp <= 60)
                        g_Console.writeToBuffer(bossParticles[6].m_cLocation, (char)43, bossColor);
                    break;
                case 8:
                    if (bossHp.hp <= 0)
                        g_Console.writeToBuffer(bossParticles[8].m_cLocation, (char)43, bossColor);
                    break;
                }
            }
            else
            {   // all remaining parts are coloured purple
                g_Console.writeToBuffer(bossParticles[i].m_cLocation, (char)43, bossColor);
            }
    }

}

//--------------------------------------------------------------
// Purpose  : function is to render the enemy attacks
// Input    : void
// Output   : void
//--------------------------------------------------------------
char renderProj()
{
    // spawn proj in the matched direction
    switch (phantomSearchPlayer())
    {
    case 'r':
        pro1.m_bActive = true;
        pro1.m_cLocation.X = p1.m_cLocation.X;
        pro1.m_cLocation.Y = p1.m_cLocation.Y - 1;
        return 't';
        break; // right
    }
}

char renderProj2()
{
    switch (phantomSearchPlayer2())
    {
    case 'r':
        pro2.m_bActive = true;
        pro2.m_cLocation.X = p2.m_cLocation.X;
        pro2.m_cLocation.Y = p2.m_cLocation.Y - 1;
        return 't';
        break; // right
    }
}

char renderProj3()
{
    switch (phantomSearchPlayer3())
    {
    case 'r':
        pro3.m_bActive = true;
        pro3.m_cLocation.X = p3.m_cLocation.X;
        pro3.m_cLocation.Y = p3.m_cLocation.Y - 1;
        return 't';
        break; // top side
    }
}

char renderProj4()
{
    switch (phantomSearchPlayer4())
    {
    case 'r':
        pro4.m_bActive = true;
        pro4.m_cLocation.X = p4.m_cLocation.X;
        pro4.m_cLocation.Y = p4.m_cLocation.Y - 1;
        return 't';
        break; // top side
    }
}

char renderProj5()
{
    switch (phantomSearchPlayer5())
    {
    case 'r':
        pro5.m_bActive = true;
        pro5.m_cLocation.X = p5.m_cLocation.X;
        pro5.m_cLocation.Y = p5.m_cLocation.Y - 1;
        return 't';
        break; // top side
    }
}

//--------------------------------------------------------------
// Purpose  : function is to render the frame rate
// Input    : void
// Output   : void
//--------------------------------------------------------------
void renderFramerate()
{
    COORD c;
    // displays the framerate
    std::ostringstream ss;
    ss << std::fixed << std::setprecision(3);
    ss << 1.0 / g_dDeltaTime << "fps";
    c.X = g_Console.getConsoleSize().X - 9;
    c.Y = 0;
    g_Console.writeToBuffer(c, ss.str());

    // displays the elapsed time
    ss.str("");
    ss << g_dElapsedTime << "secs";
    c.X = 0;
    c.Y = 0;
    g_Console.writeToBuffer(c, ss.str(), 0x59);
}

// this is an example of how you would use the input events
void renderInputEvents()
{
    // keyboard events
    // W, A ,S, D text show position
    //COORD startPos = {50, 2};

    COORD startPos = { 210, 2 };

    std::ostringstream ss;
    std::string key;
    for (int i = 0; i < K_COUNT; ++i)
    {
        ss.str("");
        switch (i)
        {
        case K_W: key = "W";
            break;
        case K_S: key = "S";
            break;
        case K_A: key = "A";
            break;
        case K_D: key = "D";
            break;
        case K_SPACE: key = "SPACE";
            break;
        default: continue;
        }
        if (g_skKeyEvent[i].keyDown)
            ss << key << " pressed";
        else if (g_skKeyEvent[i].keyReleased)
            ss << key << " released";
        else
            ss << key << " not pressed";

        COORD c = { startPos.X, startPos.Y + i };
        g_Console.writeToBuffer(c, ss.str(), 0x17);
    }

    // mouse events    
    ss.str("");
    ss << "Mouse position (" << g_mouseEvent.mousePosition.X << ", " << g_mouseEvent.mousePosition.Y << ")";
    g_Console.writeToBuffer(g_mouseEvent.mousePosition, ss.str(), 0x59);
    ss.str("");
    switch (g_mouseEvent.eventFlags)
    {
    case 0:
        if (g_mouseEvent.buttonState == FROM_LEFT_1ST_BUTTON_PRESSED)
        {
            ss.str("Left Button Pressed");
            g_Console.writeToBuffer(g_mouseEvent.mousePosition.X, g_mouseEvent.mousePosition.Y + 1, ss.str(), 0x59);
        }
        else if (g_mouseEvent.buttonState == RIGHTMOST_BUTTON_PRESSED)
        {
            ss.str("Right Button Pressed");
            g_Console.writeToBuffer(g_mouseEvent.mousePosition.X, g_mouseEvent.mousePosition.Y + 2, ss.str(), 0x59);
        }
        else
        {
            ss.str("Some Button Pressed");
            g_Console.writeToBuffer(g_mouseEvent.mousePosition.X, g_mouseEvent.mousePosition.Y + 3, ss.str(), 0x59);
        }
        break;
    case DOUBLE_CLICK:
        ss.str("Double Clicked");
        g_Console.writeToBuffer(g_mouseEvent.mousePosition.X, g_mouseEvent.mousePosition.Y + 4, ss.str(), 0x59);
        break;
    case MOUSE_WHEELED:
        if (g_mouseEvent.buttonState & 0xFF000000)
            ss.str("Mouse wheeled down");
        else
            ss.str("Mouse wheeled up");
        g_Console.writeToBuffer(g_mouseEvent.mousePosition.X, g_mouseEvent.mousePosition.Y + 5, ss.str(), 0x59);
        break;
    default:
        break;
    }

}

//--------------------------------------------------------------
// Purpose  : function is to initalize the enemies and behaviours
// Input    : void
// Output   : void
//--------------------------------------------------------------
void setStalkerCoords(SGameChar EArr[12])
{

    int X, Y;

    for (int i = 0; i < 12; i++)
    {
        while (true)
        {

            switch (i)
            {
            case 0:
                X = 70;
                Y = 10;
                break;
            case 1:
                X = 70;
                Y = 30;
                break;
            case 2:
                X = 50;
                Y = 40;
                break;
            case 3:
                X = 30;
                Y = 40;
                break;
            case 4:
                X = 70;
                Y = 50;
                break;
            case 5:
                X = 80;
                Y = 50;
                break;
            case 6:
                X = 90;
                Y = 50;
                break;
            case 7:
                X = 130;
                Y = 30;
                break;
            case 8:
                X = 165;
                Y = 25;
                break;
            case 9:
                X = 170;
                Y = 15;
                break;
            case 10:
                X = 130;
                Y = 35;
                break;
            case 11:
                X = 160;
                Y = 35;
                break;
            }
            EArr[i].m_cLocation.X = X;
            EArr[i].m_cLocation.Y = Y;
            EArr[i].m_bActive = true;
            break;

        }
    }
}
// makes up the 3x3 boss cube and is able to set position to any part of map
void bossBodyCoord(SGameChar BArr[9], int x, int y)
{
    int count = 0;

    for (int i = 0; i < 9; i++)
    {
        BArr[0].m_cLocation.X = x;
        BArr[0].m_cLocation.Y = y;
        count++;
        if (i > 0)
        {
            BArr[i].m_cLocation.X = BArr[0].m_cLocation.X + count - 1;
            BArr[i].m_cLocation.Y = BArr[0].m_cLocation.Y;
        }
        if (i > 2)
        {
            BArr[i].m_cLocation.X = BArr[0].m_cLocation.X;
            BArr[i].m_cLocation.Y = BArr[0].m_cLocation.Y + 1;
        }
        if (i > 3)
        {
            BArr[i].m_cLocation.X = BArr[0].m_cLocation.X + count - 4;
            BArr[i].m_cLocation.Y = BArr[0].m_cLocation.Y + 1;
        }
        if (i > 5)
        {
            BArr[i].m_cLocation.X = BArr[0].m_cLocation.X - count + 9;
            BArr[i].m_cLocation.Y = BArr[0].m_cLocation.Y + 2;
        }
        BArr[i].m_bActive = true;
    }
}

// to initialise boss bullet position
void bossInitAtk(int x, int y)
{
    bpro.m_cLocation.X = x;
    bpro.m_cLocation.Y = y;
}

void bossMovement(SGameChar BArr[9])
{
    // if boss found player, and has not fired a bullet, initialises bullet to boss position
    // and fires a homing missile till it hits the player
    bossSearchPlayer(bossParticles);
    if ((bossSearchPlayer(bossParticles) == 'y') && (bossProjCount == 0))
    {
        bossInitAtk(bpro.m_cLocation.X = bossParticles[1].m_cLocation.X, bpro.m_cLocation.Y = bossParticles[1].m_cLocation.Y - 1);
    }
    if (player->getX() > bpro.m_cLocation.X)
    {
        bpro.m_cLocation.X++;
    }
    else if (player->getX() < bpro.m_cLocation.X)
    {
        bpro.m_cLocation.X--;
    }
    else if (player->getY() < bpro.m_cLocation.Y)
    {
        bpro.m_cLocation.Y--;
    }
    else if (player->getY() > bpro.m_cLocation.Y)
    {
        bpro.m_cLocation.Y++;
    }
}

// function to see if player is in range of sight
char bossSearchPlayer(SGameChar BArr[9])
{
    int X, Y;
    for (int i = 0; i < 9; i++)
    {
        X = player->getX() - BArr[i].m_cLocation.Y;
        Y = player->getY() - BArr[i].m_cLocation.X;
        if (X < 0)
        {
            X *= -1;
        }
        if (Y < 0)
        {
            Y *= -1;
        }
        if (X < 20 && Y < 10)
        {
            return 'y';
        }

        return 'n';
    }
}

// checks if boss is dead
void bossDeath()
{
    if (bossHp.hp < 0)
        for (int i = 0; i < 9; i++)
        {
            // disable all boss related entities
            bossParticles[i].m_bActive = false;
            bpro.m_bActive = false;
        }
}

void phantomMovement()
{
    // random movement
    int dir = (rand() % 4) + 1;
    int x, y;
    x = p1.m_cLocation.X;
    y = p1.m_cLocation.Y;
    switch (dir)
    {
    case 1:
        if (p1.m_cLocation.X < g_Console.getConsoleSize().X - 1)
        {
            // check for collision
            if (map[y][x - 1] != '#')
                p1.m_cLocation.X--;
        }
        break;
    case 2:
        if (p1.m_cLocation.Y < g_Console.getConsoleSize().Y - 1)
        {
            if (map[y + 1][x] != '#')
                p1.m_cLocation.Y++;
        }
        break;
    case 3:
        if (p1.m_cLocation.Y > 0)
        {
            if (map[y - 1][x] != '#')
                p1.m_cLocation.Y--;
        }
        break;
    case 4:
        if (p1.m_cLocation.X > 0)
        {
            if (map[y][x + 1] != '#')
                p1.m_cLocation.X++;
        }
        break;
    }
    // if player in sight, fire projectile and render
    // projectile in returned side of projectile
    phantomSearchPlayer();
    phantomFireProj();
    if (renderProj() != 's')
    {
        pro1.m_cLocation.Y--;
    }
}

void phantomMovement2()
{
    int dir = (rand() % 4) + 1;
    int x, y;
    x = p2.m_cLocation.X;
    y = p2.m_cLocation.Y;
    switch (dir)
    {
    case 4:
        if (p2.m_cLocation.X < g_Console.getConsoleSize().X - 1)
        {
            if (map[y][x - 1] != '#')
                p2.m_cLocation.X--;
        }
        break;
    case 3:
        if (p2.m_cLocation.Y < g_Console.getConsoleSize().Y - 1)
        {
            if (map[y + 1][x] != '#')
                p2.m_cLocation.Y++;
        }
        break;
    case 2:
        if (p2.m_cLocation.Y > 0)
        {
            if (map[y - 1][x] != '#')
                p2.m_cLocation.Y--;
        }
        break;
    case 1:
        if (p2.m_cLocation.X > 0)
        {
            if (map[y][x + 1] != '#')
                p2.m_cLocation.X++;
        }
        break;
    }
    phantomSearchPlayer2();
    phantomFireProj2();
    if (renderProj2() != 's')
    {
        pro2.m_cLocation.Y--;
    }
}

void phantomMovement3()
{
    int dir = (rand() % 4) + 1;
    int x, y;
    x = p3.m_cLocation.X;
    y = p3.m_cLocation.Y;

    switch (dir)
    {
    case 2:
        if (p3.m_cLocation.X < g_Console.getConsoleSize().X - 1)
        {
            if (map[y][x - 1] != '#')
                p3.m_cLocation.X--;
        }
        break;
    case 3:
        if (p3.m_cLocation.Y < g_Console.getConsoleSize().Y - 1)
        {
            if (map[y + 1][x] != '#')
                p3.m_cLocation.Y++;
        }
        break;
    case 4:
        if (p3.m_cLocation.Y > 0)
        {
            if (map[y - 1][x] != '#')
                p3.m_cLocation.Y--;
        }
        break;
    case 1:
        if (p3.m_cLocation.X > 0)
        {
            if (map[y][x + 1] != '#')
                p3.m_cLocation.X++;
        }
        break;
    }
    phantomSearchPlayer3();
    phantomFireProj3();
    if (renderProj3() != 's')
    {
        pro3.m_cLocation.Y--;
    }
}

void phantomMovement4()
{
    int dir = (rand() % 4) + 1;
    int x, y;
    x = p4.m_cLocation.X;
    y = p4.m_cLocation.Y;
    switch (dir)
    {
    case 2:
        if (p4.m_cLocation.X < g_Console.getConsoleSize().X - 1)
        {
            if (map[y][x - 1] != '#')
                p4.m_cLocation.X--;
        }
        break;
    case 3:
        if (p4.m_cLocation.Y < g_Console.getConsoleSize().Y - 1)
        {
            if (map[y + 1][x] != '#')
                p4.m_cLocation.Y++;
        }
        break;
    case 1:
        if (p4.m_cLocation.Y > 0)
        {
            if (map[y - 1][x] != '#')
                p4.m_cLocation.Y--;
        }
        break;
    case 4:
        if (p4.m_cLocation.X > 0)
        {
            if (map[y][x + 1] != '#')
                p4.m_cLocation.X++;
        }
        break;
    }

    phantomSearchPlayer4();
    phantomFireProj4();
    if (renderProj4() != 's')
    {
        pro4.m_cLocation.Y--;
    }
}

void phantomMovement5()
{
    int dir = (rand() % 4) + 1;
    int x, y;
    x = p5.m_cLocation.X;
    y = p5.m_cLocation.Y;
    switch (dir)
    {
    case 3:
        if (p5.m_cLocation.X < g_Console.getConsoleSize().X - 1)
        {
            if (map[y][x - 1] != '#')
                p5.m_cLocation.X--;
        }
        break;
    case 4:
        if (p5.m_cLocation.Y < g_Console.getConsoleSize().Y - 1)
        {
            if (map[y + 1][x] != '#')
                p5.m_cLocation.Y++;
        }
        break;
    case 1:
        if (p5.m_cLocation.Y > 0)
        {
            if (map[y - 1][x] != '#')
                p5.m_cLocation.Y--;
        }
        break;
    case 2:
        if (p5.m_cLocation.X > 0)
        {
            if (map[y][x + 1] != '#')
                p5.m_cLocation.X++;
        }
        break;
    }

    phantomSearchPlayer5();
    phantomFireProj5();
    if (renderProj5() != 's')
    {
        pro5.m_cLocation.Y--;
    }
}

// checks if player is in sight of phantom
char phantomSearchPlayer()
{
    int dist = 30;
    if ((p1.m_cLocation.Y - dist <= player->getY())
        && (player->getX() == p1.m_cLocation.X))
    {
        // so bottom side will not follow
        if ((p1.m_cLocation.Y <= player->getY())
            && (p1.m_cLocation.X == player->getX()))
        {
            return 'n';
        }
        return 'r'; // if player is right side of phantom
    }
    else
    {
        return 'n';
    }
}

char phantomSearchPlayer2()
{
    int dist = 30;
    if ((p2.m_cLocation.Y - dist <= player->getY())
        && (player->getX() == p2.m_cLocation.X))
    {
        // so bottom side will not follow
        if ((p2.m_cLocation.Y <= player->getY())
            && (p2.m_cLocation.X == player->getX()))
        {
            return 'n';
        }
        return 'r'; // if player is right side of phantom
    }
    else
    {
        return 'n';
    }
}

char phantomSearchPlayer3()
{
    int dist = 30;
    if ((p3.m_cLocation.Y - dist <= player->getY())
        && (player->getX() == p3.m_cLocation.X))
    {
        // so bottom side will not follow
        if ((p3.m_cLocation.Y <= player->getY())
            && (p3.m_cLocation.X == player->getX()))
        {
            return 'n';
        }
        return 'r'; // if player is right side of phantom
    }
    else
    {
        return 'n'; // if not in sight
    }
}

char phantomSearchPlayer4()
{
    int dist = 30;
    if ((p4.m_cLocation.Y - dist <= player->getY())
        && (player->getX() == p4.m_cLocation.X))
    {
        // so bottom side will not follow
        if ((p4.m_cLocation.Y <= player->getY())
            && (p4.m_cLocation.X == player->getX()))
        {
            return 'n';
        }
        return 'r'; // if player is right side of phantom
    }
    else
    {
        return 'n';
    }
}

char phantomSearchPlayer5()
{
    int dist = 35;
    if ((p5.m_cLocation.Y - dist <= player->getY())
        && (player->getX() == p5.m_cLocation.X))
    {
        // so bottom side will not follow
        if ((p5.m_cLocation.Y <= player->getY())
            && (p5.m_cLocation.X == player->getX()))
        {
            return 'n';
        }
        return 'r'; // if player is right side of phantom
    }
    else
    {
        return 'n';
    }
}

// Renders projectile through initialising position if player in sight
void phantomFireProj()
{
    if (phantomSearchPlayer() != 'n')
    {
        renderProj();
    }
}

void phantomFireProj2()
{
    if (phantomSearchPlayer2() != 'n')
    {
        renderProj2();
    }
}

void phantomFireProj3()
{
    if (phantomSearchPlayer3() != 'n')
    {
        renderProj3();
    }
}

void phantomFireProj4()
{
    if (phantomSearchPlayer4() != 'n')
    {
        renderProj4();
    }
}

void phantomFireProj5()
{
    if (phantomSearchPlayer5() != 'n')
    {
        renderProj5();
    }
}

//--------------------------------------------------------------
// Purpose  : function is to check the enemy projectile collisions
// Input    : void
// Output   : void
//--------------------------------------------------------------

void projReachPlayer()
{
    // player Hp - 5 if hit by phantom bullet
    // phantom bullet resets
    if ((player->getX() == pro1.m_cLocation.X) && (player->getY() == pro1.m_cLocation.Y))
    {
        player->setHealth(player->getHealth() - 5);
        pro1.m_bActive = false;
    }
    else if ((player->getX() == pro2.m_cLocation.X) && (player->getY() == pro2.m_cLocation.Y))
    {
        player->setHealth(player->getHealth() - 5);
        pro2.m_bActive = false;
    }
    else if ((player->getX() == pro3.m_cLocation.X) && (player->getY() == pro3.m_cLocation.Y))
    {
        player->setHealth(player->getHealth() - 5);
        pro3.m_bActive = false;
    }
    else if ((player->getX() == pro4.m_cLocation.X) && (player->getY() == pro4.m_cLocation.Y))
    {
        player->setHealth(player->getHealth() - 5);
        pro4.m_bActive = false;
    }
    else if ((player->getX() == pro5.m_cLocation.X) && (player->getY() == pro5.m_cLocation.Y))
    {
        player->setHealth(player->getHealth() - 5);
        pro5.m_bActive = false;
    }
    else if ((player->getX() == bpro.m_cLocation.X) && (player->getY() == bpro.m_cLocation.Y))
    {
        // player Hp - 10 if hit by boss bullet
        if (bpro.m_bActive == true)
        {
            player->setHealth(player->getHealth() - 10);
        }
        // reset boss bullet count for next fire
        bossProjCount--;
        // reintialise bullet pos
        bossInitAtk(bpro.m_cLocation.X = bossParticles[1].m_cLocation.X, bpro.m_cLocation.Y = bossParticles[1].m_cLocation.Y - 1);
    }
}

// checkers for stalker movements, check all directions for collisions
void checkerY1(int i, int n) // y--
{
    int x = stalkers[i].m_cLocation.X;
    int y = stalkers[i].m_cLocation.Y;
    if (player->getY() < stalkers[i].m_cLocation.Y)
        if ((map[y + n][x] != '#') && // check for all collisions
            (map[y + n][x] != '=') &&
            (map[y + n][x] != '[') &&
            (map[y + n][x] != ']') &&
            (map[y + n][x] != ')') &&
            (map[y + n][x] != '(') &&
            (map[y + n][x] != '*') &&
            (map[y + n][x] != '`') &&
            (map[y + n][x] != 'S')) // collision between other enemies
        {
            stalkers[i].m_cLocation.Y += n;
        }
}

void checkerY2(int i, int n) // y++
{
    int x = stalkers[i].m_cLocation.X;
    int y = stalkers[i].m_cLocation.Y;
    if (player->getY() > stalkers[i].m_cLocation.Y)
        if ((map[y + n][x] != '#') &&
            (map[y + n][x] != '=') &&
            (map[y + n][x] != '[') &&
            (map[y + n][x] != ']') &&
            (map[y + n][x] != ')') &&
            (map[y + n][x] != '(') &&
            (map[y + n][x] != '*') &&
            (map[y + n][x] != '`') &&
            (map[y + n][x] != 'S'))
        {
            stalkers[i].m_cLocation.Y += n;
        }
}

void checkerX1(int i, int n) // x--
{
    int x = stalkers[i].m_cLocation.X;
    int y = stalkers[i].m_cLocation.Y;
    if (player->getX() < stalkers[i].m_cLocation.X)
        if ((map[y][x + n] != '#') &&
            (map[y][x + n] != '=') &&
            (map[y][x + n] != '[') &&
            (map[y][x + n] != ']') &&
            (map[y][x + n] != ')') &&
            (map[y][x + n] != '(') &&
            (map[y][x + n] != '*') &&
            (map[y][x + n] != '`') &&
            (map[y][x + n] != 'S'))
        {
            stalkers[i].m_cLocation.X += n;
        }
}

void checkerX2(int i, int n)// x++
{
    int x = stalkers[i].m_cLocation.X;
    int y = stalkers[i].m_cLocation.Y;
    if (player->getX() > stalkers[i].m_cLocation.X)
        if ((map[y][x + n] != '#') &&
            (map[y][x + n] != '=') &&
            (map[y][x + n] != '[') &&
            (map[y][x + n] != ']') &&
            (map[y][x + n] != ')') &&
            (map[y][x + n] != '(') &&
            (map[y][x + n] != '*') &&
            (map[y][x + n] != '`') &&
            (map[y][x + n] != 'S'))
        {
            stalkers[i].m_cLocation.X += n;
        }
}

void stalkerMovement(SGameChar EArr[12])
{
    // randomise movement
    int dir = (rand() % 4) + 1;
    int x, y;

    for (int i = 0; i < 12; i++)
    {
        x = EArr[i].m_cLocation.X;
        y = EArr[i].m_cLocation.Y;
        if (i % 2 == 0) // every even enemy moves differently
        {
            switch (dir)
            {
                //Down
            case 2:
                if ((EArr[i].m_cLocation.Y < g_Console.getConsoleSize().Y - 1) &&
                    (map[y + 1][x] != '#') &&
                    (map[y + 1][x] != '=') && // check collisions
                    (map[y + 1][x] != '[') &&
                    (map[y + 1][x] != ']') &&
                    (map[y + 1][x] != ')') &&
                    (map[y + 1][x] != '(') &&
                    (map[y + 1][x] != '*') &&
                    (map[y + 1][x] != '`') &&
                    (map[y + 1][x] != 'S'))
                {

                    EArr[i].m_cLocation.Y++;
                }
                break;
                //Right
            case 4:
                if ((EArr[i].m_cLocation.X < g_Console.getConsoleSize().X - 1) &&
                    (map[y][x - 1] != '#') &&
                    (map[y][x - 1] != '=') &&
                    (map[y][x - 1] != '[') &&
                    (map[y][x - 1] != ']') &&
                    (map[y][x - 1] != ')') &&
                    (map[y][x - 1] != '(') &&
                    (map[y][x - 1] != '*') &&
                    (map[y][x - 1] != '`') &&
                    (map[y][x - 1] != 'S'))
                {

                    EArr[i].m_cLocation.X--;
                }
                break;
                //Up
            case 3:
                if ((EArr[i].m_cLocation.Y > 0) &&
                    (map[y - 1][x] != '#') &&
                    (map[y - 1][x] != '=') &&
                    (map[y - 1][x] != '[') &&
                    (map[y - 1][x] != ']') &&
                    (map[y - 1][x] != ')') &&
                    (map[y - 1][x] != '(') &&
                    (map[y - 1][x] != '*') &&
                    (map[y - 1][x] != '`') &&
                    (map[y - 1][x] != 'S'))
                {

                    EArr[i].m_cLocation.Y--;
                }
                break;
                //Left
            case 1:
                if ((EArr[i].m_cLocation.X > 0) &&
                    (map[y][x + 1] != '#') &&
                    (map[y][x + 1] != '=') &&
                    (map[y][x + 1] != '[') &&
                    (map[y][x + 1] != ']') &&
                    (map[y][x + 1] != ')') &&
                    (map[y][x + 1] != '(') &&
                    (map[y][x + 1] != '*') &&
                    (map[y][x + 1] != '`') &&
                    (map[y][x + 1] != 'S'))
                {

                    EArr[i].m_cLocation.X++;
                }

                break;
            }
        }
        else // odd enemies movement
        {
            switch (dir)
            {
                //Up
            case 1:
                if ((EArr[i].m_cLocation.Y > 0) &&
                    (map[y - 1][x] != '#') &&
                    (map[y - 1][x] != '=') &&
                    (map[y - 1][x] != '[') &&
                    (map[y - 1][x] != ']') &&
                    (map[y - 1][x] != ')') &&
                    (map[y - 1][x] != '(') &&
                    (map[y - 1][x] != '*') &&
                    (map[y - 1][x] != '`') &&
                    (map[y - 1][x] != 'S'))
                {

                    EArr[i].m_cLocation.Y--;
                }

                break;
                //Left
            case 2:
                if ((EArr[i].m_cLocation.X > 0) &&
                    (map[y][x - 1] != '#') &&
                    (map[y][x - 1] != '=') &&
                    (map[y][x - 1] != '[') &&
                    (map[y][x - 1] != ']') &&
                    (map[y][x - 1] != ')') &&
                    (map[y][x - 1] != '(') &&
                    (map[y][x - 1] != '*') &&
                    (map[y][x - 1] != '`') &&
                    (map[y][x - 1] != 'S'))
                {

                    EArr[i].m_cLocation.X--;
                }

                break;
                //Down
            case 3:
                if ((EArr[i].m_cLocation.Y < g_Console.getConsoleSize().Y - 1) &&
                    (map[y + 1][x] != '#') &&
                    (map[y + 1][x] != '=') &&
                    (map[y + 1][x] != '[') &&
                    (map[y + 1][x] != ']') &&
                    (map[y + 1][x] != ')') &&
                    (map[y + 1][x] != '(') &&
                    (map[y + 1][x] != '*') &&
                    (map[y + 1][x] != '`') &&
                    (map[y + 1][x] != 'S'))
                {

                    EArr[i].m_cLocation.Y++;
                }

                break;
                //Right
            case 4:
                if ((EArr[i].m_cLocation.X < g_Console.getConsoleSize().X - 1) &&
                    (map[y][x + 1] != '#') &&
                    (map[y][x + 1] != '=') &&
                    (map[y][x + 1] != '[') &&
                    (map[y][x + 1] != ']') &&
                    (map[y][x + 1] != ')') &&
                    (map[y][x + 1] != '(') &&
                    (map[y][x + 1] != '*') &&
                    (map[y][x + 1] != '`') &&
                    (map[y][x + 1] != 'S'))
                {

                    EArr[i].m_cLocation.X++;
                }
                break;
            }
        }
        // check if player in sight then chase 
        stalkerSearchPlayer();
        stalkerChasePlayer();
    }
}

char stalkerSearchPlayer()
{
    int x_final, y_final;
    int maxNumOfSteps = 0;
    // assume 5x5 grid radius except for same lane:
    // check if the player and enemy are in the same lane
    for (int i = 0; i < 12; i++)
    {
        if ((player->getY() == stalkers[i].m_cLocation.Y) ||
            (player->getX() == stalkers[i].m_cLocation.X))
            if (player->getX() - stalkers[i].m_cLocation.X <= 2)
            {
                switch (i)  // switch case check for individual enemy
                {           
                case 0:
                    return 'a';
                    break;
                case 1:
                    return 'b';
                    break;
                case 2:
                    return 'c';
                    break;
                case 3:
                    return 'd';
                    break;
                case 4:
                    return 'e';
                    break;
                case 5:
                    return 'f';
                    break;
                case 6:
                    return 'g';
                    break;
                case 7:
                    return 'h';
                    break;
                case 8:
                    return 'i';
                case 9:
                    return 'j';
                    break;
                case 10:
                    return 'k';
                    break;
                case 11:
                    return 'l';
                    break;
                }
            }
        // check for other possibilities
            else
            {
                x_final = player->getX() - stalkers[i].m_cLocation.X;
                y_final = player->getY() - stalkers[i].m_cLocation.Y;
                // remove negative val
                if (x_final < 0)
                {
                    x_final *= -1;
                }
                if (y_final < 0)
                {
                    y_final *= -1;
                }
                // add together, if within the 5x5 grid, 
                // this calculation always garauntees a result 4 and less 
                // if player and stalker in same 5x5 grid
                maxNumOfSteps += x_final += y_final;
                if (maxNumOfSteps <= 4)
                {
                    switch (i) // switch case to check for individual enemy
                    {
                    case 0:
                        return 'a';
                        break;
                    case 1:
                        return 'b';
                        break;
                    case 2:
                        return 'c';
                        break;
                    case 3:
                        return 'd';
                        break;
                    case 4:
                        return 'e';
                        break;
                    case 5:
                        return 'f';
                        break;
                    case 6:
                        return 'g';
                        break;
                    case 7:
                        return 'h';
                        break;
                    case 8:
                        return 'i';
                        break;
                    case 9:
                        return 'j';
                        break;
                    case 10:
                        return 'k';
                        break;
                    case 11:
                        return 'l';
                        break;
                    }
                }
            }
    }
    return 'n';
    // if both conditions not met, 
    // player not in range of enemy
}

void stalkerChasePlayer()
{
    // if player in sight, check for individual stalkers and their collisions
    // in every x and y directions in the map
    if (stalkerSearchPlayer() != 'n')
        switch (stalkerSearchPlayer())
        {
        case 'a':
            checkerX1(0, -1);
            checkerX2(0, 1);
            checkerY1(0, -1);
            checkerY2(0, 1);
            break;
        case 'b':
            checkerX1(1, -1);
            checkerX2(1, 1);
            checkerY1(1, -1);
            checkerY2(1, 1);
            break;
        case 'c':
            checkerX1(2, -1);
            checkerX2(2, 1);
            checkerY1(2, -1);
            checkerY2(2, 1);
            break;
        case 'd':
            checkerX1(3, -1);
            checkerX2(3, 1);
            checkerY1(3, -1);
            checkerY2(3, 1);
            break;
        case 'e':
            checkerX1(4, -1);
            checkerX2(4, 1);
            checkerY1(4, -1);
            checkerY2(4, 1);
            break;
        case 'f':
            checkerX1(5, -1);
            checkerX2(5, 1);
            checkerY1(5, -1);
            checkerY2(5, 1);
            break;
        case 'g':
            checkerX1(6, -1);
            checkerX2(6, 1);
            checkerY1(6, -1);
            checkerY2(6, 1);
            break;
        case 'h':
            checkerX1(7, -1);
            checkerX2(7, 1);
            checkerY1(7, -1);
            checkerY2(7, 1);
            break;
        case 'i':
            checkerX1(8, -1);
            checkerX2(8, 1);
            checkerY1(8, -1);
            checkerY2(8, 1);
            break;

        case 'j':
            checkerX1(9, -1);
            checkerX2(9, 1);
            checkerY1(9, -1);
            checkerY2(9, 1);
            break;
        case 'k':
            checkerX1(10, -1);
            checkerX2(10, 1);
            checkerY1(10, -1);
            checkerY2(10, 1);
            break;
        case 'l':
            checkerX1(11, -1);
            checkerX2(11, 1);
            checkerY1(11, -1);
            checkerY2(11, 1);
            break;

        }
}

// if stalker hits player, sticks to player and causes damage consistently
void stalkerReachPlayer()
{
    for (int i = 0; i < 12; i++)
    {
        if ((player->getX() == stalkers[i].m_cLocation.X) && (player->getY() == stalkers[i].m_cLocation.Y))
        {
            if (stalkers[i].m_bActive == true)
                player->setHealth(player->getHealth() - 1);
            stalkers[i].m_cLocation.X = player->getX() + 1;
        }
    }
}

//--------------------------------------------------------------
// Purpose  : function is to let the player attack
// Input    : void
// Output   : void
// made by luke
//--------------------------------------------------------------
void shoot()
{
    for (int i = 0; i < 100; i++)
    {
        if (bulletArray[i] == nullptr)
        {
            bulletArray[i] = new Bullet(player->getX(), player->getY(), player->getDirection());
            return;
        }
    }
}

//--------------------------------------------------------------
// Purpose  : function is to set the bullet interaction
// Input    : void
// Output   : void
// made by luke
//--------------------------------------------------------------
void bulletInteraction()
{
    for (int i = 0; i < 100; i++)
    {
        if (bulletArray[i] != nullptr)
        {
            bulletArray[i]->updatebulletpos();
            int x, y;
            x = bulletArray[i]->getX();
            y = bulletArray[i]->getY();

            if (bulletArray[i]->X <= 0 || bulletArray[i]->X >= 300 ||
                bulletArray[i]->Y <= 0 || bulletArray[i]->Y >= 65 ||
                map[bulletArray[i]->Y][bulletArray[i]->X] == '#')
            {
                delete bulletArray[i];
                bulletArray[i] = nullptr;
            }
            // detect enemies
            for (int l = 0; l < 12; l++)
            {
                if ((stalkers[l].m_cLocation.X == x) && (stalkers[l].m_cLocation.Y == y))
                {
                    stalkers[l].m_bActive = false;
                    delete bulletArray[i];
                    bulletArray[i] = nullptr;
                    return;
                }
            }
            // detect boss, if corners hit, bossHp decreases
            for (int j = 0; j < 9; j++)
            {
                if (j == 0 || j == 2 || j == 6 || j == 8)
                {
                    if ((bossParticles[j].m_cLocation.X == x) && (bossParticles[j].m_cLocation.Y == y))
                    {
                        bossHp.hp -= 30;
                        delete bulletArray[i];
                        bulletArray[i] = nullptr;
                        return;
                    }
                }
            }
        }
    }
}


