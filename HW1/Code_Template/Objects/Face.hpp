#ifndef FACE_HPP
#define FACE_HPP

class Face{
    public:
    
        Vec3* v0;
        Vec3* v1;
        Vec3* v2;
        Face(Vec3* v0, Vec3* v1, Vec3* v2){
            this -> v0 = v0;
            this -> v1 = v1;
            this -> v2 = v2;
        }
        ~Face(){
            
        }
        Face(const Face &copy) {
            v0 = copy.v0;
            v1 = copy.v1;
            v2 = copy.v2;
        }
};

#endif