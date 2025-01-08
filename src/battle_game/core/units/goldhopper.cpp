#include "goldhopper.h"

#include "battle_game/core/bullets/bullets.h"
#include "battle_game/core/game_core.h"
#include "battle_game/graphics/graphics.h"

namespace battle_game::unit {

namespace {
uint32_t hopper_model_index = 0xffffffffu;
}  // namespace

GoldHopper::GoldHopper(GameCore *game_core, uint32_t id, uint32_t player_id)
    : Unit(game_core, id, player_id) {
  if (!~hopper_model_index) {
    auto mgr = AssetsManager::GetInstance();
    std::vector<ObjectVertex> vertices;
    std::vector<uint32_t> indices;
    const int precision = 60;
    const float inv_precision = 1.0f / float(precision);
    for (int i = 0; i < precision; i++) {
      auto theta = (float(i) + 0.5f) * inv_precision;
      theta *= glm::pi<float>() * 2.0f;
      auto sin_theta = std::sin(theta);
      auto cos_theta = std::cos(theta);
      vertices.push_back({{sin_theta * 0.5f, cos_theta * 0.5f},
                          {0.0f, 0.0f},
                          {0.5f, 0.5f, 0.0f, 1.0f}});
      indices.push_back(i);
      indices.push_back((i + 1) % precision);
      indices.push_back(precision);
    }
    vertices.push_back({{0.0f, 0.0f}, {0.0f, 0.0f}, {0.5f, 0.5f, 0.0f, 1.0f}});
    vertices.push_back({{0.1f, 0.0f}, {0.0f, 0.0f}, {0.5f, 0.5f, 0.0f, 1.0f}});
    vertices.push_back({{-0.1f, 0.0f}, {0.0f, 0.0f}, {0.5f, 0.5f, 0.0f, 1.0f}});
    vertices.push_back({{0.0f, 0.7f}, {0.0f, 0.0f}, {0.5f, 0.5f, 0.0f, 1.0f}});
    indices.push_back(precision+1);
    indices.push_back(precision+2);
    indices.push_back(precision+3);
    hopper_model_index = mgr->RegisterModel(vertices, indices);
  }
  OldHealth=GetHealth();
}

void GoldHopper::Render() {
  battle_game::SetTransformation(position_, rotation_);
  battle_game::SetTexture(0);
  battle_game::SetColor({1,0.5,0,1});
  battle_game::DrawModel(hopper_model_index);
}

void GoldHopper::RandomRun()
{
  if(!move_count_down_)return;
  float move_speed = (float) move_count_down_ / kTickPerSecond;
  float rotate_angular_speed = glm::radians(180.0f) * ((float)move_count_down_ / kTickPerSecond / 2);
  move_count_down_--;
  auto player = game_core_->GetPlayer(player_id_);
  if (player) {
    glm::vec2 offset{game_core_->RandomFloat()/2+0.5f};
    float speed = move_speed * GetSpeedScale();
    offset *= kSecondPerTick * speed;
    auto new_position =
        position_ + glm::vec2{glm::rotate(glm::mat4{1.0f}, rotation_,
                                          glm::vec3{0.0f, 0.0f, 1.0f}) *
                              glm::vec4{offset, 0.0f, 0.0f}};
    if (!game_core_->IsBlockedByObstacles(new_position)) {
      game_core_->PushEventMoveUnit(id_, new_position);
    }
    float rotation_offset = std::clamp(game_core_->RandomFloat()*3.0f-1.5f,-1.0f,1.0f);
    rotation_offset *= kSecondPerTick * rotate_angular_speed * GetSpeedScale();
    game_core_->PushEventRotateUnit(id_, rotation_ + rotation_offset);
  }
}

void GoldHopper::Bang()
{
  if(!bang_count_down_)return;
  bang_count_down_--;
  if(health_>=0.4)return;
  float t = 0.4 / health_ * bang_count_down_ / kTickPerSecond / 10000;
  if(game_core_->RandomFloat() < t)
  {
    game_core_->PushEventDealDamage(id_,id_,2*GetMaxHealth());
    GenerateBullet<bullet::KillerBall>(position_, 0, GetDamageScale());
  }
}

void GoldHopper::Update()
{
  if(OldHealth>GetHealth()+0.0001)
  {
    uint32_t c=20*kTickPerSecond*(OldHealth-GetHealth());
    bang_count_down_+=c,move_count_down_+=c;
  }
  OldHealth=GetHealth();
  RandomRun();
  Bang();
}

bool GoldHopper::IsHit(glm::vec2 position) const {
  position = WorldToLocal(position);
  return position.x*position.x+position.y*position.y<0.25||(position.y>0&&7*fabs(position.x)+position.y<0.7);
}

float GoldHopper::GetDamageScale() const {
  return 3.0f;
}

float GoldHopper::GetSpeedScale() const {
  return 3.5f;
}

float GoldHopper::BasicMaxHealth() const {
  return 25.0f;
}

const char *GoldHopper::UnitName() const {
  return "GoldHopper";
}

const char *GoldHopper::Author() const {
  return "MyeeYe";
}
}  // namespace battle_game::unit
