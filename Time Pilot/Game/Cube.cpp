#include "Cube.h"

Cube::Cube()
{
}

Cube::Cube(const glm::vec3& centre, const glm::vec3& dimensions)
{
    mCentre = centre;
    mDimensions = dimensions;
}

void Cube::SetCentre(const glm::vec3& centre)
{
    mCentre = centre;
}

void Cube::SetDimensions(const glm::vec3& dimensions)
{
    mDimensions = dimensions;
}

bool Cube::DoesPointFallInCube(const glm::vec3& point)
{
    return (
        (
            point.x > (mCentre.x - (mDimensions.x / 2)) &&
            point.x < (mCentre.x + (mDimensions.x / 2))
        )
        &&
        (
            point.y > (mCentre.y - (mDimensions.y / 2)) &&
            point.y < (mCentre.y + (mDimensions.y / 2))
        )
        &&
        (
            point.z > (mCentre.z - (mDimensions.z / 2)) &&
            point.z < (mCentre.z + (mDimensions.z / 2))
        )
    );
}

Cube::~Cube()
{
}
