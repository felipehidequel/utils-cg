/**
 * \file luz.cpp
 *
 * \brief Este arquivo contem os calculos que foram definidos no arquivo Luz.hpp, sendo este responsavel pela definicoes de funcoes de luz de todos
 * os objetos da cena que serao renderizados. Foi utilizado o texto [introducao ao ray tracing] como base para a implementacao do arquivo.
 * [Texto de introducao ao ray tracing]: www.visgraf.impa.br/Data/RefBib/PS_PDF/silva94/rt.pdf "introducao ao ray tracing"
 *
 * \author 
 * Petrucio Ricardo Tavares de Medeiros \n
 * Universidade Federal do Rio Grande do Norte \n
 * Departamento de Computacao e Automacao Industrial \n
 * petrucior at gmail (dot) com
 *
 * \version 0.1
 * \date Outubro 2013
 */
 
#include "luz.hpp"		//rayTracing::Luz
#include <math.h>		//pow (expoente)

/** 
 * \defgroup RayTracingNameSpace Namespace rayTracing.
 * @{
 */
 
/**
 * \namespace rayTracing
 *
 * \brief O espaco de nomes rayTracing contem todas os arquivos que sao utilizados para sua implementacao.
 */
namespace rayTracing{
  //------------------------------
  //	Metodos publicos
  //------------------------------
  /**
   * \fn Luz::Luz();
   *
   * \brief Construtor da classe.
   */
  Luz::Luz(){
    //Instanciando a posicao da luz
    pos_luz = new Vetor();
    //Instanciando vetores n, l, o e r
    n = new Vetor();
    l = new Vetor();
    o = new Vetor();
    r = new Vetor();
  }
	
  /**
   * \fn void Luz::posicao_luz(double pos_luz_x, double pos_luz_y, double pos_luz_z);
   *
   * \brief Atualiza os valores dos vetores de posicao da luz.
   *
   * \param pos_luz_x - posicao x da luz
   * \param pos_luz_y - posicao y da luz
   * \param pos_luz_z - posicao z da luz
   */
  void 
  Luz::posicao_luz(double pos_luz_x, double pos_luz_y, double pos_luz_z){
    pos_luz->valores_vetor(pos_luz_x, pos_luz_y, pos_luz_z);
  }
	
  /**
   * \fn void Luz::atualizar_vetores_auxiliares(Vetor* interseccao_esfera, Vetor* centro_esfera, Vetor* lookfrom);
   *
   * \brief Pega todos os vetores necessarios para quantificacao da luz em um determinado ponto.
   *
   * \param interseccao_esfera - ponteiro para o vetor de interseccao da esfera
   * \param centro_esfera - ponteiro para o vetor de centro da esfera
   * \param lookfrom - ponteiro para o vetor de posicao da camera
   */
  void Luz::atualizar_vetores_auxiliares(Vetor* interseccao_esfera, Vetor* centro_esfera, Vetor* lookfrom){
    //Pegando valores da interseccao_esfera
    int_esf_x = interseccao_esfera->vx();
    int_esf_y = interseccao_esfera->vy();
    int_esf_z = interseccao_esfera->vz();
	  
    //Pegando valores do centro da esfera
    c_esf_x = centro_esfera->vx();
    c_esf_y = centro_esfera->vy();
    c_esf_z = centro_esfera->vz();
		
    //Pegando valores da posicao da camera
    lkf_x = lookfrom->vx();
    lkf_y = lookfrom->vy();
    lkf_z = lookfrom->vz();
  }
	
  /**
   * \fn void Luz::vetor_normal();
   *
   * \brief Encontra o vetor normal do ponto da interseccao.
   */
  void 
  Luz::vetor_normal(){
    n->valores_vetor( int_esf_x - c_esf_x, int_esf_y - c_esf_y, int_esf_z - c_esf_z );
		
    //Normalizando o vetor
    double n_norma = n->norma();
    n->valores_vetor( (int_esf_x - c_esf_x)/n_norma, (int_esf_y - c_esf_y)/n_norma, (int_esf_z - c_esf_z)/n_norma );
  }
	
  /**
   * \fn void Luz::vetor_luz();
   *
   * \brief Encontra o vetor da luz partindo do ponto de interseccao.
   */
  void 
  Luz::vetor_luz(){
    double x, y, z;
    x = pos_luz->vx();
    y = pos_luz->vy();
    z = pos_luz->vz();
		
    l->valores_vetor( x - int_esf_x, y - int_esf_y, z - int_esf_z );
		
    //Normalizando o vetor
    double l_norma = l->norma();
    l->valores_vetor( (x - int_esf_x)/l_norma, (y - int_esf_y)/l_norma , (z - int_esf_z)/l_norma );
  }
	
  /**
   * \fn void Luz::vetor_observador();
   *
   * \brief Encontra o vetor do ponto de interseccao da esfera ate o observador
   */
  void 
  Luz::vetor_observador(){
    o->valores_vetor( lkf_x - int_esf_x, lkf_y - int_esf_y, lkf_z - int_esf_z );
		
    //Normalizando o vetor
    double o_norma = o->norma();
    o->valores_vetor( (lkf_x - int_esf_x)/o_norma, (lkf_y - int_esf_y)/o_norma, (lkf_z - int_esf_z)/o_norma );
  }
	
