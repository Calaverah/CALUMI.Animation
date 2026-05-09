//Copyright © 2025-2026 Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com

#include "utilities/CALUMI_Json.h"

#include <iostream>
#include <ostream>
#include <ranges>

#include "internal/internalvectordef.h"

#include <string>
#include <typeinfo>
#include <unordered_map>
#include <variant>
#include <vector>

#include "internalplatform.h"

namespace CALUMI::Utilities
{
    std::string s_RemoveWhiteSpaces(std::string&& text)
    {
        std::string output;
        bool inString = false;

        for (const char c : text)
        {
            if (c == '"')
                inString = !inString;

            if (std::isspace(c))
            {
                if (inString)
                    output += c;
            }
            else
            {
                output += c;
            }
        }

        return output;
    }

    std::string s_GetScope(std::string&& text)
    {
        std::string output;

        if (text.empty())
            return output;

        int64_t scopeCount = 1;
        char scopeStart;
        char scopeEnd;
        switch (text.at(0))
        {
        case '{':
            {
                scopeStart = '{';
                scopeEnd = '}';
                break;
            }
        case '[':
            {
                scopeStart = '[';
                scopeEnd = ']';
                break;
            }
        default:
            {
                return output;
            }
        }


        for (uint64_t i = 1; i < text.length() && scopeCount > 0; ++i)
        {
            if (text.at(i) == scopeStart)
                scopeCount++;
            else if (text.at(i) == scopeEnd)
                scopeCount--;

            if (scopeCount == 0)
                break;

            output += text.at(i);
        }

        return output;
    }


    VECTORDEF(JsonArray, JsonValue);

    struct JsonValue::Impl
    {
        std::variant<char, std::string,
                     signed char, unsigned char,
                     short, unsigned short,
                     int, unsigned int,
                     long long, unsigned long long,
                     float, double,
                     bool,
                     JsonObject, JsonArray> m_value;
        std::string m_stringHolder = "";
    };

    JsonValue::JsonValue() : pImpl(new Impl()) {}

    JsonValue::JsonValue(bool value) : JsonValue()
    {
        pImpl->m_value = value;
    }

    JsonValue::JsonValue(float value) : JsonValue()
    {
        pImpl->m_value = value;
    }

    JsonValue::JsonValue(double value) : JsonValue()
    {
        pImpl->m_value = value;
    }

    JsonValue::JsonValue(const char* value) : JsonValue()
    {
        pImpl->m_value = value;
    }

    JsonValue::JsonValue(char value) : JsonValue()
    {
        pImpl->m_value = value;
    }

    JsonValue::JsonValue(signed char value) : JsonValue()
    {
        pImpl->m_value = value;
    }

    JsonValue::JsonValue(unsigned char value) : JsonValue()
    {
        pImpl->m_value = value;
    }

    JsonValue::JsonValue(short value) : JsonValue()
    {
        pImpl->m_value = value;
    }

    JsonValue::JsonValue(unsigned short value) : JsonValue()
    {
        pImpl->m_value = value;
    }

    JsonValue::JsonValue(int value) : JsonValue()
    {
        pImpl->m_value = value;
    }

    JsonValue::JsonValue(unsigned int value) : JsonValue()
    {
        pImpl->m_value = value;
    }

    JsonValue::JsonValue(long long value) : JsonValue()
    {
        pImpl->m_value = value;
    }

    JsonValue::JsonValue(unsigned long long value) : JsonValue()
    {
        pImpl->m_value = value;
    }

    JsonValue::JsonValue(const JsonObject& value) : JsonValue()
    {
        pImpl->m_value = value;
    }

    JsonValue::JsonValue(const JsonArray& value) : JsonValue()
    {
        pImpl->m_value = value;
    }

    JsonValue::JsonValue(const JsonValue& input) : JsonValue()
    {
        *this = input;
    }

    JsonValue::JsonValue(JsonValue&& input) noexcept : JsonValue()
    {
        *this = input;
    }

    JsonValue::~JsonValue()
    {
        if (pImpl)
        {
            delete pImpl;
            pImpl = nullptr;
        }
    }

    JsonValue::VarType JsonValue::varType() const
    {
        return static_cast<VarType>(pImpl->m_value.index());
    }

