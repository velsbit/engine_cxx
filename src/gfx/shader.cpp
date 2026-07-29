#include "gfx/shader.hpp"

#include <glad/gl.h>

#include <fstream>
#include <iosfwd>
#include <stdexcept>
#include <vector>

namespace gfx {

constexpr GLenum to_gl_type(ShaderStageBit stage) {
  switch (stage) {
    case ShaderStageBit::VertexBit:
      return GL_VERTEX_SHADER;
    case ShaderStageBit::FragmentBit:
      return GL_FRAGMENT_SHADER;
    case ShaderStageBit::GeometryBit:
      return GL_GEOMETRY_SHADER;
    case ShaderStageBit::TessControlBit:
      return GL_TESS_CONTROL_SHADER;
    case ShaderStageBit::TessEvaluationBit:
      return GL_TESS_EVALUATION_SHADER;
    case ShaderStageBit::ComputeBit:
      return GL_COMPUTE_SHADER;
  }
  throw std::runtime_error("Unknown shader stage bit");
}

constexpr GLbitfield to_gl_bit(ShaderStageBit stage) {
  switch (stage) {
    case ShaderStageBit::VertexBit:
      return GL_VERTEX_SHADER_BIT;
    case ShaderStageBit::FragmentBit:
      return GL_FRAGMENT_SHADER_BIT;
    case ShaderStageBit::GeometryBit:
      return GL_GEOMETRY_SHADER_BIT;
    case ShaderStageBit::TessControlBit:
      return GL_TESS_CONTROL_SHADER_BIT;
    case ShaderStageBit::TessEvaluationBit:
      return GL_TESS_EVALUATION_SHADER_BIT;
    case ShaderStageBit::ComputeBit:
      return GL_COMPUTE_SHADER_BIT;
  }
  throw std::runtime_error("Unknown shader stage bit");
}

constexpr std::string_view to_string(ShaderStageBit stage) {
  switch (stage) {
    case ShaderStageBit::VertexBit:
      return "Vertex";
    case ShaderStageBit::FragmentBit:
      return "Fragment";
    case ShaderStageBit::GeometryBit:
      return "Geometry";
    case ShaderStageBit::TessControlBit:
      return "TessControl";
    case ShaderStageBit::TessEvaluationBit:
      return "TessEvaluation";
    case ShaderStageBit::ComputeBit:
      return "Compute";
  }
  return "Unknown";
}

std::vector<uint32_t> load_spirv(const char* path) {
  std::ifstream file(path, std::ios::binary | std::ios::ate);

  if (!file) {
    throw std::runtime_error("Failed to open SPIR-V file");
  }

  const std::streamsize size = file.tellg();

  if (size <= 0 || size % sizeof(uint32_t) != 0) {
    throw std::runtime_error("Invalid SPIR-V file size");
  }

  file.seekg(0, std::ios::beg);

  std::vector<uint32_t> spirv(static_cast<size_t>(size) / sizeof(uint32_t));

  if (!file.read(reinterpret_cast<char*>(spirv.data()), size)) {
    throw std::runtime_error("Failed to read SPIR-V file");
  }

  return spirv;
}

void check_shader(uint32_t shader) {
  GLint status = GL_FALSE;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &status);

  if (status == GL_TRUE) {
    return;
  }

  GLint length = 0;
  glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);

  std::vector<char> log(static_cast<size_t>(length));
  glGetShaderInfoLog(shader, length, nullptr, log.data());

  throw std::runtime_error(log.data());
}

void check_program(uint32_t program) {
  GLint status = GL_FALSE;
  glGetProgramiv(program, GL_LINK_STATUS, &status);

  if (status == GL_TRUE) {
    return;
  }

  GLint length = 0;
  glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);

  std::vector<char> log(static_cast<size_t>(length));
  glGetProgramInfoLog(program, length, nullptr, log.data());

  throw std::runtime_error(log.data());
}

ShaderStage create_shader_stage(ShaderStageBit stageBit, const char* spvPath) {
  const GLenum shaderType = to_gl_type(stageBit);
  const std::vector<uint32_t> spirv = load_spirv(spvPath);

  const uint32_t shader = glCreateShader(shaderType);

  glShaderBinary(1, &shader, GL_SHADER_BINARY_FORMAT_SPIR_V, spirv.data(),
                 static_cast<GLsizei>(spirv.size() * sizeof(uint32_t)));

  glSpecializeShader(shader, "main", 0, nullptr, nullptr);

  try {
    check_shader(shader);
  } catch (const std::exception& e) {
    glDeleteShader(shader);
    throw std::runtime_error(std::string(to_string(stageBit)) +
                             " shader error: " + e.what());
  }

  const uint32_t program = glCreateProgram();

  glProgramParameteri(program, GL_PROGRAM_SEPARABLE, GL_TRUE);

  glAttachShader(program, shader);
  glLinkProgram(program);
  glDetachShader(program, shader);
  glDeleteShader(shader);

  try {
    check_program(program);
  } catch (...) {
    glDeleteProgram(program);
    throw;
  }

  return {.programID = program, .stageBit = stageBit};
}

ShaderPipeline create_shader_pipeline() {
  ShaderPipeline pipeline;
  glCreateProgramPipelines(1, &pipeline.pipelineID);
  return pipeline;
}

void attach_shader_stage(const ShaderPipeline& pipeline,
                         const ShaderStage& stage) {
  glUseProgramStages(pipeline.pipelineID, to_gl_bit(stage.stageBit),
                     stage.programID);
}

void use_shader_pipeline(const ShaderPipeline& pipeline) {
  glBindProgramPipeline(pipeline.pipelineID);
}

void unbind_shader_pipeline() { glBindProgramPipeline(0); }

uint32_t create_uniform_buffer(uint32_t size) {
  uint32_t ubo_id;
  glGenBuffers(1, &ubo_id);
  glBindBuffer(GL_UNIFORM_BUFFER, ubo_id);
  glBufferData(GL_UNIFORM_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
  glBindBuffer(GL_UNIFORM_BUFFER, 0);
  return ubo_id;
}

void bind_uniform_buffer(uint32_t ubo_id, uint32_t binding_point) {
  glBindBufferBase(GL_UNIFORM_BUFFER, binding_point, ubo_id);
}

void update_uniform_buffer(uint32_t ubo_id, uint32_t byte_offset, uint32_t size,
                           const void* data) {
  glBindBuffer(GL_UNIFORM_BUFFER, ubo_id);
  glBufferSubData(GL_UNIFORM_BUFFER, byte_offset, size, data);
  glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void destroy_uniform_buffer(uint32_t ubo_id) {
  if (ubo_id != 0) {
    glDeleteBuffers(1, &ubo_id);
  }
}

void destroy_shader_stage(ShaderStage& stage) {
  if (stage.programID == 0) {
    return;
  }

  glDeleteProgram(stage.programID);
  stage.programID = 0;
  stage.stageBit = ShaderStageBit::VertexBit;
}

void destroy_shader_pipeline(ShaderPipeline& pipeline) {
  if (pipeline.pipelineID == 0) {
    return;
  }

  glDeleteProgramPipelines(1, &pipeline.pipelineID);
  pipeline.pipelineID = 0;
}

}  // namespace gfx