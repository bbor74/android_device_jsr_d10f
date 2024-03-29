/*
 * Copyright (C) 2014 The CyanogenMod Project
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


//#define LOG_NDEBUG 0

#define LOG_TAG "qdlights"
#include <cutils/log.h>
#include <cutils/properties.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <pthread.h>
#include <math.h>

#include <sys/ioctl.h>
#include <sys/types.h>

#include <hardware/lights.h>

/******************************************************************************/

#ifndef LIGHTS_HARDWARE_MODULE_ID
#define LIGHTS_HARDWARE_MODULE_ID "lights"
#endif

#define MAX_PATH_SIZE 80

static pthread_once_t g_init = PTHREAD_ONCE_INIT;
static pthread_mutex_t g_lock = PTHREAD_MUTEX_INITIALIZER;
static struct light_state_t g_notification;
static struct light_state_t g_battery;
static int g_attention = 0;

char const*const RED_LED_FILE
        = "/sys/class/leds/red/brightness";

char const*const GREEN_LED_FILE
        = "/sys/class/leds/green/brightness";

char const*const BLUE_LED_FILE
        = "/sys/class/leds/blue/brightness";

char const*const LCD_FILE
        = "/sys/class/leds/lcd-backlight/brightness";

char const*const BUTTON_FILE
        = "/sys/class/leds/button-backlight/brightness";

char const*const RED_BLINK_FILE
        = "/sys/class/leds/red/blink";

char const*const GREEN_BLINK_FILE
        = "/sys/class/leds/green/blink";

char const*const BLUE_BLINK_FILE
        = "/sys/class/leds/blue/blink";

char const*const BATTERY_STATUS
        = "/sys/class/power_supply/battery/status";

/**
 * device methods
 */

void init_globals(void)
{
    // init the mutex
    pthread_mutex_init(&g_lock, NULL);
}

static int already_warned = 0;
static int already_warn_2 = 0;

static int
write_int(char const* path, int value)
{
    int fd = open(path, O_RDWR);
    if (fd >= 0) {
        char buffer[20];
        int bytes = sprintf(buffer, "%d\n", value);
        int amt = write(fd, buffer, bytes);
        close(fd);
        return amt == -1 ? -errno : 0;
    } else {
        if (already_warned == 0) {
            ALOGE("write_int failed to open %s\n", path);
            already_warned = 1;
        }
        return -errno;
    }
}

static int
is_lit(void)
{
    char buf[24];
    int fd = open(BATTERY_STATUS, O_RDONLY);
    if (fd < 0) {
      if (already_warn_2 == 0) {
        ALOGE("is_lit failed to open %s\n", BATTERY_STATUS);
        already_warn_2 = 1;
      }
      return 0;
    }
    int len = read(fd, buf, sizeof(buf)-2);
    close(fd);
    if (len < 3)
      return 0;
    buf[len] = 0;
    if (buf[len-1] == '\n')
      buf[len-1] = 0;
    if (strcmp(buf, "Charging") == 0)
      return 1;
    if (strcmp(buf, "Full") == 0)
      return 1;
    return 0;
}

static int
rgb_to_brightness(struct light_state_t const* state)
{
    int color = state->color & 0x00ffffff;
    return (  (77*((color>>16)&0x00ff))
            + (150*((color>>8)&0x00ff))
            + (29*(color&0x00ff))) >> 8;
}

static int
set_light_backlight(struct light_device_t* dev,
        struct light_state_t const* state)
{
    int err = 0;
    int brightness = rgb_to_brightness(state);
    //ALOGD("set_light_backlight: mode %d, color=%08X, onMS=%d, offMS=%d, brightness=%d",
    //  state->flashMode, state->color, state->flashOnMS, state->flashOffMS, brightness);
    pthread_mutex_lock(&g_lock);
    err = write_int(LCD_FILE, brightness);
    pthread_mutex_unlock(&g_lock);
    return err;
}

static int
set_speaker_light_locked(struct light_device_t* dev,
        struct light_state_t const* state)
{
    int red, green, blue;
    int blink;
    int onMS, offMS;
    unsigned int colorRGB;

    switch (state->flashMode) {
        case LIGHT_FLASH_TIMED:      // 1
            onMS = state->flashOnMS;
            offMS = state->flashOffMS;
            break;
        case LIGHT_FLASH_NONE:       // 0
        default:
            onMS = 0;
            offMS = 0;
            break;
    }

    colorRGB = state->color;

    ALOGD("set_speaker_light_locked: mode %d, colorRGB=%08X, onMS=%d, offMS=%d\n",
            state->flashMode, colorRGB, onMS, offMS);

    red = (colorRGB >> 16) & 0xFF;
    green = (colorRGB >> 8) & 0xFF;
    blue = colorRGB & 0xFF;

    blink = state->flashMode;

