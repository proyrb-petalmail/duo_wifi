#include "context.hpp"
#include "debug.hpp"
#include "error.hpp"
#include "main.hpp"

#include <fstream>
#include <unistd.h>

using namespace std;
using namespace configor;

char *file_system_directory = nullptr; /* directory of file system */

namespace wifi
{
    context *context::unique = context::get_unique();

    context::context()
    {
        Debug_Log("the unique of context generation");
        try
        {
            /* get directory of program */
            char temporary_buffer[256] = "";
            if (-1 == readlink("/proc/self/exe", temporary_buffer, 256))
            {
                Debug_Error("failed to read link");
                throw error(error::error_enum::Read);
            }
            this->program_directory = temporary_buffer;
            this->program_directory.erase(this->program_directory.find_last_of("/") + 1,
                                          this->program_directory.length() -
                                              this->program_directory.find_last_of("/") + 1);
            Debug_Log("program_directory:" << this->program_directory);

            /* get directory of file system */
            string temporary_string(this->program_directory + "%s");
            file_system_directory = new char[temporary_string.length() + 1];
            if (temporary_string.copy(file_system_directory, temporary_string.length() + 1, 0) <
                temporary_string.length())
            {
                Debug_Error("failed to copy string");
                throw error(error::error_enum::Copy);
            }
            Debug_Log("file_system_directory:" << file_system_directory);

            atexit(record); /* register function at exit */
        }
        catch (error &error)
        {
            exit(error.get_type()); /* exit with error code */
        }
        Debug_Notice("initialize context successfully");
    }

    context::~context() {}

    context *context::get_unique()
    {
        if (nullptr != context::unique)
        {
            Debug_Log("the unique of context exists one");
            return context::unique;
        }
        return (context::unique = new context());
    }

    const string &context::get_program_directory() { return this->program_directory; }

    void context::load_gui_json(const std::string &gui_json_name)
    {
        this->gui_json_name = gui_json_name;
        ifstream infile_stream(this->gui_json_name); /* open gui json */
        if (false == infile_stream.is_open())
        {
            Debug_Error("failed to open gui json");
            throw error(error::error_enum::Open);
        }
        infile_stream >> json::wrap(this->gui_json); /* read gui json */
        Debug_Log(json::dump(this->gui_json, {json::serializer::with_indent(4, ' ')}));
        infile_stream.close();
        Debug_Notice("load gui json file successfully");
    }

    configor::json::value &context::get_pipe_value(const std::string &label)
    {
        return this->pipe_json[label.data()];
    }

    void context::replace_pipe_value(const std::string &label, const std::string &value)
    {
        if (!this->pipe_json[label.data()].is_string())
        {
            Debug_Error(label.data() << "is not a string");
            throw error(error::error_enum::Type);
        }
        this->pipe_json[label.data()] = value;
        Debug_Notice("replace the value of pipe successfully");
    }

    configor::json::value &context::get_gui_value(const std::string &label)
    {
        return this->gui_json[label.data()];
    }

    void context::replace_gui_value(const std::string &label, const std::string &value)
    {
        if (!this->gui_json[label.data()].is_string())
        {
            Debug_Error(label.data() << "is not a string");
            throw error(error::error_enum::Type);
        }
        this->gui_json[label.data()] = value;
        Debug_Notice("replace the value of gui successfully");
    }

    void context::record()
    {
        ofstream outfile_stream(context::unique->pipe_json_name); /* open pipe json */
        if (false == outfile_stream.is_open())
        {
            Debug_Error("failed to open pipe json");
            throw error(error::error_enum::Open);
        }
        outfile_stream << setw(4) << json::wrap(context::unique->pipe_json); /* read pipe json */
        outfile_stream.close();
        Debug_Notice("record pipe successfully");

        outfile_stream.open(context::unique->gui_json_name); /* open gui json */
        if (false == outfile_stream.is_open())
        {
            Debug_Error("failed to open gui json");
            throw error(error::error_enum::Open);
        }
        outfile_stream << setw(4) << json::wrap(context::unique->gui_json); /* read gui json */
        outfile_stream.close();
        Debug_Notice("record gui successfully");
    }
} // namespace wifi
