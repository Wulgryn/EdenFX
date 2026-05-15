#include "io/stream_reader.hpp"

#include <sstream>

namespace Eden::IO
{
    StreamReader::StreamReader() = default;

    StreamReader::StreamReader(const Eden::IO::Path& path)
    {
        this->data()->m_Stream = Eden::IO::FileStream(path, FileMode::Open, FileAccess::Read);
        this->data()->m_LeaveOpen = false;
    }

    StreamReader::StreamReader(const Eden::IO::FileStream& stream, bool leaveOpen)
    {
        this->data()->m_Stream = stream;
        this->data()->m_LeaveOpen = leaveOpen;
    }

    StreamReader::~StreamReader()
    {
        if (getRefCount() <= 1 && !this->data()->m_LeaveOpen)
        {
            close();
        }
    }

    bool StreamReader::end_of_stream()
    {
        if (!this->data()->m_Stream.is_open())
        {
            return true;
        }

        return this->data()->m_Stream.native().peek() == EOF;
    }

    int StreamReader::peek()
    {
        if (!this->data()->m_Stream.is_open())
        {
            return EOF;
        }

        return this->data()->m_Stream.native().peek();
    }

    int StreamReader::read()
    {
        return this->data()->m_Stream.read();
    }

    Eden::Text::String StreamReader::read_line()
    {
        if (!this->data()->m_Stream.is_open())
        {
            return Eden::Text::String();
        }

        std::string line;

        if (std::getline(this->data()->m_Stream.native(), line))
        {
            return Eden::Text::String(line);
        }

        return Eden::Text::String();
    }

    Eden::Text::String StreamReader::read_to_end()
    {
        if (!this->data()->m_Stream.is_open())
        {
            return Eden::Text::String();
        }

        std::ostringstream builder;
        builder << this->data()->m_Stream.native().rdbuf();
        return Eden::Text::String(builder.str());
    }

    Collections::List<Eden::Text::String> StreamReader::read_all_lines()
    {
        Collections::List<Eden::Text::String> result;

        while (!end_of_stream())
        {
            result.add(read_line());
        }

        return result;
    }

    void StreamReader::close()
    {
        this->data()->m_Stream.close();
    }

    Eden::IO::FileStream& StreamReader::base_stream()
    {
        return this->data()->m_Stream;
    }

    const Eden::IO::FileStream& StreamReader::base_stream() const
    {
        return this->data()->m_Stream;
    }

    StreamReader StreamReader::OpenText(const Eden::IO::Path& path)
    {
        return StreamReader(path);
    }

    Eden::Text::String StreamReader::ReadToEnd(const Eden::IO::Path& path)
    {
        StreamReader reader(path);
        return reader.read_to_end();
    }

    Collections::List<Eden::Text::String> StreamReader::ReadAllLines(const Eden::IO::Path& path)
    {
        StreamReader reader(path);
        return reader.read_all_lines();
    }
}
