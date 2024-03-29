/*
 * Copyright (C) 2015 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "log.h"
#include <libgen.h>
#include <stdlib.h>
#include <string.h>
#include <sys/uio.h>


static void init_klog_vwrite(int level, const char* fmt, va_list ap) {
    const char* tag = basename(getprogname());

   // if (level > klog_get_level()) return;

    // The kernel's printk buffer is only 1024 bytes.
    // TODO: should we automatically break up long lines into multiple lines?
    // Or we could log but with something like "..." at the end?
    char buf[1024];
    size_t prefix_size = snprintf(buf, sizeof(buf), "<%d>%s: ", level, tag);
    size_t msg_size = vsnprintf(buf + prefix_size, sizeof(buf) - prefix_size, fmt, ap);
    if (msg_size >= sizeof(buf) - prefix_size) {
        msg_size = snprintf(buf + prefix_size, sizeof(buf) - prefix_size,
                            "(%zu-byte message too long for printk)\n", msg_size);
    }

    struct iovec iov[1];
    iov[0].iov_base = buf;
    iov[0].iov_len = prefix_size + msg_size;

    klog_writev(level, iov, 1);
}

void init_klog_write(int level, const char* fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    init_klog_vwrite(level, fmt, ap);
    va_end(ap);
}
