#include <math.h>
#include "Unit.h"
#include "Main.h"

/*
 * A method that will execute on each frame. The overall game state will not change until this method
 * finish its execution.
 */
void bt_OnFrame() {
  
}

/*
 * A method that will execute on the first game state. When this method executes, all the initial units
 * are available on things like "vector<Unit> unit_Filter(uint8_t (*method)(Unit));"
 */
void bt_OnInit() {

}

/*
 * A method that will execute on the when a unit is destroyed. The responsability of having a consistent state
 * regarding the implementation of the Bot needs to be implemented by the enginer user. In other words, if you issue
 * commands to a unit that does no longer exist anymore, it will result in "undefined" behaviour.
 * This method is not called only when a unit dies, it might happen with other events.
 */
void bt_OnUnitDestroy(Unit unit) {

}

/*
 * A method that will execute when a unit is created.
 */
void bt_OnUnitCreate(Unit unit) {
  
}

/*
 * This method executes when the game loads. This method should used to free and initialize different datastructures
 * that the developer will use to code behaviour. Please note that at this state of initialization there are not units
 * initialized
 */
void bt_OnGamePrepare() {
  
}