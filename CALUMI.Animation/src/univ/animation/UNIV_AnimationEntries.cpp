//Copyright © 2025-2026 Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com

// ReSharper disable CppDFALocalValueEscapesFunction
#include "univ/animation/UNIV_AnimationEntries.h"
#include <algorithm>
#include <vector>
#include <string>
#include "internal/internalvectordef.h"

namespace CALUMI::UNIV
{

#pragma region TRANSLATION
    struct Translation::Impl
    {
        uint16_t frame = 0;
        Math::Vector3D translation;
    };
    uint16_t Translation::frame() const
    {
        return pImpl->frame;
    }
    void Translation::setFrame(const uint16_t frame) const
    {
        pImpl->frame = frame;
    }
    Math::Vector3D& Translation::translationVector() const
    {
        return pImpl->translation;
    }
    Translation::~Translation()
    {
        if (pImpl)
        {
            delete pImpl;
            pImpl = nullptr;
        }
    }
    Translation::Translation()
    {
        pImpl = new Impl;
    }
    Translation::Translation(const uint16_t& frame, const Math::Vector3D& translation) : Translation()
    {
        pImpl->frame = frame;
        pImpl->translation = translation;
    }
    Translation::Translation(const Translation& input) : Translation()
    {
        *this = input;
    }
    bool Translation::isIdentical(const Translation& input) const
    {
        return pImpl->frame == input.pImpl->frame &&
            pImpl->translation == input.pImpl->translation;
    }
    Translation& Translation::operator=(const Translation& other)
    {
        if (this != &other)
        {
            pImpl->frame = other.pImpl->frame;
            pImpl->translation = other.pImpl->translation;
        }
        return *this;
    }

    bool operator<(const Translation& A, const Translation& B) { return A.frame() < B.frame(); }
    bool operator<=(const Translation& A, const Translation& B) { return A.frame() <= B.frame(); }
    bool operator>(const Translation& A, const Translation& B) { return A.frame() > B.frame(); }
    bool operator>=(const Translation& A, const Translation& B) { return A.frame() >= B.frame(); }
    bool operator==(const Translation& A, const Translation& B) { return A.frame() == B.frame(); }
    bool operator!=(const Translation& A, const Translation& B) { return A.frame() != B.frame(); }

    Utilities::StringContainer Translation::toJSON(const uint64_t indents) const {
        std::string output(Utilities::Indent(indents + 1).c_str());
        output += "{\"frame\": " + std::to_string(frame()) + ", \"value\": [" + std::to_string(translationVector().x()) + ", " + std::to_string(translationVector().y()) + ", " + std::to_string(translationVector().z()) + "]}";
        return {output.c_str()};
    }
#pragma endregion

#pragma region ROTATION
    struct Rotation::Impl
    {
        uint16_t frame = 0;
        Math::Quaternion rotation;
        Impl() = default;
    };

    uint16_t Rotation::frame() const
    {
        return pImpl->frame;
    }
    void Rotation::setFrame(const uint16_t frame) const
    {
        pImpl->frame = frame;
    }
    Math::Quaternion& Rotation::rotationQuaternion() const
    {
        return pImpl->rotation;
    }
    Rotation::Rotation()
    {
        pImpl = new Impl;
    }
    Rotation::Rotation(const uint16_t& frame, const Math::Quaternion& rotation) : Rotation()
    {
        pImpl->frame = frame;
        pImpl->rotation = rotation;
    }
    Rotation::Rotation(const Rotation& input) : Rotation()
    {
        *this = input;
    }
    Rotation::~Rotation()
    {
        if (pImpl)
        {
            delete pImpl;
            pImpl = nullptr;
        }
    }

    bool Rotation::isIdentical(const Rotation& input) const
    {
        return pImpl->frame == input.pImpl->frame &&
            pImpl->rotation == input.pImpl->rotation;
    }

    Rotation& Rotation::operator=(const Rotation& other)
    {
        if (this != &other)
        {
            pImpl->frame = other.pImpl->frame;
            pImpl->rotation = other.pImpl->rotation;
        }
        return *this;
    }

    bool operator<(const Rotation& A, const Rotation& B)    { return A.frame() < B.frame(); }
    bool operator<=(const Rotation& A, const Rotation& B)   { return A.frame() <= B.frame();}
    bool operator>(const Rotation& A, const Rotation& B)    { return A.frame() > B.frame(); }
    bool operator>=(const Rotation& A, const Rotation& B)   { return A.frame() >= B.frame();}
    bool operator==(const Rotation& A, const Rotation& B)   { return A.frame() == B.frame();}
    bool operator!=(const Rotation& A, const Rotation& B)   { return A.frame() != B.frame();}

