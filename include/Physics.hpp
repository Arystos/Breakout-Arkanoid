//
// Created by Aristide Sessa on 21/08/25.
//

#ifndef BREAKOUT_PHYSICS_HPP
#define BREAKOUT_PHYSICS_HPP


#include <iostream>
#include <SDL.h>
#include <glm/geometric.hpp>
#include "Entity.hpp"
#include "Entity_Ball.hpp"
#include "glm/vec2.hpp"

class Physics {
public:
    // Check collision between a circle and an AABB
    static bool circleVsAABB(const glm::vec2& center, float radius, const SDL_FRect& box, glm::vec2& hitNormal);

    // Check collision between two AABBs
    static bool aabbVsAABB(const SDL_FRect& a, const SDL_FRect& b);

    // Reflect the ball based on the collision normal
    static void reflectBall(Entity_Ball& ball, const glm::vec2& normal);

    // General collision detection between two entities
    static bool Collision(Entity *e1, Entity *e2);
};


#endif //BREAKOUT_PHYSICS_HPP
