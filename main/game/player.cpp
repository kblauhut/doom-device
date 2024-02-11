#include "player.h"

float max_velocity = 0.1f;
float acceleration = 0.001f;
float deceleration = 0.002f;

Player::Player() :
        position(std::make_unique<std::vector<float>>(std::vector<float>{0.0f, 0.0f, 1.0f}))
        {}


void Player::update_player(controller_input *input, int delta_ticks) {
        this->yaw -= input->left * 0.3;
        this->yaw += input->right * 0.3;



        if (input->forward) {
        float new_velocity = this->forward_velocity + (acceleration * delta_ticks);
        this->forward_velocity = std::min(new_velocity, max_velocity);
        } else {
        float new_velocity = this->forward_velocity - (deceleration * delta_ticks);
				this->forward_velocity = std::max(new_velocity, 0.0f);
        }


        this->position->at(0) += this->forward_velocity * std::cos(this->yaw) * delta_ticks;
        this->position->at(1) += this->forward_velocity * std::sin(this->yaw) * delta_ticks;
}
