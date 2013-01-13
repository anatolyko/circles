#ifndef __KOCHERGIN_TEST_TASK_GAME_H__
#define __KOCHERGIN_TEST_TASK_GAME_H__

/*
* Here resides all the game logic
*/

#include "math\circle.h"
#include "math\color.h"

#include <list>

// some typedefs to ease carpal tunnel syndrome 
typedef basic_point<double> point_type;
typedef basic_rgb_color<double> color_type;
typedef basic_circle<double> circle_type;


// Add color, speed and score. It's still a brainless struct
struct GameCircle : public circle_type
{
	GameCircle():score(0) {} 

	color_type color;
	point_type speed; // per second, vector

	size_t score;
};

typedef std::list<GameCircle> game_circle_list;


// Main game class 
class Game 
{
public:
	Game();
	~Game();

	bool init();
	bool cleanup();

	void adjust_viewport_size(size_t width, size_t height);

	void update(double time_elapsed); // time_elapsed in seconds

	void handle_mouse_click(double x, double y);

	// getters
	const game_circle_list& circles() const { return circles_; }
	size_t get_overall_score() const { return overall_score_; }

	// getters for predicate for remove_if 
	double viewport_width() const { return viewport_width_; }
	double viewport_height() const { return viewport_height_; }

	// predicate for remove_if
	static bool is_outside_window(const circle_type& circle);

	// Meyers singleton
	static Game& instance()
	{
		static Game the_game;
		return the_game;
	}

private: 	
	// private data
	game_circle_list circles_;

	double viewport_width_;
	double viewport_height_;

	// private methods
	void generate_circles(double time_elapsed);

	size_t overall_score_;

private:
	// make it non-copyable (its a singleton)	
	Game(const Game&);
	Game& operator=(const Game&);
};

#endif // #define __KOCHERGIN_TEST_TASK_GAME_H__
