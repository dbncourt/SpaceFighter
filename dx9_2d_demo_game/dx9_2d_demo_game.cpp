//-----------------------------------------------------------------------------
//           Name: dx9_2d_demo_game.cpp
//         Author: Kevin Harris
//  Last Modified: 02/01/05
//    Description: 2D Sample game written in DirectX 9.0
//       Controls: Use the arrow keys to move and the space bar to shoot!
//-----------------------------------------------------------------------------

#define STRICT
#define WIN32_LEAN_AND_MEAN

#define INITGUID
#define DIRECTINPUT_VERSION 0x0800

#include <windows.h>
#include <mmsystem.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <dinput.h>
#include "resource.h"
#include "sprite.h"
#include <list>
using namespace std;

#pragma comment(lib, "dxerr.lib") 
//-----------------------------------------------------------------------------
// SYMBOLIC CONSTANTS
//-----------------------------------------------------------------------------
const int SCREEN_WIDTH    = 800;
const int SCREEN_HEIGHT   = 600;
const int SCREEN_BPP      = 16;

const int TOTAL_STARS     = 100;
const int TOTAL_PARTICLES = 250;
const int SHIP_SPEED      = 5;

//-----------------------------------------------------------------------------
// MACROS
//-----------------------------------------------------------------------------
#define SAFE_DELETE(p)  { if(p) { delete (p);     (p)=NULL; } }
#define SAFE_RELEASE(p) { if(p) { (p)->Release(); (p)=NULL; } }
#define KEYDOWN(name,key) (name[key] & 0x80)

//-----------------------------------------------------------------------------
// GLOBALS
//-----------------------------------------------------------------------------
HWND                 g_hWnd       = NULL;
LPDIRECT3D9          g_pD3D       = NULL;
LPDIRECT3DDEVICE9    g_pd3dDevice = NULL;
LPDIRECTINPUT8       g_lpdi       = NULL;
LPDIRECTINPUTDEVICE8 g_pKeyboard  = NULL;

RECT  g_rcWindow;
RECT  g_rcViewport;
RECT  g_rcScreen;
BOOL  g_bWindowed       = true;
BOOL  g_bActive         = false;
DWORD g_dwLastFireTick  = 0;
int   g_nShipsLeft      = 3;
int   g_nScore          = 0;
int   g_nPowerLevel     = 20;
int   g_nCurrentLevel   = 0;

double g_dElpasedTime;
double g_dCurTime;
double g_dLastTime;
double g_dAnimationTimer = 0.0;

struct STAR
{
    int x;          // Star posit x
    int y;          // Star posit y
    int nVelocity;  // Star velocity
    COLORREF color; // Star color
};

STAR g_stars[TOTAL_STARS]; // Star field array

struct PARTICLE
{
    int  x;          // Particle posit x
    int  y;          // Particle posit y
    int  nVelocity;  // Particle velocity
    bool bVisible;   // Is particle visible or active
    COLORREF color;  // Particle color
};

PARTICLE g_exhaust[TOTAL_PARTICLES]; // Particle array for engine exhaust

// Create a linked list with STL to hold and manage CSprite objects
typedef list<CSprite> SPRITELIST;

SPRITELIST g_SpriteList;
SPRITELIST::iterator g_sprite_i;
SPRITELIST::iterator g_sprite_j;
SPRITELIST::iterator g_sprite_k;

//-----------------------------------------------------------------------------
// PROTOTYPES
//-----------------------------------------------------------------------------
LRESULT CALLBACK MainWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
HRESULT WinInit(HINSTANCE hInst, int nCmdShow, HWND* phWnd, HACCEL* phAccel);
HRESULT GameStart(HWND hWnd);
HRESULT GameMain(HWND hWnd);
void    GameOver(void);
HRESULT InitDirect3D(HWND hWnd, BOOL bWindowed);
void    FreeDirect3D(void);
HRESULT InitDirectInput(HWND hWnd);
void    FreeDirectInput(void);
void    InitializeSprites(void);
void    InitializeStars(void);
void    InitializeExhaust(void);
void    MoveShip(void);
void    MoveSprites(void);
void    MoveStars(void);
void    MoveExhaust(void);
void    CheckCollisions(void);
bool    SpriteCollisionTest(CSprite& sprite1, CSprite& sprite2);
bool    CollisionTest(int x1, int y1, int w1, int h1, 
                      int x2, int y2, int w2, int h2);
void    ComputeScore(void);
HRESULT DisplayFrame(void);
HRESULT DrawStars(void);
HRESULT DrawExhaust(void);
HRESULT DrawPowerBar(void);
int     RandomInt(int nLow, int nHigh);

//-----------------------------------------------------------------------------
// Name: WinMain()
// Desc: Entry point to the program. Initializes everything and calls.
//-----------------------------------------------------------------------------
int APIENTRY WinMain( HINSTANCE hInst, 
                      HINSTANCE hPrevInst, 
                      LPSTR     pCmdLine, 
                      int       nCmdShow )
{
    MSG    msg;
    HWND   hWnd;
    HACCEL hAccel;

    memset(&msg,0,sizeof(msg));
    
    WinInit( hInst, nCmdShow, &hWnd, &hAccel );

    GameStart( hWnd );

    while( TRUE )
    {
        // Look for messages, if none are found then 
        // update the state and display it
        if( PeekMessage( &msg, NULL, 0, 0, PM_NOREMOVE ) )
        {
            if( 0 == GetMessage(&msg, NULL, 0, 0 ) )
            {
                // WM_QUIT was posted, so exit
                return (int)msg.wParam;
            }

            // Translate and dispatch the message
            if( 0 == TranslateAccelerator( hWnd, hAccel, &msg ) )
            {
                TranslateMessage( &msg ); 
                DispatchMessage( &msg );
            }
        }
        else
        {
            if( g_bActive )
            {
                // Move the sprites, blt them to the back buffer, then 
                // flip or blt the back buffer to the primary buffer

                g_dCurTime     = timeGetTime();
                g_dElpasedTime = ((g_dCurTime - g_dLastTime) * 0.001);
                g_dLastTime    = g_dCurTime;

                if( FAILED( GameMain( hWnd ) ) )
                {
                    //SAFE_DELETE( g_pDisplay );

                    MessageBox( hWnd, TEXT("GameMain() failed. ")
                        TEXT("The game will now exit. "), TEXT("PrototypeX"), 
                        MB_ICONERROR | MB_OK );
                    return FALSE;
                }
            }
            else
            {
                // Go to sleep if we have nothing else to do
                WaitMessage();

                // Ignore time spent inactive 
                //g_dwLastTick = timeGetTime();
            }
        }
    }
}

//-----------------------------------------------------------------------------
// Name: WinInit()
// Desc: Init our game's window
//-----------------------------------------------------------------------------
HRESULT WinInit( HINSTANCE hInst, int nCmdShow, HWND *phWnd, HACCEL *phAccel )
{
    WNDCLASSEX wc;
    HWND       hWnd;
    HACCEL     hAccel;

    // Register the window class
    wc.cbSize        = sizeof(wc);
    wc.lpszClassName = TEXT("MY_WINDOWS_CLASS");
    wc.lpfnWndProc   = MainWndProc;
    wc.style         = CS_VREDRAW | CS_HREDRAW;
    wc.hInstance     = hInst;
    wc.hIcon         = LoadIcon( hInst, MAKEINTRESOURCE(IDI_MAIN_ICON) );
    wc.hIconSm       = LoadIcon( hInst, MAKEINTRESOURCE(IDI_MAIN_ICON) );
    wc.hCursor       = LoadCursor( NULL, IDC_ARROW );
    wc.hbrBackground = (HBRUSH) (COLOR_WINDOW + 1);
    wc.lpszMenuName  = MAKEINTRESOURCE(IDR_MENU);
    wc.cbClsExtra    = 0;
    wc.cbWndExtra    = 0;

    if( RegisterClassEx( &wc ) == 0 )
        return E_FAIL;

    // Load keyboard accelerators
    hAccel = LoadAccelerators( hInst, MAKEINTRESOURCE(IDR_MAIN_ACCEL) );

    // Calculate the proper size for the window given a client of 640x480
    DWORD dwFrameWidth    = GetSystemMetrics( SM_CXSIZEFRAME );
    DWORD dwFrameHeight   = GetSystemMetrics( SM_CYSIZEFRAME );
    DWORD dwMenuHeight    = GetSystemMetrics( SM_CYMENU );
    DWORD dwCaptionHeight = GetSystemMetrics( SM_CYCAPTION );
    DWORD dwWindowWidth   = SCREEN_WIDTH  + dwFrameWidth * 2;
    DWORD dwWindowHeight  = SCREEN_HEIGHT + dwFrameHeight * 2 + 
                            dwMenuHeight + dwCaptionHeight;

    // Create and show the main window
    DWORD dwStyle = WS_OVERLAPPEDWINDOW & ~WS_MAXIMIZEBOX;
    hWnd = CreateWindowEx( 0, TEXT("MY_WINDOWS_CLASS"), 
                           TEXT("Direct3D (DX8) - 2D Demo Game"),
                           dwStyle, CW_USEDEFAULT, CW_USEDEFAULT,
                           dwWindowWidth, dwWindowHeight, NULL, NULL, hInst, NULL );
    if( hWnd == NULL )
        return E_FAIL;

    ShowWindow( hWnd, nCmdShow );
    UpdateWindow( hWnd );

    // Save the window size/pos for switching modes
    GetWindowRect( hWnd, &g_rcWindow );

    *phWnd   = hWnd;
    *phAccel = hAccel;

    return S_OK;
}

