#ifndef FACE_HPP
#define FACE_HPP
#include "Vec3.hpp"
#include "Ray.hpp"

class Face{
    private:

        float a,b,c; // the edge between vertex0 and vertex1
        float d,e,f; // the edge between vertex0 and vertex2

        inline void preComputeEdges() {
            a = v0 -> x - v1 -> x;
            b = v0 -> y - v1 -> y;
            c = v0 -> z - v1 -> z;

            d = v0 -> x - v2 -> x;
            e = v0 -> y - v2 -> y;
            f = v0 -> z - v2 -> z;
        }

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
            preComputeEdges();
        }

        Face(const Face &copy) {
            v0 = copy.v0;
            v1 = copy.v1;
            v2 = copy.v2;
            this -> normal = new Vec3(*copy.normal);
            preComputeEdges();
        }

        ~Face() {
            delete normal;
        }

        inline double intersectRay(Ray* ray) {
            double g,h,i,j,k,l;
            double beta,gamma;
            double t;
            double tThreshold = ray -> minTValue;
            double eimhf,gfmdi,dhmeg,akmjb,jcmal,blmkc;
            double M;
            
            /**
             * a = ax - bx
             * b = ay - by
             * c = az - bz
             * d = ax - cx
             * e = ay - cy
             * f = az - cz 
             * g = dx
             * h = dy;
             * i = dz;
             * j = ax - ox
             * k = ay - oy
             * l = az - oz
            */
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
                return -1.0;
            }

            t = -(f * akmjb + e * jcmal + d * blmkc) / M;
            if (t < tThreshold) {
                return -1.0;
            }

            gamma = (i*akmjb+h*jcmal+g*blmkc)/M;
            
            if (gamma < 0.0 || gamma > 1.0) {
                return -1.0;
            }
            beta = ( j * eimhf + k * gfmdi + l * dhmeg) / M;
            
            if (beta < 0.0 || beta > (1.0 - gamma)) {
                return -1.0;
            }
            return t;
        }

};

#endif