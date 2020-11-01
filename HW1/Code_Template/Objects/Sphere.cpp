#include "Sphere.hpp"
#include <iostream>

float Sphere::intersectRay(Ray* ray) {
	float A,B,C; //constants for the quadratic function	
	float delta;
    float t,t1,t2;

	C = (ray -> e -> x - center -> x) * (ray -> e -> x - center -> x) + 
        (ray -> e -> y - center -> y) * (ray -> e -> y - center -> y) +
        (ray -> e -> z - center -> z) * (ray -> e -> z - center -> z) - 
        r * r;

	B = 2 * ray -> d -> x * (ray -> e -> x - center -> x) + 
        2 * ray -> d -> y * (ray -> e -> y - center -> y) + 
        2 * ray -> d -> z * (ray -> e -> z - center -> z);
	
	A = ray -> d -> x * ray -> d -> x + 
        ray -> d -> y * ray -> d -> y + 
        ray -> d -> z * ray -> d -> z;
        
	delta = B * B - 4 * A * C;
	
	if (delta < 0.0f) {
        return -1.0;
    }

	else if (delta - 0.0f < 0.000001) {
		t = -B / (2*A);
	}
	else {
		delta = sqrt(delta);
		A = 2 * A;
		t1 = (-B + delta) / A;
		t2 = (-B - delta) / A;        
        t1 = std::min(t1,t2);
		if (t1 >= 1.0) {
            t = t1;
        }
		else {
            t = -1.0f; // time returned negative. so it doesnt intersect
        }
    }
	return t;
}

Sphere::Sphere(Material* material, Vec3* center, float r) {
    this -> material = material;
    this -> center = center;
    this -> r = r;
}

Sphere::~Sphere() {

}