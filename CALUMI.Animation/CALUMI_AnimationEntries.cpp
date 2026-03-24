//Copyright © 2025-2026 Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com
#include "pch.h"
#include "CALUMI_AnimationEntries.h"
#include <algorithm>
#include <vector>
#include <string>

namespace CALUMI {
    namespace UNIV {

#pragma region TRANSLATION
        struct Translation::Impl
        {
            uint16_t frame = 0;
            CALUMI::Math::Vector3D translation;
        };
        uint16_t UNIV::Translation::Frame() const
        {
            return pImpl->frame;
        }
        void UNIV::Translation::Frame(uint16_t frame)
        {
            pImpl->frame = frame;
        }
        Math::Vector3D& UNIV::Translation::TranslationVector() const
        {
            return pImpl->translation;
        }
        UNIV::Translation::~Translation()
        {
            if (pImpl) delete pImpl;
        }
        UNIV::Translation::Translation()
        {
            pImpl = new Impl;
        }
        UNIV::Translation::Translation(const uint16_t& frame, const CALUMI::Math::Vector3D& translation) : Translation()
        {
            pImpl->frame = frame;
            pImpl->translation = translation;
        }
        UNIV::Translation::Translation(const Translation& input) : Translation()
        {
            *pImpl = *(input.pImpl);
        }
        bool Translation::IsIdentical(const Translation& input) const
        {
            return pImpl->frame == input.pImpl->frame &&
                pImpl->translation == input.pImpl->translation;
        }
        UNIV::Translation& UNIV::Translation::operator=(const UNIV::Translation& other) { *pImpl = *(other.pImpl); return *this; }

        bool operator<(const UNIV::Translation& A, const UNIV::Translation& B) { return A.Frame() < B.Frame(); }
        bool operator<=(const UNIV::Translation& A, const UNIV::Translation& B) { return A.Frame() <= B.Frame(); }
        bool operator>(const UNIV::Translation& A, const UNIV::Translation& B) { return A.Frame() > B.Frame(); }
        bool operator>=(const UNIV::Translation& A, const UNIV::Translation& B) { return A.Frame() >= B.Frame(); }
        bool operator==(const UNIV::Translation& A, const UNIV::Translation& B) { return A.Frame() == B.Frame(); }
        bool operator!=(const UNIV::Translation& A, const UNIV::Translation& B) { return A.Frame() != B.Frame(); }

        Utilities::StringContainer Translation::ToJSON(const size_t indents) const {
            std::string output(Utilities::Indent(indents + 1).c_str());
            output += ("{\"frame\": " + std::to_string(Frame()) + ", \"value\": [" + std::to_string(TranslationVector().getX()) + ", " + std::to_string(TranslationVector().getY()) + ", " + std::to_string(TranslationVector().getZ()) + "]}");
            return output.c_str();
        }
#pragma endregion

#pragma region ROTATION
        struct Rotation::Impl
        {
            uint16_t frame = 0;
            CALUMI::Math::Quaternion rotation;
            Impl() = default;
        };

        uint16_t Rotation::Frame() const
        {
            return pImpl->frame;
        }
        void Rotation::Frame(uint16_t frame)
        {
            pImpl->frame = frame;
        }
        Math::Quaternion& Rotation::RotationQuaternion() const
        {
            return pImpl->rotation;
        }
        Rotation::Rotation()
        {
            pImpl = new Impl;
        }
        Rotation::Rotation(const uint16_t& frame, const CALUMI::Math::Quaternion& rotation) : Rotation()
        {
            pImpl->frame = frame; 
            pImpl->rotation = rotation;
        }
        Rotation::Rotation(const Rotation& input) : Rotation()
        {
            *pImpl = *(input.pImpl);
        }
        Rotation::~Rotation()
        {
            if (pImpl) delete pImpl;
        }

        bool Rotation::IsIdentical(const Rotation& input) const
        {
            return pImpl->frame == input.pImpl->frame &&
                   pImpl->rotation == input.pImpl->rotation;
        }

        UNIV::Rotation& UNIV::Rotation::operator=(const UNIV::Rotation& other) { *pImpl = *(other.pImpl); return *this; }

