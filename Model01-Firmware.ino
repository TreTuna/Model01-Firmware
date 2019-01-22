#ifndef BUILD_INFORMATION
#define BUILD_INFORMATION "locally built"
#endif

#include "Kaleidoscope.h"
#include "Kaleidoscope-EEPROM-Settings.h"
#include "Kaleidoscope-EEPROM-Keymap.h"
#include "Kaleidoscope-FocusSerial.h"
#include "Kaleidoscope-Macros.h"
#include "Kaleidoscope-LEDControl.h"
#include "Kaleidoscope-NumPad.h"
#include "Kaleidoscope-LEDEffect-BootGreeting.h"
#include "Kaleidoscope-LEDEffect-SolidColor.h"
#include "Kaleidoscope-Model01-TestMode.h"
#include "Kaleidoscope-HostPowerManagement.h"
#include "Kaleidoscope-MagicCombo.h"
#include "Kaleidoscope-USB-Quirks.h"
#include "Kaleidoscope-OneShot.h"
#include "Kaleidoscope-Escape-OneShot.h"
#include "Kaleidoscope-LED-ActiveModColor.h"
#include "Kaleidoscope-ShapeShifter.h"
#include "Kaleidoscope-TopsyTurvy.h"
#include "Kaleidoscope-TapDance.h"
#include "Kaleidoscope-Colormap.h"
#include "Kaleidoscope-LED-Palette-Theme.h"

enum {
  MACRO_VERSION_INFO,
  MACRO_ANY,
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

#define PRIMARY_KEYMAP

KEYMAPS(

  #if defined(PRIMARY_KEYMAP)
    [PRIMARY] = KEYMAP_STACKED(
      ___,          Key_1, Key_2, Key_3, Key_4, Key_5, Key_LEDEffectNext,
      Key_Backtick, Key_Q, Key_W, Key_E, Key_R, Key_T, TD(0),
      Key_Tab,      Key_A, Key_S, Key_D, Key_F, Key_G,
      Key_Escape,   Key_Z, Key_X, Key_C, Key_V, Key_B, M(MACRO_HYPER),
      OSM(LeftControl), OSM(LeftAlt), OSM(LeftGui), OSM(LeftShift),
      ShiftToLayer(FUNCTION),

      Key_Backslash,      Key_6, Key_7, Key_8,     Key_9,         Key_0,     LockLayer(NUMPAD),
      TD(1),              Key_Y, Key_U, Key_I,     Key_O,         Key_P,     Key_Equals,
      Key_H,              Key_J, Key_K, Key_L,     Key_Semicolon, Key_Quote,
      M(MACRO_FAT_ARROW), Key_N, Key_M, Key_Comma, Key_Period,    Key_Slash, Key_Minus,
      OSM(RightGui), Key_Enter, Key_Spacebar, Key_Backspace,
      ShiftToLayer(FUNCTION)
    ),

  #else

  #error "No default keymap defined. You should make sure that you have a line like '#define PRIMARY_KEYMAP' in your sketch"

  #endif

    [NUMPAD] = KEYMAP_STACKED(
      ___, ___, ___, ___, ___, ___, ___,
      ___, ___, ___, ___, ___, ___, ___,
      ___, ___, ___, ___, ___, ___,
      ___, ___, ___, ___, ___, ___, ___,
      ___, ___, ___, ___,
      ___,

      M(MACRO_VERSION_INFO), ___,         Key_Keypad7, Key_Keypad8,   Key_Keypad9,        Key_KeypadSubtract, ___,
      ___,                   ___,         Key_Keypad4, Key_Keypad5,   Key_Keypad6,        Key_KeypadAdd,      ___,
      ___,                   Key_Keypad1, Key_Keypad2, Key_Keypad3,   Key_Equals,         ___,
      ___,                   ___,         Key_Keypad0, Key_KeypadDot, Key_KeypadMultiply, Key_KeypadDivide,   Key_Enter,
      ___, ___, ___, ___,
      ___
    ),

    [FUNCTION] = KEYMAP_STACKED(
      ___, Key_F1,             Key_F2,            Key_F3,             Key_F4,         Key_F5, XXX,
      ___, ___,                ___,               ___,                ___,            ___,    ___,
      ___, M(MACRO_CTRL_SHFT), M(MACRO_CMD_CTRL), M(MACRO_OPTN_SHFT), M(MACRO_HYPER), ___,
      ___, M(MACRO_MEHA),      M(MACRO_MEHB),     M(MACRO_MEHC),      M(MACRO_MEHD),  ___,    ___,
      ___, ___, ___, ___,
      ___,

      ___,                Key_F6,                     Key_F7,                   Key_F8,                   Key_F9,                 Key_F10, Key_F11,
      ___,                Key_Home,                   Key_PageDown,             Key_PageUp,               Key_End,                ___,     Key_F12,
                          Key_LeftArrow,              Key_DownArrow,            Key_UpArrow,              Key_RightArrow,         ___,     Consumer_PlaySlashPause,
      M(MACRO_FAT_ARROW), Consumer_ScanPreviousTrack, Consumer_VolumeDecrement, Consumer_VolumeIncrement, Consumer_ScanNextTrack, ___,     Consumer_Mute,
      ___, ___, ___, Key_Delete,
      ___
    )
)

static void versionInfoMacro(uint8_t keyState) {
  if (keyToggledOn(keyState)) {
    Macros.type(PSTR("Keyboardio Model 01 - Kaleidoscope "));
    Macros.type(PSTR(BUILD_INFORMATION));
  }
}

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

