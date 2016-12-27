#include <jni.h>
#include <dlfcn.h>
#include <android/log.h>
#include <stdlib.h>
#include <Substrate.h>

#include "mcpe/gamemode/Gamemode.h"
#include "mcpe/client/MinecraftClient.h"
#include "mcpe/client/gui/screen/ScreenChooser.h"
#include "mcpe/entity/player/Player.h"
#include "mcpe/client/gui/GuiData.h"
#include "mcpe/item/ItemInstance.h"
#include "mcpe/client/gui/screen/Screen.h"
#include "mcpe/util/Util.h"
#include "mcpe/item/Item.h"
#include "mcpe/item/BlockItem.h"
#include "mcpe/item/AuxDataBlockItem.h"
#include "mcpe/block/Block.h"
#include "mcpe/block/BlockGraphics.h"
#include "mcpe/client/gui/screen/InventoryScreen.h"

MinecraftClient* mcClient;

static void (*_useItemOn)(GameMode*, Player&, ItemInstance*, const BlockPos&, signed char&, const Vec3&);
static void (*_initClient)(MinecraftClient*);
static void (*_initItems)();
static void (*_initCreativeItems)();

static void initClient(MinecraftClient* self)
{
	_initClient(self);
	::mcClient=self;
}

static void useItemOn(GameMode* self, Player& player, ItemInstance* item, const BlockPos& pos, signed char& side, const Vec3& idk){
	_useItemOn(self, player, item, pos, side, idk);
	if(item->getId()==500)
		mcClient->getScreenChooser()->pushWorkbenchScreen(pos, false);	
}

static void initItems(){
	_initItems();
	
	Item::mItems[500] = new Item("portable_workbench", 500- 256);
	Item::mItems[500]->setIcon("portable_craft", 0);
	Item::mItems[500]->setCategory(CreativeItemCategory::TOOLS);
}

static void initCreativeItems(){
	_initCreativeItems();
	
	Item::addCreativeItem(500,0);
}

JNIEXPORT jint JNI_OnLoad(JavaVM* vm, void* reserved) {

	MSHookFunction((void*) &GameMode::useItemOn, (void*) &useItemOn, (void**) &_useItemOn);
	MSHookFunction((void*) &MinecraftClient::init, (void*) &initClient, (void**) &_initClient);
	MSHookFunction((void*) &Item::initClientData, (void*) &initItems, (void**) &_initItems);
	MSHookFunction((void*) &Item::initCreativeItems, (void*) &initCreativeItems, (void**) &_initCreativeItems);
	return JNI_VERSION_1_2;
}
