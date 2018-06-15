#include "object.h"
#include <glm/gtc/matrix_transform.hpp>
#include <stdexcept>

namespace three {
namespace scene {
    Object::Object(py::array_t<int> np_faces, py::array_t<float> np_vertices, py::tuple color)
    {
        if (np_faces.ndim() != 2) {
            throw std::runtime_error("(np_faces.ndim() != 2) -> false");
        }
        if (np_vertices.ndim() != 2) {
            throw std::runtime_error("(np_vertices.ndim() != 2) -> false");
        }
        if (np_faces.shape(1) != 3) {
            throw std::runtime_error("(np_faces.shape(1) != 3) -> false");
        }
        if (np_vertices.shape(1) != 4) {
            throw std::runtime_error("(np_vertices.shape(1) != 4) -> false");
        }
        _num_faces = np_faces.shape(0);
        _num_vertices = np_vertices.shape(0);
        _faces = std::make_unique<glm::vec3i[]>(_num_faces);
        _vertices = std::make_unique<glm::vec4f[]>(_num_vertices);

        auto faces = np_faces.mutable_unchecked<2>();
        for (ssize_t face_index = 0; face_index < np_faces.shape(0); face_index++) {
            _faces[face_index] = glm::vec3i(faces(face_index, 0), faces(face_index, 1), faces(face_index, 2));
        }

        auto vertices = np_vertices.mutable_unchecked<2>();
        for (ssize_t vertex_index = 0; vertex_index < np_vertices.shape(0); vertex_index++) {
            _vertices[vertex_index] = glm::vec4f(vertices(vertex_index, 0), vertices(vertex_index, 1), vertices(vertex_index, 2), vertices(vertex_index, 3));
        }

        _position = glm::vec3(0.0);
        _rotation_rad = glm::vec3(0.0);
        _update_model_matrix();

        _color[0] = color[0].cast<float>();
        _color[1] = color[1].cast<float>();
        _color[2] = color[2].cast<float>();
        _color[3] = color[3].cast<float>();
    }
    void Object::set_position(py::tuple position)
    {
        _position[0] = position[0].cast<float>();
        _position[1] = position[1].cast<float>();
        _position[2] = position[2].cast<float>();
        _update_model_matrix();
    }
    void Object::set_rotation(py::tuple rotation_rad)
    {
        _rotation_rad[0] = rotation_rad[0].cast<float>();
        _rotation_rad[1] = rotation_rad[1].cast<float>();
        _rotation_rad[2] = rotation_rad[2].cast<float>();
        _update_model_matrix();
    }
    void Object::_update_model_matrix()
    {
        glm::mat4 translation_mat = glm::translate(glm::mat4(), _position);
        glm::mat4 rotation_mat = glm::rotate(glm::mat4(), 1.0f, _rotation_rad);
        _model_matrix = translation_mat * rotation_mat;
    }
}
}