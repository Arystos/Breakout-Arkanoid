//
// Created by Aristide Sessa on 21/08/25.
//

#include "Physics.hpp"

bool Physics::circleVsAABB(const glm::vec2 &center, float radius, const SDL_FRect &box, glm::vec2 &hitNormal) {
    const glm::vec2 closest{
            std::clamp(center.x, box.x, box.x + box.w),
            std::clamp(center.y, box.y, box.y + box.h)
    };

    // distance from center to closest point on AABB
    const glm::vec2 d = center - closest;
    const float dist2 = glm::dot(d, d);
    const float r2    = radius * radius;

    // no collision
    if (dist2 >= r2) return false; // no collision

    // collision: compute normal
    if (dist2 > 1e-12f) { // safe normalize
        hitNormal = d / glm::sqrt(dist2);
        return true;
    }

    // center effectively inside AABB: pick nearest face
    const float faces[4] = { // (left, right, top, bottom)
            center.x - box.x,
            (box.x + box.w) - center.x,
            center.y - box.y,
            (box.y + box.h) - center.y
    };
    // smallest face distance
    const int idx = int(std::min_element(std::begin(faces), std::end(faces)) - std::begin(faces));
    switch (idx) { // (left, right, top, bottom)
        case 0: hitNormal = {-1.f,  0.f}; break;
        case 1: hitNormal = { 1.f,  0.f}; break;
        case 2: hitNormal = { 0.f, -1.f}; break;
        default:hitNormal = { 0.f,  1.f}; break;
    }
    return true;
}

bool Physics::aabbVsAABB(const SDL_FRect &a, const SDL_FRect &b) {
    return (a.x < b.x + b.w && a.x + a.w > b.x &&
            a.y < b.y + b.h && a.y + a.h > b.y);
}

void Physics::reflectBall(Entity_Ball &ball, const glm::vec2 &normal) {
    // simple reflection: r = v − 2 (v · n) n
    float dot = ball.velocity.x * normal.x + ball.velocity.y * normal.y;
    ball.velocity.x -= 2 * dot * normal.x;
    ball.velocity.y -= 2 * dot * normal.y;

    // ensure ball moves away from the surface
    glm::vec2 newPos = glm::vec2(ball.position.x, ball.position.y);
    if (normal.x != 0) {
        newPos.x = ball.position.x + (normal.x > 0 ? 1 : -1) * ball.radius;
    }
    if (normal.y != 0) {
        newPos.y = ball.position.y + (normal.y > 0 ? 1 : -1) * ball.radius;
    }
    ball.position = newPos;
}
