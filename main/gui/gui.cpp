#include "gui.hpp"
#include "debug.hpp"
#include "error.hpp"
#include "main.hpp"

#include <fstream>
#include <string.h>
#include <unistd.h>

using namespace std;
using namespace configor;

#define BACKGROUND_COLOR       0xEEEEEE
#define FOREGROUND_COLOR       0xFFFFFF
#define CONTAINER_MARGIN       100
#define BAR_HEIGHT             100
#define BAR_RADIUS             BAR_HEIGHT / 3
#define EXIT_BUTTON_SIZE       BAR_HEIGHT * 0.65
#define EXIT_BUTTON_IMAGE_SIZE BAR_HEIGHT * 0.55
#define BAR_PAD                (BAR_HEIGHT - EXIT_BUTTON_SIZE) / 2
#define LINE_WIDTH             4
#define PANEL_HEIGHT           60

namespace wifi
{
    gui *gui::unique = nullptr;
    bool gui::wifi_status;

    gui::gui() { Debug_Log("the unique of gui generation"); }

    gui::~gui() {}

    bool get_wlan_status(void)
    {
        bool return_value = false;              /*返回值*/
        char chk_std[] = "wpa_state=COMPLETED"; /*比对标准*/
        char command_buffer[128];
        sprintf(command_buffer, "wpa_cli -i wlan0 status | grep -r \"%s\"",
                chk_std);                           /*生成格式化命令*/
        FILE *tmp_fil = popen(command_buffer, "r"); /*打开文件*/
        if (NULL != tmp_fil)
        {
            if (NULL != fgets(command_buffer, 128, tmp_fil))
            {
                if (0 == memcmp(command_buffer, chk_std, strlen(chk_std)))
                    return_value = true; /*如果与比对标准一致则说明连接成功*/
            }
            pclose(tmp_fil); /*关闭文件*/
        }
        return return_value;
    }

    void gui::timer_callback(lv_timer_t *const timer)
    {
        Debug_Log("timer timeout");
        wifi_status = get_wlan_status(); /*检测WLAN的网络连接状态*/
        Debug_Log("wifi_status: " << wifi_status);
        gui *const data = ((gui *)lv_timer_get_user_data(timer));
        if (false == wifi_status)
        {
            lv_led_set_color(data->status_led, lv_color_hex(0xFF0000));
        }
        else
        {
            lv_led_set_color(data->status_led, lv_color_hex(0x0000FF));
        }
    }

    void gui::short_click_event_callback(lv_event_t *const event)
    {
        lv_obj_t *const event_trigger = (lv_obj_t *)lv_event_get_target(event);
        gui *const data = ((gui *)lv_event_get_user_data(event));

        if (event_trigger == data->exit_button)
        {
            Debug_Log("exit");
            exit(0);
        }
        else if (event_trigger == data->name_editor)
        {
            Debug_Log("name_editor trigger short_click_event");
            lv_keyboard_set_textarea(data->keyboard, event_trigger);
            lv_anim_t animation;
            lv_anim_init(&animation);
            lv_anim_set_exec_cb(&animation, (lv_anim_exec_xcb_t)lv_obj_set_y);
            lv_anim_set_var(&animation, data->input_container);
            lv_anim_set_duration(&animation, 500);
            lv_anim_set_values(&animation, lv_obj_get_y_aligned(data->input_container), 0);
            lv_anim_set_path_cb(&animation, lv_anim_path_ease_out);
            lv_anim_start(&animation);
        }
        else if (event_trigger == data->password_editor)
        {
            Debug_Log("password_editor trigger short_click_event");
            lv_keyboard_set_textarea(data->keyboard, event_trigger);
            lv_anim_t animation;
            lv_anim_init(&animation);
            lv_anim_set_exec_cb(&animation, (lv_anim_exec_xcb_t)lv_obj_set_y);
            lv_anim_set_var(&animation, data->input_container);
            lv_anim_set_duration(&animation, 500);
            lv_anim_set_values(&animation, lv_obj_get_y_aligned(data->input_container), 0);
            lv_anim_set_path_cb(&animation, lv_anim_path_ease_out);
            lv_anim_start(&animation);
        }
        else if (event_trigger == data->disconnect_button)
        {
            Debug_Log("disconnect_button trigger short_click_event");
            system("ps | grep \"wpa_supplicant\" | awk '{print $1}' | head - n1 | xargs kill");
        }
        else if (event_trigger == data->connect_button)
        {
            Debug_Log("connect_button trigger short_click_event");
            if (false == wifi_status)
            {
                char command_buffer[512];
                sprintf(command_buffer,
                        "echo \"ctrl_interface=/var/run/wpa_supplicant\n"
                        "ap_scan=1\nupdate_config=1\nnetwork={\n"
                        "    ssid=\\\"%s\\\"\n    psk=\\\"%s\\\"\n"
                        "    key_mgmt=WPA-PSK\n}\" > /etc/wpa_supplicant.conf",
                        lv_textarea_get_text(data->name_editor),
                        lv_textarea_get_text(data->password_editor));
                Debug_Log("name_editor: " << lv_textarea_get_text(data->name_editor));
                Debug_Log("password_editor: " << lv_textarea_get_text(data->password_editor));
                system(command_buffer);
                system("wpa_supplicant -B -i wlan0 -c /etc/wpa_supplicant.conf");
            }
        }
        else
        {
            Debug_Log("screen or container trigger short_click_event");
            lv_keyboard_set_textarea(data->keyboard, nullptr);
            lv_anim_t animation;
            lv_anim_init(&animation);
            lv_anim_set_exec_cb(&animation, (lv_anim_exec_xcb_t)lv_obj_set_y);
            lv_anim_set_var(&animation, data->input_container);
            lv_anim_set_duration(&animation, 500);
            lv_anim_set_values(&animation, lv_obj_get_y_aligned(data->input_container),
                               lv_obj_get_height(data->input_container));
            lv_anim_set_path_cb(&animation, lv_anim_path_ease_out);
            lv_anim_start(&animation);
        }

        Debug_Log("click");
    }

