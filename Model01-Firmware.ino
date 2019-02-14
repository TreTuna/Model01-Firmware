#include "Kaleidoscope.h"

#include "Kaleidoscope-EEPROM-Settings.h"
#include "Kaleidoscope-EEPROM-Keymap.h"
#include "Kaleidoscope-FocusSerial.h"
#include "Kaleidoscope-HostPowerManagement.h"
#include "Kaleidoscope-Macros.h"
#include "Kaleidoscope-MagicCombo.h"
#include "Kaleidoscope-OneShot.h"
#include "Kaleidoscope-Escape-OneShot.h"
#include "Kaleidoscope-USB-Quirks.h"

#include "Kaleidoscope-LEDControl.h"
#include "Kaleidoscope-Colormap.h"
#include "Kaleidoscope-LEDEffect-BootGreeting.h"
#include "Kaleidoscope-LED-ActiveModColor.h"
#include "Kaleidoscope-LED-Palette-Theme.h"

enum {
  MACRO_VERSION_INFO,
  MACRO_HYPER,
  MACRO_MEHA,
  MACRO_MEHB,
  MACRO_MEHC,
  MACRO_MEHD,
  MACRO_CMD_CTRL,
  MACRO_OPTN_SHFT,
  MACRO_CTRL_SHFT,
  MACRO_FAT_ARROW
};

enum {
  PRIMARY,
  NUMPAD,
  FUNCTION
};

KEYMAPS(
  [PRIMARY] = KEYMAP_STACKED(
    ___,          Key_1, Key_2, Key_3, Key_4, Key_5, Key_LEDEffectNext,
    Key_Backtick, Key_Q, Key_W, Key_E, Key_R, Key_T, Key_LeftBracket,
    Key_Tab,      Key_A, Key_S, Key_D, Key_F, Key_G,
    Key_Escape,   Key_Z, Key_X, Key_C, Key_V, Key_B, M(MACRO_HYPER),
    OSM(LeftControl), OSM(LeftAlt), OSM(LeftGui), OSM(LeftShift),
    ShiftToLayer(FUNCTION),

    Key_Backslash,      Key_6, Key_7, Key_8,     Key_9,         Key_0,     LockLayer(NUMPAD),
    Key_RightBracket,   Key_Y, Key_U, Key_I,     Key_O,         Key_P,     Key_Equals,
    Key_H,              Key_J, Key_K, Key_L,     Key_Semicolon, Key_Quote,
    M(MACRO_FAT_ARROW), Key_N, Key_M, Key_Comma, Key_Period,    Key_Slash, Key_Minus,
    OSM(RightGui), Key_Enter, Key_Spacebar, Key_Backspace,
    ShiftToLayer(FUNCTION)
  ),

  [NUMPAD] = KEYMAP_STACKED(
    ___, ___, ___, ___, ___, ___, ___,
    ___, ___, ___, ___, ___, ___, ___,
    ___, ___, ___, ___, ___, ___,
    ___, ___, ___, ___, ___, ___, ___,
    ___, ___, ___, ___,
    ___,

    M(MACRO_VERSION_INFO),  ___,         Key_Keypad7, Key_Keypad8,   Key_Keypad9,        Key_KeypadSubtract, ___,
    ___,                    ___,         Key_Keypad4, Key_Keypad5,   Key_Keypad6,        Key_KeypadAdd,      ___,
                            ___,         Key_Keypad1, Key_Keypad2,   Key_Keypad3,        Key_Equals,         ___,
    ___,                    ___,         Key_Keypad0, Key_KeypadDot, Key_KeypadMultiply, Key_KeypadDivide,   Key_Enter,
    ___, ___, ___, ___,
    ___
  ),

  [FUNCTION] = KEYMAP_STACKED(
    ___, Key_F1,             Key_F2,            Key_F3,             Key_F4,         Key_F5, ___,
    ___, ___,                ___,               ___,                ___,            ___,    Key_LeftParen,
    ___, M(MACRO_CTRL_SHFT), M(MACRO_CMD_CTRL), M(MACRO_OPTN_SHFT), M(MACRO_HYPER), ___,
    ___, M(MACRO_MEHA),      M(MACRO_MEHB),     M(MACRO_MEHC),      M(MACRO_MEHD),  ___,    ___,
    ___, ___, ___, ___,
    ___,

    ___,                Key_F6,                     Key_F7,                   Key_F8,                   Key_F9,                 Key_F10, Key_F11,
    Key_RightParen,     Key_Home,                   Key_PageDown,             Key_PageUp,               Key_End,                ___,     Key_F12,
                        Key_LeftArrow,              Key_DownArrow,            Key_UpArrow,              Key_RightArrow,         ___,     Consumer_PlaySlashPause,
    M(MACRO_FAT_ARROW), Consumer_ScanPreviousTrack, Consumer_VolumeDecrement, Consumer_VolumeIncrement, Consumer_ScanNextTrack, ___,     Consumer_Mute,
    ___, ___, ___, Key_Delete,
    ___
  )
)