//-----------------------------------------------------------------------------
// Name: MainWndProc()
// Desc: The main window procedure
//-----------------------------------------------------------------------------
LRESULT CALLBACK MainWndProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
    switch( msg )
    {
        case WM_COMMAND:
            switch( LOWORD(wParam) )
            {
                case IDM_TOGGLEFULLSCREEN:
/*
                    // Toggle the fullscreen/window mode
                    if( g_bWindowed )
                        GetWindowRect( hWnd, &g_rcWindow );

                    g_bWindowed = !g_bWindowed;

SAFE_RELEASE( g_pd3dDevice );
//FreeDirect3D();

                    if( FAILED( InitDirect3D( hWnd, g_bWindowed ) ) )
                    {
                        SAFE_RELEASE( g_pd3dDevice );

                        MessageBox( hWnd, TEXT("InitDirect3D() failed. ")
                                    TEXT("The game will now exit. "), TEXT("PrototypeX"), 
                                    MB_ICONERROR | MB_OK );
                        PostMessage( hWnd, WM_CLOSE, 0, 0 );
                    }
//*/
                    return 0L;

                case IDM_EXIT:
                    // Received key/menu command to exit app
                    PostMessage( hWnd, WM_CLOSE, 0, 0 );
                    return 0L;
            }
            break;

        case WM_GETMINMAXINFO:
            {
                // Don't allow resizing in windowed mode.  
                // Fix the size of the window to 800x600 (client size)
                MINMAXINFO* pMinMax = (MINMAXINFO*) lParam;

                DWORD dwFrameWidth    = GetSystemMetrics( SM_CXSIZEFRAME );
                DWORD dwFrameHeight   = GetSystemMetrics( SM_CYSIZEFRAME );
                DWORD dwMenuHeight    = GetSystemMetrics( SM_CYMENU );
                DWORD dwCaptionHeight = GetSystemMetrics( SM_CYCAPTION );

                pMinMax->ptMinTrackSize.x = SCREEN_WIDTH  + dwFrameWidth * 2;
                pMinMax->ptMinTrackSize.y = SCREEN_HEIGHT + dwFrameHeight * 2 + 
                                            dwMenuHeight + dwCaptionHeight;

                pMinMax->ptMaxTrackSize.x = pMinMax->ptMinTrackSize.x;
                pMinMax->ptMaxTrackSize.y = pMinMax->ptMinTrackSize.y;
            }
            return 0L;

        case WM_SIZE:
            // Check to see if we are losing our window...
            if( SIZE_MAXHIDE == wParam || SIZE_MINIMIZED == wParam )
                g_bActive = FALSE;
            else
                g_bActive = TRUE;
            break;

        case WM_SETCURSOR:
            // Hide the cursor if in full screen 
            if( !g_bWindowed )
            {
                SetCursor( NULL );
                return TRUE;
            }
            break;

        case WM_EXITMENULOOP:
            // Ignore time spent in menu
            //g_dwLastTick = timeGetTime();
            break;

        case WM_EXITSIZEMOVE:
            // Ignore time spent resizing
            //g_dwLastTick = timeGetTime();
            break;

        case WM_SYSCOMMAND:
            // Prevent moving/sizing and power loss in full screen mode
            switch( wParam )
            {
                case SC_MOVE:
                case SC_SIZE:
                case SC_MAXIMIZE:
                case SC_MONITORPOWER:
                    if( !g_bWindowed )
                        return TRUE;
            }
            break;
            
        case WM_ACTIVATE:
            if( WA_INACTIVE != wParam && g_pKeyboard )
            {
                // Make sure the device is acquired, if we are gaining focus.
                g_pKeyboard->Acquire();
            }

            break;

        case WM_DESTROY:
            // Cleanup and close the app
            GameOver();
            PostQuitMessage( 0 );
            return 0L;
    }

    return DefWindowProc(hWnd, msg, wParam, lParam);
}

//-----------------------------------------------------------------------------
// Name: GameStart()
// Desc: Initialize all DirectX objects to be used
//-----------------------------------------------------------------------------
HRESULT GameStart( HWND hWnd )
{
    HRESULT hr;

    // Initialize all the surfaces we need
    if( FAILED( hr = InitDirect3D( hWnd, g_bWindowed ) ) )
        return hr;

    // Initialize all DirectInput
    if( FAILED( hr = InitDirectInput( hWnd ) ) )
        return hr;

    InitializeSprites();
    InitializeStars();
    InitializeExhaust();

    g_dwLastFireTick = timeGetTime();

    return S_OK;
}

//-----------------------------------------------------------------------------
// Name: GameMain()
// Desc: Move the sprites, blit them to the back buffer, then 
//       flip or blit the back buffer to the primary buffer
//-----------------------------------------------------------------------------
HRESULT GameMain( HWND hWnd )
{
    g_dAnimationTimer += g_dElpasedTime;

    if( g_dAnimationTimer >= 0.016 )
        g_dAnimationTimer = 0.0; // Target of 1/60th of a second (60 FPS) reached. Render a new frame.
    else
        return S_OK; // It's too early. Return now and render nothing.

    // Collect user input and move the player's ship
    MoveShip();

    // Move the game sprites
    MoveSprites();

    // Check to see who's hitting who and react to it
    CheckCollisions();

    // Assign the proper number sprites for the score
    ComputeScore();

    // Display the sprites on the screen
    DisplayFrame();

    return S_OK;
}

//-----------------------------------------------------------------------------
// Name: GameOver()
// Desc: Release all the DirectDraw objects
//-----------------------------------------------------------------------------
VOID GameOver()
{
    FreeDirect3D();
    FreeDirectInput();

    for( g_sprite_i = g_SpriteList.begin(); g_sprite_i != g_SpriteList.end(); ++g_sprite_i )
        g_sprite_i->releaseMemory();

    // Cleanup the STL generated linked list
    g_SpriteList.erase( g_SpriteList.begin(), g_SpriteList.end() );
}

//-----------------------------------------------------------------------------
// Name: InitDirect3D()
// Desc: 
//-----------------------------------------------------------------------------
HRESULT InitDirect3D( HWND hWnd, BOOL bWindowed )
{
    g_pD3D = Direct3DCreate9( D3D_SDK_VERSION );

    D3DCAPS9 d3dCaps;
    g_pD3D->GetDeviceCaps( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &d3dCaps );

    D3DDISPLAYMODE d3ddm;
    g_pD3D->GetAdapterDisplayMode( D3DADAPTER_DEFAULT, &d3ddm );

    D3DPRESENT_PARAMETERS d3dpp;
    ZeroMemory( &d3dpp, sizeof(d3dpp) );

    if( bWindowed == TRUE )
    {
        d3dpp.Windowed         = TRUE;
        d3dpp.SwapEffect       = D3DSWAPEFFECT_DISCARD;
        d3dpp.BackBufferFormat = d3ddm.Format;
    }
    else
    {
        d3dpp.Windowed         = FALSE;
        d3dpp.SwapEffect       = D3DSWAPEFFECT_DISCARD;
        d3dpp.BackBufferWidth  = SCREEN_WIDTH;
        d3dpp.BackBufferHeight = SCREEN_HEIGHT;
        d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8;
    }

    d3dpp.EnableAutoDepthStencil = TRUE;
    d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
    d3dpp.PresentationInterval   = D3DPRESENT_INTERVAL_IMMEDIATE; // Do NOT sync to vertical retrace
    //d3dpp.PresentationInterval   = D3DPRESENT_INTERVAL_DEFAULT; // Sync to vertical retrace
    d3dpp.Flags                  = D3DPRESENTFLAG_LOCKABLE_BACKBUFFER;

    g_pD3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
                          D3DCREATE_SOFTWARE_VERTEXPROCESSING,
                          &d3dpp, &g_pd3dDevice );

    D3DXMATRIX matProj;
    D3DXMatrixPerspectiveFovLH( &matProj, D3DXToRadian( 45.0f ), 
                                SCREEN_WIDTH / SCREEN_HEIGHT, 0.1f, 100.0f );
    g_pd3dDevice->SetTransform( D3DTS_PROJECTION, &matProj );

    g_pd3dDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
    g_pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

    return S_OK;
}

