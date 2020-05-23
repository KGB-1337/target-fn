#pragma once
#include "main.h"
#include <iomanip>

namespace Util
{
    namespace Vector
    {
        Vector3 Add(Vector3 point1, Vector3 point2)
        {
            Vector3 vector{ 0, 0, 0 };
            vector.x = point1.x + point2.x;
            vector.y = point1.y + point2.y;
            vector.z = point1.z + point2.z;
            return vector;
        }

        Vector3 Subtract(Vector3 point1, Vector3 point2)
        {
            Vector3 vector{ 0, 0, 0 };
            vector.x = point1.x - point2.x;
            vector.y = point1.y - point2.y;
            vector.z = point1.z - point2.z;
            return vector;
        }

        Vector3 Square(Vector3 vector)
        {
            return Vector3{ vector.x * vector.x, vector.y * vector.y, vector.z * vector.z };
        }
    }

    namespace Vector2D
    {
        FVector2D Subtract(FVector2D point1, FVector2D point2)
        {
            FVector2D vector{ 0, 0 };
            vector.X = point1.Y - point2.Y;
            vector.Y = point1.Y - point2.Y;
            return vector;
        }
    }

    std::string DistanceToString(float distance)
    {   
        auto meters = distance;
        std::stringstream ss;

        if (meters < 1000.0f)
        {
            ss << std::fixed << std::setprecision(0) << meters << _xor_("m");
        }
        else
        {
            ss << std::fixed << std::setprecision(0) << (meters / 1000.0f) << _xor_("km");
        }

        return ss.str();
    }

    float GetDistance(Vector3 x, Vector3 y)
    {
        auto z = Vector::Subtract(x, y);
        return sqrt(z.x * z.x + z.y * z.y + z.z * z.z);
    }

    float GetDistance2D(FVector2D point1, FVector2D point2)
    {
        FVector2D heading = Vector2D::Subtract(point2, point1);
        float distanceSquared;
        float distance;

        distanceSquared = heading.X * heading.X + heading.Y * heading.Y;
        distance = sqrt(distanceSquared);

        return distance;
    }

    float Normalize(float angle) {
        float a = (float)fmod(fmod(angle, 360.0) + 360.0, 360.0);
        if (a > 180.0f) {
            a -= 360.0f;
        }
        return a;
    }

    VOID CalcAngle(float* src, float* dst, float* angles) {
        float rel[3] = {
            dst[0] - src[0],
            dst[1] - src[1],
            dst[2] - src[2],
        };

        auto dist = sqrtf(rel[0] * rel[0] + rel[1] * rel[1] + rel[2] * rel[2]);
        auto yaw = atan2f(rel[1], rel[0]) * (180.0f / M_PI);
        auto pitch = (-((acosf((rel[2] / dist)) * 180.0f / M_PI) - 90.0f));

        angles[0] = Normalize(pitch);
        angles[1] = Normalize(yaw);
    }

    /*
    bool IsInFOV(DWORD_PTR m_Player, Vector3 position, float fov)
    {
        int screenSizeX, screenSizeY;
        m_Player->GetViewportSize(&screenSizeX, &screenSizeY);
        FVector2D centerScreen{ (float)screenSizeX / 2, (float)screenSizeY / 2 };
        FVector2D screenPos;
        if (Engine::WorldToScreen(m_Player, position, &screenPos))
        {
            float dist = GetDistance2D(centerScreen, screenPos);
            if (dist < fov)
                return true;
        }
        return false;
    }*/
}