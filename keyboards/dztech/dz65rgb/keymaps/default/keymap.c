#include QMK_KEYBOARD_H

enum my_keycodes {
  KC_E1M1 = SAFE_RANGE,
  KC_E1M2,
  KC_E2M1,
  KC_E2M2
};

#define MATRIX_ENCODERS_COUNT 2

const uint16_t MATRIX_ENCODERS_CW_KEYCODES[MATRIX_ENCODERS_COUNT] = { KC_A, KC_F1 };
const uint16_t MATRIX_ENCODERS_CCW_KEYCODES[MATRIX_ENCODERS_COUNT] = { KC_B, KC_F2 };

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [0] = LAYOUT_65_ansi(
        KC_GESC,        KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS, KC_EQL,  KC_BSPC, KC_HOME,
        KC_TAB,         KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRC, KC_RBRC, KC_BSLS, KC_PGUP,
        CTL_T(KC_CAPS), KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT,          KC_ENT,  KC_PGDN,
        KC_LSFT,                 KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_RSFT, KC_UP,   KC_END,
        KC_LCTL,        KC_LGUI, KC_LALT, KC_E1M1, KC_E1M2,          KC_SPC,                    KC_RALT, MO(1),   KC_RCTL, KC_LEFT, KC_DOWN, KC_RGHT
    ),
    [1] = LAYOUT_65_ansi(
        KC_GESC,        KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,  KC_DEL,  KC_HOME,
        _______,        RGB_TOG, RGB_MOD, RGB_HUI, RGB_HUD, RGB_SAI, RGB_SAD, RGB_VAI, RGB_VAD, _______, KC_PSCR, KC_SLCK, KC_PAUS, RESET,   KC_PGUP,
        CTL_T(KC_CAPS), RGB_SPI, RGB_SPD, _______, _______, _______, _______, _______, _______, _______, _______, _______,          EEP_RST, KC_PGDN,
        KC_LSFT,                 _______, _______, _______, _______, _______, NK_TOGG, _______, _______, _______, _______, _______, KC_VOLU, KC_MUTE,
        _______,        _______, _______, _______, _______,          _______,                   _______, _______, _______, KC_MPRV, KC_VOLD, KC_MNXT
    )
};

static bool padAStates[MATRIX_ENCODERS_COUNT];
static bool padBStates[MATRIX_ENCODERS_COUNT];

static bool awaitingCCW = false;
static bool awaitingCW = false;
static bool awaitingCCW_B = false;
static bool awaitingCW_B = false;

#define MATRIX_ENCODERS_DELAY 10

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  switch (keycode) {
    case KC_E1M1:
      if (record->event.pressed) {
        // Using A pad signal as main - keypresses will be simulated based on its fronts
        // B pad signal is auxiliary - it helps to find direction and errors
        if (padAStates[0] == false) { // A Front
          if (padBStates[0])
            { awaitingCCW = true; awaitingCW = false; } //tap_code_delay(MATRIX_ENCODERS_CW_KEYCODES[0], MATRIX_ENCODERS_DELAY); // when rotating in CCW direction, this gives no false positives
          else
            { awaitingCW = true; awaitingCCW = false; } //tap_code_delay(MATRIX_ENCODERS_CCW_KEYCODES[0], MATRIX_ENCODERS_DELAY); // when rotating in CW direction, this gives a little of false positives

          padAStates[0] = true;
        } else { // A invalid Front - Front after Front
          awaitingCCW = awaitingCW = awaitingCCW_B = awaitingCW_B = false;
        }
      } else {
        if (padAStates[0] == true) { // A Back
          if (padBStates[0]) {
            if (awaitingCW && awaitingCW_B) {
              tap_code_delay(MATRIX_ENCODERS_CW_KEYCODES[0], MATRIX_ENCODERS_DELAY);
            }
          } else {
            if (awaitingCCW && awaitingCCW_B) {
              tap_code_delay(MATRIX_ENCODERS_CCW_KEYCODES[0], MATRIX_ENCODERS_DELAY);
            }
          }
          padAStates[0] = false;

          awaitingCCW = awaitingCW = awaitingCCW_B = awaitingCW_B = false;
        } else { // A invalid Back - Back after Back
        }
        awaitingCCW = awaitingCW = awaitingCCW_B = awaitingCW_B = false;
      }
      return false; 
    case KC_E1M2:
      if (record->event.pressed) {
          if (padBStates[0] == false) { // B Front
            if (awaitingCW)
                awaitingCW_B = true;
            else
                awaitingCCW = awaitingCW = awaitingCCW_B = awaitingCW_B = false;

            padBStates[0] = true;
          }
      } else {
          if (padBStates[0] == true) { // B Back
            if (awaitingCCW)
                awaitingCCW_B = true;
            else
                awaitingCCW = awaitingCW = awaitingCCW_B = awaitingCW_B = false;

            padBStates[0] = false;
          }
      }
      return false;
    default:
      return true; 
  }
}

//bool encoder_update_user(uint8_t index, bool clockwise) {
//    if (index == 0) { /* First encoder */
//        if (clockwise) {
//            tap_code(KC_VOLU);
//        } else {
//            tap_code(KC_VOLD);
//        }
//    }
//    return false;
//}
//
