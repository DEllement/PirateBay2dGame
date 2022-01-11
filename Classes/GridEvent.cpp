#include "GridEvent.h"


std::string GridEvent::_TYPE = "GridEvent";

GridEvent::GridEvent()
:EventCustom(_TYPE)
{
	canDropSeaPath = false;
}