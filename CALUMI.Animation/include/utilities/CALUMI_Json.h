//Copyright © 2025-2026 Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com

// ReSharper disable CppNonExplicitConvertingConstructor
#pragma once

#include "CALUMI_Common.h"
#include "CALUMI_Utilities.h"

namespace CALUMI::Utilities
{
    class JsonObject;
    class JsonArray;

    class CALUMIANIMATION_API JsonValue
    {
    public:
        enum class VarType
        {
            Char, ///< 1 Byte Character
            String, ///< C Style String
            Int8, ///< 1 Byte Signed Value
            UInt8, ///< 1 Byte Unsigned Value
            Int16, ///< 2 Byte Signed Value
            UInt16, ///< 2 Byte Unsigned Value
            Int32, ///< 4 Byte Signed Value
            UInt32, ///< 4 Byte Unsigned Value
            Int64, ///< 8 Byte Signed Value
            UInt64, ///< 8 Byte Unsigned Value
            Float, ///< 4 Byte Floating Point Value
            Double, ///< 8 Byte Floating Point Value
            Bool, ///< Boolean Value
            JsonObject, ///< JsonObject as Value
            JsonArray ///< JsonArray as Value
        };

        JsonValue();
        JsonValue(bool value);
        JsonValue(float value);
        JsonValue(double value);
        JsonValue(const char* value);
        JsonValue(char value);
        JsonValue(signed char value);
        JsonValue(unsigned char value);
        JsonValue(short value);
        JsonValue(unsigned short value);
        JsonValue(int value);
        JsonValue(unsigned int value);
        JsonValue(long long value);
        JsonValue(unsigned long long value);
        JsonValue(const JsonObject& value);
        JsonValue(const JsonArray& value);
        JsonValue(const JsonValue& input);
        JsonValue(JsonValue&& input) noexcept;
        ~JsonValue();

        VarType varType() const;

        bool toBool(bool defaultValue = false, bool* ok = nullptr) const;
        float toFloat(float defaultValue = 0.0f, bool* ok = nullptr) const;
        double toDouble(double defaultValue = 0.0f, bool* ok = nullptr) const;
        const char* toString(bool* ok = nullptr) const;
        char toChar(char defaultValue = 0, bool* ok = nullptr) const;
        unsigned char toUInt8(unsigned char defaultValue = 0, bool* ok = nullptr) const;
        signed char toInt8(signed char defaultValue = 0, bool* ok = nullptr) const;
        unsigned short toUInt16(unsigned short defaultValue = 0, bool* ok = nullptr) const;
        short toInt16(short defaultValue = 0, bool* ok = nullptr) const;
        unsigned int toUInt(unsigned int defaultValue = 0, bool* ok = nullptr) const;
        int toInt(int defaultValue = 0, bool* ok = nullptr) const;
        unsigned long long toUInt64(unsigned long long defaultValue = 0, bool* ok = nullptr) const;
        long long toInt64(long long defaultValue = 0, bool* ok = nullptr) const;
        JsonObject toObject(bool* ok = nullptr) const;
        JsonArray toArray(bool* ok = nullptr) const;

        StringContainer serialize(unsigned int indentOffset = 0, bool raw = false) const;

        JsonValue& operator=(const JsonValue& input);
        JsonValue& operator=(JsonValue&& input) noexcept;

    private:
        struct Impl;
        Impl* pImpl;
    };

    VECTORDECF(JsonArray, JsonValue, friend class JsonObject; friend class JsonValue;);

    class CALUMIANIMATION_API JsonObject
    {

    public:
        JsonObject();
        ~JsonObject();
        JsonObject(const JsonObject& input);

        JsonObject& operator=(const JsonObject& input);
        JsonValue& operator[](const char* key) const;

        bool contains(const char* key) const;
        StringList keys() const;

        StringContainer serialize(unsigned int indentOffset = 0, bool raw = false) const;

    private:
        struct Impl;
        Impl* pImpl;
    };

}
