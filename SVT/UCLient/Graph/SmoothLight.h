#include "../../Generation/Entry.h"

namespace ULight
{
	struct SurfLight {
		float l[2][2];
		SurfLight() {
			l[0][0] = l[0][1] = l[1][0] = l[1][1] = 1.0f;
		}
	};
	SurfLight GenLight(int x, int y, int z, Generation::WorldUnit* unit, unsigned short w);
}