        bool operator<(const UNIV::Rotation& A, const UNIV::Rotation& B)    { return A.Frame() < B.Frame(); }
        bool operator<=(const UNIV::Rotation& A, const UNIV::Rotation& B)   { return A.Frame() <= B.Frame();}
        bool operator>(const UNIV::Rotation& A, const UNIV::Rotation& B)    { return A.Frame() > B.Frame(); }
        bool operator>=(const UNIV::Rotation& A, const UNIV::Rotation& B)   { return A.Frame() >= B.Frame();}
        bool operator==(const UNIV::Rotation& A, const UNIV::Rotation& B)   { return A.Frame() == B.Frame();}
        bool operator!=(const UNIV::Rotation& A, const UNIV::Rotation& B)   { return A.Frame() != B.Frame();}

        Utilities::StringContainer Rotation::ToJSON(const size_t indents) const {
            std::string output(Utilities::Indent(indents).c_str());
            output += ("{\"frame\": " + std::to_string(pImpl->frame) + ", \"value\": [" + std::to_string(pImpl->rotation.getW()) + ", " + std::to_string(pImpl->rotation.getX()) + ", " + std::to_string(pImpl->rotation.getY()) + ", " + std::to_string(pImpl->rotation.getZ()) + "]}");
            return output.c_str();
        }
#pragma endregion

#pragma region SCALAR
        struct Scalar::Impl 
        {
            uint16_t frame = 0;
            float scalar = 1.0;
            Impl() = default;
        };
        uint16_t Scalar::Frame() const { return pImpl->frame; }
        void Scalar::Frame(uint16_t frame) { pImpl->frame = frame; }
        float Scalar::ScalarValue() const { return pImpl->scalar; }
        void Scalar::ScalarValue(float value) { pImpl->scalar = value; }

        Scalar::Scalar() { pImpl = new Impl; }
        Scalar::~Scalar() { if (pImpl) delete pImpl; }

        Scalar::Scalar(const uint16_t& frame, float scalar) : Scalar()
        {
            pImpl->frame = frame;
            pImpl->scalar = scalar;
        }
        Scalar::Scalar(const Scalar& input) : Scalar() { *pImpl = *(input.pImpl); }

        bool Scalar::IsIdentical(const Scalar& input) const
        {
            return pImpl->frame == input.pImpl->frame &&
                   pImpl->scalar == input.pImpl->scalar;
        }

        UNIV::Scalar& UNIV::Scalar::operator=(const UNIV::Scalar& other) { *pImpl = *(other.pImpl); return *this; }

        bool operator<(const UNIV::Scalar& A, const UNIV::Scalar& B)    { return A.Frame()<  B.Frame(); }
        bool operator<=(const UNIV::Scalar& A, const UNIV::Scalar& B)   { return A.Frame()<= B.Frame(); }
        bool operator>(const UNIV::Scalar& A, const UNIV::Scalar& B)    { return A.Frame()>  B.Frame(); }
        bool operator>=(const UNIV::Scalar& A, const UNIV::Scalar& B)   { return A.Frame()>= B.Frame(); }
        bool operator==(const UNIV::Scalar& A, const UNIV::Scalar& B)   { return A.Frame()== B.Frame(); }
        bool operator!=(const UNIV::Scalar& A, const UNIV::Scalar& B)   { return A.Frame()!= B.Frame(); }

        Utilities::StringContainer Scalar::ToJSON(const size_t indents) const {
            std::string output(Utilities::Indent(indents + 1).c_str());
            output += ("{\"frame\": " + std::to_string(pImpl->frame) + ", \"value\": " + std::to_string(pImpl->scalar) + "}").c_str();
            return output.c_str();
        }
#pragma endregion

#pragma region PRIORITY
        struct Priority::Impl
        {
            uint16_t frame = 0;
            uint8_t priority = 90;
            Impl() = default;
        };
        uint16_t Priority::Frame() const { return pImpl->frame; }
        void Priority::Frame(uint16_t frame) { pImpl->frame = frame; }
        uint8_t Priority::PriorityValue() const { return pImpl->priority; }
        void Priority::PriorityValue(uint8_t value) { pImpl->priority = value; }

        Priority::Priority() { pImpl = new Impl; }
        Priority::~Priority() { if (pImpl) delete pImpl; }
        Priority::Priority(const uint16_t& frame, const uint8_t& priority) : Priority() { pImpl->frame = frame; pImpl->priority = priority; }
        Priority::Priority(const Priority& input) : Priority() { *pImpl = *(input.pImpl);  }

