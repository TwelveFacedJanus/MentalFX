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

        [[nodiscard]] const uint32_t* getPosition() const { return this->_position; }
        [[nodiscard]] const uint32_t* getSize() const { return this->_size; }
};

}

#endif // MENTAL_FRAMEWORK_H
