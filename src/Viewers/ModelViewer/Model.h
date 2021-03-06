/** \file       Viewers/ModelViewer/Model.h
 *  \brief      Contains the declaration of the model class.
 *  \author     Khralkatorrix
 */

/**
 * Copyright (C) 2017 Khralkatorrix <https://github.com/kytulendu>
 *
 * This file is part of Gw2Browser.
 *
 * Gw2Browser is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#ifndef VIEWERS_MODELVIEWER_MODEL_H_INCLUDED
#define VIEWERS_MODELVIEWER_MODEL_H_INCLUDED

#include <map>
#include <vector>

#include "IndexBuffer.h"
#include "VertexBuffer.h"
#include "TextureManager.h"
#include "Readers/ModelReader.h"

namespace gw2b {

    class Model {
        typedef std::unique_ptr<IndexBuffer> IBO;
        typedef std::unique_ptr<VertexBuffer> VBO;

        struct MeshCache {
            std::vector<glm::vec3>  vertices;
            std::vector<glm::vec3>  normals;
            std::vector<glm::vec2>  uvs;
            std::vector<uint>       indices;
            std::vector<glm::vec3>  tangents;
            int                     materialIndex;
        };

        struct TextureList {
            uint                    diffuseMap;
            uint                    normalMap;
            uint                    lightMap;
        };

        struct PackedVertex {
            glm::vec3 position;
            glm::vec3 normal;
            glm::vec2 uv;

            bool operator < ( const PackedVertex& that ) const {
                return memcmp( ( void* )this, ( void* ) &that, sizeof( PackedVertex ) ) > 0;
            };
        };

        // Mesh
        std::vector<MeshCache>      m_meshCache;
        std::vector<VBO>            m_vertexBuffer;     // Vertex Buffer Object
        std::vector<IBO>            m_indexBuffer;      // Index Buffer Object

        // Textures
        std::vector<TextureList>    m_textureList;      // Texture List, store texture list from material of GW2Model

        size_t                      m_numMeshes;
        size_t                      m_numVertices;
        size_t                      m_numTriangles;
        Bounds                      m_bounds;           // Model's bounds

    public:
        /** Constructor. Create model. */
        Model( const GW2Model& p_model );
        /** Destructor. Clears all data. */
        ~Model( );

        /** Draw the model. */
        void draw( TextureManager& p_textureManager );
        /** Get number of mesh. */
        size_t getNumMeshes( ) const;
        /** Get number of vertices. */
        size_t getVertSize( ) const;
        /** Get number of trangles. */
        size_t getTriSize( ) const;
        /** Get model bounds. */
        Bounds getBounds( ) const;

    private:
        void clearBuffer( );
        void drawMesh( const uint p_meshIndex );
        void loadModel( const GW2Model& p_model );
        void loadMesh( MeshCache& p_cache, const GW2Mesh& p_mesh );
        void computeTangent( MeshCache& p_mesh );
        bool getSimilarVertexIndex( PackedVertex& p_packed, std::map<PackedVertex, uint>& p_vertexToOutIndex, uint& p_result );
        void indexVBO( const MeshCache& p_inMesh, MeshCache& p_outMesh );
        void loadMaterial( const GW2Model& p_model );

    }; // class Model

}; // namespace gw2b

#endif // VIEWERS_MODELVIEWER_MODEL_H_INCLUDED
