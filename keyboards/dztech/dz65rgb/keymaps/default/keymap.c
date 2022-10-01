#include QMK_KEYBOARD_H

#define ENCODERS_PAD_A { B0 }
#define ENCODERS_PAD_B { B1 }

#define MATRIX_ENCODERS_DELAY 50

#define MATRIX_ENCODERS_COUNT 2

const uint16_t MATRIX_ENCODERS_CW_KEYCODES[MATRIX_ENCODERS_COUNT] = { KC_A, KC_F1 };
const uint16_t MATRIX_ENCODERS_CCW_KEYCODES[MATRIX_ENCODERS_COUNT] = { KC_B, KC_F2 };

//#define encoder_update_user encoder_update_user2

//#include "encoder.h"
#include "encoder.c"

enum my_keycodes {
  KC_E1M1 = SAFE_RANGE,
  KC_E1M2,
  KC_E2M1,
  KC_E2M2
};

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

static uint8_t padAStates[MATRIX_ENCODERS_COUNT];
static uint8_t padBStates[MATRIX_ENCODERS_COUNT];




/*
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  switch (keycode) {
    case KC_E1M1:
      if (record->event.pressed) {
        if (padAStates[0] == false)
        {
          if (padBStates[0])
            ;//tap_code_delay(MATRIX_ENCODERS_CW_KEYCODES[0], MATRIX_ENCODERS_DELAY); // when rotating in CCW direction, this gives no false positives
          else 
            tap_code_delay(MATRIX_ENCODERS_CCW_KEYCODES[0], MATRIX_ENCODERS_DELAY); // when rotating in CW direction, this gives a little of false positives

          padAStates[0] = true;
        }
      } else {
//        if (padAStates[0] == true)
//        {
//          if (padBStates[0])
//            ;//tap_code_delay(MATRIX_ENCODERS_CCW_KEYCODES[0], MATRIX_ENCODERS_DELAY);
//          else 
//            ;//tap_code_delay(MATRIX_ENCODERS_CW_KEYCODES[0], MATRIX_ENCODERS_DELAY);
//
//          padAStates[0] = false;
//        }
          padAStates[0] = false;
      }
      return false; 
    case KC_E1M2:
      if (record->event.pressed) {
        if (padBStates[0] == false)
        {
          if (padAStates[0])
            ;//tap_code_delay(MATRIX_ENCODERS_CW_KEYCODES[0], MATRIX_ENCODERS_DELAY); // when rotating in CCW direction, this gives no false positives
          else 
            tap_code_delay(MATRIX_ENCODERS_CW_KEYCODES[0], MATRIX_ENCODERS_DELAY); // when rotating in CW direction, this gives a little of false positives

          padBStates[0] = true;
        }
//        padBStates[0] = true;
      } else {
        padBStates[0] = false;
      }
      return false;
    default:
      return true; 
  }
}
*/
void keyboard_post_init_user(void) {
//    for (int i = 0; i < NUMBER_OF_ENCODERS; i++) {
//        encoder_state[i] = (readPin(encoders_pad_a[i]) << 0) | (readPin(encoders_pad_b[i]) << 1);
//    }
}

bool encoder_update_user2(uint8_t index, bool clockwise) {
    if (index == 0) { /* First encoder */
        if (clockwise) {
            tap_code_delay(MATRIX_ENCODERS_CW_KEYCODES[0], MATRIX_ENCODERS_DELAY); // when rotating in CW direction, this gives a little of false positives
        } else {
            tap_code_delay(MATRIX_ENCODERS_CCW_KEYCODES[0], MATRIX_ENCODERS_DELAY); // when rotating in CW direction, this gives a little of false positives
        }
    }
    return false;
}

bool encoder_update2(uint8_t index, uint8_t state) {
    bool    changed = false;
    uint8_t i       = index;

    uint8_t resolution = ENCODER_RESOLUTION;

    encoder_pulses[i] += encoder_LUT[state & 0xF];
    if (encoder_pulses[i] >= resolution) {
        encoder_value[index]++;
        changed = true;
        encoder_update_user2(index, ENCODER_COUNTER_CLOCKWISE);
    }
    if (encoder_pulses[i] <= -resolution) { // direction is arbitrary here, but this clockwise
        encoder_value[index]--;
        changed = true;
        encoder_update_user2(index, ENCODER_CLOCKWISE);
    }
    encoder_pulses[i] %= resolution;
    return changed;
}

void update_encoders(void) {
    bool changed = false;

    const int i = 0;
    //for (uint8_t i = 0; i < NUMBER_OF_ENCODERS; i++) {
        encoder_state[i] <<= 2;
        encoder_state[i] |= (padAStates[0] << 0) | (padBStates[0] << 1);
        changed |= encoder_update2(i, encoder_state[i]);

        if (changed) {

        }
    //}
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  switch (keycode) {
    case KC_E1M1:
      if (record->event.pressed) {
          padAStates[0] = 1;
      } else {
          padAStates[0] = 0;
      }

        update_encoders();

        return false; 
    case KC_E1M2:
      if (record->event.pressed) {
          padBStates[0] = 1;
      } else {
          padBStates[0] = 0;
      }

        update_encoders();

      return false;
    default:
      return true; 
  }
}
