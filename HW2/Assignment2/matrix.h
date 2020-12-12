#include "math.h"
#include "rotation.h"
#include "scaling.h"
#include "translation.h"
#include "vector4f.h"
#include "vector3f.h"
#include <iostream>

#ifndef MATRIX_H
#define MATRIX_H
typedef enum axis_type {
    X,
    Y,
    Z
} AXIS;

typedef enum trans_type {
    SCALE,
    TRANSLATE,
    ROTATE
} TRANSFORMATION;


class Matrix{
    
    private:

        void getRotationX(float a, float b, float c) {
            float d = sqrt(b*b + c*c);
            float cd;
            float bd;
            if (d == 0) {
                cd = 0;
                bd = 0;
            } else {
                cd = c/d;
                bd = b/d;
            }
            m[0][0] = 1;
            m[0][1] = 0;
            m[0][2] = 0;
            m[0][3] = 0;

            m[1][0] = 0;
            m[1][1] = cd;
            m[1][2] = -bd;
            m[1][3] = 0;

            m[2][0] = 0;
            m[2][1] = bd;
            m[2][2] = cd;
            m[2][3] = 0;

            m[3][0] = 0;
            m[3][1] = 0;
            m[3][2] = 0;
            m[3][3] = 1;
        }

        void getRotationY(float a, float b, float c) {
            float d = sqrt(b*b + c*c);
            m[0][0] = d;
            m[0][1] = 0;
            m[0][2] = -a; //
            m[0][3] = 0;
            
            m[1][0] = 0;
            m[1][1] = 1;
            m[1][2] = 0;
            m[1][3] = 0;

            m[2][0] = a;
            m[2][1] = 0;
            m[2][2] = d;
            m[2][3] = 0;
            
            m[3][0] = 0;
            m[3][1] = 0;
            m[3][2] = 0;
            m[3][3] = 1;
        }

        void getRotationZ(float angle){

            // float theta = (angle / 180) * M_PI;
            float theta = angle * M_PI / 180;
            m[0][0] = cos(theta);
            m[0][1] = -sin(theta);
            m[0][2] = 0;
            m[0][3] = 0;
            
            m[1][0] = sin(theta);
            m[1][1] = cos(theta);
            m[1][2] = 0;
            m[1][3] = 0;

            m[2][0] = 0;
            m[2][1] = 0;
            m[2][2] = 1;
            m[2][3] = 0;
            
            m[3][0] = 0;
            m[3][1] = 0;
            m[3][2] = 0;
            m[3][3] = 1;
        }

        void getTranslationMatrix(float tx, float ty, float tz) {
            m[0][0] = 1;
            m[0][1] = 0;
            m[0][2] = 0;
            m[0][3] = tx;

            m[1][0] = 0;
            m[1][1] = 1;
            m[1][2] = 0;
            m[1][3] = ty;

            m[2][0] = 0;
            m[2][1] = 0;
            m[2][2] = 1;
            m[2][3] = tz;

            m[3][0] = 0;
            m[3][1] = 0;
            m[3][2] = 0;
            m[3][3] = 1;
        }
        void getScaleMatrix(float sx, float sy, float sz) {
            m[0][0] = sx;
            m[0][1] = 0;
            m[0][2] = 0;
            m[0][3] = 0;

            m[1][0] = 0;
            m[1][1] = sy;
            m[1][2] = 0;
            m[1][3] = 0;

            m[2][0] = 0;
            m[2][1] = 0;
            m[2][2] = sz;
            m[2][3] = 0;

            m[3][0] = 0;
            m[3][1] = 0;
            m[3][2] = 0;
            m[3][3] = 1;
        }
    
    public:

        float m[4][4];
        /*
        MAIN:
        Calculate u
        one = Matrix (u.x, u.y, u.z, X)
        two = Matrix (u.x, u.y, u.z, Y)
        three = Matrix(u.x, u.y, u.z, Z, angle)
        three * two * one
        */
        Matrix() {}
        // Rotation
        Matrix (float normX, float normY, float normZ, AXIS axis, float angle=0) {
            if (axis == X) {
                this -> getRotationX(normX, normY, normZ);
            } else if (axis == Y) {
                this -> getRotationY(normX, normY, normZ);
            } else {
                this -> getRotationZ(angle);
            }
        }
        // Translation and Scaling
        Matrix(float x, float y, float z, TRANSFORMATION t) {
            if ( t == SCALE ) {
                this -> getScaleMatrix(x, y ,z); 
            }
            else {
                this -> getTranslationMatrix(x, y, z);
            }
        }
        Matrix (const Matrix& cpy) {
            for (int i = 0; i < 4; i++){
                for (int j = 0; j < 4; j++) {
                    this -> m[i][j] = cpy.m[i][j];
                }
            }
        }
        ~Matrix() {}

