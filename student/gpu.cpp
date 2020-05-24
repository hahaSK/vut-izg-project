/*!
 * @file
 * @brief This file contains implementation of gpu
 *
 * @author Tomáš Milet, imilet@fit.vutbr.cz
 */

#include <student/gpu.hpp>
#include <cstring>
#include <iostream>


/// \addtogroup gpu_init
/// @{

/**
 * @brief Constructor of GPU
 */
GPU::GPU(){
  /// \todo Zde můžete alokovat/inicializovat potřebné proměnné grafické karty
  bufferCount = 0;
  vertPullInvCount = 0;
  pullerCount = 0;
  programIdCount = 0;
}

/**
 * @brief Destructor of GPU
 */
GPU::~GPU(){
  /// \todo Zde můžete dealokovat/deinicializovat grafickou kartu
}

/// @}

/** \addtogroup buffer_tasks 01. Implementace obslužných funkcí pro buffery
 * @{
 */

/**
 * @brief This function allocates buffer on GPU.
 *
 * @param size size in bytes of new buffer on GPU.
 *
 * @return unique identificator of the buffer
 */
BufferID GPU::createBuffer(uint64_t size) { 
  /// \todo Tato funkce by měla na grafické kartě vytvořit buffer dat.<br>
  /// Velikost bufferu je v parameteru size (v bajtech).<br>
  /// Funkce by měla vrátit unikátní identifikátor identifikátor bufferu.<br>
  /// Na grafické kartě by mělo být možné alkovat libovolné množství bufferů o libovolné velikosti.<br>

  //TODO vytvoriť ID tak, aby keď sa vymaže tak sa použilo zase.
  bufferMap.insert(make_pair(bufferCount, vector<uint8_t>(size)));
  return bufferCount++;
}

/**
 * @brief This function frees allocated buffer on GPU.
 *
 * @param buffer buffer identificator
 */
void GPU::deleteBuffer(BufferID buffer) {
  /// \todo Tato funkce uvolní buffer na grafické kartě.
  /// Buffer pro smazání je vybrán identifikátorem v parameteru "buffer".
  /// Po uvolnění bufferu je identifikátor volný a může být znovu použit při vytvoření nového bufferu.

  if (!isBuffer(buffer))
      return;

  bufferMap.erase(bufferMap.find(buffer));
}

/**
 * @brief This function uploads data to selected buffer on the GPU
 *
 * @param buffer buffer identificator
 * @param offset specifies the offset into the buffer's data
 * @param size specifies the size of buffer that will be uploaded
 * @param data specifies a pointer to new data
 */
void GPU::setBufferData(BufferID buffer, uint64_t offset, uint64_t size, void const* data) {
  /// \todo Tato funkce nakopíruje data z cpu na "gpu".<br>
  /// Data by měla být nakopírována do bufferu vybraného parametrem "buffer".<br>
  /// Parametr size určuje, kolik dat (v bajtech) se překopíruje.<br>
  /// Parametr offset určuje místo v bufferu (posun v bajtech) kam se data nakopírují.<br>
  /// Parametr data obsahuje ukazatel na data na cpu pro kopírování.<br>

  if (!isBuffer(buffer))
      return;

  memcpy(bufferMap[buffer].data() + offset, data, size);
}

/**
 * @brief This function downloads data from GPU.
 *
 * @param buffer specfies buffer
 * @param offset specifies the offset into the buffer from which data will be returned, measured in bytes. 
 * @param size specifies data size that will be copied
 * @param data specifies a pointer to the location where buffer data is returned. 
 */
void GPU::getBufferData(BufferID buffer,
                        uint64_t offset,
                        uint64_t size,
                        void*    data)
{
  /// \todo Tato funkce vykopíruje data z "gpu" na cpu.
  /// Data by měla být vykopírována z bufferu vybraného parametrem "buffer".<br>
  /// Parametr size určuje kolik dat (v bajtech) se překopíruje.<br>
  /// Parametr offset určuje místo v bufferu (posun v bajtech) odkud se začne kopírovat.<br>
  /// Parametr data obsahuje ukazatel, kam se data nakopírují.<br>

  if (!isBuffer(buffer))
      return;

  auto p = bufferMap[buffer].data();
  memcpy(data, reinterpret_cast<const void *>(p + offset), size);
}

