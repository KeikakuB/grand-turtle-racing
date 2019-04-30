#include "stdafx.h"
#include "WindowsInputManager.h"

#include "util.h"
#include "resource.h"

#include <map>

using namespace UtilitairesDX;

namespace PM3D
{
  namespace meta
  {
    REGISTER_COMPONENT_CPP_ID(WindowsInputManager);
    template<>
    component_interface_id_list_t GetComponentInterfaces<WindowsInputManager>() noexcept { return{ GetInterfaceId<IInputManager>() }; }
  }
  WindowsInputManager::WindowsInputManager(HINSTANCE hInstance, HWND hWnd)
    : direct_input_{}, keyboard_{}, mouse_{}
  {
    // Objet DirectInput
    DXEssayer(DirectInput8Create(hInstance,
      DIRECTINPUT_VERSION,
      IID_IDirectInput8,
      (void**) &direct_input_,
      NULL), DXE_CREATION_DIRECTINPUT);
    // Objet Clavier
    DXEssayer(direct_input_->CreateDevice(GUID_SysKeyboard,
      &keyboard_,
      NULL),
      DXE_CREATION_CLAVIER);
    DXEssayer(keyboard_->SetDataFormat(&c_dfDIKeyboard),
      DXE_CREATION_FORMAT_CLAVIER);
    keyboard_->SetCooperativeLevel(hWnd,
      DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);
    keyboard_->Acquire();
    // Objet Souris
    DXEssayer(direct_input_->CreateDevice(GUID_SysMouse, &mouse_, NULL),
      DXE_CREATION_SOURIS);
    DXEssayer(mouse_->SetDataFormat(&c_dfDIMouse), DXE_CREATION_FORMAT_SOURIS);
    mouse_->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
    mouse_->Acquire();
  }
  bool WindowsInputManager::IsKeyDown(KeyboardKey key) const
  {
    static std::map<KeyboardKey, UINT> keys =
    {
      {KeyboardKey::UpArrow, DIK_UP },
      { KeyboardKey::LeftArrow, DIK_LEFT },
      { KeyboardKey::RightArrow, DIK_RIGHT},
      { KeyboardKey::DownArrow, DIK_DOWN },
      { KeyboardKey::SpaceBar, DIK_SPACE},
      { KeyboardKey::Escape, DIK_ESCAPE},
      { KeyboardKey::Enter, DIK_RETURN},
      { KeyboardKey::Zero, DIK_0},
      { KeyboardKey::One, DIK_1 },
      { KeyboardKey::Two, DIK_2 },
      { KeyboardKey::Three, DIK_3 },
      { KeyboardKey::Four, DIK_4 },
      { KeyboardKey::Five, DIK_5 },
      { KeyboardKey::Six, DIK_6 },
      { KeyboardKey::Seven, DIK_7 },
      { KeyboardKey::Eight, DIK_8 },
      { KeyboardKey::Nine, DIK_9 },
      { KeyboardKey::R, DIK_R },
      {KeyboardKey::L, DIK_L }
    };
    if ( keys.find(key) == keys.end() )
    {
      throw no_such_key_mapped{};
    }
    return keyboard_state_[ keys[ key ] ] & 0x80;
  }

  void WindowsInputManager::RunTask()
  {
    UpdateKeyboardState();
    UpdateMouseState();
  }

  WindowsInputManager::~WindowsInputManager()
  {
    if ( keyboard_ )
    {
      keyboard_->Unacquire();
      keyboard_->Release();
      keyboard_ = NULL;
    }
    if ( mouse_ )
    {
      mouse_->Unacquire();
      mouse_->Release();
      mouse_ = NULL;
    }
    if ( direct_input_ )
    {
      direct_input_->Release();
      keyboard_ = NULL;
    }
  }

  long WindowsInputManager::GetMouseXaxis() const
  {
    return mouse_state_.lX;
  }

  long WindowsInputManager::GetMouseYaxis() const
  {
    return mouse_state_.lY;
  }

  long WindowsInputManager::GetMouseZaxis() const
  {
    return mouse_state_.lZ;
  }

  bool WindowsInputManager::IsMouseButtonPressed(MouseButton button) const
  {
    return mouse_state_.rgbButtons[ static_cast<short>( button ) ] & 0x80;
  }
}