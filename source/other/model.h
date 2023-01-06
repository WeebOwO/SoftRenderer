/*
 * come from https://github.com/skywind3000/RenderHelp
 */

#pragma once

#include <fstream>
#include <iostream>
#include <sstream>

#include "bitmap.h"
#include "math.h"

class Model {
   public:
    inline virtual ~Model() {
        if (m_diffusemap) delete m_diffusemap;
        if (m_normalmap) delete m_normalmap;
        if (m_specularmap) delete m_specularmap;
    }

    inline Model(const char *filename) {
        m_diffusemap = NULL;
        m_normalmap = NULL;
        m_specularmap = NULL;
        std::ifstream in;
        in.open(filename, std::ifstream::in);
        if (in.fail()) return;
        std::string line;
        while (!in.eof()) {
            std::getline(in, line);
            std::istringstream iss(line.c_str());
            char trash;
            if (line.compare(0, 2, "v ") == 0) {
                iss >> trash;
                Vec3f v;
                for (int i = 0; i < 3; i++) iss >> v[i];
                m_verts.push_back(v);
            } else if (line.compare(0, 3, "vn ") == 0) {
                iss >> trash >> trash;
                Vec3f n;
                for (int i = 0; i < 3; i++) iss >> n[i];
                m_norms.push_back(n);
            } else if (line.compare(0, 3, "vt ") == 0) {
                iss >> trash >> trash;
                Vec2f uv;
                iss >> uv[0] >> uv[1];
                m_uv.push_back(uv);
            } else if (line.compare(0, 2, "f ") == 0) {
                std::vector<Vec3i> f;
                Vec3i tmp;
                iss >> trash;
                while (iss >> tmp[0] >> trash >> tmp[1] >> trash >> tmp[2]) {
                    for (int i = 0; i < 3; i++) tmp[i]--;
                    f.push_back(tmp);
                }
                m_faces.push_back(f);
            }
        }
        std::cout << "# v# " << m_verts.size() << " f# " << m_faces.size()
                  << "\n";
        m_diffusemap = load_texture(filename, "_diffuse.bmp");
        m_normalmap = load_texture(filename, "_nm.bmp");
        m_specularmap = load_texture(filename, "_spec.bmp");
    }

   public:
    inline int nverts() const { return (int)m_verts.size(); }
    inline int nfaces() const { return (int)m_faces.size(); }

    inline std::vector<int> face(int idx) const {
        std::vector<int> face;
        for (int i = 0; i < (int)m_faces[idx].size(); i++)
            face.push_back(m_faces[idx][i][0]);
        return face;
    }

    inline Vec3f vert(int i) const { return m_verts[i]; }
    inline Vec3f vert(int iface, int nthvert) {
        return m_verts[m_faces[iface][nthvert][0]];
    }

    inline Vec2f uv(int iface, int nthvert) const {
        return m_uv[m_faces[iface][nthvert][1]];
    }

    inline Vec3f normal(int iface, int nthvert) const {
        int idx = m_faces[iface][nthvert][2];
        return vector_normalize(m_norms[idx]);
    }

    inline Vec4f diffuse(Vec2f uv) const {
        assert(m_diffusemap);
        return m_diffusemap->Sample2D(uv);
    }

    inline Vec3f normal(Vec2f uv) const {
        assert(m_normalmap);
        Vec4f color = m_normalmap->Sample2D(uv);
        for (int i = 0; i < 3; i++) color[i] = color[i] * 2.0f - 1.0f;
        return {color[0], color[1], color[2]};
    }

    inline float Specular(Vec2f uv) {
        Vec4f color = m_specularmap->Sample2D(uv);
        return color.b;
    }

   protected:
    Bitmap *load_texture(std::string filename, const char *suffix) {
        std::string texfile(filename);
        size_t dot = texfile.find_last_of(".");
        if (dot == std::string::npos) return NULL;
        texfile = texfile.substr(0, dot) + std::string(suffix);
        Bitmap *texture = Bitmap::LoadFile(texfile.c_str());
        std::cout << "loading: " << texfile << ((texture) ? " OK" : " failed")
                  << "\n";
        texture->FlipVertical();
        return texture;
    }

   protected:
    std::vector<Vec3f> m_verts;
    std::vector<std::vector<Vec3i>> m_faces;
    std::vector<Vec3f> m_norms;
    std::vector<Vec2f> m_uv;
    Bitmap *m_diffusemap;
    Bitmap *m_normalmap;
    Bitmap *m_specularmap;
};
