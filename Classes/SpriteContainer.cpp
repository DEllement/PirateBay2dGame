#include "SpriteContainer.h"


SpriteContainer::SpriteContainer()
{
}


SpriteContainer::~SpriteContainer()
{
}

void SpriteContainer::setOpacity(GLubyte opacity){
	
	Sprite::setOpacity(opacity);

	for (Node* node : this->getChildren()){
		node->setOpacity(opacity);
	}

}