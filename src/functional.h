#ifndef __FUNCTIONAL_H__
#define __FUNCTIONAL_H__

namespace stl
{

template <class T>
class equal_to
{
public:
    using result_type = bool;
    using first_argument_type = T;
    using second_argument_type = T;

public:
    result_type operator()(const first_argument_type& x, const second_argument_type& y) const
    {
        return x == y;
    }    
};

}

#endif