/**
 * @brief This function tests if buffer exists
 *
 * @param buffer selected buffer id
 *
 * @return true if buffer points to existing buffer on the GPU.
 */
bool GPU::isBuffer(BufferID buffer) { 
  /// \todo Tato funkce by měla vrátit true pokud buffer je identifikátor existující bufferu.<br>
  /// Tato funkce by měla vrátit false, pokud buffer není identifikátor existujícího bufferu. (nebo bufferu, který byl smazán).<br>
  /// Pro emptyId vrací false.<br>

  if (buffer == emptyID)
      return false;

  return bufferMap.find(buffer) != bufferMap.end();
}

/// @}

/**
 * \addtogroup vertexpuller_tasks 02. Implementace obslužných funkcí pro vertex puller
 * @{
 */

/**
 * @brief This function creates new vertex puller settings on the GPU,
 *
 * @return unique vertex puller identificator
 */
ObjectID GPU::createVertexPuller     (){
  /// \todo Tato funkce vytvoří novou práznou tabulku s nastavením pro vertex puller.<br>
  /// Funkce by měla vrátit identifikátor nové tabulky.
  /// Prázdná tabulka s nastavením neobsahuje indexování a všechny čtecí hlavy jsou vypnuté.
  vertexPullerMap.insert(make_pair(pullerCount, VertexPullerData{}));
  return pullerCount++;
}

/**
 * @brief This function deletes vertex puller settings
 *
 * @param vao vertex puller identificator
 */
void     GPU::deleteVertexPuller     (VertexPullerID vao){
  /// \todo Tato funkce by měla odstranit tabulku s nastavení pro vertex puller.<br>
  /// Parameter "vao" obsahuje identifikátor tabulky s nastavením.<br>
  /// Po uvolnění nastavení je identifiktátor volný a může být znovu použit.<br>

  if (!isVertexPuller(vao))
      return;

  vertexPullerMap.erase(vao);
}

/**
 * @brief This function sets one vertex puller reading head.
 *
 * @param vao identificator of vertex puller
 * @param head id of vertex puller head
 * @param type type of attribute
 * @param stride stride in bytes
 * @param offset offset in bytes
 * @param buffer id of buffer
 */
void     GPU::setVertexPullerHead    (VertexPullerID vao,uint32_t head,AttributeType type,uint64_t stride,uint64_t offset,BufferID buffer){
  /// \todo Tato funkce nastaví jednu čtecí hlavu vertex pulleru.<br>
  /// Parametr "vao" vybírá tabulku s nastavením.<br>
  /// Parametr "head" vybírá čtecí hlavu vybraného vertex pulleru.<br>
  /// Parametr "type" nastaví typ atributu, který čtecí hlava čte. Tímto se vybere kolik dat v bajtech se přečte.<br>
  /// Parametr "stride" nastaví krok čtecí hlavy.<br>
  /// Parametr "offset" nastaví počáteční pozici čtecí hlavy.<br>
  /// Parametr "buffer" vybere buffer, ze kterého bude čtecí hlava číst.<br>

  if (!isVertexPuller(vao))
      return;

  auto data = &vertexPullerMap[vao];

  data->head[head].attType = type;
  data->head[head].stride = stride;
  data->head[head].offset = offset;
  data->head[head].bufferId = buffer;
}

/**
 * @brief This function sets vertex puller indexing.
 *
 * @param vao vertex puller id
 * @param type type of index
 * @param buffer buffer with indices
 */
void     GPU::setVertexPullerIndexing(VertexPullerID vao,IndexType type,BufferID buffer){
  /// \todo Tato funkce nastaví indexování vertex pulleru.
  /// Parametr "vao" vybírá tabulku s nastavením.<br>
  /// Parametr "type" volí typ indexu, který je uložený v bufferu.<br>
  /// Parametr "buffer" volí buffer, ve kterém jsou uloženy indexy.<br>

  if (!isVertexPuller(vao))
      return;

  vertexPullerMap[vao].indexing.bufferId = buffer;
  vertexPullerMap[vao].indexing.indexType = type;
}

/**
 * @brief This function enables vertex puller's head.
 *
 * @param vao vertex puller 
 * @param head head id
 */
