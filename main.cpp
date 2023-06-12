#include "rlImGui.h"
#include "Math.h"
#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

bool CheckCollisionLineCircle(Vector2 lineStart, Vector2 lineEnd, Vector2 circlePosition, float circleRadius)
{
    Vector2 nearest = NearestPoint(lineStart, lineEnd, circlePosition);
    return DistanceSqr(nearest, circlePosition) <= circleRadius * circleRadius;
}

struct Rigidbody
{
    Vector2 pos{};
    Vector2 vel{};
    Vector2 acc{};

    Vector2 dir{};
    float angularSpeed;
};

void Update(Rigidbody& rb, float dt)
{
    rb.vel = rb.vel + rb.acc * dt;
    rb.pos = rb.pos + rb.vel * dt + rb.acc * dt * dt * 0.5f;
    rb.dir = RotateTowards(rb.dir, Normalize(rb.vel), rb.angularSpeed * dt);
}

Vector2 Seek(const Vector2& pos, const Rigidbody& rb, float maxSpeed)
{
    return Normalize(pos - rb.pos) * maxSpeed - rb.vel;
}

Vector2 Flee(const Vector2& pos, const Rigidbody& rb, float maxSpeed)
{
    return Normalize(rb.pos - pos) * maxSpeed - rb.vel;
}

// Velocity-based avoid, simpler to understand
/*void Avoid(Rigidbody& rb, const Vector2& obstaclePosition, float obstacleRadius,
    float dt, float probeAngle, float probeLength)
{
    Vector2 probeEnd = rb.pos + Rotate(rb.dir, probeAngle * DEG2RAD) * probeLength;
    bool collision = CheckCollisionLineCircle(rb.pos, probeEnd, obstaclePosition, obstacleRadius);
    if (collision)
    {
        Vector2 linearDirection = Normalize(rb.vel);
        float linearSpeed = Length(rb.vel);
        float avoidSign = probeAngle >= 0.0f ? -1.0f : 1.0f;
        rb.vel = Rotate(linearDirection, rb.angularSpeed * avoidSign * dt) * linearSpeed;
    }
}*/

// Acceleration-based avoid, nice because you can blend it with other steering forces
Vector2 Avoid(const Rigidbody& rb, const Vector2& obstaclePosition, float obstacleRadius,
    float dt, float probeAngle/*degrees*/, float probeLength)
{
    Vector2 probeEnd = rb.pos + Rotate(rb.dir, probeAngle * DEG2RAD) * probeLength;
    bool collision = CheckCollisionLineCircle(rb.pos, probeEnd, obstaclePosition, obstacleRadius);
    if (collision)
    {
        Vector2 vi = rb.vel;
        Vector2 linearDirection = Normalize(rb.vel);
        float linearSpeed = Length(rb.vel);
        float avoidSign = probeAngle >= 0.0f ? -1.0f : 1.0f;
        Vector2 vf = Rotate(linearDirection, rb.angularSpeed * avoidSign * dt) * linearSpeed;
        // a = (vf - vi) / t
        return (vf - vi) / dt;
    }
    return {};
}

// We can flee the obstacle, but this will push us backwards instead of left/right of the obstacle which may be undesirable
// See page 53 of Lecture_3.pdf for how this would be better if we have more powerful collision detection
//Vector2 Avoid(const Rigidbody& rb, const Vector2& obstaclePosition, float obstacleRadius,
//    float dt, float probeAngle/*degrees*/, float probeLength)
//{
//    Vector2 probeEnd = rb.pos + Rotate(rb.dir, probeAngle * DEG2RAD) * probeLength;
//    bool collision = CheckCollisionLineCircle(rb.pos, probeEnd, obstaclePosition, obstacleRadius);
//    if (collision)
//    {
//        return Flee(obstaclePosition, rb, 100.0f);
//    }
//    return {};
//}

