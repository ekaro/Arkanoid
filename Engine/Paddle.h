#pragma once
#include "Ball.h"
#include "Vec2.h"
#include "RectF.h"
#include "Colors.h"
#include "Graphics.h"
#include "Keyboard.h"

class Paddle
{
public:
	Paddle(const Vec2& pos_in, float halfWidth_in, float halfHeight_in);
	void Draw(Graphics& gfx) const;
	bool BallCollision(Ball& ball);
	void WallCollision(const RectF& walls);
	void Update(const Keyboard& kbd, float dt);
	RectF GetRect() const;
	void ResetCoolodown();
private:
	static constexpr Color wingColor = Colors::Red;
	static constexpr Color color = Colors::White;
	static constexpr float wingWidth = 20.0f;
	static constexpr float speed = 300.0f;
	// control of paddle rebound behavior
	static constexpr float maximumExitRatio = 2.6f;
	static constexpr float fixedZoneHalfWidthRatio = 0.2f;
	float halfWidth;
	float halfHeight;
	// these are derived from above controls
	float ExitFactor;
	float fixedZoneHalfWidth;
	float fixedZoneExit;

	Vec2 pos;
	bool Cooldown = false;
};