void     GPU::enableVertexPullerHead (VertexPullerID vao,uint32_t head){
  /// \todo Tato funkce povolí čtecí hlavu daného vertex pulleru.<br>
  /// Pokud je čtecí hlava povolena, hodnoty z bufferu se budou kopírovat do atributu vrcholů vertex shaderu.<br>
  /// Parametr "vao" volí tabulku s nastavením vertex pulleru (vybírá vertex puller).<br>
  /// Parametr "head" volí čtecí hlavu.<br>

  if (!isVertexPuller(vao))
      return;

  vertexPullerMap[vao].head[head].enabled = true;
}

/**
 * @brief This function disables vertex puller's head
 *
 * @param vao vertex puller id
 * @param head head id
 */
void     GPU::disableVertexPullerHead(VertexPullerID vao,uint32_t head){
  /// \todo Tato funkce zakáže čtecí hlavu daného vertex pulleru.<br>
  /// Pokud je čtecí hlava zakázána, hodnoty z bufferu se nebudou kopírovat do atributu vrcholu.<br>
  /// Parametry "vao" a "head" vybírají vertex puller a čtecí hlavu.<br>

  if (!isVertexPuller(vao))
      return;

  vertexPullerMap[vao].head[head].enabled = true;
}

/**
 * @brief This function selects active vertex puller.
 *
 * @param vao id of vertex puller
 */
void     GPU::bindVertexPuller       (VertexPullerID vao){
  /// \todo Tato funkce aktivuje nastavení vertex pulleru.<br>
  /// Pokud je daný vertex puller aktivován, atributy z bufferů jsou vybírány na základě jeho nastavení.<br>

  if (!isVertexPuller(vao))
      return;

  activePuller = vao;
}

/**
 * @brief This function deactivates vertex puller.
 */
void     GPU::unbindVertexPuller     (){
  /// \todo Tato funkce deaktivuje vertex puller.
  /// To většinou znamená, že se vybere neexistující "emptyID" vertex puller.

  activePuller = emptyID;
}

/**
 * @brief This function tests if vertex puller exists.
 *
 * @param vao vertex puller
 *
 * @return true, if vertex puller "vao" exists
 */
bool     GPU::isVertexPuller         (VertexPullerID vao){
  /// \todo Tato funkce otestuje, zda daný vertex puller existuje.
  /// Pokud ano, funkce vrací true.
  return vertexPullerMap.find(vao) != vertexPullerMap.end();
}

/// @}

/** \addtogroup program_tasks 03. Implementace obslužných funkcí pro shader programy
 * @{
 */

/**
 * @brief This function creates new shader program.
 *
 * @return shader program id
 */
ProgramID        GPU::createProgram         (){
  /// \todo Tato funkce by měla vytvořit nový shader program.<br>
  /// Funkce vrací unikátní identifikátor nového proramu.<br>
  /// Program je seznam nastavení, které obsahuje: ukazatel na vertex a fragment shader.<br>
  /// Dále obsahuje uniformní proměnné a typ výstupních vertex attributů z vertex shaderu, které jsou použity pro interpolaci do fragment atributů.<br>

  programMap.insert(make_pair(programIdCount, ProgramSettings{}));
  return programIdCount++;
}

/**
 * @brief This function deletes shader program
 *
 * @param prg shader program id
 */
void             GPU::deleteProgram         (ProgramID prg){
  /// \todo Tato funkce by měla smazat vybraný shader program.<br>
  /// Funkce smaže nastavení shader programu.<br>
  /// Identifikátor programu se stane volným a může být znovu využit.<br>

  if (!isProgram(prg))
      return;

  programMap.erase(prg);
}

/**
 * @brief This function attaches vertex and frament shader to shader program.
 *
 * @param prg shader program
 * @param vs vertex shader 
 * @param fs fragment shader
 */
void             GPU::attachShaders         (ProgramID prg,VertexShader vs,FragmentShader fs){
  /// \todo Tato funkce by měla připojít k vybranému shader programu vertex a fragment shader.

  if (!isProgram(prg))
      return;

  programMap[prg].fragmentShader = fs;
  programMap[prg].vertexShader = vs;
}

/**
 * @brief This function selects which vertex attributes should be interpolated during rasterization into fragment attributes.
 *
 * @param prg shader program
 * @param attrib id of attribute
 * @param type type of attribute
 */
