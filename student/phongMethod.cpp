/*!
 * @file
 * @brief This file contains implementation of phong rendering method
 *
 * @author Tomáš Milet, imilet@fit.vutbr.cz
 */

#include <student/phongMethod.hpp>
#include <student/bunny.hpp>

/** \addtogroup shader_side 06. Implementace vertex/fragment shaderu phongovy metody
 * Vašim úkolem ve vertex a fragment shaderu je transformovat trojúhelníky pomocí view a projekční matice a spočítat phongův osvětlovací model.
 * Vašim úkolem také je správně vypočítat procedurální barvu.
 * Následující obrázek zobrazuje shadery v různém stupni implementace.
 * Horní řáděk zobrazuje procedurální texturu.
 * Prostřední řádek zobrazuje model králička s nanesenou texturou, ale bez použití transformačních matic.
 * Dolní řádek zobrazuje model po použítí transformačních matic.
 *
 * \image html images/texture.svg "Vypočet procedurální textury." width=1000
 *
 * 
 *
 *
 *
 * @{
 */


/**
 * @brief This function represents vertex shader of phong method.
 *
 * @param outVertex output vertex
 * @param inVertex input vertex
 * @param uniforms uniform variables
 */
void phong_VS(OutVertex&outVertex,InVertex const&inVertex,Uniforms const&uniforms){
  /// \todo Naimplementujte vertex shader, který transformuje vstupní vrcholy do
  /// clip-space.<br>
  /// <b>Vstupy:</b><br>
  /// Vstupní vrchol by měl v nultém atributu obsahovat pozici vrcholu ve
  /// world-space (vec3) a v prvním
  /// atributu obsahovat normálu vrcholu ve world-space (vec3).<br>
  /// <b>Výstupy:</b><br>
  /// Výstupní vrchol by měl v nultém atributu obsahovat pozici vrcholu (vec3)
  /// ve world-space a v prvním
  /// atributu obsahovat normálu vrcholu ve world-space (vec3).
  /// Výstupní vrchol obsahuje pozici a normálu vrcholu proto, že chceme počítat
  /// osvětlení ve world-space ve fragment shaderu.<br>
  /// <b>Uniformy:</b><br>
  /// Vertex shader by měl pro transformaci využít uniformní proměnné obsahující
  /// view a projekční matici.
  /// View matici čtěte z nulté uniformní proměnné a projekční matici
  /// čtěte z první uniformní proměnné.
  /// <br>
  /// Využijte vektorové a maticové funkce.
  /// Nepředávajte si data do shaderu pomocí globálních proměnných.
  /// Vrchol v clip-space by měl být zapsán do proměnné gl_Position ve výstupní
  /// struktuře.
  /// \image html images/vertex_shader_tasks.svg "Vizualizace vstupů a výstupů vertex shaderu" width=1000

    auto mat = uniforms.uniform[1].m4 * uniforms.uniform[0].m4;
    glm::vec4 vc4 {inVertex.attributes[0].v3.x, inVertex.attributes[0].v3.y, inVertex.attributes[0].v3.z, 1};
    outVertex.gl_Position = mat * vc4;

    outVertex.attributes[0] = inVertex.attributes[0];
    outVertex.attributes[1] = inVertex.attributes[1];
}

/**
 * @brief This function represents fragment shader of phong method.
 *
 * @param outFragment output fragment
 * @param inFragment input fragment
 * @param uniforms uniform variables
 */
