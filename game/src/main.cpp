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
}