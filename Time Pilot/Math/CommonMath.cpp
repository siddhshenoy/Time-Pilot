#include "CommonMath.h"


float CommonMath::DistanceBetweenPoints3D(const glm::vec3& point1, const glm::vec3& point2) {
	return Length3D((point2 - point1));
}
float CommonMath::DistanceBetweenPoints2D(const glm::vec2& point1, const glm::vec2& point2) {
	return glm::length((point2 - point1));
}

float CommonMath::Length3D(const glm::vec3& vec) {
	return sqrt((vec.x * vec.x) + (vec.y * vec.y) + (vec.z * vec.z));
	//return fast_square_root((vec.x * vec.x) + (vec.y * vec.y) + (vec.z * vec.z));
}
float CommonMath::Length2D(const glm::vec2& vec) {
	return sqrt((vec.x * vec.x) + (vec.y * vec.y));
	//return fast_square_root((vec.x * vec.x) + (vec.y * vec.y));
}
// ==========================================================================================
// Inspired by Doom game....
// ==========================================================================================
float CommonMath::fast_inverse_square_root(float number) {
	long i;
	float x2, y;
	const float threehalfs = 1.5F;
	x2 = number * 0.5F;
	y = number;
	i = *(long*)&y;
	i = 0x5f3759df - (i >> 1);               
	y = *(float*)&i;
	y = y * (threehalfs - (x2 * y * y));   
	return y;
}
float CommonMath::fast_square_root(float number) {
	return 1 / fast_inverse_square_root(number);
}