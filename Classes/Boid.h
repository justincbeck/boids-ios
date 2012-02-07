/**
 * Boid.m
 * This is an Objective-C/Cocos2D port of an AS3 Boid behavior class 
 * (original class shown here http://blog.soulwire.co.uk/laboratory/flash/as3-flocking-steering-behaviors)
 * 
 * You can modify this script in any way you choose
 * and use it for any purpose providing this header 
 * remains intact and the original author is credited
 *  
 * Created by Mario Gonzalez on 2/13/10.
 * Copyright 2010 http://onedayitwillmake.com. All rights reserved.
 * 
 * VERSION 0.2 - FIXED BIG N00B ERROR IN DISTANCESQUARED
 * VERSION 0.1 - HELLOWORLD
 **/ 
/**		
 * 
 *	Boid v1.00
 *	15/10/2008 11:31
 * 
 *	© JUSTIN WINDLE | soulwire ltd
 *	http://blog.soulwire.co.uk
 *	
 *	Released under the Creative Commons 3.0 license
 *	@see http://creativecommons.org/licenses/by/3.0/
 *	
 *	You can modify this script in any way you choose 
 *	and use it for any purpose providing this header 
 *	remains intact and the original author is credited
 * 
 **/

#import <Foundation/Foundation.h>

#import "cocos2d.h"
#import "CGPointExtension.h"

#define EDGE_NONE -1
#define EDGE_WRAP 0
#define EDGE_BOUNCE 1

@interface Boid : CCSprite
{
@protected
	float	_maxForce;
	float	_maxSpeed;
	int		_edgeBehavior;
@public	
	float	_maxForceSQ;
	float	_maxSpeedSQ;
	bool	_doRotation;
	CGPoint	_internalPosition;
	CGPoint _oldInternalPosition;
	 
	CGPoint _velocity;
	CGPoint _acceleration;
	CGPoint _steeringForce;
	
	float	_radius;
	
	float	_wanderTheta; 
	
	// This stuff is really well explained here http://www.shiffman.net/itp/classes/nature/week06_s09/wander/
	float	_wanderMaxTurnCircleRadius; // The radius of the circle, the boid randomly moves to AFTER it projects its future distance
	float	_wanderTurningRadius;  // The maximum angle, in radians, that the Boid's wander behavior can turn at each step.
	float	_wanderLookAheadDistance; // How far ahead to look when deciding where to go, further ahead 
	
	Boid*	_next;
}
// When these are set via accessor (self.property), we will set the squared version of these values at the same time
@property(nonatomic, assign) float	maxForce;
@property(nonatomic, assign) float	maxSpeed;
@property(nonatomic, assign) bool	doRotation;
// Check type
@property(nonatomic, assign) int	edgeBehavior;
/*
 * Randomizes all properties of a boid
 */
-(void) setSpeedMax:(float)speed andSteeringForceMax:(float)force;
-(void) setSpeedMax:(float)speed withRandomRangeOf:(float)speedRange andSteeringForceMax:(float)force withRandomRangeOf:(float)forceRange;

/*
 * Sets the random walk properties
 */
-(void) setWanderingRadius:(float)radius lookAheadDistance:(float)distance andMaxTurningAngle:(float)turningAngle;
/**
 * Resets the Boid's position, velocity, acceleration and 
 * current steering force to zero
 */
-(void) resetVectorsToZero;
-(void) setPos:(CGPoint)value;

/**
 * After calling one or more of the Boid's steering methods, 
 * call the update method in order to set the Boid's position 
 * in relation to the force being applied to it as a result of 
 * it's steering behaviors. If the Boid's edgeBehavior property 
 * is anything other than EDGE_NONE (no edge behavior) then the 
 * Boid's position will be modified accordingly after the 
 * steering forces have been applied
 */
-(void) update;
-(void) handleBorder;
/**
 * Applies a braking force to the boid by scaling it's 
 * velocity.
 * @param	brakingForce
 * A number between 0 and 1. 0 = no effect
 */
-(void) brake:(float)brakingForce;
/**
 * Seeks the Boid towards the specified target
 * 
 * @param	target
 * 
 * The target for the Boid to seek
 * 
 * @param	multiplier
 * 
 * By multiplying the force generated by this behavior, 
 * more or less weight can be given to this behavior in
 * comparison to other behaviors being calculated by the 
 * Boid. To increase the weighting of this behavior, use 
 * a number above 1.0, or to decrease it use a number 
 * below 1.0
 */
