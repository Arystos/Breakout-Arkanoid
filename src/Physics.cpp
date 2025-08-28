//
// Created by Aristide Sessa on 21/08/25.
//

#include "Physics.hpp"
#include <algorithm>

bool Physics::circleVsAABB(const glm::vec2& center, float radius,
                           const SDL_FRect& box, glm::vec2& hitNormal)
{
    const glm::vec2 closest{
            std::clamp(center.x, box.x, box.x + box.w),
            std::clamp(center.y, box.y, box.y + box.h)
    };
    const glm::vec2 d = center - closest;
    const float dist2 = glm::dot(d, d);
    const float r2 = radius * radius;
    constexpr float EPS = 1e-6f; // small epsilon, avoid floating point issues
    
    if (dist2 > r2 + EPS) return false; // no collision
    if (dist2 > EPS) { hitNormal = d * glm::inversesqrt(dist2); return true; } // collision

    // circle center is inside the box
    const float faces[4] = { // fastest way to find the closest face
            center.x - box.x,
            (box.x + box.w) - center.x,
            center.y - box.y,
            (box.y + box.h) - center.y
    };
    // get index of minimum distance
    const int idx = int(std::min_element(std::begin(faces), std::end(faces)) - std::begin(faces));
    // set normal based on closest face
    switch (idx){ 
        case 0: hitNormal = {-1,0}; break; 
        case 1: hitNormal = {1,0}; break; 
        case 2: hitNormal = {0,-1}; break; 
        default: hitNormal = {0,1}; 
    }
    
    return true; // collision
}

bool Physics::aabbVsAABB(const SDL_FRect &a, const SDL_FRect &b) {
    return (a.x < b.x + b.w && a.x + a.w > b.x &&
            a.y < b.y + b.h && a.y + a.h > b.y);
}

bool Physics::Collision(Entity *e1, Entity *e2) {
    
    // use the aabb vs aabb collision detection
    if (aabbVsAABB(e1->getRect(), e2->getRect())) {
        return true;
    }
    
    return false;
}
