#include "battle_game/core/bullets/killer_ball.h"

#include "battle_game/core/game_core.h"
#include "battle_game/core/particles/particles.h"

namespace battle_game::bullet {
KillerBall::KillerBall(GameCore *core,
                       uint32_t id,
                       uint32_t unit_id,
                       uint32_t player_id,
                       glm::vec2 position,
                       float rotation,
                       float damage_scale)
    : Bullet(core, id, unit_id, player_id, position, rotation, damage_scale),
    death_count_down_(kTickPerSecond*game_core_->RandomInt(5,20)) {
}

void KillerBall::Render() {
  SetTransformation(position_, rotation_, glm::vec2{0.1f});
  SetColor(game_core_->GetPlayerColor(player_id_));
  SetTexture(BATTLE_GAME_ASSETS_DIR "textures/killerball.png");
  DrawModel(0);
}

void KillerBall::Update() {
  if (!death_count_down_) {
    game_core_->PushEventRemoveBullet(id_);
    return;
  }
  death_count_down_--;

  auto &units = game_core_->GetUnits();
  for (auto &unit : units) {
    if (unit.first == unit_id_) {
      continue;
    }
    if (unit.second->IsHit(position_)) {
      game_core_->PushEventDealDamage2(unit.first, id_, unit_id_, damage_scale_ * game_core_->RandomFloat() / 2);
    }
  }
}

KillerBall::~KillerBall() {
  for (int i = 0; i < 5; i++) {
    game_core_->PushEventGenerateParticle<particle::Smoke>(
        position_, rotation_, game_core_->RandomInCircle() * 2.0f, 0.2f,
        glm::vec4{0.0f, 0.0f, 0.0f, 1.0f}, 3.0f);
  }
}
}  // namespace battle_game::bullet
