#include <stdio.h>
#include <stdbool.h>
#include "../save/save.h"
#include "../map/map.h"

#define SAVE_FILE "save.dat"


// Save/Load - the player and the whole world go to save.dat, saves the player and the whole world
int save_game(const Player *p) {
    // open file for write data
    FILE *f = fopen(SAVE_FILE, "wb");   
    if (!f) return 0;                   // check if file was open

    MapState state;                     // all locations + the current location
    map_get_state(&state);

    int opr_id = 1;                     // identifier of operation success
    
    // save player data and world
    opr_id = opr_id && fwrite(p, sizeof(Player), 1, f) == 1;
    opr_id = opr_id && fwrite(&state, sizeof(MapState), 1, f) == 1;

    fclose(f);
    return opr_id;
}

bool load_game(Player *p) {
    // open file for read data
    FILE *f = fopen(SAVE_FILE, "rb");
    if (!f) return false;                   // check if file was open

    Player tmpP;
    MapState state;

    int opr_id = 1;

    // read the saved data and check if the file is valid
    opr_id = opr_id && fread(&tmpP, sizeof(Player), 1, f) == 1;
    opr_id = opr_id && fread(&state, sizeof(MapState), 1, f) == 1;
    
    fclose(f);

    // if loading fails, the current game stays unchanged
    if (!opr_id) return false;

    // apply the loaded data only after all checks pass
    *p = tmpP;
    map_set_state(&state);
    return true;
}


// remove the save file (called when the game is finished)
void delete_save(void) {
    remove(SAVE_FILE); // does nothing if the file does not exist
}
