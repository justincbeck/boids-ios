//
//  HelloWorldLayer.m
//  BoidsExample
//
//  Created by Mario.Gonzalez on 9/16/10.
//  Copyright __MyCompanyName__ 2010. All rights reserved.
//

// Import the interfaces
#import "AvoidTouchScene.h"

// HelloWorld implementation
@implementation AvoidTouchScene
@synthesize flockPointer = _flockPointer;
@synthesize  sheet = _sheet;
@synthesize currentTouch = _currentTouch;
+(id) scene
{
	// 'scene' is an autorelease object.
	CCScene *scene = [CCScene node];
	
	// 'layer' is an autorelease object.
	AvoidTouchScene *layer = [AvoidTouchScene node];
	
	// add layer as a child to scene
	[scene addChild: layer];
	
	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
-(id) init
{
	// always call "super" init
	// Apple recommends to re-assign "self" with the "super" return value
	if( (self=[super init] ))
	{
		srandom(time(NULL));
		
		//[self setColor: ccc3(128, 128, 128)];
		CGSize screenSize = [[CCDirector sharedDirector] winSize];
		CGRect boidRect = CGRectMake(0,0, 32, 32);
		
		
		self.sheet = [CCSpriteBatchNode batchNodeWithFile:@"blocks.png" capacity:201];
		self.isTouchEnabled = YES;
		self.currentTouch = CGPointZero;
		[_sheet setBlendFunc:(ccBlendFunc){GL_ONE, GL_ONE}];
		[self addChild:_sheet z:0 tag:0];
		
		
		_flockPointer = [Boid spriteWithBatchNode:_sheet rect: boidRect];
		Boid* previousBoid = _flockPointer;
		Boid *boid = _flockPointer;
		
		// Create many of them
		float count = 180.0f;
		for (int i = 0; i < count; i++) 
		{
			// Create a linked list
			// The first one has no previous.
			if(i != 0)
			{
				boid = [Boid spriteWithBatchNode:_sheet rect: boidRect];
				previousBoid->_next = boid; // special case for the first one
			}
			
			previousBoid = boid;
			
			// Initialize behavior properties for this boid
			boid.doRotation = YES;
			[boid setSpeedMax:4.0f withRandomRangeOf:1.5f andSteeringForceMax:0.75f withRandomRangeOf:0.25f];
			[boid setWanderingRadius: 16.0f lookAheadDistance: 40.0f andMaxTurningAngle:0.2f];
			[boid setEdgeBehavior: CCRANDOM_0_1() < 0.9 ? EDGE_WRAP : EDGE_BOUNCE];
			
			// Cocos properties
			[boid setScale: 0.2 + CCRANDOM_0_1() * 1.25];
			[boid setPos: ccp(CCRANDOM_0_1() * screenSize.width, CCRANDOM_0_1() * screenSize.height )];
			// Color
			//[boid setOpacity: 128];
			float r,g,b;
			HSVtoRGB(&r, &g, &b, (float) i / count * 30.0f + 30.0f, 0.7f, 1.0f);
			[boid setColor: ccc3(r,g,b)];
			
			[_sheet addChild:boid];
		}
		
		
		[self schedule: @selector(tick:)];
		
	}
	return self;
}

- (void)onEnter
{
	[[CCTouchDispatcher sharedDispatcher] addTargetedDelegate:self priority:0 swallowsTouches:YES];
	[super onEnter];
}

- (void)onExit
{
	[[CCTouchDispatcher sharedDispatcher] removeDelegate:self];
	[super onExit];
	
}

-(void) tick: (ccTime) dt
{
	Boid* boid = _flockPointer;
	CGPoint centerOfScreen = ccp([[CCDirector sharedDirector] winSize].width/2, [[CCDirector sharedDirector] winSize].height/2);
	while(boid)
	{
		Boid* b = boid;
		boid = b->_next;
		[b wander: 0.10f];
		
		// Avoid touch
		if ( CGPointEqualToPoint( _currentTouch, CGPointZero ) == NO )
			[b flee:self.currentTouch panicAtDistance:75 usingMultiplier:0.95f];
		
		// swarm around middle of screen
		[b seek:centerOfScreen usingMultiplier:0.45f];
		[b update];
	}
}

- (BOOL)ccTouchBegan:(UITouch *)touch withEvent:(UIEvent *)event
{
	self.currentTouch = [self convertTouchToNodeSpace: touch];
	return YES;
}
// touch updates:
- (void)ccTouchMoved:(UITouch *)touch withEvent:(UIEvent *)event
{
	self.currentTouch = [self convertTouchToNodeSpace: touch];
}
- (void)ccTouchEnded:(UITouch *)touch withEvent:(UIEvent *)event
{
	self.currentTouch = CGPointZero;
}

// on "dealloc" you need to release all your retained objects
- (void) dealloc
{
	// in case you have something to dealloc, do it in this method
	// in this particular example nothing needs to be released.
	// cocos2d will automatically release all the children (Label)
	
	// don't forget to call "super dealloc"
	[super dealloc];
}
@end;