#ifndef MENTAL_FRAMEWORK_H
#define MENTAL_FRAMEWORK_H

#include <cstdint>


namespace mental_framework
{

class MFW_C_Window
{
    private:
        uint32_t _position[2] = {0, 0};
        uint32_t _size[2] = {0, 0};
    public:
        MFW_C_Window() = delete;
        ~MFW_C_Window() = delete;

        MFW_C_Window(const MFW_C_Window&) = delete;
        MFW_C_Window& operator=(const MFW_C_Window&) = delete;

        MFW_C_Window(MFW_C_Window&&) = delete;
        MFW_C_Window& operator=(MFW_C_Window&&) = delete;


        [[nodiscard]] const uint32_t*       getSize() const                         { return this->_size; }
        [[nodiscard]]       uint32_t        getSize(uint8_t index)                  { return (index < 2) ? _size[index] : 0;  }

        [[nodiscard]] const uint32_t*       getPosition() const                     { return this->_position; }
        [[nodiscard]]       uint32_t        getPosition(uint8_t index)              { return (index < 2) ? _position[index] : 0;  }

                            void            setPosition(uint32_t x, uint32_t y)     { this->_position[0] = x; this->_position[1] = y; }
                            void            setSize    (uint32_t x, uint32_t y)     { this->_size[0] = x; this->_size[1] = y; }

        [[nodiscard]]       bool            windowShouldClose();
};

}

#endif // MENTAL_FRAMEWORK_H
