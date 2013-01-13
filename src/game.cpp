#include "game.h"

// these needed for srand
#include <time.h>
#include <stdlib.h>


// Game settings
// ideally these should be loaded from config
const size_t CircleGenerationSpeed = 1; // units per second

const double CircleMinimumRadius = 20.0; // GL coordinate space
const double CircleMaximumRadius = 60.0; 

const double CircleMinimumSpeed = 2.0; // GL coordinate space
const double CircleMaximumSpeed = 6.0; 

const size_t CircleMinimumScore = 10; 
const size_t CircleMaximumScore = 100; 


Game::Game()
	: viewport_width_(0.0), viewport_height_(0.0), overall_score_(0)
{
}

Game::~Game()
{
}

bool Game::init()
{
	// init random 
	unsigned seed = (unsigned int)time(0);
	srand(seed);
	
	return true;
}

bool Game::cleanup()
{
	circles_.clear();

	return true;
}

void Game::adjust_viewport_size(size_t width, size_t height)
{
	viewport_width_ = width;
	viewport_height_ = height;
}

void Game::update(double time_elapsed)
{
	// update existing units
	game_circle_list::iterator it = circles_.begin();
	for(;it != circles_.end();++it)
	{
		it->pos += it->speed;
	}

	// check for a deadline (screen border)
	circles_.remove_if(Game::is_outside_window);

	// spawn some new ones
	generate_circles(time_elapsed);
}

bool Game::is_outside_window(const circle_type& circle)
{
	return circle.pos.y + circle.radius > instance().viewport_height();
}

void Game::generate_circles(double time_elapsed)
{
	static const double TimeToSpawn = 1.0 / CircleGenerationSpeed; // time needed to spawn one unit
	static double accumulated_time = 0.0; // leftovers of time from previous cicles

	accumulated_time += time_elapsed;
	while(accumulated_time > TimeToSpawn) 
	{
		accumulated_time -= TimeToSpawn;
	
		// spawn a unit
		GameCircle circle;

		// random radius
		double size_aspect = static_cast<double>(rand()) / RAND_MAX;
		circle.radius  = size_aspect * (CircleMaximumRadius - CircleMinimumRadius) + CircleMinimumRadius;

		// calculate speed & score 
		circle.speed.y = (1.0 - size_aspect) * (CircleMaximumSpeed - CircleMinimumSpeed) + CircleMinimumSpeed; // less size == more speed
		circle.score = static_cast<int>((1.0 - size_aspect) * (CircleMaximumScore - CircleMinimumScore)) + CircleMinimumScore; // more speed == more score

		// set position
		double position_aspect = static_cast<double>(rand()) / RAND_MAX;
		circle.pos.x = position_aspect * (viewport_width_ - circle.radius * 2) + circle.radius; // whole circle should be inside viewport
		circle.pos.y = circle.radius;

		// set color 
		double blue_color_aspect = static_cast<double>(rand()) / RAND_MAX;
		circle.color.reset(size_aspect, position_aspect, blue_color_aspect);

		// by value
		// 
		circles_.push_back(circle);
	}
}

void Game::handle_mouse_click(double x, double y)
{
	point_type pt(x, y);

	// lets find topmost circle under mouse pointer
	// counting is in reverse order due to painting order
	game_circle_list::reverse_iterator rit = circles_.rbegin();
	for(;rit != circles_.rend(); ++rit)
	{
		if(rit->is_inside(pt)) 
		{
			// increase score
			overall_score_ += rit->score;

			// kill it
			circles_.erase(--rit.base());
			break;
		}
	}

}
