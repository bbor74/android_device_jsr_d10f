/*
 * Copyright (C) 2012 The Android Open Source Project
 * Copyright (C) 2016 The CyanogenMod Project
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

#ifndef _BDROID_BUILDCFG_H
#define _BDROID_BUILDCFG_H

#include <cutils/properties.h>
#include <string.h>

#define BTM_DEF_LOCAL_NAME   "JSR D10F"

#define BLUETOOTH_QTI_SW TRUE
#define MAX_ACL_CONNECTIONS   7
#define MAX_L2CAP_CHANNELS    16
#define BLE_VND_INCLUDED   TRUE
#define BT_CLEAN_TURN_ON_DISABLED TRUE

/* Defined if the kernel does not have support for CLOCK_BOOTTIME_ALARM 
#define KERNEL_MISSING_CLOCK_BOOTTIME_ALARM TRUE */

#endif