        UNIV::Priority& UNIV::Priority::operator=(const UNIV::Priority& other) { pImpl->frame = other.pImpl->frame; pImpl->priority = other.pImpl->priority; return *this; }

        bool operator<(const UNIV::Priority& A, const UNIV::Priority& B)    { return A.Frame() <  B.Frame(); }
        bool operator<=(const UNIV::Priority& A, const UNIV::Priority& B)   { return A.Frame() <= B.Frame(); }
        bool operator>(const UNIV::Priority& A, const UNIV::Priority& B)    { return A.Frame() >  B.Frame(); }
        bool operator>=(const UNIV::Priority& A, const UNIV::Priority& B)   { return A.Frame() >= B.Frame(); }
        bool operator==(const UNIV::Priority& A, const UNIV::Priority& B)   { return A.Frame() == B.Frame(); }
        bool operator!=(const UNIV::Priority& A, const UNIV::Priority& B)   { return A.Frame() != B.Frame(); }

        Utilities::StringContainer Priority::ToJSON(const size_t indents) const {
            std::string output(Utilities::Indent(indents + 1).c_str());
            output += ("{\"frame\": " + std::to_string(pImpl->frame) + ", \"value\": " + std::to_string(pImpl->priority) + "}");
            return output.c_str();
        }
#pragma endregion

#pragma region EXTERN"C"

        Rotation* CreateRotationEntryC(uint16_t frame, float x, float y, float z, float w)
        {
            Rotation* outputRotation = new Rotation(frame, {x,y,z,w});
            outputRotation->RotationQuaternion().Normalize();
            return outputRotation;
        }
        Rotation* CreateRotationEntryFromEulerC(uint16_t frame, float x, float y, float z, uint8_t order)
        {
            Math::Quaternion::EulerOrder eOrder = order > static_cast<uint8_t>(Math::Quaternion::EulerOrder::Max) ? Math::Quaternion::EulerOrder::XYZ : static_cast<Math::Quaternion::EulerOrder>(order);

            Rotation* outputRotation = new Rotation(frame, { x,y,z,order });
            outputRotation->RotationQuaternion().Normalize();
            return outputRotation;
        }
        bool DeleteRotationEntryC(Rotation* ptr)
        {
            if (ptr)
            {
                delete ptr;
                ptr = nullptr;
                return true;
            }
            return false;
        }
        Translation* CreateTranslationEntryC(uint16_t frame, double x, double y, double z)
        {
            Translation* outputTranslation = new Translation(frame, {x,y,z});

            return outputTranslation;
        }
        bool DeleteTranslationEntryC(Translation* ptr)
        {
            if (ptr)
            {
                delete ptr;
                ptr = nullptr;
                return true;
            }
            return false;
        }
        Scalar* CreateScalarEntryC(uint16_t frame, float scalar)
        {
            Scalar* outputScalar = new Scalar(frame, scalar);
            return outputScalar;
        }
        bool DeleteScalarEntryC(Scalar* ptr)
        {
            if (ptr)
            {
                delete ptr;
                ptr = nullptr;
                return true;
            }
            return false;
        }
        Priority* CreatePriorityEntryC(uint16_t frame, uint8_t priority)
        {
            Priority* outputPriority = new Priority(frame, priority);
            return outputPriority;
        }
        bool DeletePriorityEntryC(Priority* ptr)
        {
            if (ptr)
            {
                delete ptr;
                ptr = nullptr;
                return true;
            }
            return false;
        }

        uint16_t GetFrameFromRotationEntryC(Rotation* source)
        {
            return source->Frame();
        }

        uint16_t GetFrameFromTranslationEntryC(Translation* source)
        {
            return source->Frame();
        }

        uint16_t GetFrameFromScalarEntryC(Scalar* source)
        {
            return source->Frame();
        }

        uint16_t GetFrameFromPriorityEntryC(Priority* source)
        {
            return source->Frame();
        }

        Math::Quaternion* GetValueFromRotationEntryC(Rotation* source)
        {
            return &source->RotationQuaternion();
        }

        Math::Vector3D* GetValueFromTranslationEntryC(Translation* source)
        {
            return &source->TranslationVector();
        }

        float GetValueFromScalarEntryC(Scalar* source)
        {
            return source->ScalarValue();
        }

        uint8_t GetValueFromPriorityEntryC(Priority* source)
        {
            return source->PriorityValue();
        }
#pragma endregion        

}
}