    if (red == 255 && green == 255 && blue == 0) {
      colorRGB = 0xFFFF;  // colorRGB >>= 8;
      blue = 255;
      blink = 2;
      green = 0;
      red = 0;
    } else
    if (red == 0 && green == 0 && blue == 0) {
      if ( blink == 1 ) {
        blink = 3;
        blue = 0;
        red = 0;
      } else {
        blink = 0;
        red = 0;
      }
    } else
    if (red == 0 && green == 255 && blue == 0) {
      blue = 255;
      green = 0;
    }

    ALOGD("set_speaker_light_locked: MODIFY: onMS=%d,offMS=%d,red=%x,green=%x,blue=%x,blink=%d\n",
            onMS, offMS, red, green, blue, blink);

    if (blink == 0) {
      write_int(BLUE_BLINK_FILE, 0);
      write_int(BLUE_LED_FILE, blue);
      return 0;
    }
    if (colorRGB == 0) {
      write_int(BLUE_BLINK_FILE, blue ? 1 : 0);
      write_int(BLUE_LED_FILE, blue);
      return 0;
    }

    write_int(BLUE_LED_FILE, 0);
    if (blue)
      write_int(BLUE_BLINK_FILE, blink);
    return 0;
}

static void
handle_speaker_battery_locked(struct light_device_t* dev)
{
    int lit = is_lit();
    struct light_state_t * x = &g_notification;
    if (!lit && (g_battery.color & 0xFFFFFF)) {
      x = &g_battery;
    } else
    if (lit && !(g_notification.color & 0xFFFFFF)) {
      x = &g_battery;
    }
    set_speaker_light_locked(dev, x);
}

static int
set_light_notifications(struct light_device_t* dev,
        struct light_state_t const* state)
{
    pthread_mutex_lock(&g_lock);
    g_notification = *state;
    handle_speaker_battery_locked(dev);
    pthread_mutex_unlock(&g_lock);
    return 0;
}

static int
set_light_attention(struct light_device_t* dev,
        struct light_state_t const* state)
{
    pthread_mutex_lock(&g_lock);
    if (state->flashMode == LIGHT_FLASH_HARDWARE) {
        g_attention = state->flashOnMS;
    } else
    if (state->flashMode == LIGHT_FLASH_NONE) {
        g_attention = 0;
    }
    handle_speaker_battery_locked(dev);
    pthread_mutex_unlock(&g_lock);
    return 0;
}

static int
set_light_buttons(struct light_device_t* dev,
        struct light_state_t const* state)
{
    int err = 0;
    if (!dev || !state)
      return -1;
    pthread_mutex_lock(&g_lock);
    err = write_int(BUTTON_FILE, state->color & 0xFF);
    //ALOGD("set_light_buttons: mode %d, color=%08X, onMS=%d, offMS=%d, err=%d",
    //  state->flashMode, state->color, state->flashOnMS, state->flashOffMS, err);
    pthread_mutex_unlock(&g_lock);
    return err;
}

static int
set_light_battery(struct light_device_t* dev,
        struct light_state_t const* state)
{
    pthread_mutex_lock(&g_lock);
    g_battery = *state;
    handle_speaker_battery_locked(dev);
    pthread_mutex_unlock(&g_lock);
    return 0;
}

/* Close the lights device */
static int
close_lights(struct light_device_t *dev)
{
    if (dev) {
        free(dev);
    }
    return 0;
}


/******************************************************************************/

/**
 * module methods
 */

/** Open a new instance of a lights device using name */
static int open_lights(const struct hw_module_t* module, char const* name,
        struct hw_device_t** device)
{
    int (*set_light)(struct light_device_t* dev,
            struct light_state_t const* state);

    if (0 == strcmp(LIGHT_ID_BACKLIGHT, name))
        set_light = set_light_backlight;
    else if (0 == strcmp(LIGHT_ID_ATTENTION, name))
        set_light = set_light_attention;
    else if (0 == strcmp(LIGHT_ID_NOTIFICATIONS, name))
        set_light = set_light_notifications;
    else if (0 == strcmp(LIGHT_ID_BATTERY, name))
        set_light = set_light_battery;
    else if (0 == strcmp(LIGHT_ID_BUTTONS, name))
        set_light = set_light_buttons;
    else
        return -EINVAL;

    pthread_once(&g_init, init_globals);

    struct light_device_t *dev = malloc(sizeof(struct light_device_t));
    memset(dev, 0, sizeof(*dev));

    dev->common.tag = HARDWARE_DEVICE_TAG;
    dev->common.version = 0;
    dev->common.module = (struct hw_module_t*)module;
    dev->common.close = (int (*)(struct hw_device_t*))close_lights;
    dev->set_light = set_light;

    *device = (struct hw_device_t*)dev;
    return 0;
}

static struct hw_module_methods_t lights_module_methods = {
    .open =  open_lights,
};

/*
 * The lights Module
 */
struct hw_module_t HAL_MODULE_INFO_SYM = {
    .tag = HARDWARE_MODULE_TAG,
    .version_major = 1,
    .version_minor = 0,
    .id = LIGHTS_HARDWARE_MODULE_ID,
    .name = "JSR lights Module",
    .author = "Google, Inc., CyanogenMod",
    .methods = &lights_module_methods,
};