    bool JsonValue::toBool(const bool defaultValue, bool* ok) const
    {
        if (ok)
            *ok = true;

        try
        {
            if (varType() == VarType::Bool)
                return std::get<bool>(pImpl->m_value);

            const bool output = std::visit([]<typename typeT>(typeT&& arg) -> bool {
                using T = std::decay_t<typeT>;
                if constexpr (std::is_arithmetic_v<T>) {
                   return arg;
                }

                throw std::bad_cast();

               }, pImpl->m_value);
            return output;
        }
        catch ( const std::exception&)
        {
            if (varType() == VarType::String)
            {
                if (SCOMPARE(std::get<std::string>(pImpl->m_value).c_str(), "true") == 0)
                    return true;
                if (SCOMPARE(std::get<std::string>(pImpl->m_value).c_str(), "false") == 0)
                    return false;
            }
        }

        if (ok)
            *ok = false;
        return defaultValue;
    }

    float JsonValue::toFloat(const float defaultValue, bool* ok) const
    {
        if (ok)
            *ok = true;

        try
        {
            const float output = std::visit([]<typename typeT>(typeT&& arg) -> float {
                using T = std::decay_t<typeT>;
                if constexpr (std::is_arithmetic_v<T>) {
                   return arg;
                }

                throw std::bad_cast();

               }, pImpl->m_value);
            return output;

        }
        catch ( const std::exception&) {}

        if (ok)
            *ok = false;

        return defaultValue;
    }

    double JsonValue::toDouble(const double defaultValue, bool* ok) const
    {
        if (ok)
            *ok = true;

        try
        {
            const double output = std::visit([]<typename typeT>(typeT&& arg) -> double {
                using T = std::decay_t<typeT>;
                if constexpr (std::is_arithmetic_v<T>) {
                   return arg;
                }

                throw std::bad_cast();

               }, pImpl->m_value);
            return output;
        }
        catch ( const std::exception&) {}

        if (ok)
            *ok = false;
        return defaultValue;
    }

    const char* JsonValue::toString(bool* ok) const
    {
        if (ok)
            *ok = true;

        try
        {
            switch (varType())
            {
            case VarType::Char:
                {
                    pImpl->m_stringHolder = std::get<char>(pImpl->m_value);
                    return pImpl->m_stringHolder.c_str();
                }
            case VarType::String:
                return std::get<std::string>(pImpl->m_value).c_str();
            case VarType::Int8:
            case VarType::UInt8:
            case VarType::Int16:
            case VarType::UInt16:
            case VarType::Int32:
            case VarType::UInt32:
            case VarType::Int64:
                {
                     const long long output = std::visit([]<typename typeT>(typeT&& arg) -> long long {
                     using T = std::decay_t<typeT>;
                     if constexpr (std::is_arithmetic_v<T>) {
                        return arg;
                     }

                     throw std::bad_cast();

                    }, pImpl->m_value);
                    pImpl->m_stringHolder = std::to_string(output);
                    return pImpl->m_stringHolder.c_str();
                }
            case VarType::UInt64:
                {
                    const auto output = std::get<unsigned long long>(pImpl->m_value);
                    pImpl->m_stringHolder = std::to_string(output);
                    return pImpl->m_stringHolder.c_str();
                }
            case VarType::Float:
            case VarType::Double:
                {
                    const double output = std::visit([]<typename typeT>(typeT&& arg) -> double {
                    using T = std::decay_t<typeT>;
                    if constexpr (std::is_arithmetic_v<T>) {
                       return arg;
                    }

                    throw std::bad_cast();

                   }, pImpl->m_value);
                    pImpl->m_stringHolder = std::to_string(output);
                    return pImpl->m_stringHolder.c_str();
                }
            case VarType::Bool:
                {
                    pImpl->m_stringHolder = std::get<bool>(pImpl->m_value) ? "true" : "false";
                    return pImpl->m_stringHolder.c_str();
                }
            case VarType::JsonObject:
                break;
            case VarType::JsonArray:
                break;
            default:
                break;
            }

        }
        catch ( const std::exception&) {}

        if (ok)
            *ok = false;

        return "";
    }

    char JsonValue::toChar(const char defaultValue, bool* ok) const
    {
        if (ok)
            *ok = true;

        try
        {
            return std::get<char>(pImpl->m_value);
        }
        catch ( const std::exception&) {}

        if (ok)
            *ok = false;

        return defaultValue;
    }

    unsigned char JsonValue::toUInt8(const unsigned char defaultValue, bool* ok) const
    {
        if (ok)
            *ok = true;

        try
        {
            const unsigned char output = std::visit([]<typename typeT>(typeT&& arg) -> unsigned char {
                using T = std::decay_t<typeT>;
                if constexpr (std::is_arithmetic_v<T>) {
                   return arg;
                }

                throw std::bad_cast();

               }, pImpl->m_value);
            return output;
        }
        catch ( const std::exception&) {}

        if (ok)
            *ok = false;

        return defaultValue;
    }

