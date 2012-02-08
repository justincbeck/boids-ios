//
//  main.m
//  boids-ios
//
//  Created by Justin Beck on 2/7/12.
//  Copyright Beckproduct 2012. All rights reserved.
//

#import <UIKit/UIKit.h>

int main(int argc, char *argv[]) {
    
    NSAutoreleasePool * pool = [[NSAutoreleasePool alloc] init];
    int retVal = UIApplicationMain(argc, argv, nil, @"BoidsExampleAppDelegate");
    [pool release];
    return retVal;
}
