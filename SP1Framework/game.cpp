// This is the main file for the game logic and function
//
//
#include "game.h"
#include "Framework\console.h"
#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>
#include <sstream>
#define VK_KEY_W    0x57
#define VK_KEY_A    0x41
#define VK_KEY_S    0x53
#define VK_KEY_D    0x44
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
EGAMESTATES g_eGameState = S_SPLASHSCREEN; // initial state

// Console object
Console g_Console(300, 64, "ESCAPE THE DUNGEON");

//--------------------------------------------------------------
// Purpose  : Initialisation function
//            Initialize variables, allocate memory, load data from file, etc. 
//            This is called once before entering into your main loop
// Input    : void
// Output   : void
//--------------------------------------------------------------
void init( void )
{
    // Set precision for floating point output
    g_dElapsedTime = 0.0;    

    // sets the initial state for the game
    g_eGameState = S_SPLASHSCREEN;

    g_sChar.m_cLocation.X = g_Console.getConsoleSize().X / 2;
    g_sChar.m_cLocation.Y = g_Console.getConsoleSize().Y / 2;
    g_sChar.m_bActive = true;
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

void renderGame()
{
    renderMap();        // renders the map to the buffer first
    renderCharacter();  // renders the character into the buffer
}

void renderMap()
{
    std::ifstream infile("Map1stTest.txt");
    std::string var;
    // Init and store Map
    char map[65][300];
    int y = 0;
    while (getline(infile, var)) {
        // Output the text from the file
        for (unsigned i = 0; i < var.length(); ++i)
        {
            map[y][i] = var.at(i);

        }
        y++;
    }


    //render Map
    for (int y = 0; y < 65; y++)
    {
        for (int x = 0; x < 300; x++)
        {
            if (map[y][x] == '*')
            {
                g_Console.writeToBuffer(x, y, '#', 0x0F);

            }
            else if (map[y][x] == '#')
            {
                // walls
                g_Console.writeToBuffer(x, y, ' ', 0x00);
            }
            else
            {
                g_Console.writeToBuffer(x, y, ' ', 0x80);
            }
        }
    }    //renderstart(); //render first to last room
    //renderL1();
    //renderL2();
    //renderL3();
    //renderlast();
}

void renderstart()
{
    //gate
    renderlines(66, 11, 76, 12, 35); 
    renderlines(64, 11, 66, 12, 219);
    renderlines(76, 11, 78, 12, 219);

    //walls
    renderlines(30, 10, 31, 51, 219);
    renderlines(31, 10, 32, 51, 219);
    renderlines(30, 10, 111, 11, 219);
    renderlines(110, 10, 111, 51, 219);
    renderlines(111, 10, 112, 51, 219);
    renderlines(30, 50, 111, 11, 219);
    renderlines(66, 50, 76, 51, 43);

    //obstacles 1
    renderlines(43, 14, 59, 15, 254);
    renderlines(42, 14, 44, 15, 219);
    renderlines(59, 14, 61, 15, 219);
    renderlines(42, 15, 43, 18, 222);
    renderlines(43, 15, 44, 17, 221);
    renderlines(42, 17, 44, 18, 219);
    renderlines(38, 17, 42, 18, 254);
    renderlines(36, 17, 38, 18, 219);
    renderlines(36, 18, 37, 20, 222);
    renderlines(36, 18, 37, 20, 222);
    renderlines(37, 18, 38, 20, 221);
    renderlines(36, 20, 38, 21, 219);
    renderlines(38, 20, 42, 21, 254);
    renderlines(42, 18, 43, 20, 222);
    renderlines(43, 18, 44, 20, 221);
    renderlines(42, 20, 44, 21, 219);

    //obstacles 2
    renderlines(102, 20, 104, 21, 219); 
    renderlines(96, 20, 98, 21, 219);
    renderlines(102, 17, 104, 18, 219);
    renderlines(96, 17, 98, 18, 219);
    renderlines(96, 14, 98, 15, 219);
    renderlines(80, 14, 82, 15, 219);
    renderlines(82, 14, 96, 15, 254);
    renderlines(98, 20, 102, 21, 254);
    renderlines(98, 17, 102, 18, 254);
    renderlines(96, 15, 97, 17, 222);
    renderlines(97, 15, 98, 17, 221);
    renderlines(96, 18, 97, 20, 222);
    renderlines(97, 18, 98, 20, 221);
    renderlines(102, 18, 103, 20, 222);
    renderlines(103, 18, 104, 20, 221);

    //torch 1
    renderblock(80, 20, 186);
    renderblock(80, 19, 206);
    renderblock(80, 18, 178);
}

void renderL1()
{
    renderlines(1, 1, 2, 31, 219);
    renderlines(2, 1, 3, 31, 219);
    renderlines(61, 1, 62, 31, 219);
    renderlines(62, 1, 63, 31, 219);
    renderlines(1, 1, 61, 2, 219);
    renderlines(1, 30, 75, 31, 219);
    renderlines(61, 13, 80, 14, 219);
    renderlines(61, 18, 80, 19, 219);
    renderlines(80, 1, 81, 51, 219);
    renderlines(81, 1, 82, 51, 219);
    renderlines(200, 1, 201, 46, 219);
    renderlines(201, 1, 202, 46, 219);
    renderlines(80, 1, 200, 2, 219);
    renderlines(80, 30, 215, 31, 219);
    renderlines(40, 30, 41, 51, 219);
    renderlines(41, 30, 42, 51, 219);
    renderlines(40, 50, 210, 51, 219);
    renderlines(210, 33, 211, 51, 219);
    renderlines(211, 33, 212, 51, 219);
    renderlines(210, 33, 221, 34, 219);
    renderlines(220, 20, 221, 34, 219);
    renderlines(221, 20, 222, 34, 219);
    renderlines(200, 20, 220, 21, 219);
    renderlines(90, 30, 96, 31, 255);
    renderlines(80, 14, 81, 18, 255);
    renderlines(81, 14, 82, 18, 255);
    renderlines(80, 41, 81, 45, 255);
    renderlines(81, 41, 82, 45, 255);
    renderlines(61, 14, 62, 18, 255);
    renderlines(62, 14, 63, 18, 255);
}

void renderL2()
{
}

void renderlines(int xstart, int ystart, int xend, int yend, int symbol)
{
    for (int x = xstart; x < xend; x++)
    {
        c.Y = ystart;
        c.X = x;
        
        colour(colors[8]);
        g_Console.writeToBuffer(c, (char)symbol, colors[0]);
    }
    for (int y = ystart; y < yend; y++)
    {
        c.X = xstart;
        c.Y = y;
        colour(colors[8]);
        g_Console.writeToBuffer(c, (char)symbol, colors[0]);
    }
}

void renderblock(int xpos, int ypos, int hexa)
{
    c.X = xpos;
    c.Y = ypos;
    colour(colors[8]);
    g_Console.writeToBuffer(c, (char)hexa, colors[0]);
}

void renderarea(int xstart, int ystart, int xend, int yend, int symbol)
{
    for (int x = xstart; x < xend; x++)
    {
        for (int y = ystart; y < yend; y++)
        {
            c.X = xstart;
            c.Y = ystart;
            c.X = x;
            c.Y = y;

            colour(colors[8]);
            g_Console.writeToBuffer(c, (char)symbol, colors[0]);
        }
    }
}



void renderCharacter()
{
    // Draw the location of the character
    WORD charColor = 0x0C;
    if (g_sChar.m_bActive)
    {
        charColor = 0x0A;
    }
    g_Console.writeToBuffer(g_sChar.m_cLocation, (char)1, charColor);
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
    COORD startPos = {210, 2};
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