    signed char JsonValue::toInt8(const signed char defaultValue, bool* ok) const
    {
        if (ok)
            *ok = true;

        try
        {
            const signed char output = std::visit([]<typename typeT>(typeT&& arg) -> signed char {
                using T = std::decay_t<typeT>;
                if constexpr (std::is_arithmetic_v<T>) {
                   return arg;
                }

                throw std::bad_cast();

               }, pImpl->m_value);
            return output;
        }
        catch ( const std::exception&) {}

        if (ok)
            *ok = false;

        return defaultValue;
    }

    unsigned short JsonValue::toUInt16(const unsigned short defaultValue, bool* ok) const
    {
        if (ok)
            *ok = true;

        try
        {
            const signed short output = std::visit([]<typename typeT>(typeT&& arg) -> signed short {
                using T = std::decay_t<typeT>;
                if constexpr (std::is_arithmetic_v<T>) {
                   return arg;
                }

                throw std::bad_cast();

               }, pImpl->m_value);
            return output;
        }
        catch ( const std::exception&) {}

        if (ok)
            *ok = false;

        return defaultValue;
    }

    short JsonValue::toInt16(const short defaultValue, bool* ok) const
    {
        if (ok)
            *ok = true;

        try
        {
            const short output = std::visit([]<typename typeT>(typeT&& arg) -> short {
                using T = std::decay_t<typeT>;
                if constexpr (std::is_arithmetic_v<T>) {
                   return arg;
                }

                throw std::bad_cast();

               }, pImpl->m_value);
            return output;
        }
        catch ( const std::exception&) {}

        if (ok)
            *ok = false;

        return defaultValue;
    }

    unsigned int JsonValue::toUInt(const unsigned int defaultValue, bool* ok) const
    {
        if (ok)
            *ok = true;

        try
        {
            const unsigned int output = std::visit([]<typename typeT>(typeT&& arg) -> unsigned int {
                using T = std::decay_t<typeT>;
                if constexpr (std::is_arithmetic_v<T>) {
                   return arg;
                }

                throw std::bad_cast();

               }, pImpl->m_value);
            return output;
        }
        catch ( const std::exception&) {}

        if (ok)
            *ok = false;

        return defaultValue;
    }

    int JsonValue::toInt(const int defaultValue, bool* ok) const
    {
        if (ok)
            *ok = true;

        try
        {
            const int output = std::visit([]<typename typeT>(typeT&& arg) -> int {
                using T = std::decay_t<typeT>;
                if constexpr (std::is_arithmetic_v<T>) {
                   return arg;
                }

                throw std::bad_cast();

               }, pImpl->m_value);
            return output;
        }
        catch ( const std::exception&) {}

        if (ok)
            *ok = false;

        return defaultValue;
    }

    unsigned long long JsonValue::toUInt64(const unsigned long long defaultValue, bool* ok) const
    {
        if (ok)
            *ok = true;

        try
        {
            const unsigned long long output = std::visit([]<typename typeT>(typeT&& arg) -> unsigned long long {
                using T = std::decay_t<typeT>;
                if constexpr (std::is_arithmetic_v<T>) {
                   return arg;
                }

                throw std::bad_cast();

               }, pImpl->m_value);
            return output;
        }
        catch ( const std::exception&) {}

        if (ok)
            *ok = false;

        return defaultValue;
    }

    long long JsonValue::toInt64(const long long defaultValue, bool* ok) const
    {
        if (ok)
            *ok = true;

        try
        {
            const long long output = std::visit([]<typename typeT>(typeT&& arg) -> long long {
                using T = std::decay_t<typeT>;
                if constexpr (std::is_arithmetic_v<T>) {
                   return arg;
                }

                throw std::bad_cast();

               }, pImpl->m_value);
            return output;
        }
        catch ( const std::exception&) {}

        if (ok)
            *ok = false;

        return defaultValue;
    }

    JsonObject JsonValue::toObject(bool* ok) const
    {
        if (ok)
            *ok = true;

        try
        {
            return std::get<JsonObject>(pImpl->m_value);
        }
        catch ( const std::exception&) {}

        if (ok)
            *ok = false;

        return JsonObject();
    }

    JsonArray JsonValue::toArray(bool* ok) const
    {
        if (ok)
            *ok = true;

        try
        {
            return std::get<JsonArray>(pImpl->m_value);
        }
        catch ( const std::exception&) {}

        if (ok)
            *ok = false;

        return JsonArray();
    }

    JsonValue& JsonValue::operator=(const JsonValue& input)
    {
        if (this != &input)
        {
            pImpl->m_value = input.pImpl->m_value;
        }
        return *this;
    }