    Utilities::StringContainer Rotation::toJSON(const uint64_t indents) const {
        std::string output(Utilities::Indent(indents).c_str());
        output += "{\"frame\": " + std::to_string(pImpl->frame) + ", \"value\": [" + std::to_string(pImpl->rotation.w()) + ", " + std::to_string(pImpl->rotation.x()) + ", " + std::to_string(pImpl->rotation.y()) + ", " + std::to_string(pImpl->rotation.z()) + "]}";
        return {output.c_str()};
    }
#pragma endregion

#pragma region SCALAR
    struct Scalar::Impl
    {
        uint16_t frame = 0;
        float scalar = 1.0;
        Impl() = default;
    };
    uint16_t Scalar::frame() const { return pImpl->frame; }
    void Scalar::setFrame(const uint16_t frame) const { pImpl->frame = frame; }
    float Scalar::scalarValue() const { return pImpl->scalar; }
    void Scalar::setScalarValue(const float value) const { pImpl->scalar = value; }

    Scalar::Scalar() { pImpl = new Impl; }
    Scalar::~Scalar()
    {
        if (pImpl)
        {
            delete pImpl;
            pImpl = nullptr;
        }
    }

    Scalar::Scalar(const uint16_t& frame, const float scalar) : Scalar()
    {
        pImpl->frame = frame;
        pImpl->scalar = scalar;
    }
    Scalar::Scalar(const Scalar& input) : Scalar()
    {
        *this = input;
    }

    bool Scalar::isIdentical(const Scalar& input) const
    {
        return pImpl->frame == input.pImpl->frame &&
            pImpl->scalar == input.pImpl->scalar;
    }

    Scalar& Scalar::operator=(const Scalar& other)
    {
        if (this != &other)
        {
            pImpl->frame = other.pImpl->frame;
            pImpl->scalar = other.pImpl->scalar;
        }
        return *this;
    }

    bool operator<(const Scalar& A, const Scalar& B)    { return A.frame()<  B.frame(); }
    bool operator<=(const Scalar& A, const Scalar& B)   { return A.frame()<= B.frame(); }
    bool operator>(const Scalar& A, const Scalar& B)    { return A.frame()>  B.frame(); }
    bool operator>=(const Scalar& A, const Scalar& B)   { return A.frame()>= B.frame(); }
    bool operator==(const Scalar& A, const Scalar& B)   { return A.frame()== B.frame(); }
    bool operator!=(const Scalar& A, const Scalar& B)   { return A.frame()!= B.frame(); }

    Utilities::StringContainer Scalar::toJSON(const uint64_t indents) const {
        std::string output(Utilities::Indent(indents + 1).c_str());
        output += "{\"frame\": " + std::to_string(pImpl->frame) + ", \"value\": " + std::to_string(pImpl->scalar) + "}";
        return {output.c_str()};
    }
#pragma endregion

#pragma region PRIORITY
    struct Priority::Impl
    {
        uint16_t frame = 0;
        uint8_t priority = 90;
        Impl() = default;
    };
    uint16_t Priority::frame() const { return pImpl->frame; }
    void Priority::setFrame(const uint16_t frame) const { pImpl->frame = frame; }
    uint8_t Priority::priorityValue() const { return pImpl->priority; }
    void Priority::setPriorityValue(const uint8_t value) const { pImpl->priority = value; }

    Priority::Priority() { pImpl = new Impl; }
    Priority::~Priority()
    {
        if (pImpl)
        {
            delete pImpl;
            pImpl = nullptr;
        }
    }
    Priority::Priority(const uint16_t& frame, const uint8_t& priority) : Priority() { pImpl->frame = frame; pImpl->priority = priority; }
    Priority::Priority(const Priority& input) : Priority()
    {
        *this = input;
    }

    Priority& Priority::operator=(const Priority& other)
    {
        if (this != &other)
        {
            pImpl->frame = other.pImpl->frame;
            pImpl->priority = other.pImpl->priority;
        }
        return *this;
    }
    bool operator<(const Priority& A, const Priority& B)    { return A.frame() <  B.frame(); }
    bool operator<=(const Priority& A, const Priority& B)   { return A.frame() <= B.frame(); }
    bool operator>(const Priority& A, const Priority& B)    { return A.frame() >  B.frame(); }
    bool operator>=(const Priority& A, const Priority& B)   { return A.frame() >= B.frame(); }
    bool operator==(const Priority& A, const Priority& B)   { return A.frame() == B.frame(); }
    bool operator!=(const Priority& A, const Priority& B)   { return A.frame() != B.frame(); }

