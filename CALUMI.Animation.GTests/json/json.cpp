#include <gtest/gtest.h>
#include "utilities/CALUMI_Json.h"

#define GTEST(x) TEST(JsonSuite, x)

using namespace CALUMI::Utilities;

GTEST(Base)
{
    JsonObject object;

    object["float"] = 0.112345f;
    object["double"] = 0.9900991199229933;
    object["bool"] = true;
    object["int"] = 8;
    object["uint"] = 9;
    object["string"] = "Hello World!";
    object["null"] = "null";

    EXPECT_EQ(object["float"].toFloat(), 0.112345f);
    EXPECT_EQ(object["double"].toDouble(), 0.9900991199229933);
    EXPECT_EQ(object["bool"].toBool(), true);
    EXPECT_EQ(object["int"].toInt(), 8);
    EXPECT_EQ(object["uint"].toInt(), 9);
    EXPECT_STREQ(object["string"].toString(), "Hello World!");
    EXPECT_EQ(object["null"].toInt(-6), -6);
}

GTEST(Deserialization)
{
    {
        const auto text = R"({"float":0.7071})";
        JsonObject object = JsonObject::Deserialize(text);

        bool ok = false;
        const auto value = object["float"].toFloat(0.0f, &ok);

        EXPECT_TRUE(ok);
        EXPECT_FLOAT_EQ(value, 0.7071);
    }
}

GTEST(ArrayDeserialization)
{
    const auto text = R"({"array":[{"float":0.1},{"float":0.2},{"float":0.3}]})";
    const JsonObject object = JsonObject::Deserialize(text);

    bool ok = false;

    const auto array = object["array"].toArray(&ok);
    EXPECT_TRUE(ok);
    EXPECT_EQ(array.size(), 3);

    EXPECT_FLOAT_EQ(array.at(0).toObject()["float"].toFloat(), 0.1);
    EXPECT_FLOAT_EQ(array.at(1).toObject()["float"].toFloat(), 0.2);
    EXPECT_FLOAT_EQ(array.at(2).toObject()["float"].toFloat(), 0.3);
}