void             GPU::setVS2FSType          (ProgramID prg,uint32_t attrib,AttributeType type){
  /// \todo tato funkce by měla zvolit typ vertex atributu, který je posílán z vertex shaderu do fragment shaderu.<br>
  /// V průběhu rasterizace vznikají fragment.<br>
  /// Fragment obsahují fragment atributy.<br>
  /// Tyto atributy obsahují interpolované hodnoty vertex atributů.<br>
  /// Tato funkce vybere jakého typu jsou tyto interpolované atributy.<br>
  /// Bez jakéhokoliv nastavení jsou atributy prázdne AttributeType::EMPTY<br>

  //TODO zle
  if (!isProgram(prg))
      return;

  programMap[prg].attributeType[attrib] = type;
}

/**
 * @brief This function actives selected shader program
 *
 * @param prg shader program id
 */
void             GPU::useProgram            (ProgramID prg){
  /// \todo tato funkce by měla vybrat aktivní shader program.
  activeProgram = prg;
}

/**
 * @brief This function tests if selected shader program exists.
 *
 * @param prg shader program
 *
 * @return true, if shader program "prg" exists.
 */
bool             GPU::isProgram             (ProgramID prg){
  /// \todo tato funkce by měla zjistit, zda daný program existuje.<br>
  /// Funkce vráti true, pokud program existuje.<br>
  return programMap.find(prg) != programMap.end();
}

/**
 * @brief This function sets uniform value (1 float).
 *
 * @param prg shader program
 * @param uniformId id of uniform value (number of uniform values is stored in maxUniforms variable)
 * @param d value of uniform variable
 */
void             GPU::programUniform1f      (ProgramID prg,uint32_t uniformId,float     const&d){
  /// \todo tato funkce by měla nastavit uniformní proměnnou shader programu.<br>
  /// Parametr "prg" vybírá shader program.<br>
  /// Parametr "uniformId" vybírá uniformní proměnnou. Maximální počet uniformních proměnných je uložen v programné \link maxUniforms \endlink.<br>
  /// Parametr "d" obsahuje data (1 float).<br>

  if (!isProgram(prg))
      return;

  programMap[prg].uniforms.uniform[uniformId].v1 = d;
}

/**
 * @brief This function sets uniform value (2 float).
 *
 * @param prg shader program
 * @param uniformId id of uniform value (number of uniform values is stored in maxUniforms variable)
 * @param d value of uniform variable
 */
void             GPU::programUniform2f      (ProgramID prg,uint32_t uniformId,glm::vec2 const&d){
  /// \todo tato funkce dělá obdobnou věc jako funkce programUniform1f.<br>
  /// Místo 1 floatu nahrává 2 floaty.

  if (!isProgram(prg))
      return;

  programMap[prg].uniforms.uniform[uniformId].v2 = d;
}

/**
 * @brief This function sets uniform value (3 float).
 *
 * @param prg shader program
 * @param uniformId id of uniform value (number of uniform values is stored in maxUniforms variable)
 * @param d value of uniform variable
 */
void             GPU::programUniform3f      (ProgramID prg,uint32_t uniformId,glm::vec3 const&d){
  /// \todo tato funkce dělá obdobnou věc jako funkce programUniform1f.<br>
  /// Místo 1 floatu nahrává 3 floaty.

  if (!isProgram(prg))
      return;

  programMap[prg].uniforms.uniform[uniformId].v3 = d;
}

/**
 * @brief This function sets uniform value (4 float).
 *
 * @param prg shader program
 * @param uniformId id of uniform value (number of uniform values is stored in maxUniforms variable)
 * @param d value of uniform variable
 */
void             GPU::programUniform4f      (ProgramID prg,uint32_t uniformId,glm::vec4 const&d){
  /// \todo tato funkce dělá obdobnou věc jako funkce programUniform1f.<br>
  /// Místo 1 floatu nahrává 4 floaty.

  if (!isProgram(prg))
      return;

  programMap[prg].uniforms.uniform[uniformId].v4 = d;
}

/**
 * @brief This function sets uniform value (4 float).
 *
 * @param prg shader program
 * @param uniformId id of uniform value (number of uniform values is stored in maxUniforms variable)
 * @param d value of uniform variable
 */
void             GPU::programUniformMatrix4f(ProgramID prg,uint32_t uniformId,glm::mat4 const&d){
  /// \todo tato funkce dělá obdobnou věc jako funkce programUniform1f.<br>
  /// Místo 1 floatu nahrává matici 4x4 (16 floatů).

  if (!isProgram(prg))
      return;

  programMap[prg].uniforms.uniform[uniformId].m4 = d;
}