    Utilities::StringContainer Priority::toJSON(const uint64_t indents) const {
        std::string output(Utilities::Indent(indents + 1).c_str());
        output += "{\"frame\": " + std::to_string(pImpl->frame) + ", \"value\": " + std::to_string(pImpl->priority) + "}";
        return {output.c_str()};
    }
#pragma endregion

#pragma region EXTERN"C"

    Rotation* CreateRotationEntryC(const uint16_t frame, float x, float y, float z, float w)
    {
        const auto outputRotation = new Rotation(frame, {x,y,z,w});
        outputRotation->rotationQuaternion().normalize();

        return outputRotation;
    }
    Rotation* CreateRotationEntryFromEulerC(const uint16_t frame, float x, float y, float z, const uint8_t order)
    {
        auto eOrder = Math::EulerDefinition::GetEulerOrder(order);

        const auto outputRotation = new Rotation(frame, { x,y,z,eOrder });
        outputRotation->rotationQuaternion().normalize();

        return outputRotation;
    }
    bool DeleteRotationEntryC(const Rotation* ptr)
    {
        if (ptr)
        {
            delete ptr;
            ptr = nullptr;
            return true;
        }
        return false;
    }
    Translation* CreateTranslationEntryC(const uint16_t frame, double x, double y, double z)
    {
        const auto outputTranslation = new Translation(frame, {x,y,z});

        return outputTranslation;
    }
    bool DeleteTranslationEntryC(const Translation* ptr)
    {
        if (ptr)
        {
            delete ptr;
            ptr = nullptr;
            return true;
        }
        return false;
    }
    Scalar* CreateScalarEntryC(const uint16_t frame, const float scalar)
    {
        const auto outputScalar = new Scalar(frame, scalar);

        return outputScalar;
    }
    bool DeleteScalarEntryC(const Scalar* ptr)
    {
        if (ptr)
        {
            delete ptr;
            ptr = nullptr;
            return true;
        }
        return false;
    }
    Priority* CreatePriorityEntryC(const uint16_t frame, const uint8_t priority)
    {
        const auto outputPriority = new Priority(frame, priority);

        return outputPriority;
    }
    bool DeletePriorityEntryC(const Priority* ptr)
    {
        if (ptr)
        {
            delete ptr;
            ptr = nullptr;
            return true;
        }
        return false;
    }

    uint16_t GetFrameFromRotationEntryC(const Rotation* source)
    {
        return source->frame();
    }

    uint16_t GetFrameFromTranslationEntryC(const Translation* source)
    {
        return source->frame();
    }

    uint16_t GetFrameFromScalarEntryC(const Scalar* source)
    {
        return source->frame();
    }

    uint16_t GetFrameFromPriorityEntryC(const Priority* source)
    {
        return source->frame();
    }

    Math::Quaternion* GetValueFromRotationEntryC(const Rotation* source)
    {
        return &source->rotationQuaternion();
    }

    Math::Vector3D* GetValueFromTranslationEntryC(const Translation* source)
    {
        return &source->translationVector();
    }

    float GetValueFromScalarEntryC(const Scalar* source)
    {
        return source->scalarValue();
    }

    uint8_t GetValueFromPriorityEntryC(const Priority* source)
    {
        return source->priorityValue();
    }
#pragma endregion

    VECTORDEF(TranslationSequence, Translation)
    VECTORDEF(RotationSequence, Rotation)
    VECTORDEF(ScalarSequence, Scalar)
    VECTORDEF(PrioritySequence, Priority)

    void SortTranslationSequence(const TranslationSequence &sq, const bool highToLow)
    {
        if(sq.pImpl->vector.size() < 2)
            return;

        if (highToLow)
        {
            std::ranges::sort(sq.pImpl->vector, std::greater());
        }
        else
        {
            std::ranges::sort(sq.pImpl->vector);
        }
    }

    void SortRotationSequence(const RotationSequence& sq, const bool highToLow)
    {
        if(sq.pImpl->vector.size() < 2)
            return;

        if (highToLow)
        {
            std::ranges::sort(sq.pImpl->vector, std::greater());
        }
        else
        {
            std::ranges::sort(sq.pImpl->vector);
        }
    }

    void SortScalarSequence(const ScalarSequence& sq, const bool highToLow)
    {
        if(sq.pImpl->vector.size() < 2)
            return;

        if (highToLow)
        {
            std::ranges::sort(sq.pImpl->vector, std::greater());
        }
        else
        {
            std::ranges::sort(sq.pImpl->vector);
        }
    }

    void SortPrioritySequence(const PrioritySequence& sq, const bool highToLow)
    {
        if(sq.pImpl->vector.size() < 2)
            return;

        if (highToLow)
        {
            std::ranges::sort(sq.pImpl->vector, std::greater());
        }
        else
        {
            std::ranges::sort(sq.pImpl->vector);
        }
    }

}



