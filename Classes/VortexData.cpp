#include "VortexData.h"


VortexData::VortexData():
cellX(0),
cellY(0),
colspan(1),
rowspan(1),
haveLifeSpan(false),
lifeSpanDelta(0),
lifeSpan(0),
showAtIntervalDelta(0),
showAtInterval(false),
showAtIntervalDelay(0),
showAtRandomPosition(false),
isActive(false)
{
}


VortexData::~VortexData()
{
}
