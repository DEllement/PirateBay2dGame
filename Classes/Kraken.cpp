#include "Kraken.h"


Kraken::Kraken()
{
}


Kraken::~Kraken()
{
}

Kraken* Kraken::create(){
	
	Kraken * kraken = new Kraken();
	if (kraken && kraken->initWithFile("kraken_100x.png")){
		kraken->autorelease();

		

		return kraken;
	}
	CC_SAFE_DELETE(kraken);
	return nullptr;
}
 