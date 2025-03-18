#include "func.h"
#include "jetStructure.h"
#include "blocks.h"



int main(){
    particle a1;
    a1._id = 1;
    a1._st = 1;
    a1._Ei = 1;
    a1.depth = 1;
    a1.n_mothers = 0;

    particle b1, b2;
    b1._id = 2;
    b1._st = 1;
    b1._Ei = 1;
    b1.depth = 2;
    b1.n_mothers = 1;
    b1.mother1 = &a1;

    b2._id = 3;
    b2._st = 1;
    b2._Ei = 1;
    b2.depth = 2;
    b2.n_mothers = 1;
    b2.mother1 = &a1;

    particle c1, c2, c3;
    c1._id = 4;
    c1._st = 1;
    c1._Ei = 1;
    c1.depth = 3;
    c1.n_mothers = 1;
    c1.mother1 = &b1;

    c2._id = 5;
    c2._st = 1;
    c2._Ei = 1;
    c2.depth = 3;
    c2.n_mothers = 1;
    c2.mother1 = &b2;

    c3._id = 6;
    c3._st = 1;
    c3._Ei = 1;
    c3.depth = 3;
    c3.n_mothers = 1;
    c3.mother1 = &b2;

    particle d1;
    d1._id = 7;
    d1._st = 1;
    d1._Ei = 1;
    d1.depth = 4;
    d1.n_mothers = 2;
    d1.mother1 = &c1;
    d1.mother2 = &c2;

    particle e1, e2;
    e1._id = 8;
    e1._st = 1;
    e1._Ei = 1;
    e1.depth = 5;
    e1.n_mothers = 1;
    e1.mother1 = &d1;

    e2._id = 9;
    e2._st = 1;
    e2._Ei = 1;
    e2.depth = 5;
    e2.n_mothers = 2;
    e2.mother1 = &d1;
    e2.mother2 = &c3;


    jetStructure j;
    j.pushBackParticle(a1);
    j.pushBackParticle(b1);
    j.pushBackParticle(b2);
    j.pushBackParticle(c1);
    j.pushBackParticle(c2);
    j.pushBackParticle(c3);
    j.pushBackParticle(d1);
    j.pushBackParticle(e1);
    j.pushBackParticle(e2);


//    j.print();

    blocks b(j);
    b.createFile();
    b.printToFile();
    return 0;

}