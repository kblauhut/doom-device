#include "rail.h"

float LIFETIME_TICKS = 1000.0;
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
    std::vector<float> frustum_right
) {
    int delta_ticks = tick_count - this->creation_tick_count;
    if (delta_ticks > LIFETIME_TICKS) return;

    float animation_value = 1.0 - (delta_ticks / LIFETIME_TICKS);

    std::vector<float> player_position = std::vector<float>{player->position->at(0), player->position->at(1)};
    float player_z = player->position->at(2);
    
    std::vector<float> sub1 = subtract(this->origin, this->destination);
    std::vector<float> sub2 = subtract(player_position, this->destination);
    float determinant = sub1[0] * sub2[1] - sub1[1] * sub2[0];

    std::pair<std::vector<float>, std::vector<float>> clipped_points = clipLineSegmentWithFrustum(
      player->position,
     ( determinant > 0) ? this->destination : this->origin,
     ( determinant > 0 )? this->origin : this->destination,
      frustum_left,
      frustum_right,
      VIEW_DISTANCE
    );

    if (clipped_points.first[0] == 0.0 && clipped_points.first[1] == 0.0 && 
      clipped_points.second[0] == 0.0 && clipped_points.second[1] == 0.0
    ) {
      return;
    }

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
       this->height - player_z - (10.0/animation_value),
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
       this->height - player_z - (10.0/animation_value),
      SCREEN_VERTICAL_FOV
    );


    uint16_t color = rgbColor(255, 255 * animation_value, 255 * animation_value);

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
      distance(player_position, destination_clipped)
    );
}

static std::vector<float> intersect_with_map(
  const Sector* sector,
  std::vector<float> origin,
  float yaw
) {
  int intersecting_wall_index = -1;
  std::vector<float> intersect = {};
  for (int index = 1; index < sector->vertices.size(); index++) {
    std::vector<float> vertex_a = sector->vertices[index - 1];
    std::vector<float> vertex_b = sector->vertices[index];

    intersect = getLineSegmentIntersection(
      origin,
      std::vector<float>{
        origin[0] + std::cos(yaw) * 10000,
        origin[1] + std::sin(yaw) * 10000
      },
      vertex_a,
      vertex_b
    );


    if(!intersect.empty()){
      intersecting_wall_index = index - 1;
      break;
    }
  }

  if (intersecting_wall_index == -1) return intersect;

  int portal_walls_index = -1;
  for (int i = 0; i < sector->portalWallsIndices.size(); i++) {
    if (sector->portalWallsIndices[i] == intersecting_wall_index) {
      portal_walls_index = i;
      break;
    }
  }

  if (portal_walls_index == -1) return intersect;

  int neighbour_index = sector->neighbourIds[portal_walls_index];
  const Sector* portal_sector = &MAP[0];
  for (int i = 0; i < MAP.size(); i++) {
    if (MAP[i].id == neighbour_index) {
      portal_sector = &MAP[i];
      break;
    }
  }

  return intersect_with_map(portal_sector, intersect, yaw);
}

Rail create_rail_from_player_pos(
  Player* player,
  int tick_count
) {
  std::vector<float> origin = std::vector<float>{
    player->position->at(0) + (std::cos(player->yaw) * 0.5f),
    player->position->at(1) + (std::sin(player->yaw) * 0.5f)
   };

  std::vector<float> intersect = intersect_with_map(
      player->current_sector,
      origin,
      player->yaw
  );

  return Rail(
    tick_count,
    player->position->at(2),
    origin,
    intersect
  );
}
