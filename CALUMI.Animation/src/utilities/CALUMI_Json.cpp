//Copyright © 2025-2026 Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com

#include "utilities/CALUMI_Json.h"

#include <format>
#include <ranges>

#include "internal/internalvectordef.h"

#include <string>
#include <typeinfo>
#include <unordered_map>
#include <variant>
#include <vector>

namespace CALUMI::Utilities
{
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
            return std::get<bool>(pImpl->m_value);
        }
        catch ( const std::bad_cast&) {}

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
            return std::get<float>(pImpl->m_value);
        }
        catch ( const std::bad_cast&) {}

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
            return std::get<double>(pImpl->m_value);
        }
        catch ( const std::bad_cast&) {}

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
            return std::get<std::string>(pImpl->m_value).c_str();
        }
        catch ( const std::bad_cast&) {}

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
        catch ( const std::bad_cast&) {}

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
            return std::get<unsigned char>(pImpl->m_value);
        }
        catch ( const std::bad_cast&) {}

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
            return std::get<signed char>(pImpl->m_value);
        }
        catch ( const std::bad_cast&) {}

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
            return std::get<unsigned short>(pImpl->m_value);
        }
        catch ( const std::bad_cast&) {}

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
            return std::get<short>(pImpl->m_value);
        }
        catch ( const std::bad_cast&) {}

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
            return std::get<unsigned int>(pImpl->m_value);
        }
        catch ( const std::bad_cast&) {}

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
            return std::get<int>(pImpl->m_value);
        }
        catch ( const std::bad_cast&) {}

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
            return std::get<unsigned long long>(pImpl->m_value);
        }
        catch ( const std::bad_cast&) {}

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
            return std::get<long long>(pImpl->m_value);
        }
        catch ( const std::bad_cast&) {}

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
        catch ( const std::bad_cast&) {}

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
        catch ( const std::bad_cast&) {}

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
                output += toObject().serialize(indentOffset+1,raw).c_str();
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
                            output += ", ";
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
            output += std::format(R"("{}": )", key);

            output += value.serialize(indentOffset+1, raw).c_str();

            if (keyCount < pImpl->m_values.size())
                output += ",";
        }

        if (!raw && indentOffset > 0)
            newline.pop_back();

        output += newline;
        output += "}";

        return output.c_str();
    }
}
