#pragma once

#include "lvgl.h"
#include "main.hpp"

#include <string>

namespace wifi
{
    class gui
    {
    private:
        static gui *unique;
        static bool wifi_status;
        lv_display_t *display = nullptr;
        lv_indev_t *indev = nullptr;
        context *context_pointer = nullptr;
        lv_obj_t *screen = nullptr;
        lv_obj_t *container = nullptr;
        lv_obj_t *bar = nullptr;
        lv_obj_t *exit_button = nullptr;
        lv_obj_t *title_label = nullptr;
        lv_obj_t *status_led = nullptr;
        lv_obj_t *name_group = nullptr;
        lv_obj_t *name_editor = nullptr;
        lv_obj_t *name_item = nullptr;
        lv_obj_t *name_label = nullptr;
        lv_obj_t *name_line = nullptr;
        lv_obj_t *password_group = nullptr;
        lv_obj_t *password_editor = nullptr;
        lv_obj_t *password_item = nullptr;
        lv_obj_t *password_label = nullptr;
        lv_obj_t *password_line = nullptr;
        lv_obj_t *button_group = nullptr;
        lv_obj_t *disconnect_button = nullptr;
        lv_obj_t *disconnect_label = nullptr;
        lv_obj_t *connect_button = nullptr;
        lv_obj_t *connect_label = nullptr;
        lv_obj_t *button_line = nullptr;
        lv_obj_t *input_container = nullptr;
        lv_obj_t *keyboard = nullptr;
        lv_obj_t *pinyin = nullptr;
        lv_obj_t *cand_panel = nullptr;
        gui();
        ~gui();
        static void timer_callback(lv_timer_t *const timer);
        static void animation_complete_callback(lv_anim_t *const animation);
        static void short_click_event_callback(lv_event_t *const event);

    public:
        /**
         * @brief get the unique gui.
         * @return return the unique gui pointer.
         * @version 1.0
         * @date 2024/9/19
         * @author ProYRB
         */
        static gui *get_unique();
        void initialize(const std::string &fbdev, const std::string &evdev);
        void execute();
    };
} // namespace wifi
