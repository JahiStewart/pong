#include "Frog.h"
#include "MainGame.h"
#include "MainUpdate.h"

#include <random>
#include <string>


using namespace Webfoot;

MainGame MainGame::instance;

//==============================================================================

/// Main GUI
#define GUI_LAYER_NAME "MainGame"

//-----------------------------------------------------------------------------

MainGame::MainGame()
{
   ball = NULL;
   paddle1 = NULL;
   paddle2 = NULL;
   paddles[2] = NULL;
}

//-----------------------------------------------------------------------------

void MainGame::Init()
{
   Inherited::Init();

   // Create and initialize the ball.
   ball  = frog_new Ball();
   ball->Init("ball2", NULL, NULL, 1, -1, 8);
   paddle1 = frog_new Paddle();
   paddle1->Init("paddle", 50, NULL, 1, 0, 8);
   paddle2 = frog_new Paddle();
   paddle2->Init("paddle", 500, NULL, -1, 0, 8);
   Paddle* paddles[2] = {paddle1, paddle2};
}

//-----------------------------------------------------------------------------

void MainGame::Deinit()
{
   // Deinitialize and delete the ball.
	for (Prop* prop : Prop::props) {
		if (prop){
			prop->Deinit();
			frog_delete prop;
			prop = NULL;
		}
		
	}
   Inherited::Deinit();
}

//-----------------------------------------------------------------------------

const char* MainGame::GUILayerNameGet()
{
   return GUI_LAYER_NAME;
}

//-----------------------------------------------------------------------------

void MainGame::Update()
{
   Inherited::Update();

   unsigned int dt = theClock->LoopDurationGet();

   Paddle* paddles[2] = { paddle1, paddle2};

   ball->Update(dt, paddles);
   paddle1->Update(dt);
   paddle2->Update(dt);

   // Return to the previous menu if the escape key is pressed.
   if(!theStates->StateChangeCheck() && theKeyboard->KeyJustPressed(KEY_ESCAPE))
   {
      theMainGame->StateChangeTransitionBegin(true);
      theStates->Pop();
   }
}

//-----------------------------------------------------------------------------

void MainGame::Draw()
{
   ball->Draw();
   paddle1->Draw();
   paddle2->Draw();
}

//==============================================================================

Prop::Prop()
{
   // Initialize pointers to NULL for safety.
   image = NULL;
   props.push_back(this);
}
std::vector<Prop*> Prop::props;
//------------------------------------------------------------------------------

void Prop::Init(const char* image_file, int x, int y, int velocity_x, int velocity_y, float step)
{
   Point2F center;
   float w, h;
   // Load the image of the ball.
   image = theImages->Load(image_file);//!
   size = image->SizeGet();//width, height

   center = Point2F::Create(theScreen->SizeGet() / 2);
   if (x == NULL){
	   x = center.x;
   }
   if (y == NULL){
	   y = center.y;
   }
   // Start the ball in the middle of the screen.
   position = Point2F::Create(x, y);
   
   w = size.x / 2;
   h = size.y / 2;
   br = Point2F::Create(x + w, y + h);
   tl = Point2F::Create(x - w, y - h);
   velocity.x = velocity_x;
   velocity.y = velocity_y;
   this->step = step;
}

//------------------------------------------------------------------------------

void Prop::Deinit()
{
   // Unload the image of the ball.
   if(image)
   {
      theImages->Unload(image);
      image = NULL;
   }
}

//------------------------------------------------------------------------------

void Prop::Update(unsigned int dt)
{
	//need mouse object
	//need window object
	//random values
	Move(step * velocity.x, step * velocity.y);
	Bounce();
	
}
void Prop::Move(float x, float y){
	position.x += x;
	position.y += y;
	br.x += x;
	br.y += y;
	tl.x += x;
	tl.y += y;
}

void Prop::Teleport(float x, float y){
	position.x = x;
	position.y = y;
	size = image->SizeGet();
	float w;
	float h;
	w = size.x / 2;
	h = size.y / 2;
	br = Point2F::Create(x + w, y + h);
	tl = Point2F::Create(x - w, y - h);
}
void Prop::Bounce(){
	float x = position.x;
	float y = position.y;
	if (x >= SCREEN_WIDTH_DEFAULT) {
		velocity.x = -1;
	}
	else if (x <= 0) {
		velocity.x = 1;
	}
	if (y >= SCREEN_HEIGHT_DEFAULT) {
		velocity.y = -1;
	}
	else if (y <= 0) {
		velocity.y = 1;
	}
}

//------------------------------------------------------------------------------

void Prop::Draw()
{
   // The center of the ball is in the center of the image, so use an offset.
   image->Draw(position - (Point2F::Create(image->SizeGet()) / 2.0f));
}


Ball::Ball() {
	image = NULL;

}
//------------------------------------------------------------------------------
void Ball::Update(unsigned int dt, Paddle *paddles[2])
{
	//need mouse object
	//need window object
	//random values
	Point2F old;
	BOOLEAN x_state, y_state;
	float x, y;

	old = Point2F::Create(position.x, position.y);
	x = step * velocity.x;
	y = step * velocity.y;

	Move(x, 0);
	x_state = IsCollision(paddles);
	Move(-x, 0);

	Move(0, y);
	y_state = IsCollision(paddles);
	Move(0, -y);
	//Final Move
	if (x_state) {
		x *= -1;
		velocity.x *= -1;
	}
	if (y_state) {
		y *= -1;
		velocity.y *= -1;
	}
	Move(x, y);
	Bounce();
}
 ;
//hit paddle
boolean Ball::HitPaddle(Paddle* paddle)
{
	return (
		br.x > paddle->tl.x &&
		paddle->br.x > tl.x &&
		br.y > paddle->tl.y &&
		paddle->br.y > tl.y
		);
	
}
boolean Ball::IsCollision(Paddle *paddles[2])
{
	for (int i = 0; i < 2; ++i) {

		Paddle* paddle = paddles[i];
		if (paddle != NULL && HitPaddle(paddle)){
			return true;
		}
	}
	return false;
}
//------------------------------------------------------------------------------
Paddle::Paddle()
{
	image = NULL;
}

//------------------------------------------------------------------------------
