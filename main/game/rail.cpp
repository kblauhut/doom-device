#include "rail.h"

int LIFETIME_TICKS = 1000;
float SCREEN_ASPECT_RATIO = SCREEN_WIDTH / SCREEN_HEIGHT;
float SCREEN_VERTICAL_FOV = FOV_RAD / SCREEN_ASPECT_RATIO;

Rail::Rail(
    int creation_tick_count,
    float height,
    std::vector<float> origin,
    std::vector<float> destination
) :
        creation_tick_count(creation_tick_count),
        height(height),
        origin(origin),
        destination(destination)
        {}

        
void Rail::render(
    int tick_count,
    Player* player,
    std::vector<float> frustum_left,
    std::vector<float> frustum_right,
    float* depth_buffer
) {
    int delta_ticks = tick_count - this->creation_tick_count;
    if (delta_ticks > LIFETIME_TICKS) return;

    float animation_value = 1 - delta_ticks / LIFETIME_TICKS;

    std::vector<float> player_position = std::vector<float>{player->position->at(0), player->position->at(1)};
    float player_z = player->position->at(2);

    float determinant = 1.0;
    // const determinant = det([
    //   subtract(this->origin, this->destination),
    //   subtract(player->position, this->destination),
    // ]);

    std::pair<std::vector<float>, std::vector<float>> clipped_points = clipLineSegmentWithFrustum(
      player->position,
     ( determinant > 0) ? this->destination : this->origin,
     ( determinant > 0 )? this->origin : this->destination,
      frustum_left,
      frustum_right,
      VIEW_DISTANCE
    );

    if (clipped_points == std::make_pair(std::vector<float>{0.0, 0.0}, std::vector<float>{0.0, 0.0})) return;

    std::vector<float> origin_clipped = clipped_points.first;
    std::vector<float> destination_clipped = clipped_points.second;

    float l_X = getScreenX(
     player_position,
      origin_clipped,
      frustum_left,
      frustum_right,
      SCREEN_WIDTH
    );

    float r_X = getScreenX(
     player_position,
      destination_clipped,
      frustum_left,
      frustum_right,
      SCREEN_WIDTH
    );

    float l_top_Y = getScreenY(
     player_position,
      origin_clipped,
      this->height - player_z,
      SCREEN_VERTICAL_FOV
    );

    float l_bottom_Y = getScreenY(
     player_position,
      origin_clipped,
      this->height - animation_value / 2 - player_z,
      SCREEN_VERTICAL_FOV
    );

    float r_top_Y = getScreenY(
     player_position,
      destination_clipped,
      this->height - player_z,
      SCREEN_VERTICAL_FOV
    );

    float r_bottom_Y = getScreenY(
     player_position,
      destination_clipped,
      this->height - animation_value / 2 - player_z,
      SCREEN_VERTICAL_FOV
    );


    uint16_t color = rgbColor(255 * animation_value, 255, 255 * animation_value);
    
    rasterizeParallelogramDepthClip(
        std::vector<std::vector<float>>{
            {l_X, l_top_Y},
            {r_X, r_top_Y},
        },
        std::vector<std::vector<float>>{
            {l_X, l_bottom_Y},
            {r_X, r_bottom_Y},
        },
      color,
      distance(player_position, origin_clipped),
      distance(player_position, destination_clipped),
      depth_buffer
    );
}
