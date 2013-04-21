// Micro-GLUT, bare essentials

// By Ingemar Ragnemalm 2012
// I wrote this since GLUT seems not to have been updated to support
// creation of a 3.2 context on the Mac. On Linux, you use FreeGLUT
// which has this support. You probably can use that on the Mac as well
// but I havn't had time to try.

// Several additions for the lab 3 version: Simple menu, glutTimerFunc and my own glutrepeatingTimerFunc.
// glutKeyboardUpFunc, glutInitDisplayMode.

// 120209: Some more additions, like glutMotionFunc and GLUT_RIGHT_BUTTON.

//#define GL3_PROTOTYPES
#import <Cocoa/Cocoa.h>
#include <OpenGL/gl.h>
#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include <sys/time.h>

#include "MicroGlut.h"

// Vital internal variables

void (*gDisplay)(void);
void (*gReshape)(int width, int height);
void (*gKey)(unsigned char key, int x, int y);
void (*gKeyUp)(unsigned char key, int x, int y);
void (*gMouseMoved)(int x, int y);
void (*gMouseDragged)(int x, int y);
void (*gMouseFunc)(int button, int state, int x, int y);
unsigned int gContextInitMode = GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH;
void (*gIdle)(void);
char updatePending = 1;
char gRunning = 1;

// -----------

// Globals (was in GLViewDataPtr)
NSOpenGLContext	*m_context;
float lastWidth, lastHeight;
NSView *theView;

void MakeContext(NSView *view)
{
	NSOpenGLPixelFormat *fmt;
	int zdepth, sdepth;

	if (gContextInitMode & GLUT_DEPTH)
		zdepth = 32;
	else
		zdepth = 0;

	if (gContextInitMode & GLUT_STENCIL)
		sdepth = 32;
	else
		sdepth = 0;

	NSOpenGLPixelFormatAttribute attrs[] =
	{
		NSOpenGLPFAOpenGLProfile, NSOpenGLProfileVersion3_2Core,
		NSOpenGLPFADoubleBuffer,
		NSOpenGLPFADepthSize, zdepth,
		NSOpenGLPFAStencilSize, sdepth,
		0
	};

	// Save view (should be packaged with context for multi-window application - to do)
	theView = view;

	// Init GL context
	fmt = [[NSOpenGLPixelFormat alloc] initWithAttributes: &attrs[0]];

	m_context = [[NSOpenGLContext alloc] initWithFormat: fmt shareContext: nil];
	[fmt release];
	[m_context makeCurrentContext];
}



// -------------------- View ------------------------

@interface TestView : NSView <NSWindowDelegate> { }
-(void)drawRect:(NSRect)rect;
-(void)keyDown:(NSEvent *)theEvent;
-(void)keyUp:(NSEvent *)theEvent;
-(void)mouseMoved:(NSEvent *)theEvent;
-(void)mouseDragged:(NSEvent *)theEvent;
-(void)mouseDown:(NSEvent *)theEvent;
-(void)mouseUp:(NSEvent *)theEvent;
-(void)rightMouseDown:(NSEvent *)theEvent;
-(void)rightMouseUp:(NSEvent *)theEvent;
@end

#define Pi 3.1415

@implementation TestView

-(void) mouseMoved:(NSEvent *)theEvent
{
	NSPoint p;

//	printf("Mouse moved\n");
	if (gMouseMoved != nil)
	{
		p = [theEvent locationInWindow];
		p = [self convertPoint: p fromView: nil];
		gMouseMoved(p.x, p.y);
	}
}

-(void) mouseDragged:(NSEvent *)theEvent
{
	NSPoint p;

//	printf("Mouse dragged\n");
	if (gMouseDragged != nil)
	{
		p = [theEvent locationInWindow];
		p = [self convertPoint: p fromView: nil];
		gMouseDragged(p.x, p.y);
	}
}

