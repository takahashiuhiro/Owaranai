#include "OwaranaiEngine/OwaranaiEngineInclude.h"
int main()
{ 
    Segment t = Segment(Vector(0,0), Vector(0,2));
    Segment y = Segment(std::move(Vector(-1,1)), std::move(Vector(1,1)));
    Vector res;
    std::cout<<t.SegmentCross2D(y,res)<<std::endl;
    res.PrintData();
}

