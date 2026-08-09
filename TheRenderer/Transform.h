#pragma once
#include <DirectXMath.h>
#include <algorithm>
#include <cmath>
#include <string>
#include <iostream>
using namespace DirectX;
class Vector3
{
public:
    float x;
    float y;
    float z;

    Vector3() : x(0.0f), y(0.0f), z(0.0f) {}
    Vector3(float xVal, float yVal, float zVal) : x(xVal), y(yVal), z(zVal) {}
    XMFLOAT3 ToXmFloat3() noexcept;
    XMVECTOR ToXmVector() noexcept;
    static Vector3 Forward() noexcept {
        return Vector3(0, 0, 1);
    }
    static Vector3 Right() noexcept {
        return Vector3(1, 0, 0);
    }
    static Vector3 Up() noexcept {
        return Vector3(0, 1, 0);
    }
    static Vector3 FromXmVector(XMVECTOR dxVector) noexcept;

    Vector3& operator += (const Vector3& rhs) {
        this->x += rhs.x;
        this->y += rhs.y;
        this->z += rhs.z;
        return *this;
    }

    Vector3& operator *= (const float value) { // Passing float by value is faster than const float&
        this->x *= value;
        this->y *= value;
        this->z *= value;
        return *this; 
    }
    Vector3 operator * (const float& value)
    {
        return Vector3(this->x * value, this->y * value, this->z * value);
    }

    Vector3 operator + (const Vector3& rhs)
    {
        this->x += rhs.x;
        this->y += rhs.y;
        this->z += rhs.z;
        return *this;
    }
};

class Quaternion
{
public:
    float x;
    float y;
    float z;
    float w;

    Quaternion() : x(0.0f), y(0.0f), z(0.0f), w(1.0f) {}
    Quaternion(float xVal, float yVal, float zVal, float wVal)
        :
        x(xVal), y(yVal), z(zVal), w(wVal)
    {}

    // pitch = rotation about X, yaw = rotation about Y, roll = rotation about Z
    // (radians) -- same convention as DirectX::XMMatrixRotationRollPitchYaw,
    // which Camera.cpp and Node already use, so this stays consistent with
    // the rest of the engine rather than introducing a second convention.
    static Quaternion FromEuler(float pitch, float yaw, float roll) noexcept
    {
        const DirectX::XMVECTOR q =
            DirectX::XMQuaternionRotationRollPitchYaw(pitch, yaw, roll);

        DirectX::XMFLOAT4 stored;
        DirectX::XMStoreFloat4(&stored, q);
        return Quaternion(stored.x, stored.y, stored.z, stored.w);
    }
    DirectX::XMVECTOR ToXMVector() const noexcept
    {
        return DirectX::XMVectorSet(x, y, z, w);
    }
    Vector3 ToEuler() const noexcept
    {
        Vector3 e;

        // pitch (X)
        float sinp = 2.0f * (w * x - y * z);
        sinp = std::max(-1.0f, std::min(1.0f, sinp)); // clamp: guards asin() against
        // NaN from float error at the poles
        e.x = std::asin(sinp);

        // yaw (Y)
        e.y = std::atan2(2.0f * (w * y + x * z), 1.0f - 2.0f * (x * x + y * y));

        // roll (Z)
        e.z = std::atan2(2.0f * (w * z + x * y), 1.0f - 2.0f * (x * x + z * z));

        return e;
    }
};

class Transform
{
public:
    Transform() = default;
    Vector3 position;
    Vector3 scale = Vector3(1.0f, 1.0f, 1.0f);
    Quaternion rotation;
    Vector3 forward = Vector3::Forward();
    Vector3 right = Vector3::Right();
    Vector3 up = Vector3::Up();
    // --- setters -------------------------------------------------------
    void SetPosition(const Vector3& pos) noexcept
    {
        position = pos;
    }
    void SetPosition(float x, float y, float z) noexcept
    {
        position = Vector3(x, y, z);
    }

    void SetScale(const Vector3& s) noexcept
    {
        scale = s;
    }
    void SetScale(float x, float y, float z) noexcept
    {
        scale = Vector3(x, y, z);
    }
    void SetScale(float uniform) noexcept
    {
        scale = Vector3(uniform, uniform, uniform);
    }

    void SetRotation(const Quaternion& q) noexcept
    {
        rotation = q;
        computeLocalVectors();
    }
    // Convenience: set rotation directly from Euler angles (radians).
    void SetRotationEuler(float pitch, float yawAngle, float roll) noexcept
    {
        rotation = Quaternion::FromEuler(pitch, yawAngle, roll);
        computeLocalVectors();
    }

    Vector3 GetEuletRotation()
    {
        return rotation.ToEuler();
        //computeLocalVectors();
    }



    // --- matrix build ----------------------------------------------------
    // Composes Scale * Rotation * Translation, matching the SRT order
    // already used throughout Node/Mesh (e.g. XMMatrixScaling(...) *
    // XMMatrixRotation...(...) * XMMatrixTranslation(...)).
    DirectX::XMMATRIX GetMatrix() const noexcept
    {
        const DirectX::XMMATRIX matScale =
            DirectX::XMMatrixScaling(scale.x, scale.y, scale.z);
        const DirectX::XMMATRIX matRotate =
            DirectX::XMMatrixRotationQuaternion(rotation.ToXMVector());
        const DirectX::XMMATRIX matTranslate =
            DirectX::XMMatrixTranslation(position.x, position.y, position.z);

        return matScale * matRotate * matTranslate;
    }

private:
    void computeLocalVectors() noexcept;
};

class TransformWindow
{
public:
    void SpawnTransformWindow(Transform& transform, const char* transformName) noexcept;
};