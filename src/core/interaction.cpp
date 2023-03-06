#include "interaction.h"

namespace r 
{

Interaction::Interaction() {
    happened=false;
        coords=Vector3f();
        normal=Vector3f();
        distance= std::numeric_limits<double>::max();
        m=nullptr;
}


}