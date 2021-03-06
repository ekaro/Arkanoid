/****************************************************************************************** 
 *	Chili DirectX Framework Version 16.07.20											  *	
 *	Game.cpp																			  *
 *	Copyright 2016 PlanetChili.net <http://www.planetchili.net>							  *
 *																						  *
 *	This file is part of The Chili DirectX Framework.									  *
 *																						  *
 *	The Chili DirectX Framework is free software: you can redistribute it and/or modify	  *
 *	it under the terms of the GNU General Public License as published by				  *
 *	the Free Software Foundation, either version 3 of the License, or					  *
 *	(at your option) any later version.													  *
 *																						  *
 *	The Chili DirectX Framework is distributed in the hope that it will be useful,		  *
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of						  *
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the						  *
 *	GNU General Public License for more details.										  *
 *																						  *
 *	You should have received a copy of the GNU General Public License					  *
 *	along with The Chili DirectX Framework.  If not, see <http://www.gnu.org/licenses/>.  *
 ******************************************************************************************/
#include "MainWindow.h"
#include "Game.h"
#include "SpriteCodex.h"

Game::Game( MainWindow& wnd )
	:
	wnd( wnd ),
	gfx( wnd ),
	ball(Vec2(300.0f + 24.0f, 300.0f), Vec2(-1.0f, -1.0f)),
	walls(0.0f, float(gfx.ScreenWidth), 0.0f, float(gfx.ScreenHeight)),
	pad(Vec2(400.0f,500.0f), 50.0f, 15.0f)
{
	const Color colors[4] = { Colors::Red, Colors::Green, Colors::Blue, Colors::Cyan };
	const Vec2 topLeft(40.0f, 40.0f);

	int i = 0;
	for (int y = 0; y < nBricksDown; y++)
	{
		const Color c = colors[y];
		for (int x = 0; x < nBricksAcross; x++)
		{
			bricks[i] = Brick(RectF(topLeft + Vec2(x * brickWidth, y * brickHeight), brickWidth, brickHeight), c);
			i++;
		}
	}
}

void Game::Go()
{
	gfx.BeginFrame();	
	float elapsedTime = ft.Mark();
	while (elapsedTime > 0.0f)
	{
		const float dt = std::min(0.0025f, elapsedTime);
		UpdateModel(dt);
		elapsedTime -= dt;
	}
	ComposeFrame();
	gfx.EndFrame();
}

void Game::UpdateModel(float dt)
{
	if (!GameOver)
	{
		pad.Update(wnd.kbd, dt);
		pad.WallCollision(walls);
		ball.Update(dt);

		bool CollisionHappened = false;
		float curColDistSq;
		int curColIndex;

		for ( int i = 0; i < nBricks; i++)
		{
			if (bricks[i].CheckBallCollision(ball))
			{
				const float newColDistSq = (ball.GetPosition() - bricks[i].GetCenter()).GetLengthSq();
				if (CollisionHappened)
				{
					if (newColDistSq < curColDistSq)
					{
						curColDistSq = newColDistSq;
						curColIndex = i;
					}
				}
				else
				{
					curColDistSq = newColDistSq;
					curColIndex = i;
					CollisionHappened = true;
				}
			}
		}

		if (CollisionHappened)
		{
			pad.ResetCoolodown();
			bricks[curColIndex].ExecuteBallCollision(ball);
		}

		pad.BallCollision(ball);

		int WallCollisionResult = ball.WallCollision(walls);
		if (WallCollisionResult == 1)
		{
			pad.ResetCoolodown();
		}
		else if (WallCollisionResult == 2)
		{
			GameOver = true;
		}
	}
}

void Game::ComposeFrame()
{
	if (!GameOver)
	{
		ball.Draw(gfx);
		pad.Draw(gfx);
	}
	else
	{
		SpriteCodex::DrawGameOver(Graphics::ScreenWidth/2-50, Graphics::ScreenHeight/2-35, gfx);
	}
	
	for (const Brick& b : bricks)
	{
		b.Draw(gfx);
	}
}