-(void) seek:(CGPoint)target usingMultiplier:(float)multiplier;
// Same as above but only if within a specific range to the target
-(void) seek:(CGPoint)target withinRange:(float)range usingMultiplier:(float)multiplier;
/**
 * Seeks the Boid towards the specified target and 
 * applies a deceleration force as the Boid arrives
 * 
 * @param	target
 * 
 * The target for the Boid to seek
 * 
 * @param	easeDistance
 * 
 * The distance from the target at which the Boid should 
 * begin to decelerate
 * 
 * @param	multiplier
 * 
 * By multiplying the force generated by this behavior, 
 * more or less weight can be given to this behavior in
 * comparison to other behaviors being calculated by the 
 * Boid. To increase the weighting of this behavior, use 
 * a number above 1.0, or to decrease it use a number 
 * below 1.0
 */
-(void) arrive:(CGPoint)target withEaseDistance:(float)easeDistance usingMultiplier:(float)multiplier;
/**
 * If a target is within a certain range of the Boid, as 
 * specified by the panicDistance parameter, the Boid will 
 * steer to avoid contact with the target
 * 
 * @param	target
 * 
 * The target for the Boid to avoid
 * 
 * @param	panicDistance
 * 
 * If the distance between the Boid and the target position 
 * is greater than this value, the Boid will ignore the 
 * target and it's steering force will be unchanged
 * 
 * @param	multiplier
 * 
 * By multiplying the force generated by this behavior, 
 * more or less weight can be given to this behavior in
 * comparison to other behaviors being calculated by the 
 * Boid. To increase the weighting of this behavior, use 
 * a number above 1.0, or to decrease it use a number 
 * below 1.0
 */
-(void) flee:(CGPoint)target panicAtDistance:(float)panicDistance usingMultiplier:(float)multiplier;

/**
 * Generates a random wandering force for the Boid. 
 * The results of this method can be controlled by the 
 * _wanderDistance, _wanderStep and _wanderRadius parameters
 * 
 * @param	multiplier
 * 
 * By multiplying the force generated by this behavior, 
 * more or less weight can be given to this behavior in
 * comparison to other behaviors being calculated by the 
 * Boid. To increase the weighting of this behavior, use 
 * a number above 1.0, or to decrease it use a number 
 * below 1.0
 */
-(void) wander:(float)multiplier;
-(CGPoint) steer:(CGPoint)target easeAsApproaching:(BOOL)ease withEaseDistance:(float)easeDistance;
/**
 * Use this method to simulate flocking movement in a 
 * group of Boids. Flock will combine the separate, 
 * align and cohesion steering behaviors to produce 
 * the flocking effect. Adjusting the weighting of each 
 * behavior, as well as the distance values for each 
 * can produce distinctly different flocking behaviors
 * 
 * @param	boids
 * 
 * An Array of Boids to consider when calculating the 
 * flocking behavior
 * 
 * @param	separationWeight
 * 
 * The weighting given to the separation behavior
 * 
 * @param	alignmentWeight
 * 
 * The weighting given to the alignment bahavior
 * 
 * @param	cohesionWeight
 * 
 * The weighting given to the cohesion bahavior
 * 
 * @param	separationDistance
 * 
 * The distance which each Boid will attempt to maintain
 * between itself and any other Boid in the flock
 * 
 * @param	alignmentDistance
 * 
 * If another Boid is within this distance, this Boid will 
 * consider the other Boid's heading when adjusting it's own
 * 
 * @param	cohesionDistance
 * 
 * If another Boid is within this distance, this Boid will 
 * consider the other Boid's position when adjusting it's own
 * 
 * @param	multiplier
 * 
 * By multiplying the force generated by this behavior, 
 * more or less weight can be given to this behavior in
 * comparison to other behaviors being calculated by the 
 * Boid. To increase the weighting of this behavior, use 
 * a number above 1.0, or to decrease it use a number 
 * below 1.0
 */
-(void) flock:(Boid*)head withSeparationWeight:(float)separationWeight andAlignmentWeight:(float)alignmentWeight andCohesionWeight:(float)cohesionWeight andSeparationDistance:(float)separationDistance andAlignmentDistance:(float)alignmentDistance andCohesionDistance:(float)cohesionDistance;
-(void) separate:(Boid*)head withSeparationDistance:(float)separationDistance usingMultiplier:(float)multiplier;
-(void) align:(Boid*)head withAlignmentDistance:(float)neighborDistance usingMultiplier:(float)multiplier;
-(void) cohesion:(Boid*)head withNeighborDistance:(float)neighborDistance usingMultiplier:(float)multiplier;

// Helper functions written in C
float randRange(float min,float max);
CGPoint normalize(CGPoint point);
float getDistance( CGPoint pointA, CGPoint pointB );
float getDistanceSquared( CGPoint pointA, CGPoint pointB );
@end

void HSVtoRGB( float *r, float *g, float *b, float h, float s, float v );