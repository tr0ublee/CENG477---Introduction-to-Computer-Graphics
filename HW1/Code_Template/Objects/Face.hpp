#ifndef FACE_HPP
#define FACE_HPP
#include "Vec3.hpp"
#include "Ray.hpp"

class Face{
    private:

        float a,b,c,d,e,f;

    public:
    
        Vec3* v0;
        Vec3* v1;
        Vec3* v2;
        Vec3* normal;

        Face(Vec3* v0, Vec3* v1, Vec3* v2){
            this -> v0 = v0;
            this -> v1 = v1;
            this -> v2 = v2;
            Vec3 fs = *(v1) - *(v0);
            Vec3 ft = *(v2) - *(v0);
            this -> normal = new Vec3(fs.cross(fs,ft));
            this -> normal -> normalize();
            Vec3 ma,mb,mc;

            ma = *v0;
            mb = *v1;
            mc = *v2;
            
            a = ma.x - mb.x;
            b = ma.y - mb.y;
            c = ma.z - mb.z;

            d = ma.x - mc.x;
            e = ma.y - mc.y;
            f = ma.z - mc.z;
        }

        Face(const Face &copy) {
            v0 = copy.v0;
            v1 = copy.v1;
            v2 = copy.v2;
            this -> normal = new Vec3(*copy.normal);

            Vec3 ma,mb,mc;

            ma = *v0;
            mb = *v1;
            mc = *v2;
            
            a = ma.x - mb.x;
            b = ma.y - mb.y;
            c = ma.z - mb.z;

            d = ma.x - mc.x;
            e = ma.y - mc.y;
            f = ma.z - mc.z;
        }

        ~Face() {
            delete normal;
        }

        inline float intersectRay(Ray* ray) {
            float g,h,i,j,k,l;
            float beta,gamma,t;
            
            float eimhf,gfmdi,dhmeg,akmjb,jcmal,blmkc;

            float M;
            
            float dd;
            
            g = ray -> d -> x;
            h = ray -> d -> y;
            i = ray -> d -> z;
            
            j = v0 -> x - ray -> e -> x;
            k = v0 -> y - ray -> e -> y;
            l = v0 -> z - ray -> e -> z;
            
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
            if (t < 0.0) {
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

};

#endif