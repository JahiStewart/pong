#ifndef __MAINGAME_H__
#define __MAINGAME_H__

#include "Frog.h"
#include "MenuState.h"

//#include <string>
#include <vector>

namespace Webfoot {

class Prop;
class Ball;
class Paddle;
//==============================================================================

class MainGame : public MenuState
{
public:
   typedef MenuState Inherited;

   MainGame();
   virtual ~MainGame() {};
   
   virtual void Init();
   virtual void Deinit();

   /// Call this on every frame to update the positions.
   virtual void Update();
   /// Call this on every frame to draw the images.
   virtual void Draw();

   static MainGame instance;

protected:
   /// Returns the name of the GUI layer
   virtual const char* GUILayerNameGet();

   /// The ball that bounces around the screen.
   Paddle* paddle1;
   Paddle* paddle2;
   Ball* ball;
   Paddle* paddles[2];
};

MainGame* const theMainGame = &MainGame::instance;

//==============================================================================
//Base Class
class Prop {
public:
	Prop();
	static std::vector<Prop*> props;
	/// Initialize the prop
	void Init(const char* image_file, int x, int y, int velocity_x, int velocity_y, float step);
	/// Clean up the prop
	void Deinit();
	/// Make any changes for the given frame.  'dt' is the amount of time that
	/// has passed since the last frame, in milliseconds.
	virtual void Update(unsigned int dt);
	/// Draw the prop.
	void Draw();
	/// Bounce the ball
	void Bounce();
	/// Get bounds
	//For collision
	void Move(float x, float y);
	void Teleport(float x, float y);
	//
	Point2F position;
	Point2F br;
	Point2F tl;
protected:
	/// Appearance of the prop.
	Image* image;
	/// Current position of the prop.
	//Current velocity
	Point2F velocity;
	//Hitbox
	Point2F size;
	float step;

};
/// A bouncing ball
class Ball: public Prop{
public:
	Ball();
	void Update(unsigned int dt, Paddle *paddles[2]);
	boolean HitPaddle(Paddle* paddle);
	//hit paddle
	boolean IsCollision(Paddle *paddles[2]);
	//check with HitPaddle
	//Next Update: in each 1pt step in line of movement
};
class Paddle: public Prop{
public:
	Paddle();
protected:
	int score;
};

//==============================================================================

} //namespace Webfoot {

#endif //#ifndef __MAINGAME_H__
