#include "game.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

#include "ecs/sys/movement.hpp"
#include "ecs/sys/system.hpp"
#include "gfx/primitives.hpp"
#include "gfx/shader.hpp"
#include "gfx/texture.hpp"
#include "input/binding.hpp"

namespace {

struct MoveActions {
  inp::Action moveLeft;
  inp::Action moveRight;
  inp::Action moveUp;
  inp::Action moveDown;
};

MoveActions s_move_flags;

uint32_t s_object_ubo;
uint32_t s_camera_ubo;
float s_move_x;
float s_move_y;
sys::System s_movement_system_id;

gfx::Texture s_test_texture;
gfx::Mesh s_mesh;
gfx::ShaderPipeline s_pipeline;

struct CameraData {
  glm::mat4 view_projection;
};

struct ObjectData {
  glm::mat4 model;
};

} // namespace

namespace game {

bool init() {
  s_mesh = msh::generate_primitive();

  auto vert = gfx::create_shader_stage(gfx::ShaderStageBit::VertexBit,
                                       "shaders/shader.vert.spv");

  auto frag = gfx::create_shader_stage(gfx::ShaderStageBit::FragmentBit,
                                       "shaders/shader.frag.spv");

  s_pipeline = gfx::create_shader_pipeline();

  gfx::attach_shader_stage(s_pipeline, vert);
  gfx::attach_shader_stage(s_pipeline, frag);

  s_camera_ubo = gfx::create_uniform_buffer(sizeof(CameraData));
  gfx::bind_uniform_buffer(s_camera_ubo, 0);

  s_object_ubo = gfx::create_uniform_buffer(sizeof(ObjectData));
  gfx::bind_uniform_buffer(s_object_ubo, 1);

  s_test_texture = gfx::load_texture("../assets/textures/grid.jpg");
  if (s_test_texture.id == 0) {
    std::cerr << "Failed to load debug texture!\n";
    return false;
  }

  s_move_flags.moveLeft = inp::create_action("MoveLeft");
  s_move_flags.moveRight = inp::create_action("MoveRight");
  s_move_flags.moveUp = inp::create_action("MoveUp");
  s_move_flags.moveDown = inp::create_action("MoveDown");

  inp::bind(inp::Key::A, s_move_flags.moveLeft);
  inp::bind(inp::Key::D, s_move_flags.moveRight);
  inp::bind(inp::Key::W, s_move_flags.moveUp);
  inp::bind(inp::Key::S, s_move_flags.moveDown);

  inp::bind(inp::Key::LeftArrow, s_move_flags.moveLeft);
  inp::bind(inp::Key::RightArrow, s_move_flags.moveRight);
  inp::bind(inp::Key::UpArrow, s_move_flags.moveUp);
  inp::bind(inp::Key::DownArrow, s_move_flags.moveDown);

  return true;
}

void update(const float ts) {
  if (inp::down(s_move_flags.moveLeft))
    s_move_x -= ts;
  if (inp::down(s_move_flags.moveRight))
    s_move_x += ts;
  if (inp::down(s_move_flags.moveUp))
    s_move_y += ts;
  if (inp::down(s_move_flags.moveDown))
    s_move_y -= ts;
}

void render(const float alpha, const float aspect) {
  gfx::use_shader_pipeline(s_pipeline);
  gfx::bind_texture(s_test_texture, 0);

  CameraData camera{
      .view_projection = glm::ortho(-aspect, aspect, -1.0f, 1.0f, -1.0f, 1.0f),
  };

  gfx::update_uniform_buffer(s_camera_ubo, camera);

  ObjectData object{
      .model = glm::mat4(1.0f),
  };

  object.model =
      glm::translate(object.model, glm::vec3(s_move_x, s_move_y, 0.0f));

  gfx::update_uniform_buffer(s_object_ubo, object);

  gfx::draw_mesh(s_mesh);
}

void shutdown() {
  gfx::destroy_texture(s_test_texture);
  gfx::destroy_uniform_buffer(s_camera_ubo);
  gfx::destroy_uniform_buffer(s_object_ubo);
  gfx::destroy_shader_pipeline(s_pipeline);
  gfx::destroy_mesh(s_mesh);
}

} // namespace game