static void versionInfoMacro(uint8_t keyState) {
  if (keyToggledOn(keyState)) {
    Macros.type(PSTR("Keyboardio Model 01 - Kaleidoscope; Chrysalis-Firmware-Bundle build"));
  }
}

// Command & Control & Option & Shift
static void oneShotHyperMacro(uint8_t keyState) {
  if (
    keyToggledOn(keyState) &&
    OneShot.isActive() &&
    OneShot.isSticky(OSM(LeftShift)) &&
    OneShot.isSticky(OSM(LeftControl)) &&
    OneShot.isSticky(OSM(LeftAlt)) &&
    OneShot.isSticky(OSM(LeftGui))
  ) {
    OneShot.inject(OSM(LeftGui), IS_PRESSED);
    OneShot.inject(OSM(LeftGui), WAS_PRESSED);

    OneShot.inject(OSM(LeftAlt), IS_PRESSED);
    OneShot.inject(OSM(LeftAlt), WAS_PRESSED);

    OneShot.inject(OSM(LeftShift), IS_PRESSED);
    OneShot.inject(OSM(LeftShift), WAS_PRESSED);

    OneShot.inject(OSM(LeftControl), IS_PRESSED);
    OneShot.inject(OSM(LeftControl), WAS_PRESSED);

    return;
  }

  if (
    keyToggledOn(keyState) &&
    OneShot.isActive() &&
    OneShot.isModifierActive(Key_LeftGui) &&
    OneShot.isModifierActive(Key_LeftShift) &&
    OneShot.isModifierActive(Key_LeftAlt) &&
    OneShot.isModifierActive(Key_LeftControl)
  ) {
    OneShot.cancel();

    OneShot.inject(OSM(LeftGui), keyState);
    OneShot.inject(OSM(LeftGui), keyState);

    OneShot.inject(OSM(LeftAlt), keyState);
    OneShot.inject(OSM(LeftAlt), keyState);

    OneShot.inject(OSM(LeftShift), keyState);
    OneShot.inject(OSM(LeftShift), keyState);

    OneShot.inject(OSM(LeftControl), keyState);
    OneShot.inject(OSM(LeftControl), keyState);

    return;
  }

  if (
    keyToggledOn(keyState) &&
    OneShot.isActive()
  ) {
    OneShot.cancel();

    OneShot.inject(OSM(LeftGui), keyState);
    OneShot.inject(OSM(LeftAlt), keyState);
    OneShot.inject(OSM(LeftShift), keyState);
    OneShot.inject(OSM(LeftControl), keyState);

    return;
  }

  OneShot.inject(OSM(LeftGui), keyState);
  OneShot.inject(OSM(LeftAlt), keyState);
  OneShot.inject(OSM(LeftShift), keyState);
  OneShot.inject(OSM(LeftControl), keyState);
}

