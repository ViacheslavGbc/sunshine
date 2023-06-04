/*
#include "rlImGui.h"
#include "Math.h"
#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

struct Rigidbody
{
	// insert physics variables here
};

Vector2 xLerp(Vector2 A, Vector2 B, float t)
{
	return A + (B - A) * t;
}

int main(void)
{
	InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Sunshine");
	rlImGuiSetup(true);
	SetTargetFPS(60);

	Vector2 A{ SCREEN_WIDTH * 0.1f, SCREEN_HEIGHT * 0.75f };
	Vector2 B{ SCREEN_WIDTH * 0.9f, SCREEN_HEIGHT * 0.5f };
	Vector2 C = A;
	float t = 0.0f;

	Vector2 position{ SCREEN_WIDTH * 0.1f, SCREEN_HEIGHT * 0.9f };
	Vector2 velocity{};
	Vector2 acceleration{};

	Vector2 target{ SCREEN_WIDTH * 0.9f, SCREEN_HEIGHT * 0.1f };
	float seekerSpeed = 500.0f;

	while (!WindowShouldClose())
	{
		//const float dt = GetFrameTime();
		//position = position + velocity * dt + acceleration * 0.5f * dt * dt;
		//velocity = velocity + acceleration * dt;
		//if (position.y > SCREEN_HEIGHT) position.y = 0.0f;
		//if (position.y < 0.0f) position.y = SCREEN_HEIGHT;
		//if (position.x > SCREEN_WIDTH) position.x = 0.0f;
		//if (position.x < 0.0f) position.x = SCREEN_WIDTH;

		// AB = B - A
		//Vector2 direction = Normalize(target - position);
		//Vector2 desiredVelocity = direction * seekerSpeed;
		//acceleration = desiredVelocity - velocity;

		C = xLerp(A, B, t);

		BeginDrawing();
		ClearBackground(RAYWHITE);
		DrawCircleV(A, 20.0f, RED);
		DrawCircleV(B, 20.0f, GREEN);
		DrawCircleV(C, 10.0f, GRAY);
		DrawLineV(A, B, BLACK);
		//DrawCircleV(position, 20.0f, RED);
		//DrawCircleV(target, 20.0f, BLUE);

		// Draw line 100 units above the seeker
		//Vector2 end = position + Vector2{0.0f, -100.0f};
		//DrawLineV(position, end, RED);
		//DrawText("100 pixels above", end.x, end.y, 20, RED);

		rlImGuiBegin();
		ImGui::SliderFloat("Interpolation", &t, 0.0f, 1.0f);
		//ImGui::SliderFloat("Seeker speed", &seekerSpeed, -100.0f, 100.0f);
		//ImGui::SliderFloat2("Target", &target.x, 0.0f, SCREEN_WIDTH);
		//ImGui::SliderFloat2("Position", &position.x, 0.0f, SCREEN_WIDTH);
		//ImGui::SliderFloat2("Velocity", &velocity.x, -100.0f, 100.0f);
		//ImGui::SliderFloat2("Acceleration", &acceleration.x, -100.0f, 100.0f);
		rlImGuiEnd();

		EndDrawing();
	}

	rlImGuiShutdown();
	CloseWindow();
	return 0;
}*/




///////////////////////////////////////////////////






#include "rlImGui.h"
#include "Math.h"
#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

bool CheckCollisionLineCircle(Vector2 lineStart, Vector2 lineEnd, Vector2 circlePosition, float circleRadius)
{
	Vector2 nearest = NearestPoint(lineStart, lineEnd, circlePosition);
	return Distance(nearest, circlePosition) <= circleRadius;
}

