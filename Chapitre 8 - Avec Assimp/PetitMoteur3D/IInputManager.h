#pragma once
#include "Meta.h"

namespace PM3D
{
  enum class MouseButton : short { kLeft = 0, kRight = 1, kMiddle = 2, kExtra = 3 };
  enum class KeyboardKey
  {
    Zero,
    One,
    Two,
    Three,
    Four,
    Five,
    Six,
    Seven,
    Eight,
    Nine,
    LeftArrow,
    RightArrow,
    UpArrow,
    DownArrow,
    SpaceBar,
    Escape,
    Enter,
    R,
    L
  };
  class IInputManager
  {
  public:
    virtual bool IsKeyDown(KeyboardKey key) const = 0;
    virtual long GetMouseXaxis() const = 0;
    virtual long GetMouseYaxis() const = 0;
    virtual long GetMouseZaxis() const = 0;
    virtual bool IsMouseButtonPressed(MouseButton button) const = 0;
    virtual ~IInputManager() = default;
  };
  namespace meta
  {
    REGISTER_INTERFACE_H_ID(IInputManager);
  }
}