int main(void)
{
    Vector2 result = Normalize(Vector2{ 30.0f, 70.0f });

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Sunshine");
    rlImGuiSetup(true);
    SetTargetFPS(60);

    float seekerProbeLength = 100.0f;
    float seekerProximity = seekerProbeLength * 2.0f;
    float seekerRadius = 25.0f;
    Rigidbody seeker;
    seeker.pos = { SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f };
    seeker.dir = { 0.0f, 1.0f };
    seeker.angularSpeed = 100.0f * DEG2RAD;

    Vector2 obstaclePosition{ SCREEN_WIDTH * 0.75f, SCREEN_HEIGHT * 0.25f };
    float obstacleRadius = 50.0f;

    float t = 0.0f;

    bool useGUI = false;
    while (!WindowShouldClose())
    {
        const float dt = GetFrameTime();

        Vector2 leftEnd = seeker.pos + Rotate(seeker.dir, -30.0f * DEG2RAD) * seekerProbeLength;
        //Vector2 leftEnd2 = seeker.pos + Rotate(seeker.dir, -15.0f * DEG2RAD) * seekerProbeLength * 2.0f;
        Vector2 rightEnd = seeker.pos + Rotate(seeker.dir, 30.0f * DEG2RAD) * seekerProbeLength;
        //Vector2 rightEnd2 = seeker.pos + Rotate(seeker.dir, 15.0f * DEG2RAD) * seekerProbeLength * 2.0f;

        bool leftCollision = CheckCollisionLineCircle(seeker.pos, leftEnd, obstaclePosition, obstacleRadius);
        //bool leftCollision2 = CheckCollisionLineCircle(seeker.pos, leftEnd2, obstaclePosition, obstacleRadius);
        bool rightCollision = CheckCollisionLineCircle(seeker.pos, rightEnd, obstaclePosition, obstacleRadius);
        //bool rightCollision2 = CheckCollisionLineCircle(seeker.pos, rightEnd2, obstaclePosition, obstacleRadius);

        Vector2 avoidLeft = Avoid(seeker, obstaclePosition, obstacleRadius, dt, -30.0f, seekerProbeLength);
        Vector2 avoidLeft2 = Avoid(seeker, obstaclePosition, obstacleRadius, dt, -15.0f, seekerProbeLength * 2.0f);
        Vector2 avoidRight = Avoid(seeker, obstaclePosition, obstacleRadius, dt, 30.0f, seekerProbeLength);
        Vector2 avoidRight2 = Avoid(seeker, obstaclePosition, obstacleRadius, dt, 15.0f, seekerProbeLength * 2.0f);
        Vector2 avoid = avoidLeft + avoidLeft2 + avoidRight + avoidRight2;

        float distance = Distance(seeker.pos, obstaclePosition);
        if (distance <= seekerProximity)
        {
            // Increase avoidance force as the obstacle approaches
            t = 1.0f - distance / seekerProximity;
        }
        else
        {
            // Choose seek if far away from obstacle
            t = 0.0f;
        }

        seeker.acc = Lerp(Seek(GetMousePosition(), seeker, 100.0f), avoid, t);
        Update(seeker, dt);

        Color leftColor = leftCollision ? RED : GREEN;
        //Color leftColor2 = leftCollision2 ? RED : GREEN;
        Color rightColor = rightCollision ? RED : GREEN;
        //Color rightColor2 = rightCollision2 ? RED : GREEN;

        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawCircleV(seeker.pos, seekerRadius, BLUE);
        DrawCircleV(seeker.pos, seekerProximity, {0, 121, 181,  unsigned char(t * 255.0f)});
        DrawCircleV(obstaclePosition, obstacleRadius, GRAY);
        DrawLineV(seeker.pos, leftEnd, leftColor);
        //DrawLineV(seeker.pos, leftEnd2, leftColor2);
        DrawLineV(seeker.pos, rightEnd, rightColor);
        //DrawLineV(seeker.pos, rightEnd2, rightColor2);

        if (IsKeyPressed(KEY_GRAVE)) useGUI = !useGUI;
        if (useGUI)
        {
            rlImGuiBegin();
            rlImGuiEnd();
        }

        DrawFPS(10, 10);
        DrawText("Press ~ to open/close GUI", 10, 30, 20, GRAY);
        EndDrawing();
    }

    rlImGuiShutdown();
    CloseWindow();
    return 0;
}