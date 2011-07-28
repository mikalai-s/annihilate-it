//
//  MainViewController.m
//  ait
//
//  Created by Mikalai on 11-07-19.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#import "MainViewController.h"

@interface MainViewController ()
@property (nonatomic, retain) EAGLContext *context;
@property (nonatomic, assign) CADisplayLink *displayLink;
- (BOOL)loadShaders:(GLuint)bufferId;
@end


@implementation MainViewController


@synthesize animating, context, displayLink;

- (void)awakeFromNib
{
    EAGLContext *aContext = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2];
    
    if (!aContext) {
        aContext = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES1];
    }
    
    if (!aContext)
        NSLog(@"Failed to create ES context");
    else if (![EAGLContext setCurrentContext:aContext])
        NSLog(@"Failed to set ES context current");
    
	self.context = aContext;
	[aContext release];
    
    EAGLView *eaglView = [self.view viewWithTag:101];
	
    [eaglView setContext:context];
    [eaglView setFramebuffer];
    
    if ([context API] == kEAGLRenderingAPIOpenGLES2)
        [self loadShaders:1];
    
    animating = FALSE;
    animationFrameInterval = 1;
    self.displayLink = nil;
}
- (void)dealloc
{
    if (program) {
        glDeleteProgram(program);
        program = 0;
    }
    
    // Tear down context.
    if ([EAGLContext currentContext] == context)
        [EAGLContext setCurrentContext:nil];
    
    [context release];
    
    [super dealloc];
}

- (void)didReceiveMemoryWarning
{
    // Releases the view if it doesn't have a superview.
    [super didReceiveMemoryWarning];
    
    // Release any cached data, images, etc. that aren't in use.
}

- (void)viewWillAppear:(BOOL)animated
{
    [self startAnimation];
    
    [super viewWillAppear:animated];
}

- (void)viewWillDisappear:(BOOL)animated
{
    [self stopAnimation];
    
    [super viewWillDisappear:animated];
}

- (void)viewDidUnload
{
	[super viewDidUnload];
	
    if (program) {
        glDeleteProgram(program);
        program = 0;
    }
    
    // Tear down context.
    if ([EAGLContext currentContext] == context)
        [EAGLContext setCurrentContext:nil];
	self.context = nil;	
}

- (NSInteger)animationFrameInterval
{
    return animationFrameInterval;
}

- (void)setAnimationFrameInterval:(NSInteger)frameInterval
{
    /*
	 Frame interval defines how many display frames must pass between each time the display link fires.
	 The display link will only fire 30 times a second when the frame internal is two on a display that refreshes 60 times a second. The default frame interval setting of one will fire 60 times a second when the display refreshes at 60 times a second. A frame interval setting of less than one results in undefined behavior.
	 */
    if (frameInterval >= 1) {
        animationFrameInterval = frameInterval;
        
        if (animating) {
            [self stopAnimation];
            [self startAnimation];
        }
    }
}

- (void)startAnimation
{
    if (!animating) {
        CADisplayLink *aDisplayLink = [[UIScreen mainScreen] displayLinkWithTarget:self selector:@selector(drawFrame)];
        [aDisplayLink setFrameInterval:animationFrameInterval];
        [aDisplayLink addToRunLoop:[NSRunLoop currentRunLoop] forMode:NSDefaultRunLoopMode];
        self.displayLink = aDisplayLink;
        
        animating = TRUE;
    }
}

- (void)stopAnimation
{
    if (animating) {
        [self.displayLink invalidate];
        self.displayLink = nil;
        animating = FALSE;
    }
}

- (void)drawFrame
{
    EAGLView *eaglView = [self.view viewWithTag:101];
    
    [eaglView setFramebuffer];
    
    m_scene.setMainFrameBuffer([eaglView getFramebuffer]);
    
    m_scene.drawFrame();
    
    [eaglView presentFramebuffer];
}

- (BOOL)loadShaders:(GLuint)frameId
{
    string uniforms = "/data/uniforms.txt";
    msMapDataFileName(uniforms);
    
    EAGLView *eaglView = [self.view viewWithTag:101];
    
    CGRect rect = [eaglView frame];
    
    m_scene.newSize(rect.size.width, rect.size.height);  
    m_scene.loadData(uniforms);
    m_scene.init();
    m_scene.start();
    
    
    return TRUE;
}

- (void) touched:(CGPoint)point 
{
    
}

int lastDirection = MS_BOX_SHIFT_DOWN;

int getShiftDirection()
{
    switch([UIDevice currentDevice].orientation)
    {
        case UIDeviceOrientationPortrait:
            lastDirection = MS_BOX_SHIFT_DOWN;
            return MS_BOX_SHIFT_DOWN;
            
        case UIDeviceOrientationPortraitUpsideDown:
            lastDirection = MS_BOX_SHIFT_TOP;
            return MS_BOX_SHIFT_TOP;
            
        case UIDeviceOrientationLandscapeLeft:
            lastDirection = MS_BOX_SHIFT_LEFT;
            return MS_BOX_SHIFT_LEFT;
            
        case UIDeviceOrientationLandscapeRight:
            lastDirection = MS_BOX_SHIFT_RIGHT;
            return MS_BOX_SHIFT_RIGHT;
    }
    return lastDirection;
}


- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event
{
    touchStartLocation = [[touches anyObject] locationInView:self.view];
    
    m_scene.mouseClick(touchStartLocation.x, touchStartLocation.y, getShiftDirection());
}

- (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event
{/*
    CGPoint current = [[touches anyObject] locationInView:self.view];
    float dif = (current.x - touchStartLocation.x) * (current.x - touchStartLocation.x) + (current.y - touchStartLocation.y) * (current.y - touchStartLocation.y);
    
    if(dif > 1000)
        m_scene.start();*/
}

- (IBAction)onNew:(id)sender
{
    m_scene.start();
}

- (IBAction)onUndo:(id)sender
{
    m_scene.undoLastMove();
}



@end