// Command & Option & Control
static void oneShotMehAMacro(uint8_t keyState) {
  if (
    keyToggledOn(keyState) &&
    OneShot.isActive() &&
    OneShot.isSticky(OSM(LeftControl)) &&
    OneShot.isSticky(OSM(LeftAlt)) &&
    OneShot.isSticky(OSM(LeftGui))
  ) {
    OneShot.inject(OSM(LeftGui), IS_PRESSED);
    OneShot.inject(OSM(LeftGui), WAS_PRESSED);

    OneShot.inject(OSM(LeftAlt), IS_PRESSED);
    OneShot.inject(OSM(LeftAlt), WAS_PRESSED);

    OneShot.inject(OSM(LeftControl), IS_PRESSED);
    OneShot.inject(OSM(LeftControl), WAS_PRESSED);

    return;
  }

  if (
    OneShot.isActive() &&
    keyToggledOn(keyState) &&
    OneShot.isModifierActive(Key_LeftGui) &&
    OneShot.isModifierActive(Key_LeftAlt) &&
    OneShot.isModifierActive(Key_LeftControl)
  ) {
    OneShot.inject(OSM(LeftGui), keyState);
    OneShot.inject(OSM(LeftGui), keyState);

    OneShot.inject(OSM(LeftAlt), keyState);
    OneShot.inject(OSM(LeftAlt), keyState);

    OneShot.inject(OSM(LeftControl), keyState);
    OneShot.inject(OSM(LeftControl), keyState);

    return;
  }

  if (
    keyToggledOn(keyState) &&
    OneShot.isActive()
  ) {
    OneShot.cancel();

    OneShot.inject(OSM(LeftGui), keyState);
    OneShot.inject(OSM(LeftAlt), keyState);
    OneShot.inject(OSM(LeftControl), keyState);

    return;
  }

  OneShot.inject(OSM(LeftGui), keyState);
  OneShot.inject(OSM(LeftAlt), keyState);
  OneShot.inject(OSM(LeftControl), keyState);
}

// Shift & Control & Option
static void oneShotMehBMacro(uint8_t keyState) {
  if (
    keyToggledOn(keyState) &&
    OneShot.isActive() &&
    OneShot.isSticky(OSM(LeftShift)) &&
    OneShot.isSticky(OSM(LeftControl)) &&
    OneShot.isSticky(OSM(LeftAlt))
  ) {
    OneShot.inject(OSM(LeftAlt), IS_PRESSED);
    OneShot.inject(OSM(LeftAlt), WAS_PRESSED);

    OneShot.inject(OSM(LeftShift), IS_PRESSED);
    OneShot.inject(OSM(LeftShift), WAS_PRESSED);

    OneShot.inject(OSM(LeftControl), IS_PRESSED);
    OneShot.inject(OSM(LeftControl), WAS_PRESSED);

    return;
  }

  if (
    OneShot.isActive() &&
    keyToggledOn(keyState) &&
    OneShot.isModifierActive(Key_LeftAlt) &&
    OneShot.isModifierActive(Key_LeftShift) &&
    OneShot.isModifierActive(Key_LeftControl)
  ) {
    OneShot.inject(OSM(LeftAlt), keyState);
    OneShot.inject(OSM(LeftAlt), keyState);

    OneShot.inject(OSM(LeftShift), keyState);
    OneShot.inject(OSM(LeftShift), keyState);

    OneShot.inject(OSM(LeftControl), keyState);
    OneShot.inject(OSM(LeftControl), keyState);

    return;
  }

if (
    keyToggledOn(keyState) &&
    OneShot.isActive()
  ) {
    OneShot.cancel();

    OneShot.inject(OSM(LeftAlt), keyState);
    OneShot.inject(OSM(LeftShift), keyState);
    OneShot.inject(OSM(LeftControl), keyState);

    return;
  }

  OneShot.inject(OSM(LeftAlt), keyState);
  OneShot.inject(OSM(LeftShift), keyState);
  OneShot.inject(OSM(LeftControl), keyState);
}

