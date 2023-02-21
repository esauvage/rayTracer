#include "light.h"

Light::Light(const Vec3f& position, const Vec3f& couleur)
	:pos(position), col(couleur * (1./255.))
{
}
