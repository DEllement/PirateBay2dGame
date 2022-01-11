#include "Constants.h"

const char* Constants::Groups::TILES = "tiles";
const char* Constants::Groups::PLAYER = "player";
const char* Constants::Groups::ITEMS = "items";
const char* Constants::Groups::ENNEMIES = "ennemies";
const char* Constants::Groups::LEVEL = "level";
const char* Constants::Groups::PLAYER_SHIP = "playerShip";
const char* Constants::Groups::OBJECTIVES = "objectives";

const char* Constants::Tags::PLAYER = "player";
const char* Constants::Tags::LEVEL = "level";
const char* Constants::Tags::PLAYER_SHIP = "playerShip";

//cocos2d::Size Constants::UI::designSize(640.0f, 960.0f);
//cocos2d::Size Constants::UI::stageSize(2160.0f, 2160.0f);


artemis::ComponentType Constants::ComponentType::SEA_PATH;
//artemis::ComponentType Constants::ComponentType::MINE;
artemis::ComponentType Constants::ComponentType::SHIP;
//artemis::ComponentType Constants::ComponentType::PIRATE_SHIP;
artemis::ComponentType Constants::ComponentType::ISLAND;
artemis::ComponentType Constants::ComponentType::CITY;
artemis::ComponentType Constants::ComponentType::SPRITE;
artemis::ComponentType Constants::ComponentType::ROGUE_TILE;
artemis::ComponentType Constants::ComponentType::VOLCANO;
artemis::ComponentType Constants::ComponentType::SPYGLASS;
artemis::ComponentType Constants::ComponentType::COMPASS;
artemis::ComponentType Constants::ComponentType::ECONOMIC_ASSET;
artemis::ComponentType Constants::ComponentType::POSITION;
artemis::ComponentType Constants::ComponentType::PLAYER_DATA;
artemis::ComponentType Constants::ComponentType::LEVEL_DATA;
artemis::ComponentType Constants::ComponentType::ENEMY_DATA;
artemis::ComponentType Constants::ComponentType::VORTEX_DATA;
artemis::ComponentType Constants::ComponentType::GRID_ELEMENT;
artemis::ComponentType Constants::ComponentType::GRID_MATRIX;

//SeaPath UI Event
const char* Constants::GameEvent::SP_DISTRIBUTER_ITEM_MOUSEDOWN = "SP_DISTRIBUTER_ITEM_MOUSEDOWN";
const char* Constants::GameEvent::SP_DISTRIBUTER_ITEM_CLICK = "SP_DISTRIBUTER_ITEM_CLICK";
const char* Constants::GameEvent::SP_DISTRIBUTER_MOUSEUP = "SP_DISTRIBUTER_MOUSEUP";
//Grid Event
const char* Constants::GameEvent::GRID_MOUSEUP = "GRID_MOUSEUP";
const char* Constants::GameEvent::GRID_MOUSEMOVE = "GRID_MOUSEMOVE";
const char* Constants::GameEvent::GRID_CLICK = "GRID_CLICK";
//Special Object UI Event
const char* Constants::GameEvent::SO_DISTRIBUTER_ITEM_MOUSEDOWN = "SO_DISTRIBUTER_ITEM_MOUSEDOWN";
const char* Constants::GameEvent::SO_DISTRIBUTER_ITEM_CLICK = "SO_DISTRIBUTER_ITEM_CLICK";
const char* Constants::GameEvent::SO_DISTRIBUTER_MOUSEUP = "SO_DISTRIBUTER_MOUSEUP";
//SeaPath Component Event
const char* Constants::GameEvent::SP_ADDED = "SP_ADDED";
const char* Constants::GameEvent::SP_REMOVED = "SP_REMOVED";
//Systems
const char* Constants::GameEvent::SP_ADDED_FROM_DISTRIBUTER = "SP_ADDED_FROM_DISTRIBUTER";
const char* Constants::GameEvent::DEBUG_LOG = "DEBUG_LOG";
const char* Constants::GameEvent::PAUSE_LOGIC = "PAUSE_LOGIC";
const char* Constants::GameEvent::RESUME_LOGIC = "RESUME_LOGIC";
//UI
const char* Constants::GameEvent::SHOW_UI_POPUP = "SHOW_UI_POPUP";
const char* Constants::GameEvent::REQUEST_SCENE = "REQUEST_SCENE";