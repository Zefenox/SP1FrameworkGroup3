﻿#include "game.h"
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
SGameChar stalkers[12] = { s1,s2,s3,s4,s5,s6,s7,s8,s9,s10, s11, s12};

//phantoms
SGameChar   p1, p2, p3, p4, p5;
// projectile of phantom
SGameChar pro1, pro2, pro3, pro4, pro5;
WORD projColor = 0x4D;
int projnum = 60;
// boss

SGameChar b1, b2, b3, b4, b5, b6, b7, b8, b9;
SGameChar bossParticles[9] = { b1,b2,b3,b4,b5,b6,b7,b8,b9 };
SGameChar bpro;
int bossHp = 150;

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

    // Enemy initial state
    bpro.m_bActive = true;
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
    p3.m_cLocation.Y = 50;
    p4.m_cLocation.X = 50;
    p4.m_cLocation.Y = 25;
    p5.m_cLocation.X = 150;
    p5.m_cLocation.Y = 45;
    setStalkerCoords(stalkers);
    bossBodyCoord(bossParticles, 180, 15);
 
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
    g_Console.clearBuffer();
    player->setSpawnPoint(11,11); // set spawn point
    player->setPosition(player->getSpawnPoint()); // spawn the player at his spawn point
    player->setLives(3);
    player->setMaxHealth(100);
    player->setHealth(player->getMaxHealth());
    player->setDirection('R');
    player->setCharColour(0x84);
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
    chest[6]->setPosition(203, 46);

    // for floor 2
    /*
    * chest[0] = new Chest; // initialise chest
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
    chest[9]->setPosition(98, 54);
    */
    
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
    case S_STARTSCREEN: startKBHandler(keyboardEvent);
        break;
    case S_GAME: gameplayKBHandler(keyboardEvent); // handle gameplay keyboard event 
        break;
    case S_PAUSESCREEN: pauseKBHandler(keyboardEvent); // handle pause screen keyboard event
        break;
    case S_LOSS: lossKBHandler(keyboardEvent); // handle loss screen keyboard event
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
    case S_STARTSCREEN: startMouseHandler(mouseEvent);
        break;
    case S_GAME: gameplayMouseHandler(mouseEvent); // handle gameplay mouse event
        break;
    case S_PAUSESCREEN: pauseMouseHandler(mouseEvent); // handle pause screen mouse event
        break;
    case S_LOSS: lossMouseHandler(mouseEvent); // handle pause screen mouse event
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
    case 0x4C: key = K_L; break;

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


void startMouseHandler(const MOUSE_EVENT_RECORD& mouseEvent)
{
    if (mouseEvent.dwEventFlags & MOUSE_MOVED) // update the mouse position if there are no events
    {
        g_mouseEvent.mousePosition = mouseEvent.dwMousePosition;
    }
    g_mouseEvent.buttonState = mouseEvent.dwButtonState;
    g_mouseEvent.eventFlags = mouseEvent.dwEventFlags;
}

void pauseMouseHandler(const MOUSE_EVENT_RECORD& mouseEvent)
{
    if (mouseEvent.dwEventFlags & MOUSE_MOVED) // update the mouse position if there are no events
    {
        g_mouseEvent.mousePosition = mouseEvent.dwMousePosition;
    }
    g_mouseEvent.buttonState = mouseEvent.dwButtonState;
    g_mouseEvent.eventFlags = mouseEvent.dwEventFlags;
}

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
    case S_SPLASHSCREEN: splashScreenWait(); // game logic for the splash screen
        break;
    case S_STARTSCREEN: updateStart();
        break;
    case S_GAME: updateGame(); // gameplay logic when we are in the game
        break;
    case S_PAUSESCREEN: updatePause();
        break;
    case S_LOSS: updateLoss();
        break;
    }
}


void splashScreenWait()    // waits for time to pass in splash screen
{
    if (g_dElapsedTime > 3.0) // wait for 3 seconds to switch to start screen, else do nothing
        g_eGameState = S_STARTSCREEN;


    /*processUserInput();*/
    /*getInput();*/
    /*if (g_skKeyEvent[K_SPACE].keyReleased)
    {
        g_eGameState = S_GAME;
    }*/
}

void updateStart()
{
    startInput();
}

