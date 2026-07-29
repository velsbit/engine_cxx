#include "gfx/primitives.hpp"

#include <algorithm>
#include <cmath>
#include <vector>

#include "gfx/mesh_compiler.hpp"
#include "gfx/mesh_source.hpp"

namespace msh {
using namespace gfx;

namespace {

constexpr float PI = 3.14159265358979323846f;

constexpr VertexAttribute kAttributes[] = {
    {
        .location = 0,
        .format = VertexFormat::Float2,
        .semantic = VertexSemantic::Position,
    },
    {
        .location = 1,
        .format = VertexFormat::Float2,
        .semantic = VertexSemantic::UVCoord,
    },
};

Mesh create_mesh_from_data(const VertexStream* streams, uint32_t stream_count,
                           const uint32_t* indices, uint32_t index_count,
                           uint32_t vertex_count) {
  MeshSource source{
      .streams = streams,
      .stream_count = stream_count,
      .indices = indices,
      .index_count = index_count,
      .vertex_count = vertex_count,
  };

  VertexLayout layout{
      .attributes = kAttributes,
      .attribute_count = std::size(kAttributes),
  };

  return create_mesh(compile_mesh(source, layout));
}

}  // namespace

Mesh generate_primitive(const cfg::Quad& config) {
  const float hw = config.width * 0.5f;
  const float hh = config.height * 0.5f;

  const float positions[] = {
      -hw, -hh, hw, -hh, hw, hh, -hw, hh,
  };

  const float uvs[] = {
      0.f, 0.f, 1.f, 0.f, 1.f, 1.f, 0.f, 1.f,
  };

  const uint32_t indices[] = {
      0, 1, 2, 2, 3, 0,
  };

  VertexStream streams[] = {
      {
          .semantic = VertexSemantic::Position,
          .format = VertexFormat::Float2,
          .data = positions,
          .stride = sizeof(float) * 2,
      },
      {
          .semantic = VertexSemantic::UVCoord,
          .format = VertexFormat::Float2,
          .data = uvs,
          .stride = sizeof(float) * 2,
      },
  };

  return create_mesh_from_data(streams, 2, indices, 6, 4);
}

Mesh generate_primitive(const cfg::Circle& config) {
  const uint32_t segments = std::max(config.segments, 3u);

  std::vector<float> positions;
  std::vector<float> uvs;
  std::vector<uint32_t> indices;

  positions.reserve((segments + 2) * 2);
  uvs.reserve((segments + 2) * 2);
  indices.reserve(segments * 3);

  positions.push_back(0.f);
  positions.push_back(0.f);

  uvs.push_back(0.5f);
  uvs.push_back(0.5f);

  for (uint32_t i = 0; i <= segments; ++i) {
    float angle = 2.f * PI * float(i) / float(segments);

    float x = std::cos(angle) * config.radius;
    float y = std::sin(angle) * config.radius;

    positions.push_back(x);
    positions.push_back(y);

    uvs.push_back(x / (config.radius * 2.f) + 0.5f);
    uvs.push_back(y / (config.radius * 2.f) + 0.5f);
  }

  for (uint32_t i = 1; i <= segments; ++i) {
    indices.push_back(0);
    indices.push_back(i);
    indices.push_back(i + 1);
  }

  VertexStream streams[] = {
      {
          .semantic = VertexSemantic::Position,
          .format = VertexFormat::Float2,
          .data = positions.data(),
          .stride = sizeof(float) * 2,
      },
      {
          .semantic = VertexSemantic::UVCoord,
          .format = VertexFormat::Float2,
          .data = uvs.data(),
          .stride = sizeof(float) * 2,
      },
  };

  return create_mesh_from_data(streams, 2, indices.data(),
                               static_cast<uint32_t>(indices.size()),
                               static_cast<uint32_t>(positions.size() / 2));
}

Mesh generate_primitive(const cfg::Star& config) {
  const uint32_t points = std::max(config.points, 2u);

  std::vector<float> positions;
  std::vector<float> uvs;
  std::vector<uint32_t> indices;

  positions.reserve((points * 2 + 2) * 2);
  uvs.reserve((points * 2 + 2) * 2);

  positions.push_back(0.f);
  positions.push_back(0.f);

  uvs.push_back(0.5f);
  uvs.push_back(0.5f);

  const uint32_t ring_vertices = points * 2;

  for (uint32_t i = 0; i <= ring_vertices; ++i) {
    float angle = 2.f * PI * float(i) / float(ring_vertices);

    float radius = (i % 2 == 0) ? config.outer_radius : config.inner_radius;

    float x = std::cos(angle) * radius;
    float y = std::sin(angle) * radius;

    positions.push_back(x);
    positions.push_back(y);

    uvs.push_back(x / (config.outer_radius * 2.f) + 0.5f);
    uvs.push_back(y / (config.outer_radius * 2.f) + 0.5f);
  }

  indices.reserve(ring_vertices * 3);

  for (uint32_t i = 1; i <= ring_vertices; ++i) {
    indices.push_back(0);
    indices.push_back(i);
    indices.push_back(i + 1);
  }

  VertexStream streams[] = {
      {
          .semantic = VertexSemantic::Position,
          .format = VertexFormat::Float2,
          .data = positions.data(),
          .stride = sizeof(float) * 2,
      },
      {
          .semantic = VertexSemantic::UVCoord,
          .format = VertexFormat::Float2,
          .data = uvs.data(),
          .stride = sizeof(float) * 2,
      },
  };

  return create_mesh_from_data(streams, 2, indices.data(),
                               static_cast<uint32_t>(indices.size()),
                               static_cast<uint32_t>(positions.size() / 2));
}

}  // namespace msh