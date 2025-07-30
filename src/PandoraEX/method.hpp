#ifndef PANDORAEX_METHOD_HPP
#define PANDORAEX_METHOD_HPP

/*
[&epsilon, zeta] captures epsilon by reference and zeta by value
[&] captures all variables used in the lambda by reference
[=] captures all variables used in the lambda by value
[&, epsilon] captures all variables used in the lambda by reference but captures epsilon by value
[=, &epsilon] captures all variables used in the lambda by value but captures epsilon by reference

*/

#include <functional>
#include <any>
#include <iostream>

#include "exception.hpp"
#include "../PandoraDebug/console.hpp"

namespace PandoraEX
{
    /// @brief A class that represents a method with a specific signature.
    /// @tparam ReturnType The return type of the method. Default is void.
    template <typename ReturnType = void>
    class Method
    {
    private:
        std::function<ReturnType()> __func;
        std::any __func_wo_args;

        int __arg_count;
        bool __has_stored_args = false;
        std::string __signature;

        /* REVIEW: Return type not needed
         *$ *===============================REVIEW==================================
         *$ * WHY: Enhancement
         *$ * DESCRIPTION: https://chatgpt.com/c/67a696dc-2b04-8002-a742-8fc621222b74
         *$ *=======================================================================
         *$ */

    public:
        /// @brief Gets the signature of the function.
        /// @details This function returns the signature of the function as a string.
        /// @tparam Func The type of the function.
        /// @return The signature of the function as a string.
        /// @note If the function is a lambda, it will return the type of the lambda's operator().
        template <typename Func>
        static String getSignature(Func &&)
        {
            // return PandoraEX::demangle(typeid(func).name());
            using function_type = std::decay_t<Func>;

            // Ha a Func egy lambda, akkor a function_type::operator() lesz a valódi függvény
            if constexpr (std::is_class_v<function_type>)
                return getSignature(&function_type::operator());
            else
                return typeid(function_type).name();
        }

        /// @brief Counts the number of parameters in a function.
        /// @tparam Func The type of the function.
        /// @return The number of parameters in the function.
        template <typename Func>
        static constexpr size_t paramCount(Func &&)
        {
            using function_type = std::decay_t<Func>;
            if constexpr (std::is_class_v<function_type> || !std::is_function_v<std::remove_pointer_t<std::decay_t<Func>>>)
            {
                return paramCount(&function_type::operator());
            }
            else
            {
                return std::tuple_size<typename std::function<function_type>::argument_type>::value;
            }
        }

        /// @brief Counts the number of parameters in a function pointer.
        /// @tparam Ret The return type of the function.
        /// @tparam Args The types of the parameters of the function.
        /// @return The number of parameters in the function pointer.
        template <typename Ret, typename... Args>
        static constexpr size_t paramCount(Ret (*)(Args...))
        {
            return sizeof...(Args);
        }

        /// @brief Counts the number of parameters in a member function pointer.
        /// @tparam ClassType The type of the class that contains the member function.
        /// @tparam Ret The return type of the member function.
        /// @tparam Args The types of the parameters of the member function.
        /// @return The number of parameters in the member function pointer.
        template <typename ClassType, typename Ret, typename... Args>
        static constexpr size_t paramCount(Ret (ClassType::*)(Args...) const)
        {
            return sizeof...(Args);
        }

        /// @brief Counts the number of parameters in a member function pointer.
        /// @tparam ClassType The type of the class that contains the member function.
        /// @tparam Ret The return type of the member function.
        /// @tparam Args The types of the parameters of the member function.
        /// @return The number of parameters in the member function pointer.
        template <typename ClassType, typename Ret, typename... Args>
        static constexpr size_t paramCount(Ret (ClassType::*)(Args...))
        {
            return sizeof...(Args);
        }

        /// @brief Default constructor for Method class.
        /// @details This constructor initializes the Method object with default values.
        /// @note It sets the function to an empty function, argument count to 0, and has_stored_args to false.
        Method() = default;

        /// @brief Copy constructor for Method class.

        Method(const Method &method)
        {
            
            __func = method.__func;
            __func_wo_args = method.__func_wo_args;
            __arg_count = method.__arg_count;
            __has_stored_args = method.__has_stored_args;
            __signature = method.__signature;
        }

