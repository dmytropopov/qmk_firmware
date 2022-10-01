#include QMK_KEYBOARD_H
#define _LAYER0 0
#define _LAYER1 1

enum my_keycodes {
  KC_E1M1 = SAFE_RANGE,
  KC_E1M2,
  KC_E2M1,
  KC_E2M2
};

#define MATRIX_ENCODERS_COUNT 2

const uint16_t MATRIX_ENCODERS_CW_KEYCODES[MATRIX_ENCODERS_COUNT] = { KC_UP, KC_F1 };
const uint16_t MATRIX_ENCODERS_CCW_KEYCODES[MATRIX_ENCODERS_COUNT] = { KC_DOWN, KC_F2 };

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
		[_LAYER0] = LAYOUT_65_ansi_blocker( /* Base */
			KC_GESC,       KC_1,     KC_2,     KC_3,  KC_4,  KC_5,  KC_6,    KC_7,  KC_8,    KC_9,     KC_0,     KC_MINS,  KC_EQL,  KC_BSPC,   KC_HOME,
		    KC_TAB,        KC_Q,     KC_W,     KC_E,  KC_R,  KC_T,  KC_Y,    KC_U,  KC_I,    KC_O,     KC_P,     KC_LBRC,  KC_RBRC, KC_BSLASH, KC_PGUP,
			CTL_T(KC_CAPS),KC_A,     KC_S,     KC_D,  KC_F,  KC_G,  KC_H,    KC_J,  KC_K,    KC_L,     KC_SCLN,  KC_QUOT,  KC_E1M1, KC_ENT,    KC_PGDN,
		    KC_LSFT,       KC_Z,     KC_X,     KC_C,  KC_V,  KC_B,  KC_N,    KC_M,  KC_COMM, KC_DOT,   KC_SLSH,  KC_RSFT,  KC_E1M2, KC_UP,     KC_END,
		    KC_LCTL,       KC_LGUI,  KC_LALT,                KC_SPC,                KC_RALT,           MO(1),    KC_LEFT,           KC_DOWN,   KC_RIGHT),
		[_LAYER1] = LAYOUT_65_ansi_blocker( /* FN */
			KC_GESC,       KC_F1,    KC_F2,   KC_F3,  KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,   KC_F11,  KC_F12,  KC_DEL,   KC_HOME,
		    KC_TRNS,       RGB_TOG,  RGB_MOD, RGB_HUI,RGB_HUD, RGB_SAI, RGB_SAD, RGB_VAI, RGB_VAD, KC_TRNS, KC_PSCR, KC_SLCK,  KC_PAUS, RESET,    KC_PGUP,
			CTL_T(KC_CAPS),RGB_SPI,  RGB_SPD, KC_TRNS,KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,  KC_TRNS, KC_TRNS, EEP_RST,  KC_PGDN,
		    KC_LSFT,       KC_TRNS,  KC_TRNS, KC_TRNS,KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,  KC_TRNS, KC_TRNS, KC_VOLU,  KC_MUTE,
		    KC_TRNS,       KC_TRNS,  KC_TRNS,                  KC_TRNS,                   KC_TRNS,          KC_TRNS,  KC_MPRV,          KC_VOLD,  KC_MNXT),
};

static bool padAStates[MATRIX_ENCODERS_COUNT];
static bool padBStates[MATRIX_ENCODERS_COUNT];

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  switch (keycode) {
    case KC_E1M1:
      if (record->event.pressed) {
        if (padAStates[0] == false)
        {
          if (padBStates[0])
            tap_code(MATRIX_ENCODERS_CW_KEYCODES[0]);
          else 
            tap_code(MATRIX_ENCODERS_CCW_KEYCODES[0]);

          padAStates[0] = true;
        }
      } else {
        if (padAStates[0] == true)
        {
          if (padBStates[0])
            tap_code(MATRIX_ENCODERS_CCW_KEYCODES[0]);
          else 
            tap_code(MATRIX_ENCODERS_CW_KEYCODES[0]);

          padAStates[0] = false;
        }
      }
      return false; 
    case KC_E1M2:
      if (record->event.pressed) {
          padBStates[0] = true;
      } else {
          padBStates[0] = false;
      }
      return false;
    default:
      return true; 
  }
} 
