/*!
 * @file
 * @brief This file contains class that represents graphic card.
 *
 * @author Tomáš Milet, imilet@fit.vutbr.cz
 */
#pragma once

#include <student/fwd.hpp>
#include <map>
#include <vector>
#include <set>

using namespace std;

/**
 * @brief This class represent software GPU
 */
class GPU{
    
  public:
    GPU();
    virtual ~GPU();

    //buffer object commands
    BufferID  createBuffer           (uint64_t size);
    void      deleteBuffer           (BufferID buffer);
    void      setBufferData          (BufferID buffer,uint64_t offset,uint64_t size,void const* data);
    void      getBufferData          (BufferID buffer,uint64_t offset,uint64_t size,void      * data);
    bool      isBuffer               (BufferID buffer);

    //vertex array object commands (vertex puller)
    ObjectID  createVertexPuller     ();
    void      deleteVertexPuller     (VertexPullerID vao);
    void      setVertexPullerHead    (VertexPullerID vao,uint32_t head,AttributeType type,uint64_t stride,uint64_t offset,BufferID buffer);
    void      setVertexPullerIndexing(VertexPullerID vao,IndexType type,BufferID buffer);
    void      enableVertexPullerHead (VertexPullerID vao,uint32_t head);
    void      disableVertexPullerHead(VertexPullerID vao,uint32_t head);
    void      bindVertexPuller       (VertexPullerID vao);
    void      unbindVertexPuller     ();
    bool      isVertexPuller         (VertexPullerID vao);

    //program object commands
    ProgramID createProgram          ();
    void      deleteProgram          (ProgramID prg);
    void      attachShaders          (ProgramID prg,VertexShader vs,FragmentShader fs);
    void      setVS2FSType           (ProgramID prg,uint32_t attrib,AttributeType type);
    void      useProgram             (ProgramID prg);
    bool      isProgram              (ProgramID prg);
    void      programUniform1f       (ProgramID prg,uint32_t uniformId,float     const&d);
    void      programUniform2f       (ProgramID prg,uint32_t uniformId,glm::vec2 const&d);
    void      programUniform3f       (ProgramID prg,uint32_t uniformId,glm::vec3 const&d);
    void      programUniform4f       (ProgramID prg,uint32_t uniformId,glm::vec4 const&d);
    void      programUniformMatrix4f (ProgramID prg,uint32_t uniformId,glm::mat4 const&d);

    //framebuffer functions
    void      createFramebuffer      (uint32_t width,uint32_t height);
    void      deleteFramebuffer      ();
    void      resizeFramebuffer      (uint32_t width,uint32_t height);
    uint8_t*  getFramebufferColor    ();
    float*    getFramebufferDepth    ();
    uint32_t  getFramebufferWidth    ();
    uint32_t  getFramebufferHeight   ();

    //execution commands
    void      clear                  (float r,float g,float b,float a);
    void      drawTriangles          (uint32_t  nofVertices);

    /// \addtogroup gpu_init 00. proměnné, inicializace / deinicializace grafické karty
    /// @{
    /// \todo zde si můžete vytvořit proměnné grafické karty (buffery, programy, ...)
    /// @}

private:
    struct PrimitiveTriangle
    {
        OutVertex a;
        OutVertex b;
        OutVertex c;
    };

    InVertex vertexPuller();
    PrimitiveTriangle primitiveAssembly();
    OutVertex perspectiveDivision(OutVertex &vertex);
    OutVertex viewPortTransformation(OutVertex &vertex);
    void rasterize(PrimitiveTriangle &triangle);
    void interpolate(InFragment &fragment, glm::vec2 &p, OutVertex &a, OutVertex &b, OutVertex &c);


    map<BufferID, vector<uint8_t>> bufferMap;
    BufferID bufferCount;

    //region Vertex Puller Data
    struct indexingData
    {
        indexingData()
        {
            bufferId = emptyID;
            indexType = IndexType::UINT8;
        }

        BufferID bufferId;
        IndexType indexType;
    };

    struct VertexHead{
        VertexHead()
        {
            bufferId = emptyID;
            offset = 0;
            stride = 0;
            attType = AttributeType::EMPTY;
            enabled = false;
        }

        BufferID bufferId;
        uint64_t offset;
        uint64_t stride;
        AttributeType attType;
        bool enabled;
    };

    struct VertexPullerData
    {
        indexingData indexing;
        VertexHead head[maxAttributes];
    };

    map<VertexPullerID, VertexPullerData> vertexPullerMap;

    VertexPullerID activePuller;

    unsigned int vertPullInvCount;
    VertexPullerID pullerCount;
    //endregion

    //region Shader program
    struct ProgramSettings
    {
        ProgramSettings()
        = default;

        VertexShader vertexShader;
        FragmentShader fragmentShader;
        Uniforms uniforms;
        AttributeType attributeType[maxAttributes];
        //uint32_t attribId;
    };
    map<ProgramID, ProgramSettings> programMap;

    ProgramID activeProgram;
    ProgramID programIdCount;
    //endregion

    //region Frame buffer
    struct RGBColor
    {
        uint8_t r;
        uint8_t g;
        uint8_t b;
        uint8_t a;
    };

    vector<RGBColor> ColorBuffer;
    vector<float> DepthBuffer;
    uint32_t frWidth, frHeight;
    //endregion

    //region Primitive assembly
    vector<OutVertex> outVertexBuffer;
    //endregion
    //TODO
    set<BufferID> unUsedBufferIds;
    set<BufferID> usedBufferIds;
};


