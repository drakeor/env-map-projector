#ifndef _SKYBOX_SURF_H_
#define _SKYBOX_SURF_H_

#include <cstdint>

namespace EnvProj
{
    enum SkyboxSurf : uint32_t
    {
        TopSurf = 0,
        BottomSurf = 1,
        LeftSurf = 2,
        RightSurf = 3,
        FrontSurf = 4,
        BackSurf = 5
    };
}

#endif