#ifndef __PAIR_H__
#define __PAIR_H__

namespace stl 
{

template <typename T1, typename T2>
class pair
{
public:
    using first_type = T1;
    using second_type = T2;

public:
    first_type first;
    second_type second;
};

}

#endif