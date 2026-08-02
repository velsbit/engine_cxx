#include "game.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

#include "core/input.hpp"
#include "gfx/primitives.hpp"
#include "gfx/shader.hpp"

struct MoveActions {
  inp::Action moveLeft;
  inp::Action moveRight;
  inp::Action moveUp;
  inp::Action moveDown;
};

MoveActions s_move_flags;

static gfx::Mesh s_mesh;
static gfx::ShaderPipeline s_pipeline;
static uint32_t s_object_ubo;
static uint32_t s_camera_ubo;
static float s_move_x;
static float s_move_y;

struct CameraData {
  glm::mat4 view_projection;
};

struct ObjectData {
  glm::mat4 model;
};

void Game::register_systems() {
  // Stage: Input (External)

  // Stage: AI Logic (Internal)

  // Stage: Logic & State Machines

  // Stage: Physics & Movement
  // m_systems.push_back(std::make_unique<sys::MovementSystem>());

  // Stage: Event Processing

  // Stage: Cleanup & Lifecycle
}

bool Game::init() {
  m_active_scene = std::make_unique<Scene>();

  s_mesh = msh::generate_primitive(msh::cfg::Star{});

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

  s_move_flags.moveLeft = inp::create_action("MoveLeft");
  s_move_flags.moveRight = inp::create_action("MoveRight");
  s_move_flags.moveUp = inp::create_action("MoveUp");
  s_move_flags.moveDown = inp::create_action("MoveDown");

  inp::bind_scancode(s_move_flags.moveLeft, "a");
  inp::bind_scancode(s_move_flags.moveRight, "d");
  inp::bind_scancode(s_move_flags.moveUp, "w", inp::Modifier::Ctrl);
  inp::bind_scancode(s_move_flags.moveDown, "s");

  register_systems();

  return true;
}

void Game::update(const float ts) {
  if (!m_active_scene) return;

  for (auto& system : m_systems) {
    system->update(*m_active_scene, ts);
  }

  if (inp::down(s_move_flags.moveLeft)) s_move_x -= ts;
  if (inp::down(s_move_flags.moveRight)) s_move_x += ts;
  if (inp::pressed(s_move_flags.moveUp)) s_move_y += 0.1f;
  if (inp::pressed(s_move_flags.moveDown)) s_move_y -= 0.1f;
}

void Game::render(const float alpha, const float aspect) {
  gfx::use_shader_pipeline(s_pipeline);

  CameraData camera{.view_projection =
                        glm::ortho(-aspect, aspect, -1.0f, 1.0f, -1.0f, 1.0f)};

  gfx::update_uniform_buffer(s_camera_ubo, camera);

  ObjectData object{
      .model = glm::mat4(1.0f),
  };

  object.model =
      glm::translate(object.model, glm::vec3(s_move_x, s_move_y, 0.0f));

  gfx::update_uniform_buffer(s_object_ubo, object);

  gfx::draw_mesh(s_mesh);
}

void Game::shutdown() {
  gfx::destroy_uniform_buffer(s_camera_ubo);
  gfx::destroy_uniform_buffer(s_object_ubo);
  gfx::destroy_shader_pipeline(s_pipeline);
  gfx::destroy_mesh(s_mesh);

  m_systems.clear();
  m_active_scene.reset();
}