#ifndef DATE_TIME_HPP
#define DATE_TIME_HPP

namespace PandoraEX
{
    class DateTime
    {
    public:
        DateTime();
        ~DateTime();

        /// @brief Get the current tick.
        /// @return The current tick.
        static unsigned long long Tick();

        /// @brief Get the nanosecond.
        /// @return The nanosecond.
        static unsigned short Nanosecond();

        /// @brief Get the microsecond.
        /// @return The microsecond.
        static unsigned short Microsecond();

        /// @brief Get the millisecond.
        /// @return The millisecond.
        static unsigned short Millisecond();

        /// @brief Get the second.
        /// @return The second.
        static unsigned char Second();

        /// @brief Get the minute.
        /// @return The minute.
        static unsigned char Minute();

        /// @brief Get the hour.
        /// @return The hour.
        static unsigned char Hour();

        /// @brief Get the day of the month.
        /// @return The day of the month.
        static unsigned char Day();

        /// @brief Get the day of the week.
        /// @return The day of the week.
        static unsigned char DayOfWeek();

        /// @brief Get the day of the year.
        /// @return The day of the year.
        static unsigned short DayOfYear();

        /// @brief Get the month of the year.
        /// @return The month of the year.
        static unsigned char Month();

        /// @brief Get the year.
        /// @return The year.
        static unsigned short Year();
    };
};

#endif // DATE_TIME_HPP