#pragma once

#include "Artemis/artemis.h"

namespace Constants
{
	class GameParams{
		public:
			static const float screenWidth;
			static const float screenHeight;

			static const int gridElementWidth = 88;
			static const int gridElementHeight = 88;

			static const int nbCols = 10;
			static const int nbRows = 8;

	};
	enum class GameScene
	{
		INTRO,
		MAIN_MENU,
		PUBLISHER_PAGE,
		PLAYHEAVEN,
		OPTIONS,
		SUBSCRIBE,
		MAP,
		TROPHY_ROOM,
		GAME_SESSION,
		CUTSCENE
	};
	enum class GamePopUp
	{
		IN_GAME_MENU,
		GAME_OBJECTIVE,
		LEVEL_SUCCESS,
		LEVEL_FAILED,
		PAUSE,
		SHOP
	};
	class Groups
	{
	public:
		static const char* TILES;
		static const char* PLAYER;
		static const char* PLAYER_SHIP;
		static const char* ITEMS;
		static const char* ENNEMIES;
		static const char* LEVEL;
		static const char* OBJECTIVES;
	};

	class Tags{
	public:
		static const char* PLAYER;
		static const char* PLAYER_SHIP;
		static const char* LEVEL;
	};

	class ComponentType
	{
	public:
		static artemis::ComponentType SEA_PATH;
		static artemis::ComponentType ITEM;
		//static artemis::ComponentType MINE;
		static artemis::ComponentType SHIP;
		//static artemis::ComponentType PIRATE_SHIP;
		static artemis::ComponentType ISLAND;
		static artemis::ComponentType CITY;
		static artemis::ComponentType SPRITE;
		static artemis::ComponentType ROGUE_TILE;
		static artemis::ComponentType VOLCANO;
		static artemis::ComponentType SPYGLASS;
		static artemis::ComponentType COMPASS;
		static artemis::ComponentType ENEMY_DATA;
		static artemis::ComponentType ECONOMIC_ASSET;
		static artemis::ComponentType POSITION;
		static artemis::ComponentType PLAYER_DATA;
		static artemis::ComponentType LEVEL_DATA;
		static artemis::ComponentType VORTEX_DATA;
		static artemis::ComponentType GRID_ELEMENT;
		static artemis::ComponentType GRID_MATRIX;
	};
	enum class EnemyType{
		KRAKEN,
		PIRATE_SHIP,
		MINE,
		VORTEX
	};
	enum class ItemType{
		BOMB,
	};

	enum class ZOrder
	{
		None = 0,
		Background,
		Water,
		TileBoard,
		GameEntity,
		Hud,
		PopUp,
		InGameMenu,
		DragLayer
	};

	class GameEvent{
	public:
		//SeaPath UI Event
		static const char* SP_DISTRIBUTER_ITEM_MOUSEDOWN;
		static const char* SP_DISTRIBUTER_ITEM_CLICK;
		static const char* SP_DISTRIBUTER_MOUSEUP;
		
		//Grid
		static const char* GRID_MOUSEUP;
		static const char* GRID_MOUSEMOVE;
		static const char* GRID_CLICK;

		//Boat UI Event
		//static const char* BOAT_GRID_CLICK; 
		
		//Special Object UI Event
		static const char* SO_DISTRIBUTER_ITEM_MOUSEDOWN;
		static const char* SO_DISTRIBUTER_ITEM_CLICK;
		static const char* SO_DISTRIBUTER_MOUSEUP;
		//SeaPath Component Event
		static const char* SP_ADDED;
		static const char* SP_REMOVED;

		//Systems
		static const char* SP_ADDED_FROM_DISTRIBUTER;
		static const char* DEBUG_LOG;
		static const char* PAUSE_LOGIC;
		static const char* RESUME_LOGIC;

		//UI
		static const char* SHOW_UI_POPUP;
		static const char* REQUEST_SCENE;
	};

	

}


