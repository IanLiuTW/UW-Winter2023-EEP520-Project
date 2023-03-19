#ifndef __SCORE_AGENT__H
#define __SCORE_AGENT__H

#include <string>
#include "enviro.h"

using namespace enviro;
using namespace std;

// Controller for the score label. It is responsible for keeping track of the score and displaying the score and bullet speed on the screen.
class ScoreController : public Process, public AgentInterface
{

public:
    ScoreController() : Process(), AgentInterface() {}

    void init()
    {
        // Watch for the protagonist's status and set the leader_alive variable accordingly.
        watch("leader_dead", [this](Event e)
              { leader_alive = false; });
        watch("leader_alive", [this](Event e)
              { leader_alive = true; });
        // Watch for the bullet speed and set the bullet_speed variable accordingly.
        watch("bullet_speed", [this](Event e)
              { bullet_speed = e.value()["speed"]; });
        // Watch for the goal reached event and increase the score.
        watch("goal_reached", [this](Event e)
              { score += 1; });
        // Watch if a new game has started and reset the score.
        watch("respawn_score", [this](Event e)
              { respwan(); });
    }
    void start() {}
    void update()
    {
        // Update the content of the label and display it on the screen.
        label(get_label(), label_center, -250);
    }
    void stop() {}

    // Respawns the score label to its initial state.
    void respwan()
    {
        score = 0;
        bullet_speed = 0;
        leader_alive = true;
    }

    // Returns the content of the label.
    string get_label()
    {
        // If the protagonist is dead, display "Game Over" on the screen.
        if (!leader_alive)
        {
            label_center = -30;
            return "Game Over";
        }
        // If the protagonist is alive, display the score and bullet speed on the screen.
        else
        {
            label_center = -80;
            return "Score: " + to_string(score) + " |  Bullet Speed: " + to_string(bullet_speed);
        }
    }

private:
    int score = 0, bullet_speed = 0;
    bool leader_alive = true;
    int label_center = -40;
};

class Score : public Agent
{
public:
    Score(json spec, World &world) : Agent(spec, world)
    {
        add_process(c);
    }

private:
    ScoreController c;
};

DECLARE_INTERFACE(Score)

#endif