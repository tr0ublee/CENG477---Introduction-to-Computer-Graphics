#include "./Objects/Vec3.hpp"



typedef unsigned char RGB[3];



using namespace std;

int main(int argc, char* argv[])

{

    Vec3 a = Vec3(0.5, 0.3, 0.2);

    Vec3 b = Vec3(0.8, 0.1, 0.4);

    Vec3 c = Vec3(0.2, 0.1, 0.1);

    cout <<"<< Test" << endl;

    std::cout<<a<<endl;

    std::cout<<b<<endl;

    std::cout<<c<<endl;

    

    cout << "= Test"<< endl;

    c = a;

    cout << c <<endl;

    

    cout << "+ Test" << endl;

    c = a+b;

    cout << c <<endl;

    

    cout << "-Test" << endl;

    c = a - b;

    cout << c << endl;



    cout << "* Test with 3" << endl;

    c = c*3;

    cout << c <<endl;



    cout <<"Dot test" << endl;

    cout << c.dot(b) << endl;



    cout <<"== test" << endl;

    c = a; 

    cout << (c==a) << endl;

    cout << (c==b) << endl;



    cout <<"Cross Test" << endl;

    c = c.cross(c,b);

    cout << c << endl;



    cout <<"Normalize Test" <<endl;

    c = Vec3(3,4,5);

    c.normalize();

    cout << c <<endl;



    cout << "Length Test" << endl;

    cout << c.getLength() << endl;

    cout << c.getLengthSquared()<<endl;







}


