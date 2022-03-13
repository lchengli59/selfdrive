/*-
# The full tutorial is available here: 
# http://rmgi.blog./pygame-2d-car-tutorial.html
# The code is available here: 
# https://github.com/rasmaxim/pygame-car-tutorial
#
# This is inspired by the above.
# Porting to C++
#
*/
#ifndef MYCAR_H
#define MYCAR_H

/*-------------------------------------------------------------------------*/
#include <iostream>
#include <cstdio>
#include <cstdlib>
#define _USE_MATH_DEFINES
#include <cmath>

#include <bits/stdc++.h>     
using namespace std;     

/*-------------------------------------------------------------------------*/
class myCar
/*-------------------------------------------------------------------------*/
{
/*-------------------------------------------------------------------------*/
public:
    myCar(float x, float y, 
        float ag=0.0, float len=4, float mxsteer=30, float mxaccel=5.0)
    {
        position[0] = x;
        position[1] = y;
        velocity[0] = 0.0;
        velocity[1] = 0.0;
        angle = ag;
        length = len;
        max_acceleration = mxaccel;
        max_steering = mxsteer;
        max_velocity = 20;
        brake_deceleration = 10;
        free_deceleration = 2;

        acceleration = 0.0;
        steering = 0.0;

        border[0] = 0.0;
        border[1] = 0.0;

    }

    float min(float a, float b)
    {
        if (a < b) return a;
        else return b;
    }

    float max(float a, float b)
    {
        if (a > b) return a;
        else return b;
    }

    float degrees(float a)
    {
        return 180.0 * a/M_PI;
    }

    float radians(float a)
    {
        return M_PI * a/180.0;
    }

    void forward(float dt)
    {
        if (velocity[0] < 0)
            acceleration = brake_deceleration;
        else
            acceleration += 1 * dt;
        acceleration = max(-max_acceleration, min(acceleration, max_acceleration));
    }

    void backward(float dt)
    {
        if (velocity[0] > 0)
            acceleration = -brake_deceleration;
        else
            acceleration -= 1 * dt;
        acceleration = max(-max_acceleration, min(acceleration, max_acceleration));
    }

    void brake(float dt)
    {
        if (fabs(velocity[0]) > dt * brake_deceleration)
            acceleration = -copysign(brake_deceleration, velocity[0]);
        else
            acceleration = -velocity[0] / dt;
        acceleration = max(-max_acceleration, min(acceleration, max_acceleration));        
    }

    void handsoff(float dt)
    {
        if (fabs(velocity[0]) > dt * free_deceleration)
            acceleration = -copysign(free_deceleration, velocity[0]);
        else
            if (dt != 0)
                acceleration = -velocity[0] / dt;
        acceleration = max(-max_acceleration, min(acceleration, max_acceleration));

    }

    void bounceback(void)
    {
        acceleration = -brake_deceleration;
    }

    void bouncefor(void)
    {
        acceleration = brake_deceleration;
    }

    void steerwheel(char type[], float dt)
    {
        if (strcmp(type,"RIGHT")==0)
            steering -= 30 * dt;
        else if (strcmp(type,"LEFT")==0)
            steering += 30 * dt;
        else
            steering = 0;
        steering = max(-max_steering, min(steering, max_steering));        
    }

    void reset(void)
    {
        position[0] = 20.0;
        position[1] = 20.0;
        velocity[0] = 0.0;
        velocity[1] = 0.0;
    }

    void aimangle(float target_x, float target_y, float dt)
    {
        float x_diff = target_x - position[0];
        float y_diff = target_y - position[1];
        
        float desired_radian_angle;

        if (x_diff == 0)
            desired_radian_angle = 0;
        else
            desired_radian_angle = atan(y_diff / x_diff);

        if (target_x < position[0] && (angle < 90.0 || angle > -90.0))
            desired_radian_angle += M_PI;

        if (angle >= 180) angle -= 360;
        if (angle <= -180) angle += 360;
        float difference_in_angle = angle - degrees(desired_radian_angle);
        while (difference_in_angle >= 180)
            difference_in_angle -= 360;
        while (difference_in_angle <= -180)
            difference_in_angle += 360;

        float dangle = difference_in_angle;
        float ag = steering;
        if (dangle>1.0)
            ag -= 30 * dt;
        else if (dangle<-1.0)
            ag += 30 * dt;
        else
            ag = 0;
        steering = max(-max_steering, min(ag, max_steering));

        forward(dt);

    }      

    // function to find if given point
    // lies inside a given rectangle or not.
    // x1,y1 bottom left
    // x2,y2 top right
    bool collidePoint(float x1, float y1, 
        float x2, float y2, float x, float y)
    {
        if ((x > x1 && x < x2) && (y < y1 && y > y2))
        {
            return true;
        }
     
        return false;
    }
    
    bool endgame(float target_x, float target_y)
    {
        float botleft[2],topright[2];
        botleft[0] = position[0] - (length/2);
        botleft[1] = position[1] + (length/2);
        topright[0] = position[0] + (length/2);
        topright[1] = position[1] - (length/2);
        if (collidePoint(botleft[0], botleft[1],
                        topright[0], topright[1],
                        target_x, target_y))
            return true;
        return false;
    }

    bool gotto(float target[], float dt)
    {
        if (endgame(target[0], target[1]))
        {
            brake(dt);
            velocity[0] = 0.0;
            velocity[1] = 0.0;
            acceleration = 0.0;
            steering = 0.0;
            return true;           
        }
        aimangle(target[0], target[1], dt);
        return false;
    }
 
    void reduce_speed(float dt)
    {
        velocity[0] -= acceleration * dt;
        velocity[1] -= 0.0;
    }

    void update(float dt)
    {

        velocity[0] += acceleration * dt;
        velocity[1] += 0.0f;
        velocity[0] = max(-max_velocity, min(velocity[0], max_velocity));

        float angular_velocity = 0.0f;
        if (steering != 0.0)
        {
            float turning_radius = length / sin(radians(steering));
            angular_velocity = velocity[0] / turning_radius;            
        }

        float rag = radians(angle);
        float newX = velocity[0] * cos(rag) - velocity[1] * sin(rag);
        float newY = velocity[0] * sin(rag) + velocity[1] * cos(rag);
        position[0] += (newX * dt);
        position[1] += (newY * dt);
        angle += (degrees(angular_velocity) * dt);
        
        counter += 1;
    }

    float position[2];
    float velocity[2];
    float angle;
    float length;
    float max_acceleration;
    float max_steering;
    float max_velocity;
    float brake_deceleration;
    float free_deceleration;

    float acceleration;
    float steering;

    float target[2];    
    float border[2];
    short counter;

};

#endif