/// @}





/** \addtogroup framebuffer_tasks 04. Implementace obslužných funkcí pro framebuffer
 * @{
 */

/**
 * @brief This function creates framebuffer on GPU.
 *
 * @param width width of framebuffer
 * @param height height of framebuffer
 */
void GPU::createFramebuffer      (uint32_t width,uint32_t height){
  /// \todo Tato funkce by měla alokovat framebuffer od daném rozlišení.<br>
  /// Framebuffer se skládá z barevného a hloukového bufferu.<br>
  /// Buffery obsahují width x height pixelů.<br>
  /// Barevný pixel je složen z 4 x uint8_t hodnot - to reprezentuje RGBA barvu.<br>
  /// Hloubkový pixel obsahuje 1 x float - to reprezentuje hloubku.<br>
  /// Nultý pixel framebufferu je vlevo dole.<br>

  ColorBuffer.resize(width * height);
  DepthBuffer.resize(width * height);
  frHeight = height;
  frWidth = width;
  clear(0, 0, 0, 0);
}

/**
 * @brief This function deletes framebuffer.
 */
void GPU::deleteFramebuffer      (){
  /// \todo tato funkce by měla dealokovat framebuffer.

  ColorBuffer.clear();
  DepthBuffer.clear();
  frWidth = frHeight = 0;
}

/**
 * @brief This function resizes framebuffer.
 *
 * @param width new width of framebuffer
 * @param height new heght of framebuffer
 */
void     GPU::resizeFramebuffer(uint32_t width,uint32_t height){
  /// \todo Tato funkce by měla změnit velikost framebuffer.

  ColorBuffer.resize(width * height);
  DepthBuffer.resize(width * height);
  frHeight = height;
  frWidth = width;
}

/**
 * @brief This function returns pointer to color buffer.
 *
 * @return pointer to color buffer
 */
uint8_t* GPU::getFramebufferColor  (){
  /// \todo Tato funkce by měla vrátit ukazatel na začátek barevného bufferu.<br>
  return &ColorBuffer.data()->r;
}

/**
 * @brief This function returns pointer to depth buffer.
 *
 * @return pointer to dept buffer.
 */
float* GPU::getFramebufferDepth    (){
  /// \todo tato funkce by mla vrátit ukazatel na začátek hloubkového bufferu.<br>
  return DepthBuffer.data();
}

/**
 * @brief This function returns width of framebuffer
 *
 * @return width of framebuffer
 */
uint32_t GPU::getFramebufferWidth (){
  /// \todo Tato funkce by měla vrátit šířku framebufferu.
  return frWidth;
}

/**
 * @brief This function returns height of framebuffer.
 *
 * @return height of framebuffer
 */
uint32_t GPU::getFramebufferHeight(){
  /// \todo Tato funkce by měla vrátit výšku framebufferu.
  return frHeight;
}

/// @}

/** \addtogroup draw_tasks 05. Implementace vykreslovacích funkcí
 * Bližší informace jsou uvedeny na hlavní stránce dokumentace.
 * @{
 */

/**
 * @brief This functino clears framebuffer.
 *
 * @param r red channel
 * @param g green channel
 * @param b blue channel
 * @param a alpha channel
 */
void            GPU::clear                 (float r,float g,float b,float a){
  /// \todo Tato funkce by měla vyčistit framebuffer.<br>
  /// Barevný buffer vyčistí na barvu podle parametrů r g b a (0 - nulová intenzita, 1 a větší - maximální intenzita).<br>
  /// (0,0,0) - černá barva, (1,1,1) - bílá barva.<br>
  /// Hloubkový buffer nastaví na takovou hodnotu, která umožní rasterizaci trojúhelníka, který leží v rámci pohledového tělesa.<br>
  /// Hloubka by měla být tedy větší než maximální hloubka v NDC (normalized device coordinates).<br>

  for (auto &col : ColorBuffer)
  {
      col.r = (r >= 1.0 ? 255 : (r <= 0.0 ? 0 : static_cast<uint8_t>(floor(r * 256.0))));
      col.g = (g >= 1.0 ? 255 : (g <= 0.0 ? 0 : static_cast<uint8_t>(floor(g * 256.0))));
      col.b = (b >= 1.0 ? 255 : (b <= 0.0 ? 0 : static_cast<uint8_t>(floor(b * 256.0))));
      col.a = (a >= 1.0 ? 255 : (a <= 0.0 ? 0 : static_cast<uint8_t>(floor(a * 256.0))));
  }
  for (auto &depth : DepthBuffer)
      depth = 1.f;
}

