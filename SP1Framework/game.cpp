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

// define WASD keys
#define VK_KEY_W    0x57;
#define VK_KEY_A    0x41;
#define VK_KEY_S    0x53;
#define VK_KEY_D    0x44;

// define WASDQ keys

#define VK_KEY_W    0x57
#define VK_KEY_A    0x41
#define VK_KEY_S    0x53
#define VK_KEY_D    0x44
#define VK_KEY_Q    0x51

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

// Game specific variables here
SGameChar   g_sChar;
// testing out enemy and enemy bullet vars:
// stalkers
SGameChar   s1, s2, s3, s4, s5, s6, s7, s8, s9, s10;
SGameChar Enemies[10] = { s1,s2,s3,s4,s5,s6,s7,s8,s9,s10};
// phantom
SGameChar   p1, p2, p3, p4, p5, p6, p7, p8, p9, p10;
// projectile of phantom
SGameChar pro1;
// boss
//SGameChar bx,by,bTailX[50], bTailY[50];
SGameChar b1, b2, b3, b4, b5, b6, b7, b8, b9, b10, b11, b12, b13, b14, b15;
SGameChar bossParticles[15] = {b1,b2,b3,b4,b5,b6,b7,b8,b9,
                           b10,b11,b12,b13,b14,b15};
int stalkHp, bossHp;

Player* player = new Player; // player initialisation
Chest* chest[10] = { nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr }; // chests initialisation

EGAMESTATES g_eGameState = S_SPLASHSCREEN; // initial state

// Console object
Console g_Console(300, 64, "ESCAPE THE DUNGEON");
// map
char map[65][300];

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
    //player->getX() = g_Console.getConsoleSize().X / 2; (does not allow)
    //player->getY() = g_Console.getConsoleSize().Y / 2;
    g_sChar.m_bActive = true;
    // Enemy initial state
    //s1.m_cLocation.X = (g_Console.getConsoleSize().X / 2) + 10;
    //s1.m_cLocation.Y = (g_Console.getConsoleSize().Y / 2) + 10;
    //Testing Phantom:
    p1.m_cLocation.X = (g_Console.getConsoleSize().X / 2) + 10;
    p1.m_cLocation.Y = (g_Console.getConsoleSize().Y / 2) + 10;
    p1.m_bActive = true;
    pro1.m_bActive = true;
    randEnemyCoord(Enemies);
    bossBodyCoord(bossParticles);
 
    g_dElapsedTime = 0.0;

    // sets the initial state for the game
    g_eGameState = S_SPLASHSCREEN;

    gameInit();
    
    // sets the width, height and the font name to use in the console
    g_Console.setConsoleFont(0, 16, L"Consolas");

    // remember to set your keyboard handler, so that your functions can be notified of input events
    g_Console.setKeyboardHandler(keyboardHandler);
    g_Console.setMouseHandler(mouseHandler);
}