//-----------------------------------------------------------------------------
// Name: FreeDirect3D()
// Desc: Free all DirectDraw objects
//-----------------------------------------------------------------------------
void FreeDirect3D( void )
{
    SAFE_RELEASE( g_pd3dDevice );
    SAFE_RELEASE( g_pD3D );
}

//-----------------------------------------------------------------------------
// Name: InitDirectInput()
// Desc: Creates a keyboard device using DirectInput
//-----------------------------------------------------------------------------
HRESULT InitDirectInput( HWND hWnd )
{
    HRESULT hr;

    FreeDirectInput();

    // Create a DInput object
    if( FAILED( hr = DirectInput8Create( GetModuleHandle(NULL), DIRECTINPUT_VERSION, 
        IID_IDirectInput8, (VOID**)&g_lpdi, NULL ) ) )
        return hr;

    // Obtain an interface to the system keyboard device.
    if( FAILED( hr = g_lpdi->CreateDevice( GUID_SysKeyboard, &g_pKeyboard, NULL ) ) )
        return hr;

    // Set the data format to "keyboard format" - a predefined data format 
    //
    // A data format specifies which controls on a device we
    // are interested in, and how they should be reported.
    //
    // This tells DirectInput that we will be passing an array
    // of 256 bytes to IDirectInputDevice::GetDeviceState.
    if( FAILED( hr = g_pKeyboard->SetDataFormat( &c_dfDIKeyboard ) ) )
        return hr;

    // Set the cooperative level to let DirectInput know how
    // this device should interact with the system and with other
    // DirectInput applications.
    hr = g_pKeyboard->SetCooperativeLevel( hWnd, DISCL_NOWINKEY | 
		                                         DISCL_NONEXCLUSIVE | 
												 DISCL_FOREGROUND );

    if( hr == DIERR_UNSUPPORTED )
    {
        FreeDirectInput();
        MessageBox( hWnd, TEXT("SetCooperativeLevel() returned DIERR_UNSUPPORTED.\n")
            TEXT("For security reasons, background exclusive keyboard\n")
            TEXT("access is not allowed."), TEXT("PrototypeX"), MB_OK );
        return S_OK;
    }

    if( FAILED(hr) )
        return hr;

    // Acquire the newly created device
    g_pKeyboard->Acquire();

    return S_OK;
}

//-----------------------------------------------------------------------------
// Name: FreeDirectInput()
// Desc: Initialize the DirectInput variables.
//-----------------------------------------------------------------------------
void FreeDirectInput( void )
{
    // Unacquire the device one last time just in case 
    // the app tried to exit while the device is still acquired.
    if( g_pKeyboard ) 
        g_pKeyboard->Unacquire();

    // Release any DirectInput objects.
    SAFE_RELEASE( g_pKeyboard );
    SAFE_RELEASE( g_lpdi );
}

