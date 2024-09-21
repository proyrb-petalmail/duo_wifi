#pragma once

#include "json.hpp"

#include <string>

/* configor option */
#define BACKGROUND_IMAGE_PATH "background_image_path"

namespace wifi
{
    class context
    {
    private:
        static context *unique;
        std::string program_directory;
        std::string pipe_json_name;
        configor::json::value pipe_json;
        std::string gui_json_name;
        configor::json::value gui_json;
        context();
        ~context();

    public:
        /**
         * @brief get the unique context.
         * @return return the unique context pointer.
         * @version 1.0
         * @date 2024/9/19
         * @author ProYRB
         */
        static context *get_unique();

        /**
         * @brief get the directory of program.
         * @return return the directory of program.
         * @version 1.0
         * @date 2024/9/19
         * @author ProYRB
         */
        const std::string &get_program_directory();

        /**
         * @brief load the json file of gui.
         * @param gui_json the json file of gui.
         * @version 1.0
         * @date 2024/9/19
         * @author ProYRB
         */
        void load_gui_json(const std::string &gui_json_name);

        /**
         * @brief get the value of label of pipe json.
         * @param label the label of value.
         * @return return the json value.
         * @version 1.0
         * @date 2024/9/19
         * @author ProYRB
         */
        configor::json::value &get_pipe_value(const std::string &label);

        /**
         * @brief replace the value of pipe json.
         * @param label the label of value.
         * @param value the value to replace.
         * @version 1.0
         * @date 2024/9/19
         * @author ProYRB
         */
        void replace_pipe_value(const std::string &label, const std::string &value);

        /**
         * @brief get the value of label of gui json.
         * @param label the label of value.
         * @return return the json value.
         * @version 1.0
         * @date 2024/9/19
         * @author ProYRB
         */
        configor::json::value &get_gui_value(const std::string &label);

        /**
         * @brief replace the value of gui json.
         * @param label the label of value.
         * @param value the value to replace.
         * @version 1.0
         * @date 2024/9/19
         * @author ProYRB
         */
        void replace_gui_value(const std::string &label, const std::string &value);

        /**
         * @brief record the value of context.
         * @version 1.0
         * @date 2024/9/19
         * @author ProYRB
         */
        static void record();
    };
} // namespace wifi