// Command & Shift & Control
static void oneShotMehCMacro(uint8_t keyState) {
  if (
    keyToggledOn(keyState) &&
    OneShot.isActive() &&
    OneShot.isSticky(OSM(LeftShift)) &&
    OneShot.isSticky(OSM(LeftControl)) &&
    OneShot.isSticky(OSM(LeftGui))
  ) {
    OneShot.inject(OSM(LeftGui), IS_PRESSED);
    OneShot.inject(OSM(LeftGui), WAS_PRESSED);

    OneShot.inject(OSM(LeftShift), IS_PRESSED);
    OneShot.inject(OSM(LeftShift), WAS_PRESSED);

    OneShot.inject(OSM(LeftControl), IS_PRESSED);
    OneShot.inject(OSM(LeftControl), WAS_PRESSED);

    return;
  }

  if (
    keyToggledOn(keyState) &&
    OneShot.isActive() &&
    OneShot.isModifierActive(Key_LeftGui) &&
    OneShot.isModifierActive(Key_LeftShift) &&
    OneShot.isModifierActive(Key_LeftControl)
  ) {
    OneShot.cancel();

    OneShot.inject(OSM(LeftGui), keyState);
    OneShot.inject(OSM(LeftGui), keyState);

    OneShot.inject(OSM(LeftShift), keyState);
    OneShot.inject(OSM(LeftShift), keyState);

    OneShot.inject(OSM(LeftControl), keyState);
    OneShot.inject(OSM(LeftControl), keyState);

    return;
  }

  if (
    keyToggledOn(keyState) &&
    OneShot.isActive()
  ) {
    OneShot.cancel();

    OneShot.inject(OSM(LeftGui), keyState);
    OneShot.inject(OSM(LeftShift), keyState);
    OneShot.inject(OSM(LeftControl), keyState);

    return;
  }

  OneShot.inject(OSM(LeftGui), keyState);
  OneShot.inject(OSM(LeftShift), keyState);
  OneShot.inject(OSM(LeftControl), keyState);
}

// Command & Shift & Option
static void oneShotMehDMacro(uint8_t keyState) {
  if (
    keyToggledOn(keyState) &&
    OneShot.isActive() &&
    OneShot.isSticky(OSM(LeftShift)) &&
    OneShot.isSticky(OSM(LeftAlt)) &&
    OneShot.isSticky(OSM(LeftGui))
  ) {
    OneShot.inject(OSM(LeftGui), IS_PRESSED);
    OneShot.inject(OSM(LeftGui), WAS_PRESSED);

    OneShot.inject(OSM(LeftAlt), IS_PRESSED);
    OneShot.inject(OSM(LeftAlt), WAS_PRESSED);

    OneShot.inject(OSM(LeftShift), IS_PRESSED);
    OneShot.inject(OSM(LeftShift), WAS_PRESSED);

    return;
  }

  if (
    OneShot.isActive() &&
    keyToggledOn(keyState) &&
    OneShot.isModifierActive(Key_LeftGui) &&
    OneShot.isModifierActive(Key_LeftShift) &&
    OneShot.isModifierActive(Key_LeftAlt)
  ) {
    OneShot.inject(OSM(LeftGui), keyState);
    OneShot.inject(OSM(LeftGui), keyState);

    OneShot.inject(OSM(LeftAlt), keyState);
    OneShot.inject(OSM(LeftAlt), keyState);

    OneShot.inject(OSM(LeftShift), keyState);
    OneShot.inject(OSM(LeftShift), keyState);

    return;
  }

  if (
    keyToggledOn(keyState) &&
    OneShot.isActive()
  ) {
    OneShot.cancel();

    OneShot.inject(OSM(LeftGui), keyState);
    OneShot.inject(OSM(LeftShift), keyState);
    OneShot.inject(OSM(LeftAlt), keyState);

    return;
  }

  OneShot.inject(OSM(LeftGui), keyState);
  OneShot.inject(OSM(LeftAlt), keyState);
  OneShot.inject(OSM(LeftShift), keyState);
}

