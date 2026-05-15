#ifndef EDEN_IO_STREAM_READER_HPP
#define EDEN_IO_STREAM_READER_HPP

#include "core/object/managed_object.hpp"
#include "collections/list.hpp"
#include "io/file_stream.hpp"
#include "io/path.hpp"
#include "text/string.hpp"

namespace Eden::IO
{
    namespace Managed
    {
        class StreamReaderData : public virtual OBJECT
        {
        public:
            Eden::IO::FileStream m_Stream;
            bool m_LeaveOpen = false;
        };
    }

    class StreamReader : public MANAGEDOBJECT<StreamReader,Managed::StreamReaderData>
    {
    public:
        StreamReader();
        StreamReader(const Eden::IO::Path& path);
        StreamReader(const Eden::IO::FileStream& stream, bool leaveOpen = false);
        ~StreamReader();

        bool end_of_stream();
        int peek();
        int read();
        Eden::Text::String read_line();
        Eden::Text::String read_to_end();
        Collections::List<Eden::Text::String> read_all_lines();

        void close();

        Eden::IO::FileStream& base_stream();
        const Eden::IO::FileStream& base_stream() const;

        static StreamReader OpenText(const Eden::IO::Path& path);
        static Eden::Text::String ReadToEnd(const Eden::IO::Path& path);
        static Collections::List<Eden::Text::String> ReadAllLines(const Eden::IO::Path& path);
    };
}

#if defined(AUTO_USINGS)
using namespace Eden::IO;
#endif

#endif // EDEN_IO_STREAM_READER_HPP