void phong_FS(OutFragment&outFragment,InFragment const&inFragment,Uniforms const&uniforms){
  /// \todo Naimplementujte fragment shader, který počítá phongův osvětlovací
  /// model s phongovým stínováním.<br>
  /// <b>Vstup:</b><br>
  /// Vstupní fragment by měl v nultém fragment atributu obsahovat
  /// interpolovanou pozici ve world-space a v prvním
  /// fragment atributu obsahovat interpolovanou normálu ve world-space.<br>
  /// <b>Výstup:</b><br>
  /// Barvu zapište do proměnné gl_FragColor ve výstupní struktuře.<br>
  /// <b>Uniformy:</b><br>
  /// Pozici kamery přečtěte z uniformní 3 a pozici
  /// světla přečtěte z uniformní 2.
  /// <br>
  /// <br>
  /// Dejte si pozor na velikost normálového vektoru, při lineární interpolaci v
  /// rasterizaci může dojít ke zkrácení.
  /// Zapište barvu do proměnné gl_FragColor ve výstupní struktuře.
  /// Shininess faktor nastavte na 40.f
  /// <br>
  /// <br>
  /// Difuzní barva materiálu (textura) by měla být procedurálně generována.
  /// Textura je složena zde dvou částí: sinusové pruhy a bílý sněhový poprašek.
  /// Textura je zkombinována z těchto dvou částí podle sklonu normály.
  /// V případě, že normála směřuje kolmo vzhůru je textura čistě bílá.
  /// V případě, že normála směřuje vodorovně nebo dolů je textura složena ze sinusových pruhů.
  /// Bílá textura a textura sinusových pruhů je lineráně míchana pomocí interpolačního parameteru t.
  /// Interpolační parameter t spočtěte z y komponenty normály pomocí t = y*y (samozřejmě s ohledem na negativní čísla).
  /// 
  /// \image html images/texture_combination.svg "Vizualizace kombinace textury sinusových pruhů a sněhového poprašku." width=600
  /// <br>
  /// <br>
  /// Sinusové pruhy jsou naneseny v rovině xy pomocí planárního nanášení textur, viz následující obrázek:
  /// \image html images/texture_plane.svg "Vizualizace nanášení textury sinusových pruhů." width=600
  ///
  /// Sinusová textura obsahuje 10 pruhů 5 zelených (0,0.5,0) a pět žlutých (1,1,0).
  /// Pruhy mají stejnou tloušku.
  /// Textura má velikost 1x1 a opakuje se (platí i pro záporné souřadnice).
  /// Pruhy jsou zvlněny pomocí funkce sinus.
  /// Amplituda funkce sinus je 0.1.
  /// Perioda je nastavena na \f$ \frac{2 \cdot \pi}{10} \f$.
  /// Výchylku pruhu lze tedy spočítat jako \f$ \frac{sin(y \cdot 10)}{10} \f$
  /// Tvorba textury je znázorněna na následujícím obrázku:
  ///
  /// \image html images/stripes_texture.svg "Vizualizace tvorby sinusové textury" width=600
  /// 
  /// 
  /// Spekulární barvu materiálu nastavte na čistou bílou.
  /// Barvu světla nastavte na bílou.
  /// Nepoužívejte ambientní světlo.<br>
  ///
  /// \image html images/fragment_shader_tasks.svg "Vizualizace výpočtu ve fragment shaderu" width=1000

  auto light = uniforms.uniform[2].v3 - inFragment.attributes[0].v3;
  light = glm::normalize(light);

  auto camera = uniforms.uniform[3].v3; - inFragment.attributes[0].v3;
  camera = glm::normalize(camera);

  auto normal = inFragment.attributes[1].v3;
  normal = glm::normalize(normal);

  float difuse = glm::dot(normal, light);

  auto reflection = 2 * glm::dot(light, normal) * normal - light;

  float sp = glm::dot(camera, reflection);
  if (sp < 0)
      sp = 0;

  float specular = pow(sp, 40.f);
  float textX = inFragment.attributes[0].v3.x;

  float offset = sin(inFragment.attributes[0].v3.y * 10.f) / 10.f;
  textX += offset;
  if (textX < 0)
      textX += 1.f;
  if (textX > 1.f)
      textX -= 1.f;

  textX *= 10.f;
  int textureX = static_cast<int>(textX);

  float textR{0.f}, textG{0.f}, textB{0.f};
  if (textureX % 2 == 0)
  {
      textR = 0;
      textG = 0.5f;
      textB = 0;
  }
  else
  {
      textR = 1.f;
      textG = 1.f;
      textB = 0.f;
  }

  float t{0};
  if (normal.y >= 0)
      t = pow(normal.y, 2);

  outFragment.gl_FragColor.r = t + (textR * (1.f - t));
  outFragment.gl_FragColor.g = t + (textG * (1.f - t));
  outFragment.gl_FragColor.b = t + (textB * (1.f - t));

  outFragment.gl_FragColor.a = 1.f;

  if (difuse < 1.f && difuse > 0.f)
      outFragment.gl_FragColor = outFragment.gl_FragColor * difuse;
  else if (difuse <= 0.f)
  {
      outFragment.gl_FragColor.r = 0;
      outFragment.gl_FragColor.g = 0;
      outFragment.gl_FragColor.b = 0;
  }

  outFragment.gl_FragColor.r += specular;
  outFragment.gl_FragColor.g += specular;
  outFragment.gl_FragColor.b += specular;

  if (outFragment.gl_FragColor.r > 1.f)
      outFragment.gl_FragColor.r = 1.f;
  if (outFragment.gl_FragColor.g > 1.f)
      outFragment.gl_FragColor.g = 1.f;
  if (outFragment.gl_FragColor.b > 1.f)
      outFragment.gl_FragColor.b = 1.f;
}

/// @}

/** \addtogroup cpu_side 07. Implementace vykreslení králička s phongovým osvětlovacím modelem.
 * Vaším úkolem je využít naimplementovanou grafickou kartu a vykreslit králička s phongovým osvětlovacím modelem a stínováním a procedurální texturou.
 * @{
 */