//-----------------------------------------------------------------------------
// Name: InitializeSprites()
// Desc: Creates each sprite, sets their properties, and then loads them 
//       into a linked list for future use.
//-----------------------------------------------------------------------------
void InitializeSprites(void)
{
    CSprite sprite;
    int i = 0;

    sprite.zeroSpriteValues();
    sprite.m_nID                = 555;
    strcpy( sprite.m_chType, "ship" );
    strcpy( sprite.m_chSpriteTextureName, "fighter.bmp" );
    sprite.m_nWidth             = 1152;
    sprite.m_nHeight            = 216;
    sprite.m_fPosition_x        = 0;
    sprite.m_fPosition_y        = 250;
    sprite.m_bAutoAnimate       = false; // We'll control the animation ourself through incFrame() and decFrame()
    sprite.m_nFrameRateModifier = -2;
    sprite.m_nCurrentFrame      = 14;  // Point forward until further notice!
    sprite.m_nFrameWidth        = 144;
    sprite.m_nFrameHeight       = 108;
    sprite.m_nFramesAcross      = 8;
    sprite.m_nWidthScaling      = -10; // Shrink the sprite's width
    sprite.m_nHeightScaling     = -10; // Shrink the sprite's height
    sprite.m_bModifyCollision   = true;
    sprite.m_nCollisionRight    = -5;  // Reduce sprite 's collision area on the right side
    sprite.m_nCollisionLeft     = -15; // Reduce sprite 's collision area on the left side
    sprite.m_nCollisionBottom   = -50; // Reduce sprite 's collision area on the bottom
    sprite.m_nCollisionTop      = -20; // Reduce sprite 's collision area on the top
    sprite.loadAnimationString( 0, 
        "7 7 6 6 5 5 4 4 3 3 2 2 1 1 0 0 9 9 10 10 11 11 12 12 13 13 14 14 15 15", 
        CSprite::MAINTAIN_LAST_FRAME );

    g_SpriteList.push_back(sprite);

    // Create an animated sprite for engine exhaust
    sprite.zeroSpriteValues();
    strcpy( sprite.m_chType, "animeflame" );
    strcpy( sprite.m_chSpriteTextureName, "misc.bmp" );
    sprite.m_nWidth             = 200;
    sprite.m_nHeight            = 200;
    sprite.m_bActive            = false;
    sprite.m_fPosition_x        = 0;
    sprite.m_fPosition_y        = 0;
    sprite.m_nFrameRateModifier = -10;
    sprite.m_nFrameOffset_x     = 0;
    sprite.m_nFrameOffset_y     = 74;
    sprite.m_nFramesAcross      = 3;
    sprite.m_nFrameWidth        = 46;
    sprite.m_nFrameHeight       = 13;
    sprite.loadAnimation( 0, 0, 5, CSprite::MAINTAIN_LAST_FRAME );

    g_SpriteList.push_back(sprite);

    // Create a sprite for keeping track of the number 
    // of ships the player has left
    sprite.zeroSpriteValues();
    strcpy( sprite.m_chType, "shipcount" );
    strcpy( sprite.m_chSpriteTextureName, "misc.bmp" );
    sprite.m_nWidth             = 200;
    sprite.m_nHeight            = 200;
    sprite.m_bSingleFrame       = true;
    sprite.m_fPosition_x        = 375;
    sprite.m_fPosition_y        = 5;
    sprite.m_nFrameOffset_x     = 0;
    sprite.m_nFrameOffset_y     = 17;
    sprite.m_nFrameWidth        = 67;
    sprite.m_nFrameHeight       = 38;
    
    g_SpriteList.push_back(sprite);

    // Create a single number sprite for keeping track of 
    // the number of ships the player has left
    sprite.zeroSpriteValues();
    strcpy( sprite.m_chType, "shipnumber" );
    strcpy( sprite.m_chSpriteTextureName, "numbers.bmp" );
    sprite.m_nWidth             = 185;
    sprite.m_nHeight            = 27;
    sprite.m_fPosition_x        = 428;
    sprite.m_fPosition_y        = 28;
    sprite.m_bAutoAnimate       = false;
    sprite.m_nFrameWidth        = 17;
    sprite.m_nFrameHeight       = 17;
    sprite.m_nFramesAcross      = 10;
    sprite.m_nFrameOffset_x     =  9;
    sprite.m_nFrameOffset_y     =  5;
    sprite.loadAnimation( 0, 0, 9, CSprite::LOOP_ANIMATION );

    g_SpriteList.push_back(sprite);

    // Create power bar sprite
    sprite.zeroSpriteValues();
    strcpy( sprite.m_chType, "powerbar" );
    strcpy( sprite.m_chSpriteTextureName, "misc.bmp" );
    sprite.m_nWidth             = 200;
    sprite.m_nHeight            = 200;
    sprite.m_bActive            = false;
    sprite.m_bSingleFrame       = true;
    sprite.m_nFrameOffset_x     = 0;
    sprite.m_nFrameOffset_y     = 56;
    sprite.m_nFrameWidth        = 105;
    sprite.m_nFrameHeight       = 17;

    g_SpriteList.push_back(sprite);

    // Create a red hashmark for the power level display
    sprite.zeroSpriteValues();
    strcpy( sprite.m_chType, "redmark" );
    strcpy( sprite.m_chSpriteTextureName, "misc.bmp" );
    sprite.m_nWidth             = 200;
    sprite.m_nHeight            = 200;
    sprite.m_bActive            = false;
    sprite.m_bSingleFrame       = true;
    sprite.m_nFrameOffset_x     = 51;
    sprite.m_nFrameOffset_y     = 0;
    sprite.m_nFrameWidth        = 4;
    sprite.m_nFrameHeight       = 13;

    g_SpriteList.push_back(sprite);

    // Create a yellow hashmark for the power level display
    sprite.zeroSpriteValues();
    strcpy( sprite.m_chType, "yellowmark" );
    strcpy( sprite.m_chSpriteTextureName, "misc.bmp" );
    sprite.m_nWidth             = 200;
    sprite.m_nHeight            = 200;
    sprite.m_bActive            = false;
    sprite.m_bSingleFrame       = true;
    sprite.m_nFrameOffset_x     = 56;
    sprite.m_nFrameOffset_y     = 0;
    sprite.m_nFrameWidth        = 4;
    sprite.m_nFrameHeight       = 13;
    
    g_SpriteList.push_back(sprite);

    // Create a green hashmark for the power level display
    sprite.zeroSpriteValues();
    strcpy( sprite.m_chType, "greenmark" );
    strcpy( sprite.m_chSpriteTextureName, "misc.bmp" );
    sprite.m_nWidth             = 200;
    sprite.m_nHeight            = 200;
    sprite.m_bActive            = false;
    sprite.m_bSingleFrame       = true;
    sprite.m_nFrameOffset_x     = 61;
    sprite.m_nFrameOffset_y     = 0;
    sprite.m_nFrameWidth        = 4;
    sprite.m_nFrameHeight       = 13;

    g_SpriteList.push_back(sprite);

    // Create 4 numbers for keeping score
    for( i = 0; i < 4; i++ )
    {
        sprite.zeroSpriteValues();
        strcpy( sprite.m_chType, "numbers" );
        strcpy( sprite.m_chSpriteTextureName, "numbers.bmp" );
        sprite.m_nWidth             = 185;
        sprite.m_nHeight            = 27;
        sprite.m_nState             = i;
        sprite.m_fPosition_x        = (float)300 + (i * 17);
        sprite.m_fPosition_y        = 5;
        sprite.m_bAutoAnimate       = false;
        sprite.m_nFrameOffset_x     =  9;
        sprite.m_nFrameOffset_y     =  5;
        sprite.m_nFramesAcross      = 10;
        sprite.m_nFrameWidth        = 17;
        sprite.m_nFrameHeight       = 17;
        sprite.loadAnimationString( 0, "0 1 2 3 4 5 6 7 8 9", CSprite::LOOP_ANIMATION );

        g_SpriteList.push_back(sprite);
    }

    // Create 20 mines to shoot at!
    for( i = 0; i < 20; i++ )
    {
        sprite.zeroSpriteValues();
        strcpy( sprite.m_chType, "mine" );
        strcpy( sprite.m_chSpriteTextureName, "misc.bmp" );
        sprite.m_nWidth             = 200;
        sprite.m_nHeight            = 200;
        sprite.m_bSingleFrame      = true;
        sprite.m_fPosition_x       = 800;
        sprite.m_fPosition_y       = (float)RandomInt( 0, 560 );
        sprite.m_fVelocity_x       = (float)-(RandomInt( 1, 10 ));
        sprite.m_nFrameWidth       = 16;
        sprite.m_nFrameHeight      = 16;
        sprite.m_nFrameOffset_x    = 16;
        sprite.m_nFrameOffset_y    = 1;

        g_SpriteList.push_back(sprite);
    }

    // Create 20 laser shots for blowing up mines!
    for( i = 0; i < 20; i++ )
    {
        sprite.zeroSpriteValues();
        strcpy( sprite.m_chType, "shot" );
        strcpy( sprite.m_chSpriteTextureName, "misc.bmp" );
        sprite.m_nWidth             = 200;
        sprite.m_nHeight            = 200;
        sprite.m_bActive            = false;
        sprite.m_bSingleFrame       = true;
        sprite.m_nFrameOffset_x     = 32;
        sprite.m_nFrameOffset_y     = 0;
        sprite.m_nFrameWidth        = 18;
        sprite.m_nFrameHeight       = 3;

        g_SpriteList.push_back(sprite);
    }

    // Create 20 explosions to cover each possible laser impact with a mine!
    for( i = 0; i < 20; i++ )
    {
        sprite.zeroSpriteValues();
        strcpy( sprite.m_chType, "explosion" );
        strcpy( sprite.m_chSpriteTextureName, "explosion.bmp" );
        sprite.m_nWidth             = 294;
        sprite.m_nHeight            = 72;
        sprite.m_bActive            = false;
        sprite.m_nFramesAcross      = 7;
        sprite.m_nFrameWidth        = 42;
        sprite.m_nFrameHeight       = 36;
        sprite.m_nFrameRateModifier = -3;
        sprite.loadAnimation( 0, 0, 13, CSprite::GO_INACTIVE );

        g_SpriteList.push_back(sprite);
    }
}

//-----------------------------------------------------------------------------
// Name: InitializeStars()
// Desc: Give each star a random starting position and velocity
//-----------------------------------------------------------------------------
void InitializeStars(void)
{
    for( int i = 0; i < TOTAL_STARS; i++ )
    {
        g_stars[i].x = rand()%SCREEN_WIDTH;
        g_stars[i].y = rand()%SCREEN_HEIGHT;
        g_stars[i].nVelocity = 1 + rand()%16;
        g_stars[i].color = RGB(255,255,255);
    }
}

//-----------------------------------------------------------------------------
// Name: InitializeExhaust()
// Desc: Initialize all exhaust particles
//-----------------------------------------------------------------------------
void InitializeExhaust(void)
{
    for( int i = 0; i < TOTAL_PARTICLES; i++ )
    {
        g_exhaust[i].x = 0;
        g_exhaust[i].y = 0;
        g_exhaust[i].nVelocity = 1 + rand()%16;
        g_exhaust[i].color = 700;
        g_exhaust[i].bVisible = false;
    }
}

