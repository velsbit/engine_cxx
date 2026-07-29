#pragma once

#include <cstdint>

namespace gfx {

struct CompiledMesh;
struct VertexLayout;

struct Mesh {
  uint32_t vao_id = 0;
  uint32_t vbo_id = 0;
  uint32_t ebo_id = 0;

  uint32_t index_count = 0;
};

Mesh create_mesh(const void* vertices, uint32_t vertex_buffer_size,
                 uint32_t vertex_stride, const uint32_t* indices,
                 uint32_t index_count, const VertexLayout& layout);

Mesh create_mesh(const CompiledMesh& mesh);

void draw_mesh(const Mesh& mesh);

void destroy_mesh(Mesh& mesh);

}  // namespace gfx