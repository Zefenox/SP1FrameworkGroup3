// This is the main file for the game logic and function
//
//
#include "game.h"
#include "Framework\console.h"
#include <iostream>
#include <iomanip>
#include <sstream>

double  g_dElapsedTime;
double  g_dDeltaTime;
SKeyEvent g_skKeyEvent[K_COUNT];
SMouseEvent g_mouseEvent;

// Game specific variables here
SGameChar   g_sChar;
// testing out enemy and enemy bullet vars:
// stalkers
SGameChar   s1, s2;
SGameChar Enemies[2] = { s1,s2 };
// phantom
SGameChar   p1;
// projectile of phantom
SGameChar pro1;
// boss
//SGameChar bx,by,bTailX[50], bTailY[50];
SGameChar b1, b2, b3, b4, b5, b6, b7, b8, b9, b10, b11, b12, b13, b14, b15;
SGameChar bossParticles[15] = {b1,b2,b3,b4,b5,b6,b7,b8,b9,
                           b10,b11,b12,b13,b14,b15};
int stalkHp, bossHp;
EGAMESTATES g_eGameState = S_SPLASHSCREEN; // initial state

// Console object
Console g_Console(80, 25, "ESCAPE THE DUNGEON");

//--------------------------------------------------------------
// Purpose  : Initialisation function
//            Initialize variables, allocate memory, load data from file, etc. 
//            This is called once before entering into your main loop
// Input    : void
// Output   : cvoid
//--------------------------------------------------------------
void init( void )
{
    // Set precision for floating point output
    g_dElapsedTime = 0.0;    
    //produces a new seed each run
    srand((unsigned int)time(NULL)); 
    // sets the initial state for the game
    g_eGameState = S_SPLASHSCREEN;

    g_sChar.m_cLocation.X = g_Console.getConsoleSize().X / 2;
    g_sChar.m_cLocation.Y = g_Console.getConsoleSize().Y / 2;
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
 
    // sets the width, height and the font name to use in the console
    g_Console.setConsoleFont(0, 16, L"Consolas");

    // remember to set your keyboard handler, so that your functions can be notified of input events
    g_Console.setKeyboardHandler(keyboardHandler);
    g_Console.setMouseHandler(mouseHandler);
}

//--------------------------------------------------------------
// Purpose  : Reset before exiting the program
//            Do your clean up of memory here
//            This is called once just before the game exits
// Input    : Void
// Output   : void
//--------------------------------------------------------------
void shutdown( void )
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
void getInput( void )
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
    case S_GAME: gameplayKBHandler(keyboardEvent); // handle gameplay keyboard event 
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
    case S_GAME: gameplayMouseHandler(mouseEvent); // handle gameplay mouse event
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
    /*case VK_UP: key = K_UP; break;
    case VK_DOWN: key = K_DOWN; break;
    case VK_LEFT: key = K_LEFT; break; 
    case VK_RIGHT: key = K_RIGHT; break; */
    case VK_SPACE: key = K_SPACE; break;
    case VK_KEY_W: key = K_W; break;
    case VK_KEY_S: key = K_S; break;
    case VK_KEY_A: key = K_A; break;
    case VK_KEY_D: key = K_D; break;
    case VK_ESCAPE: key = K_ESCAPE; break; 
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
        case S_SPLASHSCREEN : splashScreenWait(); // game logic for the splash screen
            break;
        case S_GAME: updateGame(); // gameplay logic when we are in the game
            break;
    }
}


void splashScreenWait()    // waits for time to pass in splash screen
{
    if (g_dElapsedTime > 3.0) // wait for 3 seconds to switch to game mode, else do nothing
        g_eGameState = S_GAME;
}

void updateGame()       // gameplay logic
{
    processUserInput(); // checks if you should change states or do something else with the game, e.g. pause, exit
    moveCharacter();    // moves the character, collision detection, physics, etc
                        // sound can be played here too.
    stalkerMovement(Enemies);
    phantomMovement();
    //bossMovement(bossParticles);
}