void            GPU::drawTriangles         (uint32_t  nofVertices){
  /// \todo Tato funkce vykreslí trojúhelníky podle daného nastavení.<br>
  /// Vrcholy se budou vybírat podle nastavení z aktivního vertex pulleru (pomocí bindVertexPuller).<br>
  /// Vertex shader a fragment shader se zvolí podle aktivního shader programu (pomocí useProgram).<br>
  /// Parametr "nofVertices" obsahuje počet vrcholů, který by se měl vykreslit (3 pro jeden trojúhelník).<br>
  vertPullInvCount = 0;

  for (unsigned int i = 0; i < nofVertices; ++i)
  {
      OutVertex outVertex;
      programMap[activeProgram].vertexShader(outVertex, vertexPuller(), programMap[activeProgram].uniforms);
      outVertexBuffer.push_back(outVertex);

      if (outVertexBuffer.size() % 3 == 0)
      {
          PrimitiveTriangle triangle = primitiveAssembly();
          rasterize(triangle);
      }
  }
}

InVertex GPU::vertexPuller()
{
    InVertex inVertex;

    auto pullerData = &vertexPullerMap[activePuller];

    uint8_t dataSize;

    inVertex.gl_VertexID = vertPullInvCount;
    if (pullerData->indexing.bufferId != emptyID)
    {
        switch (pullerData->indexing.indexType)
        {
            case IndexType::UINT8:
                dataSize = sizeof(uint8_t);
                break;
            case IndexType::UINT16:
                dataSize = sizeof(uint16_t);
                break;
            case IndexType::UINT32:
                dataSize = sizeof(uint32_t);
                break;
        }
        getBufferData(pullerData->indexing.bufferId, vertPullInvCount * dataSize, dataSize, &inVertex.gl_VertexID);
    }

    for (int i = 0; i < maxAttributes; ++i)
    {
        switch (pullerData->head[i].attType)
        {
            case AttributeType::EMPTY:
                dataSize = 0;
                break;
            case AttributeType::FLOAT:
                dataSize = sizeof(float);
                break;
            case AttributeType::VEC2:
                dataSize = sizeof(float) * 2;
                break;
            case AttributeType::VEC3:
                dataSize = sizeof(float) * 3;
                break;
            case AttributeType::VEC4:
                dataSize = sizeof(float) * 4;
                break;
        }

        getBufferData(pullerData->head[i].bufferId, pullerData->head[i].offset + pullerData->head[i].stride * inVertex.gl_VertexID,
                dataSize, &inVertex.attributes[i]);
    }

    ++vertPullInvCount;
    return inVertex;
}

GPU::PrimitiveTriangle GPU::primitiveAssembly()
{
    PrimitiveTriangle triangle;

    OutVertex vertex;
    vertex = perspectiveDivision(outVertexBuffer.back());
    triangle.c = viewPortTransformation(vertex);
    outVertexBuffer.pop_back();

    vertex = perspectiveDivision(outVertexBuffer.back());
    triangle.b = viewPortTransformation(vertex);
    outVertexBuffer.pop_back();

    vertex = perspectiveDivision(outVertexBuffer.back());
    triangle.a = viewPortTransformation(vertex);
    outVertexBuffer.pop_back();

    return triangle;
}

OutVertex GPU::perspectiveDivision(OutVertex &vertex)
{
    vertex.gl_Position.x /= vertex.gl_Position.w;
    vertex.gl_Position.y /= vertex.gl_Position.w;
    vertex.gl_Position.z /= vertex.gl_Position.w;
    return vertex;
}

OutVertex GPU::viewPortTransformation(OutVertex &vertex)
{
    vertex.gl_Position.x += 1.f;
    vertex.gl_Position.y += 1.f;

    vertex.gl_Position.x /= 2.f;
    vertex.gl_Position.y /= 2.f;

    vertex.gl_Position.x *= static_cast<float>(getFramebufferWidth());
    vertex.gl_Position.y *= static_cast<float>(getFramebufferHeight());
    return vertex;
}

