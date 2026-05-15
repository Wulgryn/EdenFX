#ifndef EDEN_IO_STREAM_WRITER_HPP
#define EDEN_IO_STREAM_WRITER_HPP

#include "core/object/managed_object.hpp"
#include "collections/list.hpp"
#include "io/file_stream.hpp"
#include "io/path.hpp"
#include "text/string.hpp"

namespace Eden::IO
{
    namespace Managed
    {
        class StreamWriterData : public virtual OBJECT
        {
        public:
            Eden::IO::FileStream m_Stream;
            bool m_LeaveOpen = false;
            bool m_AutoFlush = false;
            Eden::Text::String m_NewLine = "\n";
        };
    }

    class StreamWriter : public MANAGEDOBJECT<StreamWriter, Managed::StreamWriterData>
    {
    public:
        StreamWriter();
        StreamWriter(const Eden::IO::Path& path, bool append = false);
        StreamWriter(const Eden::IO::FileStream& stream, bool leaveOpen = false);
        ~StreamWriter();

        bool auto_flush() const;
        void set_auto_flush(bool value);

        Eden::Text::String new_line() const;
        void set_new_line(const Eden::Text::String& value);

        void write(const Eden::Text::String& text);
        void write(char value);
        void write_line();
        void write_line(const Eden::Text::String& text);
        void write_lines(const Collections::List<Eden::Text::String>& lines);

        void flush();
        void close();

        Eden::IO::FileStream& base_stream();
        const Eden::IO::FileStream& base_stream() const;

        static StreamWriter CreateText(const Eden::IO::Path& path);
        static StreamWriter AppendText(const Eden::IO::Path& path);
    };
}

#if defined(AUTO_USINGS)
using namespace Eden::IO;
#endif

#endif // EDEN_IO_STREAM_WRITER_HPP