/**
 * @brief Constructoro f phong method
 */
PhongMethod::PhongMethod(){
/// \todo Doprogramujte inicializační funkci.
/// Zde byste měli vytvořit buffery na GPU, nahrát data do bufferů, vytvořit
/// vertex puller a správně jej nakonfigurovat, vytvořit program, připojit k
/// němu shadery a nastavit atributy, které se posílají mezi vs a fs.
/// Do bufferů nahrajte vrcholy králička (pozice, normály) a indexy na vrcholy
/// ze souboru \ref bunny.hpp.
/// Shader program by měl odkazovat na funkce/shadery phong_VS a phong_FS.
/// V konfiguraci vertex pulleru nastavte dvě čtecí hlavy.
/// Jednu pro pozice vrcholů a druhou pro normály vrcholů.
/// Nultý vertex/fragment atribut by měl obsahovat pozici vertexu.
/// První vertex/fragment atribut by měl obsahovat normálu vertexu.
/// Nastavte, které atributy (jaký typ) se posílají z vertex shaderu do fragment shaderu.
/// <b>Seznam funkcí, které jistě využijete:</b>
///  - gpu.createBuffer()
///  - gpu.setBufferData()
///  - gpu.createVertexPuller()
///  - gpu.setVertexPullerIndexing()
///  - gpu.setVertexPullerHead()
///  - gpu.enableVertexPullerHead()
///  - gpu.createProgram()
///  - gpu.attachShaders()
///  - gpu.setVS2FSType()

    buffVert = gpu.createBuffer(sizeof(bunnyVertices));
    gpu.clear(0,0,0,0);
    gpu.setBufferData(buffVert, 0, sizeof(bunnyVertices), bunnyVertices);
    buffInd = gpu.createBuffer(sizeof(bunnyIndices));
    gpu.setBufferData(buffInd, 0, sizeof(bunnyIndices), bunnyIndices);
    vao = gpu.createVertexPuller();
    gpu.setVertexPullerIndexing(vao, IndexType::UINT32, buffInd);
    gpu.setVertexPullerHead(vao, 0, AttributeType::VEC3, sizeof(BunnyVertex), 0, buffVert);
    gpu.enableVertexPullerHead(vao, 0);
    gpu.setVertexPullerHead(vao, 1, AttributeType::VEC3, sizeof(BunnyVertex), 12, buffVert);
    gpu.enableVertexPullerHead(vao, 1);
    prg = gpu.createProgram();
    gpu.attachShaders(prg, phong_VS, phong_FS);
    gpu.setVS2FSType(prg, 0, AttributeType::VEC3);
    gpu.setVS2FSType(prg, 1, AttributeType::VEC3);
}


/**
 * @brief This function draws phong method.
 *
 * @param proj projection matrix
 * @param view view matrix
 * @param light light position
 * @param camera camera position
 */
void PhongMethod::onDraw(glm::mat4 const&proj,glm::mat4 const&view,glm::vec3 const&light,glm::vec3 const&camera){

/// \todo Doprogramujte kreslící funkci.
/// Zde byste měli aktivovat shader program, aktivovat vertex puller, nahrát
/// data do uniformních proměnných a
/// vykreslit trojúhelníky pomocí funkce \ref GPU::drawTriangles.
/// Data pro uniformní proměnné naleznete v parametrech této funkce.
/// <b>Seznam funkcí, které jistě využijete:</b>
///  - gpu.bindVertexPuller()
///  - gpu.useProgram()
///  - gpu.programUniformMatrix4f()
///  - gpu.programUniform3f      ()
///  - gpu.drawTriangles()
///  - gpu.unbindVertexPuller()

  gpu.clear(.5f,.5f,.5f,1.f);
  gpu.bindVertexPuller(vao);
  gpu.useProgram(prg);
  gpu.programUniformMatrix4f(prg, 0, view);
  gpu.programUniformMatrix4f(prg, 1, proj);
  gpu.programUniform3f(prg, 2, light);
  gpu.programUniform3f(prg, 3, camera);
  gpu.drawTriangles(6276);

}

/**
 * @brief Destructor of phong method.
 */
PhongMethod::~PhongMethod(){
  ///\todo Zde uvolněte alokované zdroje
  /// <b>Seznam funkcí</b>
  ///  - gpu.deleteProgram()
  ///  - gpu.deleteVertexPuller()
  ///  - gpu.deleteBuffer()

  gpu.deleteProgram(prg);
  gpu.deleteBuffer(buffVert);
  gpu.deleteBuffer(buffInd);
  gpu.deleteVertexPuller(vao);
  gpu.deleteFramebuffer();
  prg = buffInd = buffVert = vao = emptyID;
}

/// @}
