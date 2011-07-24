//
//  MainViewController.h
//  ait
//
//  Created by Mikalai on 11-07-19.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>

#import <OpenGLES/EAGL.h>

#import <OpenGLES/ES1/gl.h>
#import <OpenGLES/ES1/glext.h>
#import <OpenGLES/ES2/gl.h>
#import <OpenGLES/ES2/glext.h>
#import "EAGLView.h"
#import "ms/msScene.h"


@interface MainViewController : UIViewController {    
@private
    EAGLContext *context;
    GLuint program;
    
    BOOL animating;
    NSInteger animationFrameInterval;
    CADisplayLink *displayLink;
    msScene m_scene;
    CGPoint touchStartLocation;
}

@property (readonly, nonatomic, getter=isAnimating) BOOL animating;
@property (nonatomic) NSInteger animationFrameInterval;

- (void)startAnimation;
- (void)stopAnimation;
- (IBAction)onNew:(id)sender;
- (IBAction)onUndo:(id)sender;

@end