  /**
   * \fn void Luz::vetor_reflexao();
   *
   * \brief Encontra o vetor de reflexao atraves da fórmula R = 2*N*(N.L) - L
   */
  void 
  Luz::vetor_reflexao(){
    double produto_escalar_N_L = n->produto_escalar(l);
		
    double n_x = n->vx();
    double n_y = n->vy();
    double n_z = n->vz();
		
    double l_x = l->vx();
    double l_y = l->vy();
    double l_z = l->vz();
		
    r->valores_vetor( (2*n_x*produto_escalar_N_L) - l_x, (2*n_y*produto_escalar_N_L) - l_y, (2*n_z*produto_escalar_N_L) - l_z );
		
    //Normalizando o vetor
    double r_norma = r->norma();
    r->valores_vetor( ((2*n_x*produto_escalar_N_L) - l_x)/r_norma, ((2*n_y*produto_escalar_N_L) - l_y)/r_norma, ((2*n_z*produto_escalar_N_L) - l_z)/r_norma );
  }
	
  /**
   * \fn void Luz::atualizar_constantes_phong(double _ka, double _ks, double _kd, double _Ia, double _Ilight_red, double _Ilight_green, double _Ilight_blue, double _fat, double _nshin);
   *
   * \brief Atualiza as variaveis referentes as constantes de phong
   * 
   * \param _ka - constante ambiente
   * \param _ks - constante especular
   * \param _kd - constante difusa
   * \param _Ia - intensidade do ambiente
   * \param _Ilight_red - intensidade vermelha da luz
   * \param _Ilight_green - intensidade verde da luz
   * \param _Ilight_blue - intensidade azul da luz
   * \param _fat - fator de atenuacao
   * \param _nshin - espalhamento da luz
   */
  void Luz::atualizar_constantes_phong(double _ka, double _ks, double _kd, 
				       double _Ia, double _Ilight_red, double _Ilight_green, double _Ilight_blue,
				       double _fat, double _nshin){
    ka = _ka;
    ks = _ks;
    kd = _kd;
		
    Ia = _Ia;
    Ilight_red = _Ilight_red;
    Ilight_green = _Ilight_green;
    Ilight_blue = _Ilight_blue;
		
    fat = _fat;
    nshin = _nshin;
  }
	
  /**
   * \fn double Luz::calcula_luz_red();
   *
   * \brief Calcula o valor da luz de intensidade vermelha para o ponto de interseccao atraves da fórmula de phong.
   * 
   * \return O valor da luz no ponto de interseccao.
   */
  double 
  Luz::calcula_luz_red(){
    //Calculando os vetores: normal, luz, observador e reflexao
    vetor_normal();
    vetor_luz();
    vetor_observador();
    vetor_reflexao();
		
    double produto_escalar_N_L = n->produto_escalar(l);
    double produto_escalar_O_R = o->produto_escalar(r);
		
    double potencia = pow(produto_escalar_O_R, nshin);
		
    //equacao de iluminacao
    double value = (ka * Ia) + fat * Ilight_red * ( (kd * produto_escalar_N_L) + (ks * potencia) );
		
    return value;
  }
	
  /**
   * \fn double Luz::calcula_luz_green();
   *
   * \brief Calcula o valor da luz de intensidade verde para o ponto de interseccao atraves da fórmula de phong.
   * 
   * \return O valor da luz no ponto de interseccao.
   */
  double 
  Luz::calcula_luz_green(){
    //Calculando os vetores: normal, luz, observador e reflexao
    vetor_normal();
    vetor_luz();
    vetor_observador();
    vetor_reflexao();
		
    double produto_escalar_N_L = n->produto_escalar(l);
    double produto_escalar_O_R = o->produto_escalar(r);
		
    double potencia = pow(produto_escalar_O_R, nshin);
		
    //equacao de iluminacao
    double value = (ka * Ia) + fat * Ilight_green * ( (kd * produto_escalar_N_L) + (ks * potencia) );
		
    return value;
  }
	
  /**
   * \fn double Luz::calcula_luz_blue();
   *
   * \brief Calcula o valor da luz de intensidade azul para o ponto de interseccao atraves da fórmula de phong.
   * 
   * \return O valor da luz no ponto de interseccao.
   */
  double 
  Luz::calcula_luz_blue(){
    //Calculando os vetores: normal, luz, observador e reflexao
    vetor_normal();
    vetor_luz();
    vetor_observador();
    vetor_reflexao();
		
    double produto_escalar_N_L = n->produto_escalar(l);
    double produto_escalar_O_R = o->produto_escalar(r);
		
    double potencia = pow(produto_escalar_O_R, nshin);
		
    //equacao de iluminacao
    double value = (ka * Ia) + fat * Ilight_blue * ( (kd * produto_escalar_N_L) + (ks * potencia) );
		
    return value;
  }
	
} //Fim do namespace rayTracing
 
/** @} */ //Fim do grupo class