void updateGame()       // gameplay logic
{
    processUserInput(); // checks if you should change states or do something else with the game, e.g. pause, exit
    inventoryInput();
    moveCharacter();    // moves the character, collision detection, physics, etc
                        // sound can be played here too.
    shootInput();
    stalkerMovement(stalkers);
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


void updatePause()
{
    processUserInput();
    pauseInput();
}

void updateLoss()
{
    lossInput();
}

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

void shootInput()
{
    if (g_skKeyEvent[K_L].keyReleased)
    {
        shoot();
    }
}


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
                Y = 5;
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
                X = 180;
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
            BArr[i].m_cLocation.X = BArr[0].m_cLocation.X + count-1;
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
    }
}

void bossMovement(SGameChar BArr[9])
{
    int dir = (rand() % 4) + 1;
    int x, y;
    for (int i = 0; i < 9; i++)
    {
        x = BArr[i].m_cLocation.X;
        y = BArr[i].m_cLocation.Y;

        switch (dir)
        {
            //Up
        case 1:
            if (BArr[i].m_cLocation.Y > 0)
            {
                if ((map[y - 1][x] != '#') &&
                    (map[y - 1][x] != '[') &&
                    (map[y - 1][x] != ']')&&
                    (map[y - 1][x] != '=') && 
                    (map[y - 1][x] != (char)43) &&
                    (map[y - 1][x] != (char)33))

                    BArr[i].m_cLocation.Y--;
            }
            break;
            //Left
        case 2:
            if (BArr[i].m_cLocation.X > 0)
            {
                if ((map[y][x + 1] != '#') &&
                    (map[y - 1][x] != '[') &&
                    (map[y - 1][x] != ']') &&
                    (map[y - 1][x] != '=') &&
                    (map[y - 1][x] != (char)43) &&
                    (map[y - 1][x] != (char)33))

                    BArr[i].m_cLocation.X++;
            }
            break;
            //Down
        case 3:
            if (BArr[i].m_cLocation.Y < g_Console.getConsoleSize().Y - 1)
            {
                if ((map[y + 1][x] != '#') &&
                    (map[y - 1][x] != '[') &&
                    (map[y - 1][x] != ']') &&
                    (map[y - 1][x] != '=') &&
                    (map[y - 1][x] != (char)43) &&
                    (map[y - 1][x] != (char)33))

                    BArr[i].m_cLocation.Y++;
            }
            break;
            //Right
        case 4:
            if (BArr[i].m_cLocation.X < g_Console.getConsoleSize().X - 1)
            {
                if ((map[y][x - 1] != '#') &&
                    (map[y - 1][x] != '[') &&
                    (map[y - 1][x] != ']') &&
                    (map[y - 1][x] != '=') &&
                    (map[y - 1][x] != (char)43) &&
                    (map[y - 1][x] != (char)33))
                    BArr[i].m_cLocation.X--;
            }
            break;
        }
    }
    bossSearchPlayer(bossParticles);
    bossAttackSeq();
    if (bossProj() != 's')
    {
        if (player->getX() < bpro.m_cLocation.X)
        {
            bpro.m_cLocation.X--;
        }
        else if (player->getX() > bpro.m_cLocation.X)
        {
            bpro.m_cLocation.X++;
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
    bossDeath();
}

char bossSearchPlayer(SGameChar BArr[9])
{
    int diagX, diagY;
    int final = 0;
    for (int i = 0; i < 9; i++)
    {
        // same lanes
        if((player->getX() == BArr[i].m_cLocation.X) ||
            (player->getY() == BArr[i].m_cLocation.Y))
            if ((player->getX() - BArr[i].m_cLocation.X <= 10) &&
                (player->getY() - BArr[i].m_cLocation.Y <= 10))
            {
                return 'y';
            }
       else
       {
            diagX = player->getX() - BArr[i].m_cLocation.Y;
            diagY = player->getY() - BArr[i].m_cLocation.X;
            if (diagX < 0)
            {
                diagX *= -1;
            }
            if (diagY < 0)
            {
                diagY *= -1;
            }
            final += diagX += diagY;
            if (final <= 10)
            {
                return 'y';
            }
       }

    }
    
    return 'n';
}

void bossAttackSeq()
{
    if (bossSearchPlayer(bossParticles) != 'n')
    {
        bossProj();
    }
}

char bossProj()
{
    bpro.m_cLocation.X = bossParticles[4].m_cLocation.X - 1;
    bpro.m_cLocation.Y = bossParticles[4].m_cLocation.Y;
    return 'f';
}

void bossDeath()
{
    if (bossHp == 0)
    {
        for (int i = 0; i < 9; i++)
        {
            bossParticles[i].m_bActive = false;
        }
    }
}

void phantomMovement()
{
        int dir = (rand() % 4) + 1;
        int x, y;
        x = p1.m_cLocation.X;
        y = p1.m_cLocation.Y;
        switch (dir)
        {
        case 1:
            if (p1.m_cLocation.X < g_Console.getConsoleSize().X - 1)
            {
                if(map[y][x - 1] != '#')
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
                if(map[y - 1][x] != '#')
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
        return 'n';
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

void projReachPlayer()
{
   
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
}

void checkerY1(int i, int n) // y--
{
    int x = stalkers[i].m_cLocation.X;
    int y = stalkers[i].m_cLocation.Y;
    if (player->getY() < stalkers[i].m_cLocation.Y)
        if((map[y + n][x] != '#') &&
            (map[y + n][x] != '=') &&
            (map[y + n][x] != '[') &&
            (map[y + n][x] != ']') &&
            (map[y + n][x] != ')') &&
            (map[y + n][x] != '(') &&
            (map[y + n][x] != '*') &&
            (map[y + n][x] != '`') &&
            (map[y + n][x] != 'S'))
            {
                stalkers[i].m_cLocation.Y+=n;
            }
}

void checkerY2(int i, int n) // y++
{
    int x = stalkers[i].m_cLocation.X;
    int y = stalkers[i].m_cLocation.Y;
    if(player->getY() > stalkers[i].m_cLocation.Y)
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
            stalkers[i].m_cLocation.Y+=n;
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
        else
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
        stalkerSearchPlayer();
        stalkerChasePlayer();
    }
}

char stalkerSearchPlayer()
{
    int x_final, y_final;
    int maxNumOfSteps = 0;
    // check if the player and enemy are in the same lane
    for (int i = 0; i < 12; i++)
    {
        if ((player->getY() == stalkers[i].m_cLocation.Y) ||
            (player->getX() == stalkers[i].m_cLocation.X))
            if(player->getX() - stalkers[i].m_cLocation.X <= 2)
        {
            switch (i)
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
            // add together
            maxNumOfSteps += x_final += y_final;
            if (maxNumOfSteps <= 4)
            {
                switch (i)
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
    if(stalkerSearchPlayer() != 'n')
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

void stalkerReachPlayer()
{
    for (int i = 0; i < 12; i++)
    {
        if ((player->getX() == stalkers[i].m_cLocation.X) && (player->getY() == stalkers[i].m_cLocation.Y))
        {
            if(stalkers[i].m_bActive == true)
                player->setHealth(player->getHealth() - 1);
                stalkers[i].m_cLocation.X = player->getX() + 1;
        }
    }
}

void startInput()
{
    if (g_skKeyEvent[K_SPACE].keyDown)
        g_eGameState = S_GAME;
    if (g_skKeyEvent[K_ESCAPE].keyDown)
        g_bQuitGame = true;

}

void pauseInput()
{
    if (g_skKeyEvent[K_Q].keyDown)
        g_bQuitGame = true;
}

void lossInput()
{
    if (g_skKeyEvent[K_Q].keyDown)
        g_bQuitGame = true;
    if (g_skKeyEvent[K_SPACE].keyDown)
    {
        gameInit();
        g_eGameState = S_GAME;
    }
}

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
    case S_GAME: renderGame();
        break;
    case S_PAUSESCREEN: renderPauseScreen();
        break;
    case S_LOSS: renderLoss();
        break;
    }
    renderFramerate();      // renders debug information, frame rate, elapsed time, etc
    renderInputEvents();    // renders status of input events
    renderToScreen();       // dump the contents of the buffer to the screen, one frame worth of game
}

void clearScreen()
{
    // Clears the buffer with this colour attribute
    g_Console.clearBuffer(0x1F);
}

void renderTitle(int x,int y) // function to render title
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

void renderToScreen()
{
    // Writes the buffer to the console, hence you will see what you have written
    g_Console.flushBufferToConsole();
}

void renderSplashScreen()  // renders the splash screen
{
    loadStartmap();

    const WORD colors[] = {
        0x1A, 0x2B, 0x3C, 0x4D, 0x5E, 0x6F,
        0xA1, 0xB2, 0xC3, 0xD4, 0xE5, 0xF6
    };

    //render's BG
    COORD size = g_Console.getConsoleSize();
    /*while (g_dElapsedTime)
    {

    }*/
    for (int pos = 0; pos < 40; pos++)
    {
        g_Console.clearBuffer();
        for (int i = 0; i < size.Y; i++)
        {
            for (int x = 0; x < size.X; x++)
            {
                g_Console.writeToBuffer(x, i, " ", 0x80);
            }
        }
        renderTitle(100,65 - pos);
    }
    
}

void renderStart()
{
    
    renderStartmap();
    renderTitle(30,10);
    renderStartOptions();
}

void renderGame()
{
    loadmap();
    renderMap();        // renders the map to the buffer first
    renderCharacter();  // renders the character into the buffer

    renderEnemies(stalkers, snum, sColor);    // renders the enemies into the buffer 
    //renderEnemies(phantoms, pnum, pcolor);
    //renderEnemies(projectiles, projnum, projColor);
    //renderBossParticles(bossParticles);
    renderBoss(bossParticles);
    renderBullets();

    renderGUI();        // renders game user interface
}

void renderPauseScreen()
{
    renderGame();
    renderPauseBase();
    renderPauseOptions();
}

void renderLoss()
{
    renderLossOptions();
}

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
            // detect boss
            for (int j = 0; j < 9; j++)
            {
                if (j == 0 || j == 2 || j == 6 || j == 8)
                {
                    if ((bossParticles[j].m_cLocation.X == x) && (bossParticles[j].m_cLocation.Y == y))
                    {
                        bossHp - 5;
                        delete bulletArray[i];
                        bulletArray[i] = nullptr;
                        return;
                    }
                }
            }
            // detect boss bullet
            if ((x == bpro.m_cLocation.X) && (y == bpro.m_cLocation.Y))
            {
                bpro.m_bActive = false;
                delete bulletArray[i];
                bulletArray[i] = nullptr;
                return;

            }

        }
    }
}

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

void loadmap()
{
    if (map1Clear != true)
    {
        std::ifstream Lv1("MapLv1.txt");
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

void renderMap()
{
    //render Map
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


void renderStartOptions()
{
    COORD c = g_Console.getConsoleSize();
    c.Y = (c.Y / 20);
    c.X = c.X / 3;

    COORD cSTART = { c.X, c.Y + 25 };
    COORD cQUIT = { c.X, c.Y + 28 };

    std::string START = "PRESS SPACE TO START";
    std::string QUIT = "PRESS ESC TO QUIT";

    g_Console.writeToBuffer(cSTART, START, 0x0c, START.length());
    g_Console.writeToBuffer(cQUIT, QUIT, 0x0c, QUIT.length());
}

void renderPauseBase()
{
    COORD c = g_Console.getConsoleSize();
    c.X = c.X / 4;
    c.Y = c.Y / 3;

    for (int i = 0; i < 15; i++)
    {
        for (int i = 0; i < c.X; i++)
        {
            g_Console.writeToBuffer(c.X + i, c.Y, ' ', 0x33);
        }
        c.Y++;
    }
}

void renderPauseOptions()
{
    COORD c = g_Console.getConsoleSize();
    c.Y = (c.Y / 25);
    c.X = c.X / 3;

    COORD cCONTINUE = { c.X, c.Y + 25 };
    COORD cQUIT = { c.X, c.Y + 28 };

    std::string CONTINUE = "PRESS ESC TO CONTINUE";
    std::string QUIT = "PRESS Q TO QUIT";

    g_Console.writeToBuffer(cCONTINUE, CONTINUE, 0x0c, CONTINUE.length());
    g_Console.writeToBuffer(cQUIT, QUIT, 0x0c, QUIT.length());
}

void renderLossOptions()
{
    COORD c = g_Console.getConsoleSize();
    c.Y = (c.Y / 20);
    c.X = c.X / 10;

    COORD cLOST = { c.X, c.Y + 22 };
    COORD cRETRY = { c.X, c.Y + 25 };
    COORD cQUIT = { c.X, c.Y + 28 };

    std::string LOST = "YOU LOST";
    std::string RETRY = "PRESS SPACE TO RETRY";
    std::string QUIT = "PRESS Q TO QUIT";

    g_Console.writeToBuffer(cLOST, LOST, 0x0c, LOST.length());
    g_Console.writeToBuffer(cRETRY, RETRY, 0x0c, RETRY.length());
    g_Console.writeToBuffer(cQUIT, QUIT, 0x0c, QUIT.length());
}

void renderGUI() // render game user inferface
{
    std::string objective = "Objective: Escape the Dungeon";
    std::string lifeBar = "Lives: " + std::to_string(player->getLives());
    std::string healthBar = "Health: " + std::to_string(player->getHealth()) + "/" + std::to_string(player->getMaxHealth());
    std::string inventoryList = "Inventory: ";
    std::string tempStr;

    g_Console.writeToBuffer(1, 1, objective, 0x0C, objective.length());
    g_Console.writeToBuffer(1, 2, lifeBar, 0x0C, lifeBar.length());
    g_Console.writeToBuffer(1, 3, healthBar, 0x0C, healthBar.length());
    g_Console.writeToBuffer(1, 6, inventoryList, 0x0C, inventoryList.length());

    for (int i = 0; i < 5; i++) // print inventory contents
    {
        if (player->getInventory(i) != nullptr)
            tempStr = std::to_string(i + 1) + ". " + player->getInventory(i)->getName();
        else
            tempStr = std::to_string(i + 1) + ".";

        g_Console.writeToBuffer(1, 7 + i, tempStr, 0x0C, tempStr.length());
    }
}

void playerInteractions()
{
    for (int i = 0; i < 10; i++) // player interacts with a chest

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

    //Enemy interaction
    stalkerReachPlayer();
    projReachPlayer();

    if ((map[player->getY()][player->getX()] == '0') ||
        (map[player->getY()][player->getX()] == '1') || 
        (map[player->getY()][player->getX()] == '2') || 
        (map[player->getY()][player->getX()] == '3'))
    {
        player->setHealth(player->getHealth() - 5);
    }
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
}

void renderCharacter()
{
    // Draw the location of the character // was 1 for char
    g_Console.writeToBuffer(player->getPosition(), 'P', player->getCharColour());
}

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

    if(pro3.m_bActive == true)
        g_Console.writeToBuffer(pro3.m_cLocation, (char)projnum, projColor);

    if(pro4.m_bActive == true)
        g_Console.writeToBuffer(pro4.m_cLocation, (char)projnum, projColor);

    if (pro5.m_bActive == true)
        g_Console.writeToBuffer(pro5.m_cLocation, (char)projnum, projColor);

    if (bpro.m_bActive == true)
    {
        g_Console.writeToBuffer(bpro.m_cLocation, (char)projnum, projColor);
    }
}

//void renderBossParticles(SGameChar BArr[9])
//{
//    WORD bossColor = 0x0A;
//    for (int i = 0; i < 9; i++)
//    {
//        g_Console.writeToBuffer(BArr[i].m_cLocation, (char)43, bossColor);
//    }
//}

void renderBoss(SGameChar BArr[9])
{
    WORD bossColor = 0x5E;
    WORD bossCorner = 0x4D;
    for (int i = 0; i < 9; i++)
    {
            if (i == 0 || i == 2 || i == 6 || i == 8)
            {
                g_Console.writeToBuffer(BArr[i].m_cLocation, (char)43, bossCorner);
                switch (i)
                {
                case 0:
                    if (bossHp <= 120)
                    {
                        g_Console.writeToBuffer(BArr[i].m_cLocation, (char)33, bossColor);
                    }
                    break;
                case 2:
                    if (bossHp <= 90)
                    {
                        g_Console.writeToBuffer(BArr[i].m_cLocation, (char)33, bossColor);
                    }
                    break;
                case 6:
                    if (bossHp <= 30)
                    {
                        g_Console.writeToBuffer(BArr[i].m_cLocation, (char)33, bossColor);
                    }
                    break;
                case 8:
                    if (bossHp == 0)
                    {
                        g_Console.writeToBuffer(BArr[i].m_cLocation, (char)33, bossColor);
                    }
                    break;
                }
            }
            else
            {
                g_Console.writeToBuffer(BArr[i].m_cLocation, (char)33, bossColor);
            }
     }
    
}

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
        break; // right
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
        break; // right
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
        break; // right
    }
}

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