static void oneShotCmdCtrlMacro(uint8_t keyState) {
  if (
    keyToggledOn(keyState) &&
    OneShot.isActive() &&
    OneShot.isSticky(OSM(LeftControl)) &&
    OneShot.isSticky(OSM(LeftGui))
  ) {
    OneShot.inject(OSM(LeftGui), IS_PRESSED);
    OneShot.inject(OSM(LeftGui), WAS_PRESSED);

    OneShot.inject(OSM(LeftControl), IS_PRESSED);
    OneShot.inject(OSM(LeftControl), WAS_PRESSED);

    return;
  }

  if (
    OneShot.isActive() &&
    keyToggledOn(keyState) &&
    OneShot.isModifierActive(Key_LeftGui) &&
    OneShot.isModifierActive(Key_LeftControl)
  ) {
    OneShot.inject(OSM(LeftGui), keyState);
    OneShot.inject(OSM(LeftGui), keyState);

    OneShot.inject(OSM(LeftControl), keyState);
    OneShot.inject(OSM(LeftControl), keyState);

    return;
  }

  if (
    keyToggledOn(keyState) &&
    OneShot.isActive()
  ) {
    OneShot.cancel();

    OneShot.inject(OSM(LeftGui), keyState);
    OneShot.inject(OSM(LeftControl), keyState);

    return;
  }

  OneShot.inject(OSM(LeftGui), keyState);
  OneShot.inject(OSM(LeftControl), keyState);
}

static void oneShotOptnShftMacro(uint8_t keyState) {
  if (
    keyToggledOn(keyState) &&
    OneShot.isActive() &&
    OneShot.isSticky(OSM(LeftShift)) &&
    OneShot.isSticky(OSM(LeftAlt))
    ) {
    OneShot.inject(OSM(LeftAlt), IS_PRESSED);
    OneShot.inject(OSM(LeftAlt), WAS_PRESSED);

    OneShot.inject(OSM(LeftShift), IS_PRESSED);
    OneShot.inject(OSM(LeftShift), WAS_PRESSED);

    return;
  }

  if (
    OneShot.isActive() &&
    keyToggledOn(keyState) &&
    OneShot.isModifierActive(Key_LeftAlt) &&
    OneShot.isModifierActive(Key_LeftShift)
  ) {
    OneShot.inject(OSM(LeftAlt), keyState);
    OneShot.inject(OSM(LeftAlt), keyState);

    OneShot.inject(OSM(LeftShift), keyState);
    OneShot.inject(OSM(LeftShift), keyState);

    return;
  }

  if (
    keyToggledOn(keyState) &&
    OneShot.isActive()
  ) {
    OneShot.cancel();

    OneShot.inject(OSM(LeftAlt), keyState);
    OneShot.inject(OSM(LeftShift), keyState);

    return;
  }

  OneShot.inject(OSM(LeftAlt), keyState);
  OneShot.inject(OSM(LeftShift), keyState);
}

