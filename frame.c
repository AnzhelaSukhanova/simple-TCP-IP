#include "frame.h"

struct frame* init_frame (char* buf)
{
    struct frame *frame = (struct frame *) buf;
    return frame;
}
