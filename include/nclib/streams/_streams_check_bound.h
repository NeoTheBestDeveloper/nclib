#pragma once

#ifdef CHECK_BOUND

#include "nclib/panic.h"

#undef STREAM_CHECK_BOUND

#define STREAM_CHECK_BOUND(_stream_, _offset_diff_)                           \
    if (_stream_->_offset + _offset_diff_ > _stream_->_size) {                \
        panic("Error: stream access out of bound at %s:%d. Size=%lu, access " \
              "by index=%ld.\n",                                              \
              __FILE__, __LINE__, _stream_->_size,                            \
              _stream_->_offset + _offset_diff_);                             \
    }

#else

#define STREAM_CHECK_BOUND(_stream_, _offset_diff_)

#endif // endif !CHECK_BOUND
