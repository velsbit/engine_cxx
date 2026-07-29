// gfx/shader.hpp
#pragma once

#include <cstdint>

namespace gfx {

enum class ShaderStageBit : uint32_t {
  None = 0,
  VertexBit = 0x00000001,
  FragmentBit = 0x00000002,
  GeometryBit = 0x00000004,
  TessControlBit = 0x00000008,
  TessEvaluationBit = 0x00000010,
  ComputeBit = 0x00000020
};

inline ShaderStageBit operator|(ShaderStageBit a, ShaderStageBit b) {
  return static_cast<ShaderStageBit>(static_cast<uint32_t>(a) |
                                     static_cast<uint32_t>(b));
}

inline ShaderStageBit operator&(ShaderStageBit a, ShaderStageBit b) {
  return static_cast<ShaderStageBit>(static_cast<uint32_t>(a) &
                                     static_cast<uint32_t>(b));
}

struct ShaderStage {
  uint32_t programID = 0;
  ShaderStageBit stageBit;
};

struct ShaderPipeline {
  uint32_t pipelineID = 0;
};

ShaderStage create_shader_stage(ShaderStageBit stageBit, const char* spvPath);

ShaderPipeline create_shader_pipeline();

void attach_shader_stage(const ShaderPipeline& pipeline,
                         const ShaderStage& stage);

void use_shader_pipeline(const ShaderPipeline& pipeline);
void unbind_shader_pipeline();

uint32_t create_uniform_buffer(uint32_t size);

void bind_uniform_buffer(uint32_t ubo_id, uint32_t binding_point);

void update_uniform_buffer(uint32_t ubo_id, uint32_t byte_offset, uint32_t size,
                           const void* data);

template <typename T>
void update_uniform_buffer(uint32_t ubo_id, const T& data) {
  update_uniform_buffer(ubo_id, 0, sizeof(T), &data);
}
void destroy_uniform_buffer(uint32_t ubo_id);

void destroy_shader_stage(ShaderStage& stage);
void destroy_shader_pipeline(ShaderPipeline& pipeline);

}  // namespace gfx