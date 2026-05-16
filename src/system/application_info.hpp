#ifndef EDEN_SYSTEM_APPLICATION_INFO_HPP
#define EDEN_SYSTEM_APPLICATION_INFO_HPP

#include "core/object/managed_object.hpp"
#include "io/path.hpp"
#include "text/string.hpp"

namespace Eden::System
{
    namespace Managed
    {
        class ApplicationInfoData : public virtual OBJECT
        {
        public:
            bool m_Found = false;
            Eden::Text::String m_Name;
            Eden::Text::String m_DisplayName;
            Eden::IO::Path m_Path;
            Eden::Text::String m_Source;
            bool m_IsDirectory = false;
        };
    }

    class ApplicationInfo : public MANAGEDOBJECT<ApplicationInfo, Managed::ApplicationInfoData>
    {
    public:
        ApplicationInfo();
        ApplicationInfo(
            bool found,
            const Eden::Text::String& name,
            const Eden::IO::Path& path,
            const Eden::Text::String& source = "",
            bool isDirectory = false,
            const Eden::Text::String& displayName = "");

        bool found() const;
        explicit operator bool() const;

        Eden::Text::String name() const;
        Eden::Text::String display_name() const;
        Eden::Text::String source() const;
        const Eden::IO::Path& path() const;
        bool is_directory() const;
        bool is_file() const;

        void set_found(bool value);
        void set_name(const Eden::Text::String& value);
        void set_display_name(const Eden::Text::String& value);
        void set_path(const Eden::IO::Path& value);
        void set_source(const Eden::Text::String& value);
        void set_is_directory(bool value);

        static ApplicationInfo NotFound(const Eden::Text::String& name);
    };
}

#if defined(AUTO_USINGS)
using namespace Eden::System;
#endif

#endif // EDEN_SYSTEM_APPLICATION_INFO_HPP
