#include "fissure.h"
#include "action_layer.h"
#include "eeconfig.h"
#include "keymap_steno.h"

#ifndef __AVR__
#include "app_ble_func.h"
#endif

// Layers
#define _BASE   0
#define _LOWER 1
#define _RAISE 2
#define _STENO 3

#define KEYMAP( \
    L40, L10, L11, L12, L13, L14,                 R10, R11, R12, R13, R14, R44, \
    L41, L20, L21, L22, L23, L24,                 R20, R21, R22, R23, R24, R43, \
         L30, L31, L32, L33, L34,                 R30, R31, R32, R33, R34, \
                        L42, L43, L44,            R40, R41, R42 \
    ) \
    { \
        { L10, L11, L12, L13, L14 }, \
        { L20, L21, L22, L23, L24 }, \
        { L30, L31, L32, L33, L34 }, \
        { L40, L41, L42, L43, L44 }, \
        { R14, R13, R12, R11, R10 }, \
        { R24, R23, R22, R21, R20 }, \
        { R34, R33, R32, R31, R30 }, \
        { R44, R43, R42, R41, R40 } \
    }

void set_hsv(uint8_t a, uint8_t b, uint8_t c) {
    rgblight_sethsv(a, b, (uint8_t) c * 0.15);
}

void keyboard_pre_init_user(void) {
    set_hsv(HSV_TEAL);
#ifndef __AVR__
    set_usb_enabled(true);
    debug_matrix=true;
#endif
}

void suspend_power_down_user() {
    set_hsv(0, 0, 0);
}

void suspend_wakeup_init_user() {
    set_hsv(0, 0, 0);
}

uint32_t layer_state_set_user(uint32_t state) {
    switch (biton32(state)) {
    case _LOWER: set_hsv(HSV_GREEN); break;
    case _RAISE: set_hsv(HSV_CYAN); break;
    case _STENO: set_hsv(HSV_ORANGE); break;
    default: set_hsv(0, 0, 0); break;
    }
    return state;
}
#ifdef __AVR__
#else
// Aliases
#define C(kc) LCTL(kc)
#define S(kc) LSFT(kc)
#define A(kc) LALT(kc)
#define G(kc) LGUI(kc)

#endif

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    // If console is enabled, it will print the matrix position and status of each key pressed
#ifdef CONSOLE_ENABLE
    /* uprintf("KL: kc: %u, col: %u, row: %u, pressed: %u\n", keycode, record->event.key.col, record->event.key.row, record->event.pressed); */
    // For keylogging
    uprintf("[kl] %u:%u %c\n", record->event.key.row, record->event.key.col, record->event.pressed ? 'p' : 'P');
#endif 
    return true;
}

#define DEL MT(MOD_LGUI, KC_DEL)
#define TAB MT(MOD_LCTL, KC_TAB)
#define ESC LT(_RAISE, KC_ESC)
#define ENT LT(_LOWER, KC_ENT)
#define SPC MT(MOD_LSFT, KC_SPC)
#define BKSP MT(MOD_LALT, KC_BSPC)

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [_BASE] = KEYMAP(
        XXXXXXX, KC_Q,     KC_W,     KC_E,     KC_R,     KC_T,      KC_Y,     KC_U,        KC_I,       KC_O,        KC_P, XXXXXXX,
        XXXXXXX, KC_A,     KC_S,     KC_D,     KC_F,     KC_G,      KC_H,     KC_J,        KC_K,       KC_L,     KC_SCLN, XXXXXXX,
                 KC_Z,     KC_X,     KC_C,     KC_V,     KC_B,      KC_N,     KC_M,     KC_COMM,     KC_DOT,     KC_SLSH,
                  DEL,      TAB,      ESC,      ENT,      SPC,     BKSP),

    /* [_BASE] = KEYMAP( */
    /*     KC_Q,  KC_W,  KC_F,  KC_P,  KC_B,   KC_J,  KC_L,     KC_U,    KC_Y,  KC_SCLN, */
    /*     KC_A,  KC_R,  KC_S,  KC_T,  KC_G,   KC_M,  KC_N,     KC_E,    KC_I,     KC_O, */
    /*     KC_Z,  KC_X,  KC_C,  KC_D,  KC_V,   KC_K,  KC_H,  KC_COMM,  KC_DOT,  KC_SLSH, */
    /*      DEL,   TAB,   ESC,   ENT,   SPC,  BKSP), */

    [_LOWER] = KEYMAP(
        XXXXXXX, G(KC_1),  G(KC_2),  G(KC_3),  G(KC_4),  G(KC_5),   G(KC_6),  G(KC_7),  G(KC_8),  KC_F11,  KC_F12, XXXXXXX,
        XXXXXXX,    KC_1,     KC_2,     KC_3,     KC_4,     KC_5,      KC_6,     KC_7,     KC_8,    KC_9,    KC_0, XXXXXXX,
                   KC_F1,    KC_F2,    KC_F3,    KC_F4,    KC_F5,     KC_F6,    KC_F7,    KC_F8,   KC_F9,  KC_F10,
                 _______,  _______,  _______,  _______,  _______,  _______),

    [_RAISE] = KEYMAP(
        XXXXXXX, KC_BRID,  KC_MUTE,  KC_VOLD,  KC_VOLU,  KC_BRIU,     KC_HOME,     KC_PGDN,   KC_PGUP,       KC_END,  KC_PSCR, XXXXXXX,
        XXXXXXX, XXXXXXX,  TG(_STENO),  XXXXXXX,  XXXXXXX,  XXXXXXX,     KC_LEFT,     KC_DOWN,     KC_UP,     KC_RIGHT,  XXXXXXX, XXXXXXX,
                 XXXXXXX,  KC_MPRV,  KC_MPLY,  KC_MNXT,  XXXXXXX,  C(KC_LEFT),  C(KC_DOWN),  C(KC_UP),  C(KC_RIGHT),  XXXXXXX,
                 _______,  _______,  _______,  _______,  _______,    _______),

    [_STENO] = KEYMAP(
		XXXXXXX,  STN_S1,  STN_TL,  STN_PL,  STN_HL,  STN_ST1,  STN_ST3,  STN_FR,  STN_PR,  STN_LR,  STN_TR,  STN_DR,
		XXXXXXX,  STN_S2,  STN_KL,  STN_WL,  STN_RL,  STN_ST2,  STN_ST4,  STN_RR,  STN_BR,  STN_GR,  STN_SR,  STN_ZR,
                  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, 
              TG(_STENO),  STN_A,   STN_O,   STN_E,   STN_U,  STN_N2),
};

