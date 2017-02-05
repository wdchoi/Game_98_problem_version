/////////////////////////////////////////////////////////////////////////////
// Authored by Jeong-Mo Hong for CSE4060 course at Dongguk University CSE  //
// jeongmo.hong@gmail.com                                                  //
// Do whatever you want license.                                           //
/////////////////////////////////////////////////////////////////////////////

#pragma once

#include "GenericDefinitions.h"
#include "Vector3D.h"
//#include "RandomNumberGenerator.h"
#include "CircularQueue.h"

class GameState
{
public:
    DV ball_pos_;
    DV agent_pos_;

    GameState()
    {}

    GameState(const DV& bp, const DV& ap)
        : ball_pos_(bp), agent_pos_(ap)
    {}

    void operator = (const GameState st)
    {
        ball_pos_ = st.ball_pos_;
        agent_pos_ = st.agent_pos_;
    }
};

class Environment
{
public:
    DV ball_pos_;
    DV ball_vel_;

    DV agent_pos_;
    D  agent_width_;

    int num_records_;

    //RandomNumberGenerator rand_;
    CircularQueue<GameState> history_;

    Environment()
        //: rand_(5), num_records_(5)
		: num_records_(5)
    {
        history_.initialize(num_records_);        
    }

    Environment(const int& num_rec)
        //: rand_(5), num_records_(num_rec)
		: num_records_(num_rec)
    {
        history_.initialize(num_records_);
    }

    void initialize()
    {
        ball_pos_ = DV(0.5, 0, 0.9);    // y = 0.0 is ground
 //     ball_vel_ = DV(rand_.getNumber(), 0.0, rand_.getNumber()).getSafeNormalized();
        ball_vel_ = DV(((D)rand()/RAND_MAX - 0.5)*2.0, 0.0, -1.0).getSafeNormalized();
        //ball_vel_ = DV(0, 0, -1);

        agent_pos_ = DV(0.5, 0, 0);     // y = 0.0 is ground

        agent_width_ = 0.3;

        for (int i = 0; i < num_records_; i++)
            history_.pushBack(GameState(ball_pos_, agent_pos_));
    }

    void reset()
    {
        ball_pos_ = DV(0.5, 0, 0.9);    // y = 0.0 is ground
                                        //     ball_vel_ = DV(rand_.getNumber(), 0.0, rand_.getNumber()).getSafeNormalized();
        ball_vel_ = DV((rand()/RAND_MAX - 0.5)*2.0, 0.0, -1.0).getSafeNormalized();

        for (int i = 0; i < num_records_; i++)
            history_.pushBack(GameState(ball_pos_, agent_pos_));
    }

    void updateAgent(const D& dx, const D& dy, const D& dz, const T& dt)
    {
        agent_pos_.x_ += dx * dt;
        agent_pos_.y_ += dy * dt;
        agent_pos_.z_ += dz * dt;

        // don't let it escape 
        agent_pos_.x_ = CLAMP(agent_pos_.x_, 0 + agent_width_*0.5, 1.0 - agent_width_ * 0.5);
    }

	D update(const D& dt, D& success_rate)   // returns reward
	{
		static int counter = 0;
		static int combo = 0;
		static int ground_counter = 0;

		ground_counter++;

		ball_pos_ += ball_vel_ * dt;

	
		if (ball_pos_.x_ < 0.0 && ball_vel_.x_ < 0.0)
		{
			ball_vel_.x_ = -ball_vel_.x_;
		}
		if (ball_pos_.x_ > 1.0 && ball_vel_.x_ > 0.0)
		{
			ball_vel_.x_ = -ball_vel_.x_;
		}

		if (ball_pos_.z_ > 1.0 && ball_vel_.z_ > 0.0)
		{
			ball_vel_.z_ = -ball_vel_.z_;
		}

		//TODO: and some more wall collisions

		if (ball_pos_.z_ < 0.0 && ball_vel_.z_ < 0.0)
		{

			//std::cout << ground_counter << std::endl;
			//ground_counter = 0;

			D reward = 0.0;

			counter++;

			// agent hit the ball! reward 1.0
			if (ABS(agent_pos_.x_ - ball_pos_.x_) <= agent_width_ * 0.5)
			{
				ball_vel_.z_ = -ball_vel_.z_;

				ball_vel_.x_ += ((D)rand() / RAND_MAX - 0.5) * 0.2;

				combo++;

				reward = 1.0;
			}
			else // agent lost the ball. no reward
			{
				reset();

				reward = 0.0; // restart
			}

			if (counter == 10000)
			{
				std::cout << "Success rate " << (double)combo / (double)counter * 100.0 << " %" << std::endl;

				success_rate = (double)combo / (double)counter;

				counter = 0;
				combo = 0;
			}

			history_.pushBack(GameState(ball_pos_, agent_pos_));

			return reward;
		}		

		history_.pushBack(GameState(ball_pos_, agent_pos_));

		return 0.0;
	}

    void updateSubstep(const D& dt)
    {}
};