  if (OneShot.isActive() && keyToggledOn(keyState)) {
    OneShot.inject(OSM(LeftGui), IS_PRESSED);
    OneShot.inject(OSM(LeftGui), WAS_PRESSED);
    OneShot.inject(OSM(LeftGui), IS_PRESSED);
    OneShot.inject(OSM(LeftGui), WAS_PRESSED);

    OneShot.inject(OSM(LeftAlt), IS_PRESSED);
    OneShot.inject(OSM(LeftAlt), WAS_PRESSED);
    OneShot.inject(OSM(LeftAlt), IS_PRESSED);
    OneShot.inject(OSM(LeftAlt), WAS_PRESSED);

    OneShot.inject(OSM(LeftShift), IS_PRESSED);
    OneShot.inject(OSM(LeftShift), WAS_PRESSED);
    OneShot.inject(OSM(LeftShift), IS_PRESSED);
    OneShot.inject(OSM(LeftShift), WAS_PRESSED);

    OneShot.inject(OSM(LeftControl), IS_PRESSED);
    OneShot.inject(OSM(LeftControl), WAS_PRESSED);
    OneShot.inject(OSM(LeftControl), IS_PRESSED);
    OneShot.inject(OSM(LeftControl), WAS_PRESSED);

    return;
  }

  OneShot.inject(OSM(LeftGui), keyState);
  OneShot.inject(OSM(LeftAlt), keyState);
  OneShot.inject(OSM(LeftShift), keyState);
  OneShot.inject(OSM(LeftControl), keyState);
}

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

  if (OneShot.isActive() && keyToggledOn(keyState)) {
    OneShot.inject(OSM(LeftGui), IS_PRESSED);
    OneShot.inject(OSM(LeftGui), WAS_PRESSED);
    OneShot.inject(OSM(LeftGui), IS_PRESSED);
    OneShot.inject(OSM(LeftGui), WAS_PRESSED);

    OneShot.inject(OSM(LeftAlt), IS_PRESSED);
    OneShot.inject(OSM(LeftAlt), WAS_PRESSED);
    OneShot.inject(OSM(LeftAlt), IS_PRESSED);
    OneShot.inject(OSM(LeftAlt), WAS_PRESSED);

    OneShot.inject(OSM(LeftControl), IS_PRESSED);
    OneShot.inject(OSM(LeftControl), WAS_PRESSED);
    OneShot.inject(OSM(LeftControl), IS_PRESSED);
    OneShot.inject(OSM(LeftControl), WAS_PRESSED);

    return;
  }

  OneShot.inject(OSM(LeftGui), keyState);
  OneShot.inject(OSM(LeftAlt), keyState);
  OneShot.inject(OSM(LeftControl), keyState);
}

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

  if (OneShot.isActive() && keyToggledOn(keyState)) {
    OneShot.inject(OSM(LeftAlt), IS_PRESSED);
    OneShot.inject(OSM(LeftAlt), WAS_PRESSED);
    OneShot.inject(OSM(LeftAlt), IS_PRESSED);
    OneShot.inject(OSM(LeftAlt), WAS_PRESSED);

    OneShot.inject(OSM(LeftShift), IS_PRESSED);
    OneShot.inject(OSM(LeftShift), WAS_PRESSED);
    OneShot.inject(OSM(LeftShift), IS_PRESSED);
    OneShot.inject(OSM(LeftShift), WAS_PRESSED);

    OneShot.inject(OSM(LeftControl), IS_PRESSED);
    OneShot.inject(OSM(LeftControl), WAS_PRESSED);
    OneShot.inject(OSM(LeftControl), IS_PRESSED);
    OneShot.inject(OSM(LeftControl), WAS_PRESSED);

    return;
  }

  OneShot.inject(OSM(LeftAlt), keyState);
  OneShot.inject(OSM(LeftShift), keyState);
  OneShot.inject(OSM(LeftControl), keyState);
}

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

  if (OneShot.isActive() && keyToggledOn(keyState)) {
    OneShot.inject(OSM(LeftGui), IS_PRESSED);
    OneShot.inject(OSM(LeftGui), WAS_PRESSED);
    OneShot.inject(OSM(LeftGui), IS_PRESSED);
    OneShot.inject(OSM(LeftGui), WAS_PRESSED);

    OneShot.inject(OSM(LeftShift), IS_PRESSED);
    OneShot.inject(OSM(LeftShift), WAS_PRESSED);
    OneShot.inject(OSM(LeftShift), IS_PRESSED);
    OneShot.inject(OSM(LeftShift), WAS_PRESSED);

    OneShot.inject(OSM(LeftControl), IS_PRESSED);
    OneShot.inject(OSM(LeftControl), WAS_PRESSED);
    OneShot.inject(OSM(LeftControl), IS_PRESSED);
    OneShot.inject(OSM(LeftControl), WAS_PRESSED);

    return;
  }

  OneShot.inject(OSM(LeftGui), keyState);
  OneShot.inject(OSM(LeftShift), keyState);
  OneShot.inject(OSM(LeftControl), keyState);
}

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

  if (OneShot.isActive() && keyToggledOn(keyState)) {
    OneShot.inject(OSM(LeftGui), IS_PRESSED);
    OneShot.inject(OSM(LeftGui), WAS_PRESSED);
    OneShot.inject(OSM(LeftGui), IS_PRESSED);
    OneShot.inject(OSM(LeftGui), WAS_PRESSED);

    OneShot.inject(OSM(LeftAlt), IS_PRESSED);
    OneShot.inject(OSM(LeftAlt), WAS_PRESSED);
    OneShot.inject(OSM(LeftAlt), IS_PRESSED);
    OneShot.inject(OSM(LeftAlt), WAS_PRESSED);

    OneShot.inject(OSM(LeftShift), IS_PRESSED);
    OneShot.inject(OSM(LeftShift), WAS_PRESSED);
    OneShot.inject(OSM(LeftShift), IS_PRESSED);
    OneShot.inject(OSM(LeftShift), WAS_PRESSED);

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

  if (OneShot.isActive() && keyToggledOn(keyState)) {
    OneShot.inject(OSM(LeftGui), IS_PRESSED);
    OneShot.inject(OSM(LeftGui), WAS_PRESSED);
    OneShot.inject(OSM(LeftGui), IS_PRESSED);
    OneShot.inject(OSM(LeftGui), WAS_PRESSED);

    OneShot.inject(OSM(LeftControl), IS_PRESSED);
    OneShot.inject(OSM(LeftControl), WAS_PRESSED);
    OneShot.inject(OSM(LeftControl), IS_PRESSED);
    OneShot.inject(OSM(LeftControl), WAS_PRESSED);

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

  if (OneShot.isActive() && keyToggledOn(keyState)) {
    OneShot.inject(OSM(LeftAlt), IS_PRESSED);
    OneShot.inject(OSM(LeftAlt), WAS_PRESSED);
    OneShot.inject(OSM(LeftAlt), IS_PRESSED);
    OneShot.inject(OSM(LeftAlt), WAS_PRESSED);

    OneShot.inject(OSM(LeftShift), IS_PRESSED);
    OneShot.inject(OSM(LeftShift), WAS_PRESSED);
    OneShot.inject(OSM(LeftShift), IS_PRESSED);
    OneShot.inject(OSM(LeftShift), WAS_PRESSED);

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

  if (OneShot.isActive() && keyToggledOn(keyState)) {
    OneShot.inject(OSM(LeftControl), IS_PRESSED);
    OneShot.inject(OSM(LeftControl), WAS_PRESSED);
    OneShot.inject(OSM(LeftControl), IS_PRESSED);
    OneShot.inject(OSM(LeftControl), WAS_PRESSED);

    OneShot.inject(OSM(LeftShift), IS_PRESSED);
    OneShot.inject(OSM(LeftShift), WAS_PRESSED);
    OneShot.inject(OSM(LeftShift), IS_PRESSED);
    OneShot.inject(OSM(LeftShift), WAS_PRESSED);

    return;
  }

  OneShot.inject(OSM(LeftControl), keyState);
  OneShot.inject(OSM(LeftShift), keyState);
}

void tapDanceAction(uint8_t tap_dance_index, byte row, byte col, uint8_t tap_count,
                    kaleidoscope::TapDance::ActionType tap_dance_action) {
  switch (tap_dance_index) {
    case 0: {
      return tapDanceActionKeys(tap_count, tap_dance_action,
                                Key_LeftBracket,
                                Key_LeftParen);
    }
    case 1: {
      return tapDanceActionKeys(tap_count, tap_dance_action,
                                Key_RightBracket,
                                Key_RightParen);
    }
  }
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

static kaleidoscope::LEDSolidColor solidWhite(160, 160, 160);
static kaleidoscope::LEDSolidColor solidGreen(0, 160, 0);
static kaleidoscope::LEDSolidColor solidIndigo(0, 0, 170);

void toggleLedsOnSuspendResume(kaleidoscope::HostPowerManagement::Event event) {
  switch (event) {
  case kaleidoscope::HostPowerManagement::Suspend:
    LEDControl.paused = true;
    LEDControl.set_all_leds_to({0, 0, 0});
    LEDControl.syncLeds();
    break;
  case kaleidoscope::HostPowerManagement::Resume:
    LEDControl.paused = false;
    LEDControl.refreshAll();
    break;
  case kaleidoscope::HostPowerManagement::Sleep:
    break;
  }
}

void hostPowerManagementEventHandler(kaleidoscope::HostPowerManagement::Event event) {
  toggleLedsOnSuspendResume(event);
}

enum {
  COMBO_TOGGLE_NKRO_MODE
};

static void toggleKeyboardProtocol(uint8_t combo_index) {
  USBQuirks.toggleKeyboardProtocol();
}

USE_MAGIC_COMBOS({.action = toggleKeyboardProtocol,
                  .keys = {R3C6, R2C6, R3C7}});

KALEIDOSCOPE_INIT_PLUGINS(
  EEPROMSettings,
  EEPROMKeymap,
  Focus,
  FocusSettingsCommand,
  FocusEEPROMCommand,
  BootGreetingEffect,
  TestMode,
  LEDControl,
  LEDOff,
  solidGreen, solidWhite, solidIndigo,
  NumPad,
  Macros,
  HostPowerManagement,
  MagicCombo,
  USBQuirks,
  OneShot,
  EscapeOneShot,
  ActiveModColorEffect,
  TopsyTurvy,
  TapDance,
  LEDControl,
  LEDPaletteTheme,
  ColormapEffect
);

void setup() {
  Serial.begin(9600);
  Kaleidoscope.setup();
  ActiveModColorEffect.highlight_color = CRGB(0x00, 0xff, 0xff);
  NumPad.numPadLayer = NUMPAD;
  LEDOff.activate();
  EEPROMKeymap.setup(5, EEPROMKeymap.Mode::EXTEND);
  ColormapEffect.max_layers(3 + 5);
}

void loop() {
  Kaleidoscope.loop();
}