int main(void)
{
	Vector2 AB = { 1.0f, 0.0f };
	Vector2 CD = Normalize(Vector2{ 0.9f, 0.1f });

	float dot = Dot(AB, AB);

	InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Sunshine");
	rlImGuiSetup(true);
	SetTargetFPS(60);

	const float radius = 25.0f;
	Vector2 position{ SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f };

	bool useGUI = false;
	while (!WindowShouldClose())
	{
		Vector2 cursor = GetMousePosition();
		//AB = B - A
		//Position_to_Cursor = Cursor - Position
		//Cursor_to_Position = (Cursor - Position) * -1.0
		//Vector2 oppositeDirection = Normalize(cursor - position) * -1.0f;
		Vector2 direction = { 0.0f, 1.0f };//Normalize(cursor - position);
		Vector2 left = Rotate(direction, -30.0f * DEG2RAD);
		Vector2 right = Rotate(direction, 30.0f * DEG2RAD);
		bool collision = CheckCollisionLineCircle(position, position + left * 100.0f, cursor, radius) ||
			CheckCollisionLineCircle(position, position + right * 100.0f, cursor, radius);

		BeginDrawing();
		ClearBackground(RAYWHITE);
		DrawCircleV(position, radius, collision ? RED : GREEN);
		DrawCircleV(cursor, radius, BLACK);
		DrawLineV(position, position + right * 100.0f, BLACK);
		DrawLineV(position, position + left * 100.0f, BLACK);

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


////////////////////////////////////////////////////////////////
/*
#include "rlImGui.h"
#include "Math.h"
#include "Physics.h"

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

bool CheckCollisionLineCircle(Vector2 lineStart, Vector2 lineEnd, Vector2 circlePosition, float circleRadius)
{
	Vector2 nearest = NearestPoint(lineStart, lineEnd, circlePosition);
	return DistanceSqr(nearest, circlePosition) <= circleRadius * circleRadius;
}
/*
struct Rigidbody
{
	Vector2 pos{};
	Vector2 vel{};
	Vector2 acc{};

	Vector2 dir{};
	float angularSpeed;
};*/
/*
RMAPI float Sign(float value)
{
	float result = (value < 0.0f) ? -1.0f : 1.0f;

	return result;
}

RMAPI Vector3 Cross(Vector3 v1, Vector3 v2)
{
	Vector3 result = { v1.y * v2.z - v1.z * v2.y, v1.z * v2.x - v1.x * v2.z, v1.x * v2.y - v1.y * v2.x };

	return result;
}

// Rotate max radians towards the target
RMAPI Vector2 RotateTowards(Vector2 from, Vector2 to, float maxRadians)
{
	float deltaRadians = LineAngle(from, to);
	return Rotate(from, fminf(deltaRadians, maxRadians) * Sign(Cross(from, to)));
} */
/*
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

int main(void)
{
	Vector2 result = Normalize(Vector2{ 30.0f, 70.0f });

	InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Sunshine");
	rlImGuiSetup(true);
	SetTargetFPS(60);

	float seekerProbeLength = 100.0f;
	float seekerRadius = 25.0f;
	Rigidbody seeker;
	seeker.pos = { SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f };
	seeker.dir = { 0.0f, 1.0f };
	seeker.angularSpeed = 100.0f * DEG2RAD;

	Vector2 obstaclePosition{ SCREEN_WIDTH * 0.75f, SCREEN_HEIGHT * 0.25f };
	float obstacleRadius = 50.0f;

	bool useGUI = false;
	while (!WindowShouldClose())
	{
		const float dt = GetFrameTime();

		Vector2 right = Rotate(seeker.dir, 30.0f * DEG2RAD);
		Vector2 left = Rotate(seeker.dir, -30.0f * DEG2RAD);
		Vector2 rightEnd = seeker.pos + right * seekerProbeLength;
		Vector2 leftEnd = seeker.pos + left * seekerProbeLength;

		seeker.acc = Seek(GetMousePosition(), seeker, 100.0f);
		Update(seeker, dt);

		bool leftCollision = CheckCollisionLineCircle(seeker.pos, leftEnd, obstaclePosition, obstacleRadius);
		bool rightCollision = CheckCollisionLineCircle(seeker.pos, rightEnd, obstaclePosition, obstacleRadius);
		if (rightCollision)
		{
			Vector2 linearDirection = Normalize(seeker.vel);
			float linearSpeed = Length(seeker.vel);
			seeker.vel = Rotate(linearDirection, -seeker.angularSpeed * dt) * linearSpeed;
		}
		Color rightColor = rightCollision ? RED : GREEN;
		Color leftColor = leftCollision ? RED : GREEN;

		BeginDrawing();
		ClearBackground(RAYWHITE);
		DrawCircleV(seeker.pos, seekerRadius, BLUE);
		DrawCircleV(obstaclePosition, obstacleRadius, GRAY);
		DrawLineV(seeker.pos, rightEnd, rightColor);
		DrawLineV(seeker.pos, leftEnd, leftColor);

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
*/