-(void) mouseDown:(NSEvent *)theEvent
{
	NSPoint p;

//	printf("Mouse down\n");
	if (gMouseFunc != nil)
	{
		// Convert location in window to location in view
		p = [theEvent locationInWindow];
		p = [self convertPoint: p fromView: nil];
		gMouseFunc(GLUT_LEFT_BUTTON, GLUT_DOWN, p.x, p.y);
	}
}

-(void) mouseUp:(NSEvent *)theEvent
{
	NSPoint p;

	if (gMouseFunc != nil)
	{
		// Convert location in window to location in view
		p = [theEvent locationInWindow];
		p = [self convertPoint: p fromView: nil];
		gMouseFunc(GLUT_LEFT_BUTTON, GLUT_UP, p.x, p.y);
	}
}

-(void) rightMouseDown:(NSEvent *)theEvent
{
	NSPoint p;

	if (gMouseFunc != nil)
	{
		// Convert location in window to location in view
		p = [theEvent locationInWindow];
		p = [self convertPoint: p fromView: nil];
		gMouseFunc(GLUT_RIGHT_BUTTON, GLUT_DOWN, p.x, p.y);
	}
}

-(void) rightMouseUp:(NSEvent *)theEvent
{
	NSPoint p;

	if (gMouseFunc != nil)
	{
		// Convert location in window to location in view
		p = [theEvent locationInWindow];
		p = [self convertPoint: p fromView: nil];
		gMouseFunc(GLUT_RIGHT_BUTTON, GLUT_UP, p.x, p.y);
	}
}

-(void)keyDown:(NSEvent *)theEvent
{
	char *chars;

//	printf("Key down\n");
	if (gKey != NULL)
	{
		chars = (char *)[[theEvent characters] cStringUsingEncoding: NSASCIIStringEncoding];
		if (chars != NULL)
			gKey(chars[0], 0, 0); // TO DO: x and y
	}
}

-(void)keyUp:(NSEvent *)theEvent
{
	char *chars;

//	printf("Key up\n");
	if (gKeyUp != NULL)
	{
		chars = (char *)[[theEvent characters] cStringUsingEncoding: NSASCIIStringEncoding];
		if (chars != NULL)
			gKeyUp(chars[0], 0, 0); // TO DO: x and y
	}
}

- (BOOL)acceptsFirstResponder	{ return YES; }

- (BOOL)becomeFirstResponder	{ return YES; }

- (BOOL)resignFirstResponder	{ return YES; }

-(void)drawRect:(NSRect)rect
{
	if (([theView frame].size.width != lastWidth) || ([theView frame].size.height != lastHeight))
	{
		lastWidth = [theView frame].size.width;
		lastHeight = [theView frame].size.height;

		// Only needed on resize:
		[m_context clearDrawable];
//		glViewport(0, 0, [theView frame].size.width, [theView frame].size.height);

		if (gReshape != NULL)
			gReshape([theView frame].size.width, [theView frame].size.height);
	}

	[m_context setView: theView];
	[m_context makeCurrentContext];

	// Draw
	if (gDisplay != NULL)
		gDisplay();
	updatePending = 0;

	[m_context flushBuffer];
	[NSOpenGLContext clearCurrentContext];
}

-(void)windowWillClose:(NSNotification *)note
{
	[[NSApplication sharedApplication] terminate:self];
}
@end


// -------------------- Timer ------------------------

// Data for timer
@interface TimerInfoRec : NSObject
{
@public	void (*func)(int arg);
	int arg;
@private
}
@end

@implementation TimerInfoRec
@end

// Mini-mini class for the timer
@interface TimerController : NSObject { }
-(void)timerFireMethod:(NSTimer *)t;
@end

NSTimer	*gTimer;
TimerController	*myTimerController;
NSView	*view;

// Timer!
@implementation TimerController
-(void)timerFireMethod:(NSTimer *)t;
{
	TimerInfoRec *tr;

	if (t.userInfo != nil) // One-shot timer with a TimerInfoRec
	{
		tr = t.userInfo;
		tr->func(tr->arg);
		[tr release];
//		((TimerInfoRec *)(t.userInfo))->func(((TimerInfoRec *)(t.userInfo))->arg);
//		free((TimerInfoRec *)(t.userInfo));
	}
	else
	{
		[view setNeedsDisplay: YES];
		updatePending = 1;
	}
}
@end


