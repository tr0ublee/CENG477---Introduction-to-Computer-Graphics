#include "Vec3.hpp"
using namespace std;

Vec3::Vec3() {
    this -> x = 0;
    this -> y = 0;
    this -> z = 0;
}

Vec3::Vec3(double x, double y, double z) {
    this -> x = x;
    this -> y = y;
    this -> z = z;
}

Vec3::Vec3(const Vec3 &b) { 
    this -> x = b.x;
    this -> y = b.y;
    this -> z = b.z;
}

Vec3::~Vec3() {

}

Vec3 Vec3::operator+(const Vec3 &b) {
    double x = this -> x + b.x;
    double y = this -> y + b.y;
    double z = this -> z + b.z;
    Vec3 returned = Vec3(x, y, z);
    return returned; 
}

Vec3 Vec3::operator-(const Vec3 &b) {
    double x = this -> x - b.x;
    double y = this -> y - b.y;
    double z = this -> z - b.z;
    Vec3 returned = Vec3(x, y, z);
    return Vec3(x,y,z);
}
Vec3 Vec3::operator*(const double c) {
    double x = this -> x * c;
    double y = this -> y * c;
    double z = this -> z * c;
    return Vec3(x, y, z);
}
double Vec3::dot(const Vec3 &b) {
    return this -> x* b.x + this -> y * b.y + this -> z * b.z;
}
bool Vec3::operator==(const Vec3 &b) {
    double e = 0.000000001;
	if ((ABS((this -> x - b.x)) < e) && (ABS((this -> y - b.y)) < e) && (ABS((this -> z - b.z)) < e)) { 
        return true;
    }
    return false;
} 

Vec3& Vec3::operator=(const Vec3 &b) {
    this -> x = b.x;
    this -> y = b.y;
    this -> z = b.z;
    return *this;
} 
Vec3 Vec3::cross(Vec3 &a, Vec3 &b) {
    Vec3 tmp;
	tmp.x = a.y*b.z-b.y*a.z;
	tmp.y = b.x*a.z-a.x*b.z;
	tmp.z = a.x*b.y-b.x*a.y;
	return tmp;
}

void Vec3::normalize() {
    double d;
	d = this -> getLength();
	this -> x = this -> x/d;
	this -> y = this -> y/d;
	this -> z = this -> z/d;
}

std::ostream& operator<<(std::ostream& os, const Vec3& b) {
    return os << "X: " << b.x << " Y: " << b.y << " Z: " << b.z; 
}
        
double Vec3::getLengthSquared() {
    double x = this -> x;
    double y = this -> y;
    double z = this -> z;
    return x*x + y*y + z*z;
}
double Vec3::getLength() {
    double x = this -> x;
    double y = this -> y;
    double z = this -> z;
    return sqrt(x*x + y*y + z*z);
}