//-----------------------------------------------------------------------------
// Name: MoveShip()
// Desc: Collects user input and acts on it by modifying the ship sprite
//-----------------------------------------------------------------------------
void MoveShip(void)
{
    char buffer[256];
    int  nShips_x = 0;
    int  nShips_y = 0;
    bool bFlameOn = false;

    // Get keyboard input 
    g_pKeyboard->GetDeviceState(sizeof(buffer),(LPVOID)&buffer); 

    // Find the ship and modify it...
    for( g_sprite_i = g_SpriteList.begin(); g_sprite_i != g_SpriteList.end(); ++g_sprite_i )
    {
        if( !lstrcmp(g_sprite_i->m_chType, "ship") )
        {
            if( KEYDOWN(buffer, DIK_UP) )
            {
                if( g_sprite_i->m_fPosition_y > 0 )
                    g_sprite_i->m_fPosition_y -= SHIP_SPEED;

                if( g_sprite_i->m_nCurrentFrame < 28 )
                    g_sprite_i->incFrame();
            }
            else if( KEYDOWN(buffer, DIK_DOWN) )
            {
                if( g_sprite_i->m_fPosition_y < 520 )
                    g_sprite_i->m_fPosition_y += SHIP_SPEED;

                if( g_sprite_i->m_nCurrentFrame > 0 )
                    g_sprite_i->decFrame();
            }
            else
            {
                if( g_sprite_i->m_nCurrentFrame > 14)
                    g_sprite_i->decFrame();

                if( g_sprite_i->m_nCurrentFrame < 14)
                    g_sprite_i->incFrame();
            }

            if( KEYDOWN(buffer, DIK_LEFT) )
            {
                if( g_sprite_i->m_fPosition_x > 0 )
                    g_sprite_i->m_fPosition_x -= SHIP_SPEED;
            }

            if( KEYDOWN(buffer, DIK_RIGHT) )
            {
                if( g_sprite_i->m_fPosition_x < (SCREEN_WIDTH - 144))
                    g_sprite_i->m_fPosition_x += SHIP_SPEED;

                nShips_x = (int)g_sprite_i->m_fPosition_x;
                nShips_y = (int)g_sprite_i->m_fPosition_y;

                bFlameOn = true;
            }

            if( KEYDOWN(buffer, DIK_SPACE) )
            {
                /*
                // Single shot laser fire
                for( g_sprite_j = g_SpriteList.begin(); g_sprite_j != g_SpriteList.end(); ++g_sprite_j )
                {
                    if( !lstrcmp(g_sprite_j->m_chType, "shot") && g_sprite_j->m_bActive == false )
                    {
                        g_sprite_j->m_fPosition_x = ( g_sprite_i->m_fPosition_x + 120 );
                        g_sprite_j->m_fPosition_y = ( g_sprite_i->m_fPosition_y + 48 );
                        g_sprite_j->m_fVelocity_x = 20;
                        g_sprite_j->m_bActive = true;
                        break;
                    }
                }
                //*/

                //*
                // Triple shot laser fire

                // Figure how much time has passed since the last tri-shot
                DWORD dwCurrTick = timeGetTime();
                DWORD dwTickDiff = dwCurrTick - g_dwLastFireTick;
                int nShots = 0;

                if( dwTickDiff >= 75 )
                {
                    for( g_sprite_j = g_SpriteList.begin(); g_sprite_j != g_SpriteList.end(); ++g_sprite_j )
                    {
                        if( !lstrcmp(g_sprite_j->m_chType, "shot") && g_sprite_j->m_bActive == false )
                        {
                            g_sprite_j->m_fPosition_x = ( g_sprite_i->m_fPosition_x + 120 );
                            g_sprite_j->m_fPosition_y = ( g_sprite_i->m_fPosition_y + 48 );
                            g_sprite_j->m_bActive = true;

                            if( nShots == 0 )
                                g_sprite_j->m_fVelocity_x = 20;

                            if( nShots == 1 )
                            {
                                g_sprite_j->m_fVelocity_x = 20;
                                g_sprite_j->m_fVelocity_y = 2;
                            }

                            if( nShots == 2 )
                            {
                                g_sprite_j->m_fVelocity_x = 20;
                                g_sprite_j->m_fVelocity_y = -2;
                            }

                            ++nShots;
                            if( nShots == 3 )
                                break;
                        }
                    }

                    g_dwLastFireTick = dwCurrTick;
                }
                //*/
            }
            break;
        }
    }

    for( g_sprite_i = g_SpriteList.begin(); g_sprite_i != g_SpriteList.end(); ++g_sprite_i )
    {
        if( !lstrcmp(g_sprite_i->m_chType, "animeflame") )
        {
            if( bFlameOn == true )
            {
                g_sprite_i->m_bActive = true;
                g_sprite_i->m_fPosition_x = (float)(nShips_x - 40);
                g_sprite_i->m_fPosition_y = (float)(nShips_y + 50);

                for( int i = 0; i< TOTAL_PARTICLES; i++ )
                {
                    if( g_exhaust[i].bVisible == false )
                    {
                        int nCenter = (nShips_y + 56);
                        g_exhaust[i].x = nShips_x;
                        g_exhaust[i].y = RandomInt( (nCenter - 10), (nCenter + 10) );
                        g_exhaust[i].bVisible = true;
                        break;
                    }
                }

                bFlameOn = false;
            }
            else
            {
                g_sprite_i->m_bActive = false;
                g_sprite_i->m_nCurrentFrame = 0;

            }
            break;
        }
    }
}

//-----------------------------------------------------------------------------
// Name: MoveSprites()
// Desc: Identifies sprites in the linked list and moves each one accordingly
//-----------------------------------------------------------------------------
void MoveSprites()
{
    // Move all active player shots
    
    for( g_sprite_i = g_SpriteList.begin(); g_sprite_i != g_SpriteList.end(); ++g_sprite_i )
    {
        if( !lstrcmp(g_sprite_i->m_chType, "shot") && g_sprite_i->m_bActive == true )
        {
            if( g_sprite_i->m_fPosition_x < SCREEN_WIDTH )
            {
                g_sprite_i->m_fPosition_x += g_sprite_i->m_fVelocity_x;
                g_sprite_i->m_fPosition_y += g_sprite_i->m_fVelocity_y;
            }
            else
            {
                // Recycle player shots that have left the viewable area!
                g_sprite_i->m_bActive = false;
                g_sprite_i->m_fPosition_x = 0;
                g_sprite_i->m_fPosition_y = 0;
                g_sprite_i->m_fVelocity_x = 0;
                g_sprite_i->m_fVelocity_y = 0;
            }
        }

        if( !lstrcmp(g_sprite_i->m_chType, "mine")  )
        {
            g_sprite_i->m_fPosition_x += g_sprite_i->m_fVelocity_x;
            g_sprite_i->m_fPosition_y += g_sprite_i->m_fVelocity_y;

            if( g_sprite_i->m_fPosition_x > 800 )
                g_sprite_i->m_fPosition_x = 0;

            if( g_sprite_i->m_fPosition_x < -64 )
            {
                g_sprite_i->m_fPosition_x = 800;
                g_sprite_i->m_fPosition_y = (float)RandomInt( 0, 560 );
                g_sprite_i->m_fVelocity_x = (float)-(RandomInt( 1, 10 ));
            }
        }
    }
}

//-----------------------------------------------------------------------------
// Name: MoveStars()
// Desc: Update each star's position according to its velocity
//-----------------------------------------------------------------------------
void MoveStars(void)
{
    // Scroll stars to the left
    for( int i = 0; i< TOTAL_STARS; i++ )
    {
        // Move the star
        g_stars[i].x -= g_stars[i].nVelocity;

        // If the star falls off the screen's edge, wrap it around
        if( g_stars[i].x <= 0 )
            g_stars[i].x = SCREEN_WIDTH;
    }
}

//-----------------------------------------------------------------------------
// Name: MoveExhaust()
// Desc: Move all visible exhaust particles to the right
//-----------------------------------------------------------------------------
void MoveExhaust(void)
{
    // Move particles to the left
    for( int i = 0; i < TOTAL_PARTICLES; i++ )
    {
        // Move the particle
        if( g_exhaust[i].bVisible == true )
        {
            g_exhaust[i].x -= g_exhaust[i].nVelocity;

            if( g_exhaust[i].x <= 0 )
            {
                g_exhaust[i].x = 1;
                g_exhaust[i].bVisible = false;
            }
        }
    }
}