        /// @brief Constructor for Method class that takes a function and its arguments.
        /// @tparam Func The type of the function.
        /// @tparam Args The types of the arguments.
        /// @param func The function to be stored in the Method object.
        /// @param args The arguments to be stored in the Method object.
        /// @throws `ArgumentCountException` - If the number of arguments does not match the expected count.
        template <typename Func, typename... Args,
          typename = std::enable_if_t<!std::is_same_v<std::decay_t<Func>, Method>>>
        Method(Func &&func, Args &&...args)
        {
            __arg_count = paramCount(func);
            __signature = Utils::demangle(getSignature(func).c_str());

            if (__arg_count >= 0 && sizeof...(Args) == 0)
            {
                if constexpr (std::is_function_v<std::remove_pointer_t<std::decay_t<Func>>>)
                {
                    __func_wo_args = function(std::forward<Func>(func));
                }
                else
                    __func_wo_args = std::function(func);
            }
            else if (__arg_count > 0 && sizeof...(Args) == __arg_count)
            {
                __func_wo_args = std::function(func);
                if constexpr (sizeof...(Args) != 0)
                    __func = [f = std::forward<Func>(func), tpl = std::make_tuple(std::forward<Args>(args)...)]() mutable -> ReturnType
                    {
                        return std::apply(f, tpl);
                    };
                __has_stored_args = true;
            }
            else if (__arg_count > 0 && sizeof...(Args) != __arg_count)
            {
                throw Exceptions::ArgumentCountException(__arg_count, sizeof...(Args), __FILE__, __LINE__);
            }
        }

        /// @brief Constructor for Method class that takes a function without arguments.
        /// @tparam Func The type of the function.
        /// @param func The function to be stored in the Method object.
        /// @returns A Method object that can be invoked with the stored function.
        template <typename Func>
        Method<void> operator=(Func &&func)
        {
            return Method<void>(std::forward<Func>(func));
        }

        /// @brief Invokes the stored function with the given arguments.
        /// @tparam Args The types of the arguments.
        /// @param args The arguments to be passed to the stored function.
        /// @returns The result of the stored function.
        /// @throws `ArgumentCountException` - If the number of arguments does not match the expected count.
        /// @throws `SignatureException` - If the signature of the function does not match the expected signature.
        template <typename... Args>
        ReturnType Invoke(Args &&...args)
        {
            if (__arg_count > (int)sizeof...(Args) && !__has_stored_args)
            {
                throw Exceptions::ArgumentCountException(__arg_count, sizeof...(Args), __FILE__, __LINE__);
            }
            if (!__has_stored_args)
            {
                if (__arg_count > (int)sizeof...(Args))
                {
                    throw Exceptions::ArgumentCountException(__arg_count, sizeof...(Args), __FILE__, __LINE__);
                }
                std::function<ReturnType()> *_funcPtr_wo_args = std::any_cast<std::function<ReturnType()>>(&__func_wo_args);
                std::function<ReturnType(Args...)> *funcPtr_w_args = std::any_cast<std::function<ReturnType(Args...)>>(&__func_wo_args);
                if (_funcPtr_wo_args && __arg_count == 0)
                {
                    if constexpr (sizeof...(Args) > 0)
                        std::cout << "WARNING > Argument count mismatch. Expected 0, got " << sizeof...(Args) << std::endl;
                    return (*_funcPtr_wo_args)();
                }
                if (funcPtr_w_args && __arg_count > 0)
                {
                    if ((int)sizeof...(Args) > __arg_count)
                        std::cout << "WARNING > Argument count mismatch. Expected " << __arg_count << ", got " << sizeof...(Args) << std::endl;
                    return (*funcPtr_w_args)(std::forward<Args>(args)...);
                }

                // signatureException(args...);
            }
            else if ((int)sizeof...(Args) < __arg_count && __has_stored_args)
            {
                if (sizeof...(Args) > 0)
                    std::cout << "WARNING > Argument count mismatch. Expected " << __arg_count << ", got " << sizeof...(Args) << std::endl;
                return __func();
            }
            else if ((int)sizeof...(Args) >= __arg_count && __has_stored_args)
            {
                std::function<ReturnType(Args...)> *funcPtr = std::any_cast<std::function<ReturnType(Args...)>>(&__func_wo_args);
                if (funcPtr && __arg_count > 0)
                {
                    if ((int)sizeof...(Args) > __arg_count)
                        std::cout << "WARNING > Argument count mismatch. Expected " << __arg_count << ", got " << sizeof...(Args) << std::endl;
                    return (*funcPtr)(std::forward<Args>(args)...);
                }
                // std::cout << "Signature mismatch." << std::endl;
            }
            throw Exceptions::SignatureException(__signature, __FILE__, __LINE__, args...);
            return ReturnType(); // or throw an exception if appropriate
        }

        /// @brief Invokes the stored function with the given arguments.
        /// @tparam Args The types of the arguments.
        /// @param args The arguments to be passed to the stored function.
        /// @returns The result of the stored function.
        /// @throws `ArgumentCountException` - If the number of arguments does not match the expected count.
        /// @throws `SignatureException` - If the signature of the function does not match the expected signature.
        template <typename... Args>
        ReturnType operator()(Args &&...args)
        {
            return Invoke(std::forward<Args>(args)...);
        }
    };
}

#endif // PANDORAEX_METHOD_HPP