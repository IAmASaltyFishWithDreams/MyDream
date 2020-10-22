#ifndef __PROTOCPP_OP_TEMPLATE_CMD_HPP__
#define __PROTOCPP_OP_TEMPLATE_CMD_HPP__

#include "../../utils/macro.hpp"

NS_PROTOCPP_BEGIN
    enum{
%for key,vaule in Maps:
        ${value} = ${key},
%endfor
};
NS_PROTOCPP_END

#endif
