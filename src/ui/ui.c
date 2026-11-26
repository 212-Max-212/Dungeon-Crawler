#include <stdio.h>
#include <ctype.h>
#include <conio.h>
#include <windows.h>
#include "ui.h"
#include "../player/player.h"
#include "../save/save.h"

// Quality of Life - colors, HUD, controls without Enter
void ui_init(void) {
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);                   // getting "pointer" on console
    DWORD mode = 0;                                               // console mode (unsigned long)
    GetConsoleMode(h, &mode);                                     // get console mode and write to var mode
    SetConsoleMode(h, mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING); // set mode with ANSI sequence
}

// ANSI escape sequences to clear the terminal screen
void ui_clear_screen(void) {
    printf("\x1b[2J\x1b[H");
}

void ui_draw_hud(const Player *p) {
    
    // stats
    printf("\n\x1b[32mHP %d/%d\x1b[0m | \x1b[36mMP %d/%d\x1b[0m | LVL %d | ",
        p->hp,
        p->maxHp,
        p->mana,
        p->maxMana,
        p->level
    );
    
    if (p->level < MAX_LEVEL) printf("XP %d/%d", p->xp, player_xp_needed(p));
    else printf("XP MAX");
    
    printf(" | Attack %d Defence %d\n", p->atk, p->def);

    // keys
    printf("WASD / sipky = pohyb | I = inventar | K = ulozit | L = nacitat | Q = koniec\n");
}

void start_plot(void) {
    ui_clear_screen();
    printf("\x1b[91m=== PRIBEH ===\x1b[0m\n\n");
    printf("Kedysi sa pod starym kralovstvom usadil drak, ktory zacal nicit okolite dediny a spalovat krajinu.\n");
    printf("Kralovi vojaci sa ho pokusili porazit, ale nikto sa uz nevratil.\n");
    printf("Ste dobrodruh, ktory sa rozhodne vstupit do podzemia a porazit draka.\n");
    printf("Po prejdeni podzemia najdete dracie brloh, kde vas caka posledny a najtazsi suboj.\n");
    printf("Ak draka porazite, zlomite jeho kliatbu a zachranite kralovstvo.\n");
    
    ui_pause();
}

// The player reached the exit (it is guarded by the boss): show the victory and finish the game
void finish_game(const Player *p) {
    ui_clear_screen();
    
    printf("\x1b[92m=== VITAZSTVO ===\x1b[0m\n\n");
    printf("Drak padol a brana sa otvara. Si konecne na slobode.\n");
    printf("Presiel si podzemie na urovni %d. Gratulujem, hrdina!\n", p->level);
    
    ui_pause();

    delete_save(); // the game is finished, the old save is no longer needed
}

void ui_pause(void) {
    printf("\nStlac lubovolnu klavesu...");
    _getch();
}


// Reads one key without Enter. Arrows are converted to WASD, other special keys return 1.
int ui_get_key(void) {
    int c = _getch();

    // arrows come as two bytes: a prefix (0 or 224) and then the arrow code
    if (c == 0 || c == 224) {
        switch (_getch()) {
            case 72: return 'w';
            case 80: return 's';
            case 75: return 'a';
            case 77: return 'd';
            default: return 1;
        }
    }

    return tolower(c);
}

// Waits for a digit 1..max, other keys are ignored
// Returns -1 when the player presses Q (only if allow_cancel is true)
int ui_choose(int max, bool allow_cancel) {
    printf("\nVyber: ");
    fflush(stdout);

    for (;;) {
        int key = ui_get_key();

        if (key >= '1' && key <= '0' + max) return key - '0';
        if (key == 'q' && allow_cancel) return -1;
    }
}

// W/S moves the selection in a list with `count` items, returns the new index
int ui_move_selection(int key, int selected, int count) {
    if (key == 'w' && selected > 0) return selected - 1;
    if (key == 's' && selected < count - 1) return selected + 1;

    return selected;
}

// Asks a yes/no question, only Y means yes
bool ui_confirm(const char *question) {
    printf("\n%s [y/n]\n", question);
    return ui_get_key() == 'y';
}
