#include <windows.h>
#include <Windowsx.h>
#include <gl/gl.h>
#include <gl/glu.h>

#include "game.h"


// Application entry point is here.
// Platform-dependent and OpenGL-dependent things are also here. 


// default window width & height
static const DWORD DefaultWindowWidth = 800;
static const DWORD DefaultWindowHeight = 600;

// globals
HWND	hWnd = NULL;
HDC		hDC = NULL;
HGLRC	hRC = NULL;
bool	Paused = false;

// Set up pixel format for graphics initialization
void SetupPixelFormat()
{
	PIXELFORMATDESCRIPTOR pfd, *ppfd;
	int pixelformat;

	ppfd = &pfd;
  memset(&pfd, 0, sizeof(pfd));

	ppfd->nSize = sizeof(pfd);
	ppfd->nVersion = 1;
	ppfd->dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	ppfd->dwLayerMask = PFD_MAIN_PLANE;
	ppfd->iPixelType = PFD_TYPE_RGBA;
	ppfd->cColorBits = 24;

	pixelformat = ChoosePixelFormat(hDC, ppfd);
	SetPixelFormat(hDC, pixelformat, ppfd);
}

// Initialize OpenGL graphics
void InitOpenGL()
{
	hDC = GetDC(hWnd);

	SetupPixelFormat();

	hRC = wglCreateContext(hDC);
	wglMakeCurrent(hDC, hRC);

	glClearColor(0, 0, 0, 0.5);

	// create bitmaps for the device context font's first 256 glyphs  
	wglUseFontBitmaps(hDC, 0, 256, 1000); 
}

void CleanupOpenGL()
{
	glDeleteLists(1000, 256);

	wglMakeCurrent(NULL, NULL); 
	wglDeleteContext(hRC);
	ReleaseDC(hWnd, hDC);
}

// resize graphics to fit window
void ResizeGraphics()
{
	// get new window size
	RECT rect;
	int width, height;
	GLfloat aspect;

	GetClientRect(hWnd, &rect);
	width = rect.right;
	height = rect.bottom;
	aspect = (GLfloat)width / height;

	// adjust graphics to window size and scaling
	// this makes GL coordinate system almost the same as window coordinate system
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, width, height, 0); // (0,0) in top-left corner

  glMatrixMode(GL_MODELVIEW);

	// let game know we resized the window
	Game::instance().adjust_viewport_size(width, height);
}

void DrawCircle(const GameCircle& circle)
{
	// helper object needed for gluDisk
	GLUquadric * state = gluNewQuadric();
	
	// set color
	glColor3d(circle.color.red, circle.color.green, circle.color.blue);

	// draw
	glPushMatrix();		
  glTranslated(circle.pos.x, circle.pos.y, 0);
	gluDisk(state, 0, circle.radius, 32, 32);
	glPopMatrix();		

	// clean up
	gluDeleteQuadric(state);
}	

void DrawScore()
{
	// use white color for score display
	glColor3d(1.0, 1.0, 1.0);

	// bottom-left corner
	glRasterPos2d(10.0, Game::instance().get_viewport_height() - 10); 
 
	// convert score to string 
	char score_str[100];
	int length = _snprintf_s(score_str, 100, "Score: %i", Game::instance().get_overall_score());
 
	// set up for a string-drawing display list call  
	glListBase(1000); 

	// draw a string using font display lists  
	glCallLists(length, GL_UNSIGNED_BYTE, score_str); 
}

// Draw frame
void DrawGraphics()
{
	// clean up
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// draw circles
	const game_circle_list& circles = Game::instance().get_circles();
	for(game_circle_list::const_iterator it = circles.begin();it != circles.end(); ++it)	
	{
		DrawCircle((*it));
	}

	// draw overall score
	DrawScore();

  // Show the new scene
  SwapBuffers(hDC);
}

// Handle window events and messages
LONG WINAPI MainWndProc (HWND hWnd, UINT uMsg, WPARAM  wParam, LPARAM  lParam)
{
	switch (uMsg)
	{
	case WM_KILLFOCUS:
			Paused = true;
			break;

	case WM_SETFOCUS:
			Paused = false;
			break;

	case WM_LBUTTONDOWN:
			Game::instance().handle_mouse_click(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
			break;

	case WM_SIZE:
			ResizeGraphics();
			break;

	case WM_CLOSE: 
			DestroyWindow(hWnd);
			break;
 
	case WM_DESTROY:
			PostQuitMessage(0);
			break;
 
	// Default event handler
	default: 
			return DefWindowProc (hWnd, uMsg, wParam, lParam); 
			break; 
	} 
 
	return 1; 
}

int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	const LPCWSTR appname = TEXT("Circles");

	WNDCLASS wndclass;
	MSG      msg;
 
	// Define the window class
	wndclass.style         = 0;
	wndclass.lpfnWndProc   = (WNDPROC)MainWndProc;
	wndclass.cbClsExtra    = 0;
	wndclass.cbWndExtra    = 0;
	wndclass.hInstance     = hInstance;
	wndclass.hIcon         = LoadIcon(hInstance, MAKEINTRESOURCE(101));
	wndclass.hCursor       = LoadCursor(NULL,IDC_ARROW);
	wndclass.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
	wndclass.lpszMenuName  = appname;
	wndclass.lpszClassName = appname;
 
	// Register the window class
	if (!RegisterClass(&wndclass)) return FALSE;
 
	// Create the window
	hWnd = CreateWindow(
					appname,
					appname,
					WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
					CW_USEDEFAULT,
					CW_USEDEFAULT,
					DefaultWindowWidth,
					DefaultWindowHeight,
					NULL,
					NULL,
					hInstance,
					NULL);
 
	if (!hWnd) return FALSE;

	// Initialize OpenGL
	InitOpenGL();

	// Display the window
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	// init the game
	Game::instance().init();

	DWORD m_start = GetTickCount();

	// main loop
	while (true) 
	{
		// handle events
		if(PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE) == TRUE) 
		{
			if (!GetMessage(&msg, NULL, 0, 0)) break;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		// update time counter
		DWORD m_end = GetTickCount();
		if(!Paused) 
		{
			// update game 
			double time_elapsed = static_cast<double>(m_end - m_start) / 1000.0; // convert to seconds
			Game::instance().update(time_elapsed); 
		}
		m_start = m_end;

		// draw everything
		DrawGraphics();

		// sleep a bit
		Sleep(10);
	}

	// clean up 
	Game::instance().cleanup();
	CleanupOpenGL();

	return TRUE;
}