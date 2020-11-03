#include "Triangle.hpp"

Triangle::Triangle(Material* material, Face &indices) {
    this -> material = material;
    this -> indices = new Face(indices);
    Vec3 fs = *(indices.v1) - *(indices.v0);
    Vec3 ft = *(indices.v2) - *(indices.v0);
    this -> normal = new Vec3(fs.cross(fs,ft));
    this -> normal -> normalize();
}

Triangle::~Triangle() {
    delete indices;
    delete normal;
}

float Triangle::intersectRay(Ray* ray) {
    float a,b,c,d,e,f,g,h,i,j,k,l;
	float beta,gamma,t;
	
	float eimhf,gfmdi,dhmeg,akmjb,jcmal,blmkc;

	float M;
	
	float dd;
	Vec3 ma,mb,mc;

	ma = *indices -> v0;
	mb = *indices -> v1;
	mc = *indices -> v2;
	
	a = ma.x - mb.x;
	b = ma.y - mb.y;
	c = ma.z - mb.z;

	d = ma.x - mc.x;
	e = ma.y - mc.y;
	f = ma.z - mc.z;
	
	g = ray -> d -> x;
	h = ray -> d -> y;
	i = ray -> d -> z;
	
	j = ma.x - ray -> e -> x;
	k = ma.y - ray -> e -> y;
	l = ma.z - ray -> e -> z;
	
	eimhf = e * i - h * f;
	gfmdi = g * f - d * i;
	dhmeg = d * h - e * g;
	akmjb = a * k - j * b;
	jcmal = j * c - a * l;
	blmkc = b * l - k * c;

	M = a * eimhf + b * gfmdi + c * dhmeg;
    if (M == 0) {
        return -1.0f;
    }

	t = -(f * akmjb + e * jcmal + d * blmkc) / M;
	if (t < 1.0) {
        return -1.0f;
    }

	gamma = (i*akmjb+h*jcmal+g*blmkc)/M;
	
	if (gamma<0 || gamma>1) {
        return -1.0f;
    }
	beta = ( j * eimhf + k * gfmdi + l * dhmeg) / M;
	
	if (beta < 0 || beta > (1 - gamma)) {
        return -1.0f;
    }
	return t;
}
