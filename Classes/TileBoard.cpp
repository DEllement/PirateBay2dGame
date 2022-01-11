#include "TileBoard.h"


TileBoard::TileBoard()
{
	tiles = Vector<TileRope*>();
	boats = Vector<Boat*>();
}


TileBoard::~TileBoard()
{
}




//init

bool TileBoard::initWithParams(int cols, int rows, Size tileSize){

	this->cols = cols;
	this->rows = rows;
	this->tileSize = tileSize;

	this->initUI();

	return true;
}

void TileBoard::initUI(){

	//If we need to add anythings to the board...
	/*for (int x = 0; x < this->cols; x++){
		for (int y = 0; y < this->rows; y++){

	

		}
	}*/
}

Boat* TileBoard::createNewBoat(){

	Boat* boat = Boat::create();

	this->addChild(boat);
	boat->setZOrder(1000);
	return boat;
}

//Draw

void TileBoard::onDrawPrimitives(const kmMat4 &transform, uint32_t flags)
{
	/*kmGLPushMatrix();
	kmGLLoadMatrix(&transform);

	//add your primitive drawing code here
	DrawPrimitives::setDrawColor4F(0, 0, 10, 1);
	for (int x = 1; x < cols; x++){
		for (int y = 1; y < rows; y++){
			//Draw Cols
			if (y == 1)
				DrawPrimitives::drawLine(Vec2(x*tileSize.width, 0), Vec2(x*tileSize.width, rows*tileSize.height));
			//Draw Rows
			if (x == 1)
				DrawPrimitives::drawLine(Vec2(0, y*tileSize.height), Vec2(cols*tileSize.width, y*tileSize.height));
		}
	}*/
}

void TileBoard::draw(Renderer *renderer, const kmMat4& transform, uint32_t flags)
{
	/*_customCommand.init(_globalZOrder);
	_customCommand.func = CC_CALLBACK_0(TileBoard::onDrawPrimitives, this, transform, flags);
	renderer->addCommand(&_customCommand);*/
}

//Events Callbacks
void TileBoard::onMouseUp(Event* e){

	EventMouse* eMouse = (EventMouse*)e;
	
	//Retreive which square IF is dragging tile...
	
}









//Static functions

TileBoard* TileBoard::CreateWithParams(int cols, int rows, Size tileSize){

	TileBoard* tileBoard = new TileBoard();
	if (tileBoard && tileBoard->initWithParams(cols, rows, tileSize)){
		tileBoard->autorelease();
		return tileBoard;
	}
	CC_SAFE_DELETE(tileBoard);
	return nullptr;
}
