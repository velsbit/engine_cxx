#pragma once

#include "gfx/mesh_source.hpp"

namespace gfx {

struct VertexAttribute {
  uint32_t location;
  VertexFormat format;
  VertexSemantic semantic;
};

struct VertexLayout {
  const VertexAttribute* attributes;
  uint32_t attribute_count;
};

struct CompiledMesh {
  void* vertex_data = nullptr;

  uint32_t vertex_buffer_size = 0;
  uint32_t vertex_stride = 0;

  uint32_t* indices = nullptr;
  uint32_t index_count = 0;

  const VertexLayout* layout = nullptr;
};

CompiledMesh compile_mesh(const MeshSource& source, const VertexLayout& layout);

void destroy_compiled_mesh(CompiledMesh& mesh);

}  // namespace gfx