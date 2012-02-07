//
//  AppDelegate.h
//  boids-ios
//
//  Created by Justin Beck on 2/7/12.
//  Copyright Beckproduct 2012. All rights reserved.
//

#import <UIKit/UIKit.h>

@class RootViewController;

@interface AppDelegate : NSObject <UIApplicationDelegate> {
	UIWindow			*window;
	RootViewController	*viewController;
}

@property (nonatomic, retain) UIWindow *window;

@end