void glutPostRedisplay()
{
	[theView setNeedsDisplay: YES];
	updatePending = 1;
}


// home()

#include <Carbon/Carbon.h>
#include <stdio.h>

void home()
{
	CFBundleRef mainBundle = CFBundleGetMainBundle();
	CFURLRef resourcesURL = CFBundleCopyResourcesDirectoryURL(mainBundle);
	char path[PATH_MAX];
	if (!CFURLGetFileSystemRepresentation(resourcesURL, TRUE, (UInt8 *)path, PATH_MAX))
	{
		// error!
		return;
	}
	CFRelease(resourcesURL);

	chdir(path);
	printf("Current Path: %s\n", path);
}

// ------------------ Main program ---------------------

@interface MGApplication : NSApplication
@end

@implementation MGApplication
/* Invoked from the Quit menu item */
- (void)terminate:(id)sender
{
	gRunning = 0;
}
@end

MGApplication *myApp;
NSView *view;
NSAutoreleasePool *pool;
NSWindow *window;
static struct timeval timeStart;

void CreateMenu()
{
	NSMenu *mainMenu, *theMiniMenu;
	NSMenuItem *menuItem2, *dummyItem;

	// Create main menu = menu bar
	mainMenu = NSMenu.alloc;
	[mainMenu initWithTitle: @""];
	[NSApp setMainMenu: mainMenu];

	// Create the custom menu
	theMiniMenu = NSMenu.alloc;
	[theMiniMenu initWithTitle: @"The MiniMenu"];

	// Create a menu item with standard message
	menuItem2 = NSMenuItem.alloc;
	[menuItem2 initWithTitle: @"Hide" action: @selector(hide:) keyEquivalent: @"h"];
	[menuItem2 setKeyEquivalentModifierMask: NSCommandKeyMask];
	[theMiniMenu addItem: menuItem2];

	// Create a menu item with standard message
	menuItem2 = NSMenuItem.alloc;
	[menuItem2 initWithTitle: @"Hide others" action: @selector(hideOtherApplications:) keyEquivalent: @"h"];
	[menuItem2 setKeyEquivalentModifierMask: NSCommandKeyMask | NSAlternateKeyMask];
	[theMiniMenu addItem: menuItem2];

	// Create a menu item with standard message
	menuItem2 = NSMenuItem.alloc;
	[menuItem2 initWithTitle: @"Show all" action: @selector(unhideAllApplications:) keyEquivalent: @"h"];
	[menuItem2 setKeyEquivalentModifierMask: NSCommandKeyMask | NSControlKeyMask];
	[theMiniMenu addItem: menuItem2];

	// Create a menu item with standard message
	menuItem2 = NSMenuItem.alloc;
	[menuItem2 initWithTitle: @"Quit" action: @selector(terminate:) keyEquivalent: @"q"];
	[menuItem2 setKeyEquivalentModifierMask: NSCommandKeyMask];
	[theMiniMenu addItem: menuItem2];

	// Adding a menu is done with a dummy item to connect the menu to its parent
	dummyItem = NSMenuItem.alloc;
	[dummyItem initWithTitle: @"" action: nil keyEquivalent: @""];
	[dummyItem setSubmenu: theMiniMenu];
	[mainMenu addItem: dummyItem];
}

void glutInit(int *argcp, char **argv)
{
	pool = [NSAutoreleasePool new];
//	myApp = [NSApplication sharedApplication];
	myApp = [MGApplication sharedApplication];
	gRunning = 1;
	home();
	gettimeofday(&timeStart, NULL);
	CreateMenu();
	[myApp finishLaunching];
	myTimerController = [TimerController alloc];
}

int gWindowPosX = 10;
int gWindowPosY = 50;
int gWindowWidth = 400;
int gWindowHeight = 400;

