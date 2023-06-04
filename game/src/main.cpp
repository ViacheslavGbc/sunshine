
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
		DrawCircleV(position, radius, collision ? RED : PURPLE);
		DrawCircleV(cursor, radius, BLUE);
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

