#pragma once
#include "Meta.h"
#include "IInputManager.h"
#include "GameManagerComponent.h"

#include <dinput.h>

namespace PM3D
{
  class no_such_key_mapped {};
  class WindowsInputManager final :
    public GameManagerComponent, public IInputManager
  {
  public:
    WindowsInputManager(HINSTANCE hInstance, HWND hWnd);
    ~WindowsInputManager();
    bool IsKeyDown(KeyboardKey key) const override;
    long GetMouseXaxis() const override;
    long GetMouseYaxis() const override;
    long GetMouseZaxis() const override;
    bool IsMouseButtonPressed(MouseButton button) const override;
    void RunTask() override;
  private:
    void UpdateKeyboardState()
    {
      keyboard_->GetDeviceState(sizeof(keyboard_state_), (void*) & keyboard_state_);
    }
    void UpdateMouseState()
    {
      mouse_->GetDeviceState(sizeof(mouse_state_), (void*) &mouse_state_);
    }
    IDirectInput8* direct_input_;
    IDirectInputDevice8* keyboard_;
    char keyboard_state_[ 256 ];
    IDirectInputDevice8* mouse_;
    DIMOUSESTATE mouse_state_;
  };
  namespace meta
  {
    REGISTER_COMPONENT_H_ID(WindowsInputManager);
  }
}