void glutInitWindowPosition (int x, int y)
{
	gWindowPosX = x;
	gWindowPosY = y;
}
void glutInitWindowSize (int width, int height)
{
	gWindowWidth = width;
	gWindowHeight = height;
}
void glutCreateWindow (char *windowTitle)
{
// To do: Should get screen height instead of 1000
	NSRect frame = NSMakeRect(gWindowPosX, 1000 - gWindowPosY, gWindowWidth, gWindowHeight);

	window = [NSWindow alloc];
	[window initWithContentRect:frame
					styleMask:NSTitledWindowMask | NSClosableWindowMask | NSMiniaturizableWindowMask
					backing:NSBackingStoreBuffered
					defer:false];

	[window setTitle: [[NSString alloc] initWithCString:windowTitle
				encoding:NSMacOSRomanStringEncoding]];

	view = [TestView alloc];
	[view initWithFrame: frame];
	[window setAcceptsMouseMovedEvents:YES];

	// OpenGL init!
	MakeContext(view);

// Moved from main loop
//	[window setContentView: view];
//	[window setDelegate: (TestView*)view];
//	[window makeKeyAndOrderFront: nil];
}

void glutMainLoop()
{
	[window setContentView: view];
	[window setDelegate: (TestView*)view];
	[window makeKeyAndOrderFront: nil];
	// Main loop
//	[myApp run];

	[myApp finishLaunching];

	NSEvent *event;

	while (gRunning)
	{
		[pool release];
		pool = [NSAutoreleasePool new];

		event = [myApp nextEventMatchingMask: NSAnyEventMask
//							untilDate: [NSDate dateWithTimeIntervalSinceNow: 0.0]
							untilDate: [NSDate distantFuture]
							inMode: NSDefaultRunLoopMode
							dequeue: true
							];

		[myApp sendEvent: event];
		[myApp updateWindows];

		if (gIdle != NULL)
			if (!updatePending)
				gIdle();
	}
}

// This won't work yet
void glutCheckLoop()
{
	[myApp runOnce];

	if (gIdle != NULL)
		if (!updatePending)
			gIdle();

	[pool release];
	pool = [NSAutoreleasePool new];
}

void glutTimerFunc(int millis, void (*func)(int arg), int arg)
{
	TimerInfoRec *timerInfo = [TimerInfoRec alloc];
	timerInfo->arg = arg;
	timerInfo->func = func;

	gTimer = [NSTimer
		scheduledTimerWithTimeInterval: millis/1000.0
		target: myTimerController
		selector: @selector(timerFireMethod:)
		userInfo: timerInfo
		repeats: NO];
}

// Added by Ingemar
void glutRepeatingTimerFunc(int millis)
{
	gTimer = [NSTimer
		scheduledTimerWithTimeInterval: millis/1000.0
		target: myTimerController
		selector: @selector(timerFireMethod:)
		userInfo: nil
		repeats: YES];
}

void glutDisplayFunc(void (*func)(void))
{
	gDisplay = func;
}

void glutReshapeFunc(void (*func)(int width, int height))
{
	gReshape = func;
}

void glutKeyboardFunc(void (*func)(unsigned char key, int x, int y))
{
	gKey = func;
}

void glutKeyboardUpFunc(void (*func)(unsigned char key, int x, int y))
{
	gKeyUp = func;
}

void glutPassiveMotionFunc(void (*func)(int x, int y))
{
	gMouseMoved = func;
}

void glutMotionFunc(void (*func)(int x, int y))
{
	gMouseDragged = func;
}

void glutMouseFunc(void (*func)(int button, int state, int x, int y))
{
	gMouseFunc = func;
}

// You can safely skip this
void glutSwapBuffers()
{
 	[m_context flushBuffer];
}

int glutGet(int type)
{
	struct timeval tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_usec - timeStart.tv_usec) / 1000 + (tv.tv_sec - timeStart.tv_sec)*1000;
}

void glutInitDisplayMode(unsigned int mode)
{
	gContextInitMode = mode;
}

void glutIdleFunc(void (*func)(void))
{
//	printf('WARNING! Idle not yet implemented. Use timers instead.\n');
	gIdle = func;
	glutRepeatingTimerFunc(10);
}
