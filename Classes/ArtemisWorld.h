#ifndef __ARTEMIS_WORLD_H__
#define __ARTEMIS_WORLD_H__

// artemis
#include "Artemis\Artemis.h"

// cocos2dx
#include "cocos2d.h"

#include "Constants.h"

// game
class CameraSystem;
class DirtySystem;
class RenderSystem;
class HudRenderSystem;
class HealthRenderSystem;
class TerrainRenderSystem;

namespace artemis
{
	class ArtemisWorld : public cocos2d::Component
	{
	public:
		ArtemisWorld();
		virtual ~ArtemisWorld();

	public: // artemis
		artemis::EntitySystem* setSystem(artemis::EntitySystem* system);
		artemis::World& getWorld();
		void removeAllEntities();

	public: // cocos2dx
		virtual bool init();
		virtual void update(float delta);


	private:
		artemis::World         _world;
	};
}


#endif // __ARTEMIS_WORLD_H__