void GPU::rasterize(PrimitiveTriangle &triangle)
{
//    if (((triangle.b.gl_Position.x - triangle.a.gl_Position.x) * (triangle.c.gl_Position.y - triangle.a.gl_Position.y) - ((triangle.b.gl_Position.y - triangle.a.gl_Position.y) * (triangle.c.gl_Position.x - triangle.a.gl_Position.x))) > 0)
//    {
//        swap(triangle.b, triangle.c);
//    }

    float deltaAB_x = triangle.b.gl_Position.x - triangle.a.gl_Position.x;
    float deltaAB_y = triangle.b.gl_Position.y - triangle.a.gl_Position.y;

    float deltaBC_x = triangle.c.gl_Position.x - triangle.b.gl_Position.x;
    float deltaBC_y = triangle.c.gl_Position.y - triangle.b.gl_Position.y;

    float deltaCA_x = triangle.a.gl_Position.x - triangle.c.gl_Position.x;
    float deltaCA_y = triangle.a.gl_Position.y - triangle.c.gl_Position.y;

//    float EAB = (0 + 0.5f - triangle.a.gl_Position.x) * deltaAB_y - (0 + 0.5f - triangle.a.gl_Position.y) * deltaAB_x;
//    float EBC = (0 + 0.5f - triangle.b.gl_Position.x) * deltaBC_y - (0 + 0.5f - triangle.b.gl_Position.y) * deltaBC_x;
//    float ECA = (0 + 0.5f - triangle.c.gl_Position.x) * deltaCA_y - (0 + 0.5f - triangle.c.gl_Position.y) * deltaCA_x;

    for (unsigned int y = 0; y < getFramebufferHeight(); ++y)
    {
        for (unsigned int x = 0; x < getFramebufferWidth(); ++x)
        {
            float EAB{(static_cast<float>(x) + 0.5f - triangle.a.gl_Position.x) * deltaAB_y - (static_cast<float>(y) + 0.5f - triangle.a.gl_Position.y) * deltaAB_x};
            float EBC{(static_cast<float>(x) + 0.5f - triangle.b.gl_Position.x) * deltaBC_y - (static_cast<float>(y) + 0.5f - triangle.b.gl_Position.y) * deltaBC_x};
            float ECA{(static_cast<float>(x) + 0.5f - triangle.c.gl_Position.x) * deltaCA_y - (static_cast<float>(y) + 0.5f - triangle.c.gl_Position.y) * deltaCA_x};

            if ((EAB >= 0 && EBC >= 0 && ECA >= 0) || (EAB <= 0 && EBC <= 0 && ECA <= 0))
            {
                InFragment fragment;
                fragment.gl_FragCoord.x = static_cast<float>(x) + 0.5f;
                fragment.gl_FragCoord.y = static_cast<float>(y) + 0.5f;

                glm::vec2 p {static_cast<float>(x) + 0.5f, static_cast<float>(y) + 0.5f};
                interpolate(fragment, p, triangle.a, triangle.b, triangle.c);

                OutFragment outFragment{};
                programMap[activeProgram].fragmentShader(outFragment, fragment, programMap[activeProgram].uniforms);

                if (fragment.gl_FragCoord.z < DepthBuffer[x * getFramebufferWidth() + y])
                {
                    ColorBuffer[x * getFramebufferWidth() + y].r = (outFragment.gl_FragColor.r >= 1.0 ? 255 : (outFragment.gl_FragColor.r <= 0.0 ? 0 : static_cast<uint8_t>(floor(outFragment.gl_FragColor.r * 256.0))));
                    ColorBuffer[x * getFramebufferWidth() + y].g = (outFragment.gl_FragColor.g >= 1.0 ? 255 : (outFragment.gl_FragColor.g <= 0.0 ? 0 : static_cast<uint8_t>(floor(outFragment.gl_FragColor.g * 256.0))));
                    ColorBuffer[x * getFramebufferWidth() + y].b = (outFragment.gl_FragColor.b >= 1.0 ? 255 : (outFragment.gl_FragColor.b <= 0.0 ? 0 : static_cast<uint8_t>(floor(outFragment.gl_FragColor.b * 256.0))));
                    ColorBuffer[x * getFramebufferWidth() + y].a = (outFragment.gl_FragColor.a >= 1.0 ? 255 : (outFragment.gl_FragColor.a <= 0.0 ? 0 : static_cast<uint8_t>(floor(outFragment.gl_FragColor.a * 256.0))));

                    DepthBuffer[x * getFramebufferWidth() + y] = fragment.gl_FragCoord.z;
                }
            }
//            EAB += deltaAB_x;
//            EBC += deltaBC_x;
//            ECA += deltaCA_x;
        }
//        EAB -= deltaAB_y;
//        EBC -= deltaBC_y;
//        ECA -= deltaCA_y;
    }

}