static void oneShotCtrlShftMacro(uint8_t keyState) {
  if (
    keyToggledOn(keyState) &&
    OneShot.isActive() &&
    OneShot.isSticky(OSM(LeftShift)) &&
    OneShot.isSticky(OSM(LeftControl))
  ) {
    OneShot.inject(OSM(LeftControl), IS_PRESSED);
    OneShot.inject(OSM(LeftControl), WAS_PRESSED);

    OneShot.inject(OSM(LeftShift), IS_PRESSED);
    OneShot.inject(OSM(LeftShift), WAS_PRESSED);

    return;
  }

  if (
    OneShot.isActive() &&
    keyToggledOn(keyState) &&
    OneShot.isModifierActive(Key_LeftShift) &&
    OneShot.isModifierActive(Key_LeftControl)
  ) {
    OneShot.inject(OSM(LeftControl), keyState);
    OneShot.inject(OSM(LeftControl), keyState);

    OneShot.inject(OSM(LeftShift), keyState);
    OneShot.inject(OSM(LeftShift), keyState);

    return;
  }

  if (
    keyToggledOn(keyState) &&
    OneShot.isActive()
  ) {
    OneShot.cancel();

    OneShot.inject(OSM(LeftShift), keyState);
    OneShot.inject(OSM(LeftControl), keyState);

    return;
  }

  OneShot.inject(OSM(LeftControl), keyState);
  OneShot.inject(OSM(LeftShift), keyState);
}

const macro_t *macroAction(uint8_t macroIndex, uint8_t keyState) {
  switch (macroIndex) {
    case MACRO_FAT_ARROW:
      if (keyToggledOn(keyState))
      {
        return Macros.type(PSTR("=>"));
      }
      break;

    case MACRO_HYPER:
      oneShotHyperMacro(keyState);
      break;

    case MACRO_MEHA:
      oneShotMehAMacro(keyState);
      break;

    case MACRO_MEHB:
      oneShotMehBMacro(keyState);
      break;

    case MACRO_MEHC:
      oneShotMehCMacro(keyState);
      break;

    case MACRO_MEHD:
      oneShotMehDMacro(keyState);
      break;

    case MACRO_CMD_CTRL:
      oneShotCmdCtrlMacro(keyState);
      break;

    case MACRO_OPTN_SHFT:
      oneShotOptnShftMacro(keyState);
      break;

    case MACRO_CTRL_SHFT:
      oneShotCtrlShftMacro(keyState);
      break;

    case MACRO_VERSION_INFO:
      versionInfoMacro(keyState);
      break;

      return MACRO_NONE;
  }
}

void toggleLedsOnSuspendResume(kaleidoscope::plugin::HostPowerManagement::Event event) {
  switch (event) {
  case kaleidoscope::plugin::HostPowerManagement::Suspend:
    LEDControl.set_all_leds_to({0, 0, 0});
    LEDControl.syncLeds();
    LEDControl.paused = true;
    break;
  case kaleidoscope::plugin::HostPowerManagement::Resume:
    LEDControl.paused = false;
    LEDControl.refreshAll();
    break;
  case kaleidoscope::plugin::HostPowerManagement::Sleep:
    break;
  }
}

void hostPowerManagementEventHandler(kaleidoscope::plugin::HostPowerManagement::Event event) {
  toggleLedsOnSuspendResume(event);
}

enum {
  COMBO_TOGGLE_NKRO_MODE
};

static void toggleKeyboardProtocol(uint8_t combo_index) {
  USBQuirks.toggleKeyboardProtocol();
}

USE_MAGIC_COMBOS({.action = toggleKeyboardProtocol,
                  // Left Fn + Esc + Shift
                  .keys = { R3C6, R2C6, R3C7 }
                });

KALEIDOSCOPE_INIT_PLUGINS(
  EEPROMSettings,
  EEPROMKeymap,
  Focus,
  FocusSettingsCommand,
  FocusEEPROMCommand,
  HostPowerManagement,
  USBQuirks,

  LEDControl,
  ActiveModColorEffect,

  Macros,
  OneShot,
  EscapeOneShot,
  MagicCombo,

  BootGreetingEffect,
  LEDOff,
  LEDPaletteTheme,
  ColormapEffect
);

void setup() {
  Kaleidoscope.setup();

  LEDOff.activate();
  EEPROMKeymap.setup(3);
  EEPROMSettings.default_layer(3);
  ColormapEffect.max_layers(3 + 5);
  ActiveModColorEffect.highlight_color = CRGB(0x00, 0xff, 0x88);
}

void loop() {
  Kaleidoscope.loop();
}
