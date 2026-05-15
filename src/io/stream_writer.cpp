#include "io/stream_writer.hpp"

namespace Eden::IO
{
    StreamWriter::StreamWriter() = default;

    StreamWriter::StreamWriter(const Eden::IO::Path& path, bool append)
    {
        this->data()->m_Stream = Eden::IO::FileStream(path, append ? FileMode::Append : FileMode::Create, FileAccess::Write);
        this->data()->m_LeaveOpen = false;
    }

    StreamWriter::StreamWriter(const Eden::IO::FileStream& stream, bool leaveOpen)
    {
        this->data()->m_Stream = stream;
        this->data()->m_LeaveOpen = leaveOpen;
    }

    StreamWriter::~StreamWriter()
    {
        if (getRefCount() <= 1)
        {
            flush();

            if (!this->data()->m_LeaveOpen)
            {
                close();
            }
        }
    }

    bool StreamWriter::auto_flush() const
    {
        return this->data()->m_AutoFlush;
    }

    void StreamWriter::set_auto_flush(bool value)
    {
        this->data()->m_AutoFlush = value;
    }

    Eden::Text::String StreamWriter::new_line() const
    {
        return this->data()->m_NewLine;
    }

    void StreamWriter::set_new_line(const Eden::Text::String& value)
    {
        this->data()->m_NewLine = value;
    }

    void StreamWriter::write(const Eden::Text::String& text)
    {
        this->data()->m_Stream.write(text);

        if (this->data()->m_AutoFlush)
        {
            flush();
        }
    }

    void StreamWriter::write(char value)
    {
        this->data()->m_Stream.write(Eden::Text::String(value));

        if (this->data()->m_AutoFlush)
        {
            flush();
        }
    }

    void StreamWriter::write_line()
    {
        write(this->data()->m_NewLine);
    }

    void StreamWriter::write_line(const Eden::Text::String& text)
    {
        write(text);
        write_line();
    }

    void StreamWriter::write_lines(const Collections::List<Eden::Text::String>& lines)
    {
        for (int i = 0; i < lines.count(); i++)
        {
            write_line(lines[i]);
        }
    }

    void StreamWriter::flush()
    {
        this->data()->m_Stream.flush();
    }

    void StreamWriter::close()
    {
        this->data()->m_Stream.close();
    }

    Eden::IO::FileStream& StreamWriter::base_stream()
    {
        return this->data()->m_Stream;
    }

    const Eden::IO::FileStream& StreamWriter::base_stream() const
    {
        return this->data()->m_Stream;
    }

    StreamWriter StreamWriter::CreateText(const Eden::IO::Path& path)
    {
        return StreamWriter(path, false);
    }

    StreamWriter StreamWriter::AppendText(const Eden::IO::Path& path)
    {
        return StreamWriter(path, true);
    }
}