//-----------------------------------------------------------------------------
// Name: CheckCollisions()
// Desc: Search the sprite linked-list for sprites that touch
//-----------------------------------------------------------------------------
void CheckCollisions(void)
{
    // Perfrom collision detection 
    int nCount1 = 0;
    int nCount2 = 0;
    int size = g_SpriteList.size();

    for( g_sprite_i = g_SpriteList.begin(); g_sprite_i != g_SpriteList.end(); ++g_sprite_i )
    {
        ++nCount1; // Next sprite to process

        for( g_sprite_j = g_SpriteList.begin(); g_sprite_j != g_SpriteList.end(); ++g_sprite_j )
        {
            // Don't check g_sprite_i against sprites that it's already been checked against
            while( nCount2 != nCount1 )
            {
                ++g_sprite_j; // Don't check sprites that've already been checked.
                ++nCount2;    // Skip ahead...
            }

            // Don't bother checking the last sprite...
            if( nCount2 == size )
                break;

            if( g_sprite_i->m_bCollide == true &&
                g_sprite_j->m_bCollide == true &&
                g_sprite_i->m_bActive  == true &&
                g_sprite_j->m_bActive  == true)
            {
                if( SpriteCollisionTest( *g_sprite_i, *g_sprite_j ) )
                {
                    // Has a mine hit a shot?
                    if( !lstrcmp(g_sprite_i->m_chType, "mine") && !lstrcmp(g_sprite_j->m_chType, "shot") )
                    {
                        g_nScore += 5;

                        // Kill shot
                        g_sprite_j->m_bActive = false;

                        // Find an explosion sprite to assign
                        for( g_sprite_k = g_SpriteList.begin(); g_sprite_k != g_SpriteList.end(); ++g_sprite_k )
                        {
                            if( !lstrcmp(g_sprite_k->m_chType, "explosion") && g_sprite_k->m_bActive == false )
                            {
                                // Assign explosion to the collided mine's position and speed
                                g_sprite_k->m_fPosition_x = (g_sprite_i->m_fPosition_x - 10);
                                g_sprite_k->m_fPosition_y = (g_sprite_i->m_fPosition_y - 20);
                                g_sprite_k->m_fVelocity_x = g_sprite_i->m_fVelocity_x;
                                g_sprite_k->m_bActive = true;

                                // Place the mine into a waiting off-screen until the script engine using it again
                                g_sprite_i->m_fPosition_x = -100;
                                g_sprite_i->m_fPosition_y = -100;
                                g_sprite_i->m_fVelocity_x = 0;
                                g_sprite_i->m_fVelocity_y = 0;
                                g_sprite_i->m_bScripting = false;
                                break;
                            }
                        }
                    }

                    // Has a shot hit a mine?
                    if( !lstrcmp(g_sprite_i->m_chType, "shot") && !lstrcmp(g_sprite_j->m_chType, "mine") )
                    {
                        g_nScore += 5;

                        // Kill shot
                        g_sprite_i->m_bActive = false;

                        // Find an explosion sprite to assign
                        for( g_sprite_k = g_SpriteList.begin(); g_sprite_k != g_SpriteList.end(); ++g_sprite_k )
                        {
                            if( !lstrcmp(g_sprite_k->m_chType, "explosion") && g_sprite_k->m_bActive == false )
                            {
                                // Assign explosion to the collided mine's position and speed
                                g_sprite_k->m_fPosition_x = (g_sprite_j->m_fPosition_x - 10);
                                g_sprite_k->m_fPosition_y = (g_sprite_j->m_fPosition_y - 20);
                                g_sprite_k->m_fVelocity_x = g_sprite_j->m_fVelocity_x;
                                g_sprite_k->m_bActive = true;

                                // Place the mine into a waiting off-screen
                                g_sprite_j->m_fPosition_x = -100;
                                g_sprite_j->m_fPosition_y = -100;
                                g_sprite_j->m_fVelocity_x = 0;
                                g_sprite_j->m_fVelocity_y = 0;
                                break;
                            }
                        }
                    }

                    // Has the ship hit a mine
                    if( !lstrcmp(g_sprite_i->m_chType, "ship") && !lstrcmp(g_sprite_j->m_chType, "mine") )
                    {
                        if( g_nPowerLevel != 0 )
                            --g_nPowerLevel;

                        if( g_nPowerLevel == 0 )
                        {
                            if( g_nShipsLeft != 0 )
                                --g_nShipsLeft;

                            g_nPowerLevel = 20;
                        }

                        // Find an explosion sprite to assign
                        for( g_sprite_k = g_SpriteList.begin(); g_sprite_k != g_SpriteList.end(); ++g_sprite_k )
                        {
                            if( !lstrcmp(g_sprite_k->m_chType, "explosion") && g_sprite_k->m_bActive == false )
                            {
                                // Assign explosion to the collided mine's position and speed
                                g_sprite_k->m_fPosition_x = (g_sprite_j->m_fPosition_x - 10);
                                g_sprite_k->m_fPosition_y = (g_sprite_j->m_fPosition_y - 20);
                                g_sprite_k->m_fVelocity_x = g_sprite_j->m_fVelocity_x;
                                g_sprite_k->m_bActive = true;

                                // Place the mine into a waiting off-screen
                                g_sprite_j->m_fPosition_x = -100;
                                g_sprite_j->m_fPosition_y = -100;
                                g_sprite_j->m_fVelocity_x = 0;
                                g_sprite_j->m_fVelocity_y = 0;
                                break;
                            }
                        }
                    }
                }
            }
        }

        nCount2 = 0;
    }
}

//-----------------------------------------------------------------------------
// Name: SpriteCollisionTest()
// Desc: Simple function wrapper that passes the proper sprite values 
//       to CollisionTest() for checking.
//-----------------------------------------------------------------------------
bool SpriteCollisionTest( CSprite& sprite1, CSprite& sprite2 )
{
    // This function passes sprite values to the CollisionTest() function
    // in several different ways depending on whether or not the sprites
    // involved are being scaled or have had there size properties adjusted
    // for the purposes of fine-tuning collision detection.

    bool bCollide = false;
    
    if( sprite1.m_bModifyCollision == true && sprite2.m_bModifyCollision == true )
    {
        // Both sprites require modification before sending.
        bCollide = CollisionTest( (int)sprite1.m_fPosition_x  + sprite1.m_nCollisionLeft, 
                                  (int)sprite1.m_fPosition_y  - sprite1.m_nCollisionTop, 
                                  sprite1.m_nFrameWidth  + sprite1.m_nWidthScaling  + sprite1.m_nCollisionRight, 
                                  sprite1.m_nFrameHeight + sprite1.m_nHeightScaling + sprite1.m_nCollisionBottom, 
                                  (int)sprite2.m_fPosition_x  + sprite2.m_nCollisionLeft,
                                  (int)sprite2.m_fPosition_y  - sprite2.m_nCollisionTop,
                                  sprite2.m_nFrameWidth  + sprite2.m_nWidthScaling  + sprite2.m_nCollisionRight, 
                                  sprite2.m_nFrameHeight + sprite2.m_nHeightScaling + sprite2.m_nCollisionBottom);
    }
    else if( sprite1.m_bModifyCollision == true && sprite2.m_bModifyCollision == false )
    {
        // Only the first sprite requires modification before sending.
        bCollide = CollisionTest( (int)sprite1.m_fPosition_x  + sprite1.m_nCollisionLeft,
                                  (int)sprite1.m_fPosition_y  - sprite1.m_nCollisionTop,
                                  sprite1.m_nFrameWidth  + sprite1.m_nWidthScaling  + sprite1.m_nCollisionRight,
                                  sprite1.m_nFrameHeight + sprite1.m_nHeightScaling + sprite1.m_nCollisionBottom,
                                  (int)sprite2.m_fPosition_x,
                                  (int)sprite2.m_fPosition_y,
                                  sprite2.m_nFrameWidth,
                                  sprite2.m_nFrameHeight);
    }
    else if( sprite1.m_bModifyCollision == false && sprite2.m_bModifyCollision == true )
    {
        // Only the second sprite requires modification before sending.
        bCollide = CollisionTest( (int)sprite1.m_fPosition_x,
                                  (int)sprite1.m_fPosition_y,
                                  sprite1.m_nFrameWidth,
                                  sprite1.m_nFrameHeight,
                                  (int)sprite2.m_fPosition_x  + sprite2.m_nCollisionLeft,
                                  (int)sprite2.m_fPosition_y  - sprite2.m_nCollisionTop,
                                  sprite2.m_nFrameWidth  + sprite2.m_nWidthScaling  + sprite2.m_nCollisionRight,
                                  sprite2.m_nFrameHeight + sprite2.m_nHeightScaling + sprite2.m_nCollisionBottom);
    }
    else
    {
        // Neither of the sprites require modification before sending.
        bCollide = CollisionTest( (int)sprite1.m_fPosition_x,
                                  (int)sprite1.m_fPosition_y,
                                  sprite1.m_nFrameWidth,
                                  sprite1.m_nFrameHeight,
                                  (int)sprite2.m_fPosition_x,
                                  (int)sprite2.m_fPosition_y,
                                  sprite2.m_nFrameWidth, 
                                  sprite2.m_nFrameHeight);
    }

    return(bCollide);
}

//-----------------------------------------------------------------------------
// Name: CollisionTest()
// Desc: Checks to see if two rectangular regions overlap or not.
//-----------------------------------------------------------------------------
bool CollisionTest(int x1, int y1, int w1, int h1, int x2, int y2, int w2, int h2) 
{
    // This function tests if the two rectangles overlap

    // Get the radi of each rect
    int width1  = (w1>>1) - (w1>>3);
    int height1 = (h1>>1) - (h1>>3);

    int width2  = (w2>>1) - (w2>>3);
    int height2 = (h2>>1) - (h2>>3);

    // Compute center of each rect
    int cx1 = x1 + width1;
    int cy1 = y1 + height1;

    int cx2 = x2 + width2;
    int cy2 = y2 + height2;

    // Compute deltas
    int dx = abs(cx2 - cx1);
    int dy = abs(cy2 - cy1);

    // Test if rects overlap
    if(dx < (width1+width2) && dy < (height1+height2))
        return true;
    else // Else no collision
        return false;
}