    JsonValue& JsonValue::operator=(JsonValue&& input) noexcept
    {
        if (this != &input)
        {
            pImpl->m_value = input.pImpl->m_value;
        }
        return *this;
    }

    JsonValue JsonValue::Deserialize(const char* input)
    {
        const std::string text = s_RemoveWhiteSpaces(input);

        if (SCOMPARE(text.c_str(), "null") == 0 || text.empty())
            return JsonValue();

        if (SCOMPARE(text.c_str(), "true") == 0)
            return JsonValue(true);

        if (SCOMPARE(text.c_str(), "false") == 0)
            return JsonValue(false);

        if (*text.begin() == '{' && text.back() == '}')
            return JsonObject::Deserialize(s_GetScope(text.c_str()).c_str());

        if (*text.begin() == '[' && text.back() == ']')
            return DeserializeArray(s_GetScope(text.c_str()).c_str());

        if (*text.begin() == '"' && text.back() == '"' && text.size() > 2)
        {
            std::string sVal = text.substr(1, text.size());
            sVal.pop_back();
            return sVal.c_str();
        }

        if (*text.begin() == '\'' && text.back() == '\'')
        {
            if (text.size() > 2)
                return text.at(2);

            return '\0';
        }

        if (text.contains('.'))
        {
            try
            {
                const double dVal = std::stod(text.c_str());
                return dVal;
            }
            catch ( const std::exception&) {}
        }

        if (text.contains('-'))
        {
            try
            {
                const int64_t iVal = std::stoll(text.c_str());
                return iVal;
            }
            catch ( const std::exception&) {}
        }

        try
        {
            const uint64_t uVal = std::stoull(text.c_str());
            return uVal;
        }
        catch (const std::exception&)
        {}


        return JsonValue();
    }

    JsonArray JsonValue::DeserializeArray(const char* input)
    {
        JsonArray output;

        const std::string text = s_RemoveWhiteSpaces(input);

        uint64_t pos = 0;

        while (pos < text.size())
        {
            if (text[pos] == '{')
            {
                std::string subScope = s_GetScope(text.substr(pos).c_str());
                if (!subScope.empty())
                {
                    JsonValue subValue = JsonObject::Deserialize(subScope.c_str());
                    output.push_back(subValue);
                    pos += subScope.size() + 1;
                }
                pos++;
            }
            else if (text[pos] == '[')
            {
                std::string subScope = s_GetScope(text.substr(pos).c_str());
                if (!subScope.empty())
                {
                    JsonValue subValue = DeserializeArray(subScope.c_str());
                    output.push_back(subValue);
                    pos += subScope.size() + 1;
                }
                pos++;
            }
            else if (text[pos] != ',')
            {
                auto size = text.find(',', pos);
                size = size == std::string::npos ? std::string::npos : size - pos;
                std::string subScope = text.substr(pos, size);
                if (subScope.empty())
                    output.push_back(0);
                else
                {
                    JsonValue subValue = Deserialize(subScope.c_str());
                    output.push_back(subValue);
                    pos += subScope.size();
                    continue;
                }
            }

            pos++;
        }
        return output;
    }

    struct JsonObject::Impl
    {
        std::unordered_map<std::string, JsonValue> m_values;
    };

    JsonObject::JsonObject() : pImpl(new Impl()) {}

    JsonObject::~JsonObject()
    {
        if (pImpl)
        {
            delete pImpl;
            pImpl = nullptr;
        }
    }

    JsonObject::JsonObject(const JsonObject& input) : JsonObject()
    {
        *this = input;
    }

    JsonObject& JsonObject::operator=(const JsonObject& input)
    {
        if (this != &input)
        {
            pImpl->m_values = input.pImpl->m_values;
        }
        return *this;
    }

    JsonValue& JsonObject::operator[](const char* key) const
    {
        return pImpl->m_values[key];
    }

    bool JsonObject::contains(const char* key) const
    {
        return pImpl->m_values.contains(key);
    }

    StringList JsonObject::keys() const
    {
        const StringList output;
        output.reserve(pImpl->m_values.size());
        for (const auto& key : pImpl->m_values | std::views::keys)
        {
            output.push_back(key.c_str());
        }
        return output;
    }

