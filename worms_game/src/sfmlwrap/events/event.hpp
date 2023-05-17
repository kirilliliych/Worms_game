#pragma once


#include <chrono>
#include "physics_entity.hpp"
#include "point2d.hpp"


enum class EventType
{
    NO_EVENT,
    MOUSE_BUTTON_PRESSED,
    MOUSE_BUTTON_RELEASED,
    MOUSE_MOVED,
    MOUSE_ENTERED,
    MOUSE_LEFT,
    KEY_PRESSED,
    KEY_RELEASED,
    TIME_PASSED,
    QUIT_EVENT,
    EXPLOSION_EVENT,
    COLLISION_EVENT,
    STABILITY_EVENT,
    OTHER_EVENT,
    EVENT_TYPE_CNT
};


enum class KeyboardKey
{
    Unknown = -1, 
    A = 0,        
    B,            
    C,            
    D,            
    E,            
    F,            
    G,            
    H,            
    I,            
    J,            
    K,            
    L,            
    M,            
    N,            
    O,            
    P,            
    Q,            
    R,            
    S,            
    T,            
    U,            
    V,            
    W,            
    X,            
    Y,            
    Z,            
    Num0,         
    Num1,         
    Num2,         
    Num3,         
    Num4,         
    Num5,         
    Num6,         
    Num7,         
    Num8,         
    Num9,         
    Escape,       
    LControl,     
    LShift,       
    LAlt,         
    LSystem,      
    RControl,     
    RShift,       
    RAlt,         
    RSystem,      
    Menu,         
    LBracket,     
    RBracket,     
    Semicolon,    
    Comma,        
    Period,       
    Quote,        
    Slash,        
    Backslash,    
    Tilde,        
    Equal,        
    Hyphen,       
    Space,        
    Enter,        
    Backspace,    
    Tab,          
    PageUp,       
    PageDown,     
    End,          
    Home,         
    Insert,       
    Delete,       
    Add,          
    Subtract,     
    Multiply,     
    Divide,       
    Left,         
    Right,        
    Up,           
    Down,         
    Numpad0,      
    Numpad1,      
    Numpad2,      
    Numpad3,      
    Numpad4,      
    Numpad5,      
    Numpad6,      
    Numpad7,      
    Numpad8,      
    Numpad9,      
    F1,           
    F2,           
    F3,           
    F4,           
    F5,           
    F6,           
    F7,           
    F8,           
    F9,           
    F10,          
    F11,          
    F12,          
    F13,          
    F14,          
    F15,          
    Pause
};

struct KeyEventData
{
    KeyboardKey key_code;
};


enum class MouseButton
{
    UNKNOWN = -1,
    LEFT,
    RIGHT,
    WHEEL,
    EXTRA1,
    EXTRA2
};

struct MouseButtonEventData
{
    MouseButton button;
    Point2d<int> position;
};

struct MouseMoveEventData
{
    Point2d<int> position;
};

struct MouseWheelEventData
{
    int delta;
    Point2d<int> position;
};

struct CollisionEventData
{
    const void *checker_address;
    PhysicsEntity checker;
    Point2d<int> position;
};

struct ExplosionEventData
{
    float radius;
    float full_damage_radius;
    int damage;
    Point2d<int> position;
};


enum class EventHandlerState
{
    Skipped,
    Accepted
};


class Event
{
public:

    Event()
      : type_(EventType::NO_EVENT)
    {}

    EventType get_type() const
    {
        return type_;
    }

    void set_type(EventType type)
    {
        type_ = type;
    }

private:

    EventType type_;

public:

    using time_delta_t = std::chrono::duration<float, std::chrono::seconds::period>;
    union
    {
        MouseButtonEventData mbedata_;
        MouseMoveEventData   mmedata_;
        MouseWheelEventData  mwedata_;
        KeyEventData         kedata_;
        CollisionEventData   cedata_;
        ExplosionEventData   eedata_;
        time_delta_t         dt_;
    };
};