//-----------------------------------------------------------------------------
// Name: ComputeScore()
// Desc: Animates the number sprites according to the current score
//-----------------------------------------------------------------------------
void ComputeScore(void)
{
    // Calculate the frame for each number sprite based on current score

    int number4 = g_nScore%10;           // Ones Place
    int number3 = (g_nScore%100)/10;     // Tens Place
    int number2 = (g_nScore%1000)/100;   // Hundreds Place
    int number1 = (g_nScore%10000)/1000; // Thousands Place

    int ships1  = g_nShipsLeft%10;       // Ones Place

    // Go through the linked list looking for the number sprites and assign values
    for( g_sprite_i = g_SpriteList.begin(); g_sprite_i != g_SpriteList.end(); ++g_sprite_i )
    {
        if( !lstrcmp(g_sprite_i->m_chType, "numbers") )
        {
            if( g_sprite_i->m_nState == 0 )
                g_sprite_i->m_nCurrentFrame = number1;

            if( g_sprite_i->m_nState == 1 )
                g_sprite_i->m_nCurrentFrame = number2;

            if( g_sprite_i->m_nState == 2 )
                g_sprite_i->m_nCurrentFrame = number3;

            if( g_sprite_i->m_nState == 3 )
                g_sprite_i->m_nCurrentFrame = number4;
        }

        if( !lstrcmp(g_sprite_i->m_chType, "shipnumber") )
        {
            g_sprite_i->m_nCurrentFrame = ships1;
        }
    }
}

//-----------------------------------------------------------------------------
// Name: DisplayFrame()
// Desc: Blts the sprites to the back buffer, then it blts or flips the 
//       back buffer onto the primary buffer.
//-----------------------------------------------------------------------------
HRESULT DisplayFrame()
{
    HRESULT hr;

    g_pd3dDevice->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
                         D3DCOLOR_COLORVALUE(0.0f, 0.0f, 0.0f, 1.0f), 1.0f, 0 );

    g_pd3dDevice->BeginScene();

    DrawStars();
    DrawExhaust();
    DrawPowerBar();

    //if( g_bWindowed )
    //{
    //    WriteToSurface( "Press Escape to quit.  Press Alt-Enter to switch to Full-Screen mode.", 
    //        NULL, 5, (SCREEN_HEIGHT - 20), g_pDisplay->GetBackBuffer(), false);
    //}
    //else
    //{
    //    WriteToSurface( "Press Escape to quit.  Press Alt-Enter to switch to Windowed mode.", 
    //        NULL, 5, (SCREEN_HEIGHT - 20), g_pDisplay->GetBackBuffer(), false);
    //}

    g_pd3dDevice->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR );
    g_pd3dDevice->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );

    for( g_sprite_i = g_SpriteList.begin(); g_sprite_i != g_SpriteList.end(); ++g_sprite_i )
        g_sprite_i->drawSprite( g_pd3dDevice );

    g_pd3dDevice->EndScene();

    if( FAILED( hr = g_pd3dDevice->Present( NULL, NULL, NULL, NULL ) ) )
        return hr;

    return S_OK;
}

//-----------------------------------------------------------------------------
// Name: DrawStars()
// Desc: Draws the stars as pixel points on the back buffer.
//-----------------------------------------------------------------------------
HRESULT DrawStars( void )
{
    LPDIRECT3DSURFACE9 pBackBuffer = NULL;
    D3DSURFACE_DESC ddsd;
    int linearPitch = 0;
    int nColorDepth = 0;
    int x = 0;
    int y = 0;

    // Move and draw the stars 
    MoveStars();

    g_pd3dDevice->GetBackBuffer( 0, 0, D3DBACKBUFFER_TYPE_MONO, &pBackBuffer );
    pBackBuffer->GetDesc( &ddsd );

    D3DLOCKED_RECT lockedRectBackBuffer;

    if( FAILED( pBackBuffer->LockRect( &lockedRectBackBuffer, NULL, 0 ) ) )
    {
        pBackBuffer->Release();
        return S_FALSE;
    }

	// Get the color-depth of the back buffer. These are the only formats used 
	// by a Direct3D back-buffer. Find which one is being used and resolve the 
	// color-depth from it.
	switch( ddsd.Format )
	{
		case D3DFMT_A2R10G10B10:
			// 32-bit pixel format using 10 bits each for red, green, and blue, 
			// and 2 bits for alpha. Used only inn full-screen mode.
			nColorDepth = 32;
			break;

		case D3DFMT_A8R8G8B8:
			// 32-bit ARGB pixel format with alpha, using 8 bits per channel.
			nColorDepth = 32;
			break;

		case D3DFMT_X8R8G8B8:
			// 32-bit RGB pixel format, where 8 bits are reserved for each color.
			nColorDepth = 32;
			break;

		case D3DFMT_A1R5G5B5:
			// 16-bit pixel format where 5 bits are reserved for each color 
			// and 1 bit is reserved for alpha.
			nColorDepth = 16;
			break;

		case D3DFMT_X1R5G5B5:
			// 16-bit pixel format where 5 bits are reserved for each color.
			nColorDepth = 16;
			break;

		case D3DFMT_R5G6B5:
			// 16-bit RGB pixel format with 5 bits for red, 6 bits for green, 
			// and 5 bits for blue.
			nColorDepth = 16;
			break;

		default:
			break;
	}

    linearPitch = (int)lockedRectBackBuffer.Pitch;

	// It seems that 8 bit color is no longer in use for Direct3D back-buffers.
    //if( nColorDepth == 8 )
    //{
    //    // Use a BYTE because 1 byte per pixel in 8 bit color
    //    BYTE *surfaceBuffer = static_cast<BYTE*>(lockedRectBackBuffer.pBits);
	//
    //    for( int i = 0; i < TOTAL_STARS; i++ )
    //    {
    //        x = g_stars[i].x;
    //        y = g_stars[i].y;
	//
    //        // Plot the pixel
    //        surfaceBuffer[x+y*linearPitch] = g_stars[i].color;
    //    }
    //}

	if( nColorDepth == 16 )
	{
		// Use a USHORT because 2 bytes per pixel in 16 bit color
		USHORT *surfaceBuffer = static_cast<USHORT*>(lockedRectBackBuffer.pBits);
		// And half the linear pitch because each pixel is now worth 2 bytes
		linearPitch = (linearPitch>>1);   

		for( int i = 0; i < TOTAL_STARS; i++ )
		{
			int x = g_stars[i].x;
			int y = g_stars[i].y;

			// Plot the pixel
			surfaceBuffer[x+y*linearPitch] = (USHORT)g_stars[i].color;
		} 
	}
	else if( nColorDepth == 32 )
	{
		// Use a UINT because 4 bytes per pixel in 32 bit color
		UINT *surfaceBuffer = static_cast<UINT*>(lockedRectBackBuffer.pBits);
		// And half the linear pitch twice because each pixel is now worth 4 bytes
		linearPitch = (linearPitch>>2);

		for( int i = 0; i < TOTAL_STARS; i++ )
		{
			x = g_stars[i].x;
			y = g_stars[i].y;

			// Plot the pixel
			surfaceBuffer[x+y*linearPitch] = g_stars[i].color;
		}
	}

    if( FAILED( pBackBuffer->UnlockRect() ) )
        return S_FALSE;

    pBackBuffer->Release();

    return S_OK;
}

