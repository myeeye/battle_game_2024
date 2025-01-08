#pragma once
#include "battle_game/core/unit.h"

namespace battle_game::unit {
class GoldHopper : public Unit {
 public:
  GoldHopper(GameCore *game_core, uint32_t id, uint32_t player_id);
  void Render() override;
  void Update() override;
  [[nodiscard]] bool IsHit(glm::vec2 position) const override;

 protected:
  void RandomRun();
  void Bang();
  [[nodiscard]] float GetDamageScale() const override;
  [[nodiscard]] float GetSpeedScale() const override;
  [[nodiscard]] float BasicMaxHealth() const override;
  [[nodiscard]] const char *UnitName() const override;
  [[nodiscard]] const char *Author() const override;
  uint32_t move_count_down_{0};
  uint32_t bang_count_down_{0};
  float OldHealth{0};
};
}  // namespace battle_game::unit