        Matrix operator*(const Matrix& rhs){
            Matrix output;
            
            output.m[0][0] = m[0][0] * rhs.m[0][0] + m[0][1] * rhs.m[1][0] + m[0][2] * rhs.m[2][0] + m[0][3] * rhs.m[3][0];
            output.m[0][1] = m[0][0] * rhs.m[0][1] + m[0][1] * rhs.m[1][1] + m[0][2] * rhs.m[2][1] + m[0][3] * rhs.m[3][1];
            output.m[0][2] = m[0][0] * rhs.m[0][2] + m[0][1] * rhs.m[1][2] + m[0][2] * rhs.m[2][2] + m[0][3] * rhs.m[3][2];
            output.m[0][3] = m[0][0] * rhs.m[0][3] + m[0][1] * rhs.m[1][3] + m[0][2] * rhs.m[2][3] + m[0][3] * rhs.m[3][3];

            output.m[1][0] = m[1][0] * rhs.m[0][0] + m[1][1] * rhs.m[1][0] + m[1][2] * rhs.m[2][0] + m[1][3] * rhs.m[3][0];
            output.m[1][1] = m[1][0] * rhs.m[0][1] + m[1][1] * rhs.m[1][1] + m[1][2] * rhs.m[2][1] + m[1][3] * rhs.m[3][1];
            output.m[1][2] = m[1][0] * rhs.m[0][2] + m[1][1] * rhs.m[1][2] + m[1][2] * rhs.m[2][2] + m[1][3] * rhs.m[3][2];
            output.m[1][3] = m[1][0] * rhs.m[0][3] + m[1][1] * rhs.m[1][3] + m[1][2] * rhs.m[2][3] + m[1][3] * rhs.m[3][3];

            output.m[2][0] = m[2][0] * rhs.m[0][0] + m[2][1] * rhs.m[1][0] + m[2][2] * rhs.m[2][0] + m[2][3] * rhs.m[3][0];
            output.m[2][1] = m[2][0] * rhs.m[0][1] + m[2][1] * rhs.m[1][1] + m[2][2] * rhs.m[2][1] + m[2][3] * rhs.m[3][1];
            output.m[2][2] = m[2][0] * rhs.m[0][2] + m[2][1] * rhs.m[1][2] + m[2][2] * rhs.m[2][2] + m[2][3] * rhs.m[3][2];
            output.m[2][3] = m[2][0] * rhs.m[0][3] + m[2][1] * rhs.m[1][3] + m[2][2] * rhs.m[2][3] + m[2][3] * rhs.m[3][3];

            output.m[3][0] = m[3][0] * rhs.m[0][0] + m[3][1] * rhs.m[1][0] + m[3][2] * rhs.m[2][0] + m[3][3] * rhs.m[3][0];
            output.m[3][1] = m[3][0] * rhs.m[0][1] + m[3][1] * rhs.m[1][1] + m[3][2] * rhs.m[2][1] + m[3][3] * rhs.m[3][1];
            output.m[3][2] = m[3][0] * rhs.m[0][2] + m[3][1] * rhs.m[1][2] + m[3][2] * rhs.m[2][2] + m[3][3] * rhs.m[3][2];
            output.m[3][3] = m[3][0] * rhs.m[0][3] + m[3][1] * rhs.m[1][3] + m[3][2] * rhs.m[2][3] + m[3][3] * rhs.m[3][3];
            return output;
        }

        fst::math::Vector4f operator*(const fst::math::Vector4f& rhs){
            fst::math::Vector4f output;
            
            output.x = m[0][0] * rhs.x + m[0][1] * rhs.y + m[0][2] * rhs.z + m[0][3] * rhs.w;
            output.y = m[1][0] * rhs.x + m[1][1] * rhs.y + m[1][2] * rhs.z + m[1][3] * rhs.w;
            output.z = m[2][0] * rhs.x + m[2][1] * rhs.y + m[2][2] * rhs.z + m[2][3] * rhs.w;
            output.w = m[3][0] * rhs.x + m[3][1] * rhs.y + m[3][2] * rhs.z + m[3][3] * rhs.w;

            return output;
        }

        friend std::ostream& operator<<(std::ostream& os, const Matrix &m) {
            for (int i = 0; i < 4; i++) {
                for (int j = 0; j < 4; j++) {
                    os << m.m[i][j] << " ";
                }
                os << std::endl;
            }
            return os;
        }                                                                                                          
};

#endif