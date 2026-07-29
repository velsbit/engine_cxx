#include "gfx/mesh_compiler.hpp"

#include <cstring>

namespace gfx {

namespace {

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

const VertexStream* find_stream(const MeshSource& source,
                                VertexSemantic semantic) {
  for (uint32_t i = 0; i < source.stream_count; ++i) {
    if (source.streams[i].semantic == semantic) return &source.streams[i];
  }

  return nullptr;
}

}  // namespace

CompiledMesh compile_mesh(const MeshSource& source,
                          const VertexLayout& layout) {
  CompiledMesh mesh{};

  for (uint32_t i = 0; i < layout.attribute_count; ++i) {
    const VertexStream* stream =
        find_stream(source, layout.attributes[i].semantic);

    if (!stream) continue;

    mesh.vertex_stride += format_size(stream->format);
  }

  mesh.vertex_buffer_size = mesh.vertex_stride * source.vertex_count;

  uint8_t* vertex_data = new uint8_t[mesh.vertex_buffer_size];

  for (uint32_t vertex = 0; vertex < source.vertex_count; ++vertex) {
    uint8_t* dst = vertex_data + vertex * mesh.vertex_stride;

    for (uint32_t attr = 0; attr < layout.attribute_count; ++attr) {
      const VertexStream* stream =
          find_stream(source, layout.attributes[attr].semantic);

      if (!stream) continue;

      uint32_t size = format_size(stream->format);

      const uint8_t* src =
          static_cast<const uint8_t*>(stream->data) + vertex * stream->stride;

      std::memcpy(dst, src, size);

      dst += size;
    }
  }

  mesh.vertex_data = vertex_data;
  mesh.layout = &layout;

  if (source.index_count) {
    uint32_t* indices = new uint32_t[source.index_count];

    std::memcpy(indices, source.indices, source.index_count * sizeof(uint32_t));

    mesh.indices = indices;
    mesh.index_count = source.index_count;
  }

  return mesh;
}

void destroy_compiled_mesh(CompiledMesh& mesh) {
  delete[] static_cast<uint8_t*>(mesh.vertex_data);
  delete[] mesh.indices;

  mesh.vertex_data = nullptr;
  mesh.indices = nullptr;

  mesh.vertex_buffer_size = 0;
  mesh.vertex_stride = 0;
  mesh.index_count = 0;
}

}  // namespace gfx