void moveCharacter()
{    
    // Updating the location of the character based on the key release
    // providing a beep sound whenver we shift the character
    if (g_skKeyEvent[K_W].keyDown && g_sChar.m_cLocation.Y > 0)
    {
        //Beep(1440, 30);
        g_sChar.m_cLocation.Y--;       
    }
    if (g_skKeyEvent[K_A].keyDown && g_sChar.m_cLocation.X > 0)
    {
        //Beep(1440, 30);
        g_sChar.m_cLocation.X--;        
    }
    if (g_skKeyEvent[K_S].keyDown && g_sChar.m_cLocation.Y < g_Console.getConsoleSize().Y - 1)
    {
        //Beep(1440, 30);
        g_sChar.m_cLocation.Y++;        
    }
    if (g_skKeyEvent[K_D].keyDown && g_sChar.m_cLocation.X < g_Console.getConsoleSize().X - 1)
    {
        //Beep(1440, 30);
        g_sChar.m_cLocation.X++;        
    }
    if (g_skKeyEvent[K_SPACE].keyReleased)
    {
        g_sChar.m_bActive = !g_sChar.m_bActive;        
    }

}

bool coordCheck(std::string arr[100], std::string cmb)
{
    for (int i = 0; i < 100; i++)
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


void randEnemyCoord(SGameChar EArr[2])
{
    int rndX, rndY;
    std::string used[2]; // size dependent on num of enemies
    std::string cmb;

    for (int i = 0; i < 2; i++)
    {
        while (true)
        {
            rndX = (rand() % g_Console.getConsoleSize().X / 2) + 5;
            rndY = (rand() % g_Console.getConsoleSize().Y / 2) + 2;
            cmb = std::to_string(rndX) + std::to_string(rndY);

            EArr[i].m_cLocation.X = rndX;
            EArr[i].m_cLocation.Y = rndY;
            used[i] = cmb[i];
            if (coordCheck(used, cmb) == true)
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
    if (g_sChar.m_cLocation.X < pro1.m_cLocation.X)
    {
        pro1.m_cLocation.X--;
    }
    else if(g_sChar.m_cLocation.Y < pro1.m_cLocation.Y)
    {
        pro1.m_cLocation.Y--;
    }
    else if (g_sChar.m_cLocation.X > pro1.m_cLocation.X)
    {
        pro1.m_cLocation.X++;
    }
    else if (g_sChar.m_cLocation.Y > pro1.m_cLocation.Y)
    {
        pro1.m_cLocation.Y++;
    }
    else if((g_sChar.m_cLocation.X == pro1.m_cLocation.X)
        && (g_sChar.m_cLocation.Y == pro1.m_cLocation.Y))
    {
        pro1.m_bActive = false; //bullet vanishes
        p1.m_bActive = false;   //phantom dies
    }
}

char phantomSearchPlayer()
{
    int dist = 15;
    
    if ((p1.m_cLocation.X - dist <= g_sChar.m_cLocation.X)
        && (g_sChar.m_cLocation.Y == p1.m_cLocation.Y))
    {   // so that left side will not follow
        if((p1.m_cLocation.X <= g_sChar.m_cLocation.X)
            && (p1.m_cLocation.Y == g_sChar.m_cLocation.Y))
            {
            return 'n';
            }
        return 'f'; // if player is infront of phantom
    }
    else if ((p1.m_cLocation.Y - dist <= g_sChar.m_cLocation.Y)
        && (g_sChar.m_cLocation.X == p1.m_cLocation.X))
    {
        // so bottom side will not follow
        if ((p1.m_cLocation.Y <= g_sChar.m_cLocation.Y)
            && (p1.m_cLocation.X == g_sChar.m_cLocation.X))
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

void stalkerMovement(SGameChar EArr[2])
{
    int dir = (rand() % 4) + 1;
    //Note: find a way to make them move individually, arr[i] applies to both same.
   
    for (int i = 0; i < 2; i++)
    {
        switch (dir)
        {
            //Up
        case 1:
            if (EArr[i].m_cLocation.Y > 0)
            {
                EArr[i].m_cLocation.Y--;
            }
            break;
            //Left
        case 2:
            if (EArr[i].m_cLocation.X > 0)
            {
                EArr[i].m_cLocation.X--;
            }
            break;
            //Down
        case 3:
            if (EArr[i].m_cLocation.Y < g_Console.getConsoleSize().Y - 1)
            {
                EArr[i].m_cLocation.Y++;
            }
            break;
            //Right
        case 4:
            if (EArr[i].m_cLocation.X < g_Console.getConsoleSize().X - 1)
            {
                EArr[i].m_cLocation.X++;

            }
            break;

        default:
            break;
        }
        if ((EArr[0].m_cLocation.X == EArr[1].m_cLocation.X) &&
            (EArr[0].m_cLocation.Y == EArr[1].m_cLocation.Y)) 
        {
            EArr[0].m_cLocation.Y -= 2;
            EArr[1].m_cLocation.X -= 2;
        }
    }
        
    stalkerSearchPlayer(Enemies);
    stalkerChasePlayer(Enemies);
}

bool stalkerSearchPlayer(SGameChar EArr[2])
{
    // Assume a 5x5 grid radius of range
    int x_final, y_final;
    int maxNumOfSteps = 0;
    // check if the player and enemy are in the same lane
    for (int i = 0; i < 2; i++)
    {
        if ((g_sChar.m_cLocation.Y == EArr[i].m_cLocation.Y + 5) ||
            (g_sChar.m_cLocation.X == EArr[i].m_cLocation.X + 5))
        {
            if (g_sChar.m_cLocation.X - EArr[i].m_cLocation.X <= 2)
            {
                return true;
            }
        }
        // check for other possibilities
        else
        {
            x_final = g_sChar.m_cLocation.X - EArr[i].m_cLocation.X;
            y_final = g_sChar.m_cLocation.Y - EArr[i].m_cLocation.Y;
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

void stalkerChasePlayer(SGameChar EArr[2]) 
{
    for (int i = 0; i < 2; i++)
    {
        if (stalkerSearchPlayer(Enemies) == true)
        {
            // After 10 steps stop chasing
            if ((g_sChar.m_cLocation.X - 10 <= EArr[i].m_cLocation.X) ||
                (g_sChar.m_cLocation.Y - 10 <= EArr[i].m_cLocation.Y))
            {
                if (g_sChar.m_cLocation.X > EArr[i].m_cLocation.X)
                {
                    EArr[i].m_cLocation.X++;
                }
                else if (g_sChar.m_cLocation.X < EArr[i].m_cLocation.X)
                {
                    EArr[i].m_cLocation.X--;
                }
                else if (g_sChar.m_cLocation.Y > EArr[i].m_cLocation.Y)
                {
                    EArr[i].m_cLocation.Y++;
                }
                else if (g_sChar.m_cLocation.Y < EArr[i].m_cLocation.Y)
                {
                    EArr[i].m_cLocation.Y--;
                }
            }
        }
    }
}

void processUserInput()
{
    // quits the game if player hits the escape key
    if (g_skKeyEvent[K_ESCAPE].keyReleased)
        g_bQuitGame = true;    
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
    case S_GAME: renderGame();
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

void renderToScreen()
{
    // Writes the buffer to the console, hence you will see what you have written
    g_Console.flushBufferToConsole();
}

void renderSplashScreen()  // renders the splash screen
{
    COORD c = g_Console.getConsoleSize();
    c.Y /= 3;
    c.X = c.X / 2 - 9;
    g_Console.writeToBuffer(c, "A game in 3 seconds", 0x03);
    c.Y += 1;
    c.X = g_Console.getConsoleSize().X / 2 - 20;
    g_Console.writeToBuffer(c, "Press <Space> to change character colour", 0x09);
    c.Y += 1;
    c.X = g_Console.getConsoleSize().X / 2 - 9;
    g_Console.writeToBuffer(c, "Press 'Esc' to quit", 0x09);
}

void renderGame()
{
    renderMap();        // renders the map to the buffer first
    renderCharacter();  // renders the character into the buffer
    renderEnemies(Enemies);    // renders the enemies into the buffer 
    //renderBossParticles(bossParticles);
    renderBoss(bossParticles);
}

void renderMap()
{
    // Set up sample colours, and output shadings
    const WORD colors[] = {
        0x1A, 0x2B, 0x3C, 0x4D, 0x5E, 0x6F,
        0xA1, 0xB2, 0xC3, 0xD4, 0xE5, 0xF6
    };

    COORD c;
    for (int i = 0; i < 12; ++i)
    {
        c.X = 5 * i;
        c.Y = i + 1;
        colour(colors[i]);
        g_Console.writeToBuffer(c, " °±²Û", colors[i]);
    }
}

// colour for character
void renderCharacter()
{
    // Draw the location of the character
    WORD charColor = 0x0C; // background colour of character when non active. (Sherryan)
    
    if (g_sChar.m_bActive)
    {
        charColor = 0x0A;
    }
    g_Console.writeToBuffer(g_sChar.m_cLocation, (char)1, charColor);
}

void renderEnemies(SGameChar EArr[2])
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
    for (int i = 0; i < 2; i++)
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
    COORD startPos = {50, 2};
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