void GPU::interpolate(InFragment &fragment, glm::vec2 &p, OutVertex &a, OutVertex &b, OutVertex &c)
{
    //source https://gamedev.stackexchange.com/questions/23743/whats-the-most-efficient-way-to-find-barycentric-coordinates

    glm::vec2 v0 = {b.gl_Position - a.gl_Position},
            v1 = {c.gl_Position - a.gl_Position},
            v2 = {p.x - a.gl_Position.x, p.y - a.gl_Position.y};

//    glm::vec2 v0 = {b.gl_Position - a.gl_Position},
//    v1 = {c.gl_Position - a.gl_Position},
//    v2 = {p.x - a.gl_Position.x, p.y - a.gl_Position.y};
    float d00 = glm::dot(v0, v0);
    float d01 = glm::dot(v0, v1);
    float d11 = glm::dot(v1, v1);
    float d20 = glm::dot(v2, v0);
    float d21 = glm::dot(v2, v1);
    float denom = d00 * d11 - d01 * d01;

//    float l0 = ((d11 * d20 - d01 * d21) / denom);
//    float l1 = ((d00 * d21 - d01 * d20) / denom);
//    float l2 = 1.0f - l0 - l1;

    float l1 = (d11 * d20 - d01 * d21) / denom;
    float l2 = (d00 * d21 - d01 * d20) / denom;
    float l0 = 1.0f - l1 - l2;

    float h0 = a.gl_Position.w;
    float h1 = b.gl_Position.w;
    float h2 = c.gl_Position.w;

    Attribute A0, A1, A2;
    for (unsigned int i = 0; i < maxAttributes; ++i)
    {
        switch (programMap[activeProgram].attributeType[i])
        {
            case AttributeType::EMPTY:
                break;
            case AttributeType::FLOAT:
                A0.v1 = a.attributes[i].v1;
                A1.v1 = b.attributes[i].v1;
                A2.v1 = c.attributes[i].v1;

                fragment.attributes[i].v1 = (((A0.v1 * l0 / h0) + (A1.v1 * l1 / h1) + (A2.v1 * l2 / h2)) / (l0 / h0 + l1 / h1 + l2 / h2));
                break;
            case AttributeType::VEC2:
                A0.v2 = a.attributes[i].v2;
                A1.v2 = b.attributes[i].v2;
                A2.v2 = c.attributes[i].v2;

                fragment.attributes[i].v2 = (((A0.v2 * l0 / h0) + (A1.v2 * l1 / h1) + (A2.v2 * l2 / h2)) / (l0 / h0 + l1 / h1 + l2 / h2));
                break;
            case AttributeType::VEC3:
                A0.v3 = a.attributes[i].v3;
                A1.v3 = b.attributes[i].v3;
                A2.v3 = c.attributes[i].v3;

                fragment.attributes[i].v3 = (((A0.v3 * l0 / h0) + (A1.v3 * l1 / h1) + (A2.v3 * l2 / h2)) / (l0 / h0 + l1 / h1 + l2 / h2));
                break;
            case AttributeType::VEC4:
                A0.v4 = a.attributes[i].v4;
                A1.v4 = b.attributes[i].v4;
                A2.v4 = c.attributes[i].v4;

                fragment.attributes[i].v4 = (((A0.v4 * l0 / h0) + (A1.v4 * l1 / h1) + (A2.v4 * l2 / h2)) / (l0 / h0 + l1 / h1 + l2 / h2));
                break;
        }
    }
    fragment.gl_FragCoord.z = (((a.gl_Position.z * l0 / h0) + (b.gl_Position.z * l1 / h1) + (c.gl_Position.z * l2 / h2)) / (l0 / h0 + l1 / h1 + l2 / h2));
}

/// @}
