#include "gfx/mesh.hpp"

#include <glad/gl.h>

#include "gfx/mesh_compiler.hpp"

namespace gfx {

namespace {

uint32_t format_components(VertexFormat format) {
  switch (format) {
    case VertexFormat::Float:
    case VertexFormat::UInt:
      return 1;

    case VertexFormat::Float2:
    case VertexFormat::UInt2:
      return 2;

    case VertexFormat::Float3:
    case VertexFormat::UInt3:
      return 3;

    case VertexFormat::Float4:
    case VertexFormat::UInt4:
      return 4;
  }

  return 0;
}

GLenum format_type(VertexFormat format) {
  switch (format) {
    case VertexFormat::Float:
    case VertexFormat::Float2:
    case VertexFormat::Float3:
    case VertexFormat::Float4:
      return GL_FLOAT;

    case VertexFormat::UInt:
    case VertexFormat::UInt2:
    case VertexFormat::UInt3:
    case VertexFormat::UInt4:
      return GL_UNSIGNED_INT;
  }

  return GL_FLOAT;
}

uint32_t format_size(VertexFormat format) {
  switch (format) {
    case VertexFormat::Float:
      return sizeof(float);
    case VertexFormat::Float2:
      return sizeof(float) * 2;
    case VertexFormat::Float3:
      return sizeof(float) * 3;
    case VertexFormat::Float4:
      return sizeof(float) * 4;

    case VertexFormat::UInt:
      return sizeof(uint32_t);
    case VertexFormat::UInt2:
      return sizeof(uint32_t) * 2;
    case VertexFormat::UInt3:
      return sizeof(uint32_t) * 3;
    case VertexFormat::UInt4:
      return sizeof(uint32_t) * 4;
  }

  return 0;
}

}  // namespace

Mesh create_mesh(const void* vertices, uint32_t vertex_buffer_size,
                 uint32_t vertex_stride, const uint32_t* indices,
                 uint32_t index_count, const VertexLayout& layout) {
  Mesh mesh{};

  mesh.index_count = index_count;

  glCreateVertexArrays(1, &mesh.vao_id);

  glCreateBuffers(1, &mesh.vbo_id);
  glNamedBufferData(mesh.vbo_id, vertex_buffer_size, vertices, GL_STATIC_DRAW);

  glVertexArrayVertexBuffer(mesh.vao_id, 0, mesh.vbo_id, 0, vertex_stride);

  if (indices && index_count) {
    glCreateBuffers(1, &mesh.ebo_id);

    glNamedBufferData(mesh.ebo_id, sizeof(uint32_t) * index_count, indices,
                      GL_STATIC_DRAW);

    glVertexArrayElementBuffer(mesh.vao_id, mesh.ebo_id);
  }

  uint32_t offset = 0;

  for (uint32_t i = 0; i < layout.attribute_count; ++i) {
    const auto& attr = layout.attributes[i];

    glEnableVertexArrayAttrib(mesh.vao_id, attr.location);

    glVertexArrayAttribBinding(mesh.vao_id, attr.location, 0);

    glVertexArrayAttribFormat(mesh.vao_id, attr.location,
                              format_components(attr.format),
                              format_type(attr.format), GL_FALSE, offset);

    offset += format_size(attr.format);
  }

  return mesh;
}

Mesh create_mesh(const CompiledMesh& mesh) {
  return create_mesh(mesh.vertex_data, mesh.vertex_buffer_size,
                     mesh.vertex_stride, mesh.indices, mesh.index_count,
                     *mesh.layout);
}

void draw_mesh(const Mesh& mesh) {
  glBindVertexArray(mesh.vao_id);

  if (mesh.ebo_id) {
    glDrawElements(GL_TRIANGLES, mesh.index_count, GL_UNSIGNED_INT, nullptr);
  }

  glBindVertexArray(0);
}

void destroy_mesh(Mesh& mesh) {
  if (mesh.ebo_id) glDeleteBuffers(1, &mesh.ebo_id);

  if (mesh.vbo_id) glDeleteBuffers(1, &mesh.vbo_id);

  if (mesh.vao_id) glDeleteVertexArrays(1, &mesh.vao_id);

  mesh = {};
}

}  // namespace gfx