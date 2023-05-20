#include "types.h"

namespace kq
{

bool DynamicBody::outOfBounds(float x, float y, float width, float length)
{
    return (x < 0 || x > width  || y < 0 || y > length);
}

Entity::Entity() {}
Entity::~Entity() {}

} // namespace kq