const uint16_t PROGMEM combos[COMBO_COUNT][3] = {
#define K(r, c) (keymaps[0][r][c])
    {K(0, 1), K(0, 2), COMBO_END},
    {K(0, 2), K(0, 3), COMBO_END},
    {K(1, 0), K(1, 1), COMBO_END},
    {K(1, 1), K(1, 2), COMBO_END},
    {K(1, 2), K(1, 3), COMBO_END},
    {K(2, 0), K(2, 1), COMBO_END},
    {K(2, 1), K(2, 2), COMBO_END},
    {K(2, 2), K(2, 3), COMBO_END},
    {K(0, 1), K(1, 1), COMBO_END},
    {K(1, 1), K(2, 1), COMBO_END},
    {K(0, 2), K(1, 2), COMBO_END},
    {K(1, 2), K(2, 2), COMBO_END},
    {K(0, 3), K(1, 3), COMBO_END},
    {K(1, 3), K(2, 3), COMBO_END},
    {K(4, 3), K(4, 2), COMBO_END},
    {K(4, 2), K(4, 1), COMBO_END},
    {K(5, 3), K(5, 2), COMBO_END},
    {K(5, 2), K(5, 1), COMBO_END},
    {K(5, 1), K(5, 0), COMBO_END},
    {K(6, 3), K(6, 2), COMBO_END},
    {K(6, 2), K(6, 1), COMBO_END},
    {K(4, 3), K(5, 3), COMBO_END},
    {K(5, 3), K(6, 3), COMBO_END},
    {K(4, 2), K(5, 2), COMBO_END},
    {K(5, 2), K(6, 2), COMBO_END},
    {K(4, 1), K(5, 1), COMBO_END},
    {K(5, 1), K(6, 1), COMBO_END},
    {K(6, 1), K(6, 0), COMBO_END},
#undef K
};

combo_t key_combos[COMBO_COUNT] = {
    COMBO(combos[0], KC_BSLS),
    COMBO(combos[1], KC_LBRC),
    COMBO(combos[2], KC_CIRC),
    COMBO(combos[3], KC_DLR),
    COMBO(combos[4], KC_LPRN),
    COMBO(combos[5], KC_EXLM),
    COMBO(combos[6], KC_AT),
    COMBO(combos[7], KC_LCBR),
    COMBO(combos[8], KC_GRV),
    COMBO(combos[9], KC_TILD),
    COMBO(combos[10], KC_AMPR),
    COMBO(combos[11], KC_PIPE),
    COMBO(combos[12], KC_ASTR),
    COMBO(combos[13], KC_HASH),
    COMBO(combos[14], KC_RBRC),
    COMBO(combos[15], KC_PLUS),
    COMBO(combos[16], KC_RPRN),
    COMBO(combos[17], KC_EQL),
    COMBO(combos[18], KC_COLN),
    COMBO(combos[19], KC_RCBR),
    COMBO(combos[20], KC_PERC),
    COMBO(combos[21], KC_UNDS),
    COMBO(combos[22], KC_MINS),
    COMBO(combos[23], KC_DQUO),
    COMBO(combos[24], KC_LT),
    COMBO(combos[25], KC_QUOT),
    COMBO(combos[26], KC_GT),
    COMBO(combos[27], KC_QUES),
};
