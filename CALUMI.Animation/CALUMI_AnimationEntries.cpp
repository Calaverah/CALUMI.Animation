#include "pch.h"
#include "CALUMI_AnimationEntries.h"
#include <algorithm>
#include <vector>
#include <string>

namespace CALUMI {
    namespace UNIV {

        UNIV::Translation& UNIV::Translation::operator=(const UNIV::Translation& other) { frame = other.frame; translation = other.translation; return *this; }
        UNIV::Rotation& UNIV::Rotation::operator=(const UNIV::Rotation& other) { frame = other.frame; rotation = other.rotation; return *this; }
        UNIV::Scalar& UNIV::Scalar::operator=(const UNIV::Scalar& other) { frame = other.frame; scalar = other.scalar; return *this; }
        UNIV::Priority& UNIV::Priority::operator=(const UNIV::Priority& other) { frame = other.frame; priority = other.priority; return *this; }


        bool operator<(const UNIV::Translation& A, const UNIV::Translation& B) { return A.frame < B.frame; }
        bool operator<=(const UNIV::Translation& A, const UNIV::Translation& B) { return A.frame <= B.frame; }
        bool operator>(const UNIV::Translation& A, const UNIV::Translation& B) { return A.frame > B.frame; }
        bool operator>=(const UNIV::Translation& A, const UNIV::Translation& B) { return A.frame >= B.frame; }
        bool operator==(const UNIV::Translation& A, const UNIV::Translation& B) { return A.frame == B.frame; }
        bool operator!=(const UNIV::Translation& A, const UNIV::Translation& B) { return A.frame != B.frame; }

        bool operator<(const UNIV::Rotation& A, const UNIV::Rotation& B) { return A.frame < B.frame; }
        bool operator<=(const UNIV::Rotation& A, const UNIV::Rotation& B) { return A.frame <= B.frame; }
        bool operator>(const UNIV::Rotation& A, const UNIV::Rotation& B) { return A.frame > B.frame; }
        bool operator>=(const UNIV::Rotation& A, const UNIV::Rotation& B) { return A.frame >= B.frame; }
        bool operator==(const UNIV::Rotation& A, const UNIV::Rotation& B) { return A.frame == B.frame; }
        bool operator!=(const UNIV::Rotation& A, const UNIV::Rotation& B) { return A.frame != B.frame; }

        bool operator<(const UNIV::Scalar& A, const UNIV::Scalar& B) { return A.frame < B.frame; }
        bool operator<=(const UNIV::Scalar& A, const UNIV::Scalar& B) { return A.frame <= B.frame; }
        bool operator>(const UNIV::Scalar& A, const UNIV::Scalar& B) { return A.frame > B.frame; }
        bool operator>=(const UNIV::Scalar& A, const UNIV::Scalar& B) { return A.frame >= B.frame; }
        bool operator==(const UNIV::Scalar& A, const UNIV::Scalar& B) { return A.frame == B.frame; }
        bool operator!=(const UNIV::Scalar& A, const UNIV::Scalar& B) { return A.frame != B.frame; }

        bool operator<(const UNIV::Priority& A, const UNIV::Priority& B) { return A.frame < B.frame; }
        bool operator<=(const UNIV::Priority& A, const UNIV::Priority& B) { return A.frame <= B.frame; }
        bool operator>(const UNIV::Priority& A, const UNIV::Priority& B) { return A.frame > B.frame; }
        bool operator>=(const UNIV::Priority& A, const UNIV::Priority& B) { return A.frame >= B.frame; }
        bool operator==(const UNIV::Priority& A, const UNIV::Priority& B) { return A.frame == B.frame; }
        bool operator!=(const UNIV::Priority& A, const UNIV::Priority& B) { return A.frame != B.frame; }


        //Extern C functions
        Rotation* CreateRotationEntryC(uint16_t frame, float x, float y, float z, float w)
        {
            Rotation* outputRotation = new Rotation(frame, {x,y,z,w});
            outputRotation->rotation.Normalize();
            return outputRotation;
        }
        bool DeleteRotationEntryC(Rotation* ptr)
        {
            if (ptr)
            {
                delete ptr;
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
                return true;
            }
            return false;
        }

        uint16_t GetFrameFromEntryC(void* source)
        {
            uint16_t output;
            std::memcpy(&output, source, sizeof(uint16_t));
            return output;
        }

        Math::Quaternion* GetValueFromRotationEntryC(Rotation* source)
        {
            return &source->rotation;
        }

        Math::Vector3D* GetValueFromTranslationEntryC(Translation* source)
        {
            return &source->translation;
        }

        float GetValueFromScalarEntryC(Scalar* source)
        {
            return source->scalar;
        }

        uint8_t GetValueFromPriorityEntryC(Priority* source)
        {
            return source->priority;
        }

        Utilities::StringContainer Rotation::ToJSON(const size_t indents) const {
            std::string output(Utilities::Indent(indents).c_str());
            output += ("{\"frame\": " + std::to_string(frame) + ", \"value\": [" + std::to_string(rotation.w) + ", " + std::to_string(rotation.x) + ", " + std::to_string(rotation.y) + ", " + std::to_string(rotation.z) + "]}");
            return output.c_str();
        }

        Utilities::StringContainer Translation::ToJSON(const size_t indents) const {
            std::string output(Utilities::Indent(indents + 1).c_str());
            output += ("{\"frame\": " + std::to_string(frame) + ", \"value\": [" + std::to_string(translation.x) + ", " + std::to_string(translation.y) + ", " + std::to_string(translation.z) + "]}");
            return output.c_str();
        }

        Utilities::StringContainer Scalar::ToJSON(const size_t indents) const {
            std::string output(Utilities::Indent(indents + 1).c_str());
            output += ("{\"frame\": " + std::to_string(frame) + ", \"value\": " + std::to_string(scalar) + "}").c_str();
            return output.c_str();
        }

        Utilities::StringContainer Priority::ToJSON(const size_t indents) const {
            std::string output(Utilities::Indent(indents + 1).c_str());
            output += ("{\"frame\": " + std::to_string(frame) + ", \"value\": " + std::to_string(priority) + "}");
            return output.c_str();
        }


        

} }