    StringContainer JsonValue::serialize(const unsigned int indentOffset, const bool raw) const
    {
        const std::string indent = !raw ? "\t" : "";
        std::string newline = !raw ? "\n" : "";

        for (unsigned int i = 0; i < indentOffset; i++)
        {
            newline += indent;
        }

        std::string output;

        switch (varType())
            {
            case VarType::Char:
                output += std::format("{}", toChar()).c_str();
                break;
            case VarType::String:
                output += std::format(R"("{}")", toString()).c_str();
                break;
            case VarType::Int8:
                output += std::format("{}", toInt8()).c_str();
                break;
            case VarType::UInt8:
                output += std::format("{}", toUInt8()).c_str();
                break;
            case VarType::Int16:
                output += std::format("{}", toInt16()).c_str();
                break;
            case VarType::UInt16:
                output += std::format("{}", toUInt16()).c_str();
                break;
            case VarType::Int32:
                output += std::format("{}", toInt()).c_str();
                break;
            case VarType::UInt32:
                output += std::format("{}", toUInt()).c_str();
                break;
            case VarType::Int64:
                output += std::format("{}", toInt64()).c_str();
                break;
            case VarType::UInt64:
                output += std::format("{}", toUInt64()).c_str();
                break;
            case VarType::Float:
                output += std::format("{}", toFloat()).c_str();
                break;
            case VarType::Double:
                output += std::format("{}", toDouble()).c_str();
                break;
            case VarType::Bool:
                output += toBool() ? "true" : "false";
                break;
            case VarType::JsonObject:
                output += toObject().serialize(indentOffset + 1, raw).c_str();
                break;
            case VarType::JsonArray:
                {
                    output += "[";

                    uint64_t count = 0;
                    for (const auto arrayCopy = toArray(); const auto& entry : arrayCopy.pImpl->vector)
                    {
                        count++;
                        output += newline + indent + entry.serialize(indentOffset + 1, raw).c_str();

                        if (count < arrayCopy.pImpl->vector.size())
                            output += ",";
                    }
                    output += newline + "]";
                    break;
                }
            default:
                output += "null";
                break;
            }
        return output.c_str();
    }

    StringContainer JsonObject::serialize(const unsigned int indentOffset, const bool raw) const
    {
        const std::string indent = !raw ? "\t" : "";
        std::string newline = !raw ? "\n" : "";

        for (unsigned int i = 0; i < indentOffset; i++)
        {
            newline += indent;
        }


        std::string output = "{";
        uint64_t keyCount = 0;
        for (const auto& [key, value] : pImpl->m_values)
        {
            keyCount++;
            output += newline;

            if (!raw && indentOffset == 0)
                output += indent;

            output += std::format(R"("{}": )", key);

            output += value.serialize(indentOffset + 1, raw).c_str();

            if (keyCount < pImpl->m_values.size())
                output += ",";
        }

        if (!raw && indentOffset > 0)
            newline.pop_back();

        output += newline;
        output += "}";

        return output.c_str();
    }

    JsonObject JsonObject::Deserialize(const char* input)
    {
        JsonObject output;

        std::string text = s_RemoveWhiteSpaces(input);

        if (text.empty())
            return output;

        if (text.at(0) == '{')
            text = s_GetScope(text.c_str());

        uint64_t pos = 0;

        while (pos < text.size())
        {
            if (text[pos] == '}' || text[pos] == ']' || text[pos] == ',')
            {
                pos++;
                continue;
            }

            const uint64_t sPos = text[pos] == '"' ? pos + 1 : pos;
            const uint64_t ePos = text.find('"', sPos +1);
            const uint64_t splitPos = text.find(':', ePos);
             std::string key = text.substr(sPos, ePos - sPos);

            pos = splitPos + 1;

            if (text[pos] != ':')
            {
                if (text[pos] == '{')
                {
                    std::string subScope = s_GetScope(text.substr(pos).c_str());

                    if (!subScope.empty())
                    {
                        const JsonValue subValue = Deserialize(subScope.c_str());
                        output[key.c_str()] = subValue;
                        pos += subScope.size() + 2;
                    }
                    else
                        output[key.c_str()] = JsonObject();
                }
                else if (text[pos] == '[')
                {
                    std::string subScope = s_GetScope(text.substr(pos).c_str());

                    if (!subScope.empty())
                    {
                        const JsonValue subValue = JsonValue::DeserializeArray(subScope.c_str());
                        output[key.c_str()] = subValue;
                        pos += subScope.size() + 2;
                    }
                    else
                        output[key.c_str()] = JsonArray();
                }
                else if (text[pos] != ',')
                {
                    auto size = text.find(',', pos);
                    size = size == std::string::npos ? std::string::npos : size - pos;
                    std::string subScope = text.substr(pos, size);

                    if (!subScope.empty())
                    {
                        const JsonValue subValue = JsonValue::Deserialize(subScope.c_str());
                        output[key.c_str()] = subValue;
                        pos += subScope.size();
                    }
                    else
                        output[key.c_str()] = 0;

                }
            }

            pos++;
        }

        return output;
    }
}
