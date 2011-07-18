//
//  aitAppDelegate.h
//  ait
//
//  Created by Mikalai Silivonik on 11-04-20.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>

@class aitViewController;

@interface aitAppDelegate : NSObject <UIApplicationDelegate> {

}

@property (nonatomic, retain) IBOutlet UIWindow *window;

@property (nonatomic, retain) IBOutlet aitViewController *viewController;

@end