    gui *gui::get_unique()
    {
        if (nullptr != gui::unique)
        {
            Debug_Log("the unique of gui exists one");
            return gui::unique;
        }
        return (gui::unique = new gui());
    }

    void gui::initialize(const string &fbdev, const string &evdev)
    {
        lv_init();
        Debug_Log("initialize lvgl successfully");

        try
        {
            this->display = lv_linux_fbdev_create();
            if (nullptr == display)
            {
                Debug_Error("failed to create display from fbdev");
                throw error(error::error_enum::Create);
            }
            lv_linux_fbdev_set_file(display, fbdev.data());
            lv_display_set_rotation(display, LV_DISPLAY_ROTATION_0);
            Debug_Log("create display successfully");

            this->indev = lv_evdev_create(LV_INDEV_TYPE_POINTER, evdev.data());
            if (nullptr == this->indev)
            {
                Debug_Error("failed to create indev from evdev");
                throw error(error::error_enum::Create);
            }
            Debug_Log("create indev successfully");
        }
        catch (error &error)
        {
            exit(error.get_type()); /* exit with error code */
        }

        /* deploy screen */
        this->screen = lv_screen_active();
        lv_obj_set_style_bg_color(this->screen, lv_color_hex(BACKGROUND_COLOR),
                                  LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_remove_flag(this->screen, LV_OBJ_FLAG_SCROLLABLE);
        lv_obj_add_event_cb(this->screen, short_click_event_callback, LV_EVENT_SHORT_CLICKED, this);
        lv_obj_update_layout(this->screen);
        Debug_Log("deploy screen");

        /* deploy container */
        this->container = lv_obj_create(this->screen);
        lv_obj_remove_style_all(this->container);
        lv_obj_set_size(this->container, lv_obj_get_width(this->screen) - 2 * CONTAINER_MARGIN,
                        lv_obj_get_height(this->screen) - 2 * CONTAINER_MARGIN);
        lv_obj_set_align(this->container, LV_ALIGN_CENTER);
        lv_obj_remove_flag(this->container, LV_OBJ_FLAG_SCROLLABLE);
        lv_obj_add_event_cb(this->container, short_click_event_callback, LV_EVENT_SHORT_CLICKED,
                            this);
        lv_obj_update_layout(this->screen);
        Debug_Log("deploy container");

        /* deploy bar */
        this->bar = lv_obj_create(this->container);
        lv_obj_set_size(this->bar, lv_obj_get_width(this->container), BAR_HEIGHT);
        lv_obj_set_align(this->bar, LV_ALIGN_TOP_MID);
        lv_obj_set_style_bg_color(this->bar, lv_color_hex(FOREGROUND_COLOR),
                                  LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_radius(this->bar, BAR_RADIUS, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_pad_hor(this->bar, BAR_PAD, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_remove_flag(this->bar, LV_OBJ_FLAG_SCROLLABLE);
        lv_obj_update_layout(this->screen);
        Debug_Log("deploy bar");

        /* deploy exit_button */
        this->exit_button = lv_button_create(this->bar);
        lv_obj_set_size(this->exit_button, EXIT_BUTTON_SIZE, EXIT_BUTTON_SIZE);
        lv_obj_set_align(this->exit_button, LV_ALIGN_LEFT_MID);
        lv_obj_set_style_bg_color(this->exit_button, lv_color_hex(0xFF0000),
                                  LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_image_src(this->exit_button, "assets/exit_button_image.png",
                                      LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_remove_flag(this->exit_button, LV_OBJ_FLAG_SCROLLABLE);
        lv_obj_add_event_cb(this->exit_button, short_click_event_callback, LV_EVENT_SHORT_CLICKED,
                            this);
        lv_obj_update_layout(this->screen);
        Debug_Log("deploy exit_button");

        /* deploy title_label */
        this->title_label = lv_label_create(this->bar);
        lv_obj_set_align(this->title_label, LV_ALIGN_LEFT_MID);
        lv_obj_set_pos(this->title_label, lv_obj_get_width(this->exit_button) + BAR_PAD, 0);
        lv_obj_set_style_text_font(this->title_label, &lv_font_custom_40,
                                   LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_text_letter_space(this->title_label, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_label_set_text(this->title_label, "WiFi设置");
        lv_obj_remove_flag(this->title_label, LV_OBJ_FLAG_SCROLLABLE);
        lv_obj_update_layout(this->screen);
        Debug_Log("deploy title_label");

        /* deploy status_led */
        this->status_led = lv_led_create(this->bar);
        lv_obj_set_size(this->status_led, EXIT_BUTTON_IMAGE_SIZE, EXIT_BUTTON_IMAGE_SIZE);
        lv_obj_set_align(this->status_led, LV_ALIGN_RIGHT_MID);
        lv_led_set_color(this->status_led, lv_color_hex(0xFF0000));
        lv_obj_update_layout(this->screen);
        Debug_Log("deploy status_led");

        /* deploy name_group */
        this->name_group = lv_obj_create(this->container);
        lv_obj_remove_style_all(this->name_group);
        lv_obj_set_size(this->name_group, lv_obj_get_width(this->container), LV_SIZE_CONTENT);
        lv_obj_set_align(this->name_group, LV_ALIGN_TOP_MID);
        lv_obj_set_pos(this->name_group, 0,
                       lv_obj_get_y(this->bar) + lv_obj_get_height(this->bar) + BAR_PAD);
        lv_obj_remove_flag(this->name_group, LV_OBJ_FLAG_SCROLLABLE);
        lv_obj_update_layout(this->screen);
        Debug_Log("deploy name_group");

        /* deploy name_editor */
        this->name_editor = lv_textarea_create(this->name_group);
        lv_obj_set_align(this->name_editor, LV_ALIGN_RIGHT_MID);
        lv_textarea_set_placeholder_text(this->name_editor, "");
        lv_textarea_set_one_line(this->name_editor, true);
        lv_obj_set_style_bg_color(this->name_editor, lv_color_hex(FOREGROUND_COLOR),
                                  LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_radius(this->name_editor, BAR_RADIUS, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_pad_hor(this->name_editor, BAR_PAD, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_text_font(this->name_editor, &lv_font_montserrat_30,
                                   LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_text_letter_space(this->name_editor, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_remove_flag(this->name_editor, LV_OBJ_FLAG_SCROLLABLE);
        lv_obj_add_event_cb(this->name_editor, short_click_event_callback, LV_EVENT_SHORT_CLICKED,
                            this);
        lv_obj_update_layout(this->screen);

        /* deploy name_item */
        this->name_item = lv_obj_create(this->name_group);
        lv_obj_set_size(this->name_item, LV_SIZE_CONTENT, lv_obj_get_height(this->name_editor));
        lv_obj_set_align(this->name_item, LV_ALIGN_LEFT_MID);
        lv_obj_set_style_radius(this->name_item, BAR_RADIUS, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_remove_flag(this->name_item, LV_OBJ_FLAG_SCROLLABLE);
        lv_obj_update_layout(this->screen);
        Debug_Log("deploy name_item");

        /* deploy name_label */
        this->name_label = lv_label_create(this->name_item);
        lv_obj_set_align(this->name_label, LV_ALIGN_CENTER);
        lv_obj_set_style_text_font(this->name_label, &lv_font_custom_30,
                                   LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_text_letter_space(this->name_label, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_label_set_text(this->name_label, "名称");
        lv_obj_remove_flag(this->name_label, LV_OBJ_FLAG_SCROLLABLE);
        lv_obj_update_layout(this->screen);
        Debug_Log("deploy name_label");

        /* deploy name_line */
        static lv_point_precise_t name_line_points[2];
        name_line_points[0].x = lv_obj_get_width(this->name_item) + (BAR_PAD / 2);
        name_line_points[0].y = BAR_RADIUS / 2;
        name_line_points[1].x = lv_obj_get_width(this->name_item) + (BAR_PAD / 2);
        name_line_points[1].y = lv_obj_get_height(this->name_item) - (BAR_RADIUS / 2);
        this->name_line = lv_line_create(this->name_group);
        lv_line_set_points(this->name_line, name_line_points, 2);
        lv_obj_set_style_line_width(this->name_line, LINE_WIDTH, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_line_color(this->name_line,
                                    lv_obj_get_style_border_color(this->name_item, LV_PART_MAIN),
                                    LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_remove_flag(this->name_line, LV_OBJ_FLAG_SCROLLABLE);
        lv_obj_update_layout(this->screen);
        Debug_Log("deploy name_line");

        /* deploy name_editor */
        lv_obj_set_width(this->name_editor, lv_obj_get_width(this->name_group) -
                                                lv_obj_get_width(this->name_item) - BAR_PAD);
        lv_obj_update_layout(this->screen);
        Debug_Log("deploy name_editor");

        /* deploy password_group */
        this->password_group = lv_obj_create(this->container);
        lv_obj_remove_style_all(this->password_group);
        lv_obj_set_size(this->password_group, lv_obj_get_width(this->container), LV_SIZE_CONTENT);
        lv_obj_set_align(this->password_group, LV_ALIGN_TOP_MID);
        lv_obj_set_pos(this->password_group, 0,
                       lv_obj_get_y(this->name_group) + lv_obj_get_height(this->name_group) +
                           BAR_PAD);
        lv_obj_remove_flag(this->password_group, LV_OBJ_FLAG_SCROLLABLE);
        lv_obj_update_layout(this->screen);
        Debug_Log("deploy password_group");

        /* deploy password_editor */
        this->password_editor = lv_textarea_create(this->password_group);
        lv_obj_set_align(this->password_editor, LV_ALIGN_RIGHT_MID);
        lv_textarea_set_placeholder_text(this->password_editor, "");
        lv_textarea_set_one_line(this->password_editor, true);
        lv_obj_set_style_bg_color(this->password_editor, lv_color_hex(FOREGROUND_COLOR),
                                  LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_radius(this->password_editor, BAR_RADIUS, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_pad_hor(this->password_editor, BAR_PAD, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_text_font(this->password_editor, &lv_font_montserrat_30,
                                   LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_text_letter_space(this->password_editor, 2,
                                           LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_remove_flag(this->password_editor, LV_OBJ_FLAG_SCROLLABLE);
        lv_obj_add_event_cb(this->password_editor, short_click_event_callback,
                            LV_EVENT_SHORT_CLICKED, this);
        lv_obj_update_layout(this->screen);

        /* deploy password_item */
        this->password_item = lv_obj_create(this->password_group);
        lv_obj_set_size(this->password_item, LV_SIZE_CONTENT,
                        lv_obj_get_height(this->password_editor));
        lv_obj_set_align(this->password_item, LV_ALIGN_LEFT_MID);
        lv_obj_set_style_radius(this->password_item, BAR_RADIUS, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_remove_flag(this->password_item, LV_OBJ_FLAG_SCROLLABLE);
        lv_obj_update_layout(this->screen);
        Debug_Log("deploy password_item");

        /* deploy password_label */
        this->password_label = lv_label_create(this->password_item);
        lv_obj_set_align(this->password_label, LV_ALIGN_CENTER);
        lv_obj_set_style_text_font(this->password_label, &lv_font_custom_30,
                                   LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_text_letter_space(this->password_label, 2,
                                           LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_label_set_text(this->password_label, "密码");
        lv_obj_remove_flag(this->password_label, LV_OBJ_FLAG_SCROLLABLE);
        lv_obj_update_layout(this->screen);
        Debug_Log("deploy password_label");

        /* deploy password_line */
        static lv_point_precise_t password_line_points[2];
        password_line_points[0].x = lv_obj_get_width(this->password_item) + (BAR_PAD / 2);
        password_line_points[0].y = BAR_RADIUS / 2;
        password_line_points[1].x = lv_obj_get_width(this->password_item) + (BAR_PAD / 2);
        password_line_points[1].y = lv_obj_get_height(this->password_item) - (BAR_RADIUS / 2);
        this->password_line = lv_line_create(this->password_group);
        lv_line_set_points(this->password_line, password_line_points, 2);
        lv_obj_set_style_line_width(this->password_line, LINE_WIDTH,
                                    LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_line_color(
            this->password_line, lv_obj_get_style_border_color(this->password_item, LV_PART_MAIN),
            LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_remove_flag(this->password_line, LV_OBJ_FLAG_SCROLLABLE);
        lv_obj_update_layout(this->screen);
        Debug_Log("deploy password_line");

        /* deploy password_editor */
        lv_obj_set_width(this->password_editor, lv_obj_get_width(this->password_group) -
                                                    lv_obj_get_width(this->password_item) -
                                                    BAR_PAD);
        lv_obj_update_layout(this->password_editor);
        Debug_Log("deploy password_editor");

        /* deploy button_group */
        this->button_group = lv_obj_create(this->container);
        lv_obj_set_size(this->button_group, lv_obj_get_width(this->container), 90);
        lv_obj_set_align(this->button_group, LV_ALIGN_TOP_MID);
        lv_obj_set_pos(this->button_group, 0,
                       lv_obj_get_y(this->password_group) +
                           lv_obj_get_height(this->password_group) + BAR_PAD);
        lv_obj_set_style_radius(this->button_group, BAR_RADIUS, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_pad_ver(this->button_group, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_pad_hor(this->button_group, BAR_PAD / 2, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_remove_flag(this->button_group, LV_OBJ_FLAG_SCROLLABLE);
        lv_obj_update_layout(this->screen);
        Debug_Log("deploy button_group");

        /* deploy disconnect_button */
        this->disconnect_button = lv_button_create(this->button_group);
        lv_obj_set_size(this->disconnect_button,
                        (lv_obj_get_width(this->button_group) - 2 * BAR_PAD - LINE_WIDTH) / 2,
                        lv_obj_get_height(this->button_group) - BAR_PAD);
        lv_obj_set_align(this->disconnect_button, LV_ALIGN_LEFT_MID);
        lv_obj_set_style_bg_color(this->disconnect_button, lv_color_hex(0xFF0000),
                                  LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_radius(this->disconnect_button, BAR_RADIUS - (BAR_PAD / 2),
                                LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_remove_flag(this->disconnect_button, LV_OBJ_FLAG_SCROLLABLE);
        lv_obj_add_event_cb(this->disconnect_button, short_click_event_callback,
                            LV_EVENT_SHORT_CLICKED, this);
        lv_obj_update_layout(this->screen);
        Debug_Log("deploy disconnect_button");

        /* deploy disconnect_label */
        this->disconnect_label = lv_label_create(this->disconnect_button);
        lv_obj_set_align(this->disconnect_label, LV_ALIGN_CENTER);
        lv_obj_set_style_text_font(this->disconnect_label, &lv_font_custom_30,
                                   LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_text_letter_space(this->disconnect_label, 2,
                                           LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_label_set_text(this->disconnect_label, "断开连接");
        lv_obj_remove_flag(this->disconnect_label, LV_OBJ_FLAG_SCROLLABLE);
        lv_obj_update_layout(this->screen);
        Debug_Log("deploy disconnect_label");

        /* deploy connect_button */
        this->connect_button = lv_button_create(this->button_group);
        lv_obj_set_size(this->connect_button, lv_obj_get_width(this->disconnect_button),
                        lv_obj_get_height(this->disconnect_button));
        lv_obj_set_align(this->connect_button, LV_ALIGN_RIGHT_MID);
        lv_obj_set_style_radius(this->connect_button, BAR_RADIUS - (BAR_PAD / 2),
                                LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_remove_flag(this->connect_button, LV_OBJ_FLAG_SCROLLABLE);
        lv_obj_add_event_cb(this->connect_button, short_click_event_callback,
                            LV_EVENT_SHORT_CLICKED, this);
        lv_obj_update_layout(this->screen);
        Debug_Log("deploy connect_button");

        /* deploy connect_label */
        this->connect_label = lv_label_create(this->connect_button);
        lv_obj_set_align(this->connect_label, LV_ALIGN_CENTER);
        lv_obj_set_style_text_font(this->connect_label, &lv_font_custom_30,
                                   LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_text_letter_space(this->connect_label, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_label_set_text(this->connect_label, "尝试连接");
        lv_obj_remove_flag(this->connect_label, LV_OBJ_FLAG_SCROLLABLE);
        lv_obj_update_layout(this->screen);
        Debug_Log("deploy connect_label");

        /* deploy button_line */
        static lv_point_precise_t button_line_points[2]{{0, 0}, {0, 0}};
        button_line_points[1].y = lv_obj_get_height(this->button_group) - BAR_PAD -
                                  lv_obj_get_style_radius(this->disconnect_button, LV_PART_MAIN);
        this->button_line = lv_line_create(this->button_group);
        lv_obj_set_align(this->button_line, LV_ALIGN_CENTER);
        lv_line_set_points(this->button_line, button_line_points, 2);
        lv_obj_set_style_line_width(this->button_line, LINE_WIDTH, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_line_color(this->button_line,
                                    lv_obj_get_style_border_color(this->button_group, LV_PART_MAIN),
                                    LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_remove_flag(this->button_line, LV_OBJ_FLAG_SCROLLABLE);
        lv_obj_update_layout(this->screen);
        Debug_Log("deploy button_line");

        /* deploy input container */
        this->input_container = lv_obj_create(this->screen);
        lv_obj_remove_style_all(this->input_container);
        lv_obj_set_size(this->input_container, lv_obj_get_width(this->screen),
                        (lv_obj_get_height(this->screen) / 3) + PANEL_HEIGHT);
        lv_obj_set_align(this->input_container, LV_ALIGN_BOTTOM_MID);
        lv_obj_remove_flag(this->input_container, LV_OBJ_FLAG_SCROLLABLE);
        lv_obj_update_layout(this->screen);
        Debug_Log("deploy input_container");

        /* deploy keyboard */
        this->keyboard = lv_keyboard_create(this->input_container);
        lv_obj_set_size(this->keyboard, lv_obj_get_width(this->input_container),
                        lv_obj_get_height(this->screen) / 3);
        lv_obj_set_align(this->keyboard, LV_ALIGN_BOTTOM_MID);
        lv_keyboard_set_textarea(this->keyboard, name_editor);
        lv_obj_add_flag(this->keyboard, LV_OBJ_FLAG_OVERFLOW_VISIBLE);
        lv_obj_update_layout(this->screen);
        Debug_Log("deploy keyboard");

        Debug_Notice("initialize gui successfully");

        // system("ifconfig wlan0 up");
        // system("wpa_supplicant -B -i wlan0 -c /etc/wpa_supplicant.conf");
        // system("wpa_cli -i wlan0 scan");
        // system("wpa_cli -i wlan0 scan_result");

        /* deploy timer */
        lv_timer_t *const timer = lv_timer_create(timer_callback, 500, this);
        lv_timer_set_repeat_count(timer, -1);
        lv_timer_reset(timer);
        Debug_Log("deploy timer");

        Debug_Notice("initialize wlan");
    }

    void gui::execute()
    {
        Debug_Notice("execute gui successfully");
        while (true)
        {
            usleep(lv_timer_handler()); /* handle lvgl */
        }
    }
} // namespace wifi