//-----------------------------------------------------------------------------
// Name: DrawExhaust()
// Desc: Draw all visible exhaust particles.
//-----------------------------------------------------------------------------
HRESULT DrawExhaust( void )
{
    LPDIRECT3DSURFACE9 pBackBuffer = NULL;
    D3DSURFACE_DESC ddsd;
    int linearPitch = 0;
    int nColorDepth = 0;
    int x = 0;
    int y = 0;

    // Move and draw the particles 
    MoveExhaust();

    g_pd3dDevice->GetBackBuffer( 0, 0, D3DBACKBUFFER_TYPE_MONO, &pBackBuffer );
    pBackBuffer->GetDesc( &ddsd );

    D3DLOCKED_RECT lockedRectBackBuffer;

    if( FAILED( pBackBuffer->LockRect( &lockedRectBackBuffer, NULL, 0 ) ) )
    {
        pBackBuffer->Release();
        return S_FALSE;
    }

	// Get the color-depth of the back buffer. These are the only formats used 
	// by a Direct3D back-buffer. Find which one is being used and resolve the 
	// color-depth from it.
	switch( ddsd.Format )
	{
		case D3DFMT_A2R10G10B10:
			// 32-bit pixel format using 10 bits each for red, green, and blue, 
			// and 2 bits for alpha. Used only inn full-screen mode.
			nColorDepth = 32;
			break;

		case D3DFMT_A8R8G8B8:
			// 32-bit ARGB pixel format with alpha, using 8 bits per channel.
			nColorDepth = 32;
			break;

		case D3DFMT_X8R8G8B8:
			// 32-bit RGB pixel format, where 8 bits are reserved for each color.
			nColorDepth = 32;
			break;

		case D3DFMT_A1R5G5B5:
			// 16-bit pixel format where 5 bits are reserved for each color 
			// and 1 bit is reserved for alpha.
			nColorDepth = 16;
			break;

		case D3DFMT_X1R5G5B5:
			// 16-bit pixel format where 5 bits are reserved for each color.
			nColorDepth = 16;
			break;

		case D3DFMT_R5G6B5:
			// 16-bit RGB pixel format with 5 bits for red, 6 bits for green, 
			// and 5 bits for blue.
			nColorDepth = 16;
			break;

		default:
			break;
	}

    linearPitch = (int)lockedRectBackBuffer.Pitch;

	//if( nColorDepth == 8 )
	//{
	//	// Use a BYTE because 1 byte per pixel in 8 bit color
	//	BYTE *surfaceBuffer = static_cast<BYTE*>(lockedRectBackBuffer.pBits);
	//
	//	for( int i = 0; i < TOTAL_PARTICLES; i++ )
	//	{
	//		if( g_exhaust[i].bVisible == true )
	//		{
	//			x = g_exhaust[i].x;
	//			y = g_exhaust[i].y;
	//
	//			// Plot the center pixel
	//			surfaceBuffer[x+(y*linearPitch)]     = g_exhaust[i].color;
	//			// Plot the left pixel
	//			surfaceBuffer[(x+1)+(y*linearPitch)] = g_exhaust[i].color;
	//			// Plot the right pixel
	//			surfaceBuffer[(x-1)+(y*linearPitch)] = g_exhaust[i].color;
	//			// Plot the top pixel
	//			surfaceBuffer[x+((y-1)*linearPitch)] = g_exhaust[i].color;
	//			// Plot the bottom pixel
	//			surfaceBuffer[x+((y+1)*linearPitch)] = g_exhaust[i].color;
	//		}
	//	}
	//}

	if( nColorDepth == 16 )
	{
		// Use a USHORT because 2 bytes per pixel in 16 bit color
		USHORT *surfaceBuffer = static_cast<USHORT*>(lockedRectBackBuffer.pBits);
		// And half the linear pitch because each pixel is now worth 2 bytes
		linearPitch = (linearPitch>>1);     

		for( int i = 0; i < TOTAL_PARTICLES; i++ )
		{
			if( g_exhaust[i].bVisible == true )
			{
				x = g_exhaust[i].x;
				y = g_exhaust[i].y;

				// Plot the center pixel
				surfaceBuffer[x+(y*linearPitch)]     = (USHORT)g_exhaust[i].color;
				// Plot the left pixel
				surfaceBuffer[(x+1)+(y*linearPitch)] = (USHORT)g_exhaust[i].color;
				// Plot the right pixel
				surfaceBuffer[(x-1)+(y*linearPitch)] = (USHORT)g_exhaust[i].color;
				// Plot the top pixel
				surfaceBuffer[x+((y-1)*linearPitch)] = (USHORT)g_exhaust[i].color;
				// Plot the bottom pixel
				surfaceBuffer[x+((y+1)*linearPitch)] = (USHORT)g_exhaust[i].color;
			}
		}
	}
	else if( nColorDepth == 32 )
	{
		// Use a UINT because 4 bytes per pixel in 32 bit color
		UINT *surfaceBuffer = static_cast<UINT*>(lockedRectBackBuffer.pBits);
		// And half the linear pitch twice because each pixel is now worth 4 bytes
		linearPitch = (linearPitch>>2);

		for( int i = 0; i < TOTAL_PARTICLES; i++ )
		{
			if( g_exhaust[i].bVisible == true )
			{
				x = g_exhaust[i].x;
				y = g_exhaust[i].y;

				// Plot the center pixel
				surfaceBuffer[x+(y*linearPitch)]     = g_exhaust[i].color;
				// Plot the left pixel
				surfaceBuffer[(x+1)+(y*linearPitch)] = g_exhaust[i].color;
				// Plot the right pixel
				surfaceBuffer[(x-1)+(y*linearPitch)] = g_exhaust[i].color;
				// Plot the top pixel
				surfaceBuffer[x+((y-1)*linearPitch)] = g_exhaust[i].color;
				// Plot the bottom pixel
				surfaceBuffer[x+((y+1)*linearPitch)] = g_exhaust[i].color;
			}
		}
	}

    if( FAILED( pBackBuffer->UnlockRect() ) )
        return S_FALSE;

    pBackBuffer->Release();

    return S_OK;
}

//-----------------------------------------------------------------------------
// Name: DrawPowerBar()
// Desc: Draws the power bar and the colored hash mark sprites that make up  
//       the power bar indicator.
//-----------------------------------------------------------------------------
HRESULT DrawPowerBar( void )
{
    // Warning: This function assumes that the sprites that make up the power 
    // bar have been placed in the linked list in a certain order. If this 
    // order is changed, the power bar will be drawn incorrectly or not at all.

    int nStartingPosit_x = 450;
    int nStartingPosit_y = 10;
    int nMarkSpacing = 5;
    int nMarkCount   = 0;
    int nTotalRed    = 5;
    int nTotalYellow = 5;
    int nTotalGreen  = 10;
    int i = 0;

    for( g_sprite_i = g_SpriteList.begin(); g_sprite_i != g_SpriteList.end(); ++g_sprite_i )
    {
        if( !lstrcmp(g_sprite_i->m_chType, "powerbar") )
        {
            g_sprite_i->m_bActive = true;

            g_sprite_i->m_fPosition_x = (float)nStartingPosit_x - 3;
            g_sprite_i->m_fPosition_y = (float)nStartingPosit_y - 2;
            g_sprite_i->drawSprite( g_pd3dDevice );

            g_sprite_i->m_bActive = false;
        }

        if( !lstrcmp(g_sprite_i->m_chType, "redmark") )
        {
            // Keep drawing red hash marks until we hit the limit or
            // they're no longer required.
            g_sprite_i->m_bActive = true;

            if( g_nPowerLevel > 0 )
            {
                for( i = 0; i < nTotalRed; ++i )
                {
                    g_sprite_i->m_fPosition_x = (float)nStartingPosit_x;
                    g_sprite_i->m_fPosition_y = (float)nStartingPosit_y;
                    g_sprite_i->drawSprite( g_pd3dDevice );

                    nStartingPosit_x += nMarkSpacing;
                    ++nMarkCount;

                    if( nMarkCount >= g_nPowerLevel )
                        break;
                }
            }

            g_sprite_i->m_bActive = false;
        }

        if( !lstrcmp(g_sprite_i->m_chType, "yellowmark") )
        {
            // Keep drawing yellow hash marks until we hit the limit or
            // they're no longer required.
            g_sprite_i->m_bActive = true;

            if( g_nPowerLevel > nTotalYellow )
            {
                for( i = 0; i < nTotalYellow; ++i )
                {
                    g_sprite_i->m_fPosition_x = (float)nStartingPosit_x;
                    g_sprite_i->m_fPosition_y = (float)nStartingPosit_y;
                    g_sprite_i->drawSprite( g_pd3dDevice );

                    nStartingPosit_x += nMarkSpacing;
                    ++nMarkCount;

                    if( nMarkCount >= g_nPowerLevel )
                        break;
                }
            }

            g_sprite_i->m_bActive = false;
        }

        if( !lstrcmp(g_sprite_i->m_chType, "greenmark") )
        {
            // Keep drawing green hash marks until we hit the limit or
            // they're no longer required.
            g_sprite_i->m_bActive = true;

            if( g_nPowerLevel > nTotalGreen )
            {
                for( i = 0; i < nTotalGreen; ++i )
                {
                    g_sprite_i->m_fPosition_x = (float)nStartingPosit_x;
                    g_sprite_i->m_fPosition_y = (float)nStartingPosit_y;
                    g_sprite_i->drawSprite( g_pd3dDevice );

                    nStartingPosit_x += nMarkSpacing;
                    ++nMarkCount;

                    if( nMarkCount >= g_nPowerLevel )
                        break;
                }
            }

            g_sprite_i->m_bActive = false;
            break; // There's no reason to keep searching the linked list - Bail out!
        }
    }

    return S_OK;
}

//-----------------------------------------------------------------------------
// Name: RandomInt()
// Desc: Produces a random int value between some given range
//-----------------------------------------------------------------------------
int RandomInt( int nLow, int nHigh )
{
    int nRange = nHigh - nLow;
    int nNum = rand() % nRange;
    return( nNum + nLow );
}
