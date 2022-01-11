#pragma once
#include "cocos2d.h"
#include "TileRope.h"
#include "Boat.h"

USING_NS_CC;

class TileBoard :
	public Layer
{
public:
	TileBoard();
	~TileBoard();

	Size tileSize = Size(60, 60);
	int cols = 10;
	int rows = 10;
	Vector<TileRope*> tiles;
	Vector<Boat*> boats;

	Boat* createNewBoat();

	/* Override */
	virtual void draw(Renderer *renderer, const Mat4 &transform, uint32_t flags) override;

	/* Static */

	static TileBoard* CreateWithParams(int cols, int rows, Size tileSize);

protected:
	bool initWithParams(int cols, int rows, Size tileSize);
	void initUI();
	void onMouseUp(Event* e); 

private:
	CustomCommand _customCommand;
	void onDrawPrimitives(const kmMat4 &transform, uint32_t flags);

};

