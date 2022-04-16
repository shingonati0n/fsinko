#ifdef OLED_ENABLE

char mod_str[12];
static uint32_t oled_timer = 0;

oled_rotation_t oled_init_user(oled_rotation_t rotation) {
    return OLED_ROTATION_270;  // Vertical Orientation for OLED
}

void oled_render_separator(void) {
    oled_write_ln("_____", false);
}

void oled_render_space(void) {
    oled_write_ln("     ", false);
}

//#define L_BASE 0
//#define L_LOWER 2
//#define L_RAISE 4
//#define L_ADJUST 8

void oled_render_layer_state(void) {
    oled_write_P(PSTR("Layer: "), false);
    switch (layer_state) {
        case _QWERTY:
            oled_write(PSTR("QWR"), false);
            break;
        case _LOWER:
            oled_write(PSTR("LWR"), false);
            break;
        case _RAISE:
            oled_write(PSTR("RSE"), false);
            break;
        case _ADJUST:
        case _ADJUST|_LOWER:
        case _ADJUST|_RAISE:
        case _ADJUST|_LOWER|_RAISE:
            oled_write_ln_P(PSTR("ADJ"), false);
            break;
    }
}


char keylog_str[24] = {};

const char code_to_name[60] = {
    ' ', ' ', ' ', ' ', 'a', 'b', 'c', 'd', 'e', 'f',
    'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p',
    'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
    '1', '2', '3', '4', '5', '6', '7', '8', '9', '0',
    'R', 'E', 'B', 'T', '_', '-', '=', '[', ']', '\\',
    '#', ';', '\'', '`', ',', '.', '/', ' ', ' ', ' '};

void set_keylog(uint16_t keycode, keyrecord_t *record) {
  char name = ' ';
    if ((keycode >= QK_MOD_TAP && keycode <= QK_MOD_TAP_MAX) ||
        (keycode >= QK_LAYER_TAP && keycode <= QK_LAYER_TAP_MAX)) { keycode = keycode & 0xFF; }
  if (keycode < 60) {
    name = code_to_name[keycode];
  }

  // update keylog
  snprintf(keylog_str, sizeof(keylog_str), "[MAT] %dx%d [KYC] %03d ",
           record->event.key.row, record->event.key.col,
           keycode, name);
}

void oled_render_keylog(void) {
    if (strlen(keylog_str) == 0) {
        snprintf(keylog_str, sizeof(keylog_str), "[MAT] 0x0 [KYC] 000 ");
    }
    oled_write(keylog_str, false);
}

void render_bootmagic_status(bool status) {
    /* Show Ctrl-Gui Swap options */
    static const char PROGMEM logo[][2][3] = {
        {{0x97, 0x98, 0}, {0xb7, 0xb8, 0}},
        {{0x95, 0x96, 0}, {0xb5, 0xb6, 0}},
    };
    if (status) {
        oled_write_ln_P(logo[0][0], false);
        oled_write_ln_P(logo[0][1], false);
    } else {
        oled_write_ln_P(logo[1][0], false);
        oled_write_ln_P(logo[1][1], false);
    }
}

void oled_render_keymods(uint8_t led_usb_state){
    sprintf(mod_str, "num %scap %s",
        led_usb_state & (1 << USB_LED_NUM_LOCK) ? "+" : "-",
        led_usb_state & (1 << USB_LED_CAPS_LOCK) ? "+" : "-",
    );
    oled_write(mod_str, false);
}

void oled_render_logo(void) {
    static const char PROGMEM crkbd_logo[16] = {
        0x81, 0x82, 0x83, 0x84, 0x85,
        0xa1, 0xa2, 0xa3, 0xa4, 0xa5,
        0xc1, 0xc2, 0xc3, 0xc4, 0xc5,
    0};
    oled_write_P(crkbd_logo, false);
}

bool oled_task_user(void) {
    if (is_keyboard_master()) {
        oled_render_layer_state();
        oled_render_keylog();
    } else {
        oled_render_logo();
        oled_write_ln("", false);
        oled_write_ln("F5   ", false);
        oled_write_ln("Crkbd", false);
        oled_write_ln("FirmW", false);
    }
    return false;
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  if (record->event.pressed) {
    set_keylog(keycode, record);
  }
  return true;
}
#endif // OLED_ENABLE