void gameInit()
{
    
    player->setSpawnPoint(g_Console.getConsoleSize().X / 2, g_Console.getConsoleSize().Y / 2); // set spawn point
    player->setPosition(player->getSpawnPoint()); // spawn the player at his spawn point
    player->setLives(3);
    player->setMaxHealth(100);
    player->setHealth(player->getMaxHealth());
    player->setDirection('W');
    player->setCharColour(0x84);
    for (int i = 0; i < 5; i++)
        player->setInventory(i, nullptr); // clear inventory
    player->setActive(true); // set him to be active

    // initialises chests
    chest[0] = new Chest; // initialise chest
    chest[0]->setPosition(156, 36); // set its position
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
    case VK_UP: key = K_W; break;
    case VK_DOWN: key = K_S; break;
    case VK_LEFT: key = K_A; break;
    case VK_RIGHT: key = K_D; break;
    case VK_ESCAPE: key = K_ESCAPE; break;
    case 0x31: key = K_1; break;
    case 0x32: key = K_2; break;
    case 0x33: key = K_3; break;
    case 0x34: key = K_4; break;
    case 0x35: key = K_5; break;

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
    stalkerMovement(Enemies);
    
    phantomMovement();
    //bossMovement(bossParticles);

    playerInteractions();
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

bool coordCheck(std::string arr[10], std::string cmb)
{
    for (int i = 0; i < 10; i++)
    {
        if (cmb == arr[i])
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    //return false;
}


void randEnemyCoord(SGameChar EArr[10])
{
    int rndX, rndY, x, y;
    std::string used[10]; // size dependent on num of enemies
    std::string cmb;

    for (int i = 0; i < 10; i++)
    {
        while (true)
        {
            rndX = (rand() % g_Console.getConsoleSize().X / 2) + 5;
            rndY = (rand() % g_Console.getConsoleSize().Y / 2) + 2;
            cmb = std::to_string(rndX) + std::to_string(rndY);

            EArr[i].m_cLocation.X = rndX;
            EArr[i].m_cLocation.Y = rndY;
            used[i] += cmb;
            x = rndX;
            y = rndY;
            if ((coordCheck(used, cmb) == true) || ((map[y][x] == '#') ||
                (map[y][x] == '=') || (map[y][x] == '[') ||
                (map[y][x] == ']') || (map[y][x] == ')') ||
                (map[y][x] == '(') || (map[y][x] == '*') ||
                (map[y][x] == '-') || (map[y][x] ==  '%') ||
                (map[y][x] == '`')))
            {
                continue;
            }
            else
            {
                break;
            }
        }
    }
}
void bossBodyCoord(SGameChar BArr[15])
{
    int count = 0;
    int rndX  = (rand() % g_Console.getConsoleSize().X / 2) + 5;
    int rndY  = (rand() % g_Console.getConsoleSize().Y / 2) + 5;

    for (int i = 0; i < 15; i++)
    {
        BArr[0].m_cLocation.X = rndX;
        BArr[0].m_cLocation.Y = rndY;
        count++;
        if (i > 0)
        {
            BArr[i].m_cLocation.X = BArr[0].m_cLocation.X + count-1;
            BArr[i].m_cLocation.Y = BArr[0].m_cLocation.Y;
        }
        if (i > 4)
        {
            BArr[i].m_cLocation.X = BArr[0].m_cLocation.X;
            BArr[i].m_cLocation.Y = BArr[0].m_cLocation.Y + 1;
        }
        if (i > 5)
        {
            BArr[i].m_cLocation.X = BArr[0].m_cLocation.X + count-6;
            BArr[i].m_cLocation.Y = BArr[5].m_cLocation.Y;
        }
        if (i > 9)
        {
            BArr[i].m_cLocation.X = BArr[0].m_cLocation.X;
            BArr[i].m_cLocation.Y = BArr[5].m_cLocation.Y + 1;
        }
        if (i > 10) 
        {
            BArr[i].m_cLocation.X = BArr[0].m_cLocation.X + count-11;
            BArr[i].m_cLocation.Y = BArr[10].m_cLocation.Y;
        }
    }
}


void bossMovement(SGameChar BArr[15])
{
    int dir = (rand() % 4) + 1;
    for (int i = 0; i < 15; i++)
    {
        switch (dir)
        {
            //Up
        case 1:
            if (BArr[i].m_cLocation.Y > 0 && BArr[i].m_cLocation.Y < 5)
            {
                BArr[i].m_cLocation.Y--;
            }
            break;
            //Left
        case 2:
            if (BArr[i].m_cLocation.X > 0 && BArr[i].m_cLocation.X < 5)
            {
                BArr[i].m_cLocation.X--;
            }
            break;
            //Down
        case 3:
            if (BArr[i].m_cLocation.Y < g_Console.getConsoleSize().Y - 20)
            {
                BArr[i].m_cLocation.Y++;
            }
            break;
            //Right
        case 4:
            if (BArr[i].m_cLocation.X < g_Console.getConsoleSize().X - 20)
            {
                BArr[i].m_cLocation.X++;

            }
            break;

        default:
            break;
        }
    }
}

void bossSearchPlayer()
{

}

void bossAttackSeq()
{

}

void bossDeath()
{

}

void phantomMovement()
{
    int diagdir = (rand() % 4) + 1;
    switch (diagdir)
    {
    case 1:
        if ((p1.m_cLocation.X < g_Console.getConsoleSize().X - 1)
            && (p1.m_cLocation.Y > 0))
        {

            p1.m_cLocation.X++;
            p1.m_cLocation.Y--;
        }
        break;
    case 2:
        if ((p1.m_cLocation.Y < g_Console.getConsoleSize().Y - 1)
            && (p1.m_cLocation.X > 0))
        {
            p1.m_cLocation.X--;
            p1.m_cLocation.Y++;
        }
        break;
    case 3:
        if ((p1.m_cLocation.Y > 0) && (p1.m_cLocation.X > 0))
        {
            p1.m_cLocation.Y--;
            p1.m_cLocation.X--;
        }
        break;
    case 4:
        if ((p1.m_cLocation.X < g_Console.getConsoleSize().X - 1)
            && (p1.m_cLocation.Y < g_Console.getConsoleSize().Y - 1))
        {
            p1.m_cLocation.Y++;
            p1.m_cLocation.X++;
        }
        break;
    }
    
    phantomSearchPlayer();
    phantomFireProj();
    //fix rand spawn pt
    if (player->getX() < pro1.m_cLocation.X)
    {
        pro1.m_cLocation.X--;
    }
    else if(player->getY() < pro1.m_cLocation.Y)
    {
        pro1.m_cLocation.Y--;
    }
    else if (player->getX() > pro1.m_cLocation.X)
    {
        pro1.m_cLocation.X++;
    }
    else if (player->getY() > pro1.m_cLocation.Y)
    {
        pro1.m_cLocation.Y++;
    }
    else if((player->getX() == pro1.m_cLocation.X)
        && (player->getY() == pro1.m_cLocation.Y))
    {
        pro1.m_bActive = false; //bullet vanishes
        p1.m_bActive = false;   //phantom dies
    }
}

char phantomSearchPlayer()
{
    int dist = 15;
    
    if ((p1.m_cLocation.X - dist <= player->getX())
        && (player->getY() == p1.m_cLocation.Y))
    {   // so that left side will not follow
        if((p1.m_cLocation.X <= player->getX())
            && (p1.m_cLocation.Y == player->getY()))
            {
            return 'n';
            }
        return 'f'; // if player is infront of phantom
    }
    else if ((p1.m_cLocation.Y - dist <= player->getY())
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

void phantomFireProj()
{
    if (phantomSearchPlayer() != 'n')
    {
        renderProj();
    }
}

int getSCurrPosX(SGameChar EArr[2])
{
    int pX;
    for (int i = 0; i < 2; i++)
    {
        pX = EArr[i].m_cLocation.X;
    }
    return pX;
}

int getSCurrPosY(SGameChar EArr[2])
{
    int pY;
    for (int i = 0; i < 2; i++)
    {
        pY = EArr[i].m_cLocation.Y;
    }
    return pY;
}

void setSCurrPos(int x, int y)
{
    x = getSCurrPosX(Enemies);
    y = getSCurrPosY(Enemies);
}

void stalkerMovement(SGameChar EArr[10])
{
    int dir = (rand() % 4) + 1;
    int x, y;

    //Note: find a way to make them move individually, arr[i] applies to both same.

    for (int i = 0; i < 10; i++)
    {
        x = EArr[i].m_cLocation.X;
        y = EArr[i].m_cLocation.Y;
        if (i % 2 == 0) // every even enemy moves differently
        {
            switch (dir)
            {
                //Down
            case 4:
                if ((EArr[i].m_cLocation.Y < g_Console.getConsoleSize().Y - 1) &&
                    (map[y + 2][x] != '#') &&
                    (map[y + 2][x] != '=') &&
                    (map[y + 2][x] != '[') &&
                    (map[y + 2][x] != ']') &&
                    (map[y + 2][x] != ')') &&
                    (map[y + 2][x] != '(') &&
                    (map[y + 2][x] != '*') &&
                    (map[y + 2][x] != '`'))
                {
                    if (i > 0)
                    {
                        if (map[y + 2][x] != EArr[i - 1].m_cLocation.Y)
                        {
                            EArr[i].m_cLocation.Y += 2;
                            break;
                        }
                    }
                    EArr[i].m_cLocation.Y += 2;
                }

                break;
                //Right
            case 3:
                if ((EArr[i].m_cLocation.X < g_Console.getConsoleSize().X - 1) &&
                    (map[y][x - 2] != '#') &&
                    (map[y][x - 2] != '=') &&
                    (map[y][x - 2] != '[') &&
                    (map[y][x - 2] != ']') &&
                    (map[y][x - 2] != ')') &&
                    (map[y][x - 2] != '(') &&
                    (map[y][x - 2] != '*') &&
                    (map[y][x - 2] != '`'))
                {
                    if (i > 0)
                    {
                        if (map[y][x - 2] != EArr[i - 1].m_cLocation.X)
                        {
                            EArr[i].m_cLocation.X -= 2;
                            break;
                        }
                    }
                    EArr[i].m_cLocation.X -= 2;
                }


                break;
                //Up
            case 2:
                if ((EArr[i].m_cLocation.Y > 0) &&
                    (map[y - 2][x] != '#') &&
                    (map[y - 2][x] != '=') &&
                    (map[y - 2][x] != '[') &&
                    (map[y - 2][x] != ']') &&
                    (map[y - 2][x] != ')') &&
                    (map[y - 2][x] != '(') &&
                    (map[y - 2][x] != '*') &&
                    (map[y - 2][x] != '`'))
                {
                    if (i > 0)
                    {
                        if (map[y - 2][x] != EArr[i - 1].m_cLocation.Y)
                        {
                            EArr[i].m_cLocation.Y -= 2;
                            break;
                        }
                    }
                    EArr[i].m_cLocation.Y -= 2;
                }

                break;
                //Left
            case 1:
                if ((EArr[i].m_cLocation.X > 0) &&
                    (map[y][x + 2] != '#') &&
                    (map[y][x + 2] != '=') &&
                    (map[y][x + 2] != '[') &&
                    (map[y][x + 2] != ']') &&
                    (map[y][x + 2] != ')') &&
                    (map[y][x + 2] != '(') &&
                    (map[y][x + 2] != '*') &&
                    (map[y][x + 2] != '`'))
                {
                    if (i > 0)
                    {
                        if (map[y][x + 2] != EArr[i - 1].m_cLocation.X)
                        {
                            EArr[i].m_cLocation.X += 2;
                            break;
                        }
                    }
                    EArr[i].m_cLocation.X += 2;
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
                    (map[y - 1][x] != '`'))
                {
                    if (i > 0)
                    {
                        if (map[y - 1][x] != EArr[i - 1].m_cLocation.Y)
                        {
                            EArr[i].m_cLocation.Y--;
                            break;
                        }
                    }
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
                    (map[y][x - 1] != '`'))
                {
                    if (i > 0)
                    {
                        if (map[y][x - 1] != EArr[i - 1].m_cLocation.X)
                        {
                            EArr[i].m_cLocation.X--;
                            break;
                        }
                    }
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
                    (map[y + 1][x] != '`'))
                {
                    if (i > 0)
                    {
                        if (map[y + 1][x] != EArr[i - 1].m_cLocation.Y)
                        {
                            EArr[i].m_cLocation.Y++;
                            break;
                        }
                    }
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
                    (map[y][x + 1] != '`'))
                {
                    if (i > 0)
                    {
                        if (map[y][x + 1] != EArr[i - 1].m_cLocation.X)
                        {
                            EArr[i].m_cLocation.X++;
                            break;
                        }
                    }
                    EArr[i].m_cLocation.X++;
                }

                break;
            }

        }

        stalkerSearchPlayer(Enemies);
        stalkerChasePlayer(Enemies);
    }
}

bool stalkerSearchPlayer(SGameChar EArr[10])
{
    // Assume a 5x5 grid radius of range
    int x_final, y_final;
    int maxNumOfSteps = 0;
    // check if the player and enemy are in the same lane
    for (int i = 0; i < 10; i++)
    {
        if ((player->getY() == EArr[i].m_cLocation.Y + 5) ||
            (player->getX() == EArr[i].m_cLocation.X + 5))
        {
            if (player->getX() - EArr[i].m_cLocation.X <= 2)
            {
                return true;
            }
        }
        // check for other possibilities
        else
        {
            x_final = player->getX() - EArr[i].m_cLocation.X;
            y_final = player->getY() - EArr[i].m_cLocation.Y;
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
                return true;
            }
        }
    }
    return false;
    // if both conditions not met, 
    // player not in range of enemy
}

void stalkerChasePlayer(SGameChar EArr[10]) 
{
    for (int i = 0; i < 10; i++)
    {
        if (stalkerSearchPlayer(Enemies) == true)
        {
            // After 10 steps stop chasing
            if ((player->getX() - 10 <= EArr[i].m_cLocation.X) ||
                (player->getY() - 10 <= EArr[i].m_cLocation.Y))
            {
                if (player->getX() > EArr[i].m_cLocation.X)
                {
                    EArr[i].m_cLocation.X++;
                }
                else if (player->getX() < EArr[i].m_cLocation.X)
                {
                    EArr[i].m_cLocation.X--;
                }
                else if (player->getY() > EArr[i].m_cLocation.Y)
                {
                    EArr[i].m_cLocation.Y++;
                }
                else if (player->getY() < EArr[i].m_cLocation.Y)
                {
                    EArr[i].m_cLocation.Y--;
                }
                else if ((player->getY() && player->getX())
                    == (EArr[i].m_cLocation.Y && EArr[i].m_cLocation.X))
                {
                    player->getHealth() - 5; // lose hp
                }
            }
        }
    }

    //if (g_skKeyEvent[K_SPACE].keyDown) // debugging purposes
    //    player->setHealth(player->getHealth() - 10);

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

void renderTitle() // function to render title
{
    COORD c = g_Console.getConsoleSize();
    c.Y /= 20;
    c.X = c.X / 10;

    // ESCAPE
    g_Console.writeToBuffer(c, "    //   / /  //   ) )  //   ) )  // | |     //   ) ) //   / / ", 0x0F);
    c.Y += 1;
    g_Console.writeToBuffer(c, "   //____    ((        //        //__| |    //___/ / //____    ", 0x0F);
    c.Y += 1;
    g_Console.writeToBuffer(c, "  / ____       ", 0x0F);
    c.X += 15;
    g_Console.writeToBuffer(c, (char)92, 0x0F);
    c.X += 1;
    g_Console.writeToBuffer(c, (char)92, 0x0F);
    c.X += 1;
    g_Console.writeToBuffer(c, "     //        / ___  |   / ____ / / ____     ", 0x0F);
    c.X -= 17;
    c.Y += 1;
    g_Console.writeToBuffer(c, " //              ) ) //        //    | |  //       //          ", 0x0F);
    c.Y += 1;
    g_Console.writeToBuffer(c, "//____/ / ((___ / / ((____/ / //     | | //       //____/ /    ", 0x0F);


    // THE
    c.Y += 2;
    c.X += 15;
    g_Console.writeToBuffer(c, " /__  ___/ //    / / //   / / ", 0x0F);
    c.Y += 1;
    g_Console.writeToBuffer(c, "   / /    //___ / / //____    ", 0x0F);
    c.Y += 1;
    g_Console.writeToBuffer(c, "  / /    / ___   / / ____     ", 0x0F);
    c.Y += 1;
    g_Console.writeToBuffer(c, " / /    //    / / //          ", 0x0F);
    c.Y += 1;
    g_Console.writeToBuffer(c, "/ /    //    / / //____/ /    ", 0x0F);


    // DUNGEON
    c.Y += 2;
    c.X -= 20;
    g_Console.writeToBuffer(c, "    //    ) ) //   / / /|    / / //   ) )  //   / /  //   ) ) /|    / / ", 0x0F);
    c.Y += 1;
    g_Console.writeToBuffer(c, "   //    / / //   / / //|   / / //        //____    //   / / //|   / /  ", 0x0F);
    c.Y += 1;
    g_Console.writeToBuffer(c, "  //    / / //   / / // |  / / //  ____  / ____    //   / / // |  / /   ", 0x0F);
    c.Y += 1;
    g_Console.writeToBuffer(c, " //    / / //   / / //  | / / //    / / //        //   / / //  | / /    ", 0x0F);
    c.Y += 1;
    g_Console.writeToBuffer(c, "//____/ / ((___/ / //   |/ / ((____/ / //____/ / ((___/ / //   |/ /     ", 0x0F);
    c.Y += 1;
}

void renderToScreen()
{
    // Writes the buffer to the console, hence you will see what you have written
    g_Console.flushBufferToConsole();
}

void renderSplashScreen()  // renders the splash screen
{

    const WORD colors[] = {
        0x1A, 0x2B, 0x3C, 0x4D, 0x5E, 0x6F,
        0xA1, 0xB2, 0xC3, 0xD4, 0xE5, 0xF6
    };

    //render's BG
    COORD size = g_Console.getConsoleSize();
    for (int i = 0; i < size.Y; i++)
    {
        for (int x = 0; x < size.X; x++)
        {
            g_Console.writeToBuffer(x, i, " ", 0x80);
        }

    }

    renderTitle();

}

void renderStart()
{
    renderTitle();
    renderStartOptions();
}

void renderGame()
{
    loadmap();
    renderMap();        // renders the map to the buffer first
    renderCharacter();  // renders the character into the buffer

    renderEnemies(Enemies);    // renders the enemies into the buffer 
    //renderBossParticles(bossParticles);
    //renderBoss(bossParticles);

    renderGUI();        // renders game user interface

}

void renderPauseScreen()
{
    renderPauseBase();
    renderPauseOptions();
}

void renderLoss()
{
    renderLossOptions();
}

void loadmap()
{
    std::ifstream infile("MapLv1.txt");
    std::string var;
    // Init and store Map
    int y = 0;
    while (getline(infile, var)) {
        // Output the text from the file
        for (unsigned i = 0; i < var.length(); ++i)
        {
            map[y][i] = var.at(i);

        }
        y++;
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
            else if (map[y][x] == '$') //Dungeon gate, cannot pass
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
    c.X = c.X / 10;

    COORD cSTART = { c.X, c.Y + 25 };
    COORD cQUIT = { c.X, c.Y + 28 };

    std::string START = "PRESS SPACE TO START";
    std::string QUIT = "PRESS ESC TO QUIT";

    g_Console.writeToBuffer(cSTART, START, 0x0c, START.length());
    g_Console.writeToBuffer(cQUIT, QUIT, 0x0c, QUIT.length());
}

void renderPauseBase()
{
}

void renderPauseOptions()
{
    COORD c = g_Console.getConsoleSize();
    c.Y = (c.Y / 20);
    c.X = c.X / 10;

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


// colour for character
//void renderCharacter()
//{
//    // Draw the location of the character
//    WORD charColor = 0x0C; // background colour of character when non active. (Sherryan)
//
//    if (g_sChar.m_bActive)
//    {
//
//    }
//}
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
}

void renderCharacter()
{
    // Draw the location of the character // was 1 for char
    g_Console.writeToBuffer(player->getPosition(), (char)80 , player->getCharColour());
}

void renderEnemies(SGameChar EArr[10])
{
    WORD charEnemyColor = 0x0A; // when non active. 
    WORD pcolor = 0x5E;
    WORD projColor = 0x4D;
    // Enemy render:
    // character change? at (char)04 // S for Stalker
    if (EArr[0].m_bActive || EArr[1].m_bActive)
    {
        charEnemyColor = 0x0A;
    }
    for (int i = 0; i < 10; i++)
    {
        g_Console.writeToBuffer(EArr[i].m_cLocation, (char)83, charEnemyColor);
    }
    //Testing out phantom
    if (p1.m_bActive == true)
    {
        g_Console.writeToBuffer(p1.m_cLocation, (char)231, pcolor);
    }
    
    if (pro1.m_bActive == true)
    {
        g_Console.writeToBuffer(pro1.m_cLocation, (char)60, projColor);
    }
}

void renderBossParticles(SGameChar BArr[15])
{
    WORD bossColor = 0x0A;
    for (int i = 0; i < 15; i++)
    {
        g_Console.writeToBuffer(BArr[i].m_cLocation, (char)43, bossColor);
    }

}

void renderBoss(SGameChar BArr[15])
{
    WORD bossColor = 0x5E;
    WORD bossCorner = 0x4D;
    int num = 134;
    int num2 = 248;
    for (int i = 0; i < 15; i++)
    {
        if (i == 0 || i == 4 || i == 10 || i == 14)
        {
            g_Console.writeToBuffer(BArr[i].m_cLocation, (char)43, bossCorner);
        }
        else if(i == 6 || i == 8)
        {
            g_Console.writeToBuffer(BArr[i].m_cLocation, (char)num, bossColor);
        }
        else
        {
            g_Console.writeToBuffer(BArr[i].m_cLocation, (char)num2, bossColor);
        }
        
    }
}

void renderProj()
{
    // spawn proj in the matched direction
    switch (phantomSearchPlayer())
    {
    case 'f':
        pro1.m_cLocation.X = p1.m_cLocation.X-1;
        pro1.m_cLocation.Y = p1.m_cLocation.Y;
        break; // front
    case 'r':
        pro1.m_cLocation.X = p1.m_cLocation.X;
        pro1.m_cLocation.Y = p1.m_cLocation.Y-1;
        break; // right
    default:
        break;
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