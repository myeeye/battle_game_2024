#pragma once
#include "battle_game/core/bullet.h"

namespace battle_game::bullet {
class KillerBall : public Bullet {
 public:
  KillerBall(GameCore *core,
             uint32_t id,
             uint32_t unit_id,
             uint32_t player_id,
             glm::vec2 position,
             float rotation,
             float damage_scale);
  ~KillerBall() override;
  void Render() override;
  void Update() override;

 private:
    int death_count_down_; 
};
}